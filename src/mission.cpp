#include "stdafx.h"
#include "fodder.hpp"
#include "mission.hpp"
#include "map.hpp"

cMission::cMission() {

	mMissionNumber = 0;
	mMapNumber = 1;
	mPhase = 0;
	mPhaseCount = 1;
	mPhaseRemain = 1;

	mMap = 0;
}

cMission::~cMission() {

	delete mMap;
}

void cMission::phaseNext() {
	++mPhase;
	--mPhaseRemain;

	// TODO: Work here
	//seg000:0C2D
	if(mPhaseRemain == 0) {

		mPhaseRemain = 1;
		mPhaseCount = 1;

		mPhase = 0;
		++mMissionNumber;
	}
}

bool cMission::mapLoad( bool pNext ) {

	if(pNext) {
		++mMapNumber;
		phaseNext();

		delete mMap;
	}

	mMap = new cMap( mMapNumber );
	return mMap->load();
}
