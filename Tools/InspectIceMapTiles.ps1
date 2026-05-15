<#
.SYNOPSIS
Tabulate tile-id usage at bank/wet/water cells in the regenerated map.

.DESCRIPTION
Reads Run/random.map and Run/icecharmap_<seed>.txt for the same seed and emits
a count of which tile IDs are appearing at each char-class. This is a quick
sanity check that the new bm rules are firing (e.g. shallow cells should be
using 80-99/112/113/128-131/148-151 plus pure-shallow 111/114/115).
#>
param(
    [string]$MapPath = (Join-Path $PSScriptRoot '..\Run\random.map'),
    [uint32]$Seed = 1968570037,
    [string]$CharPath = (Join-Path $PSScriptRoot ("..\Run\icecharmap_" + $Seed + ".txt")),
    [int]$Width = 96,
    [int]$Height = 72
)

$ErrorActionPreference = 'Stop'

$bytes = [IO.File]::ReadAllBytes($MapPath)
$tiles = New-Object 'int[]' ($Width * $Height)
for($i = 0; $i -lt $Width * $Height; ++$i) {
    $hi = $bytes[0x60 + $i * 2]
    $lo = $bytes[0x60 + $i * 2 + 1]
    $tiles[$i] = ([int]$hi -shl 8) -bor [int]$lo
}

$rawLines = [IO.File]::ReadAllLines($CharPath)
$charLines = New-Object 'System.Collections.Generic.List[string]'
foreach($line in $rawLines) {
    # Comment lines start with "# " (hash followed by space). Data rows can be
    # all-hash (pure ground) so we can't filter on first char alone.
    if($line.StartsWith('# ')) { continue }
    if($line.Length -lt $Width) { continue }
    [void]$charLines.Add($line)
}
if($charLines.Count -lt $Height) {
    throw "Expected $Height char rows, got $($charLines.Count)"
}

$counts = @{}
$bankList = @()
$wetList = @()
for($y = 0; $y -lt $Height; ++$y) {
    $row = $charLines[$y]
    for($x = 0; $x -lt $Width; ++$x) {
        $ch = $row[$x]
        $tile = $tiles[$y * $Width + $x]
        if(-not $counts.ContainsKey($ch)) { $counts[$ch] = @{} }
        if(-not $counts[$ch].ContainsKey($tile)) { $counts[$ch][$tile] = 0 }
        $counts[$ch][$tile]++
        if($ch -eq '~' -and $tile -eq 84) { $bankList += "$x,$y" }
        if($ch -eq 'W') { $wetList += "$x,$y=$tile" }
    }
}

Write-Host ("rows={0}  keys={1}" -f $charLines.Count, ($counts.Keys -join ','))
foreach($ch in @([char]'.', [char]'~', [char]'W', [char]'#', [char]'+', [char]'T')) {
    if(-not $counts.ContainsKey($ch)) { continue }
    $totals = $counts[$ch]
    $sum = 0; foreach($v in $totals.Values) { $sum += $v }
    Write-Host ('=== char {0} : {1} cells ===' -f $ch, $sum)
    $top = $totals.GetEnumerator() | Sort-Object -Property Value -Descending | Select-Object -First 12
    foreach($e in $top) {
        Write-Host ('  tile {0,3} : {1,4}' -f $e.Key, $e.Value)
    }
}
