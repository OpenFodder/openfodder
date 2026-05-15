<#
.SYNOPSIS
For each ice tile we care about, compute the average palette index per octant
(NW, N, NE, W, E, SW, S, SE). Higher index = lighter pixels = snow/ice.
Lower index = water/wet.

The tile is split into a 4x4 cell grid; the 8 outer cells map to the 8 octants
(corners + edges). The 4 center cells are ignored — they are usually a
mixture and don't represent the transition direction.

The output table tells us, for each tile, where the snow lives — which is
exactly what the bitmask encodes (bit=1 → ground/snow neighbor).
#>

param(
    [string]$BaseBlk = 'D:\Games\OpenFodder\Data\Amiga\ICEBASE.BLK',
    [string]$SubBlk  = 'D:\Games\OpenFodder\Data\Amiga\ICESUB0.BLK',
    [int[]]$TileIds  = @(80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,112,113,128,129,130,131,148,149,150,151)
)

$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'Lib/RncUnpack.ps1')

$baseBytes = Read-MaybeRnc -Path $BaseBlk
$subBytes  = Read-MaybeRnc -Path $SubBlk
$blk = New-Object byte[] ($baseBytes.Length + $subBytes.Length)
[Array]::Copy($baseBytes, 0, $blk, 0, $baseBytes.Length)
[Array]::Copy($subBytes,  0, $blk, $baseBytes.Length, $subBytes.Length)

function Decode-Tile([byte[]]$Blk, [int]$TileId) {
    $px = New-Object byte[] 256
    $offset = $TileId -shl 7
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

# Octant cell coordinates in a 4x4 grid (cellX, cellY) — each cell = 4x4 pixels.
# NW=top-left, N=top-mid, NE=top-right; W=mid-left, E=mid-right; SW,S,SE bottom.
$octants = @{
    'NW' = @(0, 0)
    'N1' = @(1, 0); 'N2' = @(2, 0)
    'NE' = @(3, 0)
    'W1' = @(0, 1); 'W2' = @(0, 2)
    'E1' = @(3, 1); 'E2' = @(3, 2)
    'SW' = @(0, 3)
    'S1' = @(1, 3); 'S2' = @(2, 3)
    'SE' = @(3, 3)
}

# Collapse to 8 directions by averaging multi-cell groups.
function Octant-Brightness([byte[]]$Px) {
    $r = @{}
    foreach($k in 'NW','NE','SW','SE') {
        $c = $octants[$k]; $cx = $c[0]; $cy = $c[1]
        $sum = 0
        for($py = 0; $py -lt 4; ++$py) {
            for($pxL = 0; $pxL -lt 4; ++$pxL) {
                $sum += $Px[(($cy * 4) + $py) * 16 + $cx * 4 + $pxL]
            }
        }
        $r[$k] = [math]::Round($sum / 16.0, 2)
    }
    foreach($pair in @('N','W','E','S')) {
        $cells = switch ($pair) {
            'N' { @('N1','N2') }
            'W' { @('W1','W2') }
            'E' { @('E1','E2') }
            'S' { @('S1','S2') }
        }
        $sum = 0
        foreach($k in $cells) {
            $c = $octants[$k]; $cx = $c[0]; $cy = $c[1]
            for($py = 0; $py -lt 4; ++$py) {
                for($pxL = 0; $pxL -lt 4; ++$pxL) {
                    $sum += $Px[(($cy * 4) + $py) * 16 + $cx * 4 + $pxL]
                }
            }
        }
        $r[$pair] = [math]::Round($sum / 32.0, 2)
    }
    return $r
}

Write-Output ("{0,5} | {1,5} {2,5} {3,5} | {4,5} {5,5} | {6,5} {7,5} {8,5} | bm" -f 'Tile','NW','N','NE','W','E','SW','S','SE')
Write-Output (("-" * 70))

# Threshold for "snow" vs "wet" — we'll classify each octant as 1 or 0
# by comparing to the tile's median brightness.
$rows = @()
foreach($id in $TileIds) {
    $tile = Decode-Tile $blk $id
    $oct = Octant-Brightness $tile
    $vals = @($oct['NW'], $oct['N'], $oct['NE'], $oct['W'], $oct['E'], $oct['SW'], $oct['S'], $oct['SE'])
    # Per-tile threshold = mean of all 8.
    $mean = ($vals | Measure-Object -Average).Average
    $bits = ''
    foreach($v in $vals) {
        if($v -gt $mean) { $bits += '1' } else { $bits += '0' }
    }
    Write-Output ("{0,5:D3} | {1,5} {2,5} {3,5} | {4,5} {5,5} | {6,5} {7,5} {8,5} | {9}" -f `
        $id,$oct['NW'],$oct['N'],$oct['NE'],$oct['W'],$oct['E'],$oct['SW'],$oct['S'],$oct['SE'],$bits)
}
