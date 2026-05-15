<#
.SYNOPSIS
Patch Run/random.map's tileset header bytes (0x00 base, 0x10 sub) to point at
ICEBASE.BLK / ICESUB0.BLK so RenderMapWithTileIds picks the ice palette.

Tile dimensions and tile data are untouched — this only swaps the BLK names so
the renderer reaches for the correct tilesheet.
#>

param(
    [string]$MapPath = (Join-Path (Split-Path -Parent $PSScriptRoot) 'Run/random.map')
)

$ErrorActionPreference = 'Stop'

if(-not (Test-Path -LiteralPath $MapPath)) {
    throw "Map missing: $MapPath"
}

$bytes = [IO.File]::ReadAllBytes($MapPath)

function Write-AsciiAt {
    param([byte[]]$Buffer, [int]$Offset, [string]$Text, [int]$Length)
    $padded = $Text.PadRight($Length, [char]0)
    for($i = 0; $i -lt $Length; ++$i) {
        $Buffer[$Offset + $i] = [byte][int]$padded[$i]
    }
}

Write-AsciiAt -Buffer $bytes -Offset 0x00 -Text 'ICEBASE.BLK' -Length 11
Write-AsciiAt -Buffer $bytes -Offset 0x10 -Text 'ICESUB0.BLK' -Length 11

[IO.File]::WriteAllBytes($MapPath, $bytes)
Write-Output ("patched=$MapPath base=ICEBASE.BLK sub=ICESUB0.BLK")
