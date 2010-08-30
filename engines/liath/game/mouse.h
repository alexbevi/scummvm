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

#ifndef LIATH_MOUSEMANAGER_H
#define LIATH_MOUSEMANAGER_H

#include "liath/shared.h"

#include "common/array.h"

namespace Liath {

class LiathEngine;

class MouseManager {
public:

	MouseManager(LiathEngine *engine);
	~MouseManager();

	// Opcodes
	OpcodeRet commandNumberBox(OpcodeParameters *parameters);
	OpcodeRet push();
	OpcodeRet pop();
	OpcodeRet type(OpcodeParameters *parameters);
	OpcodeRet reset(OpcodeParameters *parameters);
	OpcodeRet key(OpcodeParameters *parameters);
	OpcodeRet press(OpcodeParameters *parameters);
	OpcodeRet scroll(OpcodeParameters *parameters);
	OpcodeRet check(OpcodeParameters *parameters);
	OpcodeRet addBox(OpcodeParameters *parameters);
	OpcodeRet removeBox(OpcodeParameters *parameters);

private:
	LiathEngine* _engine;

	int32 _count;
	bool _pressMouse;

	Common::Array<MouseBox *> _boxes;

	uint32 numberBox(uint32 param1, uint32 param2);
};

} // End of namespace Liath

#endif // LIATH_MOUSEMANAGER_H
