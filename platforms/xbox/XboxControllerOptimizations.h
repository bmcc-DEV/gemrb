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

#ifndef XBOX_CONTROLLER_OPTIMIZATIONS_H
#define XBOX_CONTROLLER_OPTIMIZATIONS_H

#ifdef XBOX

	#include <cstdint>
	#include <string>
	#include <windows.h>

namespace GemRB {

// Xbox controller constants
static constexpr int XBOX_MAX_CONTROLLERS = 4;
static constexpr float XBOX_RUMBLE_INTENSITY_LOW = 0.3f;
static constexpr float XBOX_RUMBLE_INTENSITY_MEDIUM = 0.6f;
static constexpr float XBOX_RUMBLE_INTENSITY_HIGH = 1.0f;

// Xbox controller rumble effects
enum class XboxRumbleEffect {
	NONE = 0,
	DAMAGE_TAKEN,
	SPELL_CAST,
	CRITICAL_HIT,
	LEVEL_UP,
	INVENTORY_FULL,
	LOW_HEALTH,
	COMBAT_START,
	DIALOGUE_CHOICE
};

// Xbox controller LED states
enum class XboxLEDState {
	OFF = 0,
	PLAYER_1 = 1,
	PLAYER_2 = 2,
	PLAYER_3 = 3,
	PLAYER_4 = 4,
	ROTATING = 5,
	BLINKING = 6,
	SLOW_BLINK = 7
};

// Xbox controller input optimization
class XboxControllerManager {
public:
	static XboxControllerManager& GetInstance();

	// Initialize Xbox controller system
	void Initialize();

	// Controller detection and management
	bool IsControllerConnected(int port = 0) const;
	int GetConnectedControllerCount() const;
	void RefreshControllerState();

	// Xbox controller rumble features
	void TriggerRumble(XboxRumbleEffect effect, int controller = 0);
	void SetRumbleIntensity(float leftMotor, float rightMotor, int controller = 0);
	void StopRumble(int controller = 0);

	// Xbox controller LED control
	void SetLEDState(XboxLEDState state, int controller = 0);
	void SetPlayerLED(int playerNumber, int controller = 0);

	// Enhanced input handling
	bool IsAnalogStickMoved(int controller = 0) const;
	float GetTriggerPressure(bool rightTrigger, int controller = 0) const;

	// Xbox-specific controller features
	void EnableEnhancedPrecision(bool enable, int controller = 0);
	void SetAnalogDeadzone(float deadzone, int controller = 0);

private:
	XboxControllerManager() = default;
	bool initialized = false;
	bool controllersConnected[XBOX_MAX_CONTROLLERS] = { false };

	// Rumble state tracking
	struct RumbleState {
		float leftMotor = 0.0f;
		float rightMotor = 0.0f;
		uint32_t endTime = 0;
	};

	RumbleState rumbleStates[XBOX_MAX_CONTROLLERS];

	// Helper methods
	void InitializeController(int port);
	void UpdateRumbleStates();
	uint32_t GetCurrentTimeMs() const;
};

// Xbox-specific game event integration
class XboxGameEventHandler {
public:
	// Initialize game event handling
	static void Initialize();

	// Game event rumble triggers
	static void OnPlayerDamaged(int damage, int playerController = 0);
	static void OnSpellCast(const std::string& spellName, int playerController = 0);
	static void OnCriticalHit(int playerController = 0);
	static void OnLevelUp(int playerController = 0);
	static void OnCombatStart(int playerController = 0);
	static void OnInventoryFull(int playerController = 0);
	static void OnLowHealth(int playerController = 0);

	// Visual feedback events
	static void OnDialogueChoice(int playerController = 0);
	static void OnImportantEvent(int playerController = 0);

	// Multi-controller support for party members
	static void OnPartyMemberAction(int partyMemberIndex, XboxRumbleEffect effect);

private:
	static bool initialized;
	static bool rumbleEnabled;

	// Helper methods
	static int GetControllerForPartyMember(int partyMemberIndex);
	static void TriggerControllerFeedback(XboxRumbleEffect effect, int controller);
};

// Xbox memory card and save game optimization
class XboxSaveGameManager {
public:
	// Initialize Xbox save game system
	static void Initialize();

	// Xbox memory card detection
	static bool IsMemoryCardInserted(int slot = 0);
	static size_t GetMemoryCardFreeSpace(int slot = 0);

	// Xbox save game optimization
	static bool OptimizeSaveGame(const std::string& saveFile);
	static void CompressSaveData(void* data, size_t size);

	// Xbox-specific save features
	static void CreateSaveGameThumbnail(const std::string& saveFile);
	static void SetSaveGameMetadata(const std::string& saveFile, const std::string& description);

private:
	static bool initialized;
};

// Xbox system integration features
class XboxSystemIntegration {
public:
	// Initialize Xbox system integration
	static void Initialize();

	// Xbox dashboard integration
	static void SetDashboardTitle(const std::string& title);
	static void UpdateDashboardStatus(const std::string& status);

	// Xbox system LED control (front panel)
	static void SetSystemLEDColor(uint8_t red, uint8_t green, uint8_t blue);
	static void SetSystemLEDPattern(const std::string& pattern);

	// Xbox clock and timer integration
	static uint64_t GetXboxSystemTime();
	static void SetGameTimer(uint32_t seconds);

	// Xbox power management
	static void PreventSleep();
	static void AllowSleep();
	static bool IsACPowerConnected();

	// Xbox audio/video settings integration
	static void ApplySystemAudioSettings();
	static void ApplySystemVideoSettings();

private:
	static bool initialized;
	static bool sleepPrevented;
};

} // namespace GemRB

#endif // XBOX
#endif // XBOX_CONTROLLER_OPTIMIZATIONS_H
