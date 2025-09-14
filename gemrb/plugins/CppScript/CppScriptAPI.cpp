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

#include "CppScriptAPI.h"
#include "Interface.h"
#include "Game.h"
#include "GUI/WindowManager.h"
#include "GUI/GUIFactory.h"
#include "DisplayMessage.h"
#include "GameData.h"
#include "Logging/Logging.h"
#include "StringMap.h"

namespace GemRB {
namespace CppAPI {

unsigned long GetGameTime() {
	Game* game = core->GetGame();
	if (game) {
		return game->GameTime;
	}
	return 0;
}

void GamePause(int mode, int count) {
	Game* game = core->GetGame();
	if (game) {
		game->SetPause(mode, count);
	}
}

bool HasResource(const std::string& resource, unsigned long type) {
	return gamedata->Exists(resource, type);
}

Window* GetView(const std::string& name) {
	WindowManager* wm = core->GetWindowManager();
	if (wm) {
		return wm->GetWindow(name);
	}
	return nullptr;
}

Control* CreateView(int id, int type, const Region& frame, const std::vector<Parameter>& args) {
	// This would need more implementation to match the Python API
	// For now, return nullptr
	return nullptr;
}

std::string GetString(unsigned long strref) {
	return core->GetString(strref, STRING_FLAGS::NONE);
}

void SetToken(const std::string& name, const std::string& value) {
	core->GetTokenDictionary().SetAtCopy(name, value);
}

namespace GameCheck {

bool IsIWD2() {
	return core->HasFeature(GFFlags::IWD2_DEATHVARFORMAT);
}

bool IsBG2OrEE() {
	return core->HasFeature(GFFlags::HAS_KAPUTZ);
}

bool IsPST() {
	return core->HasFeature(GFFlags::PST_STATE_FLAGS);
}

}

namespace GUICommon {

void SetGamedaysAndHourToken() {
	Game* game = core->GetGame();
	if (!game) return;

	int days = game->GameTime / 7200;
	int hours = (game->GameTime % 7200) / 300;
	int realHours = (hours + 12) % 24;

	core->GetTokenDictionary().SetAtCopy("GAMEDAYS", std::to_string(days));
	core->GetTokenDictionary().SetAtCopy("HOUR", std::to_string(realHours));
}

}

void Log(LogLevel level, const std::string& source, const std::string& message) {
	::GemRB::Log(static_cast<log_level>(level), source, message);
}

}
}