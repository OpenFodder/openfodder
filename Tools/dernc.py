"""RNC1 unpacker, ported from Source/Amiga/dernc.cpp.

Use: import dernc; raw = dernc.unpack(open('foo.blk','rb').read())
"""

RNC_SIGNATURE = 0x524E4301  # 'RNC\x01'


class _BitStream:
    __slots__ = ("bitbuf", "bitcount")

    def __init__(self, data, p):
        self.bitbuf = data[p[0]] | (data[p[0] + 1] << 8)
        self.bitcount = 16


def _bit_advance(bs, n, data, p):
    bs.bitbuf >>= n
    bs.bitcount -= n
    if bs.bitcount < 16:
        p[0] += 2
        lw = data[p[0]] | (data[p[0] + 1] << 8)
        bs.bitbuf |= lw << bs.bitcount
        bs.bitcount += 16


def _bit_read(bs, mask, n, data, p):
    result = bs.bitbuf & mask
    _bit_advance(bs, n, data, p)
    return result


def _bitread_fix(bs, data, p):
    bs.bitcount -= 16
    bs.bitbuf &= (1 << bs.bitcount) - 1
    lw = data[p[0]] | (data[p[0] + 1] << 8)
    bs.bitbuf |= lw << bs.bitcount
    bs.bitcount += 16


def _mirror(x, n):
    top = 1 << (n - 1)
    bottom = 1
    while top > bottom:
        mask = top | bottom
        masked = x & mask
        if masked and masked != mask:
            x ^= mask
        top >>= 1
        bottom <<= 1
    return x


def _read_huftable(bs, data, p):
    num = _bit_read(bs, 0x1F, 5, data, p)
    table = []
    if not num:
        return table

    leaflen = []
    leafmax = 1
    for _ in range(num):
        ll = _bit_read(bs, 0x0F, 4, data, p)
        leaflen.append(ll)
        if leafmax < ll:
            leafmax = ll

    codeb = 0
    for i in range(1, leafmax + 1):
        for j in range(num):
            if leaflen[j] == i:
                table.append((_mirror(codeb, i), i, j))
                codeb += 1
        codeb <<= 1
    return table


def _huf_read(table, bs, data, p):
    for code, codelen, value in table:
        mask = (1 << codelen) - 1
        if (bs.bitbuf & mask) == code:
            _bit_advance(bs, codelen, data, p)
            if value >= 2:
                val = 1 << (value - 1)
                val |= _bit_read(bs, val - 1, value - 1, data, p)
                return val
            return value
    return -1


def unpack(packed):
    data = bytearray(packed) + b"\x00\x00\x00\x00"
    if int.from_bytes(data[:4], "big") != RNC_SIGNATURE:
        raise ValueError("Not RNC1 data")

    ret_len = int.from_bytes(data[4:8], "big")
    in_size = int.from_bytes(data[8:12], "big")
    output = bytearray(ret_len)
    out_pos = 0
    p = [18]                              # input pointer (mutable list cell)
    inputend = 18 + in_size

    bs = _BitStream(data, p)
    _bit_advance(bs, 2, data, p)          # discard first two bits

    while out_pos < ret_len:
        raw = _read_huftable(bs, data, p)
        dist = _read_huftable(bs, data, p)
        length_t = _read_huftable(bs, data, p)
        ch_count = _bit_read(bs, 0xFFFF, 16, data, p)

        while True:
            length = _huf_read(raw, bs, data, p)
            if length == -1:
                raise ValueError("Huffman decode error")
            if length:
                for _ in range(length):
                    output[out_pos] = data[p[0]]
                    out_pos += 1
                    p[0] += 1
                _bitread_fix(bs, data, p)
            ch_count -= 1
            if ch_count <= 0:
                break

            posn = _huf_read(dist, bs, data, p)
            if posn == -1:
                raise ValueError("Huffman decode error")
            length = _huf_read(length_t, bs, data, p)
            if length == -1:
                raise ValueError("Huffman decode error")
            posn += 1
            length += 2
            for _ in range(length):
                output[out_pos] = output[out_pos - posn]
                out_pos += 1

    if out_pos != ret_len:
        raise ValueError("Size mismatch")
    return bytes(output)
