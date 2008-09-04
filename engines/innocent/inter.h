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

	friend class Opcode;

	void defaultHandler(const Argument args[]);

	static const uint8 _argumentsCounts[];
//	typedef void (*OpcodeHandler)(const Innocent::Argument*);
	typedef void(Innocent::Interpreter::*OpcodeHandler)(const Argument args[]);
	static OpcodeHandler _handlers[];

	Logic *_logic;

private:
	uint8 _currentCode; // for error reporting
};

} // End of namespace Innocent

#endif // INNOCENT_INTER_H
