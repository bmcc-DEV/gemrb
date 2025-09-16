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

#ifndef XBOX_CONTROLLER_H
#define XBOX_CONTROLLER_H

#include "exports.h"
#include "ie_types.h"

#include "GUI/EventMgr.h"

namespace GemRB {

class GEM_EXPORT XboxController {
public:
	struct ControllerState {
		bool connected;
		float leftStickX, leftStickY;
		float rightStickX, rightStickY;
		float leftTrigger, rightTrigger;
		bool buttons[16];
		float deadzone;
		bool rumbleEnabled;
		float rumbleLeft, rumbleRight;
		tick_t lastUpdate;
	};

	static void Initialize();
	static void Shutdown();
	static void Update();

	// Controller management
	static int GetControllerCount();
	static bool IsControllerConnected(int controller);
	static ControllerState* GetControllerState(int controller);

	// Input processing
	static bool ProcessControllerEvent(ControllerEvent& event);
	static void SetDeadzone(int controller, float deadzone);
	static float ApplyDeadzone(float value, float deadzone);

	// Xbox-specific features
	static void SetRumble(int controller, float leftMotor, float rightMotor, uint32_t duration = 0);
	static void StopRumble(int controller);
	static void SetLED(int controller, bool on);

	// Event generation
	static void TriggerRumbleOnEvent(const String& eventType, float intensity = 0.5f);

private:
	static ControllerState controllers[4];
	static bool initialized;
	static float globalDeadzone;
	static bool rumbleEnabled;

	static void DetectControllerChanges();
	static void UpdateControllerInput(int controller);
	static void ProcessRumbleQueue();
};

} // namespace GemRB

#endif // XBOX_CONTROLLER_H
