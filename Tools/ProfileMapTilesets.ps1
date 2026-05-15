param(
    [string]$DataRoot = "D:\Games\OpenFodder\Data",
    [int]$TopCount = 18,
    [switch]$Markdown,
    [string]$JsonPath = "",
    [switch]$ValidateCatalog,
    [string]$CatalogPath = "Run\Scripts\MapGen\Terrain\TileCatalog.js"
)

$terrainPrefixes = @{
    "jun" = "jungle"
    "des" = "desert"
    "ice" = "ice"
    "mor" = "moors"
    "int" = "interior"
    "afx" = "amigaFormat"
    "hid" = "hid"
}

$terrainResources = @{
    "jungle" = @{ Folder = "3DO"; Prefix = "JUN"; Indestructible = @(267, 268, 269, 287, 288, 289, 307, 308, 309, 98, 177, 217) }
    "desert" = @{ Folder = "3DO"; Prefix = "DES"; Indestructible = @(9, 10, 11, 29, 30, 31, 50) }
    "ice" = @{ Folder = "3DO"; Prefix = "ICE"; Indestructible = @(307, 308, 309, 327, 328, 329, 348, 349, 363, 378, 379) }
    "moors" = @{ Folder = "3DO"; Prefix = "MOR"; Indestructible = @(257, 276, 277, 160, 161, 162, 180, 181, 182, 201, 306, 384, 398) }
    "interior" = @{ Folder = "3DO"; Prefix = "INT"; Indestructible = @(23, 40, 41, 42, 63, 103, 50, 51, 70, 71, 90, 91, 346, 331, 351) }
    "amigaFormat" = @{ Folder = "AmigaFormat_XMAS"; Prefix = "AFX"; Indestructible = @(307, 308, 309, 327, 328, 329, 348, 349, 363, 378, 379) }
}

$featureNames = @{
    0 = "land"
    1 = "rocky"
    2 = "rocky2"
    3 = "block"
    4 = "quicksand"
    5 = "waterEdge"
    6 = "water"
    7 = "snow"
    8 = "quicksandEdge"
    9 = "drop"
    10 = "drop2"
    11 = "sink"
    12 = "c"
    13 = "d"
    14 = "jump"
}

$notFlyable = @(0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1)
$notDriveable = @(0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0)
$notWalkable = @(0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)

function Add-Count($Table, $Key, [int]$Amount = 1) {
    if(-not $Table.ContainsKey($Key)) {
        $Table[$Key] = 0
    }

    $Table[$Key] += $Amount
}

function Top-Counts($Table, [int]$Count) {
    return @($Table.GetEnumerator() |
        Sort-Object Value -Descending |
        Select-Object -First $Count |
        ForEach-Object { "$($_.Key)=$($_.Value)" })
}

function Read-SignedBEWord($Bytes, [int]$Offset) {
    if($Offset + 1 -ge $Bytes.Length) {
        return 0
    }

    $value = (($Bytes[$Offset] -shl 8) + $Bytes[$Offset + 1]) -band 0xFFFF
    if($value -ge 0x8000) {
        $value -= 0x10000
    }

    return [int]$value
}

function Load-HitBytes($Path) {
    if(Test-Path -LiteralPath $Path) {
        return [IO.File]::ReadAllBytes($Path)
    }

    return @()
}

function Load-TerrainMetadata($Root, $TerrainKey) {
    $resource = $terrainResources[$TerrainKey]
    if(-not $resource) {
        return $null
    }

    $folder = Join-Path $Root $resource.Folder
    $prefix = $resource.Prefix
    $hit = New-Object int[] 512
    $bht = New-Object 'byte[,]' 512,8
    $swp = New-Object int[] 400
    $indestructible = @{}

    foreach($tile in $resource.Indestructible) {
        $indestructible[[int]$tile] = $true
    }

    $baseHit = Load-HitBytes (Join-Path $folder ($prefix + "BASE.HIT"))
    $subHit = Load-HitBytes (Join-Path $folder ($prefix + "SUB0.HIT"))
    $baseBht = Load-HitBytes (Join-Path $folder ($prefix + "BASE.BHT"))
    $subBht = Load-HitBytes (Join-Path $folder ($prefix + "SUB0.BHT"))
    $baseSwp = Load-HitBytes (Join-Path $folder ($prefix + "BASE.SWP"))
    $subSwp = Load-HitBytes (Join-Path $folder ($prefix + "SUB0.SWP"))

    for($index = 0; $index -lt [Math]::Min(240, [Math]::Floor($baseHit.Length / 2)); ++$index) {
        $hit[$index] = Read-SignedBEWord $baseHit ($index * 2)
    }

    for($index = 0; $index -lt [Math]::Min(160, [Math]::Floor($subHit.Length / 2)); ++$index) {
        $hit[240 + $index] = Read-SignedBEWord $subHit ($index * 2)
    }

    for($tile = 0; $tile -lt [Math]::Min(240, [Math]::Floor($baseBht.Length / 8)); ++$tile) {
        for($row = 0; $row -lt 8; ++$row) {
            $bht[$tile,$row] = $baseBht[($tile * 8) + $row]
        }
    }

    for($tile = 0; $tile -lt [Math]::Min(160, [Math]::Floor($subBht.Length / 8)); ++$tile) {
        for($row = 0; $row -lt 8; ++$row) {
            $bht[(240 + $tile),$row] = $subBht[($tile * 8) + $row]
        }
    }

    for($index = 0; $index -lt [Math]::Min(240, [Math]::Floor($baseSwp.Length / 2)); ++$index) {
        $swp[$index] = Read-SignedBEWord $baseSwp ($index * 2)
    }

    for($index = 0; $index -lt [Math]::Min(160, [Math]::Floor($subSwp.Length / 2)); ++$index) {
        $swp[240 + $index] = Read-SignedBEWord $subSwp ($index * 2)
    }

    return @{
        Hit = $hit
        BHit = $bht
        Swap = $swp
        Indestructible = $indestructible
        ResourceFolder = $folder
    }
}

function Tile-Features($HitValue) {
    $low = $HitValue -band 0x0F

    if($HitValue -ge 0) {
        return @($low)
    }

    $high = ($HitValue -shr 4) -band 0x0F
    if($high -eq $low) {
        return @($low)
    }

    return @($low, $high)
}

function Feature-Names($Features) {
    return ($Features | ForEach-Object {
        if($featureNames.ContainsKey($_)) {
            $featureNames[$_]
        }
        else {
            "feature$_"
        }
    }) -join "+"
}

function Feature-NameList($Features) {
    return @($Features | ForEach-Object {
        if($featureNames.ContainsKey($_)) {
            $featureNames[$_]
        }
        else {
            "feature$_"
        }
    })
}

function Any-Blocked($Features, $Lookup) {
    foreach($feature in $Features) {
        if($Lookup[$feature]) {
            return $true
        }
    }

    return $false
}

function Tile-Behavior($Metadata, [int]$TileID) {
    if(-not $Metadata) {
        return $null
    }

    $hit = $Metadata.Hit[$TileID]
    $features = Tile-Features $hit
    $destroysTo = if($TileID -lt $Metadata.Swap.Length) { $Metadata.Swap[$TileID] } else { 0 }
    $destructible = $destroysTo -ge 0 -and -not $Metadata.Indestructible.ContainsKey($TileID)

    return [pscustomobject]@{
        Tile = $TileID
        Hit = $hit
        Features = $features
        FeatureNames = Feature-Names $features
        WalkBlocked = Any-Blocked $features $notWalkable
        DriveBlocked = Any-Blocked $features $notDriveable
        FlyBlocked = Any-Blocked $features $notFlyable
        Destructible = $destructible
        DestroysTo = $destroysTo
    }
}

function Tile-Category($Behavior) {
    if(-not $Behavior) {
        return "unknown"
    }

    if($Behavior.WalkBlocked) {
        return "blocked"
    }

    if($Behavior.Features -contains 6) {
        return "water"
    }

    if($Behavior.Features -contains 11) {
        return "sink"
    }

    if($Behavior.Features -contains 4) {
        return "quicksand"
    }

    if($Behavior.Features -contains 8) {
        return "quicksandEdge"
    }

    if($Behavior.Features -contains 5) {
        return "waterEdge"
    }

    if($Behavior.Features -contains 10) {
        return "drop2"
    }

    if($Behavior.Features -contains 9) {
        return "drop"
    }

    if($Behavior.Features -contains 14) {
        return "jump"
    }

    if($Behavior.Features -contains 2) {
        return "rocky2"
    }

    if($Behavior.Features -contains 1) {
        return "rocky"
    }

    if($Behavior.Features -contains 7) {
        return "snow"
    }

    return "land"
}

function Tile-SemanticClasses($Behavior) {
    if(-not $Behavior) {
        return @("unknown")
    }

    $classes = @()

    foreach($name in (Feature-NameList $Behavior.Features)) {
        if($classes -notcontains $name) {
            $classes += $name
        }
    }

    if($Behavior.WalkBlocked -and $classes -notcontains "blocked") {
        $classes += "blocked"
    }

    return $classes
}

function Tile-IsSafeWalkable($Behavior) {
    if(-not $Behavior -or $Behavior.WalkBlocked) {
        return $false
    }

    foreach($feature in @(4, 5, 6, 8, 9, 10, 11)) {
        if($Behavior.Features -contains $feature) {
            return $false
        }
    }

    return $true
}

function Top-CategoryTiles($Counts, $Metadata, $Category, [int]$Count) {
    return @($Counts.GetEnumerator() |
        ForEach-Object {
            $tile = [int]$_.Key
            $behavior = Tile-Behavior $Metadata $tile
            [pscustomobject]@{
                Tile = $tile
                Count = $_.Value
                Category = Tile-Category $behavior
                SafeWalkable = Tile-IsSafeWalkable $behavior
            }
        } |
        Where-Object {
            if($Category -eq "safeWalkable") {
                $_.SafeWalkable
            }
            else {
                $_.Category -eq $Category
            }
        } |
        Sort-Object Count -Descending |
        Select-Object -First $Count |
        ForEach-Object { "$($_.Tile)=$($_.Count)" })
}

function Ensure-TerrainStats($Stats, $Key) {
    if(-not $Stats.ContainsKey($Key)) {
        $Stats[$Key] = [ordered]@{
            maps = 0
            tiles = 0
            counts = @{}
            pairs = @{}
            examples = @()
        }
    }

    return $Stats[$Key]
}

$stats = @{}
$valid = 0
$ignored = 0
$ignoredReasons = @{}

Get-ChildItem -Path $DataRoot -Recurse -Filter *.map -ErrorAction SilentlyContinue | ForEach-Object {
    $bytes = [IO.File]::ReadAllBytes($_.FullName)
    $reason = $null

    if($bytes.Length -lt 0x60) {
        $reason = "short_header"
    }
    else {
        $width = ($bytes[0x54] -shl 8) + $bytes[0x55]
        $height = ($bytes[0x56] -shl 8) + $bytes[0x57]
        $expected = 0x60 + ($width * $height * 2)

        if($width -le 0 -or $height -le 0) {
            $reason = "invalid_dimensions"
        }
        elseif($width -gt 512 -or $height -gt 512) {
            $reason = "implausible_dimensions"
        }
        elseif($expected -ne $bytes.Length) {
            $reason = "resource_layer_or_packed_duplicate"
        }
    }

    if($reason) {
        ++$script:ignored
        Add-Count $ignoredReasons $reason
        return
    }

    $prefix = [Text.Encoding]::ASCII.GetString($bytes, 0, 3).ToLowerInvariant()
    if(-not $terrainPrefixes.ContainsKey($prefix)) {
        $prefix = "jun"
    }

    $terrain = $terrainPrefixes[$prefix]
    if($terrain -eq "hid") {
        $terrain = "jungle"
    }

    $sub = if($bytes[0x10] -eq [byte][char]'1') { "1" } else { "0" }
    $key = "$terrain-sub$sub"
    $entry = Ensure-TerrainStats $stats $key
    ++$entry.maps
    $entry.tiles += ($width * $height)

    if($entry.examples.Count -lt 5) {
        $entry.examples += $_.FullName
    }

    $grid = New-Object int[] ($width * $height)
    $offset = 0x60

    for($index = 0; $index -lt $grid.Length; ++$index) {
        $tile = ((($bytes[$offset] -shl 8) + $bytes[$offset + 1]) -band 0x1FF)
        $offset += 2
        $grid[$index] = $tile
        Add-Count $entry.counts $tile
    }

    for($y = 0; $y -lt $height; ++$y) {
        $row = $y * $width

        for($x = 0; $x -lt $width; ++$x) {
            $tile = $grid[$row + $x]

            if($x + 1 -lt $width) {
                $other = $grid[$row + $x + 1]
                $pair = if($tile -le $other) { "$tile|$other" } else { "$other|$tile" }
                Add-Count $entry.pairs $pair
            }

            if($y + 1 -lt $height) {
                $other = $grid[$row + $x + $width]
                $pair = if($tile -le $other) { "$tile|$other" } else { "$other|$tile" }
                Add-Count $entry.pairs $pair
            }
        }
    }

    ++$script:valid
}

function Write-Line($Text = "") {
    Write-Output $Text
}

function Count-Objects($Table, [int]$Count = 0) {
    $items = @($Table.GetEnumerator() |
        Sort-Object @{ Expression = { $_.Value }; Descending = $true }, @{ Expression = { [string]$_.Key }; Descending = $false })

    if($Count -gt 0) {
        $items = @($items | Select-Object -First $Count)
    }

    return @($items | ForEach-Object {
        [ordered]@{
            key = [string]$_.Key
            count = [int]$_.Value
        }
    })
}

function Semantic-UsageCounts($Counts, $Metadata) {
    $semanticCounts = @{}

    foreach($item in $Counts.GetEnumerator()) {
        $tile = [int]$item.Key
        $uses = [int]$item.Value
        $behavior = Tile-Behavior $Metadata $tile
        $class = Tile-Category $behavior

        Add-Count $semanticCounts $class $uses
        if(Tile-IsSafeWalkable $behavior) {
            Add-Count $semanticCounts "safeWalkable" $uses
        }
    }

    return Count-Objects $semanticCounts
}

function Parse-TilePair([string]$Pair) {
    $parts = $Pair -split "\|"

    if($parts.Count -ne 2) {
        return $null
    }

    return @([int]$parts[0], [int]$parts[1])
}

function Tile-TransitionClass($Behavior) {
    if(Tile-IsSafeWalkable $Behavior) {
        return "safeWalkable"
    }

    return Tile-Category $Behavior
}

function Ordered-PairKey([string]$Left, [string]$Right) {
    if([string]::CompareOrdinal($Left, $Right) -le 0) {
        return "$Left|$Right"
    }

    return "$Right|$Left"
}

function Semantic-AdjacentPairCounts($Pairs, $Metadata) {
    $semanticPairs = @{}

    foreach($item in $Pairs.GetEnumerator()) {
        $tiles = Parse-TilePair $item.Key
        if(-not $tiles) {
            continue
        }

        $leftClass = Tile-TransitionClass (Tile-Behavior $Metadata $tiles[0])
        $rightClass = Tile-TransitionClass (Tile-Behavior $Metadata $tiles[1])
        Add-Count $semanticPairs (Ordered-PairKey $leftClass $rightClass) ([int]$item.Value)
    }

    return Count-Objects $semanticPairs
}

function Transition-Candidates($Pairs, $Metadata, [int]$Count) {
    $relations = @{}

    foreach($item in $Pairs.GetEnumerator()) {
        $tiles = Parse-TilePair $item.Key
        if(-not $tiles) {
            continue
        }

        $leftTile = [int]$tiles[0]
        $rightTile = [int]$tiles[1]
        $leftClass = Tile-TransitionClass (Tile-Behavior $Metadata $leftTile)
        $rightClass = Tile-TransitionClass (Tile-Behavior $Metadata $rightTile)

        if($leftClass -eq $rightClass) {
            continue
        }

        $relationKey = Ordered-PairKey $leftClass $rightClass
        if(-not $relations.ContainsKey($relationKey)) {
            $relations[$relationKey] = @{
                count = 0
                pairs = @{}
                tilesByClass = @{}
            }
        }

        $relation = $relations[$relationKey]
        $uses = [int]$item.Value
        $relation.count += $uses
        Add-Count $relation.pairs $item.Key $uses

        if(-not $relation.tilesByClass.ContainsKey($leftClass)) {
            $relation.tilesByClass[$leftClass] = @{}
        }
        if(-not $relation.tilesByClass.ContainsKey($rightClass)) {
            $relation.tilesByClass[$rightClass] = @{}
        }

        Add-Count $relation.tilesByClass[$leftClass] $leftTile $uses
        Add-Count $relation.tilesByClass[$rightClass] $rightTile $uses
    }

    return @($relations.GetEnumerator() |
        Sort-Object @{ Expression = { $_.Value.count }; Descending = $true }, @{ Expression = { [string]$_.Key }; Descending = $false } |
        Select-Object -First $Count |
        ForEach-Object {
            $classes = $_.Key -split "\|"
            $tilesByClass = [ordered]@{}

            foreach($class in $classes) {
                $tilesByClass[$class] = @(Count-Objects $_.Value.tilesByClass[$class] $Count)
            }

            [ordered]@{
                relation = [string]$_.Key
                count = [int]$_.Value.count
                topAdjacentPairs = @(Count-Objects $_.Value.pairs $Count)
                topTilesByClass = $tilesByClass
            }
        })
}

function Tile-Summary($Metadata, [int]$TileID, [int]$Uses) {
    $behavior = Tile-Behavior $Metadata $TileID

    if(-not $behavior) {
        return [ordered]@{
            tile = $TileID
            uses = $Uses
            primaryClass = "unknown"
            semanticClasses = @("unknown")
        }
    }

    return [ordered]@{
        tile = $TileID
        uses = $Uses
        hit = [int]$behavior.Hit
        features = @($behavior.Features | ForEach-Object { [int]$_ })
        featureNames = @(Feature-NameList $behavior.Features)
        primaryClass = Tile-Category $behavior
        semanticClasses = @(Tile-SemanticClasses $behavior)
        safeWalkable = [bool](Tile-IsSafeWalkable $behavior)
        walkBlocked = [bool]$behavior.WalkBlocked
        driveBlocked = [bool]$behavior.DriveBlocked
        flyBlocked = [bool]$behavior.FlyBlocked
        destructible = [bool]$behavior.Destructible
        destroysTo = [int]$behavior.DestroysTo
    }
}

function Build-ProfileExport {
    $featureNameExport = [ordered]@{}
    foreach($featureId in ($featureNames.Keys | Sort-Object)) {
        $featureNameExport[[string]$featureId] = $featureNames[$featureId]
    }

    $classOrder = @(
        "safeWalkable",
        "land",
        "snow",
        "rocky",
        "rocky2",
        "water",
        "waterEdge",
        "quicksand",
        "quicksandEdge",
        "sink",
        "drop",
        "drop2",
        "jump",
        "blocked",
        "unknown"
    )
    $profiles = @()

    foreach($key in ($stats.Keys | Sort-Object)) {
        $entry = $stats[$key]
        $terrain = ($key -split "-")[0]
        $metadata = Load-TerrainMetadata $DataRoot $terrain
        $sub = ""

        if($key -match "-sub(.+)$") {
            $sub = $Matches[1]
        }

        $topByClass = [ordered]@{}
        foreach($class in $classOrder) {
            $topByClass[$class] = @(Top-CategoryTiles $entry.counts $metadata $class $TopCount)
        }

        $tileBehaviors = @($entry.counts.GetEnumerator() |
            Sort-Object @{ Expression = { [int]$_.Key }; Descending = $false } |
            ForEach-Object { Tile-Summary $metadata ([int]$_.Key) ([int]$_.Value) })

        $profiles += [ordered]@{
            key = $key
            terrain = $terrain
            sub = $sub
            maps = [int]$entry.maps
            tiles = [int]$entry.tiles
            uniqueTileIds = [int]$entry.counts.Count
            uniqueAdjacentPairs = [int]$entry.pairs.Count
            collisionSource = if($metadata) { $metadata.ResourceFolder } else { $null }
            examples = @($entry.examples | ForEach-Object { Split-Path $_ -Leaf })
            topTiles = @(Count-Objects $entry.counts $TopCount)
            topAdjacentPairs = @(Count-Objects $entry.pairs $TopCount)
            topBySemanticClass = $topByClass
            semanticUsageCounts = @(Semantic-UsageCounts $entry.counts $metadata)
            semanticAdjacentPairs = @(Semantic-AdjacentPairCounts $entry.pairs $metadata)
            transitionCandidates = @(Transition-Candidates $entry.pairs $metadata $TopCount)
            tileCounts = @(Count-Objects $entry.counts)
            adjacencyCounts = @(Count-Objects $entry.pairs)
            tileBehaviors = $tileBehaviors
        }
    }

    return [ordered]@{
        schemaVersion = 1
        generatedAtUtc = (Get-Date).ToUniversalTime().ToString("o")
        sourceRoot = $DataRoot
        topCount = $TopCount
        directRawMapsScanned = [int]$valid
        ignoredMaps = [int]$ignored
        ignoredReasons = @(Count-Objects $ignoredReasons)
        featureNames = $featureNameExport
        semanticClasses = $classOrder
        profiles = $profiles
    }
}

function Resolve-ProjectPath([string]$Path) {
    if([IO.Path]::IsPathRooted($Path)) {
        return $Path
    }

    $projectRoot = Split-Path -Parent $PSScriptRoot
    if(-not $projectRoot) {
        $projectRoot = Get-Location
    }

    return Join-Path $projectRoot $Path
}

function Find-JsMatchingToken([string]$Text, [int]$OpenIndex, [char]$OpenChar, [char]$CloseChar) {
    $depth = 0
    $inString = $false
    $quote = [char]0
    $escape = $false

    for($index = $OpenIndex; $index -lt $Text.Length; ++$index) {
        $ch = $Text[$index]

        if($inString) {
            if($escape) {
                $escape = $false
                continue
            }

            if($ch -eq [char]92) {
                $escape = $true
                continue
            }

            if($ch -eq $quote) {
                $inString = $false
            }

            continue
        }

        if($ch -eq [char]34 -or $ch -eq [char]39) {
            $inString = $true
            $quote = $ch
            continue
        }

        if($ch -eq [char]47 -and $index + 1 -lt $Text.Length) {
            $next = $Text[$index + 1]

            if($next -eq [char]47) {
                while($index -lt $Text.Length -and $Text[$index] -ne [char]10) {
                    ++$index
                }
                continue
            }

            if($next -eq [char]42) {
                $index += 2
                while($index + 1 -lt $Text.Length -and -not ($Text[$index] -eq [char]42 -and $Text[$index + 1] -eq [char]47)) {
                    ++$index
                }
                ++$index
                continue
            }
        }

        if($ch -eq $OpenChar) {
            ++$depth
        }
        elseif($ch -eq $CloseChar) {
            --$depth
            if($depth -eq 0) {
                return $index
            }
        }
    }

    return -1
}

function Extract-JsBlock([string]$Text, [string]$Name, [char]$OpenChar, [char]$CloseChar) {
    $escapedOpen = [regex]::Escape([string]$OpenChar)
    $pattern = "(?m)^\s*" + [regex]::Escape($Name) + "\s*:\s*" + $escapedOpen
    $match = [regex]::Match($Text, $pattern)

    if(-not $match.Success) {
        return $null
    }

    $openIndex = $Text.IndexOf([string]$OpenChar, $match.Index)
    if($openIndex -lt 0) {
        return $null
    }

    $closeIndex = Find-JsMatchingToken $Text $openIndex $OpenChar $CloseChar
    if($closeIndex -lt 0) {
        return $null
    }

    return $Text.Substring($openIndex, $closeIndex - $openIndex + 1)
}

function Parse-JsTileList([string]$Text) {
    return @([regex]::Matches($Text, "-?\d+") | ForEach-Object { [int]$_.Value })
}

function Parse-JsTileArrays([string]$Text) {
    $arrays = [ordered]@{}
    $matches = [regex]::Matches($Text, "(?m)([A-Za-z0-9_]+)\s*:\s*\[([^\]]*)\]")

    foreach($match in $matches) {
        $arrays[$match.Groups[1].Value] = @(Parse-JsTileList $match.Groups[2].Value)
    }

    return $arrays
}

function Read-MapGenTileCatalog([string]$Path) {
    $resolvedPath = Resolve-ProjectPath $Path
    if(-not (Test-Path -LiteralPath $resolvedPath)) {
        throw "Tile catalog not found: $resolvedPath"
    }

    $text = Get-Content -LiteralPath $resolvedPath -Raw
    $terrainNames = @("Jungle", "Desert", "Ice", "Moors", "Interior", "AmigaFormat")
    $terrainKeys = @{
        Jungle = "jungle"
        Desert = "desert"
        Ice = "ice"
        Moors = "moors"
        Interior = "interior"
        AmigaFormat = "amigaFormat"
    }
    $profiles = @()

    foreach($terrainName in $terrainNames) {
        $block = Extract-JsBlock $text $terrainName ([char]'{') ([char]'}')
        if(-not $block) {
            throw "Could not parse terrain catalog block: $terrainName"
        }

        $supportsWater = $true
        if($block -match "supportsWater\s*:\s*(true|false)") {
            $supportsWater = $Matches[1] -eq "true"
        }

        $paletteBlock = Extract-JsBlock $block "palette" ([char]'{') ([char]'}')
        $stampsBlock = Extract-JsBlock $block "stamps" ([char]'{') ([char]'}')
        $familiesBlock = Extract-JsBlock $block "families" ([char]'[') ([char]']')
        $families = @()

        if($familiesBlock) {
            $familyMatches = [regex]::Matches($familiesBlock, '(?s)id:\s*"([^"]+)".*?tiles:\s*\[([^\]]*)\]')
            foreach($familyMatch in $familyMatches) {
                $families += [ordered]@{
                    id = $familyMatch.Groups[1].Value
                    tiles = @(Parse-JsTileList $familyMatch.Groups[2].Value)
                }
            }
        }

        $profiles += [ordered]@{
            name = $terrainName
            terrainKey = $terrainKeys[$terrainName]
            supportsWater = [bool]$supportsWater
            palette = if($paletteBlock) { Parse-JsTileArrays $paletteBlock } else { [ordered]@{} }
            stamps = if($stampsBlock) { Parse-JsTileArrays $stampsBlock } else { [ordered]@{} }
            families = $families
        }
    }

    return [ordered]@{
        path = $resolvedPath
        profiles = $profiles
    }
}

function Describe-TileBehavior($Behavior) {
    if(-not $Behavior) {
        return "unknown"
    }

    return "class=$(Tile-Category $Behavior) features=$($Behavior.FeatureNames) walkBlocked=$($Behavior.WalkBlocked) driveBlocked=$($Behavior.DriveBlocked) flyBlocked=$($Behavior.FlyBlocked)"
}

function Add-CatalogIssue($Issues, [string]$Severity, [string]$Terrain, [string]$Section, [string]$Key, [int]$Tile, [string]$Message) {
    [void]$Issues.Add([pscustomobject]@{
        Severity = $Severity
        Terrain = $Terrain
        Section = $Section
        Key = $Key
        Tile = $Tile
        Message = $Message
    })
}

function Tile-HasAnyFeature($Behavior, $Features) {
    if(-not $Behavior) {
        return $false
    }

    foreach($feature in $Features) {
        if($Behavior.Features -contains $feature) {
            return $true
        }
    }

    return $false
}

function Test-CatalogPolicy($Behavior, [string]$Policy, [bool]$SupportsWater) {
    if(-not $Behavior) {
        return $false
    }

    switch($Policy) {
        "safe" {
            return Tile-IsSafeWalkable $Behavior
        }
        "blocked" {
            return [bool]$Behavior.WalkBlocked
        }
        "water" {
            return $Behavior.Features -contains 6
        }
        "waterOrSafePlaceholder" {
            if($SupportsWater) {
                return $Behavior.Features -contains 6
            }

            return Tile-IsSafeWalkable $Behavior
        }
        "bank" {
            if($Behavior.WalkBlocked) {
                return $false
            }

            return -not (Tile-HasAnyFeature $Behavior @(6, 9, 10, 14))
        }
        "hazard" {
            return Tile-HasAnyFeature $Behavior @(4, 5, 8, 9, 10, 11)
        }
        default {
            return $true
        }
    }
}

function Catalog-PolicyMessage([string]$Policy, [bool]$SupportsWater) {
    switch($Policy) {
        "safe" { return "must be safe walkable ordinary ground" }
        "blocked" { return "must use walk-blocking cover/obstacle tiles" }
        "water" { return "must use water/liquid tiles" }
        "waterOrSafePlaceholder" {
            if($SupportsWater) {
                return "must use water/liquid tiles"
            }
            return "must use safe placeholder ground because this terrain disables water generation"
        }
        "bank" { return "must be non-blocking bank or soft-edge material, not water/drop/jump/cover" }
        "hazard" { return "must be explicit soft-hazard or edge material" }
        default { return "has no validation policy" }
    }
}

function Validate-CatalogTileSet($Issues, $Metadata, [string]$Terrain, [bool]$SupportsWater, [string]$Section, [string]$Key, $Tiles, [string]$Policy) {
    $seen = @{}

    foreach($tile in $Tiles) {
        if($seen.ContainsKey($tile)) {
            continue
        }
        $seen[$tile] = $true

        if($tile -lt 0 -or $tile -gt 511) {
            Add-CatalogIssue $Issues "error" $Terrain $Section $Key $tile "tile ID is outside the supported 0..511 range"
            continue
        }

        $behavior = Tile-Behavior $Metadata $tile
        if(-not (Test-CatalogPolicy $behavior $Policy $SupportsWater)) {
            $message = "$(Catalog-PolicyMessage $Policy $SupportsWater); actual $(Describe-TileBehavior $behavior)"
            Add-CatalogIssue $Issues "error" $Terrain $Section $Key $tile $message
        }
    }

    return $seen.Count
}

function Palette-Policy([string]$Key) {
    switch($Key) {
        "path" { return "safe" }
        "grass" { return "safe" }
        "ford" { return "safe" }
        "crossing" { return "safe" }
        "jungleEdge" { return "safe" }
        "featureGround" { return "safe" }
        "beach" { return "safe" }
        "tree" { return "blocked" }
        "water" { return "waterOrSafePlaceholder" }
        "riverBank" { return "bank" }
        "cliff" { return "hazard" }
        "cliffHard" { return "hazard" }
        default { return "" }
    }
}

function Stamp-Policy([string]$Key) {
    switch($Key) {
        "riverBend" { return "waterOrSafePlaceholder" }
        "bankAccent" { return "bank" }
        "fordApproach" { return "safe" }
        "hutClearing" { return "safe" }
        "baseEntrance" { return "safe" }
        "beach" { return "safe" }
        "roughGround" { return "safe" }
        "bloom" { return "safe" }
        "palmBase" { return "safe" }
        "bushTopLeft" { return "safe" }
        "bushTopRight" { return "safe" }
        "bushBottomLeft" { return "safe" }
        "bushBottomRight" { return "safe" }
        default { return "" }
    }
}

function Family-Policy([string]$Key) {
    switch($Key) {
        "path" { return "safe" }
        "grass" { return "safe" }
        "beach" { return "safe" }
        "sand" { return "safe" }
        "snow" { return "safe" }
        "floor" { return "safe" }
        "moorGround" { return "safe" }
        "tree" { return "blocked" }
        "blockedRock" { return "blocked" }
        "blockedIce" { return "blocked" }
        "moorBlocked" { return "blocked" }
        "walls" { return "blocked" }
        "blocked" { return "blocked" }
        "water" { return "water" }
        "moorWater" { return "water" }
        "riverBank" { return "bank" }
        "cliffDrop" { return "hazard" }
        "cliffDrop2" { return "hazard" }
        "wetIce" { return "bank" }
        "quicksandTransition" { return "hazard" }
        "softHazard" { return "hazard" }
        "decor" { return "safe" }
        default { return "" }
    }
}

function Invoke-CatalogValidation([string]$Path) {
    $catalog = Read-MapGenTileCatalog $Path
    $issues = New-Object System.Collections.ArrayList
    $checkedTileSets = 0
    $checkedUniqueTiles = 0

    foreach($profile in $catalog.profiles) {
        $metadata = Load-TerrainMetadata $DataRoot $profile.terrainKey

        if(-not $metadata) {
            Add-CatalogIssue $issues "error" $profile.name "profile" "metadata" -1 "missing terrain collision metadata"
            continue
        }

        foreach($key in $profile.palette.Keys) {
            $policy = Palette-Policy $key
            if(-not $policy) {
                Add-CatalogIssue $issues "warning" $profile.name "palette" $key -1 "no validation policy for palette key"
                continue
            }

            ++$checkedTileSets
            $checkedUniqueTiles += Validate-CatalogTileSet $issues $metadata $profile.name $profile.supportsWater "palette" $key $profile.palette[$key] $policy
        }

        foreach($key in $profile.stamps.Keys) {
            $policy = Stamp-Policy $key
            if(-not $policy) {
                Add-CatalogIssue $issues "warning" $profile.name "stamps" $key -1 "no validation policy for stamp key"
                continue
            }

            ++$checkedTileSets
            $checkedUniqueTiles += Validate-CatalogTileSet $issues $metadata $profile.name $profile.supportsWater "stamps" $key $profile.stamps[$key] $policy
        }

        foreach($family in $profile.families) {
            $policy = Family-Policy $family.id
            if(-not $policy) {
                Add-CatalogIssue $issues "warning" $profile.name "families" $family.id -1 "no validation policy for family"
                continue
            }

            ++$checkedTileSets
            $checkedUniqueTiles += Validate-CatalogTileSet $issues $metadata $profile.name $profile.supportsWater "families" $family.id $family.tiles $policy
        }
    }

    $errors = @($issues | Where-Object { $_.Severity -eq "error" })
    $warnings = @($issues | Where-Object { $_.Severity -eq "warning" })

    return [ordered]@{
        ok = $errors.Count -eq 0
        path = $catalog.path
        checkedProfiles = $catalog.profiles.Count
        checkedTileSets = $checkedTileSets
        checkedUniqueTiles = $checkedUniqueTiles
        errors = $errors
        warnings = $warnings
    }
}

function Write-CatalogValidationReport($Report, [bool]$AsMarkdown) {
    if($AsMarkdown) {
        Write-Line "## Tile Catalog Validation"
        Write-Line ""
        Write-Line "- Catalog: ``$($Report.path)``"
        Write-Line "- Profiles checked: $($Report.checkedProfiles)"
        Write-Line "- Tile sets checked: $($Report.checkedTileSets)"
        Write-Line "- Unique tile references checked: $($Report.checkedUniqueTiles)"
        Write-Line "- Errors: $($Report.errors.Count)"
        Write-Line "- Warnings: $($Report.warnings.Count)"
        Write-Line ""
    }
    else {
        Write-Line ""
        Write-Line "[tile catalog validation] catalog=$($Report.path)"
        Write-Line "profiles=$($Report.checkedProfiles) tileSets=$($Report.checkedTileSets) uniqueTileRefs=$($Report.checkedUniqueTiles) errors=$($Report.errors.Count) warnings=$($Report.warnings.Count)"
    }

    foreach($issue in $Report.errors + $Report.warnings) {
        if($AsMarkdown) {
            Write-Line "- [$($issue.Severity)] `$($issue.Terrain).$($issue.Section).$($issue.Key)` tile `$($issue.Tile)`: $($issue.Message)"
        }
        else {
            Write-Line "$($issue.Severity): $($issue.Terrain).$($issue.Section).$($issue.Key) tile=$($issue.Tile) $($issue.Message)"
        }
    }

    if($AsMarkdown) {
        Write-Line ""
    }
}

if($JsonPath) {
    $export = Build-ProfileExport
    $resolvedJsonPath = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($JsonPath)
    $jsonFolder = Split-Path -Parent $resolvedJsonPath

    if($jsonFolder -and -not (Test-Path -LiteralPath $jsonFolder)) {
        New-Item -ItemType Directory -Path $jsonFolder -Force | Out-Null
    }

    $export | ConvertTo-Json -Depth 12 | Set-Content -LiteralPath $resolvedJsonPath -Encoding UTF8
}

$catalogValidationReport = $null
if($ValidateCatalog) {
    $catalogValidationReport = Invoke-CatalogValidation $CatalogPath
}

if($Markdown) {
    Write-Line "# Random Map Tileset Profile"
    Write-Line ""
    Write-Line "Source root: ``$DataRoot``"
    Write-Line ""
    Write-Line "Direct raw maps scanned: $valid. Ignored packed/resource-layer duplicate maps: $ignored."
    Write-Line ""
    Write-Line "Ignored reasons: $((Top-Counts $ignoredReasons 10) -join ', ')"
    Write-Line ""
}
else {
    Write-Line "valid=$valid ignored=$ignored root=$DataRoot"
    Write-Line "ignored reasons: $((Top-Counts $ignoredReasons 10) -join ', ')"
}

foreach($key in ($stats.Keys | Sort-Object)) {
    $entry = $stats[$key]
    $terrain = ($key -split "-")[0]
    $metadata = Load-TerrainMetadata $DataRoot $terrain
    $topTiles = Top-Counts $entry.counts $TopCount
    $topPairs = Top-Counts $entry.pairs $TopCount
    $topTileIds = @($entry.counts.GetEnumerator() | Sort-Object Value -Descending | Select-Object -First $TopCount | ForEach-Object { [int]$_.Key })
    $topSafeWalkable = Top-CategoryTiles $entry.counts $metadata "safeWalkable" $TopCount
    $topWater = Top-CategoryTiles $entry.counts $metadata "water" $TopCount
    $topWaterEdge = Top-CategoryTiles $entry.counts $metadata "waterEdge" $TopCount
    $topQuicksand = Top-CategoryTiles $entry.counts $metadata "quicksand" $TopCount
    $topQuicksandEdge = Top-CategoryTiles $entry.counts $metadata "quicksandEdge" $TopCount
    $topSink = Top-CategoryTiles $entry.counts $metadata "sink" $TopCount
    $topDrop = Top-CategoryTiles $entry.counts $metadata "drop" $TopCount
    $topDrop2 = Top-CategoryTiles $entry.counts $metadata "drop2" $TopCount
    $topBlocked = Top-CategoryTiles $entry.counts $metadata "blocked" $TopCount
    $topSemanticPairs = @((Semantic-AdjacentPairCounts $entry.pairs $metadata) |
        Select-Object -First $TopCount |
        ForEach-Object { "$($_.key)=$($_.count)" })
    $topTransitionCandidates = @((Transition-Candidates $entry.pairs $metadata $TopCount) |
        ForEach-Object { "$($_.relation)=$($_.count)" })

    if($Markdown) {
        Write-Line "## $key"
        Write-Line ""
        Write-Line "- Maps: $($entry.maps)"
        Write-Line "- Tiles: $($entry.tiles)"
        Write-Line "- Unique tile IDs: $($entry.counts.Count)"
        Write-Line "- Unique adjacent pairs: $($entry.pairs.Count)"
        if($metadata) {
            Write-Line "- Collision source: ``$($metadata.ResourceFolder)``"
        }
        Write-Line "- Examples: $(($entry.examples | ForEach-Object { Split-Path $_ -Leaf }) -join ', ')"
        Write-Line "- Top tiles: $($topTiles -join ', ')"
        Write-Line "- Top adjacent pairs: $($topPairs -join ', ')"
        Write-Line "- Top safe walkable tiles: $($topSafeWalkable -join ', ')"
        Write-Line "- Top water tiles: $($topWater -join ', ')"
        Write-Line "- Top water-edge tiles: $($topWaterEdge -join ', ')"
        Write-Line "- Top quicksand tiles: $($topQuicksand -join ', ')"
        Write-Line "- Top quicksand-edge tiles: $($topQuicksandEdge -join ', ')"
        Write-Line "- Top sink tiles: $($topSink -join ', ')"
        Write-Line "- Top drop tiles: $($topDrop -join ', ')"
        Write-Line "- Top drop2 tiles: $($topDrop2 -join ', ')"
        Write-Line "- Top blocked tiles: $($topBlocked -join ', ')"
        Write-Line "- Top semantic adjacent pairs: $($topSemanticPairs -join ', ')"
        Write-Line "- Top transition candidates: $($topTransitionCandidates -join ', ')"
        Write-Line ""
        Write-Line "| Tile | Uses | Class | HIT | Features | Walk blocked | Drive blocked | Fly blocked | Destructible | Destroys to |"
        Write-Line "| ---: | ---: | --- | ---: | --- | --- | --- | --- | --- | ---: |"
    }
    else {
        Write-Line ""
        Write-Line "[$key] maps=$($entry.maps) tiles=$($entry.tiles) uniqueTiles=$($entry.counts.Count) uniquePairs=$($entry.pairs.Count)"
        if($metadata) {
            Write-Line "collision source: $($metadata.ResourceFolder)"
        }
        Write-Line ("examples: " + (($entry.examples | ForEach-Object { Split-Path $_ -Leaf }) -join ", "))
        Write-Line ("top tiles: " + ($topTiles -join ", "))
        Write-Line ("top pairs: " + ($topPairs -join ", "))
        Write-Line ("top safe walkable: " + ($topSafeWalkable -join ", "))
        Write-Line ("top water: " + ($topWater -join ", "))
        Write-Line ("top waterEdge: " + ($topWaterEdge -join ", "))
        Write-Line ("top quicksand: " + ($topQuicksand -join ", "))
        Write-Line ("top quicksandEdge: " + ($topQuicksandEdge -join ", "))
        Write-Line ("top sink: " + ($topSink -join ", "))
        Write-Line ("top drop: " + ($topDrop -join ", "))
        Write-Line ("top drop2: " + ($topDrop2 -join ", "))
        Write-Line ("top blocked: " + ($topBlocked -join ", "))
        Write-Line ("top semantic pairs: " + ($topSemanticPairs -join ", "))
        Write-Line ("top transition candidates: " + ($topTransitionCandidates -join ", "))
    }

    foreach($tile in $topTileIds) {
        $behavior = Tile-Behavior $metadata $tile
        $uses = $entry.counts[$tile]

        if($Markdown) {
            if($behavior) {
                Write-Line "| $tile | $uses | $(Tile-Category $behavior) | $($behavior.Hit) | $($behavior.FeatureNames) | $($behavior.WalkBlocked) | $($behavior.DriveBlocked) | $($behavior.FlyBlocked) | $($behavior.Destructible) | $($behavior.DestroysTo) |"
            }
            else {
                Write-Line "| $tile | $uses | unknown | | | | | | | |"
            }
        }
        else {
            if($behavior) {
                Write-Line ("tile {0,3}: uses={1,-6} class={2,-14} hit={3,-6} features={4,-18} walkBlocked={5,-5} driveBlocked={6,-5} flyBlocked={7,-5} destructible={8,-5} destroysTo={9}" -f $tile, $uses, (Tile-Category $behavior), $behavior.Hit, $behavior.FeatureNames, $behavior.WalkBlocked, $behavior.DriveBlocked, $behavior.FlyBlocked, $behavior.Destructible, $behavior.DestroysTo)
            }
            else {
                Write-Line ("tile {0,3}: uses={1}" -f $tile, $uses)
            }
        }
    }

    if($Markdown) {
        Write-Line ""
    }
}

if($catalogValidationReport) {
    Write-CatalogValidationReport $catalogValidationReport $Markdown

    if(-not $catalogValidationReport.ok) {
        exit 1
    }
}
