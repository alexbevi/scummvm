#ifndef INNOCENT_ACTOR_H
#define INNOCENT_ACTOR_H

#include <vector>

#include "common/endian.h"
#include "common/queue.h"

#include "innocent/animation.h"
#include "innocent/value.h"

namespace Innocent {
//

class MainDat;
class Program;

class Puppeteer {
public:
	enum Offsets {
		kActorId =     0,
		kMainCode =    2,
		kSize = 	0x24
	};
	Puppeteer() : _offset(0), _actorId(0) {}
	Puppeteer(const byte *data) { parse(data); }

	uint16 mainCodeOffset() const { return _offset; }
	uint16 actorId() const { return _actorId; }

private:
	void parse(const byte *data) {
		_actorId = READ_LE_UINT16(data + kActorId);
		_offset = READ_LE_UINT16(data + kMainCode);
	}

	uint16 _actorId;
	uint16 _offset;
};

class Actor : public Animation {
//
public:
	class Frame {
	public:
		Frame() : _left(999), _top(999), _nexts(8) {}
		Frame(int16 l, int16 t, std::vector<byte> nexts) : _left(l), _top(t), _nexts(nexts) {}

		int16 left() const { return _left; }
		int16 top() const { return _top; }

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

	void setFrame(uint16 f);

	uint16 room() const { return _room; }
	void setRoom(uint16, uint16 frame = 0, uint16 nextFrame = 0);

	bool isFine() const;

	void setAnimation(const CodePointer &anim);

	void hide();
	void callMe(const CodePointer &cp);
	void tellMe(const CodePointer &cp, uint16 timeout);

	Animation::Status tick();

	void toggleDebug();

	void setPuppeteer(const Puppeteer &p) { _puppeteer = p; }
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
	uint16 _nextAnimator; // to change to whenever possible
	bool _attentionNeeded;
	Puppeteer _puppeteer;

	Common::Queue<CodePointer> _callBacks;
	void callBacks();

	struct RoomCallback {
		uint16 timeout;
		CodePointer callback;
		RoomCallback(uint16 t, const CodePointer &p) : timeout(t), callback(p) {}
	};
	Common::List<RoomCallback> _roomCallbacks;

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
