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

#include "ClockScript.h"
#include "CppScriptAPI.h"

#include "GUI/Button.h"
#include "GUI/Control.h"
#include "GUI/Window.h"
#include "Logging/Logging.h"

namespace GemRB {
namespace ClockScript {

/**
 * C++ translation of CreateClockButton from Clock.py
 * Sets up the clock button with appropriate animation and callbacks
 */
Parameter CreateClockButton(const FunctionParameters& params) {
	if (params.empty()) {
		Log(ERROR, "ClockScript", "CreateClockButton: No button parameter provided");
		return Parameter();
	}

	// Extract button from parameters (would need proper conversion from Parameter to Button*)
	// For now, this is a placeholder implementation
	Button* button = nullptr; // TODO: Extract from params[0]
	
	if (!button) {
		return Parameter();
	}

	using namespace CppAPI;

	// Set button flags: IE_GUI_BUTTON_PICTURE | IE_GUI_BUTTON_NORMAL
	unsigned long flags = 0x00000002 | 0x00000004;

	// FIXME: display all animations: CPEN, CGEAR, CDIAL
	button->SetAnimation("CGEAR", 0, A_ANI_GAMEANIM);
	button->SetState(Control::State::Enabled);
	button->SetFlags(flags, BitOp::SET);
	
	// Set the pause callback: lambda: GemRB.GamePause(2, 0)
	button->SetEvent(Control::EventType::MouseUp, [](Control*) {
		CppAPI::GamePause(2, 0);
	});

	if (GameCheck::IsIWD2()) {
		button->SetState(Control::State::Locked); // no button depression, timer is an inset stone planet
	} else if (GameCheck::IsBG2OrEE()) {
		// Create pen animation button
		Button* pen = button->CreateSubview<Button>(0x10000009);
		if (pen) {
			pen->SetFlags(flags | 0x10000000, BitOp::SET); // IE_GUI_VIEW_IGNORE_EVENTS
			pen->SetAnimation("CPEN", 0, A_ANI_GAMEANIM);
		}
	}

	UpdateClock(FunctionParameters());
	return Parameter();
}

/**
 * C++ translation of UpdateClock from Clock.py
 * Updates the clock display with current game time
 */
Parameter UpdateClock(const FunctionParameters& params) {
	using namespace CppAPI;

	Window* optionsWindow = GetView("OPTWIN");
	Window* actionsWindow = GetView("ACTWIN");
	
	Control* clock = nullptr;
	
	if (optionsWindow) {
		if (GameCheck::IsIWD2()) {
			clock = optionsWindow->GetControl(10);
		} else {
			Control* control9 = optionsWindow->GetControl(9);
			if (control9) {
				clock = control9;
			}
		}
		
		if (clock && !clock->IsVisible()) {
			clock = nullptr;
		}
	}
	
	if (!clock && actionsWindow) {
		clock = actionsWindow->GetControl(62);
	}
	
	if (clock) {
		unsigned long gameTime = GetGameTime();
		int hours = (gameTime % 7200) / 300;
		
		GUICommon::SetGamedaysAndHourToken();
		
		// Set clock BAM animation frame
		Button* clockButton = dynamic_cast<Button*>(clock);
		if (clockButton) {
			int frame = (hours + 12) % 24;
			clockButton->SetPicture("CDIAL", 0, frame);
			
			// Set tooltip
			std::string tooltip = GetString(16041); // refetch the string, since the tokens changed
			clockButton->SetTooltip(tooltip);
		}
	}

	return Parameter();
}

}
}