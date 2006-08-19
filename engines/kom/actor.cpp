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
	File f;
	char magicName[8];

	f.open(dirNode.getChild(name + ".act"));

	f.read(magicName, 7);
	magicName[7] = '\0';
	assert(strcmp(magicName, "DCB_ACT") == 0);

	Actor *act = new Actor(_vm);
	_actors.push_back(act);

	act->_name = name;

	act->_isPlayerControlled = !f.readByte();
	act->_framesNum = f.readByte();

	f.seek(10);
	act->_framesDataSize = f.size() - f.pos();

	act->_framesData = new byte[act->_framesDataSize];

	f.read(act->_framesData, act->_framesDataSize);

	f.close();

	return _actors.size() - 1;
}

void ActorManager::displayAll() {
	Common::Array<Actor *>::iterator act;

	for (act = _actors.begin(); act != _actors.end(); act++) {
		if (*act != NULL) {
			(*act)->display();
		}
	}
}

Actor::Actor(KomEngine *vm) : _vm(vm) {
	_scope = _frame = 255;
	_isAnimating = false;
	_xRatio = _yRatio = 1024;
	_frame = 0;
}

Actor::~Actor() {
	delete[] _framesData;
}

void Actor::defineScope(uint8 scopeId, uint8 firstFrame, uint8 lastFrame, uint8 ringFrame) {
	assert(scopeId <= 7);

	_scopes[scopeId].firstFrame = firstFrame;
	_scopes[scopeId].lastFrame = lastFrame;
	_scopes[scopeId].ringFrame = ringFrame;
}

void Actor::setScope(uint8 scopeId, int animSpeed) {
	assert(scopeId <= 7);

	_scope = scopeId;

	// TODO: figure out why firstFrame is abs()-ed in the original
	setAnim(_scopes[_scope].firstFrame, _scopes[_scope].lastFrame, animSpeed);
	_frame = _scopes[_scope].ringFrame;
}

void Actor::setAnim(uint8 firstFrame, uint8 lastFrame, int animSpeed) {
	if (firstFrame <= lastFrame) {
		_animFirstFrame = firstFrame;
		_animLastFrame = lastFrame;
		_reverseAnim = false;
	} else {
		_animFirstFrame = lastFrame;
		_animLastFrame = firstFrame;
		_reverseAnim = true;
	}

	if (_frame < 255) {
		_animDurationTimer = _animDuration = animSpeed;
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
		_frame--;
		if (_frame <= _animFirstFrame)
			_frame = _animLastFrame;
	} else {
		_frame++;
		if (_frame > _animLastFrame)
			_frame = _animFirstFrame;
	}
}

void Actor::display() {
	uint8 frame;
	uint32 offset;
	uint16 width, height;
	uint16 xPos = 0; // FIXME
	uint16 yPos = 0;

	doAnim();

	frame = _frame;

	// Handle scope alias
	if (_scope != 255 && _scopes[_scope].aliasData != NULL)
		frame = _scopes[_scope].aliasData[_frame];

	MemoryReadStream frameStream(_framesData, _framesDataSize);

	frameStream.seek(frame * 4);
	offset = frameStream.readUint32LE() - 10;

	frameStream.seek(offset);
	width = frameStream.readUint16LE();
	height = frameStream.readUint16LE();

	if (width == 0 || height == 0)
		return;

	// TODO - handle scaling and modify x/y pos, width/height accordingly
	frameStream.skip(4);
	frameStream.skip(height * 2);

	if (_isPlayerControlled) {
		error("TODO: display player-controlled actors");
	} else {
		// TODO:
		// * more drawing functions for effects
		// * more arguments for scaling
		// * displayMask

		_vm->screen()->drawActorFrame((int8 *)(_framesData + frameStream.pos()), width, height, xPos, yPos);
	}


}

} // End of namespace Kom
