/*
 *  Open Fodder
 *  ---------------
 *
 *  OFHUB/2 binary data-plane frame — client-side mirror of
 *  openfodder-hub/include/openfodder_hub/RoomFrame.hpp.
 *
 *  Wire layout (see openfodder-hub/docs/OFHUB2.md §6.1):
 *
 *      +--------+----------+-----------+-------------+----------+
 *      | type:1 |  seq:4   |  peer:1   | payload:N   |  tag:8   |
 *      +--------+----------+-----------+-------------+----------+
 *
 *    type    : 0x01 DATA, 0x02 REGISTER, 0x03 BYE
 *    seq     : big-endian, monotonic per-peer, wraps at 2^32
 *    peer    : sender's peer index in the room (0..capacity-1)
 *    payload : opaque game bytes (may be empty, e.g. for REGISTER)
 *    tag     : first 8 bytes of HMAC-SHA256(session_key,
 *                                           type || seq_be || peer || payload)
 *
 *  session_key is the 32-byte value returned in CREATEOK / JOINOK, scoped to
 *  one (room, peer). The hub re-tags on fan-out (§6.4); this module only
 *  exposes the per-(direction, key) primitives.
 */

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace openfodder_hubframe {

// Header (type + seq + peer) and tag sizes; minimum on-wire frame size.
inline constexpr std::size_t kHeaderSize = 6;
inline constexpr std::size_t kTagSize = 8;
inline constexpr std::size_t kMinFrameSize = kHeaderSize + kTagSize;  // 14
inline constexpr std::size_t kSessionKeySize = 32;

enum class FrameType : uint8_t {
    Data     = 0x01,
    Register = 0x02,
    Bye      = 0x03,
};

struct ParsedFrame {
    FrameType            type;
    uint32_t             seq;
    uint8_t              peer;
    const unsigned char* payload;
    std::size_t          payloadLen;
};

// Verify the trailing HMAC tag against `sessionKey` and decode the header
// fields into `*out`. Returns false (and does not touch `*out`) if:
//   * `length` < kMinFrameSize, or
//   * the type byte is not one of the FrameType values, or
//   * the tag does not match (constant-time compare via sodium_memcmp).
//
// On success, `out->payload` aliases into `dgram` (no copy); the caller must
// keep `dgram` alive while reading the payload.
bool VerifyAndParse(const unsigned char* dgram,
                    std::size_t length,
                    const std::array<unsigned char, kSessionKeySize>& sessionKey,
                    ParsedFrame* out);

// Serialise (type, seq, peer, payload) into `outBuffer` and append the 8-byte
// HMAC tag. The caller must size `outBuffer` to at least
// kMinFrameSize + payloadLen. Returns the number of bytes written.
std::size_t BuildAndTag(FrameType type,
                        uint32_t seq,
                        uint8_t peer,
                        const unsigned char* payload,
                        std::size_t payloadLen,
                        const std::array<unsigned char, kSessionKeySize>& sessionKey,
                        unsigned char* outBuffer);

// 1024-bit sliding-window replay filter. One instance per peer, per direction.
//
// Bitmap convention: `mBitmap[0]` bit 0 represents `mHighest` itself; bit k
// represents `mHighest - k`. Words higher in the array cover deeper history.
// Initial state (mHighest = 0, bitmap all clear) treats every slot as
// "unseen" — the first AcceptAndAdvance(0) sets bit 0 and is accepted; any
// repeat is rejected by the bitmap check.
//
//   * seq >  mHighest : advance, shift bitmap left by (seq - mHighest),
//                       set bit 0 for the new highest, accept.
//   * seq == mHighest : consult bit 0; reject if set, otherwise set & accept
//                       (covers cold-start seq=0).
//   * seq <  mHighest : reject if (mHighest - seq) >= 1024 (too old);
//                       otherwise consult the bit; if set reject, else
//                       set & accept.
class ReplayWindow {
public:
    bool AcceptAndAdvance(uint32_t seq);

private:
    uint32_t                  mHighest = 0;
    std::array<uint64_t, 16>  mBitmap{};
};

}  // namespace openfodder_hubframe
