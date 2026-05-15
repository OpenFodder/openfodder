param(
    [string]$DataRoot = "D:\Games\OpenFodder\Data\Amiga",
    [int[]]$Seeds = @(101, 202, 303, 404, 505),
    [int]$Width = 64,
    [int]$Height = 48,
    [string]$Profile = "ice_outpost",
    [string[]]$AuthoredMaps = @(
        "mapm4.map",
        "mapm9.map",
        "mapm10.map",
        "mapm11.map",
        "mapm14.map",
        "mapm15.map",
        "mapm16.map",
        "mapm21.map",
        "mapm22.map",
        "mapm31.map",
        "mapm32.map",
        "mapm33.map",
        "mapm34.map",
        "mapm35.map",
        "mapm36.map"
    ),
    [int]$Top = 20
)

$ErrorActionPreference = "Stop"

$treeTiles = @(152,153,154,155,170,171,172,173,174,175,190,191,192,193,194,195,196,197,198,199,210,211,212,213,214,215,216,217,218,219,230,231,232,233,234,235,236,237,238,239)
$bottomTiles = @(210,211,212,233,234)
$trunkTiles = @(190,191,192,213,214)
$interiorTiles = @(152,153,170,171,172,193,194,230,231,232)
$topTiles = @(173,174,195,196,197,198,215,216,217,218,219)
$edgeTiles = @(154,155,175,199,235,236,237,238,239)

function New-Set([int[]]$Values) {
    $set = @{}
    foreach($value in $Values) {
        $set[$value] = $true
    }
    return $set
}

$treeSet = New-Set $treeTiles
$bottomSet = New-Set $bottomTiles
$trunkSet = New-Set $trunkTiles
$interiorSet = New-Set $interiorTiles
$topSet = New-Set $topTiles
$edgeSet = New-Set $edgeTiles

function Add-Count($Table, [string]$Key, [int]$Amount = 1) {
    if(-not $Table.ContainsKey($Key)) {
        $Table[$Key] = 0
    }
    $Table[$Key] += $Amount
}

function Role-ForTile([int]$Tile) {
    if($bottomSet.ContainsKey($Tile)) { return "B" }
    if($trunkSet.ContainsKey($Tile)) { return "K" }
    if($interiorSet.ContainsKey($Tile)) { return "I" }
    if($topSet.ContainsKey($Tile)) { return "T" }
    if($edgeSet.ContainsKey($Tile)) { return "E" }
    if($treeSet.ContainsKey($Tile)) { return "C" }
    return "."
}

function New-Stats([string]$Name) {
    [pscustomobject]@{
        Name = $Name
        Maps = 0
        Cells = 0
        TreeCells = 0
        Tiles = @{}
        Roles = @{}
        VerticalPairs = @{}
        HorizontalPairs = @{}
        RoleSignatures = @{}
        TrunkAboveTiles = @{}
        SideMissingBottom = @{}
        BoundaryGround = @{}
        TopLikeAboveTrunk = 0
        BottomWithTreeBelow = 0
    }
}

function Read-RawMap([string]$Path) {
    $bytes = [IO.File]::ReadAllBytes($Path)
    $width = ($bytes[0x54] -shl 8) -bor $bytes[0x55]
    $height = ($bytes[0x56] -shl 8) -bor $bytes[0x57]
    $tiles = New-Object int[] ($width * $height)
    $offset = 0x60

    for($index = 0; $index -lt $tiles.Length; $index++) {
        $tiles[$index] = ($bytes[$offset + ($index * 2)] -shl 8) -bor $bytes[$offset + ($index * 2) + 1]
    }

    [pscustomobject]@{
        Name = [IO.Path]::GetFileName($Path)
        Width = $width
        Height = $height
        Tiles = $tiles
    }
}

function Read-GeneratedMaps {
    $seedArg = ($Seeds -join ",")
    $dumpScript = Join-Path $PSScriptRoot "DumpGeneratedIceMapTiles.js"
    $lines = & cscript //nologo $dumpScript "--seeds=$seedArg" "--width=$Width" "--height=$Height" "--profile=$Profile" 2>&1
    if($LASTEXITCODE -ne 0) {
        throw ($lines -join [Environment]::NewLine)
    }

    $maps = @()
    $current = $null
    foreach($line in $lines) {
        if($line -match '^begin\s+(\S+)\s+(\d+)\s+(\d+)$') {
            $current = [pscustomobject]@{
                Name = $Matches[1]
                Width = [int]$Matches[2]
                Height = [int]$Matches[3]
                Rows = @()
            }
            continue
        }
        if($line -match '^end\s+') {
            $tiles = New-Object int[] ($current.Width * $current.Height)
            for($y = 0; $y -lt $current.Height; $y++) {
                $row = $current.Rows[$y]
                for($x = 0; $x -lt $current.Width; $x++) {
                    $tiles[$y * $current.Width + $x] = [int]$row[$x]
                }
            }
            $maps += [pscustomobject]@{
                Name = $current.Name
                Width = $current.Width
                Height = $current.Height
                Tiles = $tiles
            }
            $current = $null
            continue
        }
        if($current -ne $null -and $line -match '^\d') {
            $current.Rows += ,($line -split ',' | ForEach-Object { [int]$_ })
        }
    }

    return $maps
}

function Get-Tile($Map, [int]$X, [int]$Y) {
    if($X -lt 0 -or $Y -lt 0 -or $X -ge $Map.Width -or $Y -ge $Map.Height) {
        return -1
    }
    return $Map.Tiles[$Y * $Map.Width + $X]
}

function Role-Signature($Map, [int]$X, [int]$Y) {
    $rows = @()
    for($dy = -1; $dy -le 1; $dy++) {
        $row = ""
        for($dx = -1; $dx -le 1; $dx++) {
            $row += Role-ForTile (Get-Tile $Map ($X + $dx) ($Y + $dy))
        }
        $rows += $row
    }
    return ($rows -join "/")
}

function Record-Map($Stats, $Map) {
    $Stats.Maps++
    $Stats.Cells += $Map.Width * $Map.Height

    for($y = 0; $y -lt $Map.Height; $y++) {
        for($x = 0; $x -lt $Map.Width; $x++) {
            $tile = Get-Tile $Map $x $y
            if(-not $treeSet.ContainsKey($tile)) {
                continue
            }

            $Stats.TreeCells++
            Add-Count $Stats.Tiles ([string]$tile)
            Add-Count $Stats.Roles (Role-ForTile $tile)
            Add-Count $Stats.RoleSignatures (Role-Signature $Map $x $y)

            $above = Get-Tile $Map $x ($y - 1)
            $below = Get-Tile $Map $x ($y + 1)
            $right = Get-Tile $Map ($x + 1) $y

            if($treeSet.ContainsKey($below)) {
                Add-Count $Stats.VerticalPairs ("$tile>$below")
            }
            if($treeSet.ContainsKey($right)) {
                Add-Count $Stats.HorizontalPairs ("$tile|$right")
            }
            if($trunkSet.ContainsKey($tile)) {
                Add-Count $Stats.TrunkAboveTiles ([string]$above)
                if($topSet.ContainsKey($above)) {
                    $Stats.TopLikeAboveTrunk++
                }
            }
            if($bottomSet.ContainsKey($tile) -and $treeSet.ContainsKey($below)) {
                $Stats.BottomWithTreeBelow++
            }
            if($tile -eq 213 -and $below -ne 233) {
                Add-Count $Stats.SideMissingBottom ("213>$below")
            }
            if($tile -eq 214 -and $below -ne 234) {
                Add-Count $Stats.SideMissingBottom ("214>$below")
            }

            foreach($edge in @(@(0, -1, "N"), @(1, 0, "E"), @(0, 1, "S"), @(-1, 0, "W"))) {
                $edgeTile = Get-Tile $Map ($x + $edge[0]) ($y + $edge[1])
                if($treeSet.ContainsKey($edgeTile) -or $edgeTile -lt 0) {
                    continue
                }
                Add-Count $Stats.BoundaryGround ("$tile$($edge[2])$edgeTile")
            }
        }
    }
}

function Format-Top($Table, [int]$Count = $Top) {
    $items = $Table.GetEnumerator() | Sort-Object -Property @{ Expression = "Value"; Descending = $true }, @{ Expression = "Name"; Descending = $false } | Select-Object -First $Count
    if(-not $items) {
        return "none"
    }
    return (($items | ForEach-Object { "$($_.Name)=$($_.Value)" }) -join ", ")
}

function Format-GeneratedOnly($Generated, $Authored, [int]$Count = $Top) {
    $items = $Generated.GetEnumerator() |
        Where-Object { -not $Authored.ContainsKey($_.Name) } |
        Sort-Object -Property @{ Expression = "Value"; Descending = $true }, @{ Expression = "Name"; Descending = $false } |
        Select-Object -First $Count

    if(-not $items) {
        return "none"
    }
    return (($items | ForEach-Object { "$($_.Name)=$($_.Value)" }) -join ", ")
}

$authoredStats = New-Stats "authored"
$generatedStats = New-Stats "generated"

foreach($mapName in $AuthoredMaps) {
    $path = if([IO.Path]::IsPathRooted($mapName)) { $mapName } else { Join-Path $DataRoot $mapName }
    Record-Map $authoredStats (Read-RawMap $path)
}

foreach($map in (Read-GeneratedMaps)) {
    Record-Map $generatedStats $map
}

Write-Output "compare profile=$Profile seeds=$(($Seeds -join ',')) size=${Width}x${Height} authoredMaps=$(($AuthoredMaps -join ','))"
Write-Output "summary authoredTreeCells=$($authoredStats.TreeCells) generatedTreeCells=$($generatedStats.TreeCells)"
Write-Output "authored roles: $(Format-Top $authoredStats.Roles)"
Write-Output "generated roles: $(Format-Top $generatedStats.Roles)"
Write-Output "authored tiles: $(Format-Top $authoredStats.Tiles)"
Write-Output "generated tiles: $(Format-Top $generatedStats.Tiles)"
Write-Output "authored trunkAbove: $(Format-Top $authoredStats.TrunkAboveTiles)"
Write-Output "generated trunkAbove: $(Format-Top $generatedStats.TrunkAboveTiles)"
Write-Output "authored sideMissingBottom: $(Format-Top $authoredStats.SideMissingBottom)"
Write-Output "generated sideMissingBottom: $(Format-Top $generatedStats.SideMissingBottom)"
Write-Output "authored boundaryGround: $(Format-Top $authoredStats.BoundaryGround)"
Write-Output "generated boundaryGround: $(Format-Top $generatedStats.BoundaryGround)"
Write-Output "authored topLikeAboveTrunk=$($authoredStats.TopLikeAboveTrunk) bottomWithTreeBelow=$($authoredStats.BottomWithTreeBelow)"
Write-Output "generated topLikeAboveTrunk=$($generatedStats.TopLikeAboveTrunk) bottomWithTreeBelow=$($generatedStats.BottomWithTreeBelow)"
Write-Output "generated-only vertical pairs: $(Format-GeneratedOnly $generatedStats.VerticalPairs $authoredStats.VerticalPairs)"
Write-Output "generated-only horizontal pairs: $(Format-GeneratedOnly $generatedStats.HorizontalPairs $authoredStats.HorizontalPairs)"
Write-Output "generated-only role signatures: $(Format-GeneratedOnly $generatedStats.RoleSignatures $authoredStats.RoleSignatures)"
