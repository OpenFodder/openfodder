/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2018 Open Fodder
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "stdafx.hpp"
#include <chrono>

cUnitTesting::cUnitTesting() {

    g_Fodder->mStartParams->mSinglePhase = true;
    g_Fodder->mStartParams->mSkipBriefing = true;
    g_Fodder->mStartParams->mSkipIntro = true;
    g_Fodder->mStartParams->mSkipService = true;
    g_Fodder->mStartParams->mSkipRecruit = true;

    if (!g_Fodder->mStartParams->mDemoRecord)
        g_Fodder->mStartParams->mDemoPlayback = true;
}

std::string cUnitTesting::getCurrentTestFileName() {
    std::string MissionPhase = "";
    MissionPhase += "m" + std::to_string(g_Fodder->mGame_Data.mMission_Number);
    MissionPhase += "p" + std::to_string(g_Fodder->mGame_Data.mMission_Phase);
    return MissionPhase;
}

std::string cUnitTesting::getCurrentTestName() {
    std::string MissionTitle = "Mission " + std::to_string(g_Fodder->mGame_Data.mMission_Number);
    MissionTitle += " Phase " + std::to_string(g_Fodder->mGame_Data.mMission_Phase);
    MissionTitle += ": " + g_Fodder->mGame_Data.mMission_Current->mName;
    if(g_Fodder->mGame_Data.mPhase_Current->mName != g_Fodder->mGame_Data.mMission_Current->mName)
        MissionTitle += " (" + g_Fodder->mGame_Data.mPhase_Current->mName + ")";
    return MissionTitle;

}
void cUnitTesting::EngineSetup() {
    g_Fodder->mIntroDone = false;

    g_Fodder->mSurface->palette_SetToBlack();
    g_Fodder->mSurface->paletteNew_SetToBlack();
    g_Fodder->mSurface->surfaceSetToPalette();
    g_Fodder->mSurface->resetPaletteAdjusting();

    g_Fodder->mPhase_TryingAgain = false;
    g_Fodder->Mouse_Setup();
}

void cUnitTesting::setDemoName() {
    // Set demo file name
    g_Fodder->mStartParams->mDemoFile = g_ResourceMan->GetTestPath(g_Fodder->mVersionCurrent, g_Fodder->mStartParams->mCampaignName) + "/" + getCurrentTestFileName();
    if (!g_ResourceMan->FileExists(g_Fodder->mStartParams->mDemoFile + ".ofd")) {

        bool AmigaExists = g_ResourceMan->FileExists(g_Fodder->mStartParams->mDemoFile + "-amiga.ofd");
        bool PcExists = g_ResourceMan->FileExists(g_Fodder->mStartParams->mDemoFile + "-pc.ofd");

        // If we a PC or Amiga specific test exists, then use it as the name
        if (AmigaExists || PcExists) {

            if (g_Fodder->mVersionCurrent->isAmiga()) {
                g_Fodder->mStartParams->mDemoFile += "-amiga";
            } else {
                g_Fodder->mStartParams->mDemoFile += "-pc";
            }
        }
    }

    g_Fodder->mStartParams->mDemoFile += ".ofd";
}

bool cUnitTesting::RunTests(const std::string pCampaign) {
    bool Retry = false;
    g_Fodder->Game_Setup();

	if (g_Fodder->mStartParams->mUnitTesting && g_Fodder->mStartParams->mDemoPlayback) {
		g_Fodder->mStartParams->mSleepDelta = 0;
		g_Fodder->mParams->mSleepDelta = 0;
	}

    while (g_Fodder->mGame_Data.mMission_Current) {
        EngineSetup();
        setDemoName();

        std::string MissionTitle = getCurrentTestName();

        if (g_Fodder->mStartParams->mDemoRecord && !Retry) {
            if (g_ResourceMan->FileExists(g_Fodder->mStartParams->mDemoFile)) {
                g_Debugger->Notice("Test exists for " + MissionTitle + ", skipping");
                g_Fodder->mGame_Data.Phase_Next();
                continue;
            }
        }

        Retry = false;
        g_Debugger->TestStart(MissionTitle, g_Fodder->mStartParams->mCampaignName);

        if (g_Fodder->mStartParams->mDemoPlayback) {
            if (!g_Fodder->mStartParams->mDemoRecord) {
                if (!g_Fodder->Demo_Load()) {
                    g_Debugger->TestComplete(MissionTitle, g_Fodder->mStartParams->mCampaignName, "No test found", 0, eTest_Skipped);
                    g_Fodder->mGame_Data.Phase_Next();
                    continue;
                }
            }

            g_Fodder->mGame_Data.mDemoRecorded.playback();
        }

        // If demo is recording
        if (g_Fodder->mStartParams->mDemoRecord) {
            // And we have a resume cycle set
            if (g_Fodder->mStartParams->mDemoRecordResumeCycle) {
                g_Debugger->Notice("Resuming " + MissionTitle);
                g_Fodder->mParams->mSleepDelta = 0;
            }
            else {
                g_Fodder->mGame_Data.mDemoRecorded.clear();
                g_Debugger->Notice("Recording " + MissionTitle);
            }
        }

        // Reset demo status
		//g_Fodder->mParams->mSleepDelta = g_Fodder->mStartParams->mSleepDelta;
		g_Fodder->mParams->mDemoFile = g_Fodder->mStartParams->mDemoFile;
        g_Fodder->mParams->mDemoRecord = g_Fodder->mStartParams->mDemoRecord;
        g_Fodder->mParams->mDemoPlayback = g_Fodder->mStartParams->mDemoPlayback;
        g_Fodder->mParams->mAppVeyor = g_Fodder->mStartParams->mAppVeyor;
        // Keep game state
        g_Fodder->mGame_Data_Backup = g_Fodder->mGame_Data;

        // Run the phase
        auto missionStartTime = std::chrono::steady_clock::now();
        g_Fodder->Mission_Loop();
        auto missionDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - missionStartTime);

        // If recording
        if (g_Fodder->mStartParams->mDemoRecord) {
            if (g_Fodder->mPhase_Complete) {
                g_Fodder->mGame_Data.mDemoRecorded.save();
            } else {

                Retry = true;
                // If the phase was aborted (ESC key), don't replay it.. start over
                if (!g_Fodder->mPhase_EscapeKeyAbort) {

                    // Less than 80 cycles, player can start over
                    if (g_Fodder->mGame_Data.mDemoRecorded.mTick > 80) {
                        g_Fodder->mStartParams->mDemoRecordResumeCycle = (g_Fodder->mGame_Data.mDemoRecorded.mTick - 80);
                        g_Fodder->mStartParams->mDemoPlayback = true;
                        
                        // Delete events occruing after resume cycle
                        g_Fodder->mGame_Data.mDemoRecorded.removeFrom(g_Fodder->mStartParams->mDemoRecordResumeCycle);
                        g_Fodder->mGame_Data_Backup.mDemoRecorded = g_Fodder->mGame_Data.mDemoRecorded;
                        // Disable video output for speed
                        g_Fodder->mStartParams->mDisableVideo = true;
                        g_Fodder->mStartParams->mDisableSound = true;
                    }
                }
                // Reset game to before start of phase
                g_Fodder->mGame_Data = g_Fodder->mGame_Data_Backup;
                continue;
            }
        }

        if (!g_Fodder->mPhase_Complete) {
            g_Debugger->TestComplete(MissionTitle, g_Fodder->mStartParams->mCampaignName, "Phase Failed", (size_t)missionDuration.count(), eTest_Failed);
            return false;
        }

        g_Debugger->TestComplete(MissionTitle, g_Fodder->mStartParams->mCampaignName, "Phase Complete", (size_t)missionDuration.count(), eTest_Passed);

        g_Fodder->mGame_Data = g_Fodder->mGame_Data_Backup;
        g_Fodder->mGame_Data.Phase_Next();
    }

    return true;
}

bool cUnitTesting::Start() {
    std::vector<std::string> Campaigns;

    // No Campaign Name
    if (!g_Fodder->mStartParams->mCampaignName.size())
        Campaigns = g_Fodder->mVersions->GetCampaignNames();
    else
        Campaigns.push_back(g_Fodder->mStartParams->mCampaignName);

    bool TotalResult = true;
    for (auto& CampaignName : Campaigns) {
        bool Result = true;

        if (CampaignName == "Random Map" || CampaignName == "Single Map")
            continue;

        *g_Fodder->mParams = *g_Fodder->mStartParams;
        g_Fodder->mStartParams->mCampaignName = CampaignName;

        g_Debugger->TestStart(CampaignName, "Campaign");

        g_Fodder->mGame_Data.mCampaign.Clear();
        if (!g_Fodder->Campaign_Load(CampaignName)) {
            g_Debugger->Error("Campaign " + CampaignName + " not found");
            g_Debugger->TestComplete(CampaignName, "Campaign", "not found", 0, eTest_Skipped);
            continue;
        }

        // FIXME: Create test folder
        if (!g_ResourceMan->FileExists(g_ResourceMan->GetTestPath(g_Fodder->mVersionCurrent, "")) && g_Fodder->mStartParams->mDemoRecord) {

            std::string Command = "mkdir \"" + g_ResourceMan->GetTestPath(g_Fodder->mVersionCurrent, "") + "\"";
            system(Command.c_str());
        }

        auto campaignStartTime = std::chrono::steady_clock::now();
        if (!RunTests(CampaignName))
            Result = false;
        auto campaignDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - campaignStartTime);

        g_Debugger->TestComplete(CampaignName, "Campaign", "", (size_t)campaignDuration.count(), Result ? eTest_Passed : eTest_Failed);

        if (!Result)
            TotalResult = false;
    }

    return TotalResult;
}
