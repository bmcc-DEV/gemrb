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

#ifndef CLOCKSCRIPT_H
#define CLOCKSCRIPT_H

#include "ScriptEngine.h"

namespace GemRB {

/**
 * C++ translation of Clock.py
 * Manages the game clock display and animation
 */
namespace ClockScript {

// Function declarations matching Python interface
Parameter CreateClockButton(const FunctionParameters& params);
Parameter UpdateClock(const FunctionParameters& params);

}

}

#endif