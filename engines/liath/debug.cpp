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

#include "liath/debug.h"

#include "liath/data/archive.h"

#include "liath/liath.h"
#include "liath/resource.h"

#include "common/debug-channels.h"
#include "common/events.h"
#include "common/file.h"
#include "common/stream.h"

#define DEBUG
#ifdef DEBUG
// For mkdir

#ifdef WIN32
#include <direct.h>
#include <sys/stat.h>
#define my_mkdir(folder, mode) mkdir(folder)
#else
#include <unistd.h>
#define my_mkdir(folder, mode) mkdir(folder, mode)
#endif

#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#endif

namespace Liath {

Debugger::Debugger(LiathEngine *engine) : _engine(engine), _command(NULL), _numParams(0), _commandParams(NULL) {

	//////////////////////////////////////////////////////////////////////////
	// Register the debugger commands

	// General
	DCmd_Register("help",      WRAP_METHOD(Debugger, cmdHelp));

	// Data
	DCmd_Register("ls",        WRAP_METHOD(Debugger, cmdListFiles));
	DCmd_Register("dump",      WRAP_METHOD(Debugger, cmdDumpArchive));

	resetCommand();
}

Debugger::~Debugger() {
	DebugMan.clearAllDebugChannels();

	// Zero passed pointers
	_engine = NULL;
	_command = NULL;
	_commandParams = NULL;
}

bool Debugger::hasCommand() const {
	return (_numParams != 0);
}

void Debugger::resetCommand() {
	_command = NULL;
	_commandParams = NULL;
	_numParams = 0;
}

int Debugger::getNumber(const char *arg) const {
	return strtol(arg, (char **)NULL, 0);
}

void Debugger::copyCommand(int argc, const char **argv) {
	_commandParams = (char **)malloc((uint)argc);
	if (!_commandParams)
		return;

	_numParams = argc;

	for (int i = 0; i < _numParams; i++) {
		_commandParams[i] = (char *)malloc(strlen(argv[i]));
		strcpy(_commandParams[i], "");
		strcpy(_commandParams[i], argv[i]);
	}

	// Exit the debugger!
	Cmd_Exit(0, 0);
}

void Debugger::callCommand() {
	if (_command)
		(*_command)(_numParams, const_cast<const char **>(_commandParams));
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

		Common::ArchiveMemberList list;
		int count = _engine->getResourceManager()->listMatchingMembers(list, filter);

		DebugPrintf("Number of matches: %d\n", count);
		for (Common::ArchiveMemberList::iterator it = list.begin(); it != list.end(); ++it)
			DebugPrintf(" %s\n", (*it)->getName().c_str());
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

void Debugger::dumpFile(Common::String filename) {
#define CREATE_FOLDER(name) { \
	folder += name; \
	int ret = my_mkdir(folder.c_str(), 600); \
	if (ret == -1 && errno != EEXIST) { \
	DebugPrintf("Cannot create folder: %s", folder.c_str()); \
	return; \
	} \
}

	filename.toLowercase();
	if (!filename.contains(".mul"))
		filename += ".mul";

	if (!_engine->getResourceManager()->hasFile(filename)) {
		DebugPrintf("Cannot find file: %s\n", filename.c_str());
		return;
	}

	// Load MUL archive
	MultiArchive *archive = new MultiArchive(filename);
	Common::ArchiveMemberList list;
	int count = archive->listMembers(list);

	// Get the current working folder
	char buffer[512];
	getcwd((char *)&buffer, 512);
	Common::String folder(buffer);

	// Create folders for dumping data
	CREATE_FOLDER("/dumps/");
	CREATE_FOLDER("/liath/");
	CREATE_FOLDER(filename.c_str());

	// Dump all members
	DebugPrintf("Dumping %d files from archive %s\n", count, filename.c_str());
	for (Common::ArchiveMemberList::iterator it = list.begin(); it != list.end(); ++it) {
		Common::String name = (*it)->getName();
		Common::SeekableReadStream *stream = archive->createReadStreamForMember(name);

		byte *data = (byte *)calloc(stream->size(), 1);
		stream->read(data, stream->size());

		Common::DumpFile out;
		if (out.open(Common::String("dumps/liath/") + filename + "/" + name)) {
			out.write(data, stream->size());
			out.close();
		}

		free(data);

		delete stream;

		DebugPrintf("  - %s\n", name.c_str());
	}

	DebugPrintf("\n");
#undef CREATE_FOLDER
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

} // End of namespace Liath
