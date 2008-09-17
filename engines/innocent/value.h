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

#ifndef INNOCENT_VARIABLES_H
#define INNOCENT_VARIABLES_H

#include <vector>

#include "common/endian.h"
#include "config.h"

#include "innocent/debug.h"

namespace Innocent {
//

enum OpcodeMode {
	kCodeInitial = 0,
	kCodeNewRoom = 1,
	kCodeRoomLoop = 2,
	kCodeItem = 4,
	kCodeNewBlock = 8
};

class Interpreter;

enum ValueType {
	kValueVoid,
	kValueConstant
};

class Value : public NumericInspectable<uint16> {
public:
	virtual ~Value() {}
	virtual ValueType type() const { return kValueVoid; }

	virtual operator uint16() const { assert(false); }
	virtual int16 signd() const { const uint16 v = *this; return *reinterpret_cast<const int16 *>(&v); }
	virtual Value &operator=(uint16 value) { assert(false); }
	virtual Value &operator=(const Value &) { assert(false); }
	virtual bool operator==(const Value &other) { return uint16(*this) == other; }
	virtual bool operator<(const Value &other) { return uint16(*this) < other; }
	virtual bool operator>(const Value &other) { return other < *this; }
	virtual Value &operator++() { return *this = uint16(*this) + 1; }
	virtual uint16 operator++(int) { uint16 old = *this; ++*this; return old; }
	virtual Value &operator--() { return *this = uint16(*this) - 1; }
	virtual uint16 operator--(int) { uint16 old = *this; --*this; return old; }
	template<typename T>
	T operator-(T she) { T me = *this; return me - she; }

	virtual bool holdsCode() const { assert(false); }

	virtual operator byte *() { assert(false); }
	virtual operator const Common::String() {
		byte *b(*this);
		return Common::String(reinterpret_cast<const char *>(b));
	}

	Value() {}

private:
	explicit Value(Value &) : NumericInspectable<uint16>() { assert(false); } // no copying
};

class Constant : public Value {
public:
	Constant(uint16 value) : _value(value) {}
	virtual ~Constant() {}

	virtual operator uint16() const { return _value; }
	virtual ValueType type() const { return kValueConstant; }
private:
	uint16 _value;
};

class ByteVariable : public Value {
public:
	ByteVariable(byte *ptr) : _ptr(ptr) {}
	virtual Value &operator=(uint16 value) { assert(value < 256); *_ptr = value; return *this; }
	virtual operator uint16() const { return *_ptr; }
private:
	byte *_ptr;
};

class WordVariable : public Value {
public:
	WordVariable(byte *ptr) : _ptr(ptr) {}
	virtual operator uint16() const { return READ_LE_UINT16(_ptr); }
	virtual Value &operator=(uint16 value);
	virtual Value &operator=(const Value &other) { return *this = uint16(other); }
private:
	byte *_ptr;
};

class ValueVector {
public:
	~ValueVector() {
		for (std::vector<Value *>::iterator it = _values.begin(); it != _values.end(); ++it)
			/*delete *it*/; // FIXME segfaults, why?
	}
	void push_back(Value *element) { _values.push_back(element); }
	Value &operator[](uint8 idx) { return *_values[idx]; }
private:
	std::vector<Value *> _values;
};

class CodePointer : public Value {
public:
	CodePointer() : _interpreter(0) {}
	CodePointer(const CodePointer &c) : Value(), _offset(c._offset), _interpreter(c._interpreter) { init(); }
	CodePointer(uint16 offset, Interpreter *interpreter);

	CodePointer &operator=(const CodePointer &cp) { _offset = cp._offset; _interpreter = cp._interpreter; return *this; }

	virtual const char *operator+() const { return _inspect; }
	virtual void run() const;
	virtual void run(OpcodeMode mode) const;
	uint16 offset() const { return _offset; }
	byte *base() const;
	virtual operator uint16() const { return _offset; }
	virtual bool holdsCode() const { return true; }
	virtual byte *code() const;
	Interpreter *interpreter() const { return _interpreter; }
	bool isEmpty() const { return _interpreter == 0; }
	void reset() { _interpreter = 0; }

	template<typename T> T &field(T &, int) const;

private:
	void init();
	char _inspect[40];
	uint16 _offset;
	Interpreter *_interpreter;
};

template<typename Enum, int N>
struct EnumName {
static const char *name() {
	assert(false);
	return 0;
}

static const char *findName(Enum a){
	if (N == a)
		return name();
	else
		return EnumName<Enum, N-1>::findName(a);
}
};

template<typename Enum>
struct EnumName<Enum, -1> {
static const char *findName(Enum a) {
	assert(false);
	return 0;
}
};


template<typename Enum>
class EnumPack : public Inspectable {
public:
	EnumPack() {}
	EnumPack(Enum a) : _a(a) {  }
	const char *operator+() const { return EnumName<Enum,40>::findName(Enum(_a)); }
	operator Enum() const { return _a; }
private:
	Enum _a;
};

#define ENAME(en, v, s) template<> const char *EnumName<en, v>::name() { return s; } enum {}

}

#endif
