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

#include "innocent/inter.h"

#include "common/endian.h"
#include "common/list.h"
#include "common/util.h"

#include "innocent/debugger.h"
#include "innocent/innocent.h"
#include "innocent/logic.h"
#include "innocent/program.h"
#include "innocent/resources.h"

// this has to be included here or else templates
// would get instantiated to the generic handler
#include "innocent/opcode_handlers.cpp"

namespace Innocent {

class Animation;

enum {
	kOpcodeMax = 0xfd
};

template <int opcode>
Interpreter::OpResult Interpreter::opcodeHandler(ValueVector args, CodePointer current, CodePointer next){
	error("unhandled opcode %d [=0x%02x] at %s", opcode, opcode, +current);
	return kThxBye;
}


template<int N>
void Interpreter::init_opcodes() {
	_handlers[N] = &Innocent::Interpreter::opcodeHandler<N>;
	init_opcodes<N-1>();
}

template<>
void Interpreter::init_opcodes<-1>() {}

Interpreter::Interpreter(Logic *l, byte *base, const char *n) :
		_logic(l),
		_engine(l->engine()),
		_resources(_engine->resources()),
		_base(base),
		_roomLoop(0)
		{
	init_opcodes<255>();
	strncpy(_name, n, 100);
	init();
}

Interpreter::~Interpreter() {
	foreach (Animation *, _animations)
		delete *it;
}

void Interpreter::tick() {
	if (_roomLoop)
		run(_roomLoop - _base, kCodeRoomLoop);
}

void Interpreter::setRoomLoop(byte *code) {
	_roomLoop = code;
}

/* mode:
0 - initialization,
1 - room handler,
8 - dataset init
*/

void Interpreter::init() {
	_graphics = _engine->graphics();
}

Status Interpreter::run(uint16 offset, OpcodeMode mode) {
	_mode = mode;
	return run(offset);
}

Status Interpreter::run(uint16 offset) {
	byte *last, *code;
	last = code = _base + offset;

	int if_depth = 0;
	forever {
		byte opcode = *code;
		last = code;

		if (opcode > kOpcodeMax) {
			return kInvalidOpcode;
		}

		uint8 nargs = _argumentsCounts[opcode];

		OpcodeHandler handler = _handlers[opcode];

		ValueVector args;

		for (uint i = 0; i < nargs; i++)
			args.push_back(getArgument(code));

		if (nargs == 0)
			code += 2;

		OpResult result(kThxBye);

		if (opcode == 0x2c || opcode == 0x2d || opcode == 1 || if_depth == 0) {
			Debug.opcodeStep();
			result = (this->*handler)(args, CodePointer(last - _base, this), CodePointer(code - _base, this));
		} else {
			debugC(3, kDebugLevelScript, "opcode 0x%02x skipped", opcode);
			if (opcode > 1 && opcode < 0x26)
				result = kFail;
		}

		switch (result.code) {
		case kReturn:
			return kReturned;
		case kFail:
			if_depth++;
			break;
		case kElse:
			if (if_depth == 1)
				if_depth = 0;
			break;
		case kEndIf:
			if_depth = MAX(if_depth - 1, 0);
			break;
		case kJump:
			code = _base + result.address;
		case kThxBye:
			// ok
			;
		}
	}

	return kReturned;
}

enum ArgumentTypes {
	kArgumentImmediate = 1,
	kArgumentMainWord = 2,
	kArgumentMainByte = 3,
	kArgumentString = 7,
	kArgumentCode = 9
};

template<>
Constant *Interpreter::readArgument<Constant>(byte *&code) {
	uint16 value = READ_LE_UINT16(code);
	code += 2;
	debugC(4, kDebugLevelScript, "read constant value %d as argument", value);
	return new Constant(value);
}

class GlobalByteVariable : public ByteVariable {
public:
	GlobalByteVariable(uint16 index, Resources *res) : ByteVariable(res->getGlobalByteVariable(index)), _index(index)  {}
	virtual const char *operator+() const {
		snprintf(_inspect, 27, "global byte variable %d [%d]", _index, byte(*this)); return _inspect;
	}
private:
	mutable char _inspect[27];
	const uint16 _index;
};

class GlobalWordVariable : public WordVariable {
public:
	GlobalWordVariable(uint16 index, Resources *res) : WordVariable(res->getGlobalWordVariable(index)), _index(index) {}
	virtual const char *operator+() const {
		snprintf(_inspect, 33, "global word variable %d [%d]", _index, uint16(*this)); return _inspect;
	}
private:
	mutable char _inspect[33];
	const uint16 _index;
};

template<>
GlobalByteVariable *Interpreter::readArgument<GlobalByteVariable>(byte *&code) {
	uint16 index = READ_LE_UINT16(code);
	code += 2;
	debugC(4, kDebugLevelScript, "read global byte variable %d as argument", index);
	return new GlobalByteVariable(index, _resources);
}

template<>
GlobalWordVariable *Interpreter::readArgument<GlobalWordVariable>(byte *&code) {
	uint16 index = READ_LE_UINT16(code) / 2;
	code += 2;
	debugC(4, kDebugLevelScript, "read global word variable %d as argument", index);
	return new GlobalWordVariable(index, _resources);
}

template<>
CodePointer *Interpreter::readArgument<CodePointer>(byte *&code) {
	uint16 offset = READ_LE_UINT16(code);
	code += 2;
	debugC(4, kDebugLevelScript, "read code offset 0x%04x as argument", offset);
	return new CodePointer(offset, this);
}

class ParametrizedString : public Value {
public:
	ParametrizedString(byte *translated, uint16 len) {
		memcpy(_translateBuf, translated, len);
		_length = len;
	}
	virtual const char *operator+() const {
		return reinterpret_cast<const char *>(_translateBuf);
	}
	virtual operator byte *() { return _translateBuf; }
	virtual operator uint16() const { return _length; }
private:
	byte _translateBuf[500];
	uint16 _length;
};

template<>
ParametrizedString *Interpreter::readArgument<ParametrizedString>(byte *&code) {
	byte translateBuf[500];
	byte ch;
	byte *str = translateBuf;
	uint16 offset, value;
	while ((ch = *(code++))) {
		assert(str - translateBuf < 500);
		switch (ch) {
		case 14:
		case kStringMove:
			*(str++) = ch;
			*(str++) = *(code++);
			*(str++) = *(code++);
			*(str++) = *(code++);
			*(str++) = *(code++);
			break;
		case kStringAdvance:
			*(str++) = ch;
			*(str++) = *(code++);
			break;
		case kStringGlobalWord:
			offset = READ_LE_UINT16(code);
			code += 2;
			value = READ_LE_UINT16(_resources->getGlobalWordVariable(offset/2));
			str += snprintf(reinterpret_cast<char *>(str), 10, "%d", value);
			break;
		case kStringSetColour:
			*(str++) = ch;
			*(str++) = *(code++);
			break;
		case kStringCountSpacesIf0:
		case kStringCountSpacesIf1:
			error("unhandled string special 0x%02x", ch);
			break;
		case kStringCountSpacesTerminate:
			break;
		case '\r':
			*(str++) = '\n';
			break;
		default:
			if (ch == 5) { // menu option
				*(str++) = ch;
				while ((*(str++) = *(code++)) != 0);
				*(str++) = *(code++);
				*(str++) = *(code++);
			} else
				*(str++) = ch;
		}
	}
	*str++ = 0;

	debugC(4, kDebugLevelScript, "read parametrized string '%s' as argument", translateBuf);

	return new ParametrizedString(translateBuf, str - translateBuf);
}

Value *Interpreter::getArgument(byte *&code) {
	uint8 argument_type = code[1];
	code += 2;

	switch (argument_type) {
		case kArgumentImmediate:
			return readArgument<Constant>(code);
		case kArgumentMainWord:
			return readArgument<GlobalWordVariable>(code);
		case kArgumentMainByte:
			return readArgument<GlobalByteVariable>(code);
		case kArgumentString:
			return readArgument<ParametrizedString>(code);
		case kArgumentCode:
			return readArgument<CodePointer>(code);
		default:
			error("don't know how to handle argument type 0x%02x", argument_type);
	}
}

const uint8 Interpreter::_argumentsCounts[] = {
	#include "opcodes_nargs.data"
};

} // End of namespace Innocent
