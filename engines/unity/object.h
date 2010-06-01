#ifndef _OBJECT_H
#define _OBJECT_H

#include "common/array.h"
#include "common/str.h"

namespace Unity {

class SpritePlayer;

struct Description {
	Common::String text;
	uint32 entry_id;
	uint32 voice_group, voice_subgroup, voice_id;
};

struct Object {
	byte world, screen, id;
	unsigned int x, y;
	unsigned int width, height;
	bool active;
	SpritePlayer *sprite;

	Common::Array<Description> descriptions;
};

} // Unity

#endif

