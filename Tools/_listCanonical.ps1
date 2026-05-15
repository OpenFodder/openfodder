param([string]$Class = 'ice_shallow')
$content = Get-Content Documentation/IceTilePixelTable.txt
$lastTile = $null
$lastPair = $null
foreach($line in $content) {
    if($line -match '^tile\s+(\d+).*pair=(\w+)') {
        $lastTile = [int]$Matches[1]
        $lastPair = $Matches[2]
    } elseif($line -match 'snow_ice=(\S+)\s+ice_shallow=(\S+)\s+shallow_deep=(\S+)') {
        $tile = $lastTile
        $isSnowIce = ($tile -le 25) -or ($tile -in 33..34) -or ($tile -in 42..44) -or ($tile -in 62..63)
        $isIceShallow = ($tile -in 80..99) -or ($tile -in 112..113) -or ($tile -in 128..131) -or ($tile -in 148..151)
        $isShallowDeep = ($tile -in 100..111) -or ($tile -in 114..119) -or ($tile -in 132..139) -or ($tile -in 156..159) -or ($tile -in 176..179)
        if($Class -eq 'snow_ice' -and $isSnowIce) {
            Write-Output ("tile={0,3}  bm={1}  pair={2}" -f $tile, $Matches[1], $lastPair)
        } elseif($Class -eq 'ice_shallow' -and $isIceShallow) {
            Write-Output ("tile={0,3}  bm={1}  pair={2}" -f $tile, $Matches[2], $lastPair)
        } elseif($Class -eq 'shallow_deep' -and $isShallowDeep) {
            Write-Output ("tile={0,3}  bm={1}  pair={2}" -f $tile, $Matches[3], $lastPair)
        }
    }
}
