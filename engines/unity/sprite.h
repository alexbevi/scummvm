#include "common/stream.h"

#include "common/array.h" // XXX

namespace Unity {

enum SpriteEntryType {
	se_None,
	se_SpeechSprite,
	se_Sprite,
	se_RandomWait,
	se_Wait,
	se_Pause,
	se_Jump,
	se_RelPos,
	se_MouthPos,
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

struct SpriteEntryJump : public SpriteEntry {
	unsigned int target;
	SpriteEntryJump(unsigned int _t) : SpriteEntry(se_Jump), target(_t) { }
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

	unsigned int xpos, ypos;

	void startAnim(unsigned int a);
	unsigned int numAnims() { return indexes.size(); }
	void update();

	unsigned int getCurrentHeight();
	unsigned int getCurrentWidth();
	byte *getCurrentData();

protected:
	Common::SeekableReadStream *_stream;

	unsigned int current_entry;
	SpriteEntrySprite *current_sprite;

	unsigned int wait_start;

	Common::Array<unsigned int> indexes;
	Common::Array<SpriteEntry *> entries;

	SpriteEntry *readBlock();
	SpriteEntry *parseBlock(char blockType[4], uint32 size);

	void readCompressedImage(uint32 size, SpriteEntrySprite *dest);
	void decodeSpriteTypeOne(byte *buf, unsigned int size, byte *data, unsigned int width, unsigned int height);
	void decodeSpriteTypeTwo(byte *buf, unsigned int size, byte *data, unsigned int targetsize);

	bool _isSprite;
};

} // Unity

