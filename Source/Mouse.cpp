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
	static bool WasClicked = false;

	const cPosition WindowPos = mWindow->GetWindowPosition();
	const cDimension ScreenSize = mWindow->GetScreenSize();
	const cDimension WindowSize = mWindow->GetWindowSize();
	const cDimension scale = mWindow->GetScale();
    static bool CursorGrabbed = true;

	mMouseButtonStatus = mMouse_EventLastButtonsPressed;

    if (!mWindow_Focus && CursorGrabbed) {
        CursorGrabbed = false;
    }

	if (mStartParams->mMouseAlternative) {
		mInputMouseX = (mMouse_EventLastPosition.mX / scale.getWidth()) + MOUSE_POSITION_X_ADJUST;
		mInputMouseY = (mMouse_EventLastPosition.mY / scale.getHeight()) + MOUSE_POSITION_Y_ADJUST;
		return;
	}

    // Check if the system mouse is grabbed
    if (!CursorGrabbed) {

        if (!mWindow_Focus && mWindow->isMouseInside()) {
            // Register mouse position even when not focused but cursor on window
            mInputMouseX = (mMouse_EventLastPosition.mX / scale.getWidth()) + MOUSE_POSITION_X_ADJUST;
            mInputMouseY = (mMouse_EventLastPosition.mY / scale.getHeight()) + MOUSE_POSITION_Y_ADJUST;
        }
        
        if (SDL_GetTicks() - mMouse_LeftWindow < 100)
            return;

        // Check if the system cursor x/y is inside our window
        // and ensure the mouse button has been released before we focus
        if (mWindow_Focus && mWindow->isMouseInside() && !mMouseButtonStatus) {
            WasClicked = true;
            CursorGrabbed = true;
            
            if (!mWindow->isFullscreen()) {
                // Ensure X not too close to a border
                if (mInputMouseX <= MOUSE_POSITION_X_ADJUST)
                    mInputMouseX = MOUSE_POSITION_X_ADJUST + 1;
                else if (mInputMouseX >= ScreenSize.getWidth() + MOUSE_POSITION_X_ADJUST - 1)
                    mInputMouseX = ScreenSize.getWidth() + MOUSE_POSITION_X_ADJUST - 2;

                // Ensure Y not too close to a border
                if (mInputMouseY <= MOUSE_POSITION_Y_ADJUST)
                    mInputMouseY = MOUSE_POSITION_Y_ADJUST + 1;
                else if (mInputMouseY >= ScreenSize.getHeight() + MOUSE_POSITION_Y_ADJUST - 1)
                    mInputMouseY = ScreenSize.getHeight() + MOUSE_POSITION_Y_ADJUST - 2;
            }
            mWindow->SetRelativeMouseMode(true);
        }
    } else {
        if (!mWindow->isFullscreen()) {
            if (!mParams->mMouseLocked) {
                cPosition BorderMouse = Mouse_GetOnBorderPosition();

                // Cursor leaving window
                if (BorderMouse.mX || BorderMouse.mY) {
                    CursorGrabbed = false;
                    mMouse_LeftWindow = SDL_GetTicks();

                    mWindow->SetRelativeMouseMode(false);
                    mWindow->SetMousePosition(BorderMouse);
                    return;
                }
            }
        }

        // hack to avoid moving cursor on window resizing
        if (mWindow->isResized()) {
            mWindow->ClearResized();

        } else {
            if (WasClicked) {
                if (!mWindow->isMouseButtonPressed_Global())
                    WasClicked = false;
            } else {
                // Calc the distance from the cursor to the centre of the window
                mInputMouseX = mMouseX + static_cast<int16>(mMouse_EventLastPositionRelative.mX);
                mInputMouseY = mMouseY + static_cast<int16>(mMouse_EventLastPositionRelative.mY);
                mMouse_EventLastPositionRelative = { 0,0 };
            }
        }
    }
}

cPosition cFodder::Mouse_GetOnBorderPosition() {
    cPosition BorderMouse;
    const cPosition WindowPos = mWindow->GetWindowPosition();
    const cDimension ScreenSize = mWindow->GetScreenSize();
    const cDimension WindowSize = mWindow->GetWindowSize();
    const cDimension scale = mWindow->GetScale();

    if ((mMouseX <= MOUSE_POSITION_X_ADJUST && mMouseY <= MOUSE_POSITION_Y_ADJUST) ||
        (mMouseX >= ScreenSize.getWidth() + MOUSE_POSITION_X_ADJUST - 1 && mMouseY <= MOUSE_POSITION_Y_ADJUST) ||
        (mMouseX <= MOUSE_POSITION_X_ADJUST && mMouseY >= ScreenSize.getHeight() + MOUSE_POSITION_Y_ADJUST - 1) ||
        (mMouseX >= ScreenSize.getWidth() + MOUSE_POSITION_X_ADJUST - 1 && mMouseY >= ScreenSize.getHeight() + MOUSE_POSITION_Y_ADJUST - 1)) {
        BorderMouse.mX = mMouseX <= MOUSE_POSITION_X_ADJUST ? WindowPos.mX - 1 : WindowPos.mX + WindowSize.getWidth() + 1;
        BorderMouse.mY = mMouseY <= MOUSE_POSITION_Y_ADJUST ? WindowPos.mY - 1 : WindowPos.mY + WindowSize.getHeight() + 1;
    }
    // Need to check if the game cursor is near a border on X axis
    else if (mMouseX <= MOUSE_POSITION_X_ADJUST || mMouseX >= ScreenSize.getWidth() + MOUSE_POSITION_X_ADJUST - 1) {
        BorderMouse.mX = mMouseX <= MOUSE_POSITION_X_ADJUST ? WindowPos.mX - 1 : WindowPos.mX + WindowSize.getWidth() + 1;
        BorderMouse.mY = WindowPos.mY + (mMouseY - MOUSE_POSITION_Y_ADJUST) * scale.getHeight();
    } // Need to check if the game cursor is near a border Y axis
    else if (mMouseY <= MOUSE_POSITION_Y_ADJUST || mMouseY >= ScreenSize.getHeight() + MOUSE_POSITION_Y_ADJUST - 1) {
        BorderMouse.mX = WindowPos.mX + (mMouseX - MOUSE_POSITION_X_ADJUST) * scale.getWidth();
        BorderMouse.mY = mMouseY <= MOUSE_POSITION_Y_ADJUST ? WindowPos.mY - 1 : WindowPos.mY + WindowSize.getHeight() + 1;
    }

    return BorderMouse;
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

            if (Vehicle->field_0 < 0)
                continue;

            if (!Vehicle->field_65)
                continue;

            // If not human
            if (Vehicle->field_22 != eSprite_PersonType_Human)
                continue;

            Data8 = Vehicle->field_0;
            if (Vehicle->field_6F == eVehicle_Turret_Cannon)
                goto loc_30BB9;
            if (Vehicle->field_6F == eVehicle_Turret_Missile)
                goto loc_30BBE;

        loc_30BB9:;
            Data8 -= 8;
        loc_30BBE:;

            if (Data0 < Data8)
                continue;

            Data8 += mSprite_Width[Vehicle->field_18];
            if (Data0 > Data8)
                continue;

            Data8 = Vehicle->field_4;
            Data8 -= Vehicle->field_20;
            Data8 -= mSprite_Height_Top[Vehicle->field_18];
            Data8 -= 0x14;

            if (Data4 < Data8)
                continue;

            Data8 = Vehicle->field_4;
            Data8 -= Vehicle->field_20;
            if (Data4 > Data8)
                continue;

            mMouseSetToCursor = -1;

            // Is vehicle off ground
            if (Vehicle->field_20) {

                // And is current
                if (Vehicle != mSquad_CurrentVehicle)
                    return;

                if (Vehicle->field_8 == 0xA5)
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

    if (Dataa24->field_6E)
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
        Troop.mSprite->field_2 = 0;
        Troop.mSprite->field_6 = 0;
    }

    int16 Data10 = mSquad_Leader->field_0;
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
