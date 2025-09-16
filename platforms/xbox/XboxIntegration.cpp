/* GemRB - Infinity Engine Emulator
 * Copyright (C) 2024 The GemRB Project
 *
 * Xbox Integration Demo - demonstrates Xbox platform features
 */

#include "Logging/Logging.h"
#include "platforms/xbox/Xbox.h"
#include "platforms/xbox/XboxController.h"
#include "platforms/xbox/XboxMemory.h"
#include "platforms/xbox/XboxUI.h"

namespace GemRB {

class XboxIntegrationDemo {
public:
	static void RunDemo()
	{
		Log(MESSAGE, "XboxDemo", "Starting Xbox integration demonstration");

		// Initialize Xbox platform
		if (XboxPlatform::IsXboxPlatform()) {
			Log(MESSAGE, "XboxDemo", "Running on Xbox hardware");
			DemonstrateXboxFeatures();
		} else {
			Log(MESSAGE, "XboxDemo", "Running Xbox simulation on other platform");
			DemonstrateSimulatedFeatures();
		}
	}

private:
	static void DemonstrateXboxFeatures()
	{
		// Memory management demo
		auto memInfo = XboxMemoryManager::GetMemoryInfo();
		Log(MESSAGE, "XboxDemo", "Memory: %.1f%% used (%zu MB / %zu MB)",
		    memInfo.usagePercent * 100.0f,
		    memInfo.usedMemory / (1024 * 1024),
		    memInfo.totalMemory / (1024 * 1024));

		// Controller demo
		int controllers = XboxController::GetControllerCount();
		Log(MESSAGE, "XboxDemo", "Found %d Xbox controllers", controllers);

		if (controllers > 0) {
			// Demonstrate rumble
			XboxController::SetRumble(0, 0.5f, 0.5f, 1000);
			Log(MESSAGE, "XboxDemo", "Triggered rumble on controller 0");
		}

		// Audio demo
		if (XboxPlatform::SupportsXMAFormat()) {
			Log(MESSAGE, "XboxDemo", "XMA audio format supported");
		}

		if (XboxPlatform::SupportsADPCMFormat()) {
			Log(MESSAGE, "XboxDemo", "ADPCM audio format supported");
		}

		// UI demo
		XboxUI::ShowLoadingScreen("Demonstrating Xbox features...", 0.5f);

		// File system demo
		const char* cDrive = XboxPlatform::GetPartitionPath('C');
		if (cDrive) {
			Log(MESSAGE, "XboxDemo", "C: drive mounted at %s", cDrive);
		}

		// Dashboard integration
		Log(MESSAGE, "XboxDemo", "Dashboard integration available");

		XboxUI::HideLoadingScreen();
		XboxUI::ShowInfoMessage("Xbox integration demo completed!", 3000);
	}

	static void DemonstrateSimulatedFeatures()
	{
		Log(MESSAGE, "XboxDemo", "Simulating Xbox features on current platform");

		// Show what would be available on Xbox
		Log(MESSAGE, "XboxDemo", "Would support:");
		Log(MESSAGE, "XboxDemo", "- XMA audio decoding");
		Log(MESSAGE, "XboxDemo", "- 64MB memory management");
		Log(MESSAGE, "XboxDemo", "- Controller rumble and LEDs");
		Log(MESSAGE, "XboxDemo", "- Dashboard integration");
		Log(MESSAGE, "XboxDemo", "- CRT TV optimizations");
		Log(MESSAGE, "XboxDemo", "- Xbox partition access");

		XboxUI::ShowInfoMessage("Xbox simulation demo - see logs for details", 5000);
	}
};

// Integration function to be called from main engine initialization
void InitializeXboxIntegration()
{
	if (XboxPlatform::IsXboxPlatform()) {
		Log(MESSAGE, "XboxIntegration", "Initializing Xbox platform support");

		XboxPlatform::Initialize();
		XboxController::Initialize();
		XboxMemoryManager::Initialize();
		XboxUI::Initialize();

		// Run demo
		XboxIntegrationDemo::RunDemo();

		Log(MESSAGE, "XboxIntegration", "Xbox platform initialization complete");
	} else {
		Log(MESSAGE, "XboxIntegration", "Xbox platform not detected, skipping initialization");
	}
}

void ShutdownXboxIntegration()
{
	if (XboxPlatform::IsXboxPlatform()) {
		Log(MESSAGE, "XboxIntegration", "Shutting down Xbox platform support");

		XboxUI::Shutdown();
		XboxMemoryManager::Shutdown();
		XboxController::Shutdown();
		XboxPlatform::Shutdown();

		Log(MESSAGE, "XboxIntegration", "Xbox platform shutdown complete");
	}
}

} // namespace GemRB
