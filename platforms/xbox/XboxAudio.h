/* GemRB - Infinity Engine Emulator
 * Copyright (C) 2024 The GemRB Project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef XBOX_AUDIO_H
#define XBOX_AUDIO_H

#include "exports.h"
#include "ie_types.h"

#include "Audio/AudioBackend.h"

namespace GemRB {

class GEM_EXPORT XboxAudio {
public:
	static void Initialize();
	static void Shutdown();

	// Xbox-specific audio format support
	static bool SupportsXMA();
	static bool SupportsADPCM();

	// XMA decoding
	static bool DecodeXMA(const uint8_t* data, size_t dataSize, uint8_t** pcmData, size_t* pcmSize);

	// ADPCM decoding
	static bool DecodeADPCM(const uint8_t* data, size_t dataSize, uint8_t** pcmData, size_t* pcmSize);

	// Audio buffer management for 64MB memory constraint
	static void OptimizeAudioBuffers();
	static void ClearUnusedAudioBuffers();
	static size_t GetAudioMemoryUsage();

	// Audio quality settings for Xbox hardware
	static void SetAudioQuality(int quality); // 0=low, 1=medium, 2=high
	static void SetMaxSimultaneousSounds(int maxSounds);

	// Audio streaming for large files
	static bool StartAudioStream(const char* filename);
	static void StopAudioStream();
	static bool IsAudioStreaming();

private:
	static bool initialized;
	static int audioQuality;
	static int maxSimultaneousSounds;
	static size_t audioMemoryUsed;
	static size_t maxAudioMemory;
};

// XMA2 decoder implementation for Xbox
class GEM_EXPORT XMA2Decoder {
public:
	XMA2Decoder();
	~XMA2Decoder();

	bool Initialize(const uint8_t* xmaData, size_t dataSize);
	bool Decode(uint8_t* pcmBuffer, size_t bufferSize, size_t* decodedSize);
	void Reset();

	size_t GetSampleRate() const { return sampleRate; }
	size_t GetChannels() const { return channels; }
	size_t GetBitsPerSample() const { return bitsPerSample; }

private:
	const uint8_t* data;
	size_t dataSize;
	size_t currentPos;
	size_t sampleRate;
	size_t channels;
	size_t bitsPerSample;
	bool initialized;
};

// ADPCM decoder for Xbox
class GEM_EXPORT ADPCMDecoder {
public:
	ADPCMDecoder();
	~ADPCMDecoder();

	bool Initialize(const uint8_t* adpcmData, size_t dataSize);
	bool Decode(uint8_t* pcmBuffer, size_t bufferSize, size_t* decodedSize);
	void Reset();

	size_t GetSampleRate() const { return sampleRate; }
	size_t GetChannels() const { return channels; }

private:
	struct ADPCMState {
		int predictor;
		int step_index;
	};

	const uint8_t* data;
	size_t dataSize;
	size_t currentPos;
	size_t sampleRate;
	size_t channels;
	ADPCMState state[2]; // For stereo
	bool initialized;

	static const int indexTable[16];
	static const int stepsizeTable[89];

	int DecodeNibble(uint8_t nibble, ADPCMState* state);
};

} // namespace GemRB

#endif // XBOX_AUDIO_H
