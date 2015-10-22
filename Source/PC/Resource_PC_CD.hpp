
class cResource_PC_CD : public cResources {
	
	uint8*							mData, *mDataCurrent;

	size_t							mDataSize;

	std::vector< cResource_File >	mFiles;
	
	uint8*							mCurPtr;
	
	int16							word_26DBE[0x273];
	int16							word_272A4[0x13A];
	int16							word_27518[0x273];
	int16							word_279FE[0x273];
	int16							word_27EE4;
	uint8							byte_27EE6[0x1A3B];
	static const uint8				byte_29921[0x100];
	static const uint8				byte_29A21[0x101];
	
	uint8							data_Read();

	void							sub_26AA4();
	void							sub_26B11();
	uint16							sub_26C06();
	uint16							sub_26CDF();
	
	uint8							byte_26DB0;

	int16							mBytesRead, word_26DA8;
	int32							 word_26DBA;
	uint16							word_26DAA, word_26DB4, saveSI, saveBP;

	bool							headerLoad();
	uint8*							file_Get( cResource_File *pFile, size_t &pFileSize, bool pDecode );


public:
	
									cResource_PC_CD( std::string pDataPath = "" );
	virtual							~cResource_PC_CD();

	virtual uint8*					fileGet( std::string pFilename, size_t &pFileSize );

	std::vector< cResource_File >*	filesGet( ) { return &mFiles; }
		
};
