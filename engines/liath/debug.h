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
 * $Id: debug.h 376 2010-04-29 22:55:31Z julien.templier $
 *
 */

#ifndef LIATH_DEBUG_H
#define LIATH_DEBUG_H

#include "gui/debugger.h"

namespace Liath {

enum {
	kLiathDebugAll         = 1 << 0,
	kLiathDebugGraphics    = 1 << 1,
	kLiathDebugResource    = 1 << 2,
	kLiathDebugSound       = 1 << 3,
	kLiathDebugSavegame    = 1 << 4,
	kLiathDebugLogic       = 1 << 5,
	kLiathDebugInterpreter = 1 << 6,
	kLiathDebugUnknown     = 1 << 7
	// the current limitation is 32 debug levels (1 << 31 is the last one)
};

class LiathEngine;

class Debugger : public GUI::Debugger {
public:
	Debugger(LiathEngine *engine);
	~Debugger();

	bool hasCommand() const;
	void callCommand();

private:
	LiathEngine *_engine;

	bool cmdHelp(int argc, const char **argv);

	bool cmdListFiles(int argc, const char **argv);
	bool cmdDumpArchive(int argc, const char **argv);

	bool cmdClear(int argc, const char **argv);

	void resetCommand();
	void copyCommand(int argc, const char **argv);
	int getNumber(const char *arg) const;

	void dumpFile(Common::String filename);

	Debuglet *_command;
	int _numParams;
	char **_commandParams;
};

} // End of namespace Liath

#endif // LIATH_DEBUG_H
