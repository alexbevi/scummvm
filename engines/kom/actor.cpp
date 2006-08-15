/* ScummVM - Scumm Interpreter
 * Copyright (C) 2005-2006 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#include <stdio.h>
#include "common/file.h"
#include "common/fs.h"
#include "common/array.h"

#include "kom/kom.h"
#include "kom/actor.h"

using Common::File;

namespace Kom {

Common::Array<Actor *> Actor::_actors;

int Actor::load(FilesystemNode fsNode) {
	File f;
	char magicName[8];

	f.open(fsNode);

	f.read(magicName, 7);
	magicName[7] = '\0';
	assert(strcmp(magicName, "DCB_ACT") == 0);

	Actor *act = new Actor();
	_actors.push_back(act);

	act->_isPlayerControlled = f.readByte();
	act->_framesNum = f.readByte();

	int framesDataSize = f.size() - f.pos();

	act->_framesData = new byte[framesDataSize];

	f.read(act->_framesData, framesDataSize);

	return _actors.size() - 1;
}

Actor::Actor() {

}

Actor::~Actor() {
	delete[] _framesData;
}

} // End of namespace Kom
