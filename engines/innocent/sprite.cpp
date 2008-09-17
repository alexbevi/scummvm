/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

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
