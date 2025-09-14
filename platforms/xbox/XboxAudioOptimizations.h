/* GemRB - Infinity Engine Emulator
 * Copyright (C) 2024 The GemRB Project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef XBOX_AUDIO_OPTIMIZATIONS_H
#define XBOX_AUDIO_OPTIMIZATIONS_H

#ifdef XBOX

	#include <string>
	#include <vector>
	#include <windows.h>

namespace GemRB {

// Xbox-specific audio optimization constants
static constexpr size_t XBOX_OPTIMAL_BUFFER_SIZE = 8192;
static constexpr size_t XBOX_MUSIC_BUFFER_SIZE = 4096;
static constexpr int XBOX_MAX_CONCURRENT_SOUNDS = 16;
static constexpr int XBOX_AUDIO_FREQUENCY = 22050; // Lower freq to save memory

// Xbox soundtrack integration
class XboxSoundtrackManager {
public:
	static XboxSoundtrackManager& GetInstance();

	// Initialize Xbox soundtrack discovery
	void Initialize();

	// Get available Xbox soundtrack paths
	std::vector<std::string> GetSoundtrackPaths() const;

	// Check if Xbox soundtrack directory exists
	bool IsXboxSoundtrackAvailable() const;

	// Get Xbox soundtrack file for music integration
	std::string GetXboxSoundtrackFile(const std::string& filename) const;

private:
	XboxSoundtrackManager() = default;
	std::vector<std::string> soundtrackPaths;
	bool initialized = false;

	void DiscoverSoundtrackDirectories();
	bool DirectoryExists(const std::string& path) const;
};

// Xbox audio buffer optimization helpers
class XboxAudioOptimizer {
public:
	// Get optimal buffer size for Xbox hardware
	static size_t GetOptimalBufferSize(bool isMusic = false);

	// Get optimal audio frequency for Xbox
	static int GetOptimalFrequency();

	// Check if we should use reduced quality for memory savings
	static bool ShouldUseReducedQuality();

	// Get maximum concurrent audio channels for Xbox
	static int GetMaxConcurrentChannels();
};

} // namespace GemRB

#endif // XBOX
#endif // XBOX_AUDIO_OPTIMIZATIONS_H
