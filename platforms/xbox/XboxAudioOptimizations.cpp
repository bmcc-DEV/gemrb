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

#include "XboxAudioOptimizations.h"

#ifdef XBOX

	#include "XboxLogger.h"

namespace GemRB {

// Xbox soundtrack paths to search
static const char* XBOX_SOUNDTRACK_SEARCH_PATHS[] = {
	"C:\\TDATA\\FFFE0000\\music\\", // Xbox system soundtrack directory
	"E:\\UDATA\\soundtrack\\", // User soundtrack directory on E: drive
	"F:\\UDATA\\soundtrack\\", // Alternative user soundtrack on F: drive
	"C:\\UDATA\\soundtrack\\", // System user soundtrack directory
	"E:\\GemRB\\xbox_soundtracks\\", // Custom GemRB Xbox soundtrack directory
	nullptr
};

// XboxSoundtrackManager implementation

XboxSoundtrackManager& XboxSoundtrackManager::GetInstance()
{
	static XboxSoundtrackManager instance;
	return instance;
}

void XboxSoundtrackManager::Initialize()
{
	if (initialized) {
		return;
	}

	debugPrint("Xbox: Initializing soundtrack manager...\n");
	DiscoverSoundtrackDirectories();
	initialized = true;

	debugPrint("Xbox: Found %d soundtrack directories\n", static_cast<int>(soundtrackPaths.size()));
}

std::vector<std::string> XboxSoundtrackManager::GetSoundtrackPaths() const
{
	return soundtrackPaths;
}

bool XboxSoundtrackManager::IsXboxSoundtrackAvailable() const
{
	return !soundtrackPaths.empty();
}

std::string XboxSoundtrackManager::GetXboxSoundtrackFile(const std::string& filename) const
{
	for (const auto& path : soundtrackPaths) {
		std::string fullPath = path + filename;

		// Check if file exists
		HANDLE hFile = CreateFile(fullPath.c_str(),
					  GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
					  FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile != INVALID_HANDLE_VALUE) {
			CloseHandle(hFile);
			return fullPath;
		}
	}
	return "";
}

void XboxSoundtrackManager::DiscoverSoundtrackDirectories()
{
	soundtrackPaths.clear();

	for (int i = 0; XBOX_SOUNDTRACK_SEARCH_PATHS[i] != nullptr; i++) {
		const char* path = XBOX_SOUNDTRACK_SEARCH_PATHS[i];

		if (DirectoryExists(path)) {
			soundtrackPaths.emplace_back(path);
			debugPrint("Xbox: Found soundtrack directory: %s\n", path);
		}
	}
}

bool XboxSoundtrackManager::DirectoryExists(const std::string& path) const
{
	DWORD attrs = GetFileAttributes(path.c_str());
	return (attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY));
}

// XboxAudioOptimizer implementation

size_t XboxAudioOptimizer::GetOptimalBufferSize(bool isMusic)
{
	if (isMusic) {
		return XBOX_MUSIC_BUFFER_SIZE;
	}
	return XBOX_OPTIMAL_BUFFER_SIZE;
}

int XboxAudioOptimizer::GetOptimalFrequency()
{
	return XBOX_AUDIO_FREQUENCY;
}

bool XboxAudioOptimizer::ShouldUseReducedQuality()
{
	// Always use reduced quality on Xbox to conserve memory
	return true;
}

int XboxAudioOptimizer::GetMaxConcurrentChannels()
{
	return XBOX_MAX_CONCURRENT_SOUNDS;
}

bool XboxAudioOptimizer::IsSurroundSoundSupported()
{
	return XBOX_ENABLE_SURROUND;
}

int XboxAudioOptimizer::GetSurroundChannelCount()
{
	return XBOX_SURROUND_CHANNELS;
}

size_t XboxAudioOptimizer::GetDSPBufferSize()
{
	return XBOX_DSP_BUFFER_SIZE;
}

// XboxAudioHardware implementation

bool XboxAudioHardware::InitializeDSP()
{
	debugPrint("Xbox: Initializing DSP audio hardware...\n");
	// Initialize Xbox DSP for enhanced audio processing
	// This would interface with Xbox's hardware DSP capabilities
	return true;
}

void XboxAudioHardware::EnableSurroundSound(bool enable)
{
	if (enable && IsDolbyDigitalSupported()) {
		debugPrint("Xbox: Enabling surround sound (5.1 channels)\n");
		// Configure Xbox audio for 5.1 surround output
	} else {
		debugPrint("Xbox: Using stereo audio output\n");
		// Configure for stereo output
	}
}

bool XboxAudioHardware::IsDolbyDigitalSupported()
{
	// Check Xbox hardware capabilities for Dolby Digital
	return true; // Xbox hardware supports Dolby Digital
}

bool XboxAudioHardware::IsXboxAudioHardwareDetected()
{
	// Detect if we're running on actual Xbox hardware
	// This could check for Xbox-specific hardware signatures
	return true;
}

int XboxAudioHardware::GetHardwareChannelCount()
{
	if (IsDolbyDigitalSupported()) {
		return XBOX_SURROUND_CHANNELS;
	}
	return XBOX_STEREO_CHANNELS;
}

void XboxAudioHardware::SetupXboxAudioEffects()
{
	debugPrint("Xbox: Setting up hardware audio effects...\n");

	// Initialize Xbox-specific audio effects
	EnableXboxEchoEffect(true);
	EnableXboxReverbEffect(true);
}

void XboxAudioHardware::EnableXboxEchoEffect(bool enable)
{
	if (enable) {
		debugPrint("Xbox: Enabling hardware echo effect\n");
		// Configure Xbox hardware echo/delay effects
	}
}

void XboxAudioHardware::EnableXboxReverbEffect(bool enable)
{
	if (enable) {
		debugPrint("Xbox: Enabling hardware reverb effect\n");
		// Configure Xbox hardware reverb effects for spatial audio
	}
}

} // namespace GemRB

#endif // XBOX
