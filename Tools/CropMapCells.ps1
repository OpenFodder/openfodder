<#
.SYNOPSIS
Crop a window of cells around each given map coordinate from a tiles.png render
into individual files. Window is 2*Pad+1 cells wide/tall.
#>

param(
    [Parameter(Mandatory = $true)][string]$Source,
    [Parameter(Mandatory = $true)][string]$OutputDir,
    [Parameter(Mandatory = $true)][string]$Prefix,
    [Parameter(Mandatory = $true)][string[]]$Cells, # "x,y"
    [int]$CellPx = 48,                              # 16 * scale=3
    [int]$Pad   = 4
)

$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.Drawing

if(-not (Test-Path -LiteralPath $OutputDir)) {
    New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null
}

$src = [System.Drawing.Image]::FromFile((Resolve-Path -LiteralPath $Source).Path)
try {
    foreach($cell in $Cells) {
        $parts = $cell.Split(',')
        $cx = [int]$parts[0]
        $cy = [int]$parts[1]

        $x = [Math]::Max(0, ($cx - $Pad) * $CellPx)
        $y = [Math]::Max(0, ($cy - $Pad) * $CellPx)
        $w = (2 * $Pad + 1) * $CellPx
        $h = $w

        if(($x + $w) -gt $src.Width)  { $w = $src.Width  - $x }
        if(($y + $h) -gt $src.Height) { $h = $src.Height - $y }

        $rect = New-Object Drawing.Rectangle $x, $y, $w, $h
        $crop = New-Object Drawing.Bitmap $w, $h
        $g = [Drawing.Graphics]::FromImage($crop)
        $g.DrawImage($src, (New-Object Drawing.Rectangle 0, 0, $w, $h), $rect, [Drawing.GraphicsUnit]::Pixel)
        $g.Dispose()

        $out = Join-Path $OutputDir ("{0}_{1}_{2}.png" -f $Prefix, $cx, $cy)
        $crop.Save($out, [Drawing.Imaging.ImageFormat]::Png)
        $crop.Dispose()
        Write-Output ("wrote=$out cell=$cx,$cy window=${w}x${h}")
    }
} finally {
    $src.Dispose()
}
