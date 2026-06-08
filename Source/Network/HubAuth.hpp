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

#pragma once

#ifdef OPENFODDER_ENABLE_NETWORK

#include <cstdint>
#include <string>

// OAuth pairing flow for the OpenFodder hub.
//
// The hub mints short-lived JWTs after a user logs in via Discord on the
// web companion. The desktop game never sees the OAuth dance directly —
// it generates a random device code, opens the browser to a URL that
// contains the code, and then polls /auth/claim with a short user-typed
// pair code until the web side has finished the Discord flow and bound
// the pair code to the device code.
//
// Token cache is per-user; on disk it is a single minified JSON object,
// chmod 0600 on POSIX. JWT verification lives in cNetworkHubClient (uses
// libsodium Ed25519); this class only handles acquisition + storage.

struct sHubAuthToken {
    std::string mJwt;
    int64_t     mExpiry  = 0;       // unix seconds, server-issued exp
    std::string mSubject;           // e.g. "discord:1234567890"
};

class cHubAuth {
public:
    cHubAuth();
    ~cHubAuth();

    cHubAuth(const cHubAuth&) = delete;
    cHubAuth& operator=(const cHubAuth&) = delete;

    // Read the cached token from disk. Returns false if missing, malformed,
    // or expired (an expired token is treated as absent so callers fall
    // through to BeginPairing/Refresh without extra ceremony).
    bool LoadCachedToken(sHubAuthToken& pOut) const;

    // Persist pToken atomically (write-temp + rename) and chmod 0600 on
    // POSIX. Creates parent directories as needed. Returns false on any I/O
    // or permission error; mLastError carries detail.
    bool SaveToken(const sHubAuthToken& pToken);

    // Generate a fresh device code (12 chars, [A-Za-z0-9]), open the user's
    // browser to ${hub}/auth/start?device=..., and return the device code
    // through pDeviceCodeOut so the caller can pass it to ClaimToken once
    // the user has read the short pair code printed on the web page.
    bool BeginPairing(std::string& pDeviceCodeOut);

    // POST {pair_code, device_code} JSON to ${hub}/auth/claim. Retries up
    // to 20 times with 250 ms backoff while the server returns 404 (user
    // hasn't finished the Discord step yet). Anything else (200, 4xx, 5xx,
    // network) terminates the loop.
    bool ClaimToken(const std::string& pPairCode,
                    const std::string& pDeviceCode,
                    sHubAuthToken&     pOut);

    // POST the current jwt to ${hub}/auth/refresh and replace pInOut on
    // success. Caller is expected to SaveToken on success.
    bool Refresh(sHubAuthToken& pInOut);

    const std::string& GetLastError() const { return mLastError; }

    // Per-user JSON file path. Public so the wizard / about screen can
    // surface "your token is stored at ..." in the UI.
    static std::string TokenStoragePath();

private:
    std::string mLastError;
};

#endif // OPENFODDER_ENABLE_NETWORK
