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
	enum {
		kPalSystemClock  = 7093790,
		kNtscSystemClock = 7159090,
		kPalCiaClock     = kPalSystemClock / 10,
		kNtscCiaClock    = kNtscSystemClock / 10,
		kPalPaulaClock   = kPalSystemClock / 2,
		kNtscPauleClock  = kNtscSystemClock / 2
	};

	/* TODO: Document this */
	struct Offset {
		size_t	int_off;	// integral part of the offset
		frac_t	rem_off;	// fractional part of the offset, at least 0 and less than 1

		explicit Offset(int off = 0) : int_off(off), rem_off(0) {}
	};

	Paula(bool stereo = false, int rate = 44100, size_t interruptFreq = 0);
	~Paula();

	bool playing() const { return _playing; }
	void setTimerBaseValue( uint32 ticksPerSecond ) { _timerBase = ticksPerSecond; }
	uint32 getTimerBaseValue() { return _timerBase; }
	void setSingleInterrupt(size_t sampleDelay) { assert(sampleDelay < _intFreq); _curInt = sampleDelay; }
	void setSingleInterruptUnscaled(size_t timerDelay) {
		setSingleInterrupt((size_t)(((double)timerDelay * getRate()) / _timerBase));
	}
	void setInterruptFreq(size_t sampleDelay) { _intFreq = sampleDelay; _curInt = 0; }
	void setInterruptFreqUnscaled(size_t timerDelay) {
		setInterruptFreq((size_t)(((double)timerDelay * getRate()) / _timerBase));
	}
	void clearVoice(uint8 voice);
	void clearVoices() { for (uint8 i = 0; i < NUM_VOICES; ++i) clearVoice(i); }
	void startPlay() { _playing = true; }
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
	};

	bool _end;

	virtual void interrupt() = 0;

	void startPaula() {
		_playing = true;
		_end = false;
	}

	void stopPaula() {
		_playing = false;
		_end = true;
	}

	void setChannelPanning(size_t channel, uint8 panning) {
		assert(channel < NUM_VOICES);
		_voice[channel].panning = panning;
	}

	void disableChannel(size_t channel) {
		assert(channel < NUM_VOICES);
		_voice[channel].data = 0;
	}

	void enableChannel(size_t channel) {
		assert(channel < NUM_VOICES);
		Channel &ch = _voice[channel];
		ch.data = ch.dataRepeat;
		ch.length = ch.lengthRepeat;
		// actually first 2 bytes are dropped?
		ch.offset = Offset(0);
		// ch.period = ch.periodRepeat;
	}

	void setChannelPeriod(size_t channel, int16 period) {
		assert(channel < NUM_VOICES);
		_voice[channel].period = period;
	}

	void setChannelVolume(size_t channel, uint8 volume) {
		assert(channel < NUM_VOICES);
		_voice[channel].volume = volume;
	}

	void setChannelSampleStart(size_t channel, const int8 *data) {
		assert(channel < NUM_VOICES);
		_voice[channel].dataRepeat = data;
	}

	void setChannelSampleLen(size_t channel, uint32 length) {
		assert(channel < NUM_VOICES);
		assert(length < 32768/2);
		_voice[channel].lengthRepeat = 2 * length;
	}

	void setChannelData(size_t channel, const int8 *data, const int8 *dataRepeat, uint32 length, uint32 lengthRepeat, int32 offset = 0) {
		assert(channel < NUM_VOICES);

		Channel &ch = _voice[channel];

		ch.dataRepeat = data;
		ch.lengthRepeat = length;
		enableChannel(channel);
		ch.offset = Offset(offset);

		ch.dataRepeat = dataRepeat;
		ch.lengthRepeat = lengthRepeat;
	}

	void setChannelOffset(size_t channel, Offset offset) {
		assert(channel < NUM_VOICES);
		_voice[channel].offset = offset;
	}

	Offset getChannelOffset(size_t channel) {
		assert(channel < NUM_VOICES);
		return _voice[channel].offset;
	}

	int getChannelDmaCount(size_t channel) {
		assert(channel < NUM_VOICES);
		return _voice[channel].dmaCount;
	}

	void setChannelDmaCount(size_t channel, int dmaVal = 0) {
		assert(channel < NUM_VOICES);
		_voice[channel].dmaCount = dmaVal;
	}

private:
	Channel _voice[NUM_VOICES];

	const bool _stereo;
	const int _rate;
	const double _periodScale;
	size_t _intFreq;
	size_t _curInt;
	uint32 _timerBase;
	bool _playing;

	template<bool stereo>
	int readBufferIntern(int16 *buffer, const int numSamples);
};

} // End of namespace Audio

#endif
