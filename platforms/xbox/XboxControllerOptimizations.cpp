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

#include "XboxControllerOptimizations.h"

#ifdef XBOX

	#include "XboxLogger.h"
	#include <algorithm>

namespace GemRB {

// XboxControllerManager implementation

XboxControllerManager& XboxControllerManager::GetInstance()
{
	static XboxControllerManager instance;
	return instance;
}

void XboxControllerManager::Initialize()
{
	if (initialized) {
		return;
	}

	debugPrint("Xbox: Initializing controller manager...\n");

	// Initialize all possible controller ports
	for (int i = 0; i < XBOX_MAX_CONTROLLERS; i++) {
		InitializeController(i);
	}

	RefreshControllerState();
	initialized = true;

	debugPrint("Xbox: Found %d connected controllers\n", GetConnectedControllerCount());
}

bool XboxControllerManager::IsControllerConnected(int port) const
{
	if (port < 0 || port >= XBOX_MAX_CONTROLLERS) {
		return false;
	}
	return controllersConnected[port];
}

int XboxControllerManager::GetConnectedControllerCount() const
{
	int count = 0;
	for (int i = 0; i < XBOX_MAX_CONTROLLERS; i++) {
		if (controllersConnected[i]) {
			count++;
		}
	}
	return count;
}

void XboxControllerManager::RefreshControllerState()
{
	// Check controller connection status for all ports
	for (int i = 0; i < XBOX_MAX_CONTROLLERS; i++) {
		// In real implementation, this would check Xbox controller port status
		controllersConnected[i] = (i == 0); // Assume controller 0 is connected
	}

	UpdateRumbleStates();
}

void XboxControllerManager::TriggerRumble(XboxRumbleEffect effect, int controller)
{
	if (!IsControllerConnected(controller)) {
		return;
	}

	float leftIntensity = 0.0f;
	float rightIntensity = 0.0f;
	uint32_t duration = 200; // Default 200ms

	switch (effect) {
		case XboxRumbleEffect::DAMAGE_TAKEN:
			leftIntensity = XBOX_RUMBLE_INTENSITY_HIGH;
			rightIntensity = XBOX_RUMBLE_INTENSITY_MEDIUM;
			duration = 300;
			break;
		case XboxRumbleEffect::SPELL_CAST:
			leftIntensity = XBOX_RUMBLE_INTENSITY_LOW;
			rightIntensity = XBOX_RUMBLE_INTENSITY_LOW;
			duration = 150;
			break;
		case XboxRumbleEffect::CRITICAL_HIT:
			leftIntensity = XBOX_RUMBLE_INTENSITY_HIGH;
			rightIntensity = XBOX_RUMBLE_INTENSITY_HIGH;
			duration = 400;
			break;
		case XboxRumbleEffect::LEVEL_UP:
			leftIntensity = XBOX_RUMBLE_INTENSITY_MEDIUM;
			rightIntensity = XBOX_RUMBLE_INTENSITY_MEDIUM;
			duration = 600;
			break;
		case XboxRumbleEffect::INVENTORY_FULL:
			leftIntensity = XBOX_RUMBLE_INTENSITY_LOW;
			rightIntensity = 0.0f;
			duration = 100;
			break;
		case XboxRumbleEffect::LOW_HEALTH:
			leftIntensity = XBOX_RUMBLE_INTENSITY_MEDIUM;
			rightIntensity = 0.0f;
			duration = 250;
			break;
		case XboxRumbleEffect::COMBAT_START:
			leftIntensity = XBOX_RUMBLE_INTENSITY_MEDIUM;
			rightIntensity = XBOX_RUMBLE_INTENSITY_LOW;
			duration = 200;
			break;
		case XboxRumbleEffect::DIALOGUE_CHOICE:
			leftIntensity = XBOX_RUMBLE_INTENSITY_LOW;
			rightIntensity = XBOX_RUMBLE_INTENSITY_LOW;
			duration = 100;
			break;
		default:
			return;
	}

	SetRumbleIntensity(leftIntensity, rightIntensity, controller);
	rumbleStates[controller].endTime = GetCurrentTimeMs() + duration;

	debugPrint("Xbox: Triggered rumble effect %d on controller %d\n",
		   static_cast<int>(effect), controller);
}

void XboxControllerManager::SetRumbleIntensity(float leftMotor, float rightMotor, int controller)
{
	if (!IsControllerConnected(controller)) {
		return;
	}

	// Clamp values to valid range
	leftMotor = std::clamp(leftMotor, 0.0f, 1.0f);
	rightMotor = std::clamp(rightMotor, 0.0f, 1.0f);

	rumbleStates[controller].leftMotor = leftMotor;
	rumbleStates[controller].rightMotor = rightMotor;

	// In real implementation, this would call Xbox controller rumble API
	debugPrint("Xbox: Setting rumble intensity L:%.2f R:%.2f on controller %d\n",
		   leftMotor, rightMotor, controller);
}

void XboxControllerManager::StopRumble(int controller)
{
	if (!IsControllerConnected(controller)) {
		return;
	}

	SetRumbleIntensity(0.0f, 0.0f, controller);
	rumbleStates[controller].endTime = 0;
}

void XboxControllerManager::SetLEDState(XboxLEDState state, int controller)
{
	if (!IsControllerConnected(controller)) {
		return;
	}

	debugPrint("Xbox: Setting LED state %d on controller %d\n",
		   static_cast<int>(state), controller);

	// In real implementation, this would call Xbox controller LED API
}

void XboxControllerManager::SetPlayerLED(int playerNumber, int controller)
{
	if (playerNumber >= 1 && playerNumber <= 4) {
		SetLEDState(static_cast<XboxLEDState>(playerNumber), controller);
	}
}

bool XboxControllerManager::IsAnalogStickMoved(int controller) const
{
	if (!IsControllerConnected(controller)) {
		return false;
	}

	// In real implementation, this would check analog stick position
	return false;
}

float XboxControllerManager::GetTriggerPressure(bool rightTrigger, int controller) const
{
	if (!IsControllerConnected(controller)) {
		return 0.0f;
	}

	// In real implementation, this would return trigger pressure (0.0-1.0)
	return 0.0f;
}

void XboxControllerManager::EnableEnhancedPrecision(bool enable, int controller)
{
	if (!IsControllerConnected(controller)) {
		return;
	}

	debugPrint("Xbox: %s enhanced precision for controller %d\n",
		   enable ? "Enabling" : "Disabling", controller);

	// Configure enhanced analog stick precision for fine cursor movement
}

void XboxControllerManager::SetAnalogDeadzone(float deadzone, int controller)
{
	if (!IsControllerConnected(controller)) {
		return;
	}

	deadzone = std::clamp(deadzone, 0.0f, 0.5f);
	debugPrint("Xbox: Setting analog deadzone to %.2f for controller %d\n",
		   deadzone, controller);
}

void XboxControllerManager::InitializeController(int port)
{
	debugPrint("Xbox: Initializing controller port %d...\n", port);

	// In real implementation, this would initialize Xbox controller port
	// Check if controller is connected
	// Set up default configuration

	// For simulation, assume port 0 has a controller
	controllersConnected[port] = (port == 0);
}

void XboxControllerManager::UpdateRumbleStates()
{
	uint32_t currentTime = GetCurrentTimeMs();

	for (int i = 0; i < XBOX_MAX_CONTROLLERS; i++) {
		if (rumbleStates[i].endTime > 0 && currentTime >= rumbleStates[i].endTime) {
			StopRumble(i);
		}
	}
}

uint32_t XboxControllerManager::GetCurrentTimeMs() const
{
	return GetTickCount(); // Xbox kernel function
}

// XboxGameEventHandler implementation

bool XboxGameEventHandler::initialized = false;
bool XboxGameEventHandler::rumbleEnabled = true;

void XboxGameEventHandler::Initialize()
{
	if (initialized) {
		return;
	}

	debugPrint("Xbox: Initializing game event handler...\n");

	// Initialize controller manager
	XboxControllerManager::GetInstance().Initialize();

	initialized = true;
}

void XboxGameEventHandler::OnPlayerDamaged(int damage, int playerController)
{
	if (!rumbleEnabled) return;

	// Scale rumble intensity based on damage amount
	XboxRumbleEffect effect = XboxRumbleEffect::DAMAGE_TAKEN;
	if (damage > 50) {
		// Heavy damage - stronger rumble
		XboxControllerManager::GetInstance().SetRumbleIntensity(
			XBOX_RUMBLE_INTENSITY_HIGH, XBOX_RUMBLE_INTENSITY_HIGH, playerController);
	} else {
		XboxControllerManager::GetInstance().TriggerRumble(effect, playerController);
	}
}

void XboxGameEventHandler::OnSpellCast(const std::string& spellName, int playerController)
{
	if (!rumbleEnabled) return;

	debugPrint("Xbox: Spell cast rumble for '%s'\n", spellName.c_str());
	XboxControllerManager::GetInstance().TriggerRumble(XboxRumbleEffect::SPELL_CAST, playerController);
}

void XboxGameEventHandler::OnCriticalHit(int playerController)
{
	if (!rumbleEnabled) return;

	XboxControllerManager::GetInstance().TriggerRumble(XboxRumbleEffect::CRITICAL_HIT, playerController);
}

void XboxGameEventHandler::OnLevelUp(int playerController)
{
	if (!rumbleEnabled) return;

	// Level up gets special extended rumble
	XboxControllerManager::GetInstance().TriggerRumble(XboxRumbleEffect::LEVEL_UP, playerController);

	// Also flash the LED
	XboxControllerManager::GetInstance().SetLEDState(XboxLEDState::BLINKING, playerController);
}

void XboxGameEventHandler::OnCombatStart(int playerController)
{
	if (!rumbleEnabled) return;

	XboxControllerManager::GetInstance().TriggerRumble(XboxRumbleEffect::COMBAT_START, playerController);
}

void XboxGameEventHandler::OnInventoryFull(int playerController)
{
	if (!rumbleEnabled) return;

	XboxControllerManager::GetInstance().TriggerRumble(XboxRumbleEffect::INVENTORY_FULL, playerController);
}

void XboxGameEventHandler::OnLowHealth(int playerController)
{
	if (!rumbleEnabled) return;

	XboxControllerManager::GetInstance().TriggerRumble(XboxRumbleEffect::LOW_HEALTH, playerController);
}

void XboxGameEventHandler::OnDialogueChoice(int playerController)
{
	if (!rumbleEnabled) return;

	XboxControllerManager::GetInstance().TriggerRumble(XboxRumbleEffect::DIALOGUE_CHOICE, playerController);
}

void XboxGameEventHandler::OnImportantEvent(int playerController)
{
	if (!rumbleEnabled) return;

	// Custom rumble pattern for important events
	XboxControllerManager::GetInstance().SetRumbleIntensity(
		XBOX_RUMBLE_INTENSITY_MEDIUM, XBOX_RUMBLE_INTENSITY_LOW, playerController);
}

void XboxGameEventHandler::OnPartyMemberAction(int partyMemberIndex, XboxRumbleEffect effect)
{
	int controller = GetControllerForPartyMember(partyMemberIndex);
	if (controller >= 0) {
		TriggerControllerFeedback(effect, controller);
	}
}

int XboxGameEventHandler::GetControllerForPartyMember(int partyMemberIndex)
{
	// Map party members to controllers (if multiple controllers connected)
	if (partyMemberIndex >= 0 && partyMemberIndex < XBOX_MAX_CONTROLLERS) {
		if (XboxControllerManager::GetInstance().IsControllerConnected(partyMemberIndex)) {
			return partyMemberIndex;
		}
	}

	// Default to controller 0
	return 0;
}

void XboxGameEventHandler::TriggerControllerFeedback(XboxRumbleEffect effect, int controller)
{
	XboxControllerManager::GetInstance().TriggerRumble(effect, controller);
}

// XboxSaveGameManager implementation

bool XboxSaveGameManager::initialized = false;

void XboxSaveGameManager::Initialize()
{
	if (initialized) {
		return;
	}

	debugPrint("Xbox: Initializing save game manager...\n");
	initialized = true;
}

bool XboxSaveGameManager::IsMemoryCardInserted(int slot)
{
	// Check Xbox memory card slot status
	debugPrint("Xbox: Checking memory card slot %d\n", slot);
	return true; // Assume memory card is present
}

size_t XboxSaveGameManager::GetMemoryCardFreeSpace(int slot)
{
	// Get available space on Xbox memory card
	return 8 * 1024 * 1024; // Return 8MB as example
}

bool XboxSaveGameManager::OptimizeSaveGame(const std::string& saveFile)
{
	debugPrint("Xbox: Optimizing save game: %s\n", saveFile.c_str());

	// Optimize save game for Xbox:
	// - Compress save data
	// - Remove unnecessary data
	// - Optimize for Xbox memory card

	return true;
}

void XboxSaveGameManager::CompressSaveData(void* data, size_t size)
{
	debugPrint("Xbox: Compressing save data (%zu bytes)\n", size);

	// Implement Xbox-optimized compression
}

void XboxSaveGameManager::CreateSaveGameThumbnail(const std::string& saveFile)
{
	debugPrint("Xbox: Creating thumbnail for save: %s\n", saveFile.c_str());

	// Create Xbox dashboard-compatible save game thumbnail
}

void XboxSaveGameManager::SetSaveGameMetadata(const std::string& saveFile, const std::string& description)
{
	debugPrint("Xbox: Setting save game metadata: %s\n", description.c_str());

	// Set Xbox save game metadata for dashboard display
}

// XboxSystemIntegration implementation

bool XboxSystemIntegration::initialized = false;
bool XboxSystemIntegration::sleepPrevented = false;

void XboxSystemIntegration::Initialize()
{
	if (initialized) {
		return;
	}

	debugPrint("Xbox: Initializing system integration...\n");

	SetDashboardTitle("GemRB - Infinity Engine");
	ApplySystemAudioSettings();
	ApplySystemVideoSettings();

	initialized = true;
}

void XboxSystemIntegration::SetDashboardTitle(const std::string& title)
{
	debugPrint("Xbox: Setting dashboard title: %s\n", title.c_str());

	// Set Xbox dashboard title for the application
}

void XboxSystemIntegration::UpdateDashboardStatus(const std::string& status)
{
	debugPrint("Xbox: Dashboard status: %s\n", status.c_str());

	// Update Xbox dashboard status information
}

void XboxSystemIntegration::SetSystemLEDColor(uint8_t red, uint8_t green, uint8_t blue)
{
	debugPrint("Xbox: Setting system LED color (R:%d G:%d B:%d)\n", red, green, blue);

	// Control Xbox front panel LED color
}

void XboxSystemIntegration::SetSystemLEDPattern(const std::string& pattern)
{
	debugPrint("Xbox: Setting system LED pattern: %s\n", pattern.c_str());

	// Set Xbox front panel LED pattern (flashing, etc.)
}

uint64_t XboxSystemIntegration::GetXboxSystemTime()
{
	// Get Xbox system time
	return GetTickCount64();
}

void XboxSystemIntegration::SetGameTimer(uint32_t seconds)
{
	debugPrint("Xbox: Setting game timer for %d seconds\n", seconds);

	// Set Xbox game timer for automatic sleep prevention
}

void XboxSystemIntegration::PreventSleep()
{
	if (!sleepPrevented) {
		debugPrint("Xbox: Preventing system sleep\n");
		// Prevent Xbox from going to sleep during gameplay
		sleepPrevented = true;
	}
}

void XboxSystemIntegration::AllowSleep()
{
	if (sleepPrevented) {
		debugPrint("Xbox: Allowing system sleep\n");
		// Allow Xbox to sleep when not actively playing
		sleepPrevented = false;
	}
}

bool XboxSystemIntegration::IsACPowerConnected()
{
	// Check if Xbox is connected to AC power
	return true; // Xbox consoles are typically always plugged in
}

void XboxSystemIntegration::ApplySystemAudioSettings()
{
	debugPrint("Xbox: Applying system audio settings\n");

	// Apply Xbox system audio configuration to GemRB
	// - Use system volume levels
	// - Apply surround sound settings
	// - Configure audio output format
}

void XboxSystemIntegration::ApplySystemVideoSettings()
{
	debugPrint("Xbox: Applying system video settings\n");

	// Apply Xbox system video configuration to GemRB
	// - Use system resolution settings
	// - Apply display mode preferences
	// - Configure aspect ratio
}

} // namespace GemRB

#endif // XBOX
