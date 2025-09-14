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

// Simple demonstration: Python ie_modal.py constants translated to C++

#ifndef IE_MODAL_CONSTANTS_H
#define IE_MODAL_CONSTANTS_H

namespace GemRB {
namespace ModalConstants {

// Modal states
const int MS_NONE = 0;
const int MS_BATTLESONG = 1;
const int MS_DETECTTRAPS = 2;
const int MS_STEALTH = 3;
const int MS_TURNUNDEAD = 4;
const int MS_DANCE = 5;

}
}

#endif