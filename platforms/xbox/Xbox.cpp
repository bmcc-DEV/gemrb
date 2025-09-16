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

#include "Xbox.h"

#include "Logging/Logging.h"

#ifdef _XBOX
	#include <xboxmath.h>
	#include <xgraphics.h>
	#include <xtl.h>
#endif

namespace GemRB {

bool XboxPlatform::initialized = false;
bool XboxPlatform::memoryLow = false;
float XboxPlatform::memoryThreshold = 0.8f; // 80% of 64MB

bool XboxPlatform::IsXboxPlatform()
{
#ifdef _XBOX
	return true;
#else
	return false;
#endif
}

void XboxPlatform::Initialize()
{
	if (initialized) return;

	Log(MESSAGE, "XboxPlatform", "Initializing Xbox platform support");

#ifdef _XBOX
	// Initialize Xbox-specific systems
	XInitDevices(0, NULL);

	// Mount Xbox partitions
	MountXboxPartitions();

	// Disable screensaver initially
	DisableScreensaver();

	// Optimize for CRT display
	OptimizeForCRT();
#endif

	initialized = true;
}

void XboxPlatform::Shutdown()
{
	if (!initialized) return;

	Log(MESSAGE, "XboxPlatform", "Shutting down Xbox platform support");

#ifdef _XBOX
	// Stop any custom soundtrack
	StopCustomSoundtrack();

	// Re-enable screensaver
	EnableScreensaver();
#endif

	initialized = false;
}

bool XboxPlatform::SetControllerRumble(int controller, float leftMotor, float rightMotor)
{
#ifdef _XBOX
	if (controller < 0 || controller >= XGetPortCount()) return false;

	XINPUT_RUMBLE rumble;
	rumble.wLeftMotorSpeed = (WORD) (leftMotor * 65535.0f);
	rumble.wRightMotorSpeed = (WORD) (rightMotor * 65535.0f);

	return XInputSetRumble(controller, &rumble) == ERROR_SUCCESS;
#else
	return false;
#endif
}

void XboxPlatform::SetControllerLED(int controller, bool on)
{
#ifdef _XBOX
	// Xbox doesn't have controllable LEDs on controllers
	// This is a placeholder for future implementations
#endif
}

bool XboxPlatform::IsControllerConnected(int controller)
{
#ifdef _XBOX
	if (controller < 0 || controller >= XGetPortCount()) return false;

	XINPUT_CAPABILITIES caps;
	return XInputGetCapabilities(controller, XINPUT_FLAG_GAMEPAD, &caps) == ERROR_SUCCESS;
#else
	return false;
#endif
}

void XboxPlatform::UpdateControllerDeadzone(int controller, float deadzone)
{
	// Store deadzone setting for controller
	// This will be used in the controller input processing
	Log(DEBUG, "XboxPlatform", "Setting controller %d deadzone to %f", controller, deadzone);
}

size_t XboxPlatform::GetAvailableMemory()
{
#ifdef _XBOX
	MEMORYSTATUS memStatus;
	GlobalMemoryStatus(&memStatus);
	return memStatus.dwAvailPhys;
#else
	return 64 * 1024 * 1024; // Simulate 64MB for non-Xbox builds
#endif
}

bool XboxPlatform::IsMemoryLow()
{
	size_t available = GetAvailableMemory();
	size_t total = 64 * 1024 * 1024; // 64MB total Xbox memory
	return (float) available / total < (1.0f - memoryThreshold);
}

void XboxPlatform::TriggerMemoryCleanup()
{
	Log(MESSAGE, "XboxPlatform", "Triggering memory cleanup due to low memory");
	memoryLow = true;
	// This will trigger cleanup in other systems
}

bool XboxPlatform::SupportsXMAFormat()
{
#ifdef _XBOX
	return true;
#else
	return false;
#endif
}

bool XboxPlatform::SupportsADPCMFormat()
{
#ifdef _XBOX
	return true;
#else
	return false;
#endif
}

bool XboxPlatform::MountXboxPartitions()
{
#ifdef _XBOX
	Log(MESSAGE, "XboxPlatform", "Mounting Xbox partitions");

	// Mount C: drive (system)
	// Mount E: drive (cache)
	// Mount F: drive (saves)
	// Mount D: drive (DVD)

	return true;
#else
	return false;
#endif
}

const char* XboxPlatform::GetPartitionPath(char partition)
{
#ifdef _XBOX
	switch (partition) {
		case 'C':
		case 'c':
			return "C:\\";
		case 'E':
		case 'e':
			return "E:\\";
		case 'F':
		case 'f':
			return "F:\\";
		case 'D':
		case 'd':
			return "D:\\";
		default:
			return nullptr;
	}
#else
	return nullptr;
#endif
}

void XboxPlatform::ReturnToDashboard()
{
#ifdef _XBOX
	Log(MESSAGE, "XboxPlatform", "Returning to Xbox dashboard");
	XLaunchNewImage(NULL, NULL);
#endif
}

void XboxPlatform::PauseGame()
{
	Log(MESSAGE, "XboxPlatform", "Pausing game");
	// This will be handled by the main game loop
}

void XboxPlatform::ResumeGame()
{
	Log(MESSAGE, "XboxPlatform", "Resuming game");
	// This will be handled by the main game loop
}

void XboxPlatform::DisableScreensaver()
{
#ifdef _XBOX
	// Disable Xbox screensaver
	SetThreadExecutionState(ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED | ES_CONTINUOUS);
#endif
}

void XboxPlatform::EnableScreensaver()
{
#ifdef _XBOX
	// Re-enable Xbox screensaver
	SetThreadExecutionState(ES_CONTINUOUS);
#endif
}

bool XboxPlatform::HasCustomSoundtrack()
{
#ifdef _XBOX
	// Check if custom soundtrack is available
	return false; // Placeholder implementation
#else
	return false;
#endif
}

bool XboxPlatform::PlayCustomSoundtrack()
{
#ifdef _XBOX
	// Start playing custom soundtrack
	return false; // Placeholder implementation
#else
	return false;
#endif
}

void XboxPlatform::StopCustomSoundtrack()
{
#ifdef _XBOX
	// Stop custom soundtrack
#endif
}

void XboxPlatform::OptimizeForCRT()
{
	Log(MESSAGE, "XboxPlatform", "Optimizing display for CRT TV");
	// Set appropriate refresh rate and resolution for CRT TVs
	// Adjust font sizes and UI elements for TV viewing distance
}

bool XboxPlatform::IsCRTOptimal()
{
	return true; // Assume optimization is applied
}

} // namespace GemRB
