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

#include "innocent/value.h"

#include "common/rect.h"

#include "innocent/inter.h"
#include "innocent/util.h"

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
	if (_offset)
		snprintf(_inspect, 40, "code offset 0x%04x of %s", _offset, _interpreter->name());
	else
		snprintf(_inspect, 40, "null pointer");

}

void CodePointer::run() const {
	if (_offset && _interpreter)
		_interpreter->run(_offset);
}

void CodePointer::run(OpcodeMode mode) const {
	if (_offset)
		_interpreter->run(_offset, mode);
}

byte *CodePointer::code() const {
	return _interpreter->rawCode(_offset);
}

byte *CodePointer::base() const {
	return _interpreter->rawCode(0);
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

enum Foo_ {
	Bar
};

#define ENAME(en, v, s) template<> const char *EnumName<en, v>::name() { return s; } enum {}

ENAME(Foo_, Bar, "baz");

typedef EnumPack<Foo_> Foo;

Foo baz(Bar);

} // of namespace Innocent
