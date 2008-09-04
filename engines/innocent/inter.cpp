#include "innocent/inter.h"

#include "common/util.h"

#include "innocent/logic.h"

namespace Innocent {

enum {
	kOpcodeMax = 0xfd
};

Interpreter::Interpreter(Logic *l) :
		_logic(l) {
}

void Interpreter::run(const byte *code, uint16 mode) {
	_code = code;
	_mode = mode;
	run();
}

void Interpreter::run() {
	if (_logic->_status != Logic::kStatusOk)
		return;

	byte opcode = *_code;
	if (opcode > kOpcodeMax) {
		_logic->_status = Logic::kInvalidOpcode;
		return;
	}

	uint8 nargs = _argumentsCounts[opcode];

	_currentCode = *_code;
	OpcodeHandler handler = _handlers[opcode];
	if (!handler)
		handler = &Interpreter::defaultHandler;

	Argument args[6];

	for (uint i = 0; i < nargs; i++)
		args[i] = getArgument();

	(this->*handler)(args);
}

void Interpreter::defaultHandler(const Argument /*args*/[]) {
	warning("unhandled opcode %02x", _currentCode);
}

Argument Interpreter::getArgument() {
	return Argument();
}

const uint8 Interpreter::_argumentsCounts[] = {
	#include "opcodes_nargs.data"
};

} // End of namespace Innocent
