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

#ifndef INNOCENT_ANIMATION_H
#define INNOCENT_ANIMATION_H

#include <memory>

#include "common/list.h"
#include "common/rect.h"

#include "innocent/value.h"

namespace Innocent {
//

class Graphics;
class Resources;
class Sprite;

class Animation {
public:
	enum Status_ {
		kOk,
		kRemove,
		kFrameDone
	};

	typedef EnumPack<Status_> Status;

	Animation(const CodePointer &code, Common::Point position);
	virtual ~Animation();

	uint16 baseOffset() const { return _baseOffset; }

	virtual void paint(Graphics *g);
	virtual Status tick();

	void runOnNextFrame(const CodePointer &cp);

	virtual void setAnimation(uint16 anim) { _offset = anim; }

	virtual bool isActor() const { return false; }

protected:
	class Sprite;

	uint16 shift();
	int8 shiftByte();
	int8 embeddedByte() const;

	void setMainSprite(uint16 sprite);
	void clearMainSprite();
	void clearSprites();

	void handleTrigger();

	template <int opcode>
	Status opcodeHandler();

	template <int N>
	void init_opcodes();

	virtual Status op(byte code);

	Resources *_resources;
	int8 _interval;
	int16 _ticksLeft;
	int8 _zIndex;
	Common::Point _position;
	/** start of the animation code */
	byte *_base;
	/** current position  in the animation */
	uint16 _offset;
	char _debugInfo[50];
	Common::List<Sprite *> _sprites;
	std::auto_ptr<Innocent::Sprite> _mainSprite;
	int8 _counter;
	uint16 _loopStart;
	/** offset of the animation from the start of its codeblock */
	uint16 _baseOffset;
	CodePointer _frameTrigger;

	bool _debugInvalid;

private:
	typedef Status (Animation::*OpcodeHandler)();
	OpcodeHandler _handlers[38];
};

}

#endif
