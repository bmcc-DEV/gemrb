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

#ifndef CPPSCRIPTAPI_H
#define CPPSCRIPTAPI_H

#include "ScriptEngine.h"
#include "GUI/Window.h"
#include "GUI/Control.h"
#include "GUI/Button.h"

namespace GemRB {

// Forward declarations for core classes
class Interface;
class Game;
class GameControl;

/**
 * C++ API wrapper that mimics the Python GemRB API
 * This provides the same functionality as the Python GemRB module
 */
namespace CppAPI {

// Game state functions
unsigned long GetGameTime();
void GamePause(int mode, int count);
bool HasResource(const std::string& resource, unsigned long type);

// Window and control management  
Window* GetView(const std::string& name);
Control* CreateView(int id, int type, const Region& frame, const std::vector<Parameter>& args = {});

// String and token management
std::string GetString(unsigned long strref);
void SetToken(const std::string& name, const std::string& value);

// Game check functions
namespace GameCheck {
	bool IsIWD2();
	bool IsBG2OrEE();
	bool IsPST();
}

// Common GUI functions  
namespace GUICommon {
	void SetGamedaysAndHourToken();
}

// Animation constants
enum AnimationFlags {
	A_ANI_GAMEANIM = 0x1000
};

// Log levels
enum LogLevel {
	LOG_MESSAGE = 0,
	LOG_ERROR = 3
};

void Log(LogLevel level, const std::string& source, const std::string& message);

}

}

#endif