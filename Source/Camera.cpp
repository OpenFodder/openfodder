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

const int32 CAMERA_PAN_TO_START_ACCELARATION = 0x80000; // Dos: Original 0x80000
const int32 CAMERA_PAN_TO_SQUAD_ACCELERATION = 0x20000; // Dos: Original 0x20000
const int32 CAMERA_TOWARD_SQUAD_SPEED = 0x14;           // Dos: Original 0x14

int16 cFodder::getCameraWidth() const
{

    return ((getWindowWidth() - SIDEBAR_WIDTH));
}

int16 cFodder::getCameraHeight() const
{
    if (!mParams->mWindowRows)
    {
        if (mVersionCurrent)
            return mVersionCurrent->GetScreenSize().mHeight;

        return 364;
    }

    return (int16)mParams->mWindowRows * 16;
}

void cFodder::Camera_Handle()
{

    if (mPhase_In_Progress)
    {
        Camera_Speed_Reset();
        Camera_Speed_MaxSet();
        Camera_Speed_Calculate();
        Camera_TileSpeedX_Set();

        MapTile_UpdateFromCamera();
        MapTile_Update_Position();

        Camera_Update_Mouse_Position_For_Pan();

        Camera_UpdateSpeedFromPanTarget();

        // Mouse_DrawCursor();
        Camera_UpdateSpeedFromPanTarget();

        Camera_PanTarget_AdjustToward_SquadLeader();
        Camera_Acceleration_Set();
    }
}

void cFodder::Camera_PanTarget_AdjustToward_SquadLeader()
{
    if (mSquad_Leader == INVALID_SPRITE_PTR || mSquad_Leader == 0)
        return;

    int16 SquadLeaderX = mSquad_Leader->mPosX + 0x18;
    int16 SquadLeaderY = mSquad_Leader->mPosY;
    int16 Data18 = SquadLeaderX;
    int16 Data1C = SquadLeaderY;

    mCamera_Scroll_Speed = CAMERA_TOWARD_SQUAD_SPEED;

    // Mouse near sidebar?
    if (mMouseX <= 0x0F)
    {

        if (mCamera_Panning_ToTarget)
        {

            if ((SquadLeaderX - mCamera_SquadLeaderX) > 0 ||
                (SquadLeaderY - mCamera_SquadLeaderY) > 0)
            {
                mCamera_PanTargetX = SquadLeaderX;
                mCamera_PanTargetY = SquadLeaderY;
            }

            // Original Logic
            // mCamera_PanTargetX += (SquadLeaderX - mCamera_SquadLeaderX);
            // mCamera_PanTargetY += (SquadLeaderY - mCamera_SquadLeaderY);

            mCamera_SquadLeaderX = SquadLeaderX;
            mCamera_SquadLeaderY = SquadLeaderY;
        }
        else
        {
            // loc_10A11
            mCamera_Panning_ToTarget = true;

            mCamera_PanTargetX = (mCameraX >> 16) + (getCameraWidth() / 2) - 8;
            mCamera_PanTargetY = (mCameraY >> 16) + (getCameraHeight() - 8) / 2;
        }
        // loc_10A3A
        mMouse_Locked = false;
        return;
    }

    mCamera_Panning_ToTarget = false;
    mCamera_SquadLeaderX = SquadLeaderX;
    mCamera_SquadLeaderY = SquadLeaderY;
    int16 Data0 = mMouseX + (mCameraX >> 16);
    int16 Data4 = mMouseY + (mCameraY >> 16);

    int16 Data0_Saved = Data0;
    int16 Data4_Saved = Data4;
    int16 Data8_Saved = SquadLeaderX;
    int16 DataC_Saved = SquadLeaderY;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, SquadLeaderX, SquadLeaderY);

    if (mSquad_CurrentVehicle)
    {
        if (Data0 >= 0x64)
            Data0 = 0x64;
    }
    else
    {
        if (Data0 >= 0x8C)
            Data0 = 0x8C;
    }
    // loc_10AAA
    int16 word_3ABFD = Data0;

    SquadLeaderX = Data8_Saved;
    SquadLeaderY = DataC_Saved;
    Data4 = Data4_Saved; // MouseY
    Data0 = Data0_Saved; // MouseX

    if (Direction_Between_Points(Data0, Data4, SquadLeaderX, SquadLeaderY) < 0)
        return;

    int32 DirectionX = mDirectionVectorTable[Data4 / 2];
    Data4 += 0x80;
    Data4 &= 0X1FE;

    int32 DirectionY = mDirectionVectorTable[Data4 / 2];

    DirectionX = (DirectionX * word_3ABFD) >> 16;
    DirectionY = (DirectionY * word_3ABFD) >> 16;

    Data18 += DirectionX;
    Data1C += DirectionY;
    Data18 -= 0x18;

    if (Data18 < 0)
        Data18 = 0;

    mCamera_PanTargetX = Data18;
    mCamera_PanTargetY = Data1C;
    mMouse_Locked = false;
}

void cFodder::Camera_UpdateSpeedFromPanTarget()
{
    int16 Data4 = mCamera_PanTargetY - (getCameraHeight() - 8) / 2;
    if (Data4 < 0)
        Data4 = 0;

    int16 Data0 = mCamera_PanTargetX - (getCameraWidth() / 2) + 8;
    if (Data0 < 0)
        Data0 = 0;

    int16 Data8 = mCameraX >> 16;
    int16 DataC = mCameraY >> 16;

    int16 PanTargetX = Data0;
    int16 PanTargetY = Data4;
    int16 CameraX = Data8;
    int16 CameraY = DataC;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 > 0x10)
    {
        Data4 = mCamera_Scroll_Speed;

        if (Data4 <= (dword_39F36 >> 16))
        {
            dword_39F36 = (Data4 << 16) | (dword_39F36 & 0xFFFF);
        }
        else
        {
            dword_39F36 += 0x8000;
        }
    }
    else
    {
        dword_39F36 = Data0 << 16;
    }

    DataC = CameraY;
    Data8 = CameraX;
    Data4 = PanTargetY;
    Data0 = PanTargetX;

    if (Direction_Between_Points_LUT(Data0, Data4, Data8, DataC) >= 0)
    {

        Data0 &= 0x1FE;
        if (!dword_39F36)
            return;
        Data4 = Data0;

        Data8 = mDirectionVectorTable[Data4 / 2];
        Data4 += 0x80;
        Data4 &= 0x1FE;

        DataC = mDirectionVectorTable[Data4 / 2];
        Data8 >>= 2;
        DataC >>= 2;

        // seg000:197D
        int32 Dataa8 = (int32)Data8;
        int32 DataaC = (int32)DataC;

        Data4 = (dword_39F36 >> 16);
        Dataa8 *= Data4;
        DataaC *= Data4;

        mCamera_Speed_X = Dataa8;
        mCamera_Speed_Y = DataaC;
    }
    else
    {

        // loc_119C6
        dword_39F36 -= 0x8000;
        if (dword_39F36 < 0)
        {
            dword_39F36 = 0;
            mMouse_Locked = true;
        }
    }
}

void cFodder::Camera_Speed_Calculate()
{

    mCamera_Speed_X += mCamera_AccelerationX;
    mCamera_Speed_Y += mCamera_AccelerationY;

    int32 Data0 = 1;

    if (mCamera_Speed_X < 0)
        Data0 = -1;

    if (Data0 != mCamera_MoveDirectionX)
    {
        mCamera_MoveDirectionX = Data0;
        mCamera_MovePauseX = 9;
    }

    if (mCamera_MovePauseX)
        mCamera_Speed_X = 0;

    Data0 = 1;
    if (mCamera_Speed_Y < 0)
        Data0 = -1;

    if (Data0 != mCamera_MoveDirectionY)
    {
        mCamera_MoveDirectionY = Data0;
        mCamera_MovePauseY = 9;
    }

    if (mCamera_MovePauseY)
        mCamera_Speed_Y = 0;

    // loc_11B9C
    //  Calculate maximum right position of camera
    Data0 = mMapLoaded->getWidthPixels() - getCameraWidth();
    Data0 = (Data0 << 16) | (Data0 >> 16);

    int32 Data4 = mCameraX + mCamera_Speed_X;
    if (Data4 > Data0)
    {
        Data0 -= mCameraX;
        mCamera_Speed_X = Data0;
    }
    // loc_11BE8
    Data0 = mMapLoaded->getHeightPixels() - getCameraHeight();
    Data0 = (Data0 << 16) | (Data0 >> 16);

    Data4 = mCameraY + mCamera_Speed_Y;

    if (Data4 > Data0)
    {
        Data0 -= mCameraY;
        mCamera_Speed_Y = Data0;
    }

    Data0 = mCamera_Speed_X;
    if (Data0 < 0)
        Data0 = -Data0;

    if (Data0 > mCamera_Speed_Max)
    {
        Data0 = mCamera_Speed_Max;

        if ((mCamera_Speed_X >> 16) < 0)
            Data0 = -Data0;

        mCamera_Speed_X = Data0;
    }
    // loc_11C6D
    Data0 = mCamera_Speed_Y;
    if (Data0 < 0)
        Data0 = -Data0;

    if (Data0 <= mCamera_Speed_Max)
        return;

    Data0 = mCamera_Speed_Max;
    if ((mCamera_Speed_Y >> 16) < 0)
        Data0 = -Data0;

    mCamera_Speed_Y = Data0;
}

void cFodder::Camera_Prepare()
{

    mCamera_TileX = mCameraX >> 16;
    mCamera_TileY = mCameraY >> 16;
    mCamera_Speed_Reset_X = false;
    mCamera_Speed_Reset_Y = false;
    mCamera_AccelerationX &= 0x0000FFFF;
    mCamera_AccelerationY &= 0x0000FFFF;
}

void cFodder::Camera_SetTargetToStartPosition()
{
    int16 Data0 = mCameraX >> 16;
    int16 Data4 = mCameraY >> 16;

    int16 Data8 = mCamera_StartPosition_X;
    Data8 -= (getCameraWidth() / 2) - 8;
    if (Data8 < 0)
        Data8 = 0;

    int16 Data10 = mMapLoaded->getWidthPixels();
    Data10 -= (getCameraWidth() / 2) - 8;
    if (Data8 >= Data10)
        Data8 = Data10;

    int16 DataC = mCamera_StartPosition_Y;
    DataC -= (getCameraHeight() - 8) / 2;
    if (DataC < 0)
        DataC = 0;

    Data10 = mMapLoaded->getHeightPixels();
    Data10 -= (getCameraHeight() - 8) / 2;
    if (DataC >= Data10)
        DataC = Data10;

    if (!mPhase_MapIs17x12)
    {
        Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);

        if (Data0 >= 0x8C)
            goto loc_11D8A;
    }
    // loc_11D62
    mCamera_PanTargetX = mCamera_StartPosition_X;
    mCamera_PanTargetY = mCamera_StartPosition_Y;

    return;

loc_11D8A:;

    if (mPhase_Completed_Timer)
        return;

    mCamera_MovePauseX = 6;
    mCamera_MovePauseY = 6;
    mInput_Enabled = false;

    mCamera_PanTargetX = mCamera_StartPosition_X;
    mCamera_PanTargetY = mCamera_StartPosition_Y;
    mCamera_Speed_Reset_X = false;
    mCamera_AccelerationX &= 0x0000FFFF;
    mCamera_Speed_Reset_Y = false;
    mCamera_AccelerationY &= 0x0000FFFF;

    Image_FadeOut();

    if (mMap_Destroy_Tiles_Countdown)
        Map_Destroy_Tiles();

    // THis count is really meant as a backup to stop it looping forever
    for (int count = 0; count < 10000000; ++count)
    {
        Camera_Pan_To_Target();
        Camera_Pan_To_Target();

        if (!mCamera_Reached_Target)
            break;
    }

    if (mCamera_Reached_Target)
    {
        g_Debugger->Notice("Camera didnt reach target");
    }
    Mission_Sprites_Handle();
    mGraphics->Sidebar_Copy_To_Surface();
    Camera_Prepare();
    Video_Sleep();

    mInput_Enabled = true;

    mGraphics->PaletteSet();
}

void cFodder::Camera_Pan_To_Target()
{
    Camera_Pan_ComputeSpeed();

    int32 Saved_dword_3A9FD = mCamera_Speed_Max;
    mCamera_Speed_Max = 0x100000;
    Camera_Speed_Calculate();
    mCamera_Speed_Max = Saved_dword_3A9FD;

    Camera_TileSpeedX_Set();

    MapTile_UpdateFromCamera();
    MapTile_Update_Position();
}

void cFodder::Camera_Pan_ComputeSpeed()
{
    mCamera_Reached_Target = 2;
    mCamera_Speed_X = 0;
    mCamera_Speed_Y = 0;

    int16 Data0 = mCamera_PanTargetX;
    Data0 -= getCameraWidth() / 2;
    if (Data0 < 0)
        Data0 = 0;

    Data0 >>= 4;
    int16 Data4 = mCamera_PanTargetY;
    Data4 -= (getCameraHeight() - 8) / 2;
    if (Data4 < 0)
        Data4 = 0;

    Data4 >>= 4;

    int16 Data8 = mMapLoaded->getWidth();
    Data8 -= (getCameraWidth() >> 4) + 1;

    if (Data8 < 0)
        Data8 = 0;

    if (Data0 >= Data8)
        Data0 = Data8;

    Data8 = mMapLoaded->getHeight();
    Data8 -= (getCameraHeight() + 32) >> 4;
    if (Data8 < 0)
        Data8 = 0;

    if (Data4 >= Data8)
        Data4 = Data8;

    Data8 = mCameraX >> 16;
    Data8 >>= 4;
    int16 DataC = mCameraY >> 16;
    DataC >>= 4;

    if (Data8 == Data0)
    {
        --mCamera_Reached_Target;
    }
    else if (Data8 > Data0)
    {
        mCamera_Speed_X = (mCamera_Speed_X & 0xFFFF) | -CAMERA_PAN_TO_START_ACCELARATION; // (-8 << 16);
    }
    else
        mCamera_Speed_X = (mCamera_Speed_X & 0xFFFF) | CAMERA_PAN_TO_START_ACCELARATION; // (8 << 16);

    // loc_11FAC
    if (DataC == Data4)
    {
        --mCamera_Reached_Target;
    }
    else if (DataC > Data4)
    {
        mCamera_Speed_Y = (mCamera_Speed_Y & 0xFFFF) | -CAMERA_PAN_TO_START_ACCELARATION; // (-8 << 16);
    }
    else
        mCamera_Speed_Y = (mCamera_Speed_Y & 0xFFFF) | CAMERA_PAN_TO_START_ACCELARATION; // (8 << 16);
}

void cFodder::Camera_Update_Mouse_Position_For_Pan()
{

    if (!mMouse_Locked)
    {

        // Mouse in playfield?
        if (mMouseX > 0x0F)
        {
            mMouseX -= (mCameraX >> 16) - mCamera_TileX;
            mMouseY -= (mCameraY >> 16) - mCamera_TileY;
        }
    }

    // loc_12007
    mCamera_TileX = (mCameraX >> 16);
    mCamera_TileY = (mCameraY >> 16);
}

void cFodder::Camera_Reset()
{

    mCamera_TileSpeedX = 0;
    mCamera_TileSpeedY = 0;
    mCameraX = 0;
    mCameraY = 0;
    dword_39F36 &= 0x0000FFFF;
    mCamera_AccelerationX &= 0x0000FFFF;
    mCamera_AccelerationY &= 0x0000FFFF;
    mCamera_Speed_Reset_X = false;
    mCamera_Speed_Reset_Y = false;
    mCamera_TileX = 0;
    mCamera_TileY = 0;
    mCamera_Speed_X = 0;
    mCamera_Speed_Y = 0;
    mCamera_TileSpeed_Overflow = 0;
}

void cFodder::Camera_TileSpeedX_Set()
{
    mCamera_TileSpeed_Overflow = 0;

    int32 dword_39F5A = mCameraX;

    mCameraX += mCamera_Speed_X;
    if (mCameraX < 0)
    {
        mCamera_Speed_X = dword_39F5A;
        if (mCamera_Speed_X)
            mCamera_Speed_X = -mCamera_Speed_X;

        mCameraX = 0;
    }
    // loc_12147
    mCamera_TileSpeedX += mCamera_Speed_X;

    if ((mCamera_Speed_X >> 16) < 0)
    {

        if ((mCamera_TileSpeedX >> 16) < 0)
        {

            mCamera_TileSpeedX += (getWindowWidth() << 16);
            mCamera_TileSpeed_Overflow = (0xFFFF << 16) | (mCamera_TileSpeed_Overflow & 0xFFFF);
        }
    }
    else
    {
        // loc_12181
        if ((mCamera_TileSpeedX >> 16) >= getWindowWidth())
        {
            mCamera_TileSpeedX -= (getWindowWidth() << 16);
            mCamera_TileSpeed_Overflow = (1 << 16) | (mCamera_TileSpeed_Overflow & 0xFFFF);
        }
    }
    // loc_1219F
    Camera_TileSpeedY_Set();
    mCamera_Speed_X = 0;
    mCamera_Speed_Y = 0;
}

void cFodder::Camera_TileSpeedY_Set()
{

    int32 dword_39F5A = mCameraY;

    mCameraY += mCamera_Speed_Y;
    if (mCameraY < 0)
    {
        mCamera_Speed_Y = dword_39F5A;

        if (mCamera_Speed_Y)
            mCamera_Speed_Y = -mCamera_Speed_Y;

        mCameraY = 0;
    }

    // loc_121F2

    mCamera_TileSpeedY += (mCamera_Speed_Y + mCamera_TileSpeed_Overflow);
    mCamera_TileSpeedY &= (0x00FF << 16) | 0xFFFF;
}

void cFodder::Camera_Speed_MaxSet()
{

    mCamera_Speed_Max = 0x20000;
    if (!mPhase_Aborted && !mPhase_TryAgain && !mPhase_Complete)
        return;

    int16 Data0 = 0;

    mCamera_AccelerationX = (mCamera_AccelerationX & 0xFFFF) + (Data0 << 16);
    mCamera_AccelerationY = (mCamera_AccelerationY & 0xFFFF) + (Data0 << 16);
    mCamera_Speed_X = Data0;
    mCamera_Speed_Y = Data0;
    mCamera_Speed_Max = 0;
}

void cFodder::Camera_Speed_Reset()
{

    if (mCamera_Speed_Reset_X)
        mCamera_Speed_X = 0;

    if (mCamera_Speed_Reset_Y)
        mCamera_Speed_Y = 0;
}

void cFodder::Camera_Acceleration_Set()
{

    if (mSquad_Leader == INVALID_SPRITE_PTR || mSquad_Leader == 0)
        return;

    int16 DistanceX = mSquad_Leader->mPosX + 0x40;
    int16 DistanceY = mSquad_Leader->mPosY + 0x10;

    DistanceX -= mCameraX >> 16;
    DistanceY -= mCameraY >> 16;

    if (mCamera_Speed_Reset_X && DistanceX > 0x90 && DistanceX < 0xCF)
    { // 90 / A2 = Amiga
        mCamera_Speed_Reset_X = false;
        mCamera_AccelerationX = 0;
    }

    if (mCamera_Speed_Reset_Y && DistanceY > 0x60 && DistanceY < 0x87)
    { // 64 / 8F = Amiga
        mCamera_Speed_Reset_Y = false;
        mCamera_AccelerationY = 0;
    }
    // loc_1234F
    if (DistanceX <= 0x40)
    {
        mCamera_AccelerationX = (mCamera_AccelerationX & 0xFFFF) | -CAMERA_PAN_TO_SQUAD_ACCELERATION; // (-2 << 16);
        mCamera_Speed_Reset_X = true;
    }
    // loc_12362
    if (DistanceX >= getCameraWidth() + 64)
    {
        mCamera_AccelerationX = (mCamera_AccelerationX & 0xFFFF) | CAMERA_PAN_TO_SQUAD_ACCELERATION; // (2 << 16);
        mCamera_Speed_Reset_X = true;
    }

    if (DistanceY <= 0x10)
    {
        mCamera_AccelerationY = (mCamera_AccelerationY & 0xFFFF) | -CAMERA_PAN_TO_SQUAD_ACCELERATION; // (-2 << 16);
        mCamera_Speed_Reset_Y = true;
    }

    if (DistanceY >= (getCameraHeight() - 9))
    {
        mCamera_AccelerationY = (mCamera_AccelerationY & 0xFFFF) | CAMERA_PAN_TO_SQUAD_ACCELERATION; // (2 << 16);
        mCamera_Speed_Reset_Y = true;
    }
}

int16 cFodder::Direction_Between_Points_LUT(int16 &pX1, int16 &pY1, int16 pX2, int16 pY2)
{

    int16 Data10 = 0, Data14 = 0;

    pX1 -= pX2;
    pY1 -= pY2;

    if (pX1 < 0)
        Data10 = 1;

    Data10 = -Data10;
    if (pX1 < 0)
        pX1 = -pX1;

    if (pY1 < 0)
        Data14 = 1;

    Data14 = -Data14;
    if (pY1 < 0)
        pY1 = -pY1;

    int16 count = 0;
    for (count = 0x0E; count >= 0; --count)
    {
        if (pX1 & (1 << count))
            break;
        if (pY1 & (1 << count))
            break;
    }

    count -= 4;
    if (count >= 0)
    {
        pX1 >>= count;
        pY1 >>= count;
    }
    pY1 <<= 5;
    pY1 |= pX1;

    pY1 = mMap_DirectionsBetweenPoints[pY1];
    if (pY1 < 0)
        return -1;

    pY1 <<= 1;
    if (Data10 < 0)
    {
        // loc_11AD2
        if (Data14 >= 0)
        {
            pX1 = 0x180;
            pX1 += pY1;
            pY1 = 0;
            return 0;
        }
        // loc_11AEF
        pX1 = 0x180;
        pX1 -= pY1;
        pY1 = 0;
        return 0;
    }

    if (Data14 < 0)
    {
        // loc_11ABC
        pX1 = 0x80;
        pX1 += pY1;
        pY1 = 0;
        return 0;
    }

    pX1 = 0x80;
    pX1 -= pY1;
    pY1 = 0;

    return 0;
}
