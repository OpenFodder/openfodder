<#
RNC1 (ProPack) unpacker, ported from Source/Amiga/dernc.cpp into a single C# class
loaded via Add-Type. Original C source: https://github.com/alexandergitter/THViewer

Exposes one PowerShell function:
    Read-MaybeRnc -Path <file>          -> byte[]   (decompresses if RNC, else returns raw bytes)

The renderer dot-sources this file and uses Read-MaybeRnc in place of [IO.File]::ReadAllBytes.
#>

if(-not ('OpenFodder.RncUnpack' -as [type])) {
    Add-Type -TypeDefinition @'
using System;

namespace OpenFodder {
    public static class RncUnpack {
        const uint RNC_SIGNATURE = 0x524E4301; // "RNC\x01"

        // ---- Bit-stream state ----
        struct BitStream { public uint bitbuf; public int bitcount; }

        struct HufNode { public uint code; public int codelen; public int value; }

        sealed class HufTable { public int num; public HufNode[] table = new HufNode[32]; }

        public static byte[] Unpack(byte[] packed) {
            if(packed == null || packed.Length < 18)
                throw new InvalidOperationException("RNC: input too short");

            uint sig = Blong(packed, 0);
            if(sig != RNC_SIGNATURE)
                throw new InvalidOperationException("RNC: bad signature 0x" + sig.ToString("X8"));

            uint retLen   = Blong(packed, 4);
            uint packLen  = Blong(packed, 8);
            ushort outCrc = (ushort)Bword(packed, 12);
            ushort packCrc = (ushort)Bword(packed, 14);

            int inputStart = 18;
            int inputEnd = inputStart + (int)packLen;
            if(inputEnd > packed.Length)
                throw new InvalidOperationException("RNC: declared packed size > file size");

            if(Crc(packed, inputStart, (int)packLen) != packCrc)
                throw new InvalidOperationException("RNC: packed-data CRC mismatch");

            byte[] output = new byte[retLen];
            int outPos = 0;
            int inPos = inputStart;

            BitStream bs = new BitStream();
            BitreadInit(ref bs, packed, ref inPos);
            BitAdvance(ref bs, 2, packed, ref inPos);

            HufTable raw = new HufTable();
            HufTable dist = new HufTable();
            HufTable len = new HufTable();

            while(outPos < output.Length) {
                ReadHuftable(raw,  ref bs, packed, ref inPos);
                ReadHuftable(dist, ref bs, packed, ref inPos);
                ReadHuftable(len,  ref bs, packed, ref inPos);
                long chCount = (long)BitRead(ref bs, 0xFFFF, 16, packed, ref inPos);

                while(true) {
                    long length = HufRead(raw, ref bs, packed, ref inPos);
                    if(length < 0)
                        throw new InvalidOperationException("RNC: huffman decode error (raw)");

                    if(length > 0) {
                        if(outPos + length > output.Length || inPos + length > packed.Length)
                            throw new InvalidOperationException("RNC: literal run out of bounds");
                        for(long i = 0; i < length; i++)
                            output[outPos++] = packed[inPos++];
                        BitreadFix(ref bs, packed, ref inPos);
                    }

                    if(--chCount <= 0)
                        break;

                    long posn = HufRead(dist, ref bs, packed, ref inPos);
                    if(posn < 0)
                        throw new InvalidOperationException("RNC: huffman decode error (dist)");
                    long mlen = HufRead(len, ref bs, packed, ref inPos);
                    if(mlen < 0)
                        throw new InvalidOperationException("RNC: huffman decode error (len)");

                    posn += 1;
                    mlen += 2;
                    if(outPos - posn < 0 || outPos + mlen > output.Length)
                        throw new InvalidOperationException("RNC: back-ref out of bounds");
                    for(long i = 0; i < mlen; i++) {
                        output[outPos] = output[outPos - posn];
                        outPos++;
                    }
                }
            }

            if(outPos != output.Length)
                throw new InvalidOperationException("RNC: output length mismatch");
            if(Crc(output, 0, output.Length) != outCrc)
                throw new InvalidOperationException("RNC: unpacked-data CRC mismatch");

            return output;
        }

        // ---- Huffman ----
        static void ReadHuftable(HufTable h, ref BitStream bs, byte[] data, ref int p) {
            int num = (int)BitRead(ref bs, 0x1F, 5, data, ref p);
            h.num = 0;
            if(num == 0) return;

            int[] leaflen = new int[32];
            int leafmax = 1;
            for(int i = 0; i < num; i++) {
                leaflen[i] = (int)BitRead(ref bs, 0x0F, 4, data, ref p);
                if(leafmax < leaflen[i]) leafmax = leaflen[i];
            }

            uint codeb = 0;
            int k = 0;
            for(int i = 1; i <= leafmax; i++) {
                for(int j = 0; j < num; j++) {
                    if(leaflen[j] == i) {
                        h.table[k].code = Mirror(codeb, i);
                        h.table[k].codelen = i;
                        h.table[k].value = j;
                        codeb++;
                        k++;
                    }
                }
                codeb <<= 1;
            }
            h.num = k;
        }

        static long HufRead(HufTable h, ref BitStream bs, byte[] data, ref int p) {
            int i;
            for(i = 0; i < h.num; i++) {
                uint mask = (uint)((1 << h.table[i].codelen) - 1);
                if((bs.bitbuf & mask) == h.table[i].code) break;
            }
            if(i == h.num) return -1;

            BitAdvance(ref bs, h.table[i].codelen, data, ref p);

            long val = h.table[i].value;
            if(val >= 2) {
                long bits = val - 1;
                val = 1L << (int)bits;
                val |= (long)BitRead(ref bs, (uint)(val - 1), (int)bits, data, ref p);
            }
            return val;
        }

        // ---- Bit stream ----
        static void BitreadInit(ref BitStream bs, byte[] data, ref int p) {
            bs.bitbuf = Lword(data, p);
            bs.bitcount = 16;
        }

        static void BitreadFix(ref BitStream bs, byte[] data, ref int p) {
            bs.bitcount -= 16;
            bs.bitbuf &= (uint)((1 << bs.bitcount) - 1);
            bs.bitbuf |= (Lword(data, p) << bs.bitcount);
            bs.bitcount += 16;
        }

        static void BitAdvance(ref BitStream bs, int n, byte[] data, ref int p) {
            bs.bitbuf >>= n;
            bs.bitcount -= n;
            if(bs.bitcount < 16) {
                p += 2;
                bs.bitbuf |= (Lword(data, p) << bs.bitcount);
                bs.bitcount += 16;
            }
        }

        static uint BitRead(ref BitStream bs, uint mask, int n, byte[] data, ref int p) {
            uint result = bs.bitbuf & mask;
            BitAdvance(ref bs, n, data, ref p);
            return result;
        }

        // ---- Helpers ----
        static uint Blong(byte[] p, int o) {
            return ((uint)p[o] << 24) | ((uint)p[o + 1] << 16) | ((uint)p[o + 2] << 8) | (uint)p[o + 3];
        }

        static uint Bword(byte[] p, int o) {
            return ((uint)p[o] << 8) | (uint)p[o + 1];
        }

        static uint Lword(byte[] p, int o) {
            if(o + 1 >= p.Length) {
                if(o >= p.Length) return 0;
                return (uint)p[o];
            }
            return ((uint)p[o + 1] << 8) | (uint)p[o];
        }

        static uint Mirror(uint x, int n) {
            uint top = 1u << (n - 1);
            uint bottom = 1u;
            while(top > bottom) {
                uint mask = top | bottom;
                uint masked = x & mask;
                if(masked != 0 && masked != mask)
                    x ^= mask;
                top >>= 1;
                bottom <<= 1;
            }
            return x;
        }

        static ushort Crc(byte[] data, int offset, int len) {
            ushort[] crctab = new ushort[256];
            for(int i = 0; i < 256; i++) {
                ushort val = (ushort)i;
                for(int j = 0; j < 8; j++) {
                    if((val & 1) != 0) val = (ushort)((val >> 1) ^ 0xA001);
                    else val = (ushort)(val >> 1);
                }
                crctab[i] = val;
            }

            ushort crc = 0;
            for(int i = 0; i < len; i++) {
                crc ^= data[offset + i];
                crc = (ushort)((crc >> 8) ^ crctab[crc & 0xFF]);
            }
            return crc;
        }
    }
}
'@
}

function Read-MaybeRnc {
    [CmdletBinding()]
    param([Parameter(Mandatory = $true)][string]$Path)

    $bytes = [IO.File]::ReadAllBytes($Path)
    if($bytes.Length -ge 4 -and $bytes[0] -eq 0x52 -and $bytes[1] -eq 0x4E -and $bytes[2] -eq 0x43 -and $bytes[3] -eq 0x01) {
        return [OpenFodder.RncUnpack]::Unpack($bytes)
    }
    return $bytes
}
