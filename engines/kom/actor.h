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
	Scope() : aliasData(NULL), minFrame(0) {}
	~Scope() { delete[] aliasData; }
	int minFrame;
	int maxFrame;
	int startFrame;
	byte* aliasData;
};

class Actor {

friend class ActorManager;

public:
	~Actor();

	void defineScope(uint8 scopeId, uint8 minFrame, uint8 maxFrame, uint8 startFrame);
	void setScope(uint8 scopeId, int animSpeed);
	void setAnim(uint8 minFrame, uint8 maxFrame, int animSpeed);
	void doAnim();
	void display();

	void setXPos(int xPos) { _xPos = xPos; }
	void setYPos(int yPos) { _yPos = yPos; }

protected:
	Actor(KomEngine *vm);

	Common::String _name;
	uint8 _framesNum;
	byte _isPlayerControlled;
	uint8 _currentFrame;
	uint8 _minFrame;
	uint8 _maxFrame;
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
	int _targetX;
	int _targetY;
	uint8 _scope;
	int _effect;
	int _isActive;
	uint32 _displayLeft;
	uint32 _displayRight;
	uint32 _displayTop;
	uint32 _displayBottom;
	Scope _scopes[8];

	byte *_framesData;
	int _framesDataSize;

	KomEngine *_vm;
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
