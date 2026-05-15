<#
.SYNOPSIS
Renders an Open Fodder .map as a PNG with each tile's ID overlaid in the top-left.

.DESCRIPTION
Reads the .map header to discover its base/sub tileset (e.g. junbase.blk / junsub0.blk)
and palette (e.g. junbase.pal / embedded), decodes each 16x16 tile, and stitches the
whole map into one PNG. The decimal tile ID (0..511) is drawn in the top-left of each
tile cell so the visual map and the tile-id grid line up.

Auto-detects Amiga (4-bitplane planar, separate .pal) vs PC (8bpp chunky, palette
embedded in BaseBlk at 0xFA00).

.PARAMETER MapPath
Path to the .map file.

.PARAMETER DataDir
Directory holding the matching .blk / .pal files. Defaults to the map's directory.

.PARAMETER OutputPath
PNG output path. Defaults to MapDumps/<CampaignDirName>/<map>.tiles.png in the current working directory.

.PARAMETER Scale
Pixel scale factor per tile (1 = native 16x16 per tile). Default 2 so the overlay text fits.

.PARAMETER NoOverlay
Skip the tile-ID overlay (just render the map).

.PARAMETER Coords
Overlay each tile's (x,y) instead of its tile ID (useful when cross-referencing
a debug dump produced by the smoothing scripts).

.EXAMPLE
./Tools/RenderMapWithTileIds.ps1 -MapPath Run/Data/AmigaAction/mapm1.map

.EXAMPLE
./Tools/RenderMapWithTileIds.ps1 -MapPath Run/random.map -Coords -Scale 3
#>

param(
    [Parameter(Mandatory = $true)][string]$MapPath,
    [string]$DataDir,
    [string]$OutputPath,
    [ValidateRange(1, 8)][int]$Scale = 2,
    [switch]$NoOverlay,
    [switch]$Coords
)

$ErrorActionPreference = 'Stop'

Add-Type -AssemblyName System.Drawing
. (Join-Path $PSScriptRoot 'Lib/RncUnpack.ps1')

if(-not (Test-Path -LiteralPath $MapPath)) {
    throw "Map not found: $MapPath"
}
$MapPath = (Resolve-Path -LiteralPath $MapPath).Path

if(-not $DataDir) {
    # Default to retail Amiga data (Run/Data/Amiga). The randomly-generated
    # ice tilesets target the retail tile pixels, so all renders use that
    # palette/.blk. Override with -DataDir if a different campaign is needed.
    $DataDir = Join-Path (Split-Path -Parent $PSScriptRoot) 'Run/Data/Amiga'
}
if(-not (Test-Path -LiteralPath $DataDir)) {
    throw "Data directory not found: $DataDir"
}

if(-not $OutputPath) {
    $campaign = Split-Path -Leaf (Split-Path -Parent $MapPath)
    $mapStem = [IO.Path]::GetFileNameWithoutExtension($MapPath)
    $dumpDir = Join-Path (Join-Path (Get-Location).Path 'MapDumps') $campaign
    if(-not (Test-Path -LiteralPath $dumpDir)) {
        New-Item -ItemType Directory -Path $dumpDir -Force | Out-Null
    }
    $OutputPath = Join-Path $dumpDir ($mapStem + '.tiles.png')
} else {
    $outParent = Split-Path -Parent $OutputPath
    if($outParent -and -not (Test-Path -LiteralPath $outParent)) {
        New-Item -ItemType Directory -Path $outParent -Force | Out-Null
    }
}

function Read-BEWord([byte[]]$Bytes, [int]$Offset) {
    return ([int]$Bytes[$Offset] -shl 8) -bor [int]$Bytes[$Offset + 1]
}

function Read-AsciiAt([byte[]]$Bytes, [int]$Offset, [int]$Length) {
    $end = $Offset
    $stop = $Offset + $Length
    while($end -lt $stop -and $Bytes[$end] -ne 0) { ++$end }
    return [Text.Encoding]::ASCII.GetString($Bytes, $Offset, $end - $Offset)
}

function Find-DataFile([string]$Dir, [string]$Name) {
    $direct = Join-Path $Dir $Name
    if(Test-Path -LiteralPath $direct) { return (Resolve-Path -LiteralPath $direct).Path }

    $match = Get-ChildItem -LiteralPath $Dir -File -Filter $Name -ErrorAction SilentlyContinue | Select-Object -First 1
    if($match) { return $match.FullName }

    return $null
}

$mapBytes = Read-MaybeRnc -Path $MapPath
if($mapBytes.Length -lt 0x60) { throw "Map too small: $($mapBytes.Length) bytes" }

$baseName = Read-AsciiAt $mapBytes 0x00 11
$subName  = Read-AsciiAt $mapBytes 0x10 11
$width    = Read-BEWord $mapBytes 0x54
$height   = Read-BEWord $mapBytes 0x56
$expected = 0x60 + ($width * $height * 2)

if($width -le 0 -or $height -le 0 -or $width -gt 1024 -or $height -gt 1024) {
    throw "Invalid map dimensions: ${width}x${height}"
}
if($expected -gt $mapBytes.Length) {
    throw "Map data truncated (expected >=$expected bytes, got $($mapBytes.Length))"
}

$basePath = Find-DataFile $DataDir $baseName
$subPath  = Find-DataFile $DataDir $subName
if(-not $basePath) { throw "Could not find base tileset '$baseName' in $DataDir" }
if(-not $subPath)  { throw "Could not find sub tileset '$subName' in $DataDir"  }

$baseBlk = Read-MaybeRnc -Path $basePath
$subBlk  = Read-MaybeRnc -Path $subPath

# Tile graphics blob is BaseBlk concatenated with SubBlk.
$blk = New-Object byte[] ($baseBlk.Length + $subBlk.Length)
[Array]::Copy($baseBlk, 0, $blk, 0, $baseBlk.Length)
[Array]::Copy($subBlk,  0, $blk, $baseBlk.Length, $subBlk.Length)

# Palette discovery: Amiga has a sibling .pal; PC embeds it at 0xFA00 in BaseBlk.
$palStem = [IO.Path]::GetFileNameWithoutExtension($baseName)
$palPath = Find-DataFile $DataDir ($palStem + '.pal')
if(-not $palPath) {
    $palPath = Find-DataFile $DataDir ($palStem.ToUpperInvariant() + '.PAL')
}

[byte[]]$palette = New-Object byte[] (256 * 3)   # R,G,B per index, zero-filled

if($palPath) {
    $format = 'amiga'
    $palBytes = Read-MaybeRnc -Path $palPath
    $colors = [Math]::Min(16, [int]($palBytes.Length / 2))
    for($i = 0; $i -lt $colors; ++$i) {
        $w = Read-BEWord $palBytes ($i * 2)
        # 0RRR GGGG BBBB on Amiga: each nibble scaled by <<2 to 8-bit.
        $palette[$i * 3 + 0] = [byte]((($w -shr 8) -band 0xF) -shl 4 -bor (($w -shr 8) -band 0xF))
        $palette[$i * 3 + 1] = [byte]((($w -shr 4) -band 0xF) -shl 4 -bor (($w -shr 4) -band 0xF))
        $palette[$i * 3 + 2] = [byte]((($w       ) -band 0xF) -shl 4 -bor (($w       ) -band 0xF))
    }
} elseif($baseBlk.Length -ge 0xFA00 + 0x80 * 3) {
    $format = 'pc'
    for($i = 0; $i -lt 0x80; ++$i) {
        $palette[$i * 3 + 0] = $baseBlk[0xFA00 + $i * 3 + 0]
        $palette[$i * 3 + 1] = $baseBlk[0xFA00 + $i * 3 + 1]
        $palette[$i * 3 + 2] = $baseBlk[0xFA00 + $i * 3 + 2]
    }
} else {
    throw "Could not locate palette: no $palStem.pal beside the .blk and BaseBlk too small to embed one"
}

Write-Output ("map=$([IO.Path]::GetFileName($MapPath)) format=$format size=${width}x${height} base=$baseName sub=$subName scale=$Scale")

# Decode a single tile (16x16) into an 8-bit palette-index buffer.
function Read-Tile([byte[]]$Blk, [int]$TileId, [string]$Format) {
    $px = New-Object byte[] 256

    if($Format -eq 'amiga') {
        $offset = $TileId -shl 7    # 128 bytes per tile
        if($offset -lt 0 -or ($offset + 128) -gt $Blk.Length) { return $px }

        # 4 bitplanes; each plane = 16 rows * 2 bytes, MSB-first per row.
        for($plane = 0; $plane -lt 4; ++$plane) {
            $planeBase = $offset + ($plane * 32)
            for($row = 0; $row -lt 16; ++$row) {
                $hi = $Blk[$planeBase + $row * 2]
                $lo = $Blk[$planeBase + $row * 2 + 1]
                $word = ([int]$hi -shl 8) -bor [int]$lo
                $rowBase = $row * 16
                for($x = 0; $x -lt 16; ++$x) {
                    if(($word -band (0x8000 -shr $x)) -ne 0) {
                        $px[$rowBase + $x] = [byte]($px[$rowBase + $x] -bor (1 -shl $plane))
                    }
                }
            }
        }
    } else {
        # PC chunky: tiles laid out 20-per-row, each row of 16px = 320 bytes wide.
        $col = $TileId % 20
        $rowGroup = [Math]::Floor($TileId / 20)
        $base = ($rowGroup * 0x12C0) + ($col * 0x10)
        if($base -lt 0 -or ($base + 16 * 320 - 304) -gt $Blk.Length) { return $px }
        for($row = 0; $row -lt 16; ++$row) {
            $src = $base + ($row * 320)
            $dst = $row * 16
            if(($src + 16) -le $Blk.Length) {
                [Array]::Copy($Blk, $src, $px, $dst, 16)
            }
        }
    }

    return $px
}

# Output bitmap.
$tilePx = 16 * $Scale
$imgW = $width  * $tilePx
$imgH = $height * $tilePx

$bmp = New-Object Drawing.Bitmap $imgW, $imgH, ([Drawing.Imaging.PixelFormat]::Format32bppArgb)
$rect = New-Object Drawing.Rectangle 0, 0, $imgW, $imgH
$lock = $bmp.LockBits($rect, [Drawing.Imaging.ImageLockMode]::WriteOnly, [Drawing.Imaging.PixelFormat]::Format32bppArgb)
$stride = $lock.Stride
$buf = New-Object byte[] ($stride * $imgH)

# Decode-cache: many tiles repeat across a map, so cache decoded pixel buffers.
$cache = @{}

# Pre-multiplied palette to BGRA so we can blast bytes into the locked bitmap.
$bgra = New-Object byte[] (256 * 4)
for($i = 0; $i -lt 256; ++$i) {
    $bgra[$i * 4 + 0] = $palette[$i * 3 + 2]
    $bgra[$i * 4 + 1] = $palette[$i * 3 + 1]
    $bgra[$i * 4 + 2] = $palette[$i * 3 + 0]
    $bgra[$i * 4 + 3] = 255
}

$tileWords = New-Object 'int[]' ($width * $height)
for($ty = 0; $ty -lt $height; ++$ty) {
    for($tx = 0; $tx -lt $width; ++$tx) {
        $offset = 0x60 + (($ty * $width) + $tx) * 2
        $word = Read-BEWord $mapBytes $offset
        $tileWords[$ty * $width + $tx] = $word
    }
}

for($ty = 0; $ty -lt $height; ++$ty) {
    for($tx = 0; $tx -lt $width; ++$tx) {
        $tileId = $tileWords[$ty * $width + $tx] -band 0x1FF

        $px = $cache[$tileId]
        if($null -eq $px) {
            $px = Read-Tile $blk $tileId $format
            $cache[$tileId] = $px
        }

        $dstYBase = $ty * $tilePx
        $dstXBase = $tx * $tilePx

        for($py = 0; $py -lt 16; ++$py) {
            $srcRow = $py * 16
            for($sy = 0; $sy -lt $Scale; ++$sy) {
                $rowOffset = ($dstYBase + $py * $Scale + $sy) * $stride + $dstXBase * 4
                for($pxCol = 0; $pxCol -lt 16; ++$pxCol) {
                    $idx = [int]$px[$srcRow + $pxCol] * 4
                    $b = $bgra[$idx]
                    $g = $bgra[$idx + 1]
                    $r = $bgra[$idx + 2]
                    $a = $bgra[$idx + 3]
                    for($sx = 0; $sx -lt $Scale; ++$sx) {
                        $o = $rowOffset + ($pxCol * $Scale + $sx) * 4
                        $buf[$o]     = $b
                        $buf[$o + 1] = $g
                        $buf[$o + 2] = $r
                        $buf[$o + 3] = $a
                    }
                }
            }
        }
    }
}

[Runtime.InteropServices.Marshal]::Copy($buf, 0, $lock.Scan0, $buf.Length)
$bmp.UnlockBits($lock)

# Tile-ID overlay.
if(-not $NoOverlay) {
    $g = [Drawing.Graphics]::FromImage($bmp)
    $g.TextRenderingHint = [Drawing.Text.TextRenderingHint]::SingleBitPerPixelGridFit
    $g.SmoothingMode = [Drawing.Drawing2D.SmoothingMode]::None

    # Pick a font size that fits 3 chars in the top-left without obscuring too much tile.
    $fontSize = [Math]::Max(5, [int]($tilePx / 4))
    $font = New-Object Drawing.Font 'Consolas', $fontSize, ([Drawing.FontStyle]::Bold), ([Drawing.GraphicsUnit]::Pixel)
    $whiteBrush = [Drawing.Brushes]::White
    $shadowBrush = New-Object Drawing.SolidBrush ([Drawing.Color]::FromArgb(180, 0, 0, 0))

    $format = New-Object Drawing.StringFormat
    $format.FormatFlags = [Drawing.StringFormatFlags]::NoWrap
    $format.Trimming = [Drawing.StringTrimming]::None

    for($ty = 0; $ty -lt $height; ++$ty) {
        for($tx = 0; $tx -lt $width; ++$tx) {
            $tileId = $tileWords[$ty * $width + $tx] -band 0x1FF
            if($Coords) {
                $label = "$tx,$ty"
            } else {
                $label = "{0:000}" -f $tileId
            }

            $x = $tx * $tilePx + 1
            $y = $ty * $tilePx + 1

            # Tight backdrop for legibility on any tile color.
            $size = $g.MeasureString($label, $font)
            $bgRect = New-Object Drawing.RectangleF $x, $y, ($size.Width - 2), ($size.Height - 2)
            $g.FillRectangle($shadowBrush, $bgRect)
            $g.DrawString($label, $font, $whiteBrush, [single]$x, [single]($y - 1), $format)
        }
    }

    $shadowBrush.Dispose()
    $font.Dispose()
    $g.Dispose()
}

$bmp.Save($OutputPath, [Drawing.Imaging.ImageFormat]::Png)
$bmp.Dispose()

Write-Output ("wrote=$OutputPath ${imgW}x${imgH} uniqueTiles=$($cache.Count)")
