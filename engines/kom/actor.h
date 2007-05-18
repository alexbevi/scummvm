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
#include "common/str.h"
#include "common/array.h"

namespace Kom {

struct Scope {
	Scope() : aliasData(NULL), minFrame(0), maxFrame(0) {}
	~Scope() { delete[] aliasData; }
	uint8 minFrame;
	uint8 maxFrame;
	uint8 startFrame;
	const uint8* aliasData;
};

class KomEngine;

class Actor {

friend class ActorManager;

public:
	~Actor();

	void defineScope(uint8 scopeId, uint8 minFrame, uint8 maxFrame, uint8 startFrame);

	/**
	 * Allows a hard-coded frame sequence. It is only used for the exit/inventory cursor
	 * animations which repeat the edge frames frames several times and play the frames
	 * forward and then in reverse
	 */
	void defineScopeAlias(uint8 scopeId, const uint8 *aliasData, uint8 length);

	void setScope(uint8 scopeId, uint16 animDuration);
	void switchScope(uint8 scopeId, uint16 animDuration);
	void setAnim(uint8 minFrame, uint8 maxFrame, uint16 animDuration);
	void animate();
	void display();

	void enable(bool state) { _isActive = state; }
	void setPos(int xPos, int yPos) { _xPos = xPos; _yPos = yPos; }
	void setMaskDepth(int depth) { _maskDepth = depth; }
	void setEffect(uint8 effect) { _effect = effect; }
	int getXPos() { return _xPos; }
	int getYPos() { return _yPos; }
	uint8 getFramesNum() { return _framesNum; }

protected:
	Actor(KomEngine *vm, FilesystemNode dirNode, Common::String name, bool isMouse);

	Common::String _name;
	uint8 _framesNum;
	byte _isPlayerControlled;
	uint8 _currentFrame;
	uint8 _minFrame;
	uint8 _maxFrame;
	uint16 _animDurationTimer;
	uint16 _animDuration;
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
	uint8 _effect;
	int _isActive;
	uint32 _displayLeft;
	uint32 _displayRight;
	uint32 _displayTop;
	uint32 _displayBottom;
	Scope _scopes[8];

private:

	byte *_framesData;
	int _framesDataSize;

	KomEngine *_vm;

	bool _isMouse;

	static const uint8 _exitCursorAnimation[];
	static const uint8 _inventoryCursorAnimation[];
};

class ActorManager {
public:

	ActorManager(KomEngine *vm);
	~ActorManager();
	int load(FilesystemNode dirNode, Common::String name);
	void loadMouse(FilesystemNode dirNode, Common::String name);
	Actor *get(int idx) { return _actors[idx]; }
	Actor *getMouse() { return _mouseActor; }
	void unload(int idx) { delete _actors[idx]; _actors[idx] = 0; }
	void unloadAll() { _actors.clear(); }
	void displayAll();

private:

	KomEngine *_vm;
	Common::Array<Actor *> _actors;
	Actor *_mouseActor;
};

} // End of namespace Kom

#endif
