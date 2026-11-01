/* Open Fodder - SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#include "Network/NetworkMenuText.hpp"

namespace MenuPresentation {
inline std::string FitText(const std::string& text, int width) {
    std::string result;
    int used = 0;
    for (const unsigned char raw : text) {
        const auto ch = raw >= 32 && raw < 127 ? (unsigned char)std::toupper(raw) : (unsigned char)'?';
        const int advance = mFont_Briefing_Width[ch];
        if (used + advance > width) {
            while (!result.empty() && used + 18 > width) {
                used -= mFont_Briefing_Width[(unsigned char)result.back()];
                result.pop_back();
            }
            return result + "...";
        }
        result.push_back(char(ch));
        used += advance;
    }
    return result;
}
// Compose outlined menu text over a freshly drawn scene. Keep the mask local
// so nested menus never inherit another page's lettering or hit targets.
class Frame {
    cFodder& mGame;
    std::vector<uint8> mBackground;
public:
    explicit Frame(cFodder& game) : mGame(game) {
        if (game.mMapLoaded && game.mMap && !game.mMap->empty()) {
            if (!game.mStartParams->mDisableVideo) game.mGraphics->MapTiles_Draw();
            game.Sprites_Draw();
        } else game.mSurface->clearBuffer();
        game.mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
        game.GUI_Element_Reset();
        auto* pixels = game.mSurface->GetSurfaceBuffer();
        const auto size = game.mSurface->GetSurfaceBufferSize();
        mBackground.assign(pixels, pixels + size);
        std::fill(pixels, pixels + size, uint8(0xFF));
    }
    ~Frame() {
        auto* pixels = mGame.mSurface->GetSurfaceBuffer();
        const size_t stride = mGame.mSurface->GetWidth();
        const std::vector<uint8> foreground(pixels, pixels + mBackground.size());
        std::copy(mBackground.begin(), mBackground.end(), pixels);
        for (size_t y = 1; y + 1 < mGame.mSurface->GetHeight(); ++y)
            for (size_t x = 1; x + 1 < stride; ++x) {
                if (foreground[y * stride + x] == 0xFF) continue;
                for (int dy = -1; dy <= 1; ++dy)
                    for (int dx = -1; dx <= 1; ++dx)
                        pixels[(y + dy) * stride + x + dx] = 0;
            }
        for (size_t i = 0; i < foreground.size(); ++i)
            if (foreground[i] != 0xFF) pixels[i] = foreground[i];
    }
    Frame(const Frame&) = delete;
    Frame& operator=(const Frame&) = delete;
};

inline void Animate(cFodder& game) {
    if (game.mPhase_InterruptTicks >= 3) {
        game.mPhase_InterruptTicks = 0;
        game.Mission_Sprites_Handle();
        game.Sound_Tick();
    }
}

inline void Title(cFodder& game, const char* title, const char* subtitle) {
    game.mString_GapCharID = 0;
    game.String_Print_Large(title, false, 19);
    game.String_Print_Small_CentreInBox(FitText(subtitle, 292), 14, 306, 44);
}

// Establish the same geometry for drawing and subsequent button registration.
inline void Button(cFodder& game, const std::string& text, int x1, int x2, int y,
                   bool selected = false, bool enabled = true) {
    const bool hover = enabled && game.mMouseX + 32 >= x1 && game.mMouseX + 32 <= x2 &&
        game.mMouseY >= y && game.mMouseY <= y + 15;
    const auto fitted = FitText(text, x2 - x1 - 12);
    game.String_Print_Small_CentreInBox(fitted, x1, x2, y + 5);
    if (hover || selected) game.Briefing_DrawBox(x1, y, x2 - x1, 16, 0xF3);
    game.mGUI_Temp_X = int16(x1);
    game.mGUI_Temp_Y = int16(y);
    game.mGUI_Temp_Width = int16(x2 - x1);
    game.mGUI_Draw_LastHeight = 10;
}
}
