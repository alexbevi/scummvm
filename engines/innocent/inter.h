#ifndef INNOCENT_INTER_H
#define INNOCENT_INTER_H

#include <memory>

#include "common/list.h"
#include "common/util.h"
#include "config.h"

namespace Innocent {

class Logic;
class Opcode;
class Engine;
class Resources;
class Graphics;

#define UNIMPLEMENTED { error("type conversion unimplemented"); }

enum OpcodeMode {
	kCodeInitial = 0,
	kCodeNewRoom = 1,
	kCodeRoomLoop = 2,
	kCodeNewBlock = 8
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

class PeriodiCall {
public:
	PeriodiCall(byte *code);
	void call();

private:
	byte *_code;
	static const int kCodesNumber = 38;
	void (PeriodiCall::*_handlers[kCodesNumber])();

	template<int N>
	void initializeHandlers();

	template<int N>
	void handle();

	void setZIndex(int8 index);
	int8 _zIndex;
};

class Interpreter {
public:
	Interpreter(Logic *l, byte *base);

	void init();

	/**
	 * Run bytecode.
	 * @param code a Common::ReadStream pointing to code. The interpreter takes ownership of it.
	 * @param mode interpreting mode.
	 */
	Status run(uint16 offset, OpcodeMode mode);
	void tick();
	void executeRestricted(byte *code);

	Argument *getArgument(byte *&code);

	friend class Opcode;

	template <int opcode>
	void opcodeHandler(Argument *args[]);

	template <int N>
	void init_opcodes();

	typedef void (Interpreter::*OpcodeHandler)(Argument *args[]);
	OpcodeHandler _handlers[256];
	static const uint8 _argumentsCounts[];

	Logic *_logic;

private:
	class StringArgument : public Argument {
	public:
		StringArgument(byte *code, Resources *res);
		byte *translated() { return _translateBuf; }
	private:
		byte _translateBuf[100];
	};

	byte *_base;
	uint16 _mode;

	Argument *readImmediateArg(byte *&code);
	Argument *readMainWordArg(byte *&code);
	Argument *readMainByteArg(byte *&code);
	Argument *readStringArg(byte *&code);
	Argument *readLocalArg(byte *&code);

	Status run(uint16 offset);

	void failedCondition();
	void endIf();
	void goBack();
	void addPeriodiCall(byte *code);
	void setRoomLoop(byte *code);

	uint16 _failedCondition;
	bool _return;
	Common::List<PeriodiCall> _periodiCalls;
	byte *_roomLoop;
	
	Engine *_engine;
	Resources *_resources;
	Graphics *_graphics;
};

} // End of namespace Innocent

#endif // INNOCENT_INTER_H
