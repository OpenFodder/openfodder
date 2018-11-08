class cGraphics_Amiga2 : public cGraphics_Amiga {

public:
    virtual void		Load_pStuff();

    tSharedBuffer       GetPalette(const std::string pFilename);

    void Load_And_Draw_Image(const std::string &pFilename, unsigned int pColors, unsigned int pBackColor);
    virtual sImage		GetImage(const std::string& pFilename, const size_t pPaletteIndex);


    cGraphics_Amiga2();
    ~cGraphics_Amiga2();
};
