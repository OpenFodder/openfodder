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

#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <vector>

namespace Setup {

// HTTP(S) client used by the setup wizard / updater to fetch retail-data
// manifests, scripts, and engine update archives from GitHub. Built on
// libcurl (Schannel on Windows, system OpenSSL elsewhere — see CMakeLists).
//
// Design notes:
// - libcurl is intentionally NOT included from this header; <curl/curl.h>
//   pulls in <windows.h> on MSVC which we don't want bleeding into the
//   rest of the engine. The CURL handle is stored as a void* and only
//   reified inside HttpDownload.cpp.
// - HttpClient owns one easy handle and is single-threaded by design.
//   Callers wanting concurrency should construct one client per thread.
// - Errors never throw. Methods return false and stash a human-readable
//   message in LastError() (curl_easy_strerror + HTTP status if relevant).

struct HttpResponse {
    long                                mStatus = 0;        // HTTP response code (e.g. 200)
    std::vector<uint8_t>                mBody;              // Response body (Get only)
    std::map<std::string, std::string>  mHeaders;           // Response headers (lower-cased key -> value)
};

// Progress hook fired by libcurl as bytes arrive. Return false to abort the
// transfer (libcurl will fail the call with CURLE_ABORTED_BY_CALLBACK and
// HttpClient::Get/Download will return false). pBytesTotal is 0 when the
// server doesn't advertise Content-Length.
using ProgressCallback = std::function<bool(uint64_t pBytesDone, uint64_t pBytesTotal)>;

class HttpClient {
public:
    HttpClient();   // No-op; CURL handle is created lazily on first Get/Download.
    ~HttpClient();

    HttpClient(const HttpClient&) = delete;
    HttpClient& operator=(const HttpClient&) = delete;

    // Fetch pUrl into pOut.mBody (in-memory). Returns true iff the request
    // completed AND the HTTP status is 2xx. mStatus / mHeaders are populated
    // on both success and failure (when the server responded).
    bool Get(const std::string& pUrl, HttpResponse& pOut, ProgressCallback pProgress = {});

    // Fetch pUrl directly to pOutPath, streaming. The file is opened binary
    // and truncated. On failure, the (possibly partial) file is left on disk
    // for the caller to clean up — we don't second-guess where it should go.
    bool Download(const std::string& pUrl, const std::string& pOutPath, ProgressCallback pProgress = {});

    const std::string& LastError() const { return mLastError; }

    // Idempotent — safe to call multiple times. Match with GlobalCleanup at
    // shutdown if you want a clean valgrind, but libcurl tolerates not being
    // cleaned up.
    static void GlobalInit();
    static void GlobalCleanup();

private:
    void*       mCurl = nullptr;        // CURL* (opaque here)
    std::string mLastError;

    void EnsureCurl();
    void ApplyDefaults();   // UA, timeouts, follow-redirects, ssl-verify, no-signal
};

} // namespace Setup
