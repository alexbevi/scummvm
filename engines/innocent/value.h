#ifndef INNOCENT_VARIABLES_H
#define INNOCENT_VARIABLES_H

#include <vector>

#include "config.h"

#include "innocent/debug.h"

namespace Innocent {

enum ValueType {
	kValueVoid,
	kValueConstant
};

class Value : public Inspectable<uint16> {
public:
	virtual ~Value() {}
	virtual ValueType type() const { return kValueVoid; }

	virtual operator uint16() const { assert(false); }
	virtual Value &operator=(byte value) { assert(false); }

	Value() {}

private:
	Value(Value &) : Inspectable<uint16>() { assert(false); } // no copying
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
	virtual Value &operator=(byte value) { *_ptr = value; return *this; }
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

}

#endif
