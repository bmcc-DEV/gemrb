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

// Simple demonstration: Python ie_action.py constants translated to C++
// This shows how the Python constant definitions can be replaced with C++ headers

#ifndef IE_ACTION_CONSTANTS_H
#define IE_ACTION_CONSTANTS_H

// Note: keep this file in sync with PCStatStructs.h
// These constants are already defined in PCStatStructs.h but this shows
// how Python constant files can be directly translated to C++

namespace GemRB {
namespace ActionConstants {

// Action buttons
const int ACT_STEALTH = 0;
const int ACT_THIEVING = 1;
const int ACT_CAST = 2;
const int ACT_QSPELL1 = 3;
const int ACT_QSPELL2 = 4;
const int ACT_QSPELL3 = 5;
const int ACT_TURN = 6;
const int ACT_TALK = 7;
const int ACT_USE = 8;
const int ACT_QSLOT1 = 9;
const int ACT_QSLOT4 = 10; // heh
const int ACT_QSLOT2 = 11;
const int ACT_QSLOT3 = 12;
const int ACT_QSLOT5 = 31; // 
const int ACT_INNATE = 13;
const int ACT_DEFEND = 14;
const int ACT_ATTACK = 15;
const int ACT_WEAPON1 = 16;
const int ACT_WEAPON2 = 17;
const int ACT_WEAPON3 = 18;
const int ACT_WEAPON4 = 19;
const int ACT_BARDSONG = 20;
const int ACT_STOP = 21;
const int ACT_SEARCH = 22;
const int ACT_SHAPE = 23;
const int ACT_TAMING = 24;
const int ACT_SKILLS = 25;
const int ACT_WILDERNESS = 26;
const int ACT_DANCE = 27;
const int ACT_OFFHAND = 28; // pair slot to ACT_WEAPON1 - ACT_WEAPON4

// GUI navigation (scrolling button rows left or right)
const int ACT_LEFT = 32;
const int ACT_RIGHT = 33;

// IWD2 customization buttons
const int ACT_CLEAR = 34;
const int ACT_RESTORE = 35;

const int ACT_NONE = 100;

const int ACT_BARD = 40;
const int ACT_CLERIC = 41;
const int ACT_DRUID = 42;
const int ACT_PALADIN = 43;
const int ACT_RANGER = 44;
const int ACT_SORCERER = 45;
const int ACT_WIZARD = 46;
const int ACT_DOMAIN = 47;
const int ACT_WILDSHAPE = 48;

const int ACT_IWDQSPELL = 50;
const int ACT_IWDQITEM = 60;
const int ACT_IWDQSPEC = 70;
const int ACT_IWDQSONG = 80;

}
}

#endif