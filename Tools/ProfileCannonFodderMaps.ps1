param(
    [string]$DataRoot = "D:\Games\OpenFodder\Data\Amiga",
    [string]$Campaign = "D:\Games\OpenFodder\Campaigns\Cannon Fodder.ofc",
    [string]$JsonOut = "D:\Projects\openfodder\Tools\Generated\cf1-amiga-map-profiles.json",
    [string]$MarkdownOut = "D:\Projects\openfodder\Documentation\CannonFodderOriginalMapAnalysis.md",
    [int]$TileSize = 16,
    [int]$TopTiles = 12
)

$ErrorActionPreference = "Stop"

$terrainPrefixes = @{
    "jun" = "jungle"
    "des" = "desert"
    "ice" = "ice"
    "mor" = "moors"
    "int" = "interior"
    "afx" = "amigaFormat"
}

$spriteNames = @{
    0   = "Player"
    2   = "Grenade"
    4   = "Null"
    5   = "Enemy"
    13  = "Shrub"
    14  = "Tree"
    15  = "BuildingRoof"
    16  = "Snowman"
    17  = "Shrub2"
    20  = "BuildingDoor"
    25  = "BuildingDoor2"
    37  = "GrenadeBox"
    38  = "RocketBox"
    54  = "Mine"
    55  = "Mine2"
    56  = "Spike"
    60  = "BoilingPot"
    61  = "Civilian"
    62  = "Civilian2"
    63  = "Skidu_Human"
    64  = "VehicleGun_Human"
    65  = "Tank_Human"
    69  = "Tank_Enemy"
    70  = "Civilian_Spear"
    71  = "Civilian_Spear2"
    72  = "Hostage"
    73  = "Hostage_Rescue_Tent"
    74  = "Door_Civilian"
    75  = "Door2_Civilian"
    76  = "Door_Civilian_Spear"
    77  = "Cannon"
    78  = "Turret_Missile_Human"
    80  = "VehicleNoGun_Enemy"
    81  = "VehicleGun_Enemy"
    83  = "Civilian_Invisible"
    84  = "Turret_Missile_Enemy"
    88  = "BuildingDoor3"
    90  = "Door_Civilian_Rescue"
    91  = "Seal_Mine"
    92  = "Spider_Mine"
    99  = "Helicopter_CallPad"
    100 = "BuildingDoor_Reinforced"
    106 = "Enemy_Leader"
    108 = "Computer_1"
}

$structureSprites = @(15, 20, 25, 74, 75, 76, 88, 90, 100)
$civilianSprites = @(61, 62, 70, 71, 83)
$pickupSprites = @(37, 38, 93, 94, 95, 96, 98)
$vehicleSprites = @(63, 64, 65, 69, 80, 81, 82)
$turretSprites = @(77, 78, 79, 84, 85, 105)
$hazardSprites = @(54, 55, 56, 60, 91, 92)

function Read-MapFile {
    param([string]$Path)

    $bytes = [IO.File]::ReadAllBytes($Path)
    if($bytes.Length -lt 0x60) {
        return $null
    }

    $prefix = [Text.Encoding]::ASCII.GetString($bytes, 0, 3).ToLowerInvariant()
    $terrain = if($terrainPrefixes.ContainsKey($prefix)) { $terrainPrefixes[$prefix] } else { "unknown" }
    $sub = if($bytes[0x10] -eq [byte][char]'1') { 1 } else { 0 }
    $width = ($bytes[0x54] -shl 8) + $bytes[0x55]
    $height = ($bytes[0x56] -shl 8) + $bytes[0x57]
    $expected = 0x60 + ($width * $height * 2)

    if($width -le 0 -or $height -le 0 -or $expected -ne $bytes.Length) {
        return $null
    }

    $tileCounts = @{}
    $tileGrid = New-Object int[] ($width * $height)
    $offset = 0x60

    for($index = 0; $index -lt $tileGrid.Length; ++$index) {
        $tile = ($bytes[$offset] -shl 8) + $bytes[$offset + 1]
        $offset += 2
        $tileGrid[$index] = $tile

        if(-not $tileCounts.ContainsKey($tile)) {
            $tileCounts[$tile] = 0
        }
        ++$tileCounts[$tile]
    }

    return [pscustomobject]@{
        Path        = $Path
        Terrain     = $terrain
        SubVariant  = $sub
        Width       = $width
        Height      = $height
        Area        = $width * $height
        UniqueTiles = $tileCounts.Count
        TileCounts  = $tileCounts
        Grid        = $tileGrid
    }
}

function Read-SpriteFile {
    param([string]$Path)

    if(-not (Test-Path $Path)) {
        return @()
    }

    $bytes = [IO.File]::ReadAllBytes($Path)
    $count = [int]([Math]::Floor($bytes.Length / 10))
    $sprites = New-Object 'System.Collections.ArrayList'

    for($index = 0; $index -lt $count; ++$index) {
        $base = $index * 10
        $direction = ($bytes[$base] -shl 8) + $bytes[$base + 1]
        $x = ($bytes[$base + 4] -shl 8) + $bytes[$base + 5]
        $y = ($bytes[$base + 6] -shl 8) + $bytes[$base + 7]
        $type = ($bytes[$base + 8] -shl 8) + $bytes[$base + 9]

        if($x -ge 32768) { $x -= 65536 }
        if($y -ge 32768) { $y -= 65536 }
        if($type -ge 32768) { $type -= 65536 }

        if($x -lt 0 -or $type -lt 0) {
            continue
        }

        [void]$sprites.Add([pscustomobject]@{
            Direction = $direction
            X         = $x
            Y         = $y
            Type      = $type
            TileX     = [int]([Math]::Floor($x / 16))
            TileY     = [int]([Math]::Floor($y / 16))
        })
    }

    return ,$sprites.ToArray()
}

function Get-CoarseShape {
    param($Width, $Height, $Sprites, $Buildings)

    $shape = [ordered]@{
        playerCount     = 0
        enemyCount      = 0
        civilianCount   = 0
        hostageCount    = 0
        rescueTentCount = 0
        buildingCount   = $Buildings.Count
        pickupCount     = 0
        vehicleCount    = 0
        turretCount     = 0
        hazardCount     = 0
        playerCenter    = $null
        enemyCenter     = $null
        buildingCenter  = $null
    }

    $playerXs = @(); $playerYs = @()
    $enemyXs = @(); $enemyYs = @()

    foreach($sprite in $Sprites) {
        switch($sprite.Type) {
            0 {
                ++$shape.playerCount
                $playerXs += $sprite.TileX
                $playerYs += $sprite.TileY
            }
            5 {
                ++$shape.enemyCount
                $enemyXs += $sprite.TileX
                $enemyYs += $sprite.TileY
            }
            72 { ++$shape.hostageCount }
            73 { ++$shape.rescueTentCount }
        }

        if($civilianSprites -contains $sprite.Type) { ++$shape.civilianCount }
        if($pickupSprites -contains $sprite.Type) { ++$shape.pickupCount }
        if($vehicleSprites -contains $sprite.Type) { ++$shape.vehicleCount }
        if($turretSprites -contains $sprite.Type) { ++$shape.turretCount }
        if($hazardSprites -contains $sprite.Type) { ++$shape.hazardCount }
    }

    if($playerXs.Count -gt 0) {
        $shape.playerCenter = @{ x = [int]([Math]::Round(($playerXs | Measure-Object -Sum).Sum / $playerXs.Count)); y = [int]([Math]::Round(($playerYs | Measure-Object -Sum).Sum / $playerYs.Count)) }
    }
    if($enemyXs.Count -gt 0) {
        $shape.enemyCenter = @{ x = [int]([Math]::Round(($enemyXs | Measure-Object -Sum).Sum / $enemyXs.Count)); y = [int]([Math]::Round(($enemyYs | Measure-Object -Sum).Sum / $enemyYs.Count)) }
    }

    if($Buildings.Count -gt 0) {
        $bxs = $Buildings | ForEach-Object { $_.TileX }
        $bys = $Buildings | ForEach-Object { $_.TileY }
        $shape.buildingCenter = @{ x = [int]([Math]::Round(($bxs | Measure-Object -Sum).Sum / $bxs.Count)); y = [int]([Math]::Round(($bys | Measure-Object -Sum).Sum / $bys.Count)) }
    }

    return $shape
}

function Describe-Position {
    param($Width, $Height, $Point)

    if(-not $Point) { return "n/a" }

    $fx = $Point.x / [Math]::Max(1, $Width - 1)
    $fy = $Point.y / [Math]::Max(1, $Height - 1)

    $h = if($fx -lt 0.33) { "left" } elseif($fx -lt 0.66) { "centre" } else { "right" }
    $v = if($fy -lt 0.33) { "top" } elseif($fy -lt 0.66) { "middle" } else { "bottom" }

    return "$v-$h"
}

function Describe-Shape {
    param($Width, $Height, $Shape)

    if(-not $Shape.playerCenter -or -not $Shape.enemyCenter) {
        return "single-zone"
    }

    $dx = $Shape.enemyCenter.x - $Shape.playerCenter.x
    $dy = $Shape.enemyCenter.y - $Shape.playerCenter.y
    $absX = [Math]::Abs($dx)
    $absY = [Math]::Abs($dy)

    if($absX -gt 2 * $absY) {
        return "horizontal-axis"
    }
    if($absY -gt 2 * $absX) {
        return "vertical-axis"
    }
    return "diagonal-axis"
}

$rawJson = [IO.File]::ReadAllText($Campaign)
if($rawJson.Length -gt 0 -and [int][char]$rawJson[0] -eq 0xFEFF) {
    $rawJson = $rawJson.Substring(1)
}
$campaignData = ConvertFrom-Json -InputObject $rawJson
Write-Host "Campaign loaded. Missions: $($campaignData.Missions.Count)"
$phaseInfo = @{}
$missionLookup = New-Object 'System.Collections.ArrayList'
$phaseIndex = 0

foreach($mission in $campaignData.Missions) {
    foreach($phase in $mission.Phases) {
        ++$phaseIndex
        $phaseInfo[$phase.MapName] = [pscustomobject]@{
            PhaseNumber = $phaseIndex
            Mission     = $mission.Name
            PhaseName   = $phase.Name
            Objectives  = $phase.Objectives
            Aggression  = $phase.Aggression
        }
        [void]$missionLookup.Add($phase.MapName)
    }
}

$results = New-Object 'System.Collections.ArrayList'
Write-Host "Profiling $($missionLookup.Count) maps..."

foreach($mapName in $missionLookup) {
    $mapPath = Join-Path $DataRoot ("$mapName.map")
    $sptPath = Join-Path $DataRoot ("$mapName.spt")

    if(-not (Test-Path $mapPath)) {
        Write-Warning "Map missing: $mapPath"
        continue
    }

    $map = Read-MapFile -Path $mapPath
    if($null -eq $map) {
        Write-Warning "Map invalid: $mapPath"
        continue
    }

    $sprites = Read-SpriteFile -Path $sptPath
    $buildings = $sprites | Where-Object { $structureSprites -contains $_.Type }
    $shape = Get-CoarseShape -Width $map.Width -Height $map.Height -Sprites $sprites -Buildings $buildings

    $tileTop = $map.TileCounts.GetEnumerator() |
        Sort-Object Value -Descending |
        Select-Object -First $TopTiles |
        ForEach-Object { @{ tile = [int]$_.Key; count = [int]$_.Value } }

    $spriteTypeHistogram = @{}
    foreach($sprite in $sprites) {
        $name = if($spriteNames.ContainsKey($sprite.Type)) { $spriteNames[$sprite.Type] } else { "type_$($sprite.Type)" }
        if(-not $spriteTypeHistogram.ContainsKey($name)) {
            $spriteTypeHistogram[$name] = 0
        }
        ++$spriteTypeHistogram[$name]
    }

    $info = $phaseInfo[$mapName]

    $entry = [pscustomobject]@{
        MapName          = $mapName
        PhaseNumber      = if($info) { $info.PhaseNumber } else { $null }
        Mission          = if($info) { $info.Mission } else { "" }
        PhaseName        = if($info) { $info.PhaseName } else { "" }
        Objectives       = if($info) { $info.Objectives } else { @() }
        Aggression       = if($info) { $info.Aggression } else { @() }
        Terrain          = $map.Terrain
        SubVariant       = $map.SubVariant
        Width            = $map.Width
        Height           = $map.Height
        Area             = $map.Area
        UniqueTiles      = $map.UniqueTiles
        TopTiles         = @($tileTop)
        SpriteCount      = $sprites.Count
        SpriteHistogram  = $spriteTypeHistogram
        Players          = $shape.playerCount
        Enemies          = $shape.enemyCount
        Civilians        = $shape.civilianCount
        Hostages         = $shape.hostageCount
        RescueTents      = $shape.rescueTentCount
        Buildings        = $shape.buildingCount
        Pickups          = $shape.pickupCount
        Vehicles         = $shape.vehicleCount
        Turrets          = $shape.turretCount
        Hazards          = $shape.hazardCount
        PlayerCentre     = $shape.playerCenter
        EnemyCentre      = $shape.enemyCenter
        BuildingCentre   = $shape.buildingCenter
        PlayerRegion     = (Describe-Position -Width $map.Width -Height $map.Height -Point $shape.playerCenter)
        EnemyRegion      = (Describe-Position -Width $map.Width -Height $map.Height -Point $shape.enemyCenter)
        BuildingRegion   = (Describe-Position -Width $map.Width -Height $map.Height -Point $shape.buildingCenter)
        Axis             = (Describe-Shape -Width $map.Width -Height $map.Height -Shape $shape)
    }

    [void]$results.Add($entry)
}

$jsonDir = Split-Path -Parent $JsonOut
if(-not (Test-Path $jsonDir)) {
    New-Item -ItemType Directory -Force -Path $jsonDir | Out-Null
}
$results | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath $JsonOut -Encoding UTF8
Write-Host "Wrote $($results.Count) map profiles to $JsonOut"

$lines = New-Object 'System.Collections.ArrayList'
[void]$lines.Add("# Cannon Fodder Original Map Analysis")
[void]$lines.Add("")
[void]$lines.Add("Auto-generated from `$DataRoot` and `$Campaign` by [Tools/ProfileCannonFodderMaps.ps1](../Tools/ProfileCannonFodderMaps.ps1). Run that script to regenerate this document and the matching JSON dump at `Tools/Generated/cf1-amiga-map-profiles.json`.")
[void]$lines.Add("")
[void]$lines.Add("This document captures the shape of every original Amiga Cannon Fodder map plus the campaign objective metadata for that phase. Use it as the reference target when tuning generator profiles in [RandomMapImprovementPlan.md](RandomMapImprovementPlan.md).")
[void]$lines.Add("")
[void]$lines.Add("## Aggregate Summary")
[void]$lines.Add("")

$byTerrain = $results | Group-Object Terrain
[void]$lines.Add("| Terrain | Maps | Avg W x H | Avg area | Avg unique tiles | Avg buildings | Avg enemies | Avg pickups |")
[void]$lines.Add("| --- | ---: | --- | ---: | ---: | ---: | ---: | ---: |")

foreach($group in ($byTerrain | Sort-Object Name)) {
    $maps = $group.Group
    $avgW = [int]([Math]::Round((($maps | Measure-Object Width -Average).Average)))
    $avgH = [int]([Math]::Round((($maps | Measure-Object Height -Average).Average)))
    $avgArea = [int]([Math]::Round((($maps | Measure-Object Area -Average).Average)))
    $avgUnique = [int]([Math]::Round((($maps | Measure-Object UniqueTiles -Average).Average)))
    $avgBuild = [Math]::Round((($maps | Measure-Object Buildings -Average).Average), 1)
    $avgEnemy = [Math]::Round((($maps | Measure-Object Enemies -Average).Average), 1)
    $avgPickup = [Math]::Round((($maps | Measure-Object Pickups -Average).Average), 1)
    [void]$lines.Add("| $($group.Name) | $($maps.Count) | $avgW x $avgH | $avgArea | $avgUnique | $avgBuild | $avgEnemy | $avgPickup |")
}

[void]$lines.Add("")
[void]$lines.Add("## Per-Phase Detail")
[void]$lines.Add("")

foreach($entry in ($results | Sort-Object PhaseNumber)) {
    $title = "Phase $($entry.PhaseNumber): $($entry.Mission) - $($entry.PhaseName)"
    [void]$lines.Add("### $title ($($entry.MapName))")
    [void]$lines.Add("")
    [void]$lines.Add("- Terrain: $($entry.Terrain) (sub $($entry.SubVariant))")
    [void]$lines.Add("- Size: $($entry.Width) x $($entry.Height) tiles ($($entry.Area) tiles, $($entry.UniqueTiles) unique tile IDs)")
    if($entry.Aggression -and $entry.Aggression.Count -ge 2) {
        [void]$lines.Add("- Aggression: human $($entry.Aggression[0]) / enemy $($entry.Aggression[1])")
    }
    [void]$lines.Add("- Objectives: " + (($entry.Objectives) -join "; "))
    [void]$lines.Add("- Layout axis: $($entry.Axis); player region $($entry.PlayerRegion); enemy region $($entry.EnemyRegion); building region $($entry.BuildingRegion)")
    [void]$lines.Add("- Counts: $($entry.Players) player, $($entry.Enemies) enemy, $($entry.Buildings) building, $($entry.Civilians) civilian, $($entry.Hostages) hostage, $($entry.RescueTents) rescue tent, $($entry.Pickups) pickup, $($entry.Vehicles) vehicle, $($entry.Turrets) turret, $($entry.Hazards) hazard")

    $topTileText = ($entry.TopTiles | ForEach-Object { "$($_.tile)=$($_.count)" }) -join ", "
    [void]$lines.Add("- Top tile IDs: $topTileText")

    $spriteSummary = ($entry.SpriteHistogram.GetEnumerator() | Sort-Object Value -Descending | Select-Object -First 8 | ForEach-Object { "$($_.Key)=$($_.Value)" }) -join ", "
    if($spriteSummary) {
        [void]$lines.Add("- Sprite mix: $spriteSummary")
    }

    [void]$lines.Add("")
}

[void]$lines.Add("")
[void]$lines.Add("## Notes For Generator Tuning")
[void]$lines.Add("")
[void]$lines.Add("- ``Layout axis`` is the dominant direction between the player and enemy centroids, used as a hint when picking a layout template. ``horizontal-axis`` matches the current generator default; ``vertical-axis`` and ``diagonal-axis`` need new templates from [RandomMapImprovementPlan.md](RandomMapImprovementPlan.md) Phase B.")
[void]$lines.Add("- ``Building region`` is the centroid of all structure-doors plus building roofs. Use this together with the objective string to pick the right `MapGen.Features` template.")
[void]$lines.Add("- ``Top tile IDs`` shows where to anchor base palettes. Tiles with high counts but low distinctiveness across the campaign are good candidates for the generator's ground tiles; high-count tiles confined to one or two phases are likely authored micro-stamps.")

Set-Content -LiteralPath $MarkdownOut -Value ($lines -join [Environment]::NewLine) -Encoding UTF8
Write-Host "Wrote analysis markdown to $MarkdownOut"
