#ifndef INNOCENT_INTER_H
#define INNOCENT_INTER_H

#include <memory>

#include "config.h"

namespace Innocent {

class Logic;
class Opcode;
typedef int Argument; // TODO

class Interpreter {
public:
	Interpreter(Logic *l);

	void run(const byte *code, uint16 mode);
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
	byte const *_code;
	uint8 _mode;
};

} // End of namespace Innocent

#endif // INNOCENT_INTER_H
