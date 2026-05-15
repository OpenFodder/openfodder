<#
.SYNOPSIS
Dump water-family ice tiles in a shape-friendly text format. Distinguishes
deep water (palette 8 -> 'D') from shallow water (palette 7 -> 'S'), and
also shows the original Sensible HIT/BHT data for each tile so we can see
whether the authored hit/bht tables already classify deep vs shallow.

.PARAMETER TileIds
List of tile IDs to dump. Defaults to the requested water-tile ranges.

.PARAMETER BaseBlk / SubBlk
Paths to ICEBASE.BLK / ICESUB0.BLK (decoded for pixels).

.PARAMETER BaseHit / SubHit / BaseBht / SubBht
Paths to the matching .HIT / .BHT tables.

.PARAMETER OutFile
Optional output path. Defaults to stdout.
#>
param(
    [int[]]$TileIds,
    [string]$BaseBlk = 'Run/Data/Amiga/ICEBASE.BLK',
    [string]$SubBlk  = 'Run/Data/Amiga/ICESUB0.BLK',
    [string]$OutFile
)

$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'Lib/RncUnpack.ps1')

if(-not $TileIds -or $TileIds.Count -eq 0) {
    $TileIds = @()
    $TileIds += 100..111
    $TileIds += 114..119
    $TileIds += 132..139
    $TileIds += 156..159
    $TileIds += 176..179
}

$baseBlkBytes = Read-MaybeRnc -Path $BaseBlk
$subBlkBytes  = Read-MaybeRnc -Path $SubBlk
$blk = New-Object byte[] ($baseBlkBytes.Length + $subBlkBytes.Length)
[Array]::Copy($baseBlkBytes, 0, $blk, 0, $baseBlkBytes.Length)
[Array]::Copy($subBlkBytes,  0, $blk, $baseBlkBytes.Length, $subBlkBytes.Length)

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

function Classify([int]$pal) {
    if($pal -eq 8) { return 'D' }
    if($pal -eq 7) { return 'S' }
    if($pal -le 3 -and $pal -ge 1) { return '#' }
    return ('{0:X}' -f $pal)
}

# Compute per-cell shallow density 0..1 in a square window of given radius.
function Compute-Density([byte[]]$px, [int]$radius) {
    $isS = New-Object int[] 256
    for($i = 0; $i -lt 256; ++$i) {
        $v = [int]$px[$i]
        if($v -eq 7 -or ($v -le 3 -and $v -ge 1)) { $isS[$i] = 1 }
    }
    $density = New-Object double[] 256
    for($r = 0; $r -lt 16; ++$r) {
        for($c = 0; $c -lt 16; ++$c) {
            $sum = 0
            $count = 0
            for($dr = -$radius; $dr -le $radius; ++$dr) {
                $rr = $r + $dr
                if($rr -lt 0 -or $rr -ge 16) { continue }
                for($dc = -$radius; $dc -le $radius; ++$dc) {
                    $cc = $c + $dc
                    if($cc -lt 0 -or $cc -ge 16) { continue }
                    $sum += $isS[$rr * 16 + $cc]
                    $count++
                }
            }
            $density[$r * 16 + $c] = $sum / [double]$count
        }
    }
    return $density
}

function Density-To-Digits([double[]]$density) {
    $out = New-Object char[] 256
    for($i = 0; $i -lt 256; ++$i) {
        $digit = [int][Math]::Floor($density[$i] * 10.0)
        if($digit -gt 9) { $digit = 9 }
        if($digit -eq 0) {
            $out[$i] = [char]'.'
        } else {
            $out[$i] = [char]([int][char]'0' + $digit)
        }
    }
    return $out
}

# Convert a density map to a clean binary S/D shape using a threshold.
# Default threshold 0.4 reads as: this 5x5 window is >=40% shallow -> "S".
function Density-To-Binary([double[]]$density, [double]$threshold = 0.4) {
    $out = New-Object char[] 256
    for($i = 0; $i -lt 256; ++$i) {
        if($density[$i] -ge $threshold) {
            $out[$i] = [char]'S'
        } else {
            $out[$i] = [char]'D'
        }
    }
    return $out
}

# Downsample 16x16 to a 4x4 block summary. Each block is one char encoding
# the shallow ratio: ' '(0%) / '.' / '-' / 'o' / '+' / '*' / 'S' (>=80%).
function Block-Shape([byte[]]$px) {
    $rows = New-Object string[] 4
    for($br = 0; $br -lt 4; ++$br) {
        $line = ''
        for($bc = 0; $bc -lt 4; ++$bc) {
            $sCount = 0
            for($r = 0; $r -lt 4; ++$r) {
                for($c = 0; $c -lt 4; ++$c) {
                    $v = [int]$px[(($br * 4) + $r) * 16 + ($bc * 4) + $c]
                    if($v -eq 7 -or ($v -le 3 -and $v -ge 1)) { $sCount++ }
                }
            }
            $pct = $sCount / 16.0
            if($pct -ge 0.85)      { $line += 'S' }
            elseif($pct -ge 0.65)  { $line += '*' }
            elseif($pct -ge 0.45)  { $line += '+' }
            elseif($pct -ge 0.25)  { $line += 'o' }
            elseif($pct -ge 0.10)  { $line += '-' }
            elseif($pct -gt 0)     { $line += '.' }
            else                   { $line += ' ' }
        }
        $rows[$br] = $line
    }
    return $rows
}

$lines = New-Object System.Collections.Generic.List[string]
$lines.Add('# Water/water-edge tile dump')
$lines.Add('# shape16-raw:    D = deep water (pal 8)   S = shallow (pal 7)   # = snow (pal 1-3)')
$lines.Add('# density-r=2:    digit = shallow density in 5x5 window (0-9). "." = 0%, "9" = 90-100%.')
$lines.Add('# smoothed:       density >= 0.4 -> S, else D. Clean shape derived from density.')
$lines.Add('# block4:         16x16 collapsed to 4x4 blocks. Each cell = shallow ratio for its 4x4 patch.')
$lines.Add('')

foreach($id in $TileIds) {
    $px = Decode-Tile $blk $id
    if($null -eq $px) {
        $lines.Add(("tile {0,3}  <out of range>" -f $id))
        continue
    }
    $deep = 0; $shallow = 0; $snow = 0; $other = 0
    for($i = 0; $i -lt 256; ++$i) {
        $v = [int]$px[$i]
        if($v -eq 8) { $deep++ }
        elseif($v -eq 7) { $shallow++ }
        elseif($v -le 3 -and $v -ge 1) { $snow++ }
        else { $other++ }
    }
    $density = Compute-Density $px 2
    $densityDigits = Density-To-Digits $density
    $shapeClean = Density-To-Binary $density 0.4
    $blocks = Block-Shape $px

    $lines.Add(("tile {0,3}  deep={1}  shallow={2}  snow={3}  other={4}  (block-shape ratio: '.'<10%  '-'<25%  'o'<45%  '+'<65%  '*'<85%  'S'>=85%)" -f `
        $id, $deep, $shallow, $snow, $other))
    $lines.Add('         shape16-raw       density-r=2 (0-9)  smoothed         block4')
    for($row = 0; $row -lt 16; ++$row) {
        $shapeLine = ''
        $densLine = ''
        $smoothLine = ''
        for($col = 0; $col -lt 16; ++$col) {
            $idx = $row * 16 + $col
            $v = [int]$px[$idx]
            $shapeLine += (Classify $v)
            $densLine += [string]$densityDigits[$idx]
            $smoothLine += [string]$shapeClean[$idx]
        }
        $blockCol = ''
        if(($row % 4) -eq 0) {
            $blockCol = $blocks[$row / 4]
        }
        $lines.Add(("         {0}  {1}  {2}  {3}" -f $shapeLine, $densLine, $smoothLine, $blockCol))
    }
    $lines.Add('')
}

if($OutFile) {
    [System.IO.File]::WriteAllLines($OutFile, $lines)
    Write-Output ("wrote=$OutFile tiles=$($TileIds.Count)")
} else {
    foreach($l in $lines) { Write-Output $l }
}
