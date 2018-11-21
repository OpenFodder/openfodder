/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

/**
 * @file
 * Sound decoder used in engines:
 *  - queen
 */

#ifndef AUDIO_MODS_RJP1_H
#define AUDIO_MODS_RJP1_H

namespace Audio {
	
struct Rjp1Channel {
	const int8 *waveData;
	const int8 *modulatePeriodData;
	const int8 *modulateVolumeData;
	const int8 *envelopeData;
	uint16 volumeScale;
	int16 volume;
	uint16 modulatePeriodBase;
	uint32 modulatePeriodLimit;
	uint32 modulatePeriodIndex;
	uint16 modulateVolumeBase;
	uint32 modulateVolumeLimit;
	uint32 modulateVolumeIndex;
	uint8 freqStep;
	uint32 freqInc;
	uint32 freqInit;
	const uint8 *noteData;
	const uint8 *sequenceOffsets;
	const uint8 *sequenceData;
	uint8 loopSeqCount;
	uint8 loopSeqCur;
	uint8 loopSeq2Count;
	uint8 loopSeq2Cur;
	bool active;
	int16 modulatePeriodInit;
	int16 modulatePeriodNext;
	bool setupNewNote;
	int8 envelopeMode;
	int8 envelopeScale;
	int8 envelopeEnd1;
	int8 envelopeEnd2;
	int8 envelopeStart;
	int8 envelopeVolume;
	uint8 currentInstrument;
	const int8 *data;
	uint16 pos;
	uint16 len;
	uint16 repeatPos;
	uint16 repeatLen;
	bool isSfx;
};

class Rjp1 : public Paula {
public:

	struct Vars {
		int8 *instData;
		uint8 *songData[7];
		uint8 activeChannelsMask;
		uint8 currentChannel;
		int subsongsCount;
		int instrumentsCount;
	};

	Rjp1(int rate, bool stereo);
	virtual ~Rjp1();

	bool load(uint8 *songData, uint8 *instrumentsData, size_t instrumentDataSize);
	void unload();

	void startPattern(int ch, int pat);
	void startSong(int song);

protected:

    void startSequence(uint8 channelNum, uint8 seqNum);
	void turnOffChannel(Rjp1Channel *channel);
	void playChannel(Rjp1Channel *channel);
	void turnOnChannel(Rjp1Channel *channel);
	bool executeSfxSequenceOp(Rjp1Channel *channel, uint8 code, const uint8 *&p);
	bool executeSongSequenceOp(Rjp1Channel *channel, uint8 code, const uint8 *&p);
	void playSongSequence(Rjp1Channel *channel);
	void modulateVolume(Rjp1Channel *channel);
	void modulatePeriod(Rjp1Channel *channel);
	void setupNote(Rjp1Channel *channel, int16 freq);
	void setupInstrument(Rjp1Channel *channel, uint8 num);
	void setRelease(Rjp1Channel *channel);
	void modulateVolumeEnvelope(Rjp1Channel *channel);
	void setSustain(Rjp1Channel *channel);
	void setDecay(Rjp1Channel *channel);
	void modulateVolumeWaveform(Rjp1Channel *channel);
	void setVolume(Rjp1Channel *channel);

	void stopPaulaChannel(size_t channel);
	void setupPaulaChannel(size_t channel, const int8 *waveData, uint16 offset, uint16 len, uint16 repeatPos, uint16 repeatLen);

	virtual void interrupt();

	Vars _vars;
	Rjp1Channel _channelsTable[4];

	static const int16 _periodsTable[];
	static const int _periodsCount;
};
class AudioStream;

/*
 * Factory function for RichardJoseph1 modules. Reads all data from the
 * given songData and instrumentsData streams and creates an AudioStream
 * from this. No references to these stream objects are kept.
 */
Rjp1 *makeRjp1Stream(uint8 *songData, uint8 *instrumentsData, size_t instructmentsDataSize, int num, int rate = 44100, bool stereo = true);

} // End of namespace Audio

#endif
