#ifndef INNOCENT_INTER_H
#define INNOCENT_INTER_H

#include <memory>

#include "common/util.h"
#include "config.h"

namespace Innocent {

class Logic;
class Opcode;

class Argument {
public:
	virtual ~Argument() {}
	virtual operator uint16() const { error("unimplemented attribute dereference"); }
	virtual operator byte() const { error("unimplemented attribute dereference"); }
	virtual Argument operator=(byte b) { error("unimplemented variable setting"); }
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

	Argument *getArgument();

	friend class Opcode;

	static void defaultHandler(Interpreter *interpreter, Argument *args[]);

	static const uint8 _argumentsCounts[];

	typedef void(*OpcodeHandler)(Interpreter *interpreter, Argument *args[]);
	static OpcodeHandler _handlers[];
	friend class OpcodeHandlers;

	Logic *_logic;

private:
	uint8 _currentCode; // for error reporting
	byte *_code;
	uint16 _mode;

	Argument *readImmediateArg();
	Argument *readMainByteArg();
};

} // End of namespace Innocent

#endif // INNOCENT_INTER_H
