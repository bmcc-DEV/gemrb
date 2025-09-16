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

#include "XboxAudio.h"

#include "Xbox.h"

#include "Logging/Logging.h"

#include <cstring>

#ifdef _XBOX
	#include <xaudio.h>
	#include <xtl.h>
#endif

namespace GemRB {

bool XboxAudio::initialized = false;
int XboxAudio::audioQuality = 1; // Medium quality by default
int XboxAudio::maxSimultaneousSounds = 16; // Conservative for Xbox memory
size_t XboxAudio::audioMemoryUsed = 0;
size_t XboxAudio::maxAudioMemory = 8 * 1024 * 1024; // 8MB for audio

void XboxAudio::Initialize()
{
	if (initialized) return;

	Log(MESSAGE, "XboxAudio", "Initializing Xbox audio support");

#ifdef _XBOX
	// Initialize DirectSound for Xbox
	DirectSoundCreate(NULL, NULL, NULL);
#endif

	initialized = true;
}

void XboxAudio::Shutdown()
{
	if (!initialized) return;

	Log(MESSAGE, "XboxAudio", "Shutting down Xbox audio support");

	StopAudioStream();
	ClearUnusedAudioBuffers();

	initialized = false;
}

bool XboxAudio::SupportsXMA()
{
#ifdef _XBOX
	return true;
#else
	return false;
#endif
}

bool XboxAudio::SupportsADPCM()
{
	return true; // ADPCM is supported on all platforms
}

bool XboxAudio::DecodeXMA(const uint8_t* data, size_t dataSize, uint8_t** pcmData, size_t* pcmSize)
{
#ifdef _XBOX
	XMA2Decoder decoder;
	if (!decoder.Initialize(data, dataSize)) {
		return false;
	}

	// Estimate PCM size based on compression ratio
	size_t estimatedSize = dataSize * 4; // XMA typically compresses 4:1
	*pcmData = new uint8_t[estimatedSize];

	size_t totalDecoded = 0;
	size_t bufferPos = 0;

	while (bufferPos < estimatedSize) {
		size_t decodedSize;
		if (!decoder.Decode(*pcmData + bufferPos, estimatedSize - bufferPos, &decodedSize)) {
			break;
		}
		bufferPos += decodedSize;
		totalDecoded += decodedSize;
	}

	*pcmSize = totalDecoded;
	audioMemoryUsed += totalDecoded;

	Log(DEBUG, "XboxAudio", "Decoded XMA: %zu bytes -> %zu bytes PCM", dataSize, totalDecoded);
	return true;
#else
	Log(WARNING, "XboxAudio", "XMA decoding not available on non-Xbox platforms");
	return false;
#endif
}

bool XboxAudio::DecodeADPCM(const uint8_t* data, size_t dataSize, uint8_t** pcmData, size_t* pcmSize)
{
	ADPCMDecoder decoder;
	if (!decoder.Initialize(data, dataSize)) {
		return false;
	}

	// ADPCM typically expands 4:1
	size_t estimatedSize = dataSize * 4;
	*pcmData = new uint8_t[estimatedSize];

	size_t decodedSize;
	if (!decoder.Decode(*pcmData, estimatedSize, &decodedSize)) {
		delete[] *pcmData;
		return false;
	}

	*pcmSize = decodedSize;
	audioMemoryUsed += decodedSize;

	Log(DEBUG, "XboxAudio", "Decoded ADPCM: %zu bytes -> %zu bytes PCM", dataSize, decodedSize);
	return true;
}

void XboxAudio::OptimizeAudioBuffers()
{
	if (audioMemoryUsed > maxAudioMemory * 0.8f) {
		Log(MESSAGE, "XboxAudio", "Audio memory usage high (%zu/%zu), triggering cleanup",
		    audioMemoryUsed, maxAudioMemory);
		ClearUnusedAudioBuffers();
	}
}

void XboxAudio::ClearUnusedAudioBuffers()
{
	// This would be implemented to work with the audio backend
	// to free unused audio buffers
	Log(MESSAGE, "XboxAudio", "Clearing unused audio buffers");
	audioMemoryUsed = 0; // Reset counter after cleanup
}

size_t XboxAudio::GetAudioMemoryUsage()
{
	return audioMemoryUsed;
}

void XboxAudio::SetAudioQuality(int quality)
{
	audioQuality = quality;
	Log(MESSAGE, "XboxAudio", "Set audio quality to %d", quality);
}

void XboxAudio::SetMaxSimultaneousSounds(int maxSounds)
{
	maxSimultaneousSounds = maxSounds;
	Log(MESSAGE, "XboxAudio", "Set max simultaneous sounds to %d", maxSounds);
}

bool XboxAudio::StartAudioStream(const char* filename)
{
	Log(MESSAGE, "XboxAudio", "Starting audio stream: %s", filename);
	// Implementation would start streaming audio file
	return true;
}

void XboxAudio::StopAudioStream()
{
	Log(MESSAGE, "XboxAudio", "Stopping audio stream");
	// Implementation would stop current audio stream
}

bool XboxAudio::IsAudioStreaming()
{
	return false; // Placeholder
}

// XMA2Decoder implementation
XMA2Decoder::XMA2Decoder()
	: data(nullptr), dataSize(0), currentPos(0), sampleRate(44100), channels(2), bitsPerSample(16), initialized(false)
{
}

XMA2Decoder::~XMA2Decoder()
{
}

bool XMA2Decoder::Initialize(const uint8_t* xmaData, size_t dataSize)
{
	this->data = xmaData;
	this->dataSize = dataSize;
	this->currentPos = 0;

	// Parse XMA header to get format information
	// This is a simplified implementation
	if (dataSize < 16) return false;

	// For real Xbox implementation, would use XAudio2 XMA decoder
	initialized = true;
	return true;
}

bool XMA2Decoder::Decode(uint8_t* pcmBuffer, size_t bufferSize, size_t* decodedSize)
{
	if (!initialized) return false;

#ifdef _XBOX
	// Real Xbox implementation would use XAudio2 to decode XMA
	// This is a placeholder that simulates decoding
	size_t toDecode = std::min(bufferSize, dataSize - currentPos);
	if (toDecode == 0) {
		*decodedSize = 0;
		return false;
	}

	// Simulate XMA decoding - in real implementation this would be actual XMA->PCM
	memset(pcmBuffer, 0, toDecode);
	currentPos += toDecode / 4; // Simulate compression ratio
	*decodedSize = toDecode;
	return true;
#else
	*decodedSize = 0;
	return false;
#endif
}

void XMA2Decoder::Reset()
{
	currentPos = 0;
}

// ADPCM tables and implementation
const int ADPCMDecoder::indexTable[16] = {
	-1, -1, -1, -1, 2, 4, 6, 8,
	-1, -1, -1, -1, 2, 4, 6, 8
};

const int ADPCMDecoder::stepsizeTable[89] = {
	7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
	19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
	50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
	130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
	337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
	876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
	2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
	5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
	15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
};

ADPCMDecoder::ADPCMDecoder()
	: data(nullptr), dataSize(0), currentPos(0), sampleRate(22050), channels(1), initialized(false)
{
	state[0].predictor = 0;
	state[0].step_index = 0;
	state[1].predictor = 0;
	state[1].step_index = 0;
}

ADPCMDecoder::~ADPCMDecoder()
{
}

bool ADPCMDecoder::Initialize(const uint8_t* adpcmData, size_t dataSize)
{
	this->data = adpcmData;
	this->dataSize = dataSize;
	this->currentPos = 0;

	// Parse ADPCM header
	if (dataSize < 4) return false;

	initialized = true;
	return true;
}

bool ADPCMDecoder::Decode(uint8_t* pcmBuffer, size_t bufferSize, size_t* decodedSize)
{
	if (!initialized) return false;

	int16_t* output = reinterpret_cast<int16_t*>(pcmBuffer);
	size_t samplesDecoded = 0;
	size_t maxSamples = bufferSize / sizeof(int16_t);

	while (currentPos < dataSize && samplesDecoded < maxSamples) {
		uint8_t byte = data[currentPos++];

		// Decode two 4-bit samples per byte
		uint8_t nibble1 = byte & 0x0F;
		uint8_t nibble2 = (byte >> 4) & 0x0F;

		output[samplesDecoded++] = DecodeNibble(nibble1, &state[0]);
		if (samplesDecoded < maxSamples) {
			if (channels == 2) {
				output[samplesDecoded++] = DecodeNibble(nibble2, &state[1]);
			} else {
				output[samplesDecoded++] = DecodeNibble(nibble2, &state[0]);
			}
		}
	}

	*decodedSize = samplesDecoded * sizeof(int16_t);
	return samplesDecoded > 0;
}

void ADPCMDecoder::Reset()
{
	currentPos = 0;
	state[0].predictor = 0;
	state[0].step_index = 0;
	state[1].predictor = 0;
	state[1].step_index = 0;
}

int ADPCMDecoder::DecodeNibble(uint8_t nibble, ADPCMState* state)
{
	int step = stepsizeTable[state->step_index];
	int diff = step >> 3;

	if (nibble & 4) diff += step;
	if (nibble & 2) diff += step >> 1;
	if (nibble & 1) diff += step >> 2;

	if (nibble & 8) {
		state->predictor -= diff;
	} else {
		state->predictor += diff;
	}

	// Clamp to 16-bit range
	if (state->predictor > 32767)
		state->predictor = 32767;
	else if (state->predictor < -32768)
		state->predictor = -32768;

	state->step_index += indexTable[nibble];
	if (state->step_index < 0)
		state->step_index = 0;
	else if (state->step_index > 88)
		state->step_index = 88;

	return state->predictor;
}

} // namespace GemRB
