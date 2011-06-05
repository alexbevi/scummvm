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
	unsigned int getNumAnims() { return _sprite->getNumAnims(); }
	void update();

	bool playing();
	bool valid() { return _currentSprite != 0; }

	unsigned int getCurrentHeight();
	unsigned int getCurrentWidth();
	byte *getCurrentData();

	bool speaking();
	unsigned int getSpeechHeight();
	unsigned int getSpeechWidth();
	byte *getSpeechData();

	byte *getPalette();

	int getXPos() { return _normal.xpos; }
	int getYPos() { return _normal.ypos; }
	int getXAdjust() { return _normal.xadjust; }
	int getYAdjust() { return _normal.yadjust; }
	int getSpeechXPos() { return _speech.xpos; }
	int getSpeechYPos() { return _speech.ypos; }
	int getSpeechXAdjust() { return _speech.xadjust; }
	int getSpeechYAdjust() { return _speech.yadjust; }

protected:
	Common::SeekableReadStream *_spriteStream;
	Sprite *_sprite;
	Object *_parent;
	UnityEngine *_vm;

	struct PosInfo {
		int xpos, ypos;
		int xadjust, yadjust;
	};

	PosInfo _normal, _speech, _marked;
	bool _wasSpeech, _wasMarked;

	unsigned int _currentEntry;
	SpriteEntrySprite *_currentSprite, *_currentSpeechSprite;
	SpriteEntryPalette *_currentPalette;

	unsigned int _waitTarget;

	void resetState();
};

} // Unity

#endif

