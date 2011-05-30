/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef SPRITE_PLAYER_H
#define SPRITE_PLAYER_H

#include "sprite.h"

namespace Unity {

class UnityEngine;
class Object;

class SpritePlayer {
public:
	SpritePlayer(const char *filename, Object *par, UnityEngine *vm);
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

	byte *getPalette();

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

	PosInfo normal, speech, marked;
	bool was_speech, was_marked;

	unsigned int current_entry;
	SpriteEntrySprite *current_sprite, *current_speechsprite;
	SpriteEntryPalette *current_palette;

	unsigned int wait_target;

	void resetState();
private:
	Common::SeekableReadStream *spriteStream;
};

} // Unity

#endif

