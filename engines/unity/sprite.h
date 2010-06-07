#ifndef _SPRITE_H
#define _SPRITE_H

#include "common/stream.h"
#include "common/array.h"

// XXX: is this always true?
#define COLOUR_BLANK 13

namespace Unity {

enum SpriteEntryType {
	se_None,
	se_SpeechSprite,
	se_Sprite,
	se_RandomWait,
	se_Wait,
	se_Pause,
	se_Jump,
	se_Position,
	se_RelPos,
	se_MouthPos,
	se_Mark,
	se_Exit
};

struct SpriteEntry {
	SpriteEntryType type;
	SpriteEntry(SpriteEntryType _t) : type(_t) { }
};

struct SpriteEntrySprite : public SpriteEntry {
	unsigned int width;
	unsigned int height;
	byte *data;
	SpriteEntrySprite() : SpriteEntry(se_Sprite) { }
};

struct SpriteEntryWait : public SpriteEntry {
	unsigned int wait;
	SpriteEntryWait(unsigned int _w) : SpriteEntry(se_Wait), wait(_w) { }
};

struct SpriteEntryRandomWait : public SpriteEntry {
	unsigned int lower, upper;
	SpriteEntryRandomWait(unsigned int _l, unsigned int _u) : SpriteEntry(se_RandomWait), lower(_l), upper(_u) { }
};

struct SpriteEntryJump : public SpriteEntry {
	unsigned int target;
	SpriteEntryJump(unsigned int _t) : SpriteEntry(se_Jump), target(_t) { }
};

struct SpriteEntryPosition : public SpriteEntry {
	int newx, newy;
	SpriteEntryPosition(int _ax, int _ay) : SpriteEntry(se_Position), newx(_ax), newy(_ay) { }
};

struct SpriteEntryRelPos : public SpriteEntry {
	int adjustx, adjusty;
	SpriteEntryRelPos(int _ax, int _ay) : SpriteEntry(se_RelPos), adjustx(_ax), adjusty(_ay) { }
};

struct SpriteEntryMouthPos : public SpriteEntry {
	int adjustx, adjusty;
	SpriteEntryMouthPos(int _ax, int _ay) : SpriteEntry(se_MouthPos), adjustx(_ax), adjusty(_ay) { }
};

class Sprite {
public:
	Sprite(Common::SeekableReadStream *_str);
	~Sprite();

	SpriteEntry *getEntry(unsigned int entry) { return entries[entry]; }
	unsigned int getIndexFor(unsigned int anim) { return indexes[anim]; }
	unsigned int numAnims() { return indexes.size(); }

protected:
	Common::SeekableReadStream *_stream;

	Common::Array<unsigned int> indexes;
	Common::Array<SpriteEntry *> entries;

	SpriteEntry *readBlock();
	SpriteEntry *parseBlock(char blockType[4], uint32 size);

	void readCompressedImage(uint32 size, SpriteEntrySprite *dest);
	void decodeSpriteTypeOne(byte *buf, unsigned int size, byte *data, unsigned int width, unsigned int height);
	void decodeSpriteTypeTwo(byte *buf, unsigned int size, byte *data, unsigned int targetsize);

	bool _isSprite;
};

class UnityEngine;
class Object;

class SpritePlayer {
public:
	SpritePlayer(Sprite *spr, Object *par, UnityEngine *vm);
	~SpritePlayer();

	void startAnim(unsigned int a);
	unsigned int numAnims() { return sprite->numAnims(); }
	void update();

	bool playing();
	bool valid() { return current_sprite != 0; }

	unsigned int getCurrentHeight();
	unsigned int getCurrentWidth();
	byte *getCurrentData();

	bool speaking();
	unsigned int getSpeechHeight();
	unsigned int getSpeechWidth();
	byte *getSpeechData();

	int getXPos() { return normal.xpos; }
	int getYPos() { return normal.ypos; }
	int getXAdjust() { return normal.xadjust; }
	int getYAdjust() { return normal.yadjust; }
	int getSpeechXPos() { return speech.xpos; }
	int getSpeechYPos() { return speech.ypos; }
	int getSpeechXAdjust() { return speech.xadjust; }
	int getSpeechYAdjust() { return speech.yadjust; }

protected:
	Sprite *sprite;
	Object *parent;
	UnityEngine *_vm;

	struct PosInfo {
		int xpos, ypos;
		int xadjust, yadjust;
	};

	PosInfo normal, speech;
	bool was_speech;

	unsigned int current_entry;
	SpriteEntrySprite *current_sprite, *current_speechsprite;

	unsigned int wait_target;

	void resetState();
};

} // Unity

#endif

