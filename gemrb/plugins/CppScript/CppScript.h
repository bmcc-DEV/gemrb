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

#ifndef CPPSCRIPT_H
#define CPPSCRIPT_H

#include "ScriptEngine.h"
#include <map>
#include <memory>
#include <functional>

namespace GemRB {

class CppScript : public ScriptEngine {
public:
	using ModuleFunction = std::function<Parameter(const FunctionParameters&)>;
	using ModuleFunctionMap = std::map<std::string, ModuleFunction>;
	using ModuleMap = std::map<std::string, ModuleFunctionMap>;

private:
	ModuleMap modules;
	bool initialized = false;

	void RegisterClockModule();
	void RegisterMainModule();
	void RegisterCommonModule();

public:
	CppScript() noexcept = default;
	~CppScript() override = default;

	/** Initialization Routine */
	bool Init() override;
	/** Load Script - for C++ this is registering modules */
	bool LoadScript(const std::string& filename) override;
	/** Run Function */
	Parameter RunFunction(const char* moduleName, const char* functionName, const FunctionParameters& params, bool report_error = true) override;
	/** Exec a single String - not applicable for C++ scripts */
	bool ExecString(const std::string& string, bool feedback) override;
};

}

#endif