<#
.SYNOPSIS
Regenerate Run/random.map by re-running the JS pipeline (cscript) and patching
the binary tile data, while keeping the original header (base/sub/dimensions).

We use the existing random.map as a header template since the seed/profile
won't change — only the tile IDs that fall out of the rule data.
#>

param(
    [uint32]$Seed = 1968570037,
    [int]$Width = 96,
    [int]$Height = 72,
    [string]$Profile = 'ice_outpost',
    [string]$MapPath = (Join-Path (Split-Path -Parent $PSScriptRoot) 'Run/random.map')
)

$ErrorActionPreference = 'Stop'

if(-not (Test-Path -LiteralPath $MapPath)) {
    throw "Template map missing: $MapPath"
}

$projectRoot = Split-Path -Parent $PSScriptRoot
$dumper = Join-Path $projectRoot 'Tools/DumpGeneratedIceMapTiles.js'

Push-Location $projectRoot
try {
    $output = & cscript //nologo $dumper "--seeds=$Seed" "--width=$Width" "--height=$Height" "--profile=$Profile"
} finally {
    Pop-Location
}

$lines = @($output | Where-Object { $_ -and $_.Trim().Length -gt 0 })
$beginIndex = -1
for($i = 0; $i -lt $lines.Length; ++$i) {
    if($lines[$i] -match "^begin generated_$Seed ") { $beginIndex = $i; break }
}
if($beginIndex -lt 0) { throw "Dumper did not return tiles for seed $Seed" }

$tiles = New-Object 'int[]' ($Width * $Height)
for($row = 0; $row -lt $Height; ++$row) {
    $line = $lines[$beginIndex + 1 + $row]
    $values = $line.Split(',')
    if($values.Length -lt $Width) { throw "Row $row had $($values.Length) values, expected >= $Width" }
    for($col = 0; $col -lt $Width; ++$col) {
        $tiles[$row * $Width + $col] = [int]$values[$col]
    }
}

$bytes = [System.IO.File]::ReadAllBytes($MapPath)
$expected = 0x60 + ($Width * $Height * 2)
if($bytes.Length -lt $expected) {
    throw "Existing map too small ($($bytes.Length) bytes; expected $expected)"
}

for($row = 0; $row -lt $Height; ++$row) {
    for($col = 0; $col -lt $Width; ++$col) {
        $offset = 0x60 + (($row * $Width) + $col) * 2
        $tile = $tiles[$row * $Width + $col]
        $bytes[$offset]     = [byte](($tile -shr 8) -band 0xFF)
        $bytes[$offset + 1] = [byte]($tile -band 0xFF)
    }
}

[System.IO.File]::WriteAllBytes($MapPath, $bytes)
Write-Output ("rewrote=$MapPath size=$($bytes.Length) tilesPatched=$($Width * $Height)")
