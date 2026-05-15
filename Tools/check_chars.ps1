$all = Get-Content -LiteralPath 'Run/icecharmap_1968570037.txt'
# First 3 lines are comments (# Ice char map / # Chars / # Size). Data follows.
$lines = $all | Select-Object -Skip 3 | Where-Object { $_ -ne '' }
$cells = @(@(86,0),@(87,0),@(82,9),@(73,17),@(74,19),@(70,24),@(51,43))
foreach($c in $cells) {
    $x = $c[0]; $y = $c[1]
    if($y -ge $lines.Count) { Write-Output "($x,$y) OUT-OF-RANGE (rows=$($lines.Count))"; continue }
    $row = $lines[$y]
    Write-Output ("--- ($x,$y) char='{0}' ---" -f $row.Substring($x, 1))
    for($dy = -1; $dy -le 1; ++$dy) {
        $yy = $y + $dy
        if($yy -lt 0 -or $yy -ge $lines.Count) { continue }
        $r = $lines[$yy]
        $startX = [Math]::Max(0, $x - 4)
        $endX = [Math]::Min($r.Length - 1, $x + 4)
        $piece = $r.Substring($startX, $endX - $startX + 1)
        $marker = if($dy -eq 0) { '*' } else { ' ' }
        Write-Output ("{0} y={1,2} x={2,2}..{3,2}: {4}" -f $marker, $yy, $startX, $endX, $piece)
    }
    Write-Output ""
}
