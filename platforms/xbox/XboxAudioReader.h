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

#ifndef XBOX_AUDIO_READER_H
#define XBOX_AUDIO_READER_H

#include "PluginMgr.h"
#include "SoundMgr.h"

namespace GemRB {

class XMAAudioReader : public PluginBase {
public:
	XMAAudioReader() = default;
	bool Open(DataStream* stream) override;
	int read_samples(short* buffer, int count) override;

private:
	DataStream* str = nullptr;
	int channels = 0;
	int samplerate = 0;
	bool initialized = false;
};

class ADPCMAudioReader : public PluginBase {
public:
	ADPCMAudioReader() = default;
	bool Open(DataStream* stream) override;
	int read_samples(short* buffer, int count) override;

private:
	DataStream* str = nullptr;
	int channels = 0;
	int samplerate = 0;
	bool initialized = false;

	struct ADPCMState {
		int predictor;
		int step_index;
	} state[2]; // For stereo

	static const int indexTable[16];
	static const int stepsizeTable[89];

	int DecodeNibble(uint8_t nibble, ADPCMState* state);
};

} // namespace GemRB

#endif // XBOX_AUDIO_READER_H
