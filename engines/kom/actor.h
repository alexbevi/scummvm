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

#ifndef KOM_ACTOR_H
#define KOM_ACTOR_H

#include "common/scummsys.h"
#include "common/fs.h"
#include "common/array.h"

namespace Kom {

struct Scope {
	Scope() : aliasData(NULL) {}
	~Scope() { delete[] aliasData; }
	int initialFrame;
	int lastFrame;
	int ringFrame;
	byte* aliasData;
};

class Actor {
public:
	static int load(FilesystemNode fsNode);
	static Actor *getActor(int idx) { return _actors[idx]; }
	static void unload(int idx) {  }
	static void unloadAll() { _actors.clear(); }
	~Actor();


private:
	Actor();

	static Common::Array<Actor *> _actors;

	byte _framesNum;
	byte _isPlayerControlled;
	byte _frame;
	byte _animFrame;
	byte _animLastFrame;
	int _animSpeed1;
	int _animSpeed2;
	int _xPos;
	int _yPos;
	int _maskDepth;
	int _depth;
	bool _isAnimating;
	int _pausedAnimFrame;
	int _data1;
	int _scope;
	int _effect;
	int _isUsed;
	Scope _scopes[8];

	byte *_framesData;
};

} // End of namespace Kom

#endif
