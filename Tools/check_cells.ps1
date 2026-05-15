$bytes = [IO.File]::ReadAllBytes('Run/random.map')
$w = ($bytes[0x54] -shl 8) -bor $bytes[0x55]

$all = Get-Content -LiteralPath 'Run/icecharmap_1968570037.txt'
$lines = $all | Select-Object -Skip 3 | Where-Object { $_ -ne '' }

# Print full context around the bend with tile ids
Write-Output "Chars y=12..20, x=66..80:"
for($y = 12; $y -le 20; ++$y) {
    $piece = $lines[$y].Substring(66, 15)
    Write-Output ("y={0,2} : {1}" -f $y, $piece)
}

Write-Output ""
Write-Output "Tile ids y=14..19, x=70..78:"
$header = "      "
for($x = 70; $x -le 78; ++$x) { $header += ("{0,4}" -f $x) }
Write-Output $header
for($y = 14; $y -le 19; ++$y) {
    $row = "y={0,2} :" -f $y
    for($x = 70; $x -le 78; ++$x) {
        $o = 0x60 + ($y * $w + $x) * 2
        $tid = (($bytes[$o] -shl 8) -bor $bytes[$o + 1]) -band 0x1FF
        $row += ("{0,4}" -f $tid)
    }
    Write-Output $row
}

Write-Output ""
Write-Output "Char codes y=14..19, x=70..78:"
$header = "      "
for($x = 70; $x -le 78; ++$x) { $header += ("{0,3}" -f $x) }
Write-Output $header
for($y = 14; $y -le 19; ++$y) {
    $row = "y={0,2} :" -f $y
    for($x = 70; $x -le 78; ++$x) {
        $row += ("{0,3}" -f $lines[$y].Substring($x, 1))
    }
    Write-Output $row
}
