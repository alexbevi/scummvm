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

#ifndef INNOCENT_INTER_H
#define INNOCENT_INTER_H

#include <memory>

#include "common/list.h"
#include "common/rect.h"
#include "common/util.h"
#include "config.h"

#include "innocent/value.h"

namespace Innocent {

class Animation;
class Logic;
class Opcode;
class Engine;
class Resources;
class Graphics;

#define UNIMPLEMENTED { error("type conversion unimplemented"); }

enum Status {
	kReturned = 0,
	kInvalidOpcode = 1
};

class Interpreter {
private:
	enum OpResultCode {
		kThxBye,
		kReturn,
		kFail,
		kElse,
		kEndIf,
		kJump
	};
	struct OpResult {
		OpResult(OpResultCode c) : code(c) {}
		OpResult(const CodePointer &p) : code(kJump), address(p) {}
		OpResultCode code;
		CodePointer address;
	};

public:
	Interpreter(Logic *l, byte *base, const char *name);
	~Interpreter();

	void init();

	/**
	 * Run bytecode.
	 * @param code a Common::ReadStream pointing to code. The interpreter takes ownership of it.
	 * @param mode interpreting mode.
	 */
	Status run(uint16 offset, OpcodeMode mode);
	void tick();
	void executeRestricted(byte *code);

	Value *getArgument(byte *&code);

	friend class Opcode;

	template <int opcode>
	OpResult opcodeHandler(ValueVector args, CodePointer current, CodePointer next);

	template <int N>
	void init_opcodes();

	typedef OpResult (Interpreter::*OpcodeHandler)(ValueVector args, CodePointer current, CodePointer next);
	OpcodeHandler _handlers[256];
	static const uint8 _argumentsCounts[];

	Logic *_logic;

	const char *name() const { return _name; }

	byte *rawCode(uint16 offset) const { return _base + offset; }

	friend class CodePointer;

	Resources *resources() const { return _resources; }
	void rememberAnimation(Animation *anim) { _animations.push_back(anim); }

private:
	char _name[100];
	template<class T>
	T *readArgument(byte *&code);

	byte *_base;
	uint16 _mode;

	Status run(uint16 offset);

	void setRoomLoop(byte *code);

	byte *_roomLoop;

	Common::List<Animation *> _animations;

	Engine *_engine;
	Resources *_resources;
	Graphics *_graphics;
};

} // End of namespace Innocent

#endif // INNOCENT_INTER_H
