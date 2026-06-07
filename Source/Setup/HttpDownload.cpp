/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2026 Open Fodder
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "stdafx.hpp"
#include "Setup/HttpDownload.hpp"
#include "Setup/EngineVersion.hpp"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <mutex>
#include <sstream>
#include <string>

#include <curl/curl.h>

namespace Setup {

namespace {

// One-time global init. curl_global_init must run before any easy-handle
// usage and is not safe to call concurrently with itself, so we serialize
// it through std::call_once.
std::once_flag gCurlGlobalInitOnce;
bool           gCurlGlobalInitOk = false;

void DoGlobalInit() {
    const CURLcode rc = curl_global_init(CURL_GLOBAL_DEFAULT);
    gCurlGlobalInitOk = (rc == CURLE_OK);
}

// libcurl strips trailing whitespace from header values for us, but folds
// the case of header *names* however the server sent them. Lower-case for
// stable lookup.
std::string LowerCase(std::string pStr) {
    std::transform(pStr.begin(), pStr.end(), pStr.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return pStr;
}

// ---- write callbacks -------------------------------------------------------

size_t WriteToVector(char* pData, size_t pSize, size_t pNmemb, void* pUser) {
    const size_t bytes = pSize * pNmemb;
    auto* body = static_cast<std::vector<uint8_t>*>(pUser);
    const auto* begin = reinterpret_cast<const uint8_t*>(pData);
    body->insert(body->end(), begin, begin + bytes);
    return bytes;
}

size_t WriteToOfstream(char* pData, size_t pSize, size_t pNmemb, void* pUser) {
    const size_t bytes = pSize * pNmemb;
    auto* file = static_cast<std::ofstream*>(pUser);
    file->write(pData, static_cast<std::streamsize>(bytes));
    if (!file->good())
        return 0;   // signal write error to libcurl
    return bytes;
}

size_t HeaderCallback(char* pData, size_t pSize, size_t pNmemb, void* pUser) {
    const size_t bytes = pSize * pNmemb;
    auto* headers = static_cast<std::map<std::string, std::string>*>(pUser);

    // Header line includes the trailing CRLF; status line ("HTTP/1.1 200 OK")
    // has no colon and is silently ignored.
    std::string line(pData, bytes);
    while (!line.empty() && (line.back() == '\r' || line.back() == '\n'))
        line.pop_back();

    const auto colon = line.find(':');
    if (colon == std::string::npos)
        return bytes;

    std::string name  = LowerCase(line.substr(0, colon));
    std::string value = line.substr(colon + 1);

    // Trim leading whitespace from value.
    const auto first = value.find_first_not_of(" \t");
    if (first == std::string::npos)
        value.clear();
    else
        value.erase(0, first);

    (*headers)[std::move(name)] = std::move(value);
    return bytes;
}

// XFERINFOFUNCTION (curl >= 7.32). Returning non-zero aborts the transfer
// (CURLE_ABORTED_BY_CALLBACK).
struct ProgressContext {
    ProgressCallback* mCallback;
};

int ProgressTrampoline(void* pUser,
                       curl_off_t pDlTotal, curl_off_t pDlNow,
                       curl_off_t /*pUlTotal*/, curl_off_t /*pUlNow*/) {
    auto* ctx = static_cast<ProgressContext*>(pUser);
    if (!ctx || !ctx->mCallback || !*ctx->mCallback)
        return 0;
    const bool keepGoing = (*ctx->mCallback)(static_cast<uint64_t>(pDlNow < 0 ? 0 : pDlNow),
                                             static_cast<uint64_t>(pDlTotal < 0 ? 0 : pDlTotal));
    return keepGoing ? 0 : 1;
}

} // namespace

void HttpClient::GlobalInit() {
    std::call_once(gCurlGlobalInitOnce, &DoGlobalInit);
}

void HttpClient::GlobalCleanup() {
    // Only cleanup if init actually succeeded. Multiple cleanups are
    // harmless per libcurl docs but pointless.
    if (gCurlGlobalInitOk) {
        curl_global_cleanup();
        gCurlGlobalInitOk = false;
    }
}

HttpClient::HttpClient() = default;

HttpClient::~HttpClient() {
    if (mCurl) {
        curl_easy_cleanup(static_cast<CURL*>(mCurl));
        mCurl = nullptr;
    }
}

void HttpClient::EnsureCurl() {
    GlobalInit();
    if (!mCurl) {
        mCurl = curl_easy_init();
        if (mCurl)
            ApplyDefaults();
    }
}

void HttpClient::ApplyDefaults() {
    auto* curl = static_cast<CURL*>(mCurl);

    static const std::string sUserAgent =
        std::string("OpenFodder/") + Setup::kEngineVersionString;

    curl_easy_setopt(curl, CURLOPT_USERAGENT,         sUserAgent.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,    1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS,         5L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT,    10L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT,           300L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER,    1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST,    2L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL,          1L);
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING,   "");   // accept whatever the build supports (gzip/deflate)
}

bool HttpClient::Get(const std::string& pUrl, HttpResponse& pOut, ProgressCallback pProgress) {
    mLastError.clear();
    pOut = HttpResponse{};

    EnsureCurl();
    if (!mCurl) {
        mLastError = "curl_easy_init failed";
        return false;
    }

    auto* curl = static_cast<CURL*>(mCurl);
    curl_easy_reset(curl);
    ApplyDefaults();

    curl_easy_setopt(curl, CURLOPT_URL,             pUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET,         1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,   &WriteToVector);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,       &pOut.mBody);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION,  &HeaderCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA,      &pOut.mHeaders);

    ProgressContext ctx{ &pProgress };
    if (pProgress) {
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS,        0L);
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION,  &ProgressTrampoline);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA,      &ctx);
    } else {
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS,        1L);
    }

    const CURLcode rc = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &pOut.mStatus);

    if (rc != CURLE_OK) {
        std::ostringstream os;
        os << "curl error " << static_cast<int>(rc) << ": " << curl_easy_strerror(rc);
        if (pOut.mStatus > 0)
            os << " (HTTP " << pOut.mStatus << ")";
        mLastError = os.str();
        return false;
    }

    if (pOut.mStatus < 200 || pOut.mStatus >= 300) {
        std::ostringstream os;
        os << "HTTP " << pOut.mStatus;
        mLastError = os.str();
        return false;
    }

    return true;
}

bool HttpClient::Download(const std::string& pUrl, const std::string& pOutPath, ProgressCallback pProgress) {
    mLastError.clear();

    EnsureCurl();
    if (!mCurl) {
        mLastError = "curl_easy_init failed";
        return false;
    }

    std::ofstream file(pOutPath, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        mLastError = "failed to open output file: " + pOutPath;
        return false;
    }

    auto* curl = static_cast<CURL*>(mCurl);
    curl_easy_reset(curl);
    ApplyDefaults();

    std::map<std::string, std::string> headers;
    long status = 0;

    curl_easy_setopt(curl, CURLOPT_URL,             pUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET,         1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,   &WriteToOfstream);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,       &file);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION,  &HeaderCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA,      &headers);

    ProgressContext ctx{ &pProgress };
    if (pProgress) {
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS,        0L);
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION,  &ProgressTrampoline);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA,      &ctx);
    } else {
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS,        1L);
    }

    const CURLcode rc = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);

    file.flush();
    const bool fileOk = file.good();
    file.close();

    if (rc != CURLE_OK) {
        std::ostringstream os;
        os << "curl error " << static_cast<int>(rc) << ": " << curl_easy_strerror(rc);
        if (status > 0)
            os << " (HTTP " << status << ")";
        mLastError = os.str();
        return false;
    }

    if (!fileOk) {
        mLastError = "failed writing to output file: " + pOutPath;
        return false;
    }

    if (status < 200 || status >= 300) {
        std::ostringstream os;
        os << "HTTP " << status;
        mLastError = os.str();
        return false;
    }

    return true;
}

} // namespace Setup
