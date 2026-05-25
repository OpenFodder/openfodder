/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2026 Open Fodder
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "stdafx.hpp"
#include "Amiga/dernc.hpp"

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#endif

static std::string MapPng_ToLower(std::string pValue) {
	std::transform(pValue.begin(), pValue.end(), pValue.begin(), [](unsigned char c) {
		return (char)std::tolower(c);
	});
	return pValue;
}

static bool MapPng_FileExists(const std::string& pPath) {
	std::ifstream File(pPath.c_str(), std::ios::binary);
	return File.is_open();
}

static size_t MapPng_ExtensionPos(const std::string& pPath) {
	const size_t Slash = pPath.find_last_of("/\\");
	const size_t Dot = pPath.find_last_of('.');

	if (Dot == std::string::npos)
		return std::string::npos;

	if (Slash != std::string::npos && Dot < Slash)
		return std::string::npos;

	return Dot;
}

static std::string MapPng_WithExtension(const std::string& pPath, const std::string& pExtension) {
	const size_t Dot = MapPng_ExtensionPos(pPath);

	if (Dot == std::string::npos)
		return pPath + pExtension;

	return pPath.substr(0, Dot) + pExtension;
}

static std::string MapPng_DirName(const std::string& pPath) {
	const size_t Slash = pPath.find_last_of("/\\");
	if (Slash == std::string::npos)
		return ".";
	return pPath.substr(0, Slash);
}

static std::string MapPng_BaseName(const std::string& pPath) {
	const size_t Slash = pPath.find_last_of("/\\");
	if (Slash == std::string::npos)
		return pPath;
	return pPath.substr(Slash + 1);
}

static std::string MapPng_JoinPath(const std::string& pDir, const std::string& pName) {
	if (!pDir.size() || pDir == ".")
		return pName;
	const char Last = pDir[pDir.size() - 1];
	if (Last == '/' || Last == '\\')
		return pDir + pName;
	return pDir + "/" + pName;
}

static std::string MapPng_ResolveMapPath(const std::string& pPath) {
	if (MapPng_FileExists(pPath))
		return pPath;

	if (MapPng_ExtensionPos(pPath) == std::string::npos) {
		const std::string WithMap = pPath + ".map";
		if (MapPng_FileExists(WithMap))
			return WithMap;
	}

	return pPath;
}

static tSharedBuffer MapPng_ReadFileRaw(const std::string& pPath) {
	auto Buffer = std::make_shared<std::vector<uint8>>();
	std::ifstream File(pPath.c_str(), std::ios::binary);
	if (!File.is_open())
		return Buffer;

	File.seekg(0, std::ios::end);
	const std::streamoff Size = File.tellg();
	File.seekg(0, std::ios::beg);
	if (Size <= 0)
		return Buffer;

	Buffer->resize((size_t)Size);
	File.read((char*)Buffer->data(), Size);
	if (!File.good() && !File.eof())
		Buffer->clear();

	return Buffer;
}

static tSharedBuffer MapPng_DeRnc(tSharedBuffer pBuffer) {
	if (!pBuffer || pBuffer->size() < 8)
		return pBuffer;

	if (readBEDWord(pBuffer->data()) != 0x524E4301)
		return pBuffer;

	const uint32 Size = readBEDWord(pBuffer->data() + 4);
	auto Unpacked = std::make_shared<std::vector<uint8>>();
	Unpacked->resize(Size);

	if (rnc_unpack(pBuffer->data(), Unpacked->data()) < 0)
		Unpacked->clear();

	return Unpacked;
}

static tSharedBuffer MapPng_ReadFileMaybeRnc(const std::string& pPath) {
	if (!MapPng_FileExists(pPath))
		return std::make_shared<std::vector<uint8>>();

	return MapPng_DeRnc(MapPng_ReadFileRaw(pPath));
}

static std::string MapPng_ReadAsciiAt(tSharedBuffer pBuffer, size_t pOffset, size_t pLength) {
	if (!pBuffer || pOffset >= pBuffer->size())
		return "";

	size_t End = pOffset;
	const size_t Stop = std::min<size_t>(pBuffer->size(), pOffset + pLength);
	while (End < Stop && (*pBuffer)[End])
		++End;

	return std::string((const char*)pBuffer->data() + pOffset, End - pOffset);
}

static uint16 MapPng_ReadMapTileId(tSharedBuffer pMap, size_t pOffset, bool pLittleEndianWords) {
	if (!pMap || pOffset + 1 >= pMap->size())
		return 0;
	return pLittleEndianWords ?
		(readLE<uint16>(pMap->data() + pOffset) & 0x1FF) :
		(readBEWord(pMap->data() + pOffset) & 0x1FF);
}

static std::string MapPng_UpperFileName(std::string pValue) {
	std::transform(pValue.begin(), pValue.end(), pValue.begin(), [](unsigned char c) {
		return (char)std::toupper(c);
	});
	return pValue;
}

static std::string MapPng_FindDataFile(const std::string& pMapPath, const std::string& pName) {
	const std::string MapDir = MapPng_DirName(pMapPath);
	const std::string Lower = MapPng_ToLower(pName);
	const std::string Upper = MapPng_UpperFileName(pName);
	std::vector<std::string> Dirs;
	Dirs.push_back(MapDir);
	Dirs.push_back(MapPng_JoinPath(MapDir, "Data/Amiga"));
	Dirs.push_back("Run/Data/Amiga");
	Dirs.push_back("Data/Amiga");

	for (size_t dirIndex = 0; dirIndex < Dirs.size(); ++dirIndex) {
		const std::string& Dir = Dirs[dirIndex];
		const std::string Direct = MapPng_JoinPath(Dir, pName);
		if (MapPng_FileExists(Direct))
			return Direct;
		const std::string LowerPath = MapPng_JoinPath(Dir, Lower);
		if (MapPng_FileExists(LowerPath))
			return LowerPath;
		const std::string UpperPath = MapPng_JoinPath(Dir, Upper);
		if (MapPng_FileExists(UpperPath))
			return UpperPath;
	}

	return "";
}

static void MapPng_SetPixel(SDL_Surface* pSurface, int pX, int pY, uint32 pColor) {
	if (pX < 0 || pY < 0 || pX >= pSurface->w || pY >= pSurface->h)
		return;

	uint32* Pixel = (uint32*)((uint8*)pSurface->pixels + (pY * pSurface->pitch));
	Pixel[pX] = pColor;
}

static const uint8 MapPng_Font4x6[][6] = {
	{ 0x06, 0x09, 0x09, 0x09, 0x09, 0x06 }, // 0
	{ 0x02, 0x06, 0x02, 0x02, 0x02, 0x07 }, // 1
	{ 0x0E, 0x01, 0x02, 0x04, 0x08, 0x0F }, // 2
	{ 0x0E, 0x01, 0x06, 0x01, 0x01, 0x0E }, // 3
	{ 0x09, 0x09, 0x0F, 0x01, 0x01, 0x01 }, // 4
	{ 0x0F, 0x08, 0x0E, 0x01, 0x01, 0x0E }, // 5
	{ 0x06, 0x08, 0x0E, 0x09, 0x09, 0x06 }, // 6
	{ 0x0F, 0x01, 0x02, 0x04, 0x04, 0x04 }, // 7
	{ 0x06, 0x09, 0x06, 0x09, 0x09, 0x06 }, // 8
	{ 0x06, 0x09, 0x09, 0x07, 0x01, 0x06 }, // 9
};

static void MapPng_FillPixelBlock(SDL_Surface* pSurface, int pX, int pY, int pScale, uint32 pColor) {
	for (int y = 0; y < pScale; ++y) {
		for (int x = 0; x < pScale; ++x)
			MapPng_SetPixel(pSurface, pX + x, pY + y, pColor);
	}
}

static int MapPng_DrawChar(SDL_Surface* pSurface, int pX, int pY, char pChar, uint32 pColor, int pScale) {
	if (pChar == ',') {
		MapPng_FillPixelBlock(pSurface, pX + pScale, pY + (5 * pScale), pScale, pColor);
		return 2 * pScale;
	}

	if (pChar < '0' || pChar > '9')
		return 0;

	const uint8* Glyph = MapPng_Font4x6[pChar - '0'];
	for (int y = 0; y < 6; ++y) {
		for (int x = 0; x < 4; ++x) {
			if (Glyph[y] & (1 << (3 - x)))
				MapPng_FillPixelBlock(pSurface, pX + (x * pScale), pY + (y * pScale), pScale, pColor);
		}
	}

	return 4 * pScale;
}

static void MapPng_DrawText(SDL_Surface* pSurface, int pX, int pY, const std::string& pText, uint32 pColor, int pSpacing, int pScale) {
	int X = pX;

	for (size_t i = 0; i < pText.size(); ++i) {
		const int Width = MapPng_DrawChar(pSurface, X, pY, pText[i], pColor, pScale);
		X += Width + pSpacing;
	}
}

static std::string MapPng_FormatTileId(uint16 pTileId) {
	std::string Text = std::to_string(pTileId);

	while (Text.size() < 3)
		Text = "0" + Text;

	return Text;
}

static std::string MapPng_FormatCoordNumber(int pValue) {
	std::string Text = std::to_string(pValue);
	if (pValue >= 0 && pValue < 10)
		Text = "0" + Text;
	return Text;
}

static std::string MapPng_FormatCoords(int pX, int pY) {
	return MapPng_FormatCoordNumber(pX) + "," + MapPng_FormatCoordNumber(pY);
}

static void MapPng_DrawLabel(SDL_Surface* pSurface, int pX, int pY, const std::string& pText, uint32 pBlack, uint32 pWhite, int pSpacing, int pScale) {
	MapPng_DrawText(pSurface, pX + 1, pY + 1, pText, pBlack, pSpacing, pScale);
	MapPng_DrawText(pSurface, pX, pY, pText, pWhite, pSpacing, pScale);
}

static bool MapPng_ScaleSurfaceNearest(const cSurface& pSource, cSurface& pTarget, int pScale) {
	SDL_Surface* Source = pSource.GetSurface();
	SDL_Surface* Target = pTarget.GetSurface();
	if (!Source || !Target || pScale <= 1)
		return false;

	if (Target->w != Source->w * pScale || Target->h != Source->h * pScale)
		return false;

	if (!SDL_LockSurface(Source))
		return false;

	if (!SDL_LockSurface(Target)) {
		SDL_UnlockSurface(Source);
		return false;
	}

	for (int y = 0; y < Source->h; ++y) {
		const uint32* SourceRow = (const uint32*)((const uint8*)Source->pixels + (y * Source->pitch));

		for (int yScale = 0; yScale < pScale; ++yScale) {
			uint32* TargetRow = (uint32*)((uint8*)Target->pixels + (((y * pScale) + yScale) * Target->pitch));

			for (int x = 0; x < Source->w; ++x) {
				const uint32 Pixel = SourceRow[x];
				for (int xScale = 0; xScale < pScale; ++xScale)
					TargetRow[(x * pScale) + xScale] = Pixel;
			}
		}
	}

	SDL_UnlockSurface(Target);
	SDL_UnlockSurface(Source);
	return true;
}

#if defined(_WIN32) || defined(WIN32)
static void MapPng_SystemFontTextOut(HDC pDC, int pX, int pY, const std::string& pText) {
	SetTextColor(pDC, RGB(0, 0, 0));
	TextOutA(pDC, pX + 1, pY, pText.c_str(), (int)pText.size());
	TextOutA(pDC, pX - 1, pY, pText.c_str(), (int)pText.size());
	TextOutA(pDC, pX, pY + 1, pText.c_str(), (int)pText.size());
	TextOutA(pDC, pX, pY - 1, pText.c_str(), (int)pText.size());

	SetTextColor(pDC, RGB(255, 255, 255));
	TextOutA(pDC, pX, pY, pText.c_str(), (int)pText.size());
}

static bool MapPng_OverlayLabelsSystemFont(cSurface& pSurface, tSharedBuffer pMap, int pMapWidth, int pMapHeight, bool pMapWordsLittleEndian, bool pAddCoords, bool pAddTileIds, int pScale) {
	SDL_Surface* Surface = pSurface.GetSurface();
	if (!Surface)
		return false;

	const int Width = Surface->w;
	const int Height = Surface->h;
	const int TileSize = 16 * pScale;
	const int Padding = std::max(1, pScale);
	const int FontHeight = std::max(7, std::min(12, 5 + (pScale * 2)));

	BITMAPINFO BitmapInfo;
	std::memset(&BitmapInfo, 0, sizeof(BitmapInfo));
	BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
	BitmapInfo.bmiHeader.biWidth = Width;
	BitmapInfo.bmiHeader.biHeight = -Height;
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = 32;
	BitmapInfo.bmiHeader.biCompression = BI_RGB;

	void* Bits = 0;
	HBITMAP Bitmap = CreateDIBSection(0, &BitmapInfo, DIB_RGB_COLORS, &Bits, 0, 0);
	if (!Bitmap || !Bits)
		return false;

	HDC DC = CreateCompatibleDC(0);
	if (!DC) {
		DeleteObject(Bitmap);
		return false;
	}

	HGDIOBJ OldBitmap = SelectObject(DC, Bitmap);
	HFONT Font = CreateFontA(-FontHeight, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
	if (!Font)
		Font = CreateFontA(-FontHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");

	HGDIOBJ OldFont = Font ? SelectObject(DC, Font) : 0;
	SetBkMode(DC, TRANSPARENT);

	if (!SDL_LockSurface(Surface)) {
		if (OldFont)
			SelectObject(DC, OldFont);
		if (Font)
			DeleteObject(Font);
		SelectObject(DC, OldBitmap);
		DeleteDC(DC);
		DeleteObject(Bitmap);
		return false;
	}

	const SDL_PixelFormatDetails* Format = SDL_GetPixelFormatDetails(Surface->format);
	if (!Format) {
		SDL_UnlockSurface(Surface);
		if (OldFont)
			SelectObject(DC, OldFont);
		if (Font)
			DeleteObject(Font);
		SelectObject(DC, OldBitmap);
		DeleteDC(DC);
		DeleteObject(Bitmap);
		return false;
	}

	uint8* DIB = (uint8*)Bits;
	for (int y = 0; y < Height; ++y) {
		const uint32* SourceRow = (const uint32*)((const uint8*)Surface->pixels + (y * Surface->pitch));
		uint8* TargetRow = DIB + ((size_t)y * (size_t)Width * 4);

		for (int x = 0; x < Width; ++x) {
			uint8 r = 0;
			uint8 g = 0;
			uint8 b = 0;
			SDL_GetRGB(SourceRow[x], Format, 0, &r, &g, &b);

			TargetRow[(x * 4) + 0] = b;
			TargetRow[(x * 4) + 1] = g;
			TargetRow[(x * 4) + 2] = r;
			TargetRow[(x * 4) + 3] = 0;
		}
	}

	SDL_UnlockSurface(Surface);

	for (int y = 0; y < pMapHeight; ++y) {
		for (int x = 0; x < pMapWidth; ++x) {
			const size_t Offset = 0x60 + ((size_t)y * (size_t)pMapWidth + (size_t)x) * 2;
			if (Offset + 1 >= pMap->size())
				continue;

			const int BaseX = (x * TileSize) + Padding;
			const int BaseY = (y * TileSize) + Padding;
			const uint16 TileId = MapPng_ReadMapTileId(pMap, Offset, pMapWordsLittleEndian);

			if (pAddTileIds)
				MapPng_SystemFontTextOut(DC, BaseX, BaseY, MapPng_FormatTileId(TileId));

			if (pAddCoords) {
				const int CoordY = pAddTileIds ? BaseY + FontHeight + 1 : BaseY;
				MapPng_SystemFontTextOut(DC, BaseX, CoordY, MapPng_FormatCoords(x, y));
			}
		}
	}

	if (!SDL_LockSurface(Surface)) {
		if (OldFont)
			SelectObject(DC, OldFont);
		if (Font)
			DeleteObject(Font);
		SelectObject(DC, OldBitmap);
		DeleteDC(DC);
		DeleteObject(Bitmap);
		return false;
	}

	for (int y = 0; y < Height; ++y) {
		uint32* TargetRow = (uint32*)((uint8*)Surface->pixels + (y * Surface->pitch));
		const uint8* SourceRow = DIB + ((size_t)y * (size_t)Width * 4);

		for (int x = 0; x < Width; ++x) {
			const uint8 b = SourceRow[(x * 4) + 0];
			const uint8 g = SourceRow[(x * 4) + 1];
			const uint8 r = SourceRow[(x * 4) + 2];
			TargetRow[x] = SDL_MapRGB(Format, 0, r, g, b);
		}
	}

	SDL_UnlockSurface(Surface);

	if (OldFont)
		SelectObject(DC, OldFont);
	if (Font)
		DeleteObject(Font);
	SelectObject(DC, OldBitmap);
	DeleteDC(DC);
	DeleteObject(Bitmap);
	return true;
}
#endif

static void MapPng_OverlayLabels(cSurface& pSurface, tSharedBuffer pMap, int pMapWidth, int pMapHeight, bool pMapWordsLittleEndian, bool pAddCoords, bool pAddTileIds, int pScale) {
#if defined(_WIN32) || defined(WIN32)
	if (MapPng_OverlayLabelsSystemFont(pSurface, pMap, pMapWidth, pMapHeight, pMapWordsLittleEndian, pAddCoords, pAddTileIds, pScale))
		return;
#endif

	SDL_Surface* Surface = pSurface.GetSurface();
	if (!Surface)
		return;

	if (!SDL_LockSurface(Surface))
		return;

	const SDL_PixelFormatDetails* Format = SDL_GetPixelFormatDetails(Surface->format);
	if (!Format) {
		SDL_UnlockSurface(Surface);
		return;
	}

	const uint32 White = SDL_MapRGB(Format, 0, 255, 255, 255);
	const uint32 Black = SDL_MapRGB(Format, 0, 0, 0, 0);
	const int TileSize = 16 * pScale;
	const int Padding = std::max(1, pScale);
	const int FontScale = pScale >= 3 ? 2 : 1;
	const int FontHeight = 6 * FontScale;
	const int FontSpacing = FontScale;

	for (int y = 0; y < pMapHeight; ++y) {
		for (int x = 0; x < pMapWidth; ++x) {
			const size_t Offset = 0x60 + ((size_t)y * (size_t)pMapWidth + (size_t)x) * 2;
			if (Offset + 1 >= pMap->size())
				continue;

			const int BaseX = (x * TileSize) + Padding;
			const int BaseY = (y * TileSize) + Padding;
			const uint16 TileId = MapPng_ReadMapTileId(pMap, Offset, pMapWordsLittleEndian);

			if (pAddTileIds)
				MapPng_DrawLabel(Surface, BaseX, BaseY, MapPng_FormatTileId(TileId), Black, White, FontSpacing, FontScale);

			if (pAddCoords) {
				const int CoordY = pAddTileIds ? BaseY + FontHeight + FontSpacing : BaseY;
				MapPng_DrawLabel(Surface, BaseX, CoordY, MapPng_FormatCoords(x, y), Black, White, FontSpacing, FontScale);
			}
		}
	}

	SDL_UnlockSurface(Surface);
}

static bool MapPng_LoadPalette(const std::string& pMapPath, const std::string& pBaseName, tSharedBuffer pBaseBlk, std::vector<uint32>& pPalette, bool& pAmigaFormat) {
	pPalette.assign(256, 0);
	pAmigaFormat = false;

	const std::string BaseStem = MapPng_WithExtension(pBaseName, "");
	std::string PalPath = MapPng_FindDataFile(pMapPath, BaseStem + ".pal");
	if (!PalPath.size())
		PalPath = MapPng_FindDataFile(pMapPath, BaseStem + ".PAL");

	if (PalPath.size()) {
		tSharedBuffer PalBytes = MapPng_ReadFileMaybeRnc(PalPath);
		if (!PalBytes || PalBytes->size() < 2)
			return false;

		pAmigaFormat = true;
		const size_t Colors = std::min<size_t>(16, PalBytes->size() / 2);
		for (size_t i = 0; i < Colors; ++i) {
			const uint16 Word = readBEWord(PalBytes->data() + (i * 2));
			const uint8 R = (uint8)((((Word >> 8) & 0xF) << 4) | ((Word >> 8) & 0xF));
			const uint8 G = (uint8)((((Word >> 4) & 0xF) << 4) | ((Word >> 4) & 0xF));
			const uint8 B = (uint8)(((Word & 0xF) << 4) | (Word & 0xF));
			pPalette[i] = ((uint32)R << 16) | ((uint32)G << 8) | (uint32)B;
		}
		return true;
	}

	if (!pBaseBlk || pBaseBlk->size() < 0xFA00 + (0x80 * 3))
		return false;

	for (size_t i = 0; i < 0x80; ++i) {
		const uint8 R = (*pBaseBlk)[0xFA00 + (i * 3) + 0];
		const uint8 G = (*pBaseBlk)[0xFA00 + (i * 3) + 1];
		const uint8 B = (*pBaseBlk)[0xFA00 + (i * 3) + 2];
		pPalette[i] = ((uint32)R << 16) | ((uint32)G << 8) | (uint32)B;
	}

	return true;
}

static std::vector<uint8> MapPng_DecodeTile(tSharedBuffer pBlk, int pTileId, bool pAmigaFormat) {
	std::vector<uint8> Pixels(16 * 16, 0);
	if (!pBlk || pTileId < 0)
		return Pixels;

	if (pAmigaFormat) {
		const size_t Offset = (size_t)pTileId << 7;
		if (Offset + 128 > pBlk->size())
			return Pixels;

		for (int plane = 0; plane < 4; ++plane) {
			const size_t PlaneBase = Offset + ((size_t)plane * 32);
			for (int row = 0; row < 16; ++row) {
				const uint16 Word = readBEWord(pBlk->data() + PlaneBase + ((size_t)row * 2));
				for (int x = 0; x < 16; ++x) {
					if (Word & (0x8000 >> x))
						Pixels[((size_t)row * 16) + x] |= (uint8)(1 << plane);
				}
			}
		}
	}
	else {
		const int Col = pTileId % 20;
		const int RowGroup = pTileId / 20;
		const size_t Base = ((size_t)RowGroup * 0x12C0) + ((size_t)Col * 0x10);
		if (Base >= pBlk->size())
			return Pixels;

		for (int row = 0; row < 16; ++row) {
			const size_t Src = Base + ((size_t)row * 320);
			if (Src + 16 <= pBlk->size())
				std::memcpy(Pixels.data() + ((size_t)row * 16), pBlk->data() + Src, 16);
		}
	}

	return Pixels;
}

bool cFodder::Map_RenderPng(const std::string& pMapPath, const std::string& pOutputPath, bool pAddCoords, bool pAddTileIds, size_t pScale) {
	std::string MapPath = MapPng_ResolveMapPath(pMapPath);
	tSharedBuffer MapFile = MapPng_ReadFileMaybeRnc(MapPath);
	const int OutputScale = (int)std::max<size_t>(1, std::min<size_t>(4, pScale));

	if (!MapFile || MapFile->size() < 0x60) {
		g_Debugger->Error("Map PNG render failed: map not found or too small: " + MapPath);
		return false;
	}

	const uint16 Width = readBEWord(MapFile->data() + 0x54);
	const uint16 Height = readBEWord(MapFile->data() + 0x56);
	const size_t ExpectedSize = 0x60 + ((size_t)Width * (size_t)Height * 2);

	if (!Width || !Height || Width > 4096 || Height > 4096 || ExpectedSize > MapFile->size()) {
		g_Debugger->Error("Map PNG render failed: invalid or truncated map: " + MapPath);
		return false;
	}

	const std::string BaseName = MapPng_ReadAsciiAt(MapFile, 0x00, 11);
	const std::string SubName = MapPng_ReadAsciiAt(MapFile, 0x10, 11);
	const std::string BasePath = MapPng_FindDataFile(MapPath, BaseName);
	const std::string SubPath = MapPng_FindDataFile(MapPath, SubName);

	if (!BasePath.size() || !SubPath.size()) {
		g_Debugger->Error("Map PNG render failed: could not find map tilesets: " + BaseName + " / " + SubName);
		return false;
	}

	tSharedBuffer BaseBlk = MapPng_ReadFileMaybeRnc(BasePath);
	tSharedBuffer SubBlk = MapPng_ReadFileMaybeRnc(SubPath);
	if (!BaseBlk || !BaseBlk->size() || !SubBlk || !SubBlk->size()) {
		g_Debugger->Error("Map PNG render failed: could not read map tilesets: " + BasePath + " / " + SubPath);
		return false;
	}

	auto TileBlk = std::make_shared<std::vector<uint8>>();
	TileBlk->reserve(BaseBlk->size() + SubBlk->size());
	TileBlk->insert(TileBlk->end(), BaseBlk->begin(), BaseBlk->end());
	TileBlk->insert(TileBlk->end(), SubBlk->begin(), SubBlk->end());

	bool AmigaFormat = false;
	std::vector<uint32> Palette;
	if (!MapPng_LoadPalette(MapPath, BaseName, BaseBlk, Palette, AmigaFormat)) {
		g_Debugger->Error("Map PNG render failed: could not load palette for " + BaseName);
		return false;
	}

	cSurface RenderSurface((size_t)Width * 16, (size_t)Height * 16);
	RenderSurface.clearBuffer();

	SDL_Surface* Surface = RenderSurface.GetSurface();
	if (!Surface || !SDL_LockSurface(Surface)) {
		g_Debugger->Error("Map PNG render failed: could not lock output surface");
		return false;
	}

	const SDL_PixelFormatDetails* Format = SDL_GetPixelFormatDetails(Surface->format);
	if (!Format) {
		SDL_UnlockSurface(Surface);
		g_Debugger->Error("Map PNG render failed: unsupported output pixel format");
		return false;
	}

	std::map<int, std::vector<uint8>> TileCache;
	for (int y = 0; y < (int)Height; ++y) {
		for (int x = 0; x < (int)Width; ++x) {
			const size_t Offset = 0x60 + ((size_t)y * (size_t)Width + (size_t)x) * 2;
			if (Offset + 1 >= MapFile->size())
				continue;

			const uint16 TileId = MapPng_ReadMapTileId(MapFile, Offset, false);
			if (!TileCache.count((int)TileId))
				TileCache[(int)TileId] = MapPng_DecodeTile(TileBlk, (int)TileId, AmigaFormat);
			const std::vector<uint8>& TilePixels = TileCache[(int)TileId];

			for (int ty = 0; ty < 16; ++ty) {
				for (int tx = 0; tx < 16; ++tx) {
					const uint8 Index = TilePixels[((size_t)ty * 16) + tx];
					const uint32 Rgb = Palette[Index];
					const uint8 R = (uint8)((Rgb >> 16) & 0xFF);
					const uint8 G = (uint8)((Rgb >> 8) & 0xFF);
					const uint8 B = (uint8)(Rgb & 0xFF);
					MapPng_SetPixel(Surface, (x * 16) + tx, (y * 16) + ty, SDL_MapRGB(Format, 0, R, G, B));
				}
			}
		}
	}
	SDL_UnlockSurface(Surface);

	std::unique_ptr<cSurface> ScaledSurface;
	cSurface* OutputSurface = &RenderSurface;
	if (OutputScale > 1) {
		ScaledSurface = std::make_unique<cSurface>(
			(size_t)Width * 16 * (size_t)OutputScale,
			(size_t)Height * 16 * (size_t)OutputScale);

		if (!MapPng_ScaleSurfaceNearest(RenderSurface, *ScaledSurface, OutputScale)) {
			g_Debugger->Error("Map PNG render failed: could not scale output");
			return false;
		}

		OutputSurface = ScaledSurface.get();
	}

	if (pAddCoords || pAddTileIds)
		MapPng_OverlayLabels(*OutputSurface, MapFile, Width, Height, false, pAddCoords, pAddTileIds, OutputScale);

	std::string OutputPath = pOutputPath;
	if (!OutputPath.size())
		OutputPath = MapPng_WithExtension(MapPath, (pAddCoords || pAddTileIds) ? ".tiles.png" : ".png");

	if (!OutputSurface->SavePng(OutputPath)) {
		g_Debugger->Error("Map PNG render failed: could not write output: " + OutputPath);
		return false;
	}

	g_Debugger->Notice("map=" + MapPng_BaseName(MapPath) +
		" format=" + std::string(AmigaFormat ? "amiga" : "pc") +
		" size=" + std::to_string(Width) + "x" + std::to_string(Height) +
		" base=" + BaseName +
		" sub=" + SubName +
		" scale=" + std::to_string(OutputScale));
	g_Debugger->Notice("wrote=" + OutputPath + " " + std::to_string(OutputSurface->GetWidth()) + "x" + std::to_string(OutputSurface->GetHeight()));
	return true;
}
