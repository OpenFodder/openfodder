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
 */

#include "stdafx.hpp"
#include "HubAuth.hpp"

#ifdef OPENFODDER_ENABLE_NETWORK

#include "NetworkHubClient.hpp"

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <curl/curl.h>

#ifdef WIN32
#  include <windows.h>
#  include <shellapi.h>
#else
#  include <sys/stat.h>
#  include <sys/types.h>
#  include <unistd.h>
#endif

#ifdef OPENFODDER_HAVE_SODIUM
#  include <sodium.h>
#endif

// HTTPS base for the auth flow. The UDP relay hub lives on the same
// hostname (port 27770); the auth subsystem rides 443/TLS so it survives
// captive-portal proxies and corporate egress filters that drop UDP. A
// future build flag can override this — for now hard-coded; the hub host
// override in Parameters.cpp is for the relay, not the OAuth box.
static const char* const HUB_AUTH_BASE_URL = "https://hub.openfodder.com";

// ---------------------------------------------------------------------------
// curl write callback: append response body to a std::string.
// ---------------------------------------------------------------------------
static size_t HubAuth_WriteToString(char* pData, size_t pSize, size_t pNmemb, void* pUser) {
    const size_t Bytes = pSize * pNmemb;
    auto* Body = static_cast<std::string*>(pUser);
    Body->append(pData, Bytes);
    return Bytes;
}

// ---------------------------------------------------------------------------
// Tiny JSON helpers. The auth endpoints return small, well-known objects:
// {"jwt":"...","expiry":1234,"subject":"discord:..."} — full nlohmann/json
// would be overkill, especially as miniz/sodium/SDL3/duktape are already
// bloating the dep tree. We do exact-match string lookups, then unescape
// the standard six \-escapes plus \u00xx; anything else is a parse error
// (the hub never emits unicode in these fields).
// ---------------------------------------------------------------------------
static int HubAuth_HexNibble(char pCh) {
    if (pCh >= '0' && pCh <= '9') return pCh - '0';
    if (pCh >= 'A' && pCh <= 'F') return 10 + (pCh - 'A');
    if (pCh >= 'a' && pCh <= 'f') return 10 + (pCh - 'a');
    return -1;
}

static bool HubAuth_DecodeJsonString(const std::string& pSource, size_t pBegin, size_t pEnd, std::string& pOut) {
    pOut.clear();
    pOut.reserve(pEnd - pBegin);
    for (size_t Index = pBegin; Index < pEnd; ++Index) {
        char Ch = pSource[Index];
        if (Ch != '\\') {
            pOut.push_back(Ch);
            continue;
        }
        if (Index + 1 >= pEnd) return false;
        const char Esc = pSource[++Index];
        switch (Esc) {
            case '"': pOut.push_back('"'); break;
            case '\\': pOut.push_back('\\'); break;
            case '/': pOut.push_back('/'); break;
            case 'b': pOut.push_back('\b'); break;
            case 'f': pOut.push_back('\f'); break;
            case 'n': pOut.push_back('\n'); break;
            case 'r': pOut.push_back('\r'); break;
            case 't': pOut.push_back('\t'); break;
            case 'u': {
                if (Index + 4 >= pEnd) return false;
                int H1 = HubAuth_HexNibble(pSource[Index + 1]);
                int H2 = HubAuth_HexNibble(pSource[Index + 2]);
                int H3 = HubAuth_HexNibble(pSource[Index + 3]);
                int H4 = HubAuth_HexNibble(pSource[Index + 4]);
                if (H1 < 0 || H2 < 0 || H3 < 0 || H4 < 0) return false;
                const unsigned CodePoint = (H1 << 12) | (H2 << 8) | (H3 << 4) | H4;
                // ASCII-only fast path; surrogate pairs / BMP-above-127
                // never appear in our endpoint payloads.
                if (CodePoint < 0x80) {
                    pOut.push_back((char)CodePoint);
                }
                else if (CodePoint < 0x800) {
                    pOut.push_back((char)(0xC0 | (CodePoint >> 6)));
                    pOut.push_back((char)(0x80 | (CodePoint & 0x3F)));
                }
                else {
                    pOut.push_back((char)(0xE0 | (CodePoint >> 12)));
                    pOut.push_back((char)(0x80 | ((CodePoint >> 6) & 0x3F)));
                    pOut.push_back((char)(0x80 | (CodePoint & 0x3F)));
                }
                Index += 4;
                break;
            }
            default:
                return false;
        }
    }
    return true;
}

// Find "key" in pJson and return its value start/end span. Span semantics:
//   - For string values: indices into the unescaped raw bytes between the
//     surrounding double quotes (pBegin..pEnd, exclusive of the quotes).
//   - For numeric values: indices spanning the digits / sign / dot.
// pIsString reports which kind was matched; the caller dispatches.
static bool HubAuth_FindKey(const std::string& pJson, const std::string& pKey,
                            size_t& pBegin, size_t& pEnd, bool& pIsString) {
    const std::string Needle = std::string("\"") + pKey + "\"";
    size_t Cursor = 0;
    while (true) {
        const size_t KeyAt = pJson.find(Needle, Cursor);
        if (KeyAt == std::string::npos) return false;

        size_t After = KeyAt + Needle.size();
        // Skip whitespace then ':' then whitespace.
        while (After < pJson.size() && (pJson[After] == ' ' || pJson[After] == '\t' || pJson[After] == '\r' || pJson[After] == '\n'))
            ++After;
        if (After >= pJson.size() || pJson[After] != ':') {
            // Not really our key (substring inside another value); keep looking.
            Cursor = After;
            continue;
        }
        ++After;
        while (After < pJson.size() && (pJson[After] == ' ' || pJson[After] == '\t' || pJson[After] == '\r' || pJson[After] == '\n'))
            ++After;
        if (After >= pJson.size()) return false;

        if (pJson[After] == '"') {
            // String value — walk to the matching close quote, skipping escapes.
            const size_t Start = After + 1;
            size_t End = Start;
            while (End < pJson.size()) {
                if (pJson[End] == '\\' && End + 1 < pJson.size()) {
                    End += 2;
                    continue;
                }
                if (pJson[End] == '"') break;
                ++End;
            }
            if (End >= pJson.size()) return false;
            pBegin = Start;
            pEnd   = End;
            pIsString = true;
            return true;
        }

        // Numeric / true / false / null — span until terminator.
        const size_t Start = After;
        size_t End = Start;
        while (End < pJson.size()) {
            const char Ch = pJson[End];
            if (Ch == ',' || Ch == '}' || Ch == ']' || Ch == ' ' || Ch == '\t' || Ch == '\r' || Ch == '\n')
                break;
            ++End;
        }
        if (End == Start) return false;
        pBegin = Start;
        pEnd   = End;
        pIsString = false;
        return true;
    }
}

static bool HubAuth_GetJsonString(const std::string& pJson, const std::string& pKey, std::string& pOut) {
    size_t Begin = 0, End = 0;
    bool IsString = false;
    if (!HubAuth_FindKey(pJson, pKey, Begin, End, IsString) || !IsString)
        return false;
    return HubAuth_DecodeJsonString(pJson, Begin, End, pOut);
}

static bool HubAuth_GetJsonInt64(const std::string& pJson, const std::string& pKey, int64_t& pOut) {
    size_t Begin = 0, End = 0;
    bool IsString = false;
    if (!HubAuth_FindKey(pJson, pKey, Begin, End, IsString) || IsString)
        return false;
    try {
        pOut = std::stoll(pJson.substr(Begin, End - Begin));
        return true;
    }
    catch (...) {
        return false;
    }
}

// Minified-JSON builder for outbound POST bodies. Just two-three string
// fields; avoids pulling in an encoder dep.
static std::string HubAuth_EncodeJsonString(const std::string& pValue) {
    std::string Out;
    Out.reserve(pValue.size() + 2);
    Out.push_back('"');
    for (char Ch : pValue) {
        switch (Ch) {
            case '"':  Out += "\\\""; break;
            case '\\': Out += "\\\\"; break;
            case '\b': Out += "\\b";  break;
            case '\f': Out += "\\f";  break;
            case '\n': Out += "\\n";  break;
            case '\r': Out += "\\r";  break;
            case '\t': Out += "\\t";  break;
            default:
                if ((unsigned char)Ch < 0x20) {
                    char Buf[8];
                    std::snprintf(Buf, sizeof(Buf), "\\u%04x", (unsigned)(unsigned char)Ch);
                    Out += Buf;
                }
                else {
                    Out.push_back(Ch);
                }
                break;
        }
    }
    Out.push_back('"');
    return Out;
}

// ---------------------------------------------------------------------------
// Storage path resolution. Mirrors the rules in Setup::InstallPaths but the
// auth file lives in a different root from the Data/Scripts payload — it's
// a per-user secret, not a shared install asset.
// ---------------------------------------------------------------------------
static bool HubAuth_TryGetEnv(const char* pName, std::string& pOut) {
#ifdef WIN32
    char Buffer[1024];
    size_t Needed = 0;
    if (getenv_s(&Needed, Buffer, sizeof(Buffer), pName) != 0 || Needed == 0)
        return false;
    pOut.assign(Buffer, Needed - 1);
    return !pOut.empty();
#else
    const char* Value = std::getenv(pName);
    if (!Value || !*Value) return false;
    pOut = Value;
    return !pOut.empty();
#endif
}

static std::filesystem::path HubAuth_StorageDir() {
#ifdef WIN32
    std::string Local;
    if (HubAuth_TryGetEnv("LOCALAPPDATA", Local))
        return std::filesystem::path(Local) / "openfodder";
    // Fall back to %USERPROFILE%\AppData\Local\openfodder if the env var
    // somehow vanished (RDP, stripped envs); the path is still per-user.
    std::string Profile;
    if (HubAuth_TryGetEnv("USERPROFILE", Profile))
        return std::filesystem::path(Profile) / "AppData" / "Local" / "openfodder";
    return {};
#elif defined(__APPLE__)
    std::string Home;
    if (HubAuth_TryGetEnv("HOME", Home))
        return std::filesystem::path(Home) / "Library" / "Application Support" / "openfodder";
    return {};
#else
    std::string Xdg;
    if (HubAuth_TryGetEnv("XDG_DATA_HOME", Xdg))
        return std::filesystem::path(Xdg) / "openfodder";
    std::string Home;
    if (HubAuth_TryGetEnv("HOME", Home))
        return std::filesystem::path(Home) / ".local" / "share" / "openfodder";
    return {};
#endif
}

std::string cHubAuth::TokenStoragePath() {
    auto Dir = HubAuth_StorageDir();
    if (Dir.empty()) return {};
    return (Dir / "hub_auth.json").generic_string();
}

// ---------------------------------------------------------------------------
// Browser open. Best-effort fire-and-forget — failure here doesn't make the
// pairing flow useless because the user can copy the URL out of the log.
// ---------------------------------------------------------------------------
static bool HubAuth_OpenBrowser(const std::string& pUrl) {
#ifdef WIN32
    // ShellExecuteW expects a wide URL. URLs are ASCII-clean here (we built
    // them ourselves), so a naive widen is safe.
    std::wstring Wide;
    Wide.reserve(pUrl.size());
    for (char Ch : pUrl)
        Wide.push_back((wchar_t)(unsigned char)Ch);
    HINSTANCE Result = ShellExecuteW(nullptr, L"open", Wide.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    // ShellExecute returns >32 on success.
    return (INT_PTR)Result > 32;
#elif defined(__APPLE__)
    const std::string Cmd = "open " + pUrl + " >/dev/null 2>&1 &";
    return std::system(Cmd.c_str()) == 0;
#else
    const std::string Cmd = "xdg-open " + pUrl + " >/dev/null 2>&1 &";
    return std::system(Cmd.c_str()) == 0;
#endif
}

// ---------------------------------------------------------------------------
// Random device code. 12 chars from the 62-char URL-safe alphabet. Uses
// libsodium's randombytes_buf when available (CSPRNG, same call path the
// rest of the network stack already requires). If sodium is absent at build
// time we fail loudly: a guessable device code defeats the whole point of
// the pairing handshake.
// ---------------------------------------------------------------------------
static bool HubAuth_GenerateDeviceCode(std::string& pOut, std::string& pError) {
#ifdef OPENFODDER_HAVE_SODIUM
    static const char Alphabet[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    constexpr size_t Length = 12;

    unsigned char Random[Length];
    randombytes_buf(Random, sizeof(Random));

    pOut.clear();
    pOut.reserve(Length);
    for (size_t Index = 0; Index < Length; ++Index)
        pOut.push_back(Alphabet[Random[Index] % 62]);
    return true;
#else
    (void)pOut;
    pError = "device code generation requires libsodium";
    return false;
#endif
}

// ---------------------------------------------------------------------------
// Single curl easy-handle POST/GET. Handles are short-lived (one per call) —
// auth traffic is rare enough that the savings of pooling don't justify the
// state machine HttpClient uses for the big update fetcher.
// ---------------------------------------------------------------------------
namespace {

struct HubAuthHttpResult {
    long        mStatus = 0;
    std::string mBody;
};

bool HubAuth_Perform(const std::string& pUrl,
                     const std::string& pPostBody,    // empty == GET
                     HubAuthHttpResult& pResult,
                     std::string&       pError) {
    pResult = HubAuthHttpResult{};

    CURL* Curl = curl_easy_init();
    if (!Curl) {
        pError = "curl_easy_init failed";
        return false;
    }

    struct curl_slist* Headers = nullptr;
    Headers = curl_slist_append(Headers, "Content-Type: application/json");
    Headers = curl_slist_append(Headers, "Accept: application/json");

    curl_easy_setopt(Curl, CURLOPT_URL,             pUrl.c_str());
    curl_easy_setopt(Curl, CURLOPT_FOLLOWLOCATION,  1L);
    curl_easy_setopt(Curl, CURLOPT_MAXREDIRS,       5L);
    curl_easy_setopt(Curl, CURLOPT_CONNECTTIMEOUT,  5L);
    curl_easy_setopt(Curl, CURLOPT_TIMEOUT,         10L);
    curl_easy_setopt(Curl, CURLOPT_SSL_VERIFYPEER,  1L);
    curl_easy_setopt(Curl, CURLOPT_SSL_VERIFYHOST,  2L);
    curl_easy_setopt(Curl, CURLOPT_NOSIGNAL,        1L);
    curl_easy_setopt(Curl, CURLOPT_HTTPHEADER,      Headers);
    curl_easy_setopt(Curl, CURLOPT_WRITEFUNCTION,   &HubAuth_WriteToString);
    curl_easy_setopt(Curl, CURLOPT_WRITEDATA,       &pResult.mBody);

    if (!pPostBody.empty()) {
        curl_easy_setopt(Curl, CURLOPT_POST,           1L);
        curl_easy_setopt(Curl, CURLOPT_POSTFIELDS,     pPostBody.c_str());
        curl_easy_setopt(Curl, CURLOPT_POSTFIELDSIZE,  (long)pPostBody.size());
    }

    const CURLcode Rc = curl_easy_perform(Curl);
    curl_easy_getinfo(Curl, CURLINFO_RESPONSE_CODE, &pResult.mStatus);

    curl_slist_free_all(Headers);
    curl_easy_cleanup(Curl);

    if (Rc != CURLE_OK) {
        std::ostringstream Os;
        Os << "curl error " << (int)Rc << ": " << curl_easy_strerror(Rc);
        pError = Os.str();
        return false;
    }
    return true;
}

bool HubAuth_ParseTokenJson(const std::string& pJson, sHubAuthToken& pOut, std::string& pError) {
    sHubAuthToken Parsed;
    if (!HubAuth_GetJsonString(pJson, "jwt", Parsed.mJwt) || Parsed.mJwt.empty()) {
        pError = "auth response missing jwt";
        return false;
    }
    if (!HubAuth_GetJsonInt64(pJson, "expiry", Parsed.mExpiry) || Parsed.mExpiry <= 0) {
        pError = "auth response missing expiry";
        return false;
    }
    // subject is optional on refresh — server is allowed to omit it if it
    // hasn't changed.
    HubAuth_GetJsonString(pJson, "subject", Parsed.mSubject);
    pOut = std::move(Parsed);
    return true;
}

} // anonymous namespace

// ---------------------------------------------------------------------------
// Lifecycle. curl_global_init is reference-counted by libcurl and called
// from every entry point that uses it (HttpClient::GlobalInit too). Calling
// it again here is cheap and safe.
// ---------------------------------------------------------------------------
cHubAuth::cHubAuth() {
    curl_global_init(CURL_GLOBAL_DEFAULT);

#ifdef OPENFODDER_HAVE_SODIUM
    if (sodium_init() < 0)
        mLastError = "sodium_init failed";
#endif
}

cHubAuth::~cHubAuth() {
    curl_global_cleanup();
}

// ---------------------------------------------------------------------------
// LoadCachedToken — reads the on-disk JSON, treats expired-or-malformed as
// "no token" (not an error). The mLastError member is left untouched so a
// caller chaining LoadCachedToken → BeginPairing only sees errors from the
// step that actually failed.
// ---------------------------------------------------------------------------
bool cHubAuth::LoadCachedToken(sHubAuthToken& pOut) const {
    const std::string Path = TokenStoragePath();
    if (Path.empty())
        return false;

    std::ifstream File(Path, std::ios::binary);
    if (!File.is_open())
        return false;

    std::ostringstream Buffer;
    Buffer << File.rdbuf();
    const std::string Json = Buffer.str();
    if (Json.empty())
        return false;

    sHubAuthToken Parsed;
    std::string Ignored;
    if (!HubAuth_ParseTokenJson(Json, Parsed, Ignored))
        return false;

    const auto Now = std::chrono::system_clock::now().time_since_epoch();
    const int64_t NowSec = std::chrono::duration_cast<std::chrono::seconds>(Now).count();
    if (Parsed.mExpiry <= NowSec)
        return false; // expired — caller should pair / refresh

    pOut = std::move(Parsed);
    return true;
}

// ---------------------------------------------------------------------------
// SaveToken — atomic write. Writes to <path>.tmp, fsyncs the parent dir on
// POSIX, then renames into place. chmod 0600 happens BEFORE the rename so
// the final file never exists on disk world-readable, even briefly.
// ---------------------------------------------------------------------------
bool cHubAuth::SaveToken(const sHubAuthToken& pToken) {
    mLastError.clear();

    const std::string Path = TokenStoragePath();
    if (Path.empty()) {
        mLastError = "no per-user data directory";
        return false;
    }

    std::error_code Ec;
    auto Parent = std::filesystem::path(Path).parent_path();
    std::filesystem::create_directories(Parent, Ec);
    if (Ec) {
        mLastError = "create_directories failed: " + Ec.message();
        return false;
    }

    const std::string TempPath = Path + ".tmp";
    {
        std::ofstream Out(TempPath, std::ios::binary | std::ios::trunc);
        if (!Out) {
            mLastError = "open temp file failed: " + TempPath;
            return false;
        }

        // Minified JSON: {"jwt":"...","expiry":1234,"subject":"..."}
        Out << '{'
            << "\"jwt\":"     << HubAuth_EncodeJsonString(pToken.mJwt) << ','
            << "\"expiry\":"  << pToken.mExpiry << ','
            << "\"subject\":" << HubAuth_EncodeJsonString(pToken.mSubject)
            << '}';
        Out.flush();
        if (!Out.good()) {
            mLastError = "write to temp file failed";
            std::filesystem::remove(TempPath, Ec);
            return false;
        }
    }

#ifndef WIN32
    // 0600 — owner read/write only. Before the rename, so a malicious
    // observer racing the rename never sees a world-readable inode.
    if (chmod(TempPath.c_str(), S_IRUSR | S_IWUSR) != 0) {
        mLastError = "chmod 0600 failed";
        std::filesystem::remove(TempPath, Ec);
        return false;
    }
#endif

    std::filesystem::rename(TempPath, Path, Ec);
    if (Ec) {
        // On Windows rename across an existing file fails; fall back to
        // copy+remove. std::filesystem::rename on Windows IS supposed to
        // overwrite as of C++20, but earlier toolchains don't, and the
        // engine still supports msvc++17 in some build configs.
        std::filesystem::copy_file(TempPath, Path,
                                   std::filesystem::copy_options::overwrite_existing, Ec);
        std::filesystem::remove(TempPath, Ec);
        if (Ec) {
            mLastError = "rename token file failed: " + Ec.message();
            return false;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// BeginPairing — generate the device code, fire the browser. The device
// code lives only in memory after this; the user is expected to invoke
// ClaimToken with the matching pair code from the web flow.
// ---------------------------------------------------------------------------
bool cHubAuth::BeginPairing(std::string& pDeviceCodeOut) {
    mLastError.clear();
    pDeviceCodeOut.clear();

    if (!HubAuth_GenerateDeviceCode(pDeviceCodeOut, mLastError))
        return false;

    const std::string Url = std::string(HUB_AUTH_BASE_URL)
                          + "/auth/start?device="
                          + cNetworkHubClient::PercentEncode(pDeviceCodeOut);

    if (!HubAuth_OpenBrowser(Url)) {
        // Don't fail outright — the caller can still surface the URL to
        // the user. But do report the soft failure so the UI can prompt
        // "couldn't open your browser, please visit:" instead of pretending
        // everything's fine.
        mLastError = "could not launch browser; visit " + Url + " manually";
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------------
// ClaimToken — POST {pair_code, device_code} JSON. The server returns 404
// while the user is still typing/clicking through Discord; we retry up to
// 20 times with 250 ms pauses (≈5 s of polling). Anything other than 404
// terminates the loop immediately — including 200 (success) and 400/410
// (invalid / expired pair code).
// ---------------------------------------------------------------------------
bool cHubAuth::ClaimToken(const std::string& pPairCode,
                          const std::string& pDeviceCode,
                          sHubAuthToken&     pOut) {
    mLastError.clear();
    pOut = sHubAuthToken{};

    if (pPairCode.empty() || pDeviceCode.empty()) {
        mLastError = "pair code and device code are required";
        return false;
    }

    const std::string Url  = std::string(HUB_AUTH_BASE_URL) + "/auth/claim";
    const std::string Body = std::string("{")
                           + "\"pair_code\":"   + HubAuth_EncodeJsonString(pPairCode)   + ","
                           + "\"device_code\":" + HubAuth_EncodeJsonString(pDeviceCode)
                           + "}";

    constexpr int  MaxAttempts  = 20;
    constexpr auto BackoffSleep = std::chrono::milliseconds(250);

    for (int Attempt = 0; Attempt < MaxAttempts; ++Attempt) {
        HubAuthHttpResult Result;
        std::string Error;
        if (!HubAuth_Perform(Url, Body, Result, Error)) {
            mLastError = Error;
            return false;
        }

        if (Result.mStatus == 200) {
            if (!HubAuth_ParseTokenJson(Result.mBody, pOut, mLastError))
                return false;
            return true;
        }
        if (Result.mStatus == 404) {
            // User hasn't completed the web side yet — back off and retry.
            std::this_thread::sleep_for(BackoffSleep);
            continue;
        }

        // Any other status is terminal. Surface the body if it parses, the
        // raw HTTP code if not.
        std::string ServerError;
        if (HubAuth_GetJsonString(Result.mBody, "error", ServerError) && !ServerError.empty())
            mLastError = "auth/claim: HTTP " + std::to_string(Result.mStatus) + " — " + ServerError;
        else
            mLastError = "auth/claim: HTTP " + std::to_string(Result.mStatus);
        return false;
    }

    mLastError = "auth/claim timed out waiting for pair code to be bound";
    return false;
}

// ---------------------------------------------------------------------------
// Refresh — POST {jwt} to /auth/refresh, replace pInOut on 200. The server
// is responsible for deciding whether the existing JWT is renewable.
// ---------------------------------------------------------------------------
bool cHubAuth::Refresh(sHubAuthToken& pInOut) {
    mLastError.clear();

    if (pInOut.mJwt.empty()) {
        mLastError = "no jwt to refresh";
        return false;
    }

    const std::string Url  = std::string(HUB_AUTH_BASE_URL) + "/auth/refresh";
    const std::string Body = std::string("{\"jwt\":")
                           + HubAuth_EncodeJsonString(pInOut.mJwt)
                           + "}";

    HubAuthHttpResult Result;
    std::string Error;
    if (!HubAuth_Perform(Url, Body, Result, Error)) {
        mLastError = Error;
        return false;
    }

    if (Result.mStatus != 200) {
        std::string ServerError;
        if (HubAuth_GetJsonString(Result.mBody, "error", ServerError) && !ServerError.empty())
            mLastError = "auth/refresh: HTTP " + std::to_string(Result.mStatus) + " — " + ServerError;
        else
            mLastError = "auth/refresh: HTTP " + std::to_string(Result.mStatus);
        return false;
    }

    sHubAuthToken Refreshed;
    if (!HubAuth_ParseTokenJson(Result.mBody, Refreshed, mLastError))
        return false;

    // Preserve the old subject if the server didn't echo a fresh one.
    if (Refreshed.mSubject.empty())
        Refreshed.mSubject = pInOut.mSubject;

    pInOut = std::move(Refreshed);
    return true;
}

#endif // OPENFODDER_ENABLE_NETWORK
