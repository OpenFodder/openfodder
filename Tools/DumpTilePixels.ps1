<#
.SYNOPSIS
Dump 16x16 tiles as text grids of palette indices so snow vs water can be
seen at a glance without rendering a PNG.

.DESCRIPTION
Reads a base+sub BLK pair (RNC auto-unpacked) and prints each requested tile
as a 16x16 grid. Each cell shows the palette index (0..F hex). Optionally
also prints a visual "shape" pane where SNOW (low palette index) becomes '#'
and WATER (high palette index) becomes '.', controlled by -SnowMax (cells
with index <= SnowMax are treated as snow).

.PARAMETER TileIds
List of tile IDs to dump.

.PARAMETER BaseBlk / SubBlk
Paths to ICEBASE.BLK / ICESUB0.BLK (or other terrain). Default to the Amiga
ice tileset under D:\Games\OpenFodder\Data\Amiga.

.PARAMETER SnowMax
Palette index threshold for the "shape" pane. Index <= SnowMax = snow ('#'),
otherwise water ('.'). Default 3 (matches the empirical Amiga ice palette).

.PARAMETER OutFile
Optional output path. Defaults to stdout.
#>
param(
    [Parameter(Mandatory = $true)][int[]]$TileIds,
    [string]$BaseBlk = 'D:\Games\OpenFodder\Data\Amiga\ICEBASE.BLK',
    [string]$SubBlk  = 'D:\Games\OpenFodder\Data\Amiga\ICESUB0.BLK',
    [int]$SnowMax = 3,
    [string]$OutFile
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

$lines = New-Object System.Collections.Generic.List[string]
foreach($id in $TileIds) {
    $px = Decode-Tile $blk $id
    if($null -eq $px) {
        $lines.Add(("Tile {0:D3}: <out of range>" -f $id))
        continue
    }
    $lines.Add(("Tile {0:D3}  (snow if pal<={1})" -f $id, $SnowMax))
    $lines.Add("       palette                shape")
    $lines.Add("       0123456789012345        0123456789012345")
    for($row = 0; $row -lt 16; ++$row) {
        $palLine = ''
        $shapeLine = ''
        for($col = 0; $col -lt 16; ++$col) {
            $v = [int]$px[$row * 16 + $col]
            $palLine += '{0:X}' -f $v
            if($v -le $SnowMax) { $shapeLine += '#' } else { $shapeLine += '.' }
        }
        $lines.Add(("   {0,2} {1}        {2}" -f $row, $palLine, $shapeLine))
    }
    $lines.Add('')
}

if($OutFile) {
    [System.IO.File]::WriteAllLines($OutFile, $lines)
    Write-Output ("wrote=$OutFile tiles=$($TileIds.Count)")
} else {
    foreach($l in $lines) { Write-Output $l }
}
