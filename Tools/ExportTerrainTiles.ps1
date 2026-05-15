<#
.SYNOPSIS
Exports each individual tile (16x16 PNG) for every terrain tileset found under -DataRoot.

.DESCRIPTION
Walks each campaign/data-set folder, finds tileset pairs (e.g. junbase.blk + junsub0.blk),
and writes one PNG per tile to:

    <OutputRoot>\<TerrainName>\<DataSet>[_<SubLabel>]\Tile<NNN>.png

The terrain prefix is mapped to a friendly name (jun -> Jungle, ice -> Ice, des -> Desert,
int -> Interior, moor -> Moor, hid -> Hidden). Unknown prefixes use the prefix as-is,
capitalised.

Tiles 0..239 come from the base .blk; tiles 240+ come from the sub .blk (matches the
flat tile-graphic layout used by the renderer). When a base has multiple subs (e.g.
junsub0 + junsub1) each sub becomes its own subfolder and tiles 0..239 are duplicated.

Auto-detects Amiga (4-bitplane planar, separate .pal) vs PC (8bpp chunky, palette
embedded in BaseBlk at 0xFA00).

.PARAMETER DataRoot
Top-level data folder. Defaults to Run/Data relative to the current directory.

.PARAMETER OutputRoot
Where to write the tile PNGs. Defaults to MapDumps/Terrain.

.PARAMETER Scale
Pixel scale per tile (1 = native 16x16). Default 1.

.PARAMETER SheetScale
Pixel scale for the combined tile-sheet PNG (default 2, so labels fit).

.PARAMETER SheetColumns
Tiles per row in the sheet (default 20, matches PC native layout).

.PARAMETER SheetGap
Pixel gap between tiles in the sheet (default 2).

.PARAMETER NoSheet
Skip writing the per-tileset combined sheet.

.PARAMETER NoTiles
Skip writing individual tile PNGs (handy when you only want sheets).

.EXAMPLE
./Tools/ExportTerrainTiles.ps1
./Tools/ExportTerrainTiles.ps1 -Scale 4
./Tools/ExportTerrainTiles.ps1 -NoTiles -SheetScale 3
#>

param(
    [string]$DataRoot,
    [string]$OutputRoot,
    [ValidateRange(1, 8)][int]$Scale = 1,
    [ValidateRange(1, 8)][int]$SheetScale = 2,
    [ValidateRange(4, 64)][int]$SheetColumns = 20,
    [ValidateRange(0, 16)][int]$SheetGap = 2,
    [switch]$NoSheet,
    [switch]$NoTiles
)

$ErrorActionPreference = 'Stop'

Add-Type -AssemblyName System.Drawing
. (Join-Path $PSScriptRoot 'Lib/RncUnpack.ps1')

if(-not $DataRoot) {
    $DataRoot = Join-Path (Get-Location).Path 'Run/Data'
}
if(-not (Test-Path -LiteralPath $DataRoot)) {
    throw "Data root not found: $DataRoot"
}
$DataRoot = (Resolve-Path -LiteralPath $DataRoot).Path

if(-not $OutputRoot) {
    $OutputRoot = Join-Path (Join-Path (Get-Location).Path 'MapDumps') 'Terrain'
}

$terrainNames = @{
    'jun'  = 'Jungle'
    'ice'  = 'Ice'
    'des'  = 'Desert'
    'int'  = 'Interior'
    'mor'  = 'Moor'
    'moor' = 'Moor'
    'hid'  = 'Hidden'
}

function Get-TerrainName([string]$Prefix) {
    $key = $Prefix.ToLowerInvariant()
    if($terrainNames.ContainsKey($key)) { return $terrainNames[$key] }
    return [char]::ToUpperInvariant($key[0]) + $key.Substring(1)
}

function Get-BasePrefix([string]$BaseFileName) {
    $stem = [IO.Path]::GetFileNameWithoutExtension($BaseFileName)
    if($stem.ToLowerInvariant().EndsWith('base')) {
        return $stem.Substring(0, $stem.Length - 4)
    }
    return $stem
}

function Read-BEWord([byte[]]$Bytes, [int]$Offset) {
    return ([int]$Bytes[$Offset] -shl 8) -bor [int]$Bytes[$Offset + 1]
}

function Find-DataFile([string]$Dir, [string]$Name) {
    $direct = Join-Path $Dir $Name
    if(Test-Path -LiteralPath $direct) { return (Resolve-Path -LiteralPath $direct).Path }
    $match = Get-ChildItem -LiteralPath $Dir -File -Filter $Name -ErrorAction SilentlyContinue | Select-Object -First 1
    if($match) { return $match.FullName }
    return $null
}

function Read-Tile([byte[]]$Blk, [int]$TileId, [string]$Format) {
    $px = New-Object byte[] 256

    if($Format -eq 'amiga') {
        $offset = $TileId -shl 7
        if($offset -lt 0 -or ($offset + 128) -gt $Blk.Length) { return $null }

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
        $col = $TileId % 20
        $rowGroup = [Math]::Floor($TileId / 20)
        $base = ($rowGroup * 0x12C0) + ($col * 0x10)
        if($base -lt 0 -or ($base + 16 * 320 - 304) -gt $Blk.Length) { return $null }
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

function Load-Palette([string]$DataDir, [string]$BaseName, [byte[]]$BaseBlk) {
    $palStem = [IO.Path]::GetFileNameWithoutExtension($BaseName)
    $palPath = Find-DataFile $DataDir ($palStem + '.pal')
    if(-not $palPath) {
        $palPath = Find-DataFile $DataDir ($palStem.ToUpperInvariant() + '.PAL')
    }

    [byte[]]$palette = New-Object byte[] (256 * 3)

    if($palPath) {
        $palBytes = Read-MaybeRnc -Path $palPath
        $colors = [Math]::Min(16, [int]($palBytes.Length / 2))
        for($i = 0; $i -lt $colors; ++$i) {
            $w = Read-BEWord $palBytes ($i * 2)
            $palette[$i * 3 + 0] = [byte]((($w -shr 8) -band 0xF) -shl 4 -bor (($w -shr 8) -band 0xF))
            $palette[$i * 3 + 1] = [byte]((($w -shr 4) -band 0xF) -shl 4 -bor (($w -shr 4) -band 0xF))
            $palette[$i * 3 + 2] = [byte]((($w       ) -band 0xF) -shl 4 -bor (($w       ) -band 0xF))
        }
        return @{ Format = 'amiga'; Palette = $palette }
    }

    if($BaseBlk.Length -ge 0xFA00 + 0x80 * 3) {
        for($i = 0; $i -lt 0x80; ++$i) {
            $palette[$i * 3 + 0] = $BaseBlk[0xFA00 + $i * 3 + 0]
            $palette[$i * 3 + 1] = $BaseBlk[0xFA00 + $i * 3 + 1]
            $palette[$i * 3 + 2] = $BaseBlk[0xFA00 + $i * 3 + 2]
        }
        return @{ Format = 'pc'; Palette = $palette }
    }

    return $null
}

function Save-Tile([byte[]]$Px, [byte[]]$Bgra, [int]$Scale, [string]$OutPath) {
    $size = 16 * $Scale
    $bmp = New-Object Drawing.Bitmap $size, $size, ([Drawing.Imaging.PixelFormat]::Format32bppArgb)
    $rect = New-Object Drawing.Rectangle 0, 0, $size, $size
    $lock = $bmp.LockBits($rect, [Drawing.Imaging.ImageLockMode]::WriteOnly, [Drawing.Imaging.PixelFormat]::Format32bppArgb)
    $stride = $lock.Stride
    $buf = New-Object byte[] ($stride * $size)

    for($py = 0; $py -lt 16; ++$py) {
        $srcRow = $py * 16
        for($sy = 0; $sy -lt $Scale; ++$sy) {
            $rowOffset = ($py * $Scale + $sy) * $stride
            for($pxCol = 0; $pxCol -lt 16; ++$pxCol) {
                $idx = [int]$Px[$srcRow + $pxCol] * 4
                $b = $Bgra[$idx]
                $g = $Bgra[$idx + 1]
                $r = $Bgra[$idx + 2]
                $a = $Bgra[$idx + 3]
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

    [Runtime.InteropServices.Marshal]::Copy($buf, 0, $lock.Scan0, $buf.Length)
    $bmp.UnlockBits($lock)
    $bmp.Save($OutPath, [Drawing.Imaging.ImageFormat]::Png)
    $bmp.Dispose()
}

function Save-TileSheet([byte[]]$Blk, [string]$Format, [byte[]]$Bgra, [int]$TileCount, [int]$Columns, [int]$Scale, [int]$Gap, [string]$Title, [string]$OutPath) {
    $rows = [Math]::Ceiling($TileCount / [double]$Columns)
    $tilePx = 16 * $Scale
    $cellStride = $tilePx + $Gap

    $leftMargin = 44
    $topMargin = 28
    $titleHeight = 18
    $padding = 4

    $gridW = $Columns * $cellStride - $Gap
    $gridH = [int]$rows * $cellStride - $Gap
    if($gridW -lt 0) { $gridW = 0 }
    if($gridH -lt 0) { $gridH = 0 }
    $imgW = $leftMargin + $gridW + $padding
    $imgH = $titleHeight + $topMargin + $gridH + $padding

    $bmp = New-Object Drawing.Bitmap $imgW, $imgH, ([Drawing.Imaging.PixelFormat]::Format32bppArgb)
    $g = [Drawing.Graphics]::FromImage($bmp)
    $g.Clear([Drawing.Color]::FromArgb(255, 24, 24, 24))
    $g.TextRenderingHint = [Drawing.Text.TextRenderingHint]::SingleBitPerPixelGridFit
    $g.SmoothingMode = [Drawing.Drawing2D.SmoothingMode]::None
    $g.InterpolationMode = [Drawing.Drawing2D.InterpolationMode]::NearestNeighbor
    $g.PixelOffsetMode = [Drawing.Drawing2D.PixelOffsetMode]::Half

    $titleFont = New-Object Drawing.Font 'Consolas', 12, ([Drawing.FontStyle]::Bold), ([Drawing.GraphicsUnit]::Pixel)
    $axisFont = New-Object Drawing.Font 'Consolas', 11, ([Drawing.FontStyle]::Regular), ([Drawing.GraphicsUnit]::Pixel)
    $whiteBrush = [Drawing.Brushes]::White
    $axisBrush = New-Object Drawing.SolidBrush ([Drawing.Color]::FromArgb(255, 200, 200, 200))
    $gridPen = New-Object Drawing.Pen ([Drawing.Color]::FromArgb(60, 80, 80, 80))

    $g.DrawString($Title, $titleFont, $whiteBrush, [single]4, [single]2)

    # Render each tile via its own small bitmap, then DrawImage into the grid (scales nearest-neighbour).
    $cellSize = 16
    for($id = 0; $id -lt $TileCount; ++$id) {
        $px = Read-Tile $Blk $id $Format
        if($null -eq $px) { break }

        $col = $id % $Columns
        $row = [int]([Math]::Floor($id / $Columns))

        $cellBmp = New-Object Drawing.Bitmap $cellSize, $cellSize, ([Drawing.Imaging.PixelFormat]::Format32bppArgb)
        $cellRect = New-Object Drawing.Rectangle 0, 0, $cellSize, $cellSize
        $cellLock = $cellBmp.LockBits($cellRect, [Drawing.Imaging.ImageLockMode]::WriteOnly, [Drawing.Imaging.PixelFormat]::Format32bppArgb)
        $bmpStride = $cellLock.Stride
        $cellBuf = New-Object byte[] ($bmpStride * $cellSize)
        for($py = 0; $py -lt 16; ++$py) {
            $rowOffset = $py * $bmpStride
            $srcRow = $py * 16
            for($pxCol = 0; $pxCol -lt 16; ++$pxCol) {
                $idx = [int]$px[$srcRow + $pxCol] * 4
                $o = $rowOffset + $pxCol * 4
                $cellBuf[$o]     = $Bgra[$idx]
                $cellBuf[$o + 1] = $Bgra[$idx + 1]
                $cellBuf[$o + 2] = $Bgra[$idx + 2]
                $cellBuf[$o + 3] = $Bgra[$idx + 3]
            }
        }
        [Runtime.InteropServices.Marshal]::Copy($cellBuf, 0, $cellLock.Scan0, $cellBuf.Length)
        $cellBmp.UnlockBits($cellLock)

        $dx = $leftMargin + $col * $cellStride
        $dy = $titleHeight + $topMargin + $row * $cellStride
        $destRect = New-Object Drawing.Rectangle $dx, $dy, $tilePx, $tilePx
        $g.DrawImage($cellBmp, $destRect, 0, 0, $cellSize, $cellSize, [Drawing.GraphicsUnit]::Pixel)
        $cellBmp.Dispose()
    }

    # Column headers across the top.
    $colFormat = New-Object Drawing.StringFormat
    $colFormat.Alignment = [Drawing.StringAlignment]::Center
    for($col = 0; $col -lt $Columns; ++$col) {
        $cx = $leftMargin + $col * $cellStride + ($tilePx / 2)
        $cy = $titleHeight + $topMargin - 14
        $g.DrawString(("{0}" -f $col), $axisFont, $axisBrush, [single]$cx, [single]$cy, $colFormat)
    }

    # Row headers down the left side: first tile id in that row.
    $rowFormat = New-Object Drawing.StringFormat
    $rowFormat.Alignment = [Drawing.StringAlignment]::Far
    $rowFormat.LineAlignment = [Drawing.StringAlignment]::Center
    for($row = 0; $row -lt $rows; ++$row) {
        $rowStartId = $row * $Columns
        $rx = $leftMargin - 4
        $ry = $titleHeight + $topMargin + $row * $cellStride + ($tilePx / 2)
        $g.DrawString(("{0}" -f $rowStartId), $axisFont, $axisBrush, [single]$rx, [single]$ry, $rowFormat)
    }

    # If no gap is requested, draw subtle grid lines so cells are still countable.
    if($Gap -le 0) {
        for($col = 0; $col -le $Columns; ++$col) {
            $x = $leftMargin + $col * $tilePx
            $g.DrawLine($gridPen, [single]$x, [single]($titleHeight + $topMargin), [single]$x, [single]($titleHeight + $topMargin + $gridH))
        }
        for($row = 0; $row -le [int]$rows; ++$row) {
            $y = $titleHeight + $topMargin + $row * $tilePx
            $g.DrawLine($gridPen, [single]$leftMargin, [single]$y, [single]($leftMargin + $gridW), [single]$y)
        }
    }

    $titleFont.Dispose()
    $axisFont.Dispose()
    $axisBrush.Dispose()
    $gridPen.Dispose()
    $g.Dispose()

    $bmp.Save($OutPath, [Drawing.Imaging.ImageFormat]::Png)
    $bmp.Dispose()
}

function Get-SubLabel([string]$SubFileName, [string]$Prefix) {
    $stem = [IO.Path]::GetFileNameWithoutExtension($SubFileName).ToLowerInvariant()
    $marker = $Prefix.ToLowerInvariant() + 'sub'
    if($stem.StartsWith($marker)) {
        return 'sub' + $stem.Substring($marker.Length)
    }
    return $stem
}

# Discover terrain bases across all dataset folders.
$rootHasBase = Get-ChildItem -LiteralPath $DataRoot -File -Filter '*base.blk' -ErrorAction SilentlyContinue | Select-Object -First 1
if($rootHasBase) {
    $datasets = @(Get-Item -LiteralPath $DataRoot)
} else {
    $datasets = Get-ChildItem -LiteralPath $DataRoot -Directory | Sort-Object Name
}

$totalSets = 0
$totalTiles = 0
$failed = 0

foreach($dataset in $datasets) {
    $bases = Get-ChildItem -LiteralPath $dataset.FullName -File -Filter '*base.blk' -ErrorAction SilentlyContinue | Sort-Object Name
    if(-not $bases) { continue }

    foreach($base in $bases) {
        $prefix = Get-BasePrefix $base.Name
        $terrain = Get-TerrainName $prefix

        try {
            $baseBlk = Read-MaybeRnc -Path $base.FullName
        } catch {
            Write-Output ("  FAIL {0}\{1}: cannot read base ({2})" -f $dataset.Name, $base.Name, $_.Exception.Message)
            $failed++
            continue
        }

        $palInfo = Load-Palette $dataset.FullName $base.Name $baseBlk
        if(-not $palInfo) {
            Write-Output ("  skip {0}\{1}: no palette" -f $dataset.Name, $base.Name)
            continue
        }

        $bgra = New-Object byte[] (256 * 4)
        $palette = $palInfo.Palette
        for($i = 0; $i -lt 256; ++$i) {
            $bgra[$i * 4 + 0] = $palette[$i * 3 + 2]
            $bgra[$i * 4 + 1] = $palette[$i * 3 + 1]
            $bgra[$i * 4 + 2] = $palette[$i * 3 + 0]
            $bgra[$i * 4 + 3] = 255
        }

        $subs = Get-ChildItem -LiteralPath $dataset.FullName -File -Filter ($prefix + 'sub*.blk') -ErrorAction SilentlyContinue | Sort-Object Name
        if(-not $subs) {
            $subs = @($null)
        }

        foreach($sub in $subs) {
            $totalSets++
            $subLabel = if($sub) { Get-SubLabel $sub.Name $prefix } else { 'nosub' }

            $blk = $baseBlk
            if($sub) {
                try {
                    $subBlk = Read-MaybeRnc -Path $sub.FullName
                } catch {
                    Write-Output ("  FAIL {0}\{1}: cannot read sub ({2})" -f $dataset.Name, $sub.Name, $_.Exception.Message)
                    $failed++
                    continue
                }
                $blk = New-Object byte[] ($baseBlk.Length + $subBlk.Length)
                [Array]::Copy($baseBlk, 0, $blk, 0, $baseBlk.Length)
                [Array]::Copy($subBlk,  0, $blk, $baseBlk.Length, $subBlk.Length)
            }

            $folderName = if($sub) { ('{0}_{1}' -f $dataset.Name, $subLabel) } else { $dataset.Name }
            $outDir = Join-Path (Join-Path $OutputRoot $terrain) $folderName
            if(-not (Test-Path -LiteralPath $outDir)) {
                New-Item -ItemType Directory -Path $outDir -Force | Out-Null
            }

            # Tile count: 128 bytes per tile for Amiga; PC layout uses 320*16 byte rows of 20 tiles.
            if($palInfo.Format -eq 'amiga') {
                $maxTiles = [Math]::Min(512, [int]($blk.Length / 128))
            } else {
                $rowGroups = [int]($blk.Length / 0x12C0)
                $maxTiles = [Math]::Min(512, $rowGroups * 20)
            }

            $written = 0
            for($id = 0; $id -lt $maxTiles; ++$id) {
                $px = Read-Tile $blk $id $palInfo.Format
                if($null -eq $px) { break }
                if(-not $NoTiles) {
                    $outPath = Join-Path $outDir ('Tile{0:D3}.png' -f $id)
                    Save-Tile $px $bgra $Scale $outPath
                }
                $written++
            }

            if(-not $NoSheet -and $written -gt 0) {
                $title = "{0} / {1}  {2}" -f $terrain, $folderName, $palInfo.Format
                $sheetPath = Join-Path $outDir 'TileSheet.png'
                Save-TileSheet $blk $palInfo.Format $bgra $written $SheetColumns $SheetScale $SheetGap $title $sheetPath
            }

            $totalTiles += $written
            $subDesc = if($sub) { $sub.Name } else { '(no sub)' }
            Write-Output ("  ok   {0,-22} {1,-12} -> {2,3} tiles  ({3} + {4})" -f $dataset.Name, $terrain, $written, $base.Name, $subDesc)
        }
    }
}

Write-Output ""
Write-Output ("totalSets=$totalSets totalTiles=$totalTiles failed=$failed output=$OutputRoot")
if($failed -gt 0) { exit 1 }
