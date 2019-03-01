
struct sFodderParameters {
	bool mAppVeyor;             // Running on AppVeyor
	bool mShowAbout;            // Show the about screen
	bool mSkipIntro;            // Skip the OpenFodder intro, and the game intro
	bool mSkipRecruit;          // Skip the recruit screen and go straight into the mission
	bool mSkipBriefing;         // Skip mission briefing
	bool mSkipService;          // Skip mission debrief

	bool mWindowMode;           // Start in a window
	size_t mWindowScale;		// Start with window scaled at
	bool mRandom;               // Start a random map
	ePlatform mDefaultPlatform; // Default platform to use
	eGame mDefaultGame;			// Default game to use for single/random

	bool mDemoRecord;               // Recording a demo
	bool mDemoPlayback;             // Playing back a demo
	size_t mDemoRecordResumeCycle;  // Record a demo, after playing back the event queue up until this cycle
	std::string mDemoFile;          // The Demo file to save/load to/from

	int64 mSleepDelta;              // Engine sleep delta

	std::string mCampaignName;      // Campaign to start
	size_t mMissionNumber;          // Mission to start on
	size_t mPhaseNumber;            // Phase to start on

	bool mCheatsEnabled;
	bool mUnitTesting;              // Execute unit testing
	bool mSinglePhase;              // Play a single phase
	bool mPlayground;               // Launch into the sprite playground

	bool mDisableVideo;             // Disable all video output
	bool mDisableSound;             // Disable all sound

	size_t mSpritesMax;
	size_t mSpawnEnemyMax;

	sFodderParameters() {
		mShowAbout = false;
		mPlayground = false;
		mDisableSound = false;
		mDisableVideo = false;
		mSleepDelta = 2;

		mAppVeyor = false;
		mSkipService = false;
		mSkipBriefing = false;
		mSkipIntro = false;
		mSkipRecruit = false;

		mMissionNumber = 0;
		mPhaseNumber = 0;
		mWindowMode = false;
		mWindowScale = 0;
		mRandom = false;
		mDefaultPlatform = ePlatform::Any;
		mDefaultGame = eGame::CF1;

		mDemoRecord = false;
		mDemoPlayback = false;
		mDemoRecordResumeCycle = 0;

		mCheatsEnabled = false;
		mUnitTesting = false;
		mSinglePhase = false;
		mSpritesMax = 45;
		mSpawnEnemyMax = 10;
	}

	std::string ToJson();
	bool		FromJson(const std::string& pJson);
	bool		ProcessCLI(int argc, char *argv[]);
	bool		ProcessINI();
};