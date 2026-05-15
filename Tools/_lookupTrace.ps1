param(
    [string]$CharMap = 'Run/icecharmap_1968570037.txt',
    [string]$Map = 'Run/random.map'
)
$cells = @('72,0','90,0','15,70','32,71','33,70','30,52','29,53')
$char = Get-Content $CharMap
$h = 3
function Get-Char1($x, $y, $lines, $hdr, $defaultChar) {
    if($y -lt 0 -or $y -ge 72) { return $defaultChar }
    $line = $lines[$hdr + $y]
    if($x -lt 0 -or $x -ge $line.Length) { return $defaultChar }
    $c = $line[$x]
    if($null -eq $c) { return $defaultChar }
    return $c
}

$bytes = [IO.File]::ReadAllBytes($Map)
$w = ([int]$bytes[0x54] -shl 8) -bor [int]$bytes[0x55]
function Get-Tile1($x, $y, $bytes, $w) {
    $o = 0x60 + (($y * $w) + $x) * 2
    return ([int]$bytes[$o] -shl 8) -bor [int]$bytes[$o + 1]
}

# Compute bm for each rule pass.
# Order: NW N NE W E SW S SE (MSB first).
function Get-BM($x, $y, $center, $aliases, $ground, $extras, $char, $h) {
    $offsets = @(@(-1,-1),@(0,-1),@(1,-1),@(-1,0),@(1,0),@(-1,1),@(0,1),@(1,1))
    $centers = @($center) + $aliases
    $grounds = @($ground)
    $allowed = $centers + $grounds + $extras
    $bits = ''
    foreach($d in $offsets) {
        $c = Get-Char1 ($x+$d[0]) ($y+$d[1]) $char $h $center
        if($allowed -notcontains $c) { return $null }
        if($centers -contains $c) { $bits += '0' } else { $bits += '1' }
    }
    return $bits
}

foreach($cell in $cells) {
    $xy = $cell.Split(',')
    $x = [int]$xy[0]; $y = [int]$xy[1]
    $c = Get-Char1 $x $y $char $h '#'
    $t = Get-Tile1 $x $y $bytes $w
    $nb = ''
    foreach($d in @(@(-1,-1),@(0,-1),@(1,-1),@(-1,0),@(1,0),@(-1,1),@(0,1),@(1,1))) {
        $nb += (Get-Char1 ($x+$d[0]) ($y+$d[1]) $char $h $c)
    }

    $rule = ''
    $bm = $null
    if($c -eq '.') {
        $rule = 'deepWater'
        $bm = Get-BM $x $y '.' @() '~' @('W','#','+') $char $h
    } elseif($c -eq '~') {
        $rule = 'shallowWater'
        $bm = Get-BM $x $y '~' @('.') 'W' @('#','+') $char $h
    } elseif($c -eq 'W') {
        $rule = 'wetIce'
        $bm = Get-BM $x $y 'W' @('.','~') '#' @('+') $char $h
    } else {
        $rule = '(no transition rule)'
    }
    Write-Output ("({0},{1}) char={2} 8nb={3} placed_tile={4}  rule={5}  bm={6}" -f $x, $y, $c, $nb, $t, $rule, $bm)
}
