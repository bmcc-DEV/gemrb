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

#ifndef XBOX_PLATFORM_H
#define XBOX_PLATFORM_H

#include "exports.h"
#include "ie_types.h"

namespace GemRB {

class GEM_EXPORT XboxPlatform {
public:
	static bool IsXboxPlatform();
	static void Initialize();
	static void Shutdown();

	// Controller features
	static bool SetControllerRumble(int controller, float leftMotor, float rightMotor);
	static void SetControllerLED(int controller, bool on);
	static bool IsControllerConnected(int controller);
	static void UpdateControllerDeadzone(int controller, float deadzone);

	// Memory management
	static size_t GetAvailableMemory();
	static bool IsMemoryLow();
	static void TriggerMemoryCleanup();

	// Audio
	static bool SupportsXMAFormat();
	static bool SupportsADPCMFormat();

	// File system
	static bool MountXboxPartitions();
	static const char* GetPartitionPath(char partition); // C, E, F, D

	// Dashboard integration
	static void ReturnToDashboard();
	static void PauseGame();
	static void ResumeGame();

	// Screensaver
	static void DisableScreensaver();
	static void EnableScreensaver();

	// Soundtrack
	static bool HasCustomSoundtrack();
	static bool PlayCustomSoundtrack();
	static void StopCustomSoundtrack();

	// Display optimization for CRT TVs
	static void OptimizeForCRT();
	static bool IsCRTOptimal();

private:
	static bool initialized;
	static bool memoryLow;
	static float memoryThreshold;
};

} // namespace GemRB

#endif // XBOX_PLATFORM_H
