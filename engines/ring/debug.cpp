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
 * $Id: debug.cpp 391 2010-05-04 16:07:30Z julien.templier $
 *
 */

#include "ring/debug.h"

#include "ring/ring.h"

#include "common/debug-channels.h"
#include "common/events.h"
#include "common/file.h"

namespace Ring {

Debugger::Debugger(RingEngine *engine) : _engine(engine) {

	//////////////////////////////////////////////////////////////////////////
	// Register the debugger commands

	// General
	DCmd_Register("help",      WRAP_METHOD(Debugger, cmdHelp));

	// Data
	DCmd_Register("ls",        WRAP_METHOD(Debugger, cmdListFiles));
	DCmd_Register("dump",      WRAP_METHOD(Debugger, cmdDumpArchive));
}

Debugger::~Debugger() {
	DebugMan.clearAllDebugChannels();

	// Zero passed pointers
	_engine = NULL;
}

int Debugger::getNumber(const char *arg) const {
	return strtol(arg, (char **)NULL, 0);
}

bool Debugger::cmdHelp(int, const char **) {
	DebugPrintf("Debug flags\n");
	DebugPrintf("-----------\n");
	DebugPrintf(" debugflag_list - Lists the available debug flags and their status\n");
	DebugPrintf(" debugflag_enable - Enables a debug flag\n");
	DebugPrintf(" debugflag_disable - Disables a debug flag\n");
	DebugPrintf("\n");
	DebugPrintf("Commands\n");
	DebugPrintf("--------\n");
	DebugPrintf(" ls   - list files in the archive\n");
	DebugPrintf(" dump - dump the files from an archive\n");
	DebugPrintf("\n");
	DebugPrintf(" clear - clear the screen\n");
	DebugPrintf("\n");
	return true;
}

bool Debugger::cmdListFiles(int argc, const char **argv) {
	if (argc == 2) {
		Common::String filter(const_cast<char *>(argv[1]));

		//Common::ArchiveMemberList list;
		//int count = _engine->getResourceManager()->listMatchingMembers(list, filter);

		//DebugPrintf("Number of matches: %d\n", count);
		//for (Common::ArchiveMemberList::iterator it = list.begin(); it != list.end(); ++it)
		//	DebugPrintf(" %s\n", (*it)->getName().c_str());
	} else {
		DebugPrintf("Syntax: ls <filter> (use * for all)\n");
	}

	return true;
}

bool Debugger::cmdDumpArchive(int argc, const char **argv) {
#ifdef DEBUG
	if (argc == 2) {
		Common::String filename(const_cast<char *>(argv[1]));

		if (filename == "*") {
			Common::ArchiveMemberList list;
			int count = _engine->getResourceManager()->listMatchingMembers(list, "*.mul");

			DebugPrintf("Dumping %d archives\n", count);
			for (Common::ArchiveMemberList::iterator it = list.begin(); it != list.end(); ++it)
				dumpFile((*it)->getName());
		} else
			dumpFile(filename);
	} else {
		DebugPrintf("Syntax: dump <filename>.mul (use * to dump all archives) \n");
	}

#else
	DebugPrintf("dump is not supported release mode!\n");
#endif

	return true;
}

bool Debugger::cmdClear(int argc, const char **) {
	if (argc == 1) {
		/*askForRedraw();
		redrawScreen();*/
	} else {
		DebugPrintf("Syntax: clear - clear the screen\n");
	}

	return true;
}

} // End of namespace Ring
