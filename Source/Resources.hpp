class cSurface;

struct cResource_File {
	std::string		 mName;
	uint32		 mAddressStart;
	uint32		 mSize;

	cResource_File( std::string pName, uint32 pStart, uint32 pEnd ) : 
		mName( pName ), mAddressStart( pStart ), mSize( pEnd ) { }

};

class cResources : public cSingleton<cResources> {
private:
	uint8							*mData, *mDataCurrent;
	uint8							*mExeData;

	size_t							 mDataSize, mExeDataSize;

	std::vector< cResource_File >	 mFiles;
	
	uint8							*mCurPtr;

	short int						 word_26DBE[0x1000];
	uint8							 byte_27EE6[0x1A3C];
	static const uint8				 byte_29921[0x100];
	static const uint8				 byte_29A21[0xFF];

	uint8							 byte_26DB0;

	short int						 mBytesRead, word_26DA8, word_26DBA, word_26DBC;
	uint16							 word_26DAA, word_26DB4, saveSI, saveBP;

	bool							 headerLoad();

	uint8							*file_Get( cResource_File *pFile, size_t &pFileSize, bool pDecode );
	uint8							 data_Read();

	void							 sub_26AA4();
	void							 sub_26B11();
	uint16							 sub_26C06();
	uint16							 sub_26CDF();

public:
	 								 cResources();
									~cResources();

	uint8							*fileGet( std::string pFilename, size_t &pFileSize );

	std::vector< cResource_File >	*filesGet( ) { return &mFiles; }

	cSurface						*imageLoad( std::string pFilename, size_t pColors );
	cSurface						*image4PlaneLoad( const std::string &pFilename, size_t pColors );
	cSurface						*spriteLoad( std::string pFilename, size_t pSpriteIndex );

	uint8							*spriteDataGet( size_t pIndex );

};
