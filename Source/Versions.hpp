enum Platform {
	PC		= 0,
	Amiga	= 1
};

struct sVersion {
	const char* mName;
	Platform	mPlatform;
	const char* mDataPath;
};

const sVersion Versions[] = {
	{ "Cannon Fodder",									Platform::PC,		"" },
	{ "Cannon Fodder",									Platform::Amiga,	"Amiga" },
	{ "Cannon Fodder: Amiga Format Christmas Special",	Platform::Amiga,	"AmigaFormat_XMAS" }
};
