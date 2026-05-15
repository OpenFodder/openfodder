param(
    [string]$Root = "D:\Games\OpenFodder\Data",
    [int]$TopCount = 30
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

function Add-Count($Table, $Key, [int]$Amount = 1) {
    if(-not $Table.ContainsKey($Key)) {
        $Table[$Key] = 0
    }

    $Table[$Key] += $Amount
}

function Top-Counts($Table, [int]$Count) {
    return ($Table.GetEnumerator() |
        Sort-Object Value -Descending |
        Select-Object -First $Count |
        ForEach-Object { "$($_.Key)=$($_.Value)" }) -join ", "
}

$stats = @{}
$valid = 0
$invalid = 0

Get-ChildItem -Path $Root -Recurse -Filter *.map -ErrorAction SilentlyContinue | ForEach-Object {
    $bytes = [IO.File]::ReadAllBytes($_.FullName)

    if($bytes.Length -lt 0x60) {
        ++$script:invalid
        return
    }

    $prefix = [Text.Encoding]::ASCII.GetString($bytes, 0, 3).ToLowerInvariant()
    if(-not $terrainPrefixes.ContainsKey($prefix)) {
        $prefix = "jun"
    }

    $terrain = $terrainPrefixes[$prefix]
    $sub = if($bytes[0x10] -eq [byte][char]'1') { "1" } else { "0" }
    $key = "$terrain-sub$sub"
    $width = ($bytes[0x54] -shl 8) + $bytes[0x55]
    $height = ($bytes[0x56] -shl 8) + $bytes[0x57]
    $expected = 0x60 + ($width * $height * 2)

    if($width -le 0 -or $height -le 0 -or $width -gt 512 -or $height -gt 512 -or $expected -ne $bytes.Length) {
        ++$script:invalid
        return
    }

    if(-not $stats.ContainsKey($key)) {
        $stats[$key] = [ordered]@{
            maps = 0
            tiles = 0
            counts = @{}
            pairs = @{}
            examples = @()
        }
    }

    $entry = $stats[$key]
    ++$entry.maps
    $entry.tiles += ($width * $height)
    if($entry.examples.Count -lt 4) {
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

Write-Output "valid=$valid invalid=$invalid root=$Root"

foreach($key in ($stats.Keys | Sort-Object)) {
    $entry = $stats[$key]

    Write-Output ""
    Write-Output "[$key] maps=$($entry.maps) tiles=$($entry.tiles) uniqueTiles=$($entry.counts.Count) uniquePairs=$($entry.pairs.Count)"
    Write-Output ("examples: " + (($entry.examples | ForEach-Object { Split-Path $_ -Leaf }) -join ", "))
    Write-Output ("top tiles: " + (Top-Counts $entry.counts $TopCount))
    Write-Output ("top pairs: " + (Top-Counts $entry.pairs $TopCount))
}
