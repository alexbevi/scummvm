#ifndef INNOCENT_INTER_H
#define INNOCENT_INTER_H

#include <memory>

#include "config.h"

namespace Innocent {

class Logic;
class Opcode;

class Interpreter;

class OpcodeFactory {
public:
	OpcodeFactory(Interpreter *i);
	const Opcode *operator[](byte code) const;

private:
	Interpreter *_interpreter;
	mutable std::auto_ptr<Opcode> _opcodes[0xFE];
};

class Interpreter {
public:
	Interpreter(Logic *l);

	void run(const byte *code, uint16 mode);

	friend class Opcode;

private:
	Logic *_logic;
	OpcodeFactory _opcodes;
};

typedef int Argument; // TODO

class Opcode {
public:
	void operator()(const Argument args[]) const;
	uint8 argumentsCount() const { return _argumentsCount; }

private:
	Opcode();
	friend class OpcodeFactory;

	void(*_handler)(const Argument args[]);
	uint8 _argumentsCount;
	Interpreter *_interpreter;
};

} // End of namespace Innocent

#endif // INNOCENT_INTER_H
