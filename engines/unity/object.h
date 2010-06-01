#ifndef _OBJECT_H
#define _OBJECT_H

#include "common/array.h"
#include "common/str.h"

namespace Common {
	class SeekableReadStream;
}

namespace Unity {

class UnityEngine;
class SpritePlayer;

struct Description {
	Common::String text;
	uint32 entry_id;
	uint32 voice_group, voice_subgroup, voice_id;
};

class Object {
public:
	byte world, screen, id;
	unsigned int x, y;
	unsigned int width, height;
	bool active;
	SpritePlayer *sprite;

	Common::Array<Description> descriptions;

	void loadObject(UnityEngine *_vm, unsigned int world, unsigned int screen, unsigned int id);

protected:
	void readBlock(byte type, Common::SeekableReadStream *objstream);
};

} // Unity

#endif

