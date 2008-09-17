#ifndef INNOCENT_ANIMATION_H
#define INNOCENT_ANIMATION_H

#include <memory>

#include "common/list.h"
#include "common/rect.h"

#include "innocent/value.h"

namespace Innocent {
//

class Graphics;
class Resources;
class Sprite;

class Animation {
public:
	enum Status_ {
		kOk,
		kRemove,
		kFrameDone
	};

	typedef EnumPack<Status_> Status;

	Animation(const CodePointer &code, Common::Point position);
	virtual ~Animation();

	uint16 baseOffset() const { return _baseOffset; }

	virtual void paint(Graphics *g);
	virtual Status tick();

	void runOnNextFrame(const CodePointer &cp);

	virtual void setAnimation(uint16 anim) { _offset = anim; }

protected:
	class Sprite;

	uint16 shift();
	int8 shiftByte();
	int8 embeddedByte() const;

	void setMainSprite(uint16 sprite);
	void clearMainSprite();
	void clearSprites();

	void handleTrigger();

	template <int opcode>
	Status opcodeHandler();

	template <int N>
	void init_opcodes();

	virtual Status op(byte code);

	Resources *_resources;
	int8 _interval;
	int16 _ticksLeft;
	int8 _zIndex;
	Common::Point _position;
	/** start of the animation code */
	byte *_base;
	/** current position  in the animation */
	uint16 _offset;
	char _debugInfo[50];
	Common::List<Sprite *> _sprites;
	std::auto_ptr<Innocent::Sprite> _mainSprite;
	int8 _counter;
	uint16 _loopStart;
	/** offset of the animation from the start of its codeblock */
	uint16 _baseOffset;
	CodePointer _frameTrigger;

	bool _debugInvalid;

private:
	typedef Status (Animation::*OpcodeHandler)();
	OpcodeHandler _handlers[38];
};

}

#endif
