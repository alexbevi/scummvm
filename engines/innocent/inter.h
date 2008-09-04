#ifndef INNOCENT_INTER_H
#define INNOCENT_INTER_H

#include <memory>

#include "config.h"

namespace Innocent {

class Logic;
class Opcode;
typedef int Argument; // TODO


class Interpreter;

class OpcodeFactory {
public:
	OpcodeFactory(Interpreter *i);
	const Opcode *operator[](byte code) const;

private:
	Interpreter *_interpreter;
	mutable std::auto_ptr<Opcode> _opcodes[0xFE];
	static const uint8 _argumentsCounts[];
	static void(**_handlers)(const Opcode *self, const Argument args[]);
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

class Opcode {
public:
	void operator()(const Argument args[]) const;
	uint8 argumentsCount() const { return _argumentsCount; }
	uint8 code() const { return _code; }

private:
	Opcode();
	friend class OpcodeFactory;

	void(*_handler)(const Opcode *self, const Argument args[]);
	uint8 _argumentsCount;
	Interpreter *_interpreter;
	uint8 _code; // for debug purposes
};

} // End of namespace Innocent

#endif // INNOCENT_INTER_H
