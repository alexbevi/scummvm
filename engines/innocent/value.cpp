#include "innocent/value.h"

#include "innocent/inter.h"

namespace Innocent {

CodePointer::CodePointer(uint16 off, Interpreter *i) : _offset(off), _interpreter(i) {
	init();
}

void CodePointer::init() {
	snprintf(_inspect, 40, "code offset 0x%04x of %s", _offset, _interpreter->name());
}

void CodePointer::run() const {
	_interpreter->run(_offset);
}

void CodePointer::run(OpcodeMode mode) const {
	_interpreter->run(_offset, mode);
}

byte *CodePointer::code() const {
	return _interpreter->rawCode(_offset);
}

}
