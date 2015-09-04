class cGraphics_PC : public cGraphics {
	
	uint8*				mGraphicBlkPtrs[480];

	public:

	virtual void		graphicsBlkPtrsPrepare();
	virtual void		map_Tiles_Draw();
};
