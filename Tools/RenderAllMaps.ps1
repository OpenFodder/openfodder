<#
.SYNOPSIS
Renders tile-id-overlaid PNGs for every .map file under Run/Data.

.DESCRIPTION
Walks each campaign/data-set folder under -DataRoot, finds every .map, and invokes
RenderMapWithTileIds.ps1 against it. Output lands in MapDumps/<CampaignDirName>/<map>.tiles.png.

Maps in directories that don't have their .blk/.pal assets installed (e.g. Amiga, Dos_CD)
are skipped with a [skip] line — they're placeholder folders awaiting user-provided data.

.PARAMETER DataRoot
Top-level data folder. Defaults to Run/Data relative to the current directory.

.PARAMETER Scale
Pixel scale per tile, forwarded to the renderer.

.PARAMETER NoOverlay
Skip the tile-id overlay, forwarded to the renderer.

.EXAMPLE
./Tools/RenderAllMaps.ps1
./Tools/RenderAllMaps.ps1 -Scale 3
#>

param(
    [string]$DataRoot,
    [ValidateRange(1, 8)][int]$Scale = 2,
    [switch]$NoOverlay
)

$ErrorActionPreference = 'Stop'

if(-not $DataRoot) {
    $DataRoot = Join-Path (Get-Location).Path 'Run/Data'
}
if(-not (Test-Path -LiteralPath $DataRoot)) {
    throw "Data root not found: $DataRoot"
}

$renderer = Join-Path $PSScriptRoot 'RenderMapWithTileIds.ps1'
if(-not (Test-Path -LiteralPath $renderer)) {
    throw "Renderer script not found: $renderer"
}

# Two supported layouts:
#   parent-of-campaigns: $DataRoot contains subdirs each holding .map/.blk
#   single campaign:     $DataRoot itself holds .map/.blk
$rootHasMaps = Get-ChildItem -LiteralPath $DataRoot -File -Filter '*.map' -ErrorAction SilentlyContinue | Select-Object -First 1
if($rootHasMaps) {
    $campaigns = @(Get-Item -LiteralPath $DataRoot)
} else {
    $campaigns = Get-ChildItem -LiteralPath $DataRoot -Directory | Sort-Object Name
}

$totalMaps = 0
$rendered = 0
$skipped = 0
$failed = 0

foreach($campaign in $campaigns) {
    $maps = Get-ChildItem -LiteralPath $campaign.FullName -File -Filter '*.map' -ErrorAction SilentlyContinue | Sort-Object Name
    if(-not $maps) { continue }

    # Detect missing tileset assets up front so we don't spam errors per map.
    $hasBlk = (Get-ChildItem -LiteralPath $campaign.FullName -File -Filter '*.blk' -ErrorAction SilentlyContinue | Select-Object -First 1)
    if(-not $hasBlk) {
        Write-Output ("[skip] {0} ({1} maps, no .blk assets)" -f $campaign.Name, $maps.Count)
        $skipped += $maps.Count
        $totalMaps += $maps.Count
        continue
    }

    Write-Output ("[set ] {0} ({1} maps)" -f $campaign.Name, $maps.Count)

    foreach($map in $maps) {
        $totalMaps++
        $params = @{
            MapPath = $map.FullName
            Scale = $Scale
        }
        if($NoOverlay) { $params.NoOverlay = $true }

        try {
            $output = & $renderer @params 2>&1
            $wroteLine = $output | Where-Object { $_ -is [string] -and $_ -match '^wrote=' } | Select-Object -First 1
            if($wroteLine) {
                Write-Output ("  ok   {0,-20} {1}" -f $map.Name, $wroteLine)
                $rendered++
            } else {
                $errorText = ($output | ForEach-Object { $_.ToString() }) -join ' '
                if($errorText -match 'RNC-compressed') {
                    Write-Output ("  skip {0,-20} (RNC-compressed)" -f $map.Name)
                    $skipped++
                } else {
                    Write-Output ("  FAIL {0,-20} (no output)" -f $map.Name)
                    $output | ForEach-Object { Write-Output ("       $_") }
                    $failed++
                }
            }
        }
        catch {
            if($_.Exception.Message -match 'RNC-compressed') {
                Write-Output ("  skip {0,-20} (RNC-compressed)" -f $map.Name)
                $skipped++
            } else {
                Write-Output ("  FAIL {0,-20} {1}" -f $map.Name, $_.Exception.Message)
                $failed++
            }
        }
    }
}

Write-Output ""
Write-Output ("totalMaps=$totalMaps rendered=$rendered skipped=$skipped failed=$failed")
if($failed -gt 0) { exit 1 }
