#ifndef INNOCENT_SPRITE_H
#define INNOCENT_SPRITE_H

#include "config.h"

namespace Innocent {

struct SpriteInfo {
	SpriteInfo(const byte*, uint16 index);
	uint16 left, top, width, height, image;
	int8 hotLeft, hotTop;
};

}

#endif
