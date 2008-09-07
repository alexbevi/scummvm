#ifndef INNOCENT_INTER_H
#define INNOCENT_INTER_H

#include <memory>

#include "common/util.h"
#include "config.h"

namespace Innocent {

class Logic;
class Opcode;
class Engine;
class Resources;

#define UNIMPLEMENTED { error("type conversion unimplemented"); }

enum OpcodeMode {
	kCodeInitial = 0
};

enum Status {
	kReturned = 0,
	kInvalidOpcode = 1
};

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
	Interpreter(Engine *e);

	/**
	 * Run bytecode.
	 * @param code a Common::ReadStream pointing to code. The interpreter takes ownership of it.
	 * @param mode interpreting mode.
	 */
	Status run(byte *code, OpcodeMode mode);

	Argument *getArgument();

	friend class Opcode;

	static void defaultHandler(Interpreter *interpreter, Argument *args[]);

	static const uint8 _argumentsCounts[];

	typedef void(*OpcodeHandler)(Interpreter *interpreter, Argument *args[]);
	static OpcodeHandler _handlers[];
	friend class OpcodeHandlers;

	Logic *_logic;

private:
	Status run();
	uint8 _currentCode; // for error reporting
	byte *_code;
	uint16 _mode;
	uint16 _errorCount;

	Argument *readImmediateArg();
	Argument *readMainWordArg();
	Argument *readMainByteArg();
	Argument *readLocalArg();

	void forgetLastError();
	void returnUp();
	uint8 _return;
	Engine *_engine;
	Resources *_resources;
};

} // End of namespace Innocent

#endif // INNOCENT_INTER_H
