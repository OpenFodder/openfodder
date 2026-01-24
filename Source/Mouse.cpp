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

const int16 MOUSE_POSITION_X_ADJUST = -32;
const int16 MOUSE_POSITION_Y_ADJUST = 4;

void cFodder::Mouse_Setup() {

    mMouse_EventLastButtonsPressed = 0;
    mButtonPressLeft = 0;
    mButtonPressRight = 0;
    mMouseButtonStatus = 0;

    mMouseX = (getCameraWidth() / 2) - 9;
    mMouseY = (getCameraHeight() / 2) - 9;
    mInputMouseX = mMouseX;
    mInputMouseY = mMouseY;
}

void cFodder::Mouse_Cursor_Handle() {
	static int32 CaptureBlockFrames = 0;
    static bool MouseCapturedGlobal = false;
	const cDimension scale = mWindow->GetScale();
    static bool CursorCaptured = true;
    const int edgePadding = 2;
    const uint64 recaptureDelayMs = 150;

	mMouseButtonStatus = mMouse_EventLastButtonsPressed;

    auto UpdateInputFromWindowPos = [&]() {
        float x = 0.0f;
        float y = 0.0f;
        SDL_GetMouseState(&x, &y);
        mInputMouseX = (static_cast<int>(x) / scale.getWidth()) + MOUSE_POSITION_X_ADJUST;
        mInputMouseY = (static_cast<int>(y) / scale.getHeight()) + MOUSE_POSITION_Y_ADJUST;
    };

    auto ReleaseCursor = [&](bool pWarpToEdge) {
        CursorCaptured = false;
        mMouse_LeftWindow = static_cast<uint64>(SDL_GetTicks());
        mWindow->SetRelativeMouseMode(false);
        SDL_GetRelativeMouseState(nullptr, nullptr);
        mMouse_EventLastPositionRelative = { 0, 0 };
        SDL_ShowCursor();
        mWindow_MouseInside = false;

        if (MouseCapturedGlobal) {
            SDL_CaptureMouse(false);
            MouseCapturedGlobal = false;
        }

        if (!pWarpToEdge)
            return;

        const cDimension windowSize = mWindow->GetWindowSize();
        int x = (mMouseX - MOUSE_POSITION_X_ADJUST) * scale.getWidth();
        int y = (mMouseY - MOUSE_POSITION_Y_ADJUST) * scale.getHeight();
        if (x < 0)
            x = 0;
        else if (x >= windowSize.getWidth())
            x = windowSize.getWidth() - 1;
        if (y < 0)
            y = 0;
        else if (y >= windowSize.getHeight())
            y = windowSize.getHeight() - 1;
        mWindow->SetMousePositionInWindow(static_cast<float>(x), static_cast<float>(y));
    };

	if (mStartParams->mMouseAlternative) {
		mInputMouseX = (mMouse_EventLastPosition.mX / scale.getWidth()) + MOUSE_POSITION_X_ADJUST;
		mInputMouseY = (mMouse_EventLastPosition.mY / scale.getHeight()) + MOUSE_POSITION_Y_ADJUST;
		return;
	}

    if (!mWindow_Focus) {
        if (CursorCaptured) {
            CursorCaptured = false;
            mWindow->SetRelativeMouseMode(false);
            SDL_GetRelativeMouseState(nullptr, nullptr);
            mMouse_EventLastPositionRelative = { 0, 0 };
            SDL_ShowCursor();
        }
        mWindow_MouseInside = false;
        if (MouseCapturedGlobal) {
            SDL_CaptureMouse(false);
            MouseCapturedGlobal = false;
        }
        if (mWindow->isMouseInside()) {
            // Register mouse position even when not focused but cursor on window
            UpdateInputFromWindowPos();
        }
        return;
    }

    // Free cursor mode: wait for window mouse enter event to recapture.
    if (!CursorCaptured) {
        const uint64 now = static_cast<uint64>(SDL_GetTicks());
        const bool canRecapture = (now - mMouse_LeftWindow) > recaptureDelayMs;
        const bool inside = mWindow_MouseInside || mWindow->isMouseInside();

        if (canRecapture && inside) {
            float wx = 0.0f, wy = 0.0f;
            SDL_GetMouseState(&wx, &wy);
            const cDimension windowSize = mWindow->GetWindowSize();
            const bool awayFromEdge =
                (wx > edgePadding) &&
                (wy > edgePadding) &&
                (wx < static_cast<float>(windowSize.getWidth() - edgePadding)) &&
                (wy < static_cast<float>(windowSize.getHeight() - edgePadding));
            if (!awayFromEdge)
                return;

            UpdateInputFromWindowPos();
            mMouseX = mInputMouseX;
            mMouseY = mInputMouseY;

            CaptureBlockFrames = 1;
            CursorCaptured = true;
            mWindow->SetRelativeMouseMode(true);
            SDL_HideCursor();
            SDL_GetRelativeMouseState(nullptr, nullptr);
            mMouse_EventLastPositionRelative = { 0, 0 };
        }
        return;
    }

    if (!mParams->mMouseLocked && !mWindow->isFullscreen()) {
        if (Mouse_IsOnBorder() && !mMouseButtonStatus) {
            ReleaseCursor(true);
            return;
        }
    }

    // hack to avoid moving cursor on window resizing
    if (mWindow->isResized()) {
        mWindow->ClearResized();

    } else {
        if (CaptureBlockFrames > 0) {
            --CaptureBlockFrames;
        } else {
            // Calc the distance from the cursor to the centre of the window
            mInputMouseX = mMouseX + static_cast<int16>(mMouse_EventLastPositionRelative.mX);
            mInputMouseY = mMouseY + static_cast<int16>(mMouse_EventLastPositionRelative.mY);
            mMouse_EventLastPositionRelative = { 0,0 };
        }
    }
}

bool cFodder::Mouse_IsOnBorder() const {
    const cDimension ScreenSize = mWindow->GetScreenSize();

    if (mMouseX <= MOUSE_POSITION_X_ADJUST)
        return true;
    if (mMouseX >= ScreenSize.getWidth() + MOUSE_POSITION_X_ADJUST - 1)
        return true;
    if (mMouseY <= MOUSE_POSITION_Y_ADJUST)
        return true;
    if (mMouseY >= ScreenSize.getHeight() + MOUSE_POSITION_Y_ADJUST - 1)
        return true;

    return false;
}

void cFodder::Mouse_ReadInputs() {

    if (mParams->mDemoPlayback) {

        auto State = mGame_Data.mDemoRecorded.GetState(mGame_Data.mDemoRecorded.mTick);
        if (State) {
            mInputMouseX = State->mInputMouseX;
            mInputMouseY = State->mInputMouseY;

            mMouseButtonStatus = State->mMouseButtonStatus;
        }
        else {
            if(mGame_Data.mDemoRecorded.mTick > mGame_Data.mDemoRecorded.GetTotalTicks() + 100 )
                mPhase_Aborted = true;
        }
    }
    else {
        Mouse_Cursor_Handle();
    }

    if (mParams->mDemoRecord && !mParams->mDemoPlayback)
        mGame_Data.mDemoRecorded.AddState(mGame_Data.mDemoRecorded.mTick, cStateRecorded{ mInputMouseX, mInputMouseY, mMouseButtonStatus });

    Mouse_UpdateButtons();

    int16 Data4 = mInputMouseX;

    if (mSidebar_SmallMode == 0)
        goto loc_13B3A;

    if (Data4 >= MOUSE_POSITION_X_ADJUST + 16)
        goto loc_13B58;

    goto loc_13B41;

loc_13B3A:;
    if (Data4 >= MOUSE_POSITION_X_ADJUST)
        goto loc_13B58;

loc_13B41:;
    if (mSidebar_SmallMode)
        Data4 = MOUSE_POSITION_X_ADJUST + 16;
    else
        Data4 = MOUSE_POSITION_X_ADJUST;

    goto loc_13B66;

loc_13B58:;

    if (Data4 > (int16)mWindow->GetScreenSize().getWidth() + MOUSE_POSITION_X_ADJUST - 1)
        Data4 = (int16)mWindow->GetScreenSize().getWidth() + MOUSE_POSITION_X_ADJUST - 1;

loc_13B66:;
    mMouseX = Data4;

    int16 Data0 = mInputMouseY;

    if (Data0 < MOUSE_POSITION_Y_ADJUST)
        Data0 = MOUSE_POSITION_Y_ADJUST;
    else {

        if (Data0 > (int16)mWindow->GetScreenSize().getHeight() + MOUSE_POSITION_Y_ADJUST - 1)
            Data0 = (int16)mWindow->GetScreenSize().getHeight() + MOUSE_POSITION_Y_ADJUST - 1;
    }

    mMouseY = Data0;
}

void cFodder::Mouse_UpdateButtons() {

    mButtonPressLeft = 0;
    if (mMouseButtonStatus & 1) {
        mButtonPressLeft -= 1;
        if (mMouse_Button_Left_Toggle == 0) {
            mMouse_Button_Left_Toggle = -1;
            mMouse_Exit_Loop = true;

            if (mButtonPressRight) {
                mMouse_Button_LeftRight_Toggle = true;
                mMouse_Button_LeftRight_Toggle2 = true;
            }
        }

    }
    else {
        mMouse_Button_LeftRight_Toggle2 = false;
        mMouse_Button_Left_Toggle = 0;
    }

    mButtonPressRight = 0;
    if (mMouseButtonStatus & 2) {
        mButtonPressRight -= 1;
        if (mMouse_Button_Right_Toggle == 0) {
            mSquad_Member_Fire_CoolDown_Override = true;
            mMouse_Button_Right_Toggle = -1;
        }
    }
    else {
        mMouse_Button_Right_Toggle = 0;
    }

}

void cFodder::Mouse_DrawCursor() {
    if (mParams->mDisableVideo)
        return;

    mVideo_Draw_PosX = (mMouseX + mMouseX_Offset) + SIDEBAR_WIDTH;
    mVideo_Draw_PosY = (mMouseY + mMouseY_Offset) + 12;

    if (mMouseSpriteNew >= 0) {
        mMouseSpriteCurrent = mMouseSpriteNew;
        mMouseSpriteNew = -1;
    }

    if (mGraphics)
        mGraphics->Mouse_DrawCursor();
}

void cFodder::Mouse_UpdateCursor() {
    int16 Data0, Data4, Data8;

    // Sidebar
    if (mMouseX < 32) {
        mMouseSpriteNew = eSprite_pStuff_Mouse_Cursor;
        mMouseX_Offset = 0;
        mMouseY_Offset = 0;
        return;
    }

    if (!mButtonPressRight) {

        Data0 = mMouseX + (mCameraX >> 16);
        Data4 = mMouseY + (mCameraY >> 16);

        Data0 -= 0x0F;
        Data4 -= 3;

        //loc_30B36
        for (auto& Vehicle : mSprites_HumanVehicles) {

            if (Vehicle->mPosX < 0)
                continue;

            if (!Vehicle->mVehicleEnabled)
                continue;

            // If not human
            if (Vehicle->mPersonType != eSprite_PersonType_Human)
                continue;

            Data8 = Vehicle->mPosX;
            if (Vehicle->mVehicleType == eVehicle_Turret_Cannon)
                goto loc_30BB9;
            if (Vehicle->mVehicleType == eVehicle_Turret_Missile)
                goto loc_30BBE;

        loc_30BB9:;
            Data8 -= 8;
        loc_30BBE:;

            if (Data0 < Data8)
                continue;

            Data8 += mSprite_Width[Vehicle->mSpriteType];
            if (Data0 > Data8)
                continue;

            Data8 = Vehicle->mPosY;
            Data8 -= Vehicle->mHeight;
            Data8 -= mSprite_Height_Top[Vehicle->mSpriteType];
            Data8 -= 0x14;

            if (Data4 < Data8)
                continue;

            Data8 = Vehicle->mPosY;
            Data8 -= Vehicle->mHeight;
            if (Data4 > Data8)
                continue;

            mMouseSetToCursor = -1;

            // Is vehicle off ground
            if (Vehicle->mHeight) {

                // And is current
                if (Vehicle != mSquad_CurrentVehicle)
                    return;

                if (Vehicle->mSheetIndex == 0xA5)
                    return;

                // Show the helicopter land icon
                mMouseSpriteNew = eSprite_pStuff_Mouse_Helicopter;
                mMouseX_Offset = 0;
                mMouseY_Offset = 0;
                return;
            }

            // Is this the vehicle the current squad is in
            if (Vehicle == mSquad_CurrentVehicle)
                mMouseSpriteNew = eSprite_pStuff_Mouse_Arrow_DownRight;
            else
                mMouseSpriteNew = eSprite_pStuff_Mouse_Arrow_UpLeft;

            mMouseX_Offset = 0;
            mMouseY_Offset = 0;
            return;
        }
    }

    if (mMouseSetToCursor) {
        mMouseSetToCursor = 0;
        mMouseSpriteNew = eSprite_pStuff_Mouse_Cursor;
        mMouseX_Offset = 0;
        mMouseY_Offset = 0;
    }
}

void cFodder::Mouse_Inputs_Check() {
    int16 Data0 = 0;
    int16 Data4 = 0;

    if (mMouseDisabled)
        return;

    if (mPhase_In_Progress)
        Mouse_UpdateCursor();

    if (dword_3A030) {
        // TODO: Function pointer call, but appears not to be used
        //dword_3A030();
        return;
    }

    for (sGUI_Element* Loop_Element = mGUI_Elements;; ++Loop_Element) {

        if (Loop_Element->field_0 == 0)
            break;

        if ((*this.*Loop_Element->field_0)() < 0)
            return;

        Data0 = mGUI_Mouse_Modifier_X + Loop_Element->mX;

        int16 Data4 = mMouseX + 0x20;

        if (Data0 > Data4)
            continue;

        Data0 += Loop_Element->mWidth;
        if (Data0 < Data4)
            continue;

        Data0 = mGUI_Mouse_Modifier_Y;
        Data0 += Loop_Element->mY;
        if (Data0 > mMouseY)
            continue;

        Data0 += Loop_Element->mHeight;
        if (Data0 < mMouseY)
            continue;

        (*this.*Loop_Element->mMouseInsideFuncPtr)();
        return;
    }

    if (!mButtonPressRight)
        goto loc_30814;

    if (mSquad_Selected < 0)
        return;

    if (mMouseSpriteNew < 0) {
        mMouseSpriteNew = eSprite_pStuff_Mouse_Target;
        mMouseX_Offset = -8;
        mMouseY_Offset = -8;
    }
    Squad_Member_Target_Set();
    if (!mSquad_CurrentVehicle)
        return;

loc_30814:;

    if (mSquad_CurrentVehicle) {
        Vehicle_Input_Handle();
        return;
    }

    if (Mouse_Button_Left_Toggled() < 0)
        return;

    if (mMouse_Button_LeftRight_Toggle2)
        return;

    if (mMouseSpriteNew < 0) {
        mMouseSpriteNew = eSprite_pStuff_Mouse_Cursor;
        mMouseX_Offset = 0;
        mMouseY_Offset = 0;
    }

    if (mSquad_Selected < 0) {

        Squad_Member_Click_Check();
        return;
    }

    sSprite** Data24 = mSquads[mSquad_Selected];
    sSprite* Dataa24 = *Data24;

    if (Dataa24 == INVALID_SPRITE_PTR) {
        Squad_Member_Click_Check();
        return;
    }

    if (Dataa24->mInVehicle)
        return;

    if (word_3B2F1)
        word_3B2F1 = 0;
    else
        Squad_Assign_Target_From_Mouse();

    mSquad_Member_Clicked_TroopInSameSquad = 0;
    Squad_Member_Click_Check();

    // If we clicked on a member of the current squad, nothing to do
    if (mSquad_Member_Clicked_TroopInSameSquad)
        return;

    if (mSquad_Leader == INVALID_SPRITE_PTR || mSquad_Leader == 0)
        return;

    Data0 = mMouseX + (mCameraX >> 16);
    Data4 = mMouseY + (mCameraY >> 16);

    Data0 += -22;
    if (Data0 < 0)
        Data0 = 0;

    Data4 += -3;
    if (Data4 < 0)
        Data4 = 3;

    if (Data4 < 3)
        Data4 = 3;

    mCamera_PanTargetX = Data0;
    mCamera_PanTargetY = Data4;

    mMouse_Locked = false;

    for (auto& Troop : mGame_Data.mSoldiers_Allocated) {

        if (Troop.mSprite == INVALID_SPRITE_PTR || Troop.mSprite == 0)
            continue;

        // Not in the squad?
        if (mSquad_Selected != Troop.mSprite->field_32)
            continue;

        Troop.mSprite->field_44 = 0;
        Troop.mSprite->mPosXFrac = 0;
        Troop.mSprite->mPosYFrac = 0;
    }

    int16 Data10 = mSquad_Leader->mPosX;
    int16 Data8 = Data4;
    Data4 = Data0;

    Squad_Walk_Target_Set(Data4, Data8, mSquad_Leader->field_32, Data10);

    // Reset the join target
    for (auto& JoinTargetSquad : mSquad_Join_TargetSquad) {

        if (mSquad_Selected != JoinTargetSquad)
            continue;

        JoinTargetSquad = -1;
    }
}

int16 cFodder::Mouse_Button_Right_Toggled() {

    if (mMouse_Button_Right_Toggle < 0) {
        mMouse_Button_Right_Toggle = 1;
        return 1;
    }

    return -1;
}
