#ifndef INNOCENT_INTER_H
#define INNOCENT_INTER_H

#include <memory>

#include "common/util.h"
#include "config.h"

namespace Innocent {

class Logic;
class Opcode;

#define UNIMPLEMENTED { error("type conversion unimplemented"); }

class Argument {
public:
	Argument(byte *ptr) : _ptr(ptr) {}
	virtual ~Argument() {}
	virtual operator uint16() const UNIMPLEMENTED
	virtual operator byte() const UNIMPLEMENTED
	virtual Argument operator=(byte b) UNIMPLEMENTED
	virtual Argument operator=(uint16 b) UNIMPLEMENTED
	byte *_ptr; // for debug
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
	uint16 _errorCount;

	Argument *readImmediateArg();
	Argument *readMainWordArg();
	Argument *readMainByteArg();

	void forgetLastError();
};

} // End of namespace Innocent

#endif // INNOCENT_INTER_H
