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

#include "liath/liath.h"
#include "liath/resource.h"

#include "graphics/video/avi_decoder.h"

#include "common/debug-channels.h"
#include "common/events.h"

namespace Liath {

Debugger::Debugger(LiathEngine *engine) : _engine(engine), _command(NULL), _numParams(0), _commandParams(NULL) {

	// Register the debugger commands
	DCmd_Register("clear",     WRAP_METHOD(Debugger, cmd_clear));

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

bool Debugger::cmd_clear(int argc, const char **) {
	if (argc == 1) {
		/*askForRedraw();
		redrawScreen();*/
	} else {
		DebugPrintf("Syntax: clear - clear the screen\n");
	}

	return true;
}

} // End of namespace Liath
