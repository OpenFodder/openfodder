#include "stdafx.hpp"

cGraphics::cGraphics() {

	mFodder = g_Fodder.GetSingletonPtr();
	mImage = g_Fodder.mImage;
	mImageOriginal = mImage;

	mImageTemporary.mData->resize(100000);
}

void cGraphics::SetImage( cSurface* pImage ) {
	mImage = pImage;
}

void cGraphics::SetImageOriginal() {

	mImage = mImageOriginal;
}
