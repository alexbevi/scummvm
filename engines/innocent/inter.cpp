#include "innocent/inter.h"

#include "common/endian.h"
#include "common/util.h"

#include "innocent/logic.h"

namespace Innocent {

enum Debug {
	kOpcodeDetails = 3
};

enum {
	kOpcodeMax = 0xfd
};

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
		_logic(l) {
}

void Interpreter::run(byte *code, uint16 mode) {
	_code = code;
	_mode = mode;
	run();
}

void Interpreter::run() {
	uint16 abort_exec0 = 0, abort_exec1 = 0;

	_errorCount = 0;

	while (_logic->_status == Logic::kStatusOk) {
		if (_logic->_status != Logic::kStatusOk)
			return;

		byte opcode = *_code;
		if (opcode > kOpcodeMax) {
			_logic->_status = Logic::kInvalidOpcode;
			return;
		}

		uint8 nargs = _argumentsCounts[opcode];

		_currentCode = opcode;
		OpcodeHandler handler = _handlers[opcode];
		if (!handler)
			handler = &Interpreter::defaultHandler;

		Argument *args[6];

		for (uint i = 0; i < nargs; i++)
			args[i] = getArgument();

		if (nargs == 0)
			_code += 2;

		if (opcode == 0x2c || opcode == 0x2d || opcode == 1 || !_errorCount)
			(*handler)(this, args);
		else if (opcode != 0 && opcode < 0x26)
			_errorCount++;

		for (int i = 0; i < nargs; i++)
			delete args[i];

		if (abort_exec0 || abort_exec1)
			break;
	}
}

void Interpreter::forgetLastError() {
	if (_errorCount) _errorCount--;
	debug(kOpcodeDetails, "forgotten last error, count now %d", _errorCount);
}

void Interpreter::defaultHandler(Interpreter *self, Argument /*args*/*[]) {
	warning("unhandled opcode %d [=0x%02x]", self->_currentCode, self->_currentCode);
}

enum ArgumentTypes {
	kArgumentImmediate = 1,
	kArgumentMainWord = 2,
	kArgumentMainByte = 3
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
	Argument *arg = new ByteArgument(_logic->getGlobalByteVar(index));
	debug(kOpcodeDetails, "byte wide variable in main, index 0x%04x, value 0x%02x", index, byte(*arg));
	return arg;
}

Argument *Interpreter::readMainWordArg() {
	uint16 offset = READ_LE_UINT16(_code);
	_code += 2;
	Argument *arg = new Uint16Argument(_logic->getGlobalWordVar(offset/2));
	debug(kOpcodeDetails, "word wide variable in main, index 0x%04x, value 0x%04x", offset/2, uint16(*arg));
	return arg;
}

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
		default:
			error("don't know how to handle argument type 0x%02x", argument_type);
	}
}

const uint8 Interpreter::_argumentsCounts[] = {
	#include "opcodes_nargs.data"
};

} // End of namespace Innocent
