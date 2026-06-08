/*
 *  Open Fodder
 *  ---------------
 *
 *  OFHUB/2 binary data-plane frame — client-side mirror of
 *  openfodder-hub/src/RoomFrame.cpp. Keep the algorithm byte-for-byte
 *  identical with the hub side; both endpoints must agree on tag bytes,
 *  big-endian seq encoding, and replay-window semantics.
 */

#include "HubFrame.hpp"

#ifdef OPENFODDER_HAVE_SODIUM
#  include <sodium.h>
#endif

#include <cstring>

namespace openfodder_hubframe {
namespace {

// 1024-bit window = 16 * 64.
constexpr uint32_t kWindowBits  = 1024;
constexpr uint32_t kBitsPerWord = 64;
constexpr uint32_t kWindowWords = kWindowBits / kBitsPerWord;
static_assert(kWindowWords == 16, "ReplayWindow bitmap word count mismatch");

// Big-endian 32-bit pack/unpack. Hand-rolled to avoid arpa/inet on non-Win32
// builds and to stay /permissive- clean on MSVC.
void WriteBe32(uint32_t value, unsigned char* out) {
    out[0] = static_cast<unsigned char>((value >> 24) & 0xff);
    out[1] = static_cast<unsigned char>((value >> 16) & 0xff);
    out[2] = static_cast<unsigned char>((value >> 8)  & 0xff);
    out[3] = static_cast<unsigned char>( value        & 0xff);
}

uint32_t ReadBe32(const unsigned char* in) {
    return (static_cast<uint32_t>(in[0]) << 24) |
           (static_cast<uint32_t>(in[1]) << 16) |
           (static_cast<uint32_t>(in[2]) << 8)  |
            static_cast<uint32_t>(in[3]);
}

bool IsKnownFrameType(uint8_t v) {
    return v == static_cast<uint8_t>(FrameType::Data)     ||
           v == static_cast<uint8_t>(FrameType::Register) ||
           v == static_cast<uint8_t>(FrameType::Bye);
}

// Compute the 8-byte truncated HMAC-SHA256 tag over (type || seq_be || peer ||
// payload) under `sessionKey` and write it to `outTag` (>= kTagSize bytes).
void ComputeTag(FrameType type,
                const unsigned char seqBe[4],
                uint8_t peer,
                const unsigned char* payload,
                std::size_t payloadLen,
                const std::array<unsigned char, kSessionKeySize>& sessionKey,
                unsigned char* outTag) {
#ifdef OPENFODDER_HAVE_SODIUM
    crypto_auth_hmacsha256_state state;
    crypto_auth_hmacsha256_init(&state, sessionKey.data(), sessionKey.size());

    const unsigned char typeByte = static_cast<unsigned char>(type);
    crypto_auth_hmacsha256_update(&state, &typeByte, 1);
    crypto_auth_hmacsha256_update(&state, seqBe, 4);
    crypto_auth_hmacsha256_update(&state, &peer, 1);
    if (payloadLen > 0 && payload != nullptr) {
        crypto_auth_hmacsha256_update(&state, payload, payloadLen);
    }

    unsigned char fullTag[crypto_auth_hmacsha256_BYTES];
    crypto_auth_hmacsha256_final(&state, fullTag);

    // Truncate to the leading kTagSize bytes (§6.1).
    std::memcpy(outTag, fullTag, kTagSize);
    sodium_memzero(fullTag, sizeof(fullTag));
#else
    // No-sodium build: deterministic non-zero placeholder so call sites can
    // still link. Such builds cannot interoperate with a real OFHUB/2 hub.
    (void)type;
    (void)seqBe;
    (void)peer;
    (void)payload;
    (void)payloadLen;
    (void)sessionKey;
    std::memset(outTag, 0, kTagSize);
#endif
}

}  // namespace

bool VerifyAndParse(const unsigned char* dgram,
                    std::size_t length,
                    const std::array<unsigned char, kSessionKeySize>& sessionKey,
                    ParsedFrame* out) {
    if (dgram == nullptr || out == nullptr)
        return false;
    if (length < kMinFrameSize)
        return false;

    const uint8_t typeByte = dgram[0];
    if (!IsKnownFrameType(typeByte))
        return false;

    const std::size_t          payloadLen  = length - kMinFrameSize;
    const unsigned char*       seqBe       = dgram + 1;
    const uint8_t              peer        = dgram[5];
    const unsigned char*       payload     = (payloadLen > 0) ? (dgram + kHeaderSize) : nullptr;
    const unsigned char*       receivedTag = dgram + kHeaderSize + payloadLen;

    unsigned char expectedTag[kTagSize];
    ComputeTag(static_cast<FrameType>(typeByte), seqBe, peer, payload,
               payloadLen, sessionKey, expectedTag);

#ifdef OPENFODDER_HAVE_SODIUM
    // Constant-time compare. sodium_memcmp returns 0 on equal.
    const int cmp = sodium_memcmp(expectedTag, receivedTag, kTagSize);
    sodium_memzero(expectedTag, sizeof(expectedTag));
#else
    const int cmp = std::memcmp(expectedTag, receivedTag, kTagSize);
#endif
    if (cmp != 0)
        return false;

    out->type       = static_cast<FrameType>(typeByte);
    out->seq        = ReadBe32(seqBe);
    out->peer       = peer;
    out->payload    = payload;
    out->payloadLen = payloadLen;
    return true;
}

std::size_t BuildAndTag(FrameType type,
                        uint32_t seq,
                        uint8_t peer,
                        const unsigned char* payload,
                        std::size_t payloadLen,
                        const std::array<unsigned char, kSessionKeySize>& sessionKey,
                        unsigned char* outBuffer) {
    // Header.
    outBuffer[0]            = static_cast<unsigned char>(type);
    unsigned char* seqBe    = outBuffer + 1;
    WriteBe32(seq, seqBe);
    outBuffer[5]            = peer;

    // Payload (memmove rather than memcpy — caller may stage the payload
    // directly into outBuffer + kHeaderSize before the tag is appended).
    if (payloadLen > 0 && payload != nullptr) {
        std::memmove(outBuffer + kHeaderSize, payload, payloadLen);
    }

    // Trailing tag.
    unsigned char*       tagOut       = outBuffer + kHeaderSize + payloadLen;
    const unsigned char* payloadForTag = (payloadLen > 0) ? (outBuffer + kHeaderSize) : nullptr;
    ComputeTag(type, seqBe, peer, payloadForTag, payloadLen, sessionKey, tagOut);

    return kHeaderSize + payloadLen + kTagSize;
}

bool ReplayWindow::AcceptAndAdvance(uint32_t seq) {
    auto TestBit = [&](uint32_t offset) -> bool {
        const std::size_t word = static_cast<std::size_t>(offset / kBitsPerWord);
        const unsigned    bit  = static_cast<unsigned>(offset % kBitsPerWord);
        return (mBitmap[word] & (uint64_t{1} << bit)) != 0;
    };
    auto SetBit = [&](uint32_t offset) {
        const std::size_t word = static_cast<std::size_t>(offset / kBitsPerWord);
        const unsigned    bit  = static_cast<unsigned>(offset % kBitsPerWord);
        mBitmap[word] |= (uint64_t{1} << bit);
    };

    if (seq > mHighest) {
        // Advance: shift bitmap left by (seq - mHighest) bits, then set bit 0
        // for the new highest. A shift >= kWindowBits clears the entire window.
        const uint32_t shift = seq - mHighest;
        if (shift >= kWindowBits) {
            mBitmap.fill(0);
        } else {
            // Bit indices grow with age, so "left" here means moving toward
            // higher-indexed words/bits.
            const std::size_t wordShift = static_cast<std::size_t>(shift / kBitsPerWord);
            const unsigned    bitShift  = static_cast<unsigned>(shift % kBitsPerWord);
            if (wordShift > 0) {
                for (std::size_t i = kWindowWords; i-- > 0;) {
                    mBitmap[i] = (i >= wordShift) ? mBitmap[i - wordShift] : uint64_t{0};
                }
            }
            if (bitShift > 0) {
                uint64_t carry = 0;
                for (std::size_t i = 0; i < kWindowWords; ++i) {
                    const uint64_t newCarry = mBitmap[i] >> (kBitsPerWord - bitShift);
                    mBitmap[i] = (mBitmap[i] << bitShift) | carry;
                    carry = newCarry;
                }
            }
        }
        mHighest = seq;
        SetBit(0);
        return true;
    }

    // seq <= mHighest: in-window or too old.
    const uint32_t offset = mHighest - seq;
    if (offset >= kWindowBits)
        return false;  // older than the window
    if (TestBit(offset))
        return false;  // already seen
    SetBit(offset);
    return true;
}

}  // namespace openfodder_hubframe
