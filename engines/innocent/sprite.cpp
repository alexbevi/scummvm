#include "innocent/sprite.h"

#include "common/endian.h"

namespace Innocent {
//

enum SpriteMap {
	kSpriteMapImage = 0,
	kSpriteMapLeft = 2,
	kSpriteMapTop = 4,
	kSpriteMapWidth = 6,
	kSpriteMapHeight,
	kSpriteMapHotLeft,
	kSpriteMapHotTop,
	kSpriteMapSize
};

SpriteInfo::SpriteInfo(const byte *spritemap, uint16 index) {
	spritemap += index * kSpriteMapSize;
	top = READ_LE_UINT16(spritemap + kSpriteMapTop);
	left = READ_LE_UINT16(spritemap + kSpriteMapLeft);
	width = spritemap[kSpriteMapWidth];
	height = spritemap[kSpriteMapHeight];
	image = READ_LE_UINT16(spritemap + kSpriteMapImage);
	hotLeft = *reinterpret_cast<const int8 *>(spritemap + kSpriteMapHotLeft);
	hotTop = *reinterpret_cast<const int8 *>(spritemap + kSpriteMapHotTop);
}

}
