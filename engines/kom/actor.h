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
	Scope() : aliasData(NULL), firstFrame(0) {}
	~Scope() { delete[] aliasData; }
	int firstFrame;
	int lastFrame;
	int ringFrame;
	byte* aliasData;
};

class Actor {

friend class ActorManager;

public:
	~Actor();

	void defineScope(uint8 scopeId, uint8 firstFrame, uint8 lastFrame, uint8 ringFrame);
	void setScope(uint8 scopeId, int animSpeed);
	void setAnim(uint8 firstFrame, uint8 lastFrame, int animSpeed);
	void doAnim();
	void display();

	void setXPos(int xPos) { _xPos = xPos; }
	void setYPos(int yPos) { _yPos = yPos; }

protected:
	Actor();

	Common::String _name;
	uint8 _framesNum;
	byte _isPlayerControlled;
	uint8 _frame;
	uint8 _animFirstFrame;
	uint8 _animLastFrame;
	int _animDurationTimer;
	int _animDuration;
	bool _reverseAnim;
	int _xPos;
	int _yPos;
	uint16 _xRatio;
	uint16 _yRatio;
	int _maskDepth;
	int _depth;
	bool _isAnimating;
	int _pausedAnimFrame;
	int _data1;
	uint8 _scope;
	int _effect;
	int _isUsed;
	Scope _scopes[8];

	byte *_framesData;
	int _framesDataSize;
};

class ActorManager {
public:

	ActorManager(KomEngine *vm);
	~ActorManager();
	int load(FilesystemNode dirNode, Common::String name);
	Actor *get(int idx) { return _actors[idx]; }
	void unload(int idx) {  }
	void unloadAll() { _actors.clear(); }
	void displayAll();

private:

	KomEngine *_vm;
	Common::Array<Actor *> _actors;
};

} // End of namespace Kom

#endif
