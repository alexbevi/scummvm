#ifndef INNOCENT_ACTOR_H
#define INNOCENT_ACTOR_H

#include <vector>

#include "common/queue.h"

#include "innocent/animation.h"
#include "innocent/value.h"

namespace Innocent {
//

class MainDat;
class Program;

class Actor : public Animation {
//
public:
	class Frame {
	public:
		Frame() : _left(999), _top(999), _nexts(8) {}
		Frame(int16 left, int16 top, std::vector<byte> nexts) : _left(left), _top(top), _nexts(nexts) {}

	private:
		int16 _left;
		int16 _top;
		std::vector<byte> _nexts;
	};

	friend class MainDat;
	friend class Program;
	enum {
		Size = 0x71
	};

	enum ActorOffsets {
		kOffsetOffset = 2,
		kOffsetLeft = 4,
		kOffsetTop = 6,
		kOffsetMainSprite = 8,
		kOffsetTicksLeft = 0xa,
		kOffsetCode = 0xc,
		kOffsetInterval = 0x10,
		kOffsetRoom = 0x59
	};

	void setFrame(uint16 f) { _frame = f; }

	uint16 room() const { return _room; }
	void setRoom(uint16, uint16 frame = 0, uint16 nextFrame = 0);

	bool isVisible() const;

	void setAnimation(const CodePointer &anim);

	void hide();
	void whenYouHideUpCall(const CodePointer &cp);

	Animation::Status tick();

	void toggleDebug();
private:
	Actor(const CodePointer &code);

	// just in case, we'll explicitly add those if needed
	Actor();
	Actor(const Actor &);
	Actor &operator=(const Actor &);

	void readHeader(const byte *code);

	uint16 _frame;
	uint16 _nextFrame;
	uint16 _room;
	byte _dir63;

	Common::Queue<CodePointer> _callBacks;
	void callBacks();

	bool _debug;

	template <int opcode>
	Animation::Status opcodeHandler();

	template <int N>
	void init_opcodes();

	virtual Animation::Status op(byte code);

	typedef Animation::Status (Actor::*OpcodeHandler)();
	OpcodeHandler _handlers[38];
};

}

#endif
