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
	if (_logic->_status != Logic::kStatusOk)
		return;

	byte opcode = *code;
	if (opcode > kOpcodeMax) {
		_logic->_status = Logic::kInvalidOpcode;
		return;
	}

//	uint8 nargs = _argumentsCounts[opcode];

	_currentCode = *code;
	OpcodeHandler handler = _handlers[opcode];
	if (!handler)
		handler = &Interpreter::defaultHandler;

	Argument args[6];
	(this->*handler)(args);
}

void Interpreter::defaultHandler(const Argument /*args*/[]) {
	warning("unhandled opcode %02x", _currentCode);
}

const uint8 Interpreter::_argumentsCounts[] = {
	#include "opcodes_nargs.data"
};

} // End of namespace Innocent
