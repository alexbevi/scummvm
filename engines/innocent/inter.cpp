#include "innocent/inter.h"

#include "common/util.h"

#include "innocent/logic.h"

namespace Innocent {

enum {
	kOpcodeMax = 0xfd
};

Interpreter::Interpreter(Logic *l) :
		_logic(l),
		_opcodes(this) {
}

void Interpreter::run(const byte *code, uint16 mode) {
	if (_logic->_status != Logic::kStatusOk)
		return;

	byte opcode = *code;
	if (opcode > kOpcodeMax) {
		_logic->_status = Logic::kInvalidOpcode;
		return;
	}

	const Opcode *operation = _opcodes[opcode];
	(void) operation;
}

const uint8 OpcodeFactory::_argumentsCounts[] = {
	#include "opcodes_nargs.data"
};

OpcodeFactory::OpcodeFactory(Interpreter *i) : _interpreter(i) {}

static void default_handler(const Opcode *self, const Argument /*args*/[]) {
	warning("unhandled opcode %02x", self->code());
}

const Opcode *OpcodeFactory::operator[](byte code) const {
	if (!_opcodes[code].get()) {
		Opcode * op = new Opcode;
		op->_code = code;
		op->_handler = _handlers[code];
		if (!op->_handler)
			op->_handler = default_handler;
		op->_argumentsCount = _argumentsCounts[code];
		_opcodes[code].reset(op);
	}

	return _opcodes[code].get();
}

void Opcode::operator()(const Argument args[]) const {
	_handler(this, args);
}

} // End of namespace Innocent
