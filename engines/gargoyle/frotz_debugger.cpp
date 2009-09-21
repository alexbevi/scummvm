/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#include "gargoyle/frotz_interp.h"

namespace Frotz {

/**
 * Converts a passed string to an integer value
 */
static int strToInt(const char *s) {
	if (!*s)
		// No string at all
		return 0;
	else if (toupper(s[strlen(s) - 1]) != 'H')
		// Standard decimal string
		return atoi(s);

	// Hexadecimal string
	uint tmp;
	sscanf(s, "%xh", &tmp);
	return (int)tmp;
}

void FrotzInterpreter::initDebugger() {
	// Register debugger methods
	DCmd_Register("sound",			WRAP_METHOD(FrotzInterpreter, cmd_sound));
}

/**
 * Debugger method to start a given sound number
 */
bool FrotzInterpreter::cmd_sound(int argc, const char **argv) {
	if (argc == 1) {
		DebugPrintf("sound <sound_number>\n");
		return true;
	}

	if (_os.getSound())
		_os.getSound()->play(strToInt(argv[1]), 0, 255, 0);
	else
		DebugPrintf("Game does not support sound\n");

	return true;
}

} // end of namespace Frotz
