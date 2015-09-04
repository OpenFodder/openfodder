class cGraphics : public cSingleton<cGraphics> {
	
public:

	virtual void		graphicsBlkPtrsPrepare() = 0;
	virtual void		map_Tiles_Draw() = 0;
	
};
