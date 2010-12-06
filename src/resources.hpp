struct cResource_File {
	string		 mName;
	dword		 mAddressStart;
	dword		 mSize;

	cResource_File( string pName, word pStart, word pEnd ) : 
		mName( pName ), mAddressStart( pStart ), mSize( pEnd ) { }

};

class cResources {
private:
	byte							*mData, *mDataCurrent;
	size_t							 mDataSize;

	vector< cResource_File >		 mFiles;
	
	byte							*mCurPtr;

	short int						 word_26DBE[0x1000];
	byte							 byte_27EE6[0x1A3C];
	static const byte				 byte_29921[0x100];
	static const byte				 byte_29A21[0xFF];

	byte							 byte_26DB0;

	short int						 mBytesRead, word_26DA8, word_26DBA, word_26DBC;
	word							 word_26DAA, word_26DB4, saveSI, saveBP;

	
	bool							 headerLoad();

	byte							*file_Get( cResource_File *pFile, size_t &pFileSize, bool pDecode );
	byte							 data_Read();

	void							 sub_26AA4();
	void							 sub_26B11();
	word							 sub_26C06();
	word							 sub_26CDF();

public:
	 								 cResources();
									~cResources();

	byte							*fileGet( string pFilename, size_t &pFileSize );

	vector< cResource_File >		*filesGet( ) { return &mFiles; }

};
