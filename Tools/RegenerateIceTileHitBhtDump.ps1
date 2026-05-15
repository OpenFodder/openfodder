<#
.SYNOPSIS
Regenerate Documentation/IceTileHitBhtDump.txt from Amiga retail data.

.DESCRIPTION
Reads icebase.hit + icesub0.hit (gameplay type codes), icebase.bht + icesub0.bht
(8x8 hit-table bits), and icebase.blk + icesub0.blk (16x16 pixel data) and emits
one block per tile combining the hit/bht metadata with a pixel-derived 8x8
classification so tiles whose .bht differs from their visual content (e.g. tile
84, a horizontal-split shoreline tile whose bht is empty) are obvious.

Hit byte format (big-endian):
  bit  15      = DUAL flag
  byte hi 0..6 = (unused, observed always zero apart from DUAL)
  byte lo high nibble = secondary type code
  byte lo low  nibble = primary type code

Type codes (observed):
  0 = Land (.)
  5 = WaterEdge (W)
  6 = Water (W)
  7 = Snow (S)
  8 = Ice (I)
  others printed as ?N(?)

Pixel classification per 2x2 block:
  - all palette indices <= SnowMax  : Snow      -> S
  - all palette indices >= WaterMin : Water     -> .
  - mixed                           : Boundary  -> *
  - all "other" (not snow, not water): treat as Other -> ?

.PARAMETER DataDir
Directory containing icebase.hit/.bht/.blk and icesub0.hit/.bht/.blk
.PARAMETER OutFile
Where to write the regenerated dump.
.PARAMETER SnowMax / WaterMin
Palette thresholds for snow vs water classification of each pixel.
#>
param(
    [string]$DataDir = (Join-Path $PSScriptRoot '..\Run\Data\Amiga'),
    [string]$OutFile = (Join-Path $PSScriptRoot '..\Documentation\IceTileHitBhtDump.txt'),
    [int]$SnowMax = 3,
    [int]$WaterMin = 8
)

$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'Lib/RncUnpack.ps1')

function Concat-Bytes([byte[]]$a, [byte[]]$b) {
    $out = New-Object byte[] ($a.Length + $b.Length)
    [Array]::Copy($a, 0, $out, 0, $a.Length)
    [Array]::Copy($b, 0, $out, $a.Length, $b.Length)
    return $out
}

$baseHit = [IO.File]::ReadAllBytes((Join-Path $DataDir 'icebase.hit'))
$subHit  = [IO.File]::ReadAllBytes((Join-Path $DataDir 'icesub0.hit'))
$hit = Concat-Bytes $baseHit $subHit
$tileCount = $hit.Length / 2

$baseBht = [IO.File]::ReadAllBytes((Join-Path $DataDir 'icebase.bht'))
$subBht  = [IO.File]::ReadAllBytes((Join-Path $DataDir 'icesub0.bht'))
$bht = Concat-Bytes $baseBht $subBht
if($bht.Length -ne ($tileCount * 8)) {
    throw "bht size mismatch: have $($bht.Length) bytes, expected $($tileCount*8) for $tileCount tiles"
}

$baseBlk = Read-MaybeRnc -Path (Join-Path $DataDir 'icebase.blk')
$subBlk  = Read-MaybeRnc -Path (Join-Path $DataDir 'icesub0.blk')
$blk = Concat-Bytes $baseBlk $subBlk

function Decode-Tile([byte[]]$Blk, [int]$TileId) {
    $px = New-Object byte[] 256
    $offset = $TileId -shl 7
    if(($offset + 128) -gt $Blk.Length) { return $null }
    for($plane = 0; $plane -lt 4; ++$plane) {
        $pb = $offset + ($plane * 32)
        for($row = 0; $row -lt 16; ++$row) {
            $hi = $Blk[$pb + $row * 2]
            $lo = $Blk[$pb + $row * 2 + 1]
            $word = ([int]$hi -shl 8) -bor [int]$lo
            $rb = $row * 16
            for($x = 0; $x -lt 16; ++$x) {
                if(($word -band (0x8000 -shr $x)) -ne 0) {
                    $px[$rb + $x] = [byte]($px[$rb + $x] -bor (1 -shl $plane))
                }
            }
        }
    }
    return $px
}

function Type-Char([int]$code) {
    switch($code) {
        0 { return @('Land', '.') }
        5 { return @('WaterEdge', 'e') }
        6 { return @('Water', 'W') }
        7 { return @('Snow', 'S') }
        8 { return @('Ice', 'I') }
        default { return @("?$code", '?') }
    }
}

$lines = New-Object System.Collections.Generic.List[string]

for($tileId = 0; $tileId -lt $tileCount; ++$tileId) {
    $hi = $hit[$tileId * 2]
    $lo = $hit[$tileId * 2 + 1]
    $val = ([int]$hi -shl 8) -bor [int]$lo
    $dual = (($hi -band 0x80) -ne 0)
    $primaryCode   = $lo -band 0x0F
    $secondaryCode = ($lo -shr 4) -band 0x0F
    $p = Type-Char $primaryCode
    $s = Type-Char $secondaryCode
    $primaryName = $p[0]; $primaryGlyph = $p[1]
    $secondaryName = $s[0]; $secondaryGlyph = $s[1]

    $head = "tile {0,3}  hit=0x{1:x4}  primary={2}({3})  secondary={4}({5})" -f `
        $tileId, $val, $primaryName, $primaryGlyph, $secondaryName, $secondaryGlyph
    if($dual) { $head += ' DUAL' }
    $lines.Add($head)

    # bht 8 bytes = 8 rows
    $bhtBytes = New-Object byte[] 8
    [Array]::Copy($bht, $tileId * 8, $bhtBytes, 0, 8)
    $bhtHex = ''
    $ones = 0
    foreach($byte in $bhtBytes) {
        $bhtHex += '{0:x2}' -f $byte
        $v = [int]$byte
        for($k = 0; $k -lt 8; ++$k) { if(($v -band (1 -shl $k)) -ne 0) { $ones++ } }
    }
    $lines.Add(("         bht=$bhtHex  ones=$ones/64"))

    # bht visualization: bit set = secondary glyph, bit clear = primary glyph
    for($r = 0; $r -lt 8; ++$r) {
        $row = ''
        $byte = [int]$bhtBytes[$r]
        for($c = 0; $c -lt 8; ++$c) {
            $bit = ($byte -shr (7 - $c)) -band 1
            if($bit -ne 0) { $row += $secondaryGlyph } else { $row += $primaryGlyph }
        }
        $lines.Add('         ' + $row)
    }

    # Pixel-derived 8x8 (each cell = 2x2 block of source pixels)
    $px = Decode-Tile $blk $tileId
    if($null -ne $px) {
        $lines.Add('         pixels:')
        for($r = 0; $r -lt 8; ++$r) {
            $row = ''
            for($c = 0; $c -lt 8; ++$c) {
                $snowCnt = 0; $waterCnt = 0; $otherCnt = 0
                for($dy = 0; $dy -lt 2; ++$dy) {
                    for($dx = 0; $dx -lt 2; ++$dx) {
                        $py = ($r * 2) + $dy
                        $pxv = ($c * 2) + $dx
                        $idx = [int]$px[$py * 16 + $pxv]
                        if($idx -le $SnowMax) { $snowCnt++ }
                        elseif($idx -ge $WaterMin) { $waterCnt++ }
                        else { $otherCnt++ }
                    }
                }
                if($snowCnt -eq 4) { $row += 'S' }
                elseif($waterCnt -eq 4) { $row += '.' }
                elseif($otherCnt -eq 4) { $row += '?' }
                else { $row += '*' }
            }
            $lines.Add('         ' + $row)
        }
    }
}

[System.IO.File]::WriteAllLines($OutFile, $lines)
Write-Output "wrote=$OutFile tiles=$tileCount"
