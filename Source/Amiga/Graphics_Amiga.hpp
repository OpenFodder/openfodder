class cGraphics_Amiga : public cGraphics_PC {

	uint8*				mBlkData;
	uint8*				mPalette;
	size_t				mPaletteSize;

	public:
						cGraphics_Amiga();

	virtual void		graphicsBlkPtrsPrepare();
	virtual void		map_Tiles_Draw();
};
