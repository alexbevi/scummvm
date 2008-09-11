#ifndef INNOCENT_ACTOR_H
#define INNOCENT_ACTOR_H

#include "innocent/animation.h"

namespace Innocent {
//

class MainDat;
class Program;

class Actor : public Animation {
//
public:
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


	uint16 room() const { return _room; }
	void setRoom(uint16);

	bool isVisible() const;

	Animation::Status tick();

	void setAnimation(const CodePointer &anim);

private:
	Actor(const CodePointer &code);

	// just in case, we'll explicitly add those if needed
	Actor();
	Actor(const Actor &);
	Actor &operator=(const Actor &);

	void readHeader(const byte *code);

	uint16 _room;
};

}

#endif
