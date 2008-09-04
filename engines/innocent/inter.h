#ifndef INNOCENT_INTER_H
#define INNOCENT_INTER_H

#include <memory>

#include "config.h"

namespace Innocent {

class Logic;
class Opcode;

class Argument {
};

class Uint16Argument : public Argument {
public:
	Uint16Argument(byte *ptr) : _ptr(ptr) {}

private:
	byte *_ptr;
};

class Interpreter {
public:
	Interpreter(Logic *l);

	/**
	 * Run bytecode.
	 * @param code a Common::ReadStream pointing to code. The interpreter takes ownership of it.
	 * @param mode interpreting mode.
	 */
	void run(byte *code, uint16 mode);
	void run();

	Argument getArgument();

	friend class Opcode;

	void defaultHandler(const Argument args[]);

	static const uint8 _argumentsCounts[];

	typedef void(Innocent::Interpreter::*OpcodeHandler)(const Argument args[]);
	static OpcodeHandler _handlers[];

	Logic *_logic;

private:
	uint8 _currentCode; // for error reporting
	byte *_code;
	uint16 _mode;
};

} // End of namespace Innocent

#endif // INNOCENT_INTER_H
