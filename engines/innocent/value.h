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
	kCodeNewBlock = 8
};

class Interpreter;

enum ValueType {
	kValueVoid,
	kValueConstant
};

class Value : public Inspectable<uint16> {
public:
	virtual ~Value() {}
	virtual ValueType type() const { return kValueVoid; }

	virtual operator uint16() const { assert(false); }
	virtual Value &operator=(uint16 value) { assert(false); }
	virtual Value &operator=(const Value &) { assert(false); }

	virtual bool holdsCode() const { assert(false); }

	virtual operator byte *() { assert(false); }

	Value() {}

private:
	explicit Value(Value &) : Inspectable<uint16>() { assert(false); } // no copying
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
	CodePointer(const CodePointer &c) : Value(), _offset(c._offset), _interpreter(c._interpreter) { init(); }
	CodePointer(uint16 offset, Interpreter *interpreter);
	virtual const char *operator+() const { return _inspect; }
	virtual void run() const;
	virtual void run(OpcodeMode mode) const;
	uint16 offset() const { return _offset; }
	virtual bool holdsCode() const { return true; }
	virtual byte *code() const;
	Interpreter *interpreter() const { return _interpreter; }
private:
	void init();
	char _inspect[40];
	uint16 _offset;
	Interpreter *_interpreter;
};

}

#endif
