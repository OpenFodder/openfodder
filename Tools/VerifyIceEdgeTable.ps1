# Sanity-check the generated IceTileEdges.js used by the Wang-Tile edge matcher.
#
#   - byCenter[c] is non-empty for c in {snow, ice, shallow, deep}
#   - canonical tile ids appear in exactly one byCenter bucket
#   - all expected seam class pairs have at least one tile in at least one direction
#
# Exits non-zero on any coverage gap so it can be wired into CI.

$ErrorActionPreference = "Stop"

$here = Split-Path -Parent $MyInvocation.MyCommand.Path
$root = Split-Path -Parent $here
$jsPath = Join-Path $root "Run\Scripts\MapGen\Terrain\Smoothing\Data\IceTileEdges.js"

if (-not (Test-Path $jsPath)) {
    Write-Error "IceTileEdges.js not found at $jsPath. Run 'py Tools\BuildIceEdgeTable.py' first."
    exit 1
}

$text = Get-Content $jsPath -Raw

function Parse-CenterBucket([string]$cls, [string]$body) {
    $pattern = "(?ms)^\s*$cls\s*:\s*\[(.*?)\]"
    $m = [regex]::Match($body, $pattern)
    if (-not $m.Success) { return @() }
    $inside = $m.Groups[1].Value
    return ($inside -split ",") |
        ForEach-Object { $_.Trim() } |
        Where-Object { $_ -ne "" } |
        ForEach-Object { [int]$_ }
}

function Parse-SeamPair([string]$key, [string]$body) {
    $escaped = [regex]::Escape($key)
    $pattern = "(?ms)""$escaped""\s*:\s*\{(.*?)\}\s*[,}]"
    $m = [regex]::Match($body, $pattern)
    if (-not $m.Success) { return $null }
    $rec = @{}
    foreach ($edge in @("N", "E", "S", "W")) {
        $em = [regex]::Match($m.Groups[1].Value, "(?m)^\s*$edge\s*:\s*\[(.*?)\]")
        if ($em.Success) {
            $rec[$edge] = ($em.Groups[1].Value -split ",") |
                ForEach-Object { $_.Trim() } |
                Where-Object { $_ -ne "" } |
                ForEach-Object { [int]$_ }
        } else {
            $rec[$edge] = @()
        }
    }
    return $rec
}

$byCenter = @{
    snow = Parse-CenterBucket "snow" $text
    ice = Parse-CenterBucket "ice" $text
    shallow = Parse-CenterBucket "shallow" $text
    deep = Parse-CenterBucket "deep" $text
}

$problems = @()

foreach ($cls in @("snow", "ice", "shallow", "deep")) {
    $count = $byCenter[$cls].Count
    if ($count -eq 0) {
        $problems += "byCenter[$cls] is empty"
    } else {
        Write-Host ("byCenter[{0,-7}] = {1} tiles" -f $cls, $count)
    }
}

# Verify canonical tile ids appear in exactly one bucket.
$seen = @{}
foreach ($cls in @("snow", "ice", "shallow", "deep")) {
    foreach ($id in $byCenter[$cls]) {
        if ($seen.ContainsKey($id)) {
            $problems += "tile $id is in multiple byCenter buckets ($($seen[$id]) and $cls)"
        } else {
            $seen[$id] = $cls
        }
    }
}

# Verify every expected seam pair has tiles in at least one direction.
$expectedSeams = @("deep|shallow", "ice|shallow", "ice|snow", "shallow|snow")
foreach ($key in $expectedSeams) {
    $rec = Parse-SeamPair $key $text
    if ($null -eq $rec) {
        $problems += "bySeam[$key] missing"
        continue
    }
    $total = ($rec.N.Count + $rec.E.Count + $rec.S.Count + $rec.W.Count)
    if ($total -eq 0) {
        $problems += "bySeam[$key] has no tiles in any direction"
    } else {
        Write-Host ("bySeam[{0,-15}] N={1} E={2} S={3} W={4}" -f $key, $rec.N.Count, $rec.E.Count, $rec.S.Count, $rec.W.Count)
    }
}

# Tile 98 is the motivating example: its W edge is mostly water, so it must
# live in the shallow bucket (centre = shallow) and the deep|shallow seam set.
if ($seen[98] -ne "shallow") {
    $problems += "tile 98 expected center=shallow, got center=$($seen[98])"
}

if ($problems.Count -gt 0) {
    Write-Host ""
    Write-Host "FAIL ($($problems.Count) issues):" -ForegroundColor Red
    foreach ($p in $problems) { Write-Host "  - $p" -ForegroundColor Red }
    exit 1
}

Write-Host ""
Write-Host "OK" -ForegroundColor Green
exit 0
