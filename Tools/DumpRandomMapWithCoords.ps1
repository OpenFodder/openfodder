<#
.SYNOPSIS
Generate an ice map via the new Wang-Tile edge matcher and write both the tile
grid and a (x,y) -> tile-id listing alongside the char map.

Output files (under Documentation/):
  - RandomMap_<seed>_grid.txt    full CSV-per-row grid
  - RandomMap_<seed>_coords.txt  one cell per line: x  y  char  tileId
  - RandomMap_<seed>_chars.txt   smoothed char map
#>

param(
    [uint32]$Seed = 1968570037,
    [int]$Width = 64,
    [int]$Height = 48,
    [string]$Profile = 'ice_outpost'
)

$ErrorActionPreference = 'Stop'

$projectRoot = Split-Path -Parent $PSScriptRoot
$dumper = Join-Path $projectRoot 'Tools/DumpGeneratedIceMapTiles.js'
$docDir = Join-Path $projectRoot 'Documentation'
if (-not (Test-Path $docDir)) { New-Item -ItemType Directory -Path $docDir | Out-Null }

Push-Location $projectRoot
try {
    $output = & cscript //nologo $dumper "--seeds=$Seed" "--width=$Width" "--height=$Height" "--profile=$Profile"
} finally {
    Pop-Location
}

$lines = @($output | Where-Object { $_ -and $_.Trim().Length -gt 0 })

$beginIdx = -1
for ($i = 0; $i -lt $lines.Length; ++$i) {
    if ($lines[$i] -match "^begin generated_$Seed ") { $beginIdx = $i; break }
}
if ($beginIdx -lt 0) { throw "Dumper did not emit tiles for seed $Seed" }

$tileGrid = New-Object 'int[,]' $Height, $Width
for ($row = 0; $row -lt $Height; ++$row) {
    $values = $lines[$beginIdx + 1 + $row].Split(',')
    if ($values.Length -lt $Width) { throw "Row $row produced $($values.Length) tiles, expected $Width" }
    for ($col = 0; $col -lt $Width; ++$col) {
        $tileGrid[$row, $col] = [int]$values[$col]
    }
}

$gridPath = Join-Path $docDir "RandomMap_${Seed}_grid.txt"
$coordPath = Join-Path $docDir "RandomMap_${Seed}_coords.txt"
$charPath = Join-Path $docDir "RandomMap_${Seed}_chars.txt"

$gridLines = New-Object System.Collections.Generic.List[string]
$gridLines.Add("# seed=$Seed profile=$Profile size=${Width}x${Height}")
$gridLines.Add("# tile id grid (one row per line, comma-separated)")
for ($row = 0; $row -lt $Height; ++$row) {
    $rowVals = for ($col = 0; $col -lt $Width; ++$col) { $tileGrid[$row, $col] }
    $gridLines.Add(($rowVals -join ','))
}
[System.IO.File]::WriteAllLines($gridPath, $gridLines)

# Char map: the dumper writes Run/icecharmap_<seed>.txt; copy/parse it for the
# coord listing so we can include the smoothed char alongside each tile id.
$charSrc = Join-Path $projectRoot "Run/icecharmap_${Seed}.txt"
$charGrid = $null
if (Test-Path $charSrc) {
    # Header lines start with "# " (hash space). Map data also starts with '#'
    # because '#' is the snow/ground char. Distinguish by the trailing space.
    $rawChars = [System.IO.File]::ReadAllLines($charSrc) | Where-Object { -not $_.StartsWith('# ') }
    if ($rawChars.Length -ge $Height) {
        $charGrid = New-Object 'char[,]' $Height, $Width
        for ($row = 0; $row -lt $Height; ++$row) {
            $rowText = $rawChars[$row]
            for ($col = 0; $col -lt $Width; ++$col) {
                if ($col -lt $rowText.Length) { $charGrid[$row, $col] = $rowText[$col] }
                else { $charGrid[$row, $col] = '?' }
            }
        }
        Copy-Item -Force $charSrc $charPath
    }
}

$coordLines = New-Object System.Collections.Generic.List[string]
$coordLines.Add("# seed=$Seed profile=$Profile size=${Width}x${Height}")
$coordLines.Add("# columns: x y char tile")
for ($row = 0; $row -lt $Height; ++$row) {
    for ($col = 0; $col -lt $Width; ++$col) {
        $ch = if ($null -ne $charGrid) { $charGrid[$row, $col] } else { '?' }
        $coordLines.Add(("{0,3} {1,3} {2} {3,4}" -f $col, $row, $ch, $tileGrid[$row, $col]))
    }
}
[System.IO.File]::WriteAllLines($coordPath, $coordLines)

# Tile usage summary
$counts = @{}
for ($row = 0; $row -lt $Height; ++$row) {
    for ($col = 0; $col -lt $Width; ++$col) {
        $t = $tileGrid[$row, $col]
        if ($counts.ContainsKey($t)) { $counts[$t]++ } else { $counts[$t] = 1 }
    }
}

Write-Host "wrote $gridPath"
Write-Host "wrote $coordPath"
if (Test-Path $charPath) { Write-Host "wrote $charPath" }
Write-Host ""
Write-Host "tile usage (top 20):"
$counts.GetEnumerator() | Sort-Object Value -Descending | Select-Object -First 20 |
    ForEach-Object { "{0,4} x {1}" -f $_.Value, $_.Key } | Write-Host
