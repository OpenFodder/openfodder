param([int]$YStart, [int]$YEnd, [int]$XStart, [int]$XEnd)
$lines = Get-Content Run/icecharmap_1968570037.txt
Write-Output ("    " + (-join (($XStart..$XEnd) | ForEach-Object { ($_ % 10).ToString() })))
for($y = $YStart; $y -le $YEnd; $y++) {
    $line = $lines[3 + $y]
    $end = [Math]::Min($XEnd, $line.Length - 1)
    $sub = $line.Substring($XStart, $end - $XStart + 1)
    Write-Output ("y={0,2} {1}" -f $y, $sub)
}
