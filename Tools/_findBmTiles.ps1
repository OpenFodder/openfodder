$content = Get-Content Documentation/IceTilePixelTable.txt
$lastTile = $null
$lastClass = $null
foreach($line in $content) {
    if($line -match '^tile\s+(\d+).*pair=(\w+)') {
        $lastTile = [int]$Matches[1]
        $lastClass = $Matches[2]
    } elseif($line -match 'snow_ice=(\d+)\s+ice_shallow=(\d+)\s+shallow_deep=(\d+)') {
        $tile = $lastTile
        $isSnowIce = ($tile -le 25) -or ($tile -in 33..34) -or ($tile -in 42..44) -or ($tile -in 62..63)
        $isIceShallow = ($tile -in 80..99) -or ($tile -in 112..113) -or ($tile -in 128..131) -or ($tile -in 148..151)
        $isShallowDeep = ($tile -in 100..111) -or ($tile -in 114..119) -or ($tile -in 132..139) -or ($tile -in 156..159) -or ($tile -in 176..179)
        $sBM = $Matches[1]; $iBM = $Matches[2]; $dBM = $Matches[3]
        if($isSnowIce -and ($sBM -eq '11000000' -or $sBM -eq '00001000')) { Write-Output "snow_ice tile=$tile bm=$sBM pair=$lastClass" }
        if($isIceShallow -and ($iBM -eq '11100000' -or $iBM -eq '00000111' -or $iBM -eq '11000000')) { Write-Output "ice_shallow tile=$tile bm=$iBM pair=$lastClass" }
        if($isShallowDeep -and ($dBM -eq '10010111' -or $dBM -eq '00000111' -or $dBM -eq '11100000')) { Write-Output "shallow_deep tile=$tile bm=$dBM pair=$lastClass" }
    }
}
