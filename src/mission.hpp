class cMap;

class cMission {
private:
	size_t			 mMissionNumber, mMapNumber, mPhase;
	size_t			 mPhaseCount, mPhaseRemain;
	
	cMap			*mMap;

	void			 phaseNext();


public:
					 cMission();
					~cMission();
			
	bool			 mapLoad( bool pNext = true );

	cMap			*mapGet() { return mMap; }
};
