#ifndef INNOCENT_INTER_H
#define INNOCENT_INTER_H

#include <memory>

#include "common/list.h"
#include "common/rect.h"
#include "common/util.h"
#include "config.h"

#include "innocent/value.h"

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

class Animation {
public:
	Animation(byte *code, Resources *resources);
	void tick();

private:
	byte *_code;
	static const int kCodesNumber = 38;
	void (Animation::*_handlers[kCodesNumber])();

	template<int N>
	void initializeHandlers();

	template<int N>
	void handle();

	void setZIndex(int8 index);
	void setPosition(Common::Point position);
	void setSprite(uint16 index);
	int8 _zIndex;
	Common::Point _position;
	uint16 _sprite;
	Resources *_resources;
};

class Interpreter {
public:
	Interpreter(Logic *l, byte *base, const char *name);

	void init();

	/**
	 * Run bytecode.
	 * @param code a Common::ReadStream pointing to code. The interpreter takes ownership of it.
	 * @param mode interpreting mode.
	 */
	Status run(uint16 offset, OpcodeMode mode);
	void tick();
	void executeRestricted(byte *code);

	Value *getArgument(byte *&code);

	friend class Opcode;

	template <int opcode>
	void opcodeHandler(ValueVector args);

	template <int N>
	void init_opcodes();

	typedef void (Interpreter::*OpcodeHandler)(ValueVector args);
	OpcodeHandler _handlers[256];
	static const uint8 _argumentsCounts[];

	Logic *_logic;

	const char *name() const { return _name; }

private:
	char _name[100];
	template<class T>
	T *readArgument(byte *&code);

	byte *_base;
	uint16 _mode;

	Status run(uint16 offset);

	void failedCondition();
	void endIf();
	void goBack();
	void addAnimation(byte *code);
	void setRoomLoop(byte *code);

	uint16 _failedCondition;
	bool _return;
	Common::List<Animation> _animations;
	byte *_roomLoop;

	Engine *_engine;
	Resources *_resources;
	Graphics *_graphics;
};

} // End of namespace Innocent

#endif // INNOCENT_INTER_H
