#include "innocent/inter.h"

#include "common/endian.h"
#include "common/util.h"

#include "innocent/innocent.h"
#include "innocent/logic.h"
#include "innocent/program.h"
#include "innocent/resources.h"

#include "innocent/opcode_handlers.cpp"

namespace Innocent {

enum Debug {
	kOpcodeDetails = 3
};

enum {
	kOpcodeMax = 0xfd
};

template <int opcode>
void Interpreter::opcodeHandler(Argument *args[]){
	error("unhandled opcode %d [=0x%02x]", opcode, opcode);
}


template<int N>
void Interpreter::init_opcodes() {
	_handlers[N] = &Innocent::Interpreter::opcodeHandler<N>;
	init_opcodes<N-1>();
}

template<>
void Interpreter::init_opcodes<-1>() {}

class Uint16Argument : public Argument {
public:
	Uint16Argument(byte *ptr) : Argument(ptr) {}
	operator uint16() const { return READ_LE_UINT16(_ptr); }
	Argument operator=(uint16 value) { WRITE_LE_UINT16(_ptr, value); return *this; }
	operator byte() const { return READ_LE_UINT16(_ptr); }
	Argument operator=(byte value) { WRITE_LE_UINT16(_ptr, value); return *this; }
};

class ByteArgument : public Argument {
public:
	ByteArgument(byte *ptr) : Argument(ptr) {}
	operator byte() const { return *_ptr; }
	Argument operator=(byte b) { *_ptr = b; return *this; }
};

Interpreter::Interpreter(Logic *l, byte *base) :
		_logic(l),
		_engine(l->engine()),
		_resources(_engine->resources()),
		_base(base),
		_roomLoop(0)
		{
	init_opcodes<255>();
	init();
}

void Interpreter::setRoomLoop(byte *code) {
	_roomLoop = code;
}

void Interpreter::tick() {
 	for (Common::List<PeriodiCall>::iterator it = _periodiCalls.begin(); it != _periodiCalls.end(); ++it)
 		it->call();

	if (_roomLoop)
		run(_roomLoop - _base, kCodeRoomLoop);
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
	byte *code = _base + offset;
	_failedCondition = 0;
	_return = false;

	while (!_return) {
		byte opcode = *code;
		if (opcode > kOpcodeMax) {
			return kInvalidOpcode;
		}

		uint8 nargs = _argumentsCounts[opcode];
		debug(4, "opcode %02x with %d args", opcode, nargs);

		OpcodeHandler handler = _handlers[opcode];

		Argument *args[6];

		for (uint i = 0; i < nargs; i++)
			args[i] = getArgument(code);

		if (nargs == 0)
			code += 2;

		if (opcode == 0x2c || opcode == 0x2d || opcode == 1 || !_failedCondition)
			(this->*handler)(args);
		else {
			debug(4, "skipped, skip depth = %d", _failedCondition);
			if (opcode != 0 && opcode < 0x26)
				_failedCondition++;
		}
		debug(4, "opcode executed");

		for (int i = 0; i < nargs; i++)
			delete args[i];
	}

	return kReturned;
}

enum ArgumentTypes {
	kArgumentImmediate = 1,
	kArgumentMainWord = 2,
	kArgumentMainByte = 3,
	kArgumentString = 7,
	kArgumentLocal = 9
};

Argument *Interpreter::readImmediateArg(byte *&code) {
	debug(kOpcodeDetails, "immediate, value 0x%04x", READ_LE_UINT16(code));
	byte *ptr = code;
	code += 2;
	return new Uint16Argument(ptr);
}

Argument *Interpreter::readMainByteArg(byte *&code) {
	uint16 index = READ_LE_UINT16(code);
	code += 2;
	Argument *arg = new ByteArgument(_resources->getGlobalByteVariable(index));
	debug(kOpcodeDetails, "byte wide variable in main, index 0x%04x, value 0x%02x", index, byte(*arg));
	return arg;
}

Argument *Interpreter::readMainWordArg(byte *&code) {
	uint16 offset = READ_LE_UINT16(code);
	code += 2;
	Argument *arg = new Uint16Argument(_resources->getGlobalWordVariable(offset/2));
	debug(kOpcodeDetails, "word wide variable in main, index 0x%04x, value 0x%04x", offset/2, uint16(*arg));
	return arg;
}


Argument *Interpreter::readLocalArg(byte *&code) {
	uint16 offset = READ_LE_UINT16(code);
	code += 2;
	Argument *arg = new Uint16Argument(_base + offset);
	debug(kOpcodeDetails, "local variable, offset 0x%04x, value 0x%04x", offset, uint16(*arg));
	return arg;
}

Interpreter::StringArgument::StringArgument(byte *code, Resources *res) : Argument(code) {
	byte ch;
	byte *str = _translateBuf;
	uint16 offset, value;
	while ((ch = *(code++))) {
		switch (ch) {
		case kStringGlobalWord:
			offset = READ_LE_UINT16(code);
			code += 2;
			value = READ_LE_UINT16(res->getGlobalWordVariable(offset/2));
			str += snprintf(reinterpret_cast<char *>(str), _translateBuf - str, "%d", value);
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
		default:
			*(str++) = ch;
		}
	}
}

Argument *Interpreter::readStringArg(byte *&code) {
	Argument *arg = new StringArgument(code, _resources);

	// skip the string
	byte ch;

	debug(3, "string argument");

	bool displayed = false;

	do {
		ch = *(code++);
		// try string args len
		switch (ch) {
		case 9:
		case 7:
			code ++;
		case 6:
		case 10:
		case 11:
			code ++;
		case 14:
		case 3:
			code += 2;
			displayed = false;
		}

		if (ch == 5) {
			while (*(code++) != 0);
			code += 2;
			displayed = false;
			continue;
		}

		if (!displayed) {
			debug(4, "string part: %s", code - 1);
			displayed = true;
		}
	} while (ch != 0);

	return arg;
}

Argument *Interpreter::getArgument(byte *&code) {
	uint8 argument_type = code[1];
	code += 2;
	debug(kOpcodeDetails, "argument type %02x", argument_type);

	switch (argument_type) {
		case kArgumentImmediate:
			return readImmediateArg(code);
		case kArgumentMainWord:
			return readMainWordArg(code);
		case kArgumentMainByte:
			return readMainByteArg(code);
		case kArgumentString:
			return readStringArg(code);
		case kArgumentLocal:
			return readLocalArg(code);
		default:
			error("don't know how to handle argument type 0x%02x", argument_type);
	}
}

const uint8 Interpreter::_argumentsCounts[] = {
	#include "opcodes_nargs.data"
};

void Interpreter::failedCondition() {
	_failedCondition++;
	debug(2, "if() condition failed, skipping instructions (depth %d)", _failedCondition);
}

void Interpreter::endIf() {
	debug(2, "end if");
	if (_failedCondition) _failedCondition--;
}

void Interpreter::goBack() {
	debug(2, "ceeding control");
	_return = true;
}

void Interpreter::addPeriodiCall(byte *code) {
	debug(2, "added periodicall 0x%04x", code - _base);
	_periodiCalls.push_back(PeriodiCall(code));
}

PeriodiCall::PeriodiCall(byte *code) : _code(code), _zIndex(-1) {
	initializeHandlers<kCodesNumber-1>();
}

void PeriodiCall::call() {
	byte opcode = *_code;

	debug(3, "running periodicall opcode 0x%02x", opcode);
	if (!(opcode & 0x80)) {
		debug(3, "mask fail!");
		return;
	}
	opcode = ~opcode;
	debug(3, "transformed 0x%02x", opcode);
	if (opcode >= 0x27) {
		debug(3, "code fail!");
		return;
	}

	(this->*_handlers[opcode])();
}

template<int N>
void PeriodiCall::initializeHandlers() {
	_handlers[N] = &PeriodiCall::handle<N>;
	initializeHandlers<N-1>();
}

void PeriodiCall::setZIndex(int8 index) {
	debug(3, "setting z index to %d", index);
	_zIndex = index;
}

template<>
void PeriodiCall::initializeHandlers<-1>() {}

template<int N>
void PeriodiCall::handle() {
	error("unhandled periodicall code %d", N);
}

} // End of namespace Innocent
