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

#ifndef RING_DEBUG_H
#define RING_DEBUG_H

#include "gui/debugger.h"

namespace Ring {

enum {
	kRingDebugAll         = 1 << 0,
	kRingDebugGraphics    = 1 << 1,
	kRingDebugResource    = 1 << 2,
	kRingDebugSound       = 1 << 3,
	kRingDebugSavegame    = 1 << 4,
	kRingDebugLogic       = 1 << 5,
	kRingDebugUnknown     = 1 << 6
	// the current limitation is 32 debug levels (1 << 31 is the last one)
};

class RingEngine;

class Debugger : public GUI::Debugger {
public:
	Debugger(RingEngine *engine);
	~Debugger();

private:
	RingEngine *_engine;

	bool cmdHelp(int argc, const char **argv);

	bool cmdListFiles(int argc, const char **argv);
	bool cmdDumpArchive(int argc, const char **argv);

	bool cmdClear(int argc, const char **argv);
	bool cmdShow(int argc, const char **argv);

#ifdef RING_DUMP
	void dumpFile(Common::String filename);
#endif

	int getNumber(const char *arg) const;
};

} // End of namespace Ring

#endif // RING_DEBUG_H
