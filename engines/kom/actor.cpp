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
#include "common/stream.h"
#include "common/util.h"

#include "kom/kom.h"
#include "kom/actor.h"
#include "kom/screen.h"

using Common::File;
using Common::MemoryReadStream;
using Common::String;

namespace Kom {

ActorManager::ActorManager(KomEngine *vm) : _vm(vm) {

}

ActorManager::~ActorManager() {
	unloadAll();
}

int ActorManager::load(FilesystemNode dirNode, String name) {

	Actor *act = new Actor(_vm, dirNode, name);
	_actors.push_back(act);

	return _actors.size() - 1;
}

void ActorManager::displayAll() {
	Common::Array<Actor *>::iterator act;

	for (act = _actors.begin(); act != _actors.end(); act++) {
		if (*act != NULL && (*act)->_isActive) {
			(*act)->display();
		}
	}
}

Actor::Actor(KomEngine *vm, FilesystemNode dirNode, String name) : _vm(vm) {
	File f;
	char magicName[8];

	_scope = 255;
	_isAnimating = false;
	_isActive = true;
	_currentFrame = _minFrame = _maxFrame = 0;
	_xRatio = _yRatio = 1024;
	_displayLeft = _displayRight = _displayTop = _displayBottom = 1000;

	f.open(dirNode.getChild(name + ".act"));

	f.read(magicName, 7);
	magicName[7] = '\0';
	assert(strcmp(magicName, "DCB_ACT") == 0);

	_name = name;
	_isPlayerControlled = !f.readByte();
	_framesNum = f.readByte();

	f.seek(10);
	_framesDataSize = f.size() - f.pos();
	_framesData = new byte[_framesDataSize];
	f.read(_framesData, _framesDataSize);

	f.close();
}

Actor::~Actor() {
	delete[] _framesData;
}

void Actor::defineScope(uint8 scopeId, uint8 minFrame, uint8 maxFrame, uint8 startFrame) {
	assert(scopeId < 8);

	_scopes[scopeId].minFrame = minFrame;
	_scopes[scopeId].maxFrame = maxFrame;
	_scopes[scopeId].startFrame = startFrame;
}

void Actor::defineScopeAlias(uint8 scopeId, const uint8 *aliasData, uint8 length) {
	assert(scopeId < 8);

	_scopes[scopeId].minFrame = 0;
	_scopes[scopeId].maxFrame = length - 1;
	_scopes[scopeId].aliasData = aliasData;
	_scopes[scopeId].startFrame = 0;
}

void Actor::setScope(uint8 scopeId, uint16 animDuration) {
	assert(scopeId < 8);

	_scope = scopeId;

	// TODO: figure out why minFrame is abs()-ed in the original
	setAnim(_scopes[_scope].minFrame, _scopes[_scope].maxFrame, animDuration);
	_currentFrame = _scopes[_scope].startFrame;
}

void Actor::setAnim(uint8 minFrame, uint8 maxFrame, uint16 animDuration) {
	if (minFrame <= maxFrame) {
		_minFrame = minFrame;
		_maxFrame = maxFrame;
		_reverseAnim = false;
	} else {
		_minFrame = maxFrame;
		_maxFrame = minFrame;
		_reverseAnim = true;
	}

	if (_currentFrame < 255) {
		_animDurationTimer = _animDuration = animDuration;
	}

	_isAnimating = true;
}

void Actor::doAnim() {
	if (_animDuration == 0 || !_isAnimating)
		return;

	_animDurationTimer--;
	if (_animDurationTimer != 0)
		return ;

	_animDurationTimer = _animDuration;

	if (_reverseAnim) {
		_currentFrame--;
		if (_currentFrame <= _minFrame)
			_currentFrame = _maxFrame;
	} else {
		_currentFrame++;
		if (_currentFrame > _maxFrame)
			_currentFrame = _minFrame;
	}
}

void Actor::display() {
	uint8 frame;
	uint32 offset;
	uint16 width, height;
	uint16 xOffset, yOffset;

	doAnim();

	// Handle scope alias
	if (_scope != 255 && _scopes[_scope].aliasData != NULL)
		frame = _scopes[_scope].aliasData[_currentFrame];
	else
		frame = _currentFrame;

	MemoryReadStream frameStream(_framesData, _framesDataSize);

	frameStream.seek(frame * 4);
	offset = frameStream.readUint32LE() - 10;

	frameStream.seek(offset);
	width = frameStream.readUint16LE();
	height = frameStream.readUint16LE();

	if (width == 0 || height == 0)
		return;

	// TODO - handle scaling and modify x/y pos, width/height accordingly
	xOffset = frameStream.readSint16LE();
	yOffset = frameStream.readSint16LE();

	if (_isPlayerControlled) {
		error("TODO: display player-controlled actors");
	} else {
		// TODO:
		// * more drawing functions for effects
		// * more arguments for scaling
		// * displayMask

		_vm->screen()->drawActorFrame((int8 *)(_framesData + frameStream.pos()),
		                                      width, height, _xPos, _yPos, xOffset, yOffset);
	}
}

const uint8 Actor::_exitCursorAnimation[] = {
	5, 6, 7, 8, 9, 10, 11, 12, 13, 12, 11, 10, 9, 8, 7, 6
};

const uint8 Actor::_inventoryCursorAnimation[] = {
	21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 21, 21, 21
};

} // End of namespace Kom
