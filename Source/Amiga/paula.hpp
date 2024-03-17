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

#ifndef COMMON_FRAC_H
#define COMMON_FRAC_H

/**
 * The precision of the fractional (fixed point) type we define below.
 * Normally you should never have to modify this value.
 */
enum {
	FRAC_BITS = 16,
	FRAC_LO_MASK = ((1L << FRAC_BITS) - 1),
	FRAC_HI_MASK = ((1L << FRAC_BITS) - 1) << FRAC_BITS,

	FRAC_ONE = (1L << FRAC_BITS),		// 1.0
	FRAC_HALF = (1L << (FRAC_BITS-1))	// 0.5
};

/**
 * Fixed-point fractions, used by the sound rate converter and other code.
 */
typedef int32 frac_t;

inline frac_t doubleToFrac(double value) { return (frac_t)(value * FRAC_ONE); }
inline double fracToDouble(frac_t value) { return ((double)value) / FRAC_ONE; }

inline frac_t intToFrac(int16 value) { return value << FRAC_BITS; }
inline int16 fracToInt(frac_t value) { return value >> FRAC_BITS; }

#endif

#ifndef AUDIO_MODS_PAULA_H
#define AUDIO_MODS_PAULA_H

#include "Amiga/audiostream.hpp"

namespace Audio {

/**
 * Emulation of the "Paula" Amiga music chip
 * The interrupt frequency specifies the number of mixed wavesamples between
 * calls of the interrupt method
 */
class Paula : public AudioStream {
public:
	static const int NUM_VOICES = 4;
	// Default panning value for left channels.
	static const int PANNING_LEFT = 63;
	// Default panning value for right channels.
	static const int PANNING_RIGHT = 191;
	enum {
		kPalSystemClock  = 7093790,
		kNtscSystemClock = 7159090,
		kPalCiaClock     = kPalSystemClock / 10,
		kNtscCiaClock    = kNtscSystemClock / 10,
		kPalPaulaClock   = kPalSystemClock / 2,
		kNtscPaulaClock  = kNtscSystemClock / 2
	};

	enum FilterMode {
		kFilterModeNone = 0,
		kFilterModeA500,
		kFilterModeA1200,

#if defined(__DS__)
		kFilterModeDefault = kFilterModeNone
#else
		kFilterModeDefault = kFilterModeA1200
#endif
	};

	/* TODO: Document this */
	struct Offset {
		unsigned int	int_off;	// integral part of the offset
		frac_t	rem_off;	// fractional part of the offset, at least 0 and less than 1

		explicit Offset(int off = 0) : int_off(off), rem_off(0) {}
	};

	struct FilterState {
		FilterMode mode;
		bool ledFilter;

		float a0[3];
		float rc[NUM_VOICES][5];
	};

	uint8 volumeMax[NUM_VOICES];

	Paula(bool stereo = false, int rate = 44100, unsigned int interruptFreq = 0,
	      FilterMode filterMode = kFilterModeDefault, int periodScaleDivisor = 1);
	~Paula();

	bool playing() const { return _playing; }
	void setTimerBaseValue( uint32 ticksPerSecond ) { _timerBase = ticksPerSecond; }
	uint32 getTimerBaseValue() { return _timerBase; }
	void setSingleInterrupt(unsigned int sampleDelay) { assert(sampleDelay < _intFreq); _curInt = sampleDelay; }
	void setSingleInterruptUnscaled(unsigned int timerDelay) {
		setSingleInterrupt((unsigned int)(((double)timerDelay * getRate()) / _timerBase));
	}
	void setInterruptFreq(unsigned int sampleDelay) { _intFreq = sampleDelay; _curInt = 0; }
	void setInterruptFreqUnscaled(unsigned int timerDelay) {
		setInterruptFreq((unsigned int)(((double)timerDelay * getRate()) / _timerBase));
	}
	void clearVoice(uint8 voice);
	void clearVoices() { for (int i = 0; i < NUM_VOICES; ++i) clearVoice(i); }
	void startPlay() { filterResetState(); _playing = true; }
	void stopPlay() { _playing = false; }
	void pausePlay(bool pause) { _playing = !pause; }

// AudioStream API
	int readBuffer(int16 *buffer, const int numSamples);
	bool isStereo() const { return _stereo; }
	bool endOfData() const { return _end; }
	int getRate() const { return _rate; }

protected:
	struct Channel {
		const int8 *data;
		const int8 *dataRepeat;
		uint32 length;
		uint32 lengthRepeat;
		int16 period;
		uint8 volume;
		Offset offset;
		uint8 panning; // For stereo mixing: 0 = far left, 255 = far right
		int dmaCount;
		bool interrupt;
	};

	bool _end;

	virtual void interrupt() = 0;

	virtual void interruptChannel(uint8 channel) { }

	void startPaula() {
		_playing = true;
		_end = false;
	}

	void stopPaula() {
		_playing = false;
		_end = true;
	}

	void setChannelPanning(uint8 channel, uint8 panning) {
		assert(channel < NUM_VOICES);
		_voice[channel].panning = panning;
	}

	void disableChannel(uint8 channel) {
		assert(channel < NUM_VOICES);
		_voice[channel].data = 0;
	}

	void enableChannel(uint8 channel) {
		assert(channel < NUM_VOICES);
		Channel &ch = _voice[channel];
		ch.data = ch.dataRepeat;
		ch.length = ch.lengthRepeat;
		// actually first 2 uint8s are dropped?
		ch.offset = Offset(0);
		// ch.period = ch.periodRepeat;
	}

	void setChannelInterrupt(uint8 channel, bool enable) {
		assert(channel < NUM_VOICES);
		_voice[channel].interrupt = enable;
	}

	void setChannelPeriod(uint8 channel, int16 period) {
		assert(channel < NUM_VOICES);
		_voice[channel].period = period;
	}

	void setChannelVolume(uint8 channel, uint8 volume) {
		assert(channel < NUM_VOICES);
		_voice[channel].volume = volume;
	}

	void setChannelSampleStart(uint8 channel, const int8 *data) {
		assert(channel < NUM_VOICES);
		_voice[channel].dataRepeat = data;
	}

	void setChannelSampleLen(uint8 channel, uint32 length) {
		assert(channel < NUM_VOICES);
		assert(length < 32768/2);
		_voice[channel].lengthRepeat = 2 * length;
	}

	void setChannelData(uint8 channel, const int8 *data, const int8 *dataRepeat, uint32 length, uint32 lengthRepeat, int32 offset = 0) {
		assert(channel < NUM_VOICES);

		Channel &ch = _voice[channel];

		ch.dataRepeat = data;
		ch.lengthRepeat = length;
		enableChannel(channel);
		ch.offset = Offset(offset);

		ch.dataRepeat = dataRepeat;
		ch.lengthRepeat = lengthRepeat;
	}

	void setChannelOffset(uint8 channel, Offset offset) {
		assert(channel < NUM_VOICES);
		_voice[channel].offset = offset;
	}

	Offset getChannelOffset(uint8 channel) {
		assert(channel < NUM_VOICES);
		return _voice[channel].offset;
	}

	int getChannelDmaCount(uint8 channel) {
		assert(channel < NUM_VOICES);
		return _voice[channel].dmaCount;
	}

	void setChannelDmaCount(uint8 channel, int dmaVal = 0) {
		assert(channel < NUM_VOICES);
		_voice[channel].dmaCount = dmaVal;
	}

	void setAudioFilter(bool enable) {
		_filterState.ledFilter = enable;
	}

private:
	Channel _voice[NUM_VOICES];

	const bool _stereo;
	const int _rate;
	const double _periodScale;
	unsigned int _intFreq;
	unsigned int _curInt;
	uint32 _timerBase;
	bool _playing;

	FilterState _filterState;

	template<bool stereo>
	int readBufferIntern(int16 *buffer, const int numSamples);

	void filterResetState();
	float filterCalculateA0(int rate, int cutoff);
};

} // End of namespace Audio


#endif
