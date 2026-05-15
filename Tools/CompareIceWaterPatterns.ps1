param(
    [string]$DataRoot = "D:\Games\OpenFodder\Data\Amiga",
    [int[]]$Seeds = @(1968570037),
    [int]$Width = 96,
    [int]$Height = 72,
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
    [int]$Top = 30
)

$ErrorActionPreference = "Stop"

$waterFillTiles = @(100, 101, 102)
$deepWaterTiles = @(100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 114, 115, 116, 117, 118, 119, 132, 133, 134, 135, 136, 137, 138, 139, 156, 157, 158, 159, 176, 177, 178, 179)
$shallowWaterTiles = @(80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 112, 113, 128, 129, 130, 131, 148, 149, 150, 151)
$wetIceTiles = @(3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 22, 23, 24, 25, 42, 43, 44, 45, 62, 63)
$authoredWetBankTiles = @(20, 21, 33, 34)

function New-Set([int[]]$Values) {
    $set = @{}
    foreach($value in $Values) {
        $set[$value] = $true
    }
    return $set
}

$waterFillSet = New-Set $waterFillTiles
$deepWaterSet = New-Set $deepWaterTiles
$shallowWaterSet = New-Set ($shallowWaterTiles + $authoredWetBankTiles)
$wetIceSet = New-Set ($wetIceTiles + $authoredWetBankTiles)

function Add-Count($Table, [string]$Key, [int]$Amount = 1) {
    if(-not $Table.ContainsKey($Key)) {
        $Table[$Key] = 0
    }
    $Table[$Key] += $Amount
}

function Read-RawMap([string]$Path) {
    $bytes = [IO.File]::ReadAllBytes($Path)
    $width = ($bytes[0x54] -shl 8) -bor $bytes[0x55]
    $height = ($bytes[0x56] -shl 8) -bor $bytes[0x57]
    $tiles = New-Object int[] ($width * $height)
    $offset = 0x60

    for($index = 0; $index -lt $tiles.Length; ++$index) {
        $tiles[$index] = ($bytes[$offset + ($index * 2)] -shl 8) -bor $bytes[$offset + ($index * 2) + 1]
    }

    return [pscustomobject]@{
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
            for($y = 0; $y -lt $current.Height; ++$y) {
                $row = $current.Rows[$y]
                for($x = 0; $x -lt $current.Width; ++$x) {
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

function Tile-Class([int]$Tile) {
    if($deepWaterSet.ContainsKey($Tile)) { return "D" }
    if($shallowWaterSet.ContainsKey($Tile)) { return "S" }
    if($wetIceSet.ContainsKey($Tile)) { return "W" }
    if($Tile -eq 0 -or $Tile -eq 1) { return "G" }
    return "."
}

function Is-WaterRelated([int]$Tile) {
    return $deepWaterSet.ContainsKey($Tile) -or $shallowWaterSet.ContainsKey($Tile) -or $wetIceSet.ContainsKey($Tile)
}

function Neighbour-Mask($Map, [int]$X, [int]$Y, [string]$InsideClass) {
    $offsets = @(
        @(-1, -1), @(0, -1), @(1, -1),
        @(-1, 0),           @(1, 0),
        @(-1, 1),  @(0, 1),  @(1, 1)
    )
    $mask = ""
    foreach($offset in $offsets) {
        $class = Tile-Class (Get-Tile $Map ($X + $offset[0]) ($Y + $offset[1]))
        $mask += if($class -eq $InsideClass) { "0" } else { "1" }
    }
    return $mask
}

function Role-Signature($Map, [int]$X, [int]$Y) {
    $rows = @()
    for($dy = -1; $dy -le 1; ++$dy) {
        $row = ""
        for($dx = -1; $dx -le 1; ++$dx) {
            $row += Tile-Class (Get-Tile $Map ($X + $dx) ($Y + $dy))
        }
        $rows += $row
    }
    return ($rows -join "/")
}

function New-Stats([string]$Name) {
    [pscustomobject]@{
        Name = $Name
        Maps = 0
        WaterCells = 0
        DeepEdgeCells = 0
        ShallowCells = 0
        WetCells = 0
        Tiles = @{}
        DeepMasks = @{}
        ShallowMasks = @{}
        WetMasks = @{}
        DeepMaskTiles = @{}
        ShallowMaskTiles = @{}
        WetMaskTiles = @{}
        RoleSignatures = @{}
        HorizontalPairs = @{}
        VerticalPairs = @{}
    }
}

function Record-Map($Stats, $Map) {
    ++$Stats.Maps

    for($y = 0; $y -lt $Map.Height; ++$y) {
        for($x = 0; $x -lt $Map.Width; ++$x) {
            $tile = Get-Tile $Map $x $y
            if(-not (Is-WaterRelated $tile)) {
                continue
            }

            Add-Count $Stats.Tiles ([string]$tile)
            Add-Count $Stats.RoleSignatures (Role-Signature $Map $x $y)

            $right = Get-Tile $Map ($x + 1) $y
            $below = Get-Tile $Map $x ($y + 1)
            if(Is-WaterRelated $right) {
                Add-Count $Stats.HorizontalPairs "$tile|$right"
            }
            if(Is-WaterRelated $below) {
                Add-Count $Stats.VerticalPairs "$tile>$below"
            }

            if($deepWaterSet.ContainsKey($tile)) {
                ++$Stats.WaterCells
                if(-not $waterFillSet.ContainsKey($tile)) {
                    ++$Stats.DeepEdgeCells
                }
                $mask = Neighbour-Mask $Map $x $y "D"
                Add-Count $Stats.DeepMasks $mask
                Add-Count $Stats.DeepMaskTiles "$mask=$tile"
            } elseif($shallowWaterSet.ContainsKey($tile)) {
                ++$Stats.ShallowCells
                $mask = Neighbour-Mask $Map $x $y "S"
                Add-Count $Stats.ShallowMasks $mask
                Add-Count $Stats.ShallowMaskTiles "$mask=$tile"
            } elseif($wetIceSet.ContainsKey($tile)) {
                ++$Stats.WetCells
                $mask = Neighbour-Mask $Map $x $y "W"
                Add-Count $Stats.WetMasks $mask
                Add-Count $Stats.WetMaskTiles "$mask=$tile"
            }
        }
    }
}

function Format-Top($Table, [int]$Count = $Top) {
    $items = $Table.GetEnumerator() |
        Sort-Object -Property @{ Expression = "Value"; Descending = $true }, @{ Expression = "Name"; Descending = $false } |
        Select-Object -First $Count
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
Write-Output "summary authoredWater=$($authoredStats.WaterCells) authoredDeepEdges=$($authoredStats.DeepEdgeCells) authoredShallow=$($authoredStats.ShallowCells) authoredWet=$($authoredStats.WetCells)"
Write-Output "summary generatedWater=$($generatedStats.WaterCells) generatedDeepEdges=$($generatedStats.DeepEdgeCells) generatedShallow=$($generatedStats.ShallowCells) generatedWet=$($generatedStats.WetCells)"
Write-Output "authored tiles: $(Format-Top $authoredStats.Tiles)"
Write-Output "generated tiles: $(Format-Top $generatedStats.Tiles)"
Write-Output "authored deep masks: $(Format-Top $authoredStats.DeepMasks)"
Write-Output "authored deep mask tiles: $(Format-Top $authoredStats.DeepMaskTiles)"
Write-Output "generated deep masks: $(Format-Top $generatedStats.DeepMasks)"
Write-Output "generated-only deep mask tiles: $(Format-GeneratedOnly $generatedStats.DeepMaskTiles $authoredStats.DeepMaskTiles)"
Write-Output "authored shallow masks: $(Format-Top $authoredStats.ShallowMasks)"
Write-Output "authored shallow mask tiles: $(Format-Top $authoredStats.ShallowMaskTiles)"
Write-Output "generated shallow masks: $(Format-Top $generatedStats.ShallowMasks)"
Write-Output "generated-only shallow mask tiles: $(Format-GeneratedOnly $generatedStats.ShallowMaskTiles $authoredStats.ShallowMaskTiles)"
Write-Output "authored wet masks: $(Format-Top $authoredStats.WetMasks)"
Write-Output "authored wet mask tiles: $(Format-Top $authoredStats.WetMaskTiles)"
Write-Output "generated wet masks: $(Format-Top $generatedStats.WetMasks)"
Write-Output "generated-only wet mask tiles: $(Format-GeneratedOnly $generatedStats.WetMaskTiles $authoredStats.WetMaskTiles)"
Write-Output "generated-only signatures: $(Format-GeneratedOnly $generatedStats.RoleSignatures $authoredStats.RoleSignatures)"
Write-Output "generated-only horizontal pairs: $(Format-GeneratedOnly $generatedStats.HorizontalPairs $authoredStats.HorizontalPairs)"
Write-Output "generated-only vertical pairs: $(Format-GeneratedOnly $generatedStats.VerticalPairs $authoredStats.VerticalPairs)"
