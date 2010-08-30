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

#ifndef LIATH_ACTION_H
#define LIATH_ACTION_H

#include "liath/shared.h"

#include "common/str.h"

namespace Liath {

class LiathEngine;

class ActionManager {
public:

	ActionManager(LiathEngine *engine);
	~ActionManager();

	// Opcodes
	OpcodeRet exitGame(OpcodeParameters *parameters);
	OpcodeRet start(OpcodeParameters *parameters);
	OpcodeRet startGlobal(OpcodeParameters *parameters);
	OpcodeRet startHeroVariable(OpcodeParameters *parameters);
	OpcodeRet startVariable(OpcodeParameters *parameters);
	OpcodeRet number(OpcodeParameters *parameters);
	OpcodeRet curAction(OpcodeParameters *parameters);

	ActionIndex getCurrentAction() { return _action; }
	void setCurrentAction(ActionIndex action) { _action = action; }

private:
	LiathEngine* _engine;

	ActionIndex     _action;
	ActionIndex     _previousAction;

	Common::String _aviFilename;
	byte _aviType;
	int16 _avsX;
	int16 _avsY;
};

} // End of namespace Liath

#endif // LIATH_ACTION_H
