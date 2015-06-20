#/*
 *  Cannon Fodder
 *  ------------------------
 *
 *  Copyright (C) 2008-2015 Robert Crossfield
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

const char* mBinTable[] = { "rjnull.bin", 
							"rjnull.bin", 
							"player.bin", 
							"player.bin", 
							"player.bin", 
							"player.bin" };

const uint16 mMissionPhaseTable[] = {	01, 02, 01, 04, 03, 02, 03, 04,
										02, 05, 03, 06, 01, 03, 03, 02,
										01, 05, 01, 04, 01, 04, 05, 06 };

const sIntroString mIntro_1[] = {
	{ 0xCC, "A Sensible Software Game" },
	0
};

const sIntroString mIntro_2[] = {
	{ 0x64, "STARRING" },
	0
};

const sIntroString mIntro_3[] = {
	{ 0xA4, "JOOLS" },
	{ 0xB5, "as" },
	{ 0xC8, "James Dean" },
	0
};

const sIntroString mIntro_4[] = {
	{ 0xA4, "STOO" },
	{ 0xB4, "as" },
	{ 0xC8, "Elvis" },
	0
};

const sIntroString mIntro_5[] = {
	{ 0xA4, "RICHARD JOSEPH" },
	{ 0xB4, "as" },
	{ 0xC8, "Sgt. Bilko" },
	0
};

const sIntroString mIntro_6[] = {
	{ 0x64, "Co. STARRING" },
	0
};

const sIntroString mIntro_7[] = {
	{ 0xA4, "CHRIS YATES" },
	{ 0xB4, "as" },
	{ 0xC8, "JR out of Dallas" },
	0
};

const sIntroString mIntro_8[] = {
	{ 0xA4, "JON HARE" },
	{ 0xB5, "as" },
	{ 0xC8, "Vera Lynn" },
	0
};

const sIntroString mIntro_9[] = {
	{ 0x64, "SPECIAL GUEST STAR" },
	0
};

const sIntroString mIntro_10[] = {
	{ 0xA4, "CHRIS CHAPMAN" },
	{ 0xB4, "as" },
	{ 0xC8, "Norman Wisdom" },
	0
};

const sIntroString mIntro_11[] = {
	{ 0x50, "Jools" },
	{ 0x8C, "Stoo" },
	{ 0x3C, "Program Design" },
	{ 0x78, "Graphic Design" },
	0
};

const sIntroString mIntro_12[] = {
	{ 0x32, "Richard Joseph" },
	{ 0x6E, "Jon Hare" },
	{ 0xAA, "David Hindley" },
	{ 0x1E, "Sound effects and Music" },
	{ 0x5A, "Original Soundtrack"},
	{ 0x96, "Cameraman" },
	0
};

const sIntroString mIntro_13[] = {
	{ 0x28, "Steve Cook" },
	{ 0x5A, "Graftgold" },
	{ 0x8C, "Hammonds Hire" },
	{ 0xBE, "Peter Hickman" },
	{ 0x19, "Vehicle and Stunts" },
	{ 0x46, "Flatbed Scanning" },
	{ 0x78, "Costumes" },
	{ 0xAA, "Mr Virgin" },
	0
};

const sIntroString mIntro_14[] = {
	{ 0x19, "An Audio Visual Magic" },
	{ 0x2D, "Entertainment Conversion" },
	{ 0x4B, "PC Programming" },
	{ 0x5F, "Adrian Youlden" },
	{ 0x7D, "PC Graphics" },
	{ 0x91, "Mark Smith" },
	{ 0xAF, "PC Sound and Music" },
	{ 0xC3, "Nigel Taylor" },
	0
};

const sIntroString mIntro_15[] = {
	{ 0x19, "PC Version Produced" },
	{ 0x2D, "by" },
	{ 0x4B, "Gavid Wade" },
	{ 0x5F, "and" },
	{ 0x73, "Scott Walsh" },
	0
};

const sIntroString mIntro_16[] = {
	{ 0xD2, "Sensible Software." },
	{ 0xBE, "Designed By" },
	0
};

const sIntroText mIntroText[] = {
	{ 0x31, mIntro_1 },
	{ 0xFF, mIntro_2 },
	{ 0x32, mIntro_3 },
	{ 0x33, mIntro_4 },
	{ 0x34, mIntro_5 },
	{ 0xFF, mIntro_6 },
	{ 0x36, mIntro_7 },
	{ 0x35, mIntro_8 },
	{ 0xFF, mIntro_9 },
	{ 0x37, mIntro_10 },
	{ 0xFF, mIntro_11 },
	{ 0xFF, mIntro_12 },
	{ 0xFF, mIntro_13 },
	{ 0xFF, mIntro_14 },
	{ 0xFF, mIntro_15 },
	{ 0x38, mIntro_16 },
	0
};

cFodder::cFodder() {

	mResources = new cResources();
	mWindow = new cWindow();

	mTicksDiff = 0;
	mTicks = 0;

	mIntroDone = 0;
	mErrorCode = 0;
	word_427D4 = -1;

	mMouseButtons = 0;

	mouse_Button_Status = 0;
	mouse_Pos_Column = 0;
	mouse_Pos_Row = 0;

	mButtonPressLeft = mButtonPressRight = 0;
	word_39EF8 = word_39F02 = 0;
	word_39EF6 = word_39EFA = 0;
	word_39EFC = word_39F04 = 0;
	word_3AA43 = 0;
	word_3A9B2 = 0;

	mouseData0 = new sMouseData();
	mouseData1 = new sMouseData();

	mDataPStuff = 0;

	for (unsigned int x = 0; x < 30; ++x)
		dword_3A3FD[x] = 0;

	for (unsigned int x = 0; x < 30; ++x)
		dword_3A475[x] = 0;

	for (unsigned int x = 0; x < 30; ++x)
		dword_3A4ED[x] = 0;

	for (unsigned int x = 0; x < 30; ++x)
		dword_3A565[x] = 0;

	for (unsigned int x = 0; x < 30; ++x)
		dword_3A5DD[x] = 0;

	for (unsigned int x = 0; x < 30; ++x)
		dword_3A655[x] = 0;

	for (unsigned int x = 0; x < 30; ++x)
		dword_3A6CD[x] = 0;
	 
	for (unsigned int x = 0; x < 30; ++x)
		dword_3A745[x] = 0;

	for (unsigned int x = 0; x < 30; ++x)
		dword_3A7BD[x] = 0;

	for (unsigned int x = 0; x < 30; ++x)
		dword_3A835[x] = 0;
}

cFodder::~cFodder() {
	
}

void cFodder::sub_10BBC() {
	word_390A4 = 0;
	word_390A6 = 0;
	word_390AE = 0;
	word_390B8 = 0;
	mMapNumber = 0;
	mMissionNumber = 0;
	mMissionPhase = 0;
	word_39096 = 0;
	word_390CE = 0;
	word_390D0 = 0;
	word_390D2 = 0;
	word_390D4 = 0;
	word_390D6 = 0;
	word_390E8 = 0;
	word_390EA = 0;
	mMissionPhaseRemain = 0;
	mMissionPhases = 0;

	for (unsigned int x = 0; x < 8; ++x) {
		stru_390FA[x].field_0 = 0;
		stru_390FA[x].field_1 = 0;
		stru_390FA[x].field_2 = 0;
		stru_390FA[x].field_3 = 0;
		stru_390FA[x].field_4 = 0;
		stru_390FA[x].field_5 = 0;
		stru_390FA[x].field_6 = 0;
	}

	word_3915A = 0;
	word_3915E = 0;

	for (unsigned int x = 0; x < 361; ++x)
		word_391D2[x] = 0;

	dword_394A4 = 0;
	dword_394A8 = 0;

	for (unsigned int x = 0; x < 361; ++x)
		word_394AC[x] = 0;

	dword_3977E = 0;

	for (unsigned int x = 0; x < 25; ++x)
		byte_3978E[x] = 0;
}

void cFodder::sub_10B6D() {
	word_3915E = -1;
	word_3915A = -1;

	for (unsigned int x = 0; x < 8; ++x) {
		stru_390FA[x].field_3 = -1;
		stru_390FA[x].field_0 = -1;
		stru_390FA[x].field_1 = 0;
		stru_390FA[x].field_2 = 0;
	}
}

void cFodder::sub_10BFA() {
	
	for (unsigned int x = 0; x < 25; ++x) {
		byte_3978E[x] = -1;
	}
}

void cFodder::sub_10D61() {
	// Clear memory 2454 to 3B58
	mButtonPressLeft = 0;
	mButtonPressRight = 0;
	word_39EF8 = 0;
	word_39EFA = 0;
	word_39EFC = 0;
	word_39F02 = 0;
	word_39F04 = 0;
	//word_39F06 = 0;
	word_3B2CD = 0;
	word_3B2CF = 0;
	word_3B447 = 0;
	word_3B4F3 = 0;
}

void cFodder::sub_10D9F() {
	word_3BDE7 = 1;
	word_3BDE9 = 1;
	word_3BDEB = 5;
	// -1
	off_3BDEF[0] = dword_3BE03;
	off_3BDEF[1] = dword_3BE27;
	off_3BDEF[2] = dword_3BE4B;
	off_3BDEF[3] = dword_3BE6F;
	off_3BDEF[4] = dword_3BE93;
	
	for (unsigned int x = 0; x < 9; ++x) {
		dword_3BE03[x] = -1;
	}
	for (unsigned int x = 0; x < 9; ++x) {
		dword_3BE27[x] = -1;
	}
	for (unsigned int x = 0; x < 9; ++x) {
		dword_3BE4B[x] = -1;
	}
	for (unsigned int x = 0; x < 9; ++x) {
		dword_3BE6F[x] = -1;
	}
	for (unsigned int x = 0; x < 9; ++x) {
		dword_3BE93[x] = -1;
	}
	word_3BEB9 = 1;
	word_3BEBB = 1;
	word_3BEBD = 1;
	word_3BEC1 = 0x1A;
	word_3BEC3 = 0x12;
	word_3BEC9 = 0xC8;
	word_3BED5[0] = 2;
	word_3BED5[1] = 2;
	word_3BED5[2] = 2;
	word_3BED5[3] = 2;
	word_3BED5[4] = 2;
	word_3BEDF[0] = 1;
	word_3BEDF[1] = 1;
	word_3BEDF[2] = 1;
	word_3BEDF[3] = 1;
	word_3BEDF[4] = 1;
	word_3BEDF[5] = 0;
	word_3BEDF[6] = 0;
	word_3BEDF[7] = 0;
	word_3BEDF[8] = 0;
	word_3BEDF[9] = 0;

	off_3BEF3[0] = dword_3A3FD;
	off_3BEF3[1] = dword_3A475;
	off_3BEF3[2] = dword_3A4ED;
	off_3BEF3[3] = dword_3A565;
	off_3BEF3[4] = dword_3A5DD;
	off_3BEF3[5] = dword_3A655;
	off_3BEF3[6] = dword_3A6CD;
	off_3BEF3[7] = dword_3A745;
	off_3BEF3[8] = dword_3A7BD;
	off_3BEF3[9] = dword_3A835;

	word_3BF1B = -1;
	word_3BF1E[0] = 0;
	word_3BF1E[1] = 0;
	word_3BF1E[2] = 0;
	word_3BF1E[3] = 0;
	word_3BF1E[4] = 0;
	word_3BF1E[5] = 0;
}

void cFodder::sub_10DEC() {
	word_39FD4 = 0;
	word_3A054 = 0;
	dword_3A39D = 0;
	word_39FD0 = 0;
	word_3A9AA = 0;
	word_3A9AC = 0;
	dword_39F7C = 0;
	word_3A9F7 = 0x23;
	dword_3A05A = 0;
	byte_3A05E = 0;
	dword_3A8DB = 0;
	byte_3A8DF = 0;
	word_3A06B = -1;
	word_3A28D = 0x0C;
	dword_3A3FD[0] = -1;
	dword_3A475[0] = -1;
	dword_3A4ED[0] = -1;
	dword_3A565[0] = -1;
	dword_3A5DD[0] = -1;
	dword_3A655[0] = -1;
	dword_3A6CD[0] = -1;
	dword_3A745[0] = -1;
	dword_3A7BD[0] = -1;
	dword_3A835[0] = -1;
}

void cFodder::sub_12AB1() {
	
	word_3E75B = -1;
}

void cFodder::Load_PlayerBin() {
	
}

bool cFodder::EventAdd( cEvent pEvent ) {
	
	mEvents.push_back( pEvent );

	return true;
}

void cFodder::Load_File( const std::string& pFilename ) {
	size_t FileSize = 0;

	uint8* Buffer = local_FileRead( pFilename, "", FileSize );

	word_3E7ED = readLEWord( Buffer );
	mEffectDriver = readLEWord( Buffer + 2 );
	mMusicDriver = readLEWord( Buffer + 4 );

	delete Buffer;
}

void cFodder::memory_XMS_Detect() {

	word_427D4 = 1;
	mErrorCode = 0;
}

void cFodder::eventProcess() {

	for (std::vector<cEvent>::iterator EventIT = mEvents.begin(); EventIT != mEvents.end(); ++EventIT) {

		switch (EventIT->mType) {
			case eEvent_MouseLeftDown:
				mMousePosition = EventIT->mPosition;
				mMouseButtons |= 1;
				break;

			case eEvent_MouseRightDown:
				mMousePosition = EventIT->mPosition;
				mMouseButtons |= 2;
				break;

			case eEvent_MouseLeftUp:
				mMousePosition = EventIT->mPosition;
				mMouseButtons &= ~1;
				break;

			case eEvent_MouseRightUp:
				mMousePosition = EventIT->mPosition;
				mMouseButtons &= ~2;
				break;

			case eEvent_MouseMove:
				mMousePosition = EventIT->mPosition;
				break;

			case eEvent_Quit:

				break;
		}

	}

	mEvents.clear();
}

void cFodder::mouse_Setup() {
	
	word_3BDAF = 0x7F;
	word_3BDB1 = 0x67;
	word_3BDB3 = 0;
	word_3BDB5 = 0;

	mouseData1->anonymous_0 = 0;
	mouseData1->anonymous_1 = 0;
	mouseData1->anonymous_2 = 2;
	mouseData1->mColumn = 160;
	mouseData1->mRow = 100;
	mouseData1->anonymous_5 = 0x10;
	mouseData1->anonymous_6 = 0x10;
	mouseData1->anonymous_7 = 0x10;
	mouseData1->anonymous_8 = 0x10;
	mouseData1->anonymous_9 = 0x30;
	mouseData1->anonymous_10 = 0xC8;

	mouseData0->anonymous_0 = 0;
	mouseData0->anonymous_1 = 0;
	mouseData0->anonymous_2 = 2;
	mouseData0->mColumn = 160;
	mouseData0->mRow = 100;
	mouseData0->anonymous_5 = 0x10;
	mouseData0->anonymous_6 = 0x10;
	mouseData0->anonymous_7 = 0x10;
	mouseData0->anonymous_8 = 0x10;
	mouseData0->anonymous_9 = 0x30;
	mouseData0->anonymous_10 = 0xC8;

	g_Window.SetMousePosition( cPosition( 160, 100 ) );
}

void cFodder::mouse_GetData() {

	mouse_Pos_Column = mMousePosition.mX;
	mouse_Pos_Row = mMousePosition.mY;
	mouse_Button_Status = mMouseButtons;
}

void cFodder::mouse_Handle() {
	
	mouse_GetData();
	mouse_ButtonCheck();

	g_Window.SetMousePosition( cPosition( 160, 100 ) );

	int16 dword_37AA4 = mouse_Pos_Column - 0x70;
	word_3BDB3 = dword_37AA4;

	dword_37AA4 += word_3BDAF;

	if ((word_3A024 == 0 && dword_37AA4 < -32 ) || dword_37AA4 < -16) {
		//loc_13B41
		if (word_3A024)
			dword_37AA4 = -16;
		else
			dword_37AA4 = -32;
	} else {
		//loc_13B58
		if (dword_37AA4 > 287)
			dword_37AA4 = 287;
	}

	//loc_13B66
	word_3BDAF = dword_37AA4;
	dword_37AA0 = mouse_Pos_Row - 88;

	word_3BDB5 = dword_37AA0;
	dword_37AA0 += word_3BDB1;

	if (dword_37AA0 < 4)
		dword_37AA0 = 4;
	else {

		if (dword_37AA0 > 203)
			dword_37AA0 = 203;
	}

	word_3BDB1 = dword_37AA0;
}

void cFodder::mouse_ButtonCheck() {

	mButtonPressLeft = 0;
	if (mouse_Button_Status & 1) {
		mButtonPressLeft -= 1;
		if (word_39EF8 == 0) {
			word_39EF8 = -1;
			word_39F02 = -1;

			if (word_39EF6) {
				word_39EFC = -1;
				word_3AA43 = -1;
			}
		}

	} else {
		word_3AA43 = 0;
		word_39EF8 = 0;
	}

	mButtonPressRight = 0;
	if (mouse_Button_Status & 2) {
		mButtonPressRight -= 1;
		if (word_39EFA == 0) {
			word_39F04 = -1;
			word_39EFA = -1;
		}
	} else {
		word_39EFA = 0;
	}

}

void cFodder::Prepare() {

	mWindow->InitWindow( "Open Fodder" );

	Load_File("setup.dat");
	sub_12AB1();
	//Resource_Data_Open();
	//Load_EffectDriver();
	//Load_MusicDriver();
	memory_XMS_Detect();
}

void cFodder::sub_18C45( cSurface* pImage, const sIntroString* pString ) {

}

int16 cFodder::introPlayText() {
	cSurface* CurrentImage = 0;

	//video_Draw_unk_0();
	//setSpriteDataPtrToBase();
	word_3B2CF = 0;

	//loc_165F2
	for (;;) {

		if (mIntroText[word_3B2CF].mImageNumber == 0)
			break;

		word_3B447 = 0x288;

		if (mIntroText[word_3B2CF].mImageNumber != -1) {
			//17A7
			std::stringstream ImageName;
			size_t ImageSize = 0;
			ImageName << mIntroText[word_3B2CF].mImageNumber;
			ImageName << ".dat";

			delete CurrentImage;
			CurrentImage = g_Resource.image4PlaneLoad( ImageName.str(), 0x100 );
			//g_Window.RenderAt( Image, cPosition() );
			//g_Window.FrameEnd();

		}
		else {
			//loc_166A0

			word_3B447 = 0xAF;
			
		}

		//loc_166BE
		const sIntroString* IntroString = mIntroText[word_3B2CF].mText;
		while (IntroString->mPosition) {

			sub_18C45( CurrentImage, IntroString );
			++IntroString;
		}
		//loc_16710
		
		int16 Duration = word_3B447;
		int16 Fade = -1;
		bool DoBreak = false;

		while( Fade == -1 || DoBreak == false  ) {
			g_Window.EventCheck();

			--Duration;

			if (Duration) {
				if (Fade)
					Fade = CurrentImage->paletteFade();

				mouse_GetData();
				if (mouse_Button_Status) {
					word_3B2CF = 15;
					word_3B4F3 = -1;
					CurrentImage->paletteFadeOut();
					Fade = -1;
					DoBreak = true;
				}
			}
			else {
				CurrentImage->paletteFadeOut();
				Fade = -1;
				DoBreak = true;
			}

			g_Window.RenderAt( CurrentImage, cPosition() );
			g_Window.FrameEnd();
			//videoSleep();
		}

		//seg003:18D2
		++word_3B2CF;
	}

	delete CurrentImage;
	return 0;
}

void cFodder::intro() {

	//copyprotection();
	word_42851 = 0;
	//video_Draw_unk_0();

	word_3B4F3 = 0;
	//video_Font_Load();
	//intro_Music_Play();
	if (ShowImage_ForDuration( "cftitle.dat", 0x1F8 ))
		goto introDone;

	if (introPlayText())
		goto introDone;

	if (ShowImage_ForDuration( "virgpres.dat", 0x2D0 ))
		goto introDone;
	
	if (ShowImage_ForDuration( "sensprod.dat", 0x2D0 ))
		goto introDone;
	
	word_42851 = -1;
	if (ShowImage_ForDuration( "cftitle.dat", 0x318 ))
		goto introDone;

introDone:;
	mIntroDone = -1;
	//sub_1645F();

}

int16 cFodder::ShowImage_ForDuration( const std::string& pFilename, uint16 pDuration ) {
	bool DoBreak = false;
	int16 Fade = -1;
	cSurface* img = g_Resource.image4PlaneLoad( pFilename, 0x100 );

	while( Fade == -1 || DoBreak == false  ) {
		g_Window.EventCheck();

		--pDuration;

		if (pDuration) {
			if (Fade)
				Fade = img->paletteFade();

			mouse_GetData();
			if (mouse_Button_Status) {
				word_3B4F3 = -1;
				img->paletteFadeOut();
				Fade = -1;
				DoBreak = true;
			}
		}
		else {
			img->paletteFadeOut();
			Fade = -1;
			DoBreak = true;
		}

		g_Window.RenderAt( img, cPosition() );
		g_Window.FrameEnd();
		//videoSleep();
	}

	delete img;
	return word_3B4F3;
}

void cFodder::mission_PhaseNext() {
	mMissionPhase += 1;
	mMissionPhaseRemain -= 1;
	if (mMissionPhaseRemain)
		return;

	for (unsigned int x = 0; x < 8; ++x) {
		stru_390FA[x].field_2 = 0;
	}

	word_390D0 = mMissionNumber;
	word_390D2 = mMapNumber;
	--word_390D2;

	mMissionPhaseRemain = mMissionPhases = mMissionPhaseTable[mMissionNumber];
	mMissionPhase = 0;
	++mMissionNumber;
	word_390CE += 0x0F;

	word_390E8 = word_390CE;
	word_390E8 -= 0x0F;
	word_390EA = -1;
	word_390D4 = 0;

	dword_3977E = word_394AC;
	word_394AC[0] = -1;
	dword_394A8 = dword_394A4;
}

void cFodder::videoSleep() {
	static uint64_t delta = 1;

	mTicksDiff = GetTickCount() - mTicksDiff;

	mTicks = mTicksDiff * 60 / 1000;

	sleepLoop(delta * 1000 / 60 - mTicksDiff);

	mTicksDiff = GetTickCount();
}

void cFodder::sleepLoop( int64 pMilliseconds ) {

	uint64 TimeStarted = GetTickCount();
	uint64 TimeFinish = TimeStarted + pMilliseconds;

	do {
		//_system->updateScreen();
		mWindow->EventCheck();

		if (GetTickCount() >= TimeFinish)
			break;

		_sleep( 10 );

	} while (1);

}

void cFodder::WonGame() {
	//seg003:3E6D
	cSurface* img = g_Resource.image4PlaneLoad( "won.dat", 0x100 );

	//cSurface* img = g_Resource.imageLoad( "junarmy.dat", 0x40 );
	//cSurface* img = g_Resource.image4PlaneLoad( "junarmy.dat", 0x10 );
	//img->paletteLoad( WonDat + 0xD200, 0x10, 0xF0 );

	while(img->paletteFade()) {

		g_Window.RenderAt( img, cPosition() );
		g_Window.FrameEnd();
	}

	while (1) {
		g_Window.EventCheck();
	}
}

void cFodder::Start() {

	mouse_Setup();
	mouse_Handle();

	for (;;) {
		sub_10BBC();

		word_3A9AC = 0;
		mMapNumber = 0;
		word_3901E = 0x3333;

		sub_10B6D();
		sub_10BFA();

		dword_394A4 = word_391D2;
		word_391D2[0] = -1;

		mMissionPhaseRemain = 1;
		mMissionNumber = 0;

		mission_PhaseNext();

		word_3A9AA = -1;
		word_39096 = -1;

		size_t StuffSize = 0;
		delete mDataPStuff;
		mDataPStuff = g_Resource.fileGet( "pstuff.dat", StuffSize );

		for (unsigned int x = 0; x < 0x30; ++x)
			byte_3DDA2[x] = mDataPStuff[0xA000 + x];

		//loc_1042E
		for (;;) {
			//video_?_1();

			if (!word_3A9B2 && !word_3A9AA) {

				if (mMapNumber == 71)
					WonGame();

				++mMapNumber;
				mission_PhaseNext();
				word_3901E = 0x3333;
			}

			// loc_1045F
			sub_10D61();
			sub_10D9F();
			sub_10DEC();
			//video_?_1();
			word_390A6 = 0;

			if (!mIntroDone)
				intro();

			if (dword_3901A != 0x12345678)
				dword_3901A = 0x12345678;

			//loc_10496
		}
	}
}

void cFodder::Exit( unsigned int pExitCode ) {

}
