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

#include "XboxController.h"

#include "GlobalTimer.h"
#include "Xbox.h"

#include "Logging/Logging.h"

#ifdef _XBOX
	#include <xtl.h>
#endif

namespace GemRB {

XboxController::ControllerState XboxController::controllers[4];
bool XboxController::initialized = false;
float XboxController::globalDeadzone = 0.15f;
bool XboxController::rumbleEnabled = true;

void XboxController::Initialize()
{
	if (initialized) return;

	Log(MESSAGE, "XboxController", "Initializing Xbox controller support");

	for (int i = 0; i < 4; i++) {
		controllers[i].connected = false;
		controllers[i].leftStickX = 0.0f;
		controllers[i].leftStickY = 0.0f;
		controllers[i].rightStickX = 0.0f;
		controllers[i].rightStickY = 0.0f;
		controllers[i].leftTrigger = 0.0f;
		controllers[i].rightTrigger = 0.0f;
		controllers[i].deadzone = globalDeadzone;
		controllers[i].rumbleEnabled = rumbleEnabled;
		controllers[i].rumbleLeft = 0.0f;
		controllers[i].rumbleRight = 0.0f;
		controllers[i].lastUpdate = 0;

		for (int j = 0; j < 16; j++) {
			controllers[i].buttons[j] = false;
		}
	}

	initialized = true;
}

void XboxController::Shutdown()
{
	if (!initialized) return;

	Log(MESSAGE, "XboxController", "Shutting down Xbox controller support");

	// Stop all rumble
	for (int i = 0; i < 4; i++) {
		StopRumble(i);
	}

	initialized = false;
}

void XboxController::Update()
{
	if (!initialized) return;

	DetectControllerChanges();

	for (int i = 0; i < 4; i++) {
		if (controllers[i].connected) {
			UpdateControllerInput(i);
		}
	}

	ProcessRumbleQueue();
}

int XboxController::GetControllerCount()
{
	int count = 0;
	for (int i = 0; i < 4; i++) {
		if (controllers[i].connected) {
			count++;
		}
	}
	return count;
}

bool XboxController::IsControllerConnected(int controller)
{
	if (controller < 0 || controller >= 4) return false;
	return controllers[controller].connected;
}

XboxController::ControllerState* XboxController::GetControllerState(int controller)
{
	if (controller < 0 || controller >= 4) return nullptr;
	return &controllers[controller];
}

bool XboxController::ProcessControllerEvent(ControllerEvent& event)
{
	// Apply deadzone to analog inputs
	if (event.axis == AXIS_LEFT_X || event.axis == AXIS_LEFT_Y ||
	    event.axis == AXIS_RIGHT_X || event.axis == AXIS_RIGHT_Y) {
		float deadzone = globalDeadzone;
		event.axisPct = ApplyDeadzone(event.axisPct, deadzone);
	}

	return true;
}

void XboxController::SetDeadzone(int controller, float deadzone)
{
	if (controller < 0 || controller >= 4) return;
	controllers[controller].deadzone = deadzone;
	Log(DEBUG, "XboxController", "Set controller %d deadzone to %f", controller, deadzone);
}

float XboxController::ApplyDeadzone(float value, float deadzone)
{
	if (abs(value) < deadzone) {
		return 0.0f;
	}

	// Scale the remaining value
	float sign = (value > 0) ? 1.0f : -1.0f;
	return sign * (abs(value) - deadzone) / (1.0f - deadzone);
}

void XboxController::SetRumble(int controller, float leftMotor, float rightMotor, uint32_t duration)
{
	if (controller < 0 || controller >= 4) return;
	if (!controllers[controller].connected || !controllers[controller].rumbleEnabled) return;

	controllers[controller].rumbleLeft = leftMotor;
	controllers[controller].rumbleRight = rightMotor;

#ifdef _XBOX
	XINPUT_RUMBLE rumble;
	rumble.wLeftMotorSpeed = (WORD) (leftMotor * 65535.0f);
	rumble.wRightMotorSpeed = (WORD) (rightMotor * 65535.0f);
	XInputSetRumble(controller, &rumble);
#endif

	Log(DEBUG, "XboxController", "Set rumble for controller %d: L=%f R=%f", controller, leftMotor, rightMotor);
}

void XboxController::StopRumble(int controller)
{
	SetRumble(controller, 0.0f, 0.0f);
}

void XboxController::SetLED(int controller, bool on)
{
	if (controller < 0 || controller >= 4) return;
	// Xbox controllers don't have controllable LEDs, but we log for debugging
	Log(DEBUG, "XboxController", "LED control requested for controller %d: %s", controller, on ? "ON" : "OFF");
}

void XboxController::TriggerRumbleOnEvent(const String& eventType, float intensity)
{
	if (!rumbleEnabled) return;

	// Find the first connected controller and trigger rumble
	for (int i = 0; i < 4; i++) {
		if (controllers[i].connected) {
			if (eventType == "damage") {
				SetRumble(i, intensity, 0.0f, 200); // Left motor for damage
			} else if (eventType == "hit") {
				SetRumble(i, 0.0f, intensity, 100); // Right motor for hitting
			} else if (eventType == "spell") {
				SetRumble(i, intensity * 0.7f, intensity * 0.7f, 300); // Both motors for spells
			}
			break;
		}
	}
}

void XboxController::DetectControllerChanges()
{
#ifdef _XBOX
	for (int i = 0; i < 4; i++) {
		XINPUT_CAPABILITIES caps;
		bool connected = (XInputGetCapabilities(i, XINPUT_FLAG_GAMEPAD, &caps) == ERROR_SUCCESS);

		if (connected != controllers[i].connected) {
			controllers[i].connected = connected;
			if (connected) {
				Log(MESSAGE, "XboxController", "Controller %d connected", i);
			} else {
				Log(MESSAGE, "XboxController", "Controller %d disconnected", i);
				StopRumble(i);
			}
		}
	}
#else
	// For non-Xbox builds, simulate one connected controller
	if (!controllers[0].connected) {
		controllers[0].connected = true;
		Log(MESSAGE, "XboxController", "Simulated controller 0 connected");
	}
#endif
}

void XboxController::UpdateControllerInput(int controller)
{
#ifdef _XBOX
	XINPUT_STATE state;
	if (XInputGetState(controller, &state) != ERROR_SUCCESS) {
		controllers[controller].connected = false;
		return;
	}

	XINPUT_GAMEPAD& gamepad = state.Gamepad;

	// Update analog sticks
	controllers[controller].leftStickX = (float) gamepad.sThumbLX / 32767.0f;
	controllers[controller].leftStickY = (float) gamepad.sThumbLY / 32767.0f;
	controllers[controller].rightStickX = (float) gamepad.sThumbRX / 32767.0f;
	controllers[controller].rightStickY = (float) gamepad.sThumbRY / 32767.0f;

	// Update triggers
	controllers[controller].leftTrigger = (float) gamepad.bLeftTrigger / 255.0f;
	controllers[controller].rightTrigger = (float) gamepad.bRightTrigger / 255.0f;

	// Update buttons
	controllers[controller].buttons[CONTROLLER_BUTTON_A] = (gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
	controllers[controller].buttons[CONTROLLER_BUTTON_B] = (gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
	controllers[controller].buttons[CONTROLLER_BUTTON_X] = (gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
	controllers[controller].buttons[CONTROLLER_BUTTON_Y] = (gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;
	controllers[controller].buttons[CONTROLLER_BUTTON_LEFTSHOULDER] = (gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
	controllers[controller].buttons[CONTROLLER_BUTTON_RIGHTSHOULDER] = (gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
	controllers[controller].buttons[CONTROLLER_BUTTON_BACK] = (gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
	controllers[controller].buttons[CONTROLLER_BUTTON_START] = (gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
	controllers[controller].buttons[CONTROLLER_BUTTON_LEFTSTICK] = (gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0;
	controllers[controller].buttons[CONTROLLER_BUTTON_RIGHTSTICK] = (gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;
	controllers[controller].buttons[CONTROLLER_BUTTON_DPAD_UP] = (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
	controllers[controller].buttons[CONTROLLER_BUTTON_DPAD_DOWN] = (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
	controllers[controller].buttons[CONTROLLER_BUTTON_DPAD_LEFT] = (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
	controllers[controller].buttons[CONTROLLER_BUTTON_DPAD_RIGHT] = (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;

	controllers[controller].lastUpdate = GetMilliseconds();
#endif
}

void XboxController::ProcessRumbleQueue()
{
	// Handle rumble duration timers and automatic shutoff
	tick_t currentTime = GetMilliseconds();

	for (int i = 0; i < 4; i++) {
		if (controllers[i].connected && (controllers[i].rumbleLeft > 0.0f || controllers[i].rumbleRight > 0.0f)) {
			// Auto-shutoff rumble after 1 second if no duration specified
			if (currentTime - controllers[i].lastUpdate > 1000) {
				StopRumble(i);
			}
		}
	}
}

} // namespace GemRB
