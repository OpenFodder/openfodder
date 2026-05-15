<#
.SYNOPSIS
Verify that every (bm, tile) entry in IceData.js's bm_cf1_ice_*_shallowwater
rules picks a tile whose bht layout matches the bm.

.DESCRIPTION
Each rule has its own meaning of "bm bit = 1":
  bm_cf1_ice_deepwater_shallowwater  : 1 = WaterEdge (shallow), 0 = Water (deep)
  bm_cf1_ice_wetice_shallowwater     : 1 = land (Snow/Ice/Land/...), 0 = water (Water/WaterEdge)
  bm_cf1_ice_dryice_wetice           : 1 = ground (Snow/Land), 0 = wet (Ice/...)

For each tile, build a per-cell predicate "is this cell of class 1?" using the
.bht (cell type = primary if bit=0, secondary if bit=1). Then split the 8x8
into octants and compute observed bm by majority vote.

Octant layout:
  NW: r0-2 c0-2   N:  r0-2 c3-4   NE: r0-2 c5-7
  W : r3-4 c0-2                   E : r3-4 c5-7
  SW: r5-7 c0-2   S:  r5-7 c3-4   SE: r5-7 c5-7
#>
param(
    [string]$DataDir = (Join-Path $PSScriptRoot '..\Run\Data\Amiga'),
    [string]$IceData = (Join-Path $PSScriptRoot '..\Run\Scripts\MapGen\Terrain\Smoothing\Data\IceData.js'),
    [string]$OutFile = (Join-Path $PSScriptRoot '..\Documentation\IceBmVerify.txt')
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

# Read primary/secondary type code per tile
$primaryCodes = New-Object int[] $tileCount
$secondaryCodes = New-Object int[] $tileCount
for($t = 0; $t -lt $tileCount; ++$t) {
    $lo = $hit[$t * 2 + 1]
    $primaryCodes[$t]   = $lo -band 0x0F
    $secondaryCodes[$t] = ($lo -shr 4) -band 0x0F
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

# Octant rectangles
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

function ObservedBm([int]$tileId, [scriptblock]$IsClass1) {
    $bits = ''
    foreach($oct in $octants) {
        $hits = 0; $tot = 0
        for($r = $oct.r0; $r -lt $oct.r1; ++$r) {
            $byte = [int]$bht[$tileId * 8 + $r]
            for($c = $oct.c0; $c -lt $oct.c1; ++$c) {
                $bit = ($byte -shr (7 - $c)) -band 1
                $code = if($bit -eq 0) { $primaryCodes[$tileId] } else { $secondaryCodes[$tileId] }
                $tot++
                if(& $IsClass1 $code) { $hits++ }
            }
        }
        if(($hits * 2) -ge $tot) { $bits += '1' } else { $bits += '0' }
    }
    return $bits
}

function CountClass1([int]$tileId, [scriptblock]$IsClass1) {
    $hits = 0
    for($r = 0; $r -lt 8; ++$r) {
        $byte = [int]$bht[$tileId * 8 + $r]
        for($c = 0; $c -lt 8; ++$c) {
            $bit = ($byte -shr (7 - $c)) -band 1
            $code = if($bit -eq 0) { $primaryCodes[$tileId] } else { $secondaryCodes[$tileId] }
            if(& $IsClass1 $code) { $hits++ }
        }
    }
    return $hits
}

# Parse IceData.js bm rule blocks
$src = [IO.File]::ReadAllText($IceData)
function ExtractBlock([string]$src, [string]$name) {
    $idx = $src.IndexOf($name)
    if($idx -lt 0) { return $null }
    $bIdx = $src.IndexOf('bitmask', $idx)
    if($bIdx -lt 0) { return $null }
    $arrStart = $src.IndexOf('[', $bIdx)
    if($arrStart -lt 0) { return $null }
    $depth = 0; $i = $arrStart
    while($i -lt $src.Length) {
        if($src[$i] -eq '[') { $depth++ }
        elseif($src[$i] -eq ']') { $depth--; if($depth -eq 0) { return $src.Substring($arrStart, $i - $arrStart + 1) } }
        $i++
    }
    return $null
}
function ParseEntries([string]$block) {
    $entries = New-Object 'System.Collections.Generic.List[object]'
    if($null -eq $block) { return $entries }
    $rx = [regex]'\{\s*bm:\s*"([01]{8})"\s*,\s*tiles:\s*\[([^\]]*)\]\s*\}'
    foreach($m in $rx.Matches($block)) {
        $tilesText = $m.Groups[2].Value
        $tileMatches = [regex]::Matches($tilesText, 'tile:\s*"(\d+)"')
        $tiles = @()
        foreach($tm in $tileMatches) { $tiles += [int]$tm.Groups[1].Value }
        $entries.Add(@{ Bm = $m.Groups[1].Value; Tiles = $tiles })
    }
    return $entries
}

# Class predicates per rule
$isLand = { param($c) ($c -ne 5) -and ($c -ne 6) }    # not WaterEdge, not Water
$isShallow = { param($c) $c -eq 5 }                   # WaterEdge only
$isGround = { param($c) ($c -eq 7) -or ($c -eq 0) }   # Snow or Land

$rules = @(
    @{ Name = 'bm_cf1_ice_deepwater_shallowwater'; Pred = $isShallow; PredDesc = '1=WaterEdge (shallow)' },
    @{ Name = 'bm_cf1_ice_wetice_shallowwater';    Pred = $isLand;    PredDesc = '1=land (non-water)'    },
    @{ Name = 'bm_cf1_ice_dryice_wetice';          Pred = $isGround;  PredDesc = '1=ground (Snow/Land)'  }
)

$out = New-Object 'System.Collections.Generic.List[string]'
foreach($rule in $rules) {
    $body = ExtractBlock $src $rule.Name
    $entries = ParseEntries $body
    $out.Add('===== ' + $rule.Name + ' (' + $rule.PredDesc + ') =====')
    $out.Add('')
    if($entries.Count -eq 0) { $out.Add('NO ENTRIES PARSED'); $out.Add(''); continue }

    $passCount = 0; $failCount = 0; $undefinedCount = 0
    foreach($entry in $entries) {
        foreach($tile in $entry.Tiles) {
            if($tile -ge $tileCount) {
                $out.Add(("FAIL  bm={0} tile={1,3}  out-of-range" -f $entry.Bm, $tile))
                $failCount++; continue
            }
            $obs = ObservedBm $tile $rule.Pred
            $cnt = CountClass1 $tile $rule.Pred
            $note = ''
            if($cnt -eq 0)  { $note = '  pure-class0'; $undefinedCount++ }
            elseif($cnt -eq 64) { $note = '  pure-class1'; $undefinedCount++ }
            $primName = TypeName $primaryCodes[$tile]
            $secName  = TypeName $secondaryCodes[$tile]
            $diff = ''
            $diffCount = 0
            for($i = 0; $i -lt 8; ++$i) {
                if($entry.Bm[$i] -ne $obs[$i]) {
                    $diff += $octants[$i].name + ' '
                    $diffCount++
                }
            }
            if($entry.Bm -eq $obs) {
                $out.Add(("PASS  bm={0} tile={1,3}  obs={2}  {3}/{4}{5}" -f `
                    $entry.Bm, $tile, $obs, $primName, $secName, $note))
                $passCount++
            }
            else {
                $out.Add(("FAIL  bm={0} tile={1,3}  obs={2}  diff={3}({4})  {5}/{6}{7}" -f `
                    $entry.Bm, $tile, $obs, $diff.TrimEnd(), $diffCount, $primName, $secName, $note))
                $failCount++
            }
        }
    }
    $out.Add('')
    $out.Add(("Summary: pass={0} fail={1}  (pure-class entries: {2})" -f $passCount, $failCount, $undefinedCount))
    $out.Add('')
}

[System.IO.File]::WriteAllLines($OutFile, $out)
Write-Output "wrote=$OutFile"
