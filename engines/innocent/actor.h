#ifndef INNOCENT_ACTOR_H
#define INNOCENT_ACTOR_H

#include <vector>

#include "common/endian.h"
#include "common/queue.h"
#include "common/rect.h"

#include "innocent/animation.h"
#include "innocent/value.h"

namespace Innocent {
//

class MainDat;
class Program;

enum Direction {
	kDirNone = 0,
	kDirUp,
	kDirUpRight,
	kDirRight,
	kDirDownRight,
	kDirDown,
	kDirDownLeft,
	kDirLeft,
	kDirUpLeft,
	kDirCenter = 99
};

Direction operator>>(Direction a, Direction b);

class Puppeteer {
public:
	enum Offsets {
		kActorId =     		0,
		kMainCode =    		2,
		kMoveAnimators =    4,
		kTurnAnimators = 0x14,
		kSize = 		 0x24
	};
	Puppeteer() : _offset(0), _actorId(0) {}
	Puppeteer(const byte *data) { parse(data); }

	uint16 mainCodeOffset() const { return _offset; }
	uint16 offset() const { return _offset; }
	uint16 actorId() const { return _actorId; }
	bool valid() const { return _offset; }
	CodePointer moveAnimator(Direction d);
	CodePointer turnAnimator(Direction d);

private:
	void parse(const byte *data);

	uint16 _actorId;
	uint16 _offset;
	uint16 _animators[16];
};

class Actor : public Animation {
//
public:
	class Frame {
	public:
		Frame() : _position(999, 999), _nexts(8), _nextCount(0xff) {}
		Frame(Common::Point pos, std::vector<byte> n, uint16 i) : _position(pos), _nexts(n), _index(i), _nextCount(0xff) {}

		Common::Point position() const { return _position; }
		const std::vector<byte> &nexts() const { return _nexts; }
		const uint16 index() const { return _index; }

		Direction operator-(const Frame &other) const;
		bool operator==(const Frame &other) const {
			return _index == other._index;
		}
		byte nextCount() const {
			if (_nextCount == 0xff) {
				byte ct = 0;
				for (int i = 0; i < 8; i++)
					if (_nexts[i])
						ct++;
				_nextCount = ct;
			}

			return _nextCount;
		}

	private:
		uint16 _index;
		Common::Point _position;
		std::vector<byte> _nexts;
		mutable byte _nextCount;
	};

	class Speech {
	public:
		Speech() {}
		~Speech();
		Speech(Common::String text) : _text(text) {}
		bool active() const { return !_text.empty(); }
		void callWhenDone(const CodePointer &cp) { _cb.push(cp); }

	private:
		Common::String _text;
		Common::Queue<CodePointer> _cb;
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
	void moveTo(uint16 f);
	static Common::List<Frame> findPath(Frame from, uint16 to);

	uint16 room() const { return _room; }
	void setRoom(uint16, uint16 frame = 0, uint16 nextFrame = 0);

	bool isFine() const;

	void setAnimation(const CodePointer &anim);
	void setAnimation(uint16);

	void hide();
	void callMe(const CodePointer &cp);
	void tellMe(const CodePointer &cp, uint16 timeout);

	bool isSpeaking() const;
	void callMeWhenSilent(const CodePointer &cp);
	void say(const Common::String &text);

	bool isMoving() const;
	void callMeWhenStill(const CodePointer &cp);

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

	void animate();
	bool turnTo(Direction);
	bool nextFrame();

	Common::Queue<Frame> _framequeue;
	uint16 _frame;
	uint16 _nextFrame;
	uint16 _room;
	Direction _direction, _nextDirection;
	uint16 _nextAnimator; // to change to whenever possible
	bool _attentionNeeded, _confused;
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

	Speech _speech;
};

}

#endif
