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

Interpreter::Interpreter(Logic *l) :
		_logic(l),
		_engine(l->engine()),
		_resources(_engine->resources())
		{
	init_opcodes<255>();
}


/* mode:
0 - initialization,
1 - room handler,
8 - dataset init
*/

void Interpreter::init() {
	_graphics = _engine->graphics();
}

Status Interpreter::run(byte *code, OpcodeMode mode) {
	_code = code;
	_mode = mode;
	return run();
}

Status Interpreter::run() {
	_failedCondition = 0;
	_return = false;

	while (!_return) {
		byte opcode = *_code;
		if (opcode > kOpcodeMax) {
			return kInvalidOpcode;
		}

		uint8 nargs = _argumentsCounts[opcode];
		debug(4, "opcode %02x with %d args", opcode, nargs);

		OpcodeHandler handler = _handlers[opcode];

		Argument *args[6];

		for (uint i = 0; i < nargs; i++)
			args[i] = getArgument();

		if (nargs == 0)
			_code += 2;

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
	kArgumentLocal = 9
};

Argument *Interpreter::readImmediateArg() {
	debug(kOpcodeDetails, "immediate, value 0x%04x", READ_LE_UINT16(_code));
	byte *ptr = _code;
	_code += 2;
	return new Uint16Argument(ptr);
}

Argument *Interpreter::readMainByteArg() {
	uint16 index = READ_LE_UINT16(_code);
	_code += 2;
	Argument *arg = new ByteArgument(_resources->getGlobalByteVariable(index));
	debug(kOpcodeDetails, "byte wide variable in main, index 0x%04x, value 0x%02x", index, byte(*arg));
	return arg;
}

Argument *Interpreter::readMainWordArg() {
	uint16 offset = READ_LE_UINT16(_code);
	_code += 2;
	Argument *arg = new Uint16Argument(_resources->getGlobalWordVariable(offset/2));
	debug(kOpcodeDetails, "word wide variable in main, index 0x%04x, value 0x%04x", offset/2, uint16(*arg));
	return arg;
}


/*Argument *Interpreter::readLocalArg() {
	uint16 offset = READ_LE_UINT16(_code);
	_code += 2;
	Argument *arg = new Uint16Argument(_logic->roomScript()->localVariable(offset));
	debug(kOpcodeDetails, "local variable, offset 0x%04x, value 0x%04x", offset, uint16(*arg));
	return arg;
}
*/

Argument *Interpreter::getArgument() {
	uint8 argument_type = _code[1];
	_code += 2;
	debug(kOpcodeDetails, "argument type %02x", argument_type);

	switch (argument_type) {
		case kArgumentImmediate:
			return readImmediateArg();
		case kArgumentMainWord:
			return readMainWordArg();
		case kArgumentMainByte:
			return readMainByteArg();
		/*case kArgumentLocal:
			return readLocalArg();*/
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

} // End of namespace Innocent
