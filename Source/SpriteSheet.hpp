struct sSpriteSheet {
	uint16 mLoadOffset;
	uint16 mLoadSegment;
	int16 field_4;
	int16 field_6;
	int16 mColCount;
	int16 mRowCount;
	int16 mPalleteIndex;
	int8 mModX;
	int8 mModY;

	uint8* GetGraphicsPtr() const;
};

struct sSpriteSheet_pstuff {
	int16	mX;
	int16	mY;
	int16	mColumns;
	int16	mRows;

	uint8* GetGraphicsPtr(uint16 pOffset) const;
};