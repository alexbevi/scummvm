#include "innocent/value.h"

#include "common/rect.h"

#include "innocent/inter.h"

namespace Innocent {
//

Value &WordVariable::operator=(uint16 value) {
	debugC(1, kDebugLevelValues, "setting %s to %d", +*this, value);
	WRITE_LE_UINT16(_ptr, value); return *this;
}

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

template<>
uint16 &CodePointer::field<uint16>(uint16 &p, int off) const {
	p = READ_LE_UINT16(code() + off);
	return p;
}

template<>
int16 &CodePointer::field<int16>(int16 &p, int off) const {
	uint16 z;
	field(z, off);
	p = *reinterpret_cast<int16*>(&z);
	return p;
}

template<>
Common::Point &CodePointer::field<Common::Point>(Common::Point &p, int off) const {
	field(p.x, off);
	field(p.y, off + 2);
	return p;
}

template<>
byte &CodePointer::field<byte>(byte &p, int off) const {
	p = *(code() + off);
	return p;
}

template<>
bool &CodePointer::field<bool>(bool &p, int off) const {
	byte b;
	field(b, off);
	return p = b;
}

} // of namespace Innocent
