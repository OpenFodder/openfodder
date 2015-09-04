class cGraphics : public cSingleton<cGraphics> {
	
protected:
	cSurface*			mImage;

public:
						cGraphics();

	virtual void		graphicsBlkPtrsPrepare() = 0;
	virtual void		map_Tiles_Draw() = 0;
	virtual void		PaletteSet() = 0;
};
