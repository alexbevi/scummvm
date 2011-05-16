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
	se_Palette,
	se_RandomWait,
	se_Wait,
	se_Pause,
	se_Jump,
	se_Position,
	se_RelPos,
	se_MouthPos,
	se_Mark,
	se_Mask,
	se_Static,
	se_Audio,
	se_WaitForSound,
	se_Silent,
	se_StateSet,
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

struct SpriteEntryPalette : public SpriteEntry {
	byte palette[256*3];
	SpriteEntryPalette() : SpriteEntry(se_Palette) { }
};

struct SpriteEntryWait : public SpriteEntry {
	unsigned int wait;
	SpriteEntryWait(unsigned int _w) : SpriteEntry(se_Wait), wait(_w) { }
};

struct SpriteEntryRandomWait : public SpriteEntry {
	unsigned int rand_amt, base;
	SpriteEntryRandomWait(unsigned int _r, unsigned int _b) : SpriteEntry(se_RandomWait), rand_amt(_r), base(_b) { }
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

struct SpriteEntryAudio : public SpriteEntry {
	unsigned int length;
	byte *data;
	SpriteEntryAudio() : SpriteEntry(se_Audio) { }
};

struct SpriteEntryStateSet : public SpriteEntry {
	uint32 state;
	SpriteEntryStateSet(uint32 _s) : SpriteEntry(se_StateSet), state(_s) { }
};

class Sprite {
public:
	Sprite(Common::SeekableReadStream *_str);
	~Sprite();

	SpriteEntry *getEntry(unsigned int entry) { return entries[entry]; }
	unsigned int numEntries() { return entries.size(); }
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

} // Unity

#endif

