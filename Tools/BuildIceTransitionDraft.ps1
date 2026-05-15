<#
.SYNOPSIS
Group ice tiles by (pair, bm) into transition rule blocks.

.DESCRIPTION
Reads the IceTileClassTable.txt produced by BuildIceTileTable.ps1 and emits
one block per (pair, bm) combination.
#>
param(
    [string]$InFile  = (Join-Path $PSScriptRoot '..\Documentation\IceTileClassTable.txt'),
    [string]$OutFile = (Join-Path $PSScriptRoot '..\Documentation\IceTransitionDraft.txt')
)

$ErrorActionPreference = 'Stop'

$rows = New-Object 'System.Collections.Generic.List[object]'
foreach($line in [IO.File]::ReadAllLines($InFile)) {
    if(-not $line.StartsWith('tile ')) { continue }
    # Use named tokens "pair=" and "bm=" as anchors
    $pIdx = $line.IndexOf('pair=')
    $bIdx = $line.IndexOf('bm=')
    $hIdx = $line.IndexOf('hit=0x')
    if($pIdx -lt 0 -or $bIdx -lt 0 -or $hIdx -lt 0) { continue }

    $tile = [int]($line.Substring(5, $hIdx - 5).Trim())
    $hitStr = $line.Substring($hIdx + 6, 4)
    $pairEnd = $line.IndexOf(' ', $pIdx)
    if($pairEnd -lt 0) { $pairEnd = $bIdx }
    $pairStr = $line.Substring($pIdx + 5, $pairEnd - ($pIdx + 5))
    $bmStr = $line.Substring($bIdx + 3).Trim()

    $rows.Add([pscustomobject]@{
        Tile = $tile
        Hit  = $hitStr
        Pair = $pairStr
        Bm   = $bmStr
    })
}

$out = New-Object 'System.Collections.Generic.List[string]'
$out.Add('# Ice transition draft')
$out.Add('# Generated from IceTileClassTable.txt')
$out.Add('# bm bit order: NW N NE W E SW S SE  (1 = octant dominated by class A)')
$out.Add('')

$transitions = @('snow-ice', 'ice-shallow', 'shallow-deep', 'snow-shallow', 'snow-deep', 'ice-deep')
foreach($pair in $transitions) {
    $tiles = @($rows | Where-Object { $_.Pair -eq $pair })
    if($tiles.Count -eq 0) { continue }
    $parts = $pair -split '-'
    $a = $parts[0]; $b = $parts[1]
    $out.Add("===== bm_cf1_${a}_${b} (center=$b, bm bit 1 = neighbour is $a) =====")
    $out.Add("# $($tiles.Count) tiles")
    $out.Add('')

    $byBm = $tiles | Group-Object -Property Bm | Sort-Object Name
    foreach($g in $byBm) {
        $ids = @($g.Group | ForEach-Object { $_.Tile } | Sort-Object)
        $out.Add(("  bm={0}  tiles=[{1}]" -f $g.Name, ($ids -join ', ')))
    }
    $out.Add('')
}

$out.Add('===== Pure-class tile pools (candidate centers) =====')
foreach($cls in @('snow', 'ice', 'shallow', 'deep')) {
    $pTiles = @($rows | Where-Object { $_.Pair -eq "pure-$cls" })
    if($pTiles.Count -eq 0) { continue }
    $ids = @($pTiles | ForEach-Object { $_.Tile } | Sort-Object)
    $out.Add(("  pure-$cls : tiles=[{0}]" -f ($ids -join ', ')))
}
$out.Add('')

$out.Add('===== Pair counts =====')
$grp = $rows | Group-Object -Property Pair | Sort-Object Name
foreach($g in $grp) {
    $out.Add(("  {0,-22} {1,4}" -f $g.Name, $g.Count))
}

[System.IO.File]::WriteAllLines($OutFile, $out)
Write-Output "wrote=$OutFile rows=$($rows.Count)"
