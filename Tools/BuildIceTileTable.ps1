<#
.SYNOPSIS
Build an authoritative class/pair/bm table for every ice tile.

.DESCRIPTION
Reads icebase.hit + icesub0.hit and icebase.bht + icesub0.bht. For each tile,
decodes the primary and secondary type codes, classifies the tile into a
"pair" category based on the canonical chain (snow=0, ice=1, shallow=2,
deep=3), and computes a per-octant 8-bit bm derived from the .bht layout
using strict octant majority of class-A cells (where A is the lower-index
class of the DUAL pair).

Pair categories:
  pure-snow / pure-ice / pure-shallow / pure-deep / pure-land
  snow-ice / ice-shallow / shallow-deep      (adjacent on chain)
  snow-shallow / snow-deep / ice-deep        (chain-skip; rare)
  unknown                                    (codes outside the known set)

bm convention (matches existing IceData.js):
  rule = bm_cf1_<A>_<B>
  center = class B (the more-water-ward class)
  bm bit i = 1 iff that octant is dominated by class A (the more-land-ward).

For pure tiles bm is "--------" (not applicable).

Octants (8x8):
  NW: r0-2 c0-2   N : r0-2 c3-4   NE: r0-2 c5-7
  W : r3-4 c0-2                   E : r3-4 c5-7
  SW: r5-7 c0-2   S : r5-7 c3-4   SE: r5-7 c5-7

Strict majority (count*2 > total) so ties resolve to 0.
#>
param(
    [string]$DataDir = (Join-Path $PSScriptRoot '..\Run\Data\Amiga'),
    [string]$OutFile = (Join-Path $PSScriptRoot '..\Documentation\IceTileClassTable.txt')
)

$ErrorActionPreference = 'Stop'

function ConcatBytes([byte[]]$a, [byte[]]$b) {
    $out = New-Object byte[] ($a.Length + $b.Length)
    [Array]::Copy($a, 0, $out, 0, $a.Length)
    [Array]::Copy($b, 0, $out, $a.Length, $b.Length)
    return $out
}

$baseHit = [IO.File]::ReadAllBytes((Join-Path $DataDir 'icebase.hit'))
$subHit  = [IO.File]::ReadAllBytes((Join-Path $DataDir 'icesub0.hit'))
$hit = ConcatBytes $baseHit $subHit

$baseBht = [IO.File]::ReadAllBytes((Join-Path $DataDir 'icebase.bht'))
$subBht  = [IO.File]::ReadAllBytes((Join-Path $DataDir 'icesub0.bht'))
$bht = ConcatBytes $baseBht $subBht

$tileCount = $hit.Length / 2
if($bht.Length -ne ($tileCount * 8)) {
    throw "bht size $($bht.Length) != $tileCount * 8"
}

# Chain index per type code. -1 means unknown / off-chain.
function ChainIndex([int]$code) {
    switch($code) {
        7 { 0 }   # Snow
        8 { 1 }   # Ice
        5 { 2 }   # WaterEdge / shallow
        6 { 3 }   # Water / deep
        0 { -2 }  # Land - off-chain but known
        default { -1 }
    }
}

function ChainName([int]$idx) {
    switch($idx) {
        0  { 'snow' }
        1  { 'ice' }
        2  { 'shallow' }
        3  { 'deep' }
        -2 { 'land' }
        default { 'unknown' }
    }
}

function TypeName([int]$c) {
    switch($c) {
        0 { 'Land' }
        5 { 'WaterEdge' }
        6 { 'Water' }
        7 { 'Snow' }
        8 { 'Ice' }
        default { "?$c" }
    }
}

# Octants
$octants = @(
    @{ name = 'NW'; r0 = 0; r1 = 3; c0 = 0; c1 = 3 },
    @{ name = 'N';  r0 = 0; r1 = 3; c0 = 3; c1 = 5 },
    @{ name = 'NE'; r0 = 0; r1 = 3; c0 = 5; c1 = 8 },
    @{ name = 'W';  r0 = 3; r1 = 5; c0 = 0; c1 = 3 },
    @{ name = 'E';  r0 = 3; r1 = 5; c0 = 5; c1 = 8 },
    @{ name = 'SW'; r0 = 5; r1 = 8; c0 = 0; c1 = 3 },
    @{ name = 'S';  r0 = 5; r1 = 8; c0 = 3; c1 = 5 },
    @{ name = 'SE'; r0 = 5; r1 = 8; c0 = 5; c1 = 8 }
)

# For one tile, count cells whose .bht bit yields the "A" class.
# countAIfBitIs0 = true means A = primary (bit 0); false means A = secondary (bit 1).
function ComputeBm([int]$tileId, [bool]$AIsPrimary) {
    $bits = ''
    foreach($oct in $octants) {
        $tot = 0; $hits = 0
        for($r = $oct.r0; $r -lt $oct.r1; ++$r) {
            $byte = [int]$bht[$tileId * 8 + $r]
            for($c = $oct.c0; $c -lt $oct.c1; ++$c) {
                $bit = ($byte -shr (7 - $c)) -band 1
                $isA = if($AIsPrimary) { $bit -eq 0 } else { $bit -eq 1 }
                $tot++
                if($isA) { $hits++ }
            }
        }
        if(($hits * 2) -gt $tot) { $bits += '1' } else { $bits += '0' }
    }
    return $bits
}

$lines = New-Object 'System.Collections.Generic.List[string]'
$lines.Add('# Ice tile class table')
$lines.Add('# Columns: tile  hit  primary/secondary  DUAL  pair  bm')
$lines.Add('# bm bit order: NW N NE W E SW S SE  (1 = octant dominated by class A, the more-land-ward class)')
$lines.Add('')

# Counters
$pairCounts = @{}
$pairTiles = @{}

for($t = 0; $t -lt $tileCount; ++$t) {
    $hi = $hit[$t * 2]
    $lo = $hit[$t * 2 + 1]
    $val = ([int]$hi -shl 8) -bor [int]$lo
    $dual = (($hi -band 0x80) -ne 0)
    $pri = $lo -band 0x0F
    $sec = ($lo -shr 4) -band 0x0F

    $priIdx = ChainIndex $pri
    $secIdx = ChainIndex $sec
    $priName = TypeName $pri
    $secName = TypeName $sec

    $bm = '--------'
    $pair = ''

    if($dual) {
        if($priIdx -lt 0 -or $secIdx -lt 0) {
            $pair = 'unknown-dual'
        }
        elseif($priIdx -eq $secIdx) {
            $pair = 'self-dual-' + (ChainName $priIdx)
        }
        else {
            # Determine A (lower idx) and B (higher idx)
            if($priIdx -lt $secIdx) {
                $aName = ChainName $priIdx
                $bName = ChainName $secIdx
                $aIsPri = $true
            }
            else {
                $aName = ChainName $secIdx
                $bName = ChainName $priIdx
                $aIsPri = $false
            }
            $pair = $aName + '-' + $bName
            $bm = ComputeBm $t $aIsPri
        }
    }
    else {
        # Non-DUAL: only the primary code is meaningful
        if($priIdx -lt 0) {
            $pair = 'pure-unknown'
        }
        else {
            $pair = 'pure-' + (ChainName $priIdx)
        }
    }

    if(-not $pairCounts.ContainsKey($pair)) {
        $pairCounts[$pair] = 0
        $pairTiles[$pair] = New-Object 'System.Collections.Generic.List[int]'
    }
    $pairCounts[$pair]++
    [void]$pairTiles[$pair].Add($t)

    $dualFlag = if($dual) { 'DUAL' } else { '    ' }
    $line = ('tile {0,3}  hit=0x{1:x4}  {2,-9}/{3,-9} {4}  pair={5,-16} bm={6}' `
        -f $t, $val, $priName, $secName, $dualFlag, $pair, $bm)
    $lines.Add($line)
}

$lines.Add('')
$lines.Add('===== Pair summary =====')
foreach($k in ($pairCounts.Keys | Sort-Object)) {
    $lines.Add(("  {0,-22} {1,4} tiles" -f $k, $pairCounts[$k]))
}
$lines.Add('')

$lines.Add('===== Tiles per pair =====')
foreach($k in ($pairTiles.Keys | Sort-Object)) {
    $ids = $pairTiles[$k] | Sort-Object
    $lines.Add(("{0}: " -f $k) + ($ids -join ', '))
}

[System.IO.File]::WriteAllLines($OutFile, $lines)
Write-Output "wrote=$OutFile tiles=$tileCount"
