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

#include "CppScript.h"
#include "ClockScript.h"
#include "CppScriptAPI.h"

#include "Interface.h"
#include "Logging/Logging.h"
#include "SClassID.h"

#include <algorithm>

namespace GemRB {

bool CppScript::Init() {
	if (initialized) {
		return true;
	}

	Log(MESSAGE, "CppScript", "Initializing C++ Script Engine");

	// Register all modules
	RegisterClockModule();
	RegisterMainModule();
	RegisterCommonModule();

	initialized = true;
	return true;
}

bool CppScript::LoadScript(const std::string& filename) {
	// For C++ scripts, this is a no-op since modules are registered during Init()
	// In the future, this could be used to register additional modules dynamically
	Log(MESSAGE, "CppScript", "LoadScript called for: " + filename);
	return true;
}

Parameter CppScript::RunFunction(const char* moduleName, const char* functionName, const FunctionParameters& params, bool report_error) {
	if (!initialized) {
		if (report_error) {
			Log(ERROR, "CppScript", "Script engine not initialized");
		}
		return Parameter();
	}

	std::string moduleStr(moduleName);
	std::string functionStr(functionName);

	// Convert module name to lowercase for case-insensitive lookup
	std::transform(moduleStr.begin(), moduleStr.end(), moduleStr.begin(), ::tolower);

	auto moduleIt = modules.find(moduleStr);
	if (moduleIt == modules.end()) {
		if (report_error) {
			Log(ERROR, "CppScript", "Module not found: " + moduleStr);
		}
		return Parameter();
	}

	auto funcIt = moduleIt->second.find(functionStr);
	if (funcIt == moduleIt->second.end()) {
		if (report_error) {
			Log(ERROR, "CppScript", "Function not found: " + functionStr + " in module: " + moduleStr);
		}
		return Parameter();
	}

	try {
		return funcIt->second(params);
	} catch (const std::exception& e) {
		if (report_error) {
			Log(ERROR, "CppScript", "Error calling " + moduleStr + "." + functionStr + ": " + e.what());
		}
		return Parameter();
	}
}

bool CppScript::ExecString(const std::string& string, bool feedback) {
	// C++ scripts don't support dynamic execution
	if (feedback) {
		Log(WARNING, "CppScript", "ExecString not supported for C++ scripts");
	}
	return false;
}

void CppScript::RegisterClockModule() {
	ModuleFunctionMap clockFunctions;
	clockFunctions["CreateClockButton"] = ClockScript::CreateClockButton;
	clockFunctions["UpdateClock"] = ClockScript::UpdateClock;
	
	modules["clock"] = clockFunctions;
}

void CppScript::RegisterMainModule() {
	ModuleFunctionMap mainFunctions;
	// Main module functions would be added here
	// For now, just create empty module
	modules["main"] = mainFunctions;
}

void CppScript::RegisterCommonModule() {
	ModuleFunctionMap commonFunctions;
	// Common module functions would be added here  
	// For now, just create empty module
	modules["guicommon"] = commonFunctions;
}

}

#include "plugindef.h"

GEMRB_PLUGIN(0x2DD7A9E5, "C++ Script Engine")
PLUGIN_CLASS(IE_CPP_SCRIPT_CLASS_ID, CppScript)
END_PLUGIN()