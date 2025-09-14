/* GemRB - Infinity Engine Emulator
 * Copyright (C) 2003 The GemRB Project
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

// GemRB.cpp : Defines the entry point for Xbox application.

#include "Interface.h"
#include "XboxAudioOptimizations.h"
#include "XboxControllerOptimizations.h"
#include "XboxLogger.h"
#include "XboxMemoryOptimizations.h"

#include <Python.h>
#include <SDL.h>
#include <clocale> //language encoding

#ifdef XBOX
	#include <hal/debug.h>
	#include <windows.h>
	#include <xboxkrnl/xboxkrnl.h>
#endif

// Memory allocation for Xbox - use conservative values due to 64MB limit
#ifdef XBOX
// Reserve about 48MB for GemRB, leaving 16MB for system
static const size_t XBOX_HEAP_SIZE = 48 * 1024 * 1024;

// Xbox soundtrack paths - where Xbox stores its music files
static const char* XBOX_SOUNDTRACK_PATHS[] = {
	"C:\\TDATA\\FFFE0000\\music\\", // Xbox soundtrack directory
	"E:\\UDATA\\soundtrack\\", // User soundtrack directory
	"F:\\UDATA\\soundtrack\\", // Alternative user soundtrack
	"C:\\UDATA\\soundtrack\\", // System user soundtrack
	nullptr
};

// Xbox audio optimization settings
static const size_t XBOX_AUDIO_BUFFER_SIZE = 8192; // Smaller buffer for memory efficiency
static const int XBOX_MAX_SIMULTANEOUS_SOUNDS = 16; // Limit concurrent sounds
#endif

char* xboxArgv[3];
char configPath[256];

void XboxSetArguments(int* argc, char** argv[])
{
#ifdef XBOX
	int xboxArgc = 1;
	xboxArgv[0] = (char*) "gemrb";

	// Check if config file exists and use it
	HANDLE hFile = CreateFile("E:\\GemRB\\GemRB.cfg",
				  GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
				  FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(hFile);
		xboxArgv[1] = (char*) "-c";
		strcpy(configPath, "E:\\GemRB\\GemRB.cfg");
		xboxArgv[2] = configPath;
		xboxArgc = 3;
	}

	*argc = xboxArgc;
	*argv = xboxArgv;
#else
	// Non-Xbox fallback
	*argc = 1;
	*argv = xboxArgv;
	xboxArgv[0] = (char*) "gemrb";
#endif
}

// Xbox soundtrack discovery function
#ifdef XBOX
void XboxDiscoverSoundtracks()
{
	debugPrint("Xbox: Discovering available soundtracks...\n");

	// Initialize the Xbox soundtrack manager
	XboxSoundtrackManager::GetInstance().Initialize();

	if (XboxSoundtrackManager::GetInstance().IsXboxSoundtrackAvailable()) {
		debugPrint("Xbox: Soundtrack integration enabled\n");

		// Set audio optimizations for Xbox
		debugPrint("Xbox: Applying audio optimizations...\n");
		debugPrint("Xbox: Buffer size: %d, Max channels: %d, Frequency: %d\n",
			   static_cast<int>(XboxAudioOptimizer::GetOptimalBufferSize()),
			   XboxAudioOptimizer::GetMaxConcurrentChannels(),
			   XboxAudioOptimizer::GetOptimalFrequency());
	} else {
		debugPrint("Xbox: No soundtracks found, using default audio settings\n");
	}
}
#endif

using namespace GemRB;

int main(int argc, char* argv[])
{
#ifdef XBOX
	// Initialize Xbox-specific systems
	HalInitiateShutdown();

	// Set up minimal memory management
	// Xbox has limited memory, so we need to be conservative

	// Initialize debug output early
	debugPrint("GemRB starting on Xbox...\n");
#endif

	// Selecting game config from Xbox filesystem
	XboxSetArguments(&argc, &argv);

	setlocale(LC_ALL, "");

	AddLogWriter(createXboxLogger());
	ToggleLogging(true);

	SanityCheck();

#ifdef XBOX
	// Minimize Python overhead for Xbox's limited memory
	Py_NoSiteFlag = 1;
	Py_IgnoreEnvironmentFlag = 1;
	Py_NoUserSiteDirectory = 1;
	Py_OptimizeFlag = 2; // Enable optimization to reduce memory usage
#endif

	try {
#ifdef XBOX
		// Initialize enhanced Xbox optimization systems
		debugPrint("Xbox: Initializing enhanced optimization systems...\n");

		// Initialize memory management first
		XboxMemoryManager::GetInstance().Initialize();

		// Initialize GPU and texture optimizations
		XboxGPUOptimizer::Initialize();
		XboxTextureCache::GetInstance().Initialize();

		// Initialize storage optimizations
		XboxStorageOptimizer::Initialize();

		// Initialize controller and system integration
		XboxGameEventHandler::Initialize();
		XboxSystemIntegration::Initialize();

		// Initialize enhanced audio systems
		XboxAudioHardware::InitializeDSP();
		XboxAudioHardware::SetupXboxAudioEffects();

		// Initialize Xbox soundtrack discovery
		XboxDiscoverSoundtracks();

		// Enable power management during gameplay
		XboxSystemIntegration::PreventSleep();

		// Print initial memory stats
		XboxMemoryManager::GetInstance().PrintMemoryStats();

		debugPrint("Xbox: All optimization systems initialized successfully\n");
#endif
		Interface gemrb(LoadFromArgs(argc, argv));
		gemrb.Main();
	} catch (CoreInitializationException& cie) {
		Log(FATAL, "Main", "Aborting due to fatal error... {}", cie);
		ToggleLogging(false);
#ifdef XBOX
		debugPrint("GemRB fatal error - shutting down Xbox\n");
		HalInitiateShutdown();
#endif
		return GEM_ERROR;
	}

#ifdef XBOX
	// Enhanced Xbox cleanup sequence
	debugPrint("GemRB shutting down - performing Xbox cleanup...\n");

	// Allow system to sleep again
	XboxSystemIntegration::AllowSleep();

	// Flush any remaining caches and free memory
	XboxMemoryManager::GetInstance().FlushUnusedCaches();
	XboxMemoryManager::GetInstance().CompactMemoryPools();

	// Stop all controller rumble
	for (int i = 0; i < 4; i++) {
		XboxControllerManager::GetInstance().StopRumble(i);
	}

	// Reset system LED to normal state
	XboxSystemIntegration::SetSystemLEDPattern("normal");

	// Print final memory statistics
	XboxMemoryManager::GetInstance().PrintMemoryStats();

	debugPrint("Xbox: Cleanup completed successfully\n");
#endif

	VideoDriver.reset();
	ToggleLogging(false);

#ifdef XBOX
	// Proper Xbox shutdown
	HalInitiateShutdown();
#endif

	return 0;
}
