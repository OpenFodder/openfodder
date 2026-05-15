param(
    [string]$DataRoot = "D:\Games\OpenFodder\Data",
    [string[]]$Terrains = @("jungle", "desert", "ice", "moors", "interior", "amigaFormat"),
    [int[]]$Seeds = @(101, 202, 303, 404, 505),
    [int]$Width = 64,
    [int]$Height = 48,
    [string]$ProfileName = "classic_jungle",
    [int]$TopCount = 12,
    [int]$MinRawPairCount = 3,
    [int]$MinGeneratedPairCount = 3,
    [string]$ProfileJsonPath = "",
    [string]$OutputPath = "",
    [string[]]$GeneratedOnlyTileAllowances = @(
        "jungle:241,242,243,244,245,261,262,263,264,265,266,280,281,282,283,284,285,286,304,305,306,326,346,360,361,362,363,364,365,366,380,381,382,383,384,386",
        "ice:264,265,267,284,285,287"
    ),
    [int]$MaxUnsupportedPairs = -1,
    [int]$MaxUnsupportedUses = -1,
    [double]$MaxUnsupportedRatePercent = -1,
    [int]$MaxLowSupportPairs = -1,
    [int]$MaxGeneratedOnlyTiles = -1,
    [int]$MaxIsolatedTileKinds = -1,
    [switch]$KeepProfileJson,
    [switch]$FailOnUnsupported,
    [switch]$FailOnLowSupport,
    [switch]$FailOnGeneratedOnly,
    [switch]$FailOnThreshold,
    [switch]$UsePhase7Baselines
)

$ErrorActionPreference = "Stop"

$projectRoot = Split-Path -Parent $PSScriptRoot
$profileScript = Join-Path $PSScriptRoot "ProfileMapTilesets.ps1"
$dumpScript = Join-Path $PSScriptRoot "DumpGeneratedMapAdjacency.js"
$reportLines = New-Object System.Collections.Generic.List[string]

function Write-ReportLine([string]$Line) {
    $script:reportLines.Add($Line) | Out-Null
    Write-Output $Line
}

function ConvertTo-CountTable($Items) {
    $table = @{}

    foreach($item in @($Items)) {
        if($null -eq $item) {
            continue
        }

        $table[[string]$item.key] = [int]$item.count
    }

    return $table
}

function Ensure-GeneratedEntry($Table, [string]$Terrain) {
    if(-not $Table.ContainsKey($Terrain)) {
        $Table[$Terrain] = [ordered]@{
            Summary = $null
            Tiles = @{}
            Pairs = @{}
            Isolated = @{}
            Maps = @()
        }
    }

    return $Table[$Terrain]
}

function Parse-KeyValueLine([string]$Line) {
    $result = [ordered]@{}
    $parts = $Line -split "\s+"

    if($parts.Length -gt 0) {
        $result["_type"] = $parts[0]
    }

    for($index = 1; $index -lt $parts.Length; ++$index) {
        $separator = $parts[$index].IndexOf("=")

        if($separator -lt 1) {
            continue
        }

        $key = $parts[$index].Substring(0, $separator)
        $value = $parts[$index].Substring($separator + 1)
        $result[$key] = $value
    }

    return $result
}

function Sum-Counts($Table) {
    $sum = 0

    foreach($value in $Table.Values) {
        $sum += [int]$value
    }

    return $sum
}

function New-Finding([string]$Key, [int]$Count, [int]$RawCount, [int]$Total) {
    $rate = 0
    if($Total -gt 0) {
        $rate = [Math]::Round(($Count / $Total) * 100, 4)
    }

    [pscustomobject]@{
        Key = $Key
        Count = $Count
        RawCount = $RawCount
        RatePercent = $rate
    }
}

function Top-Findings($Findings, [int]$Count) {
    if($null -eq $Findings -or $Findings.Count -eq 0) {
        return @()
    }

    @($Findings |
        Where-Object { $null -ne $_ -and $_.Key } |
        Sort-Object @{ Expression = { $_.Count }; Descending = $true }, @{ Expression = { $_.Key }; Descending = $false } |
        Select-Object -First $Count)
}

function Format-Findings($Findings) {
    $items = @($Findings | Where-Object { $null -ne $_ -and $_.Key })

    if($items.Count -eq 0) {
        return "none"
    }

    return (($items | ForEach-Object {
        if($_.RawCount -gt 0) {
            "$($_.Key)=$($_.Count) raw=$($_.RawCount)"
        }
        else {
            "$($_.Key)=$($_.Count)"
        }
    }) -join ", ")
}

function Build-ProfileJsonPath {
    if($ProfileJsonPath) {
        return $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($ProfileJsonPath)
    }

    $tempRoot = Join-Path ([IO.Path]::GetTempPath()) "openfodder-mapgen"
    if(-not (Test-Path -LiteralPath $tempRoot)) {
        New-Item -ItemType Directory -Path $tempRoot -Force | Out-Null
    }

    return Join-Path $tempRoot "authored-map-tile-profile.json"
}

function Build-AllowanceTable($Items) {
    $table = @{}

    foreach($item in @($Items)) {
        if([string]::IsNullOrWhiteSpace($item)) {
            continue
        }

        $parts = [string]$item -split ":", 2
        if($parts.Length -ne 2) {
            continue
        }

        $terrain = [string]$parts[0]
        if($terrain -eq "afx") {
            $terrain = "amigaFormat"
        }

        if(-not $table.ContainsKey($terrain)) {
            $table[$terrain] = @{}
        }

        foreach($tile in ($parts[1] -split ",")) {
            $key = ([string]$tile).Trim()
            if($key) {
                $table[$terrain][$key] = $true
            }
        }
    }

    return $table
}

function Is-AllowedGeneratedOnlyTile($AllowanceTable, [string]$Terrain, [string]$Tile) {
    return $AllowanceTable.ContainsKey($Terrain) -and $AllowanceTable[$Terrain].ContainsKey($Tile)
}

function Build-Phase7BaselineTable {
    $table = @{}

    $table["jungle"] = @{
        UnsupportedPairs = 223
        UnsupportedUses = 1240
        UnsupportedRatePercent = 4.12
        LowSupportPairs = 33
        GeneratedOnlyTiles = 0
    }
    $table["desert"] = @{
        UnsupportedPairs = 0
        UnsupportedUses = 0
        UnsupportedRatePercent = 0
        LowSupportPairs = 1
        GeneratedOnlyTiles = 0
    }
    $table["ice"] = @{
        UnsupportedPairs = 347
        UnsupportedUses = 4500
        UnsupportedRatePercent = 14.92
        LowSupportPairs = 50
        GeneratedOnlyTiles = 0
    }
    $table["moors"] = @{
        UnsupportedPairs = 0
        UnsupportedUses = 0
        UnsupportedRatePercent = 0
        LowSupportPairs = 0
        GeneratedOnlyTiles = 0
    }
    $table["interior"] = @{
        UnsupportedPairs = 0
        UnsupportedUses = 0
        UnsupportedRatePercent = 0
        LowSupportPairs = 0
        GeneratedOnlyTiles = 0
    }
    $table["amigaFormat"] = @{
        UnsupportedPairs = 0
        UnsupportedUses = 0
        UnsupportedRatePercent = 0
        LowSupportPairs = 0
        GeneratedOnlyTiles = 0
    }

    return $table
}

function Invoke-GeneratedDump([string]$SeedArg, [string]$TerrainArg) {
    Push-Location $projectRoot
    try {
        $lines = & cscript //nologo $dumpScript "--seeds=$SeedArg" "--terrains=$TerrainArg" "--width=$Width" "--height=$Height" "--profile=$ProfileName" 2>&1

        if($LASTEXITCODE -ne 0) {
            throw "generated adjacency dump failed with exit code $LASTEXITCODE`n$($lines -join [Environment]::NewLine)"
        }

        return @($lines)
    }
    finally {
        Pop-Location
    }
}

$resolvedProfileJsonPath = Build-ProfileJsonPath
$generatedOnlyAllowancesByTerrain = Build-AllowanceTable $GeneratedOnlyTileAllowances
$phase7BaselinesByTerrain = Build-Phase7BaselineTable

if(-not $ProfileJsonPath -or -not (Test-Path -LiteralPath $resolvedProfileJsonPath)) {
    & powershell -NoProfile -ExecutionPolicy Bypass -File $profileScript -DataRoot $DataRoot -TopCount ([Math]::Max($TopCount, 32)) -JsonPath $resolvedProfileJsonPath | Out-Null

    if($LASTEXITCODE -ne 0) {
        throw "authored tile profile export failed with exit code $LASTEXITCODE"
    }
}

$profileData = Get-Content -LiteralPath $resolvedProfileJsonPath -Raw | ConvertFrom-Json
$profileByTerrain = @{}

foreach($profile in @($profileData.profiles)) {
    if([string]$profile.sub -ne "0") {
        continue
    }

    $profileByTerrain[[string]$profile.terrain] = [ordered]@{
        Key = [string]$profile.key
        Tiles = ConvertTo-CountTable $profile.tileCounts
        Pairs = ConvertTo-CountTable $profile.adjacencyCounts
        RawMaps = [int]$profile.maps
        RawTiles = [int]$profile.tiles
    }
}

$generatedByTerrain = @{}
$generatedLines = Invoke-GeneratedDump (($Seeds | ForEach-Object { [string]$_ }) -join ",") (($Terrains | ForEach-Object { [string]$_ }) -join ",")

foreach($line in $generatedLines) {
    if([string]::IsNullOrWhiteSpace($line)) {
        continue
    }

    $values = Parse-KeyValueLine $line
    $terrain = [string]$values["terrain"]

    if(-not $terrain) {
        continue
    }

    $entry = Ensure-GeneratedEntry $generatedByTerrain $terrain

    switch([string]$values["_type"]) {
        "summary" {
            $entry.Summary = $values
        }
        "map" {
            $entry.Maps += $values
        }
        "tile" {
            $entry.Tiles[[string]$values["key"]] = [int]$values["count"]
        }
        "pair" {
            $entry.Pairs[[string]$values["key"]] = [int]$values["count"]
        }
        "isolated" {
            $entry.Isolated[[string]$values["key"]] = [int]$values["count"]
        }
    }
}

$hadFailure = $false

Write-ReportLine "Generated map adjacency report"
Write-ReportLine "profileJson=$resolvedProfileJsonPath dataRoot=$DataRoot seeds=$(($Seeds -join ',')) size=${Width}x${Height} profile=$ProfileName generatedOnlyAllowances=$(($GeneratedOnlyTileAllowances -join ';'))"
Write-ReportLine ""

foreach($terrain in $Terrains) {
    $canonicalTerrain = if($terrain -eq "afx") { "amigaFormat" } else { $terrain }
    $generated = $generatedByTerrain[$canonicalTerrain]
    $authored = $profileByTerrain[$canonicalTerrain]

    if(-not $generated) {
        Write-ReportLine "[$canonicalTerrain] no generated data"
        continue
    }

    if(-not $authored) {
        Write-ReportLine "[$canonicalTerrain] no authored profile data"
        continue
    }

    $totalPairs = Sum-Counts $generated.Pairs
    $unsupportedPairs = New-Object System.Collections.Generic.List[object]
    $lowSupportPairs = New-Object System.Collections.Generic.List[object]
    $generatedOnlyTiles = New-Object System.Collections.Generic.List[object]
    $allowedGeneratedOnlyTiles = New-Object System.Collections.Generic.List[object]
    $isolatedTiles = New-Object System.Collections.Generic.List[object]

    foreach($pair in $generated.Pairs.Keys) {
        $count = [int]$generated.Pairs[$pair]

        if(-not $authored.Pairs.ContainsKey($pair)) {
            $unsupportedPairs.Add((New-Finding $pair $count 0 $totalPairs)) | Out-Null
            continue
        }

        $rawCount = [int]$authored.Pairs[$pair]
        if($rawCount -le $MinRawPairCount -and $count -ge $MinGeneratedPairCount) {
            $lowSupportPairs.Add((New-Finding $pair $count $rawCount $totalPairs)) | Out-Null
        }
    }

    foreach($tile in $generated.Tiles.Keys) {
        if(-not $authored.Tiles.ContainsKey($tile)) {
            $finding = New-Finding $tile ([int]$generated.Tiles[$tile]) 0 ([int]$generated.Summary["cells"])

            if(Is-AllowedGeneratedOnlyTile $generatedOnlyAllowancesByTerrain $canonicalTerrain $tile) {
                $allowedGeneratedOnlyTiles.Add($finding) | Out-Null
            }
            else {
                $generatedOnlyTiles.Add($finding) | Out-Null
            }
        }
    }

    foreach($tile in $generated.Isolated.Keys) {
        $isolatedTiles.Add((New-Finding $tile ([int]$generated.Isolated[$tile]) 0 ([int]$generated.Summary["cells"]))) | Out-Null
    }

    $unsupportedUseCount = 0
    foreach($finding in $unsupportedPairs) {
        $unsupportedUseCount += [int]$finding.Count
    }
    $unsupportedRate = 0
    if($totalPairs -gt 0) {
        $unsupportedRate = [Math]::Round(($unsupportedUseCount / $totalPairs) * 100, 4)
    }

    if($FailOnUnsupported -and $unsupportedPairs.Count -gt 0) {
        $hadFailure = $true
    }
    if($FailOnLowSupport -and $lowSupportPairs.Count -gt 0) {
        $hadFailure = $true
    }
    if($FailOnGeneratedOnly -and $generatedOnlyTiles.Count -gt 0) {
        $hadFailure = $true
    }
    $terrainMaxUnsupportedPairs = $MaxUnsupportedPairs
    $terrainMaxUnsupportedUses = $MaxUnsupportedUses
    $terrainMaxUnsupportedRatePercent = $MaxUnsupportedRatePercent
    $terrainMaxLowSupportPairs = $MaxLowSupportPairs
    $terrainMaxGeneratedOnlyTiles = $MaxGeneratedOnlyTiles

    if($UsePhase7Baselines -and $phase7BaselinesByTerrain.ContainsKey($canonicalTerrain)) {
        $baseline = $phase7BaselinesByTerrain[$canonicalTerrain]
        if($terrainMaxUnsupportedPairs -lt 0) {
            $terrainMaxUnsupportedPairs = [int]$baseline.UnsupportedPairs
        }
        if($terrainMaxUnsupportedUses -lt 0) {
            $terrainMaxUnsupportedUses = [int]$baseline.UnsupportedUses
        }
        if($terrainMaxUnsupportedRatePercent -lt 0) {
            $terrainMaxUnsupportedRatePercent = [double]$baseline.UnsupportedRatePercent
        }
        if($terrainMaxLowSupportPairs -lt 0) {
            $terrainMaxLowSupportPairs = [int]$baseline.LowSupportPairs
        }
        if($terrainMaxGeneratedOnlyTiles -lt 0) {
            $terrainMaxGeneratedOnlyTiles = [int]$baseline.GeneratedOnlyTiles
        }
    }

    if($FailOnThreshold -or $UsePhase7Baselines) {
        if($terrainMaxUnsupportedPairs -ge 0 -and $unsupportedPairs.Count -gt $terrainMaxUnsupportedPairs) {
            $hadFailure = $true
        }
        if($terrainMaxUnsupportedUses -ge 0 -and $unsupportedUseCount -gt $terrainMaxUnsupportedUses) {
            $hadFailure = $true
        }
        if($terrainMaxUnsupportedRatePercent -ge 0 -and $unsupportedRate -gt $terrainMaxUnsupportedRatePercent) {
            $hadFailure = $true
        }
        if($terrainMaxLowSupportPairs -ge 0 -and $lowSupportPairs.Count -gt $terrainMaxLowSupportPairs) {
            $hadFailure = $true
        }
        if($terrainMaxGeneratedOnlyTiles -ge 0 -and $generatedOnlyTiles.Count -gt $terrainMaxGeneratedOnlyTiles) {
            $hadFailure = $true
        }
        if($MaxIsolatedTileKinds -ge 0 -and $isolatedTiles.Count -gt $MaxIsolatedTileKinds) {
            $hadFailure = $true
        }
    }

    Write-ReportLine "[$canonicalTerrain] generatedMaps=$($generated.Summary["maps"]) rawMaps=$($authored.RawMaps) backend=$($generated.Summary["backends"]) uniquePairs=$($generated.Summary["uniquePairs"]) unsupportedPairs=$($unsupportedPairs.Count) unsupportedUses=$unsupportedUseCount unsupportedRate=${unsupportedRate}% lowSupportPairs=$($lowSupportPairs.Count) generatedOnlyTiles=$($generatedOnlyTiles.Count) allowedGeneratedOnlyTiles=$($allowedGeneratedOnlyTiles.Count) isolatedTileKinds=$($isolatedTiles.Count)"
    Write-ReportLine "  top unsupported pairs: $(Format-Findings (Top-Findings $unsupportedPairs $TopCount))"
    Write-ReportLine "  top low-support pairs: $(Format-Findings (Top-Findings $lowSupportPairs $TopCount))"
    Write-ReportLine "  generated-only tiles: $(Format-Findings (Top-Findings $generatedOnlyTiles $TopCount))"
    Write-ReportLine "  allowed generated-only tiles: $(Format-Findings (Top-Findings $allowedGeneratedOnlyTiles $TopCount))"
    Write-ReportLine "  isolated tile counts: $(Format-Findings (Top-Findings $isolatedTiles $TopCount))"
    Write-ReportLine ""
}

if($OutputPath) {
    $resolvedOutputPath = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($OutputPath)
    $outputFolder = Split-Path -Parent $resolvedOutputPath

    if($outputFolder -and -not (Test-Path -LiteralPath $outputFolder)) {
        New-Item -ItemType Directory -Path $outputFolder -Force | Out-Null
    }

    $reportLines | Set-Content -LiteralPath $resolvedOutputPath -Encoding UTF8
}

if(-not $KeepProfileJson -and -not $ProfileJsonPath -and (Test-Path -LiteralPath $resolvedProfileJsonPath)) {
    Remove-Item -LiteralPath $resolvedProfileJsonPath -Force
}

if($hadFailure) {
    exit 1
}
