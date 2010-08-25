#include "sprite.h"

namespace Unity {

// thereotically an exhaustive list of the block types in the SPR files
const char SPRT[4] = {'T', 'R', 'P', 'S' };
const char LIST[4] = {'T', 'S', 'I', 'L' };
const char PAUS[4] = {'S', 'U', 'A', 'P' };
const char EXIT[4] = {'T', 'I', 'X', 'E' };
const char STAT[4] = {'T', 'A', 'T', 'S' };
const char TIME[4] = {'E', 'M', 'I', 'T' };
const char COMP[4] = {'P', 'M', 'O', 'C' };
const char MASK[4] = {'K', 'S', 'A', 'M' };
const char POSN[4] = {'N', 'S', 'O', 'P' };
const char SETF[4] = {'F', 'T', 'E', 'S' };
const char MARK[4] = {'K', 'R', 'A', 'M' };
const char RAND[4] = {'D', 'N', 'A', 'R' };
const char JUMP[4] = {'P', 'M', 'U', 'J' };
const char SCOM[4] = {'M', 'O', 'C', 'S' };
const char DIGI[4] = {'I', 'G', 'I', 'D' };
const char SNDW[4] = {'W', 'D', 'N', 'S' };
const char SNDF[4] = {'F', 'D', 'N', 'S' };
const char PLAY[4] = {'Y', 'A', 'L', 'P' };
const char RPOS[4] = {'S', 'O', 'P', 'R' };
const char MPOS[4] = {'S', 'O', 'P', 'M' };
const char SILE[4] = {'E', 'L', 'I', 'S' };
const char OBJS[4] = {'S', 'J', 'B', 'O' };
const char RGBP[4] = {'P', 'B', 'G', 'R' };
const char BSON[4] = {'N', 'O', 'S', 'B' };

Sprite::Sprite(Common::SeekableReadStream *_str) : _stream(_str) {
	assert(_stream);

	_isSprite = false;

	readBlock();
	assert(_isSprite);

	// make sure we read everything
	assert(_str->pos() == _str->size());
}

Sprite::~Sprite() {
	delete _stream;
	for (unsigned int i = 0; i < entries.size(); i++) {
		if (entries[i]->type == se_Sprite)
			delete[] ((SpriteEntrySprite *)entries[i])->data;
		delete entries[i];
	}
}

SpriteEntry *Sprite::readBlock() {
	assert(!_stream->eos());

	uint32 start = _stream->pos();

	// each block is in format <4-byte type> <32-bit size, including type+size> <data>
	char buf[4];
	_stream->read(buf, 4);
	uint32 size = _stream->readUint32LE();
	assert(size >= 8);
	SpriteEntry *block = parseBlock(buf, size - 8);

	assert((uint32)_stream->pos() == start + size);

	return block;
}

SpriteEntry *Sprite::parseBlock(char blockType[4], uint32 size) {
	//printf("sprite parser: trying block type %c%c%c%c at %d\n",
	//	blockType[3], blockType[2], blockType[1], blockType[0], _stream->pos() - 8);
	if (!strncmp(blockType, SPRT, 4)) {
		// start of a sprite
		_isSprite = true;
		uint32 start = _stream->pos();
		uint32 unknown = _stream->readUint32LE();
		assert(unknown = 0x1000);
		readBlock();
		assert((uint32)_stream->pos() == start + size);
	} else if (!_isSprite) {
		error("sprite didn't start with SPRT");
	} else if (!strncmp(blockType, LIST, 4)) {
		// list of blocks
		uint32 start = _stream->pos();
		uint32 num_entries = _stream->readUint32LE();

		assert(indexes.empty()); // we assume there's only ever one LIST
		indexes.resize(num_entries);

		Common::Array<uint32> offsets;
		offsets.reserve(num_entries);

		while (num_entries--) {
			uint32 offset = _stream->readUint32LE();
			// offset is relative to start of this block (start - 8)
			if (offset != 0) offset += start - 8;
			offsets.push_back(offset);
		}

		unsigned int i = 0;
		while ((uint32)_stream->pos() < start + size) {
			while (i < offsets.size()) {
				if ((uint32)_stream->pos() == offsets[i]) {
					indexes[i] = entries.size();
					i++;
				} else if (offsets[i] == 0) {
					indexes[i] = ~0; // XXX
					i++;
				} else {
					assert((uint32)_stream->pos() < offsets[i]);
					break;
				}
			}
			entries.push_back(readBlock());
		}
		assert(i == offsets.size());
	} else if (!strncmp(blockType, TIME, 4)) {
		// presumably a wait between frames, see update()
		uint32 time = _stream->readUint32LE();
		return new SpriteEntryWait(time);
	} else if (!strncmp(blockType, COMP, 4)) {
		// compressed image data
		SpriteEntrySprite *img = new SpriteEntrySprite;
		readCompressedImage(size, img);
		return img;
	} else if (!strncmp(blockType, RGBP, 4)) {
		// palette (256*3 bytes, each 0-63)
		SpriteEntryPalette *pal = new SpriteEntryPalette;
		assert(size == sizeof(pal->palette));
		_stream->read(pal->palette, size);
		return pal;
	} else if (!strncmp(blockType, POSN, 4)) {
		// change position of sprite/object?
		uint32 xpos = _stream->readUint32LE();
		uint32 ypos = _stream->readUint32LE();
		return new SpriteEntryPosition(xpos, ypos);
	} else if (!strncmp(blockType, STAT, 4)) {
		// TODO
		return new SpriteEntry(se_None); // XXX
	} else if (!strncmp(blockType, PAUS, 4)) {
		// pause animation
		return new SpriteEntry(se_Pause);
	} else if (!strncmp(blockType, EXIT, 4)) {
		// ???
		return new SpriteEntry(se_Exit);
	} else if (!strncmp(blockType, MARK, 4)) {
		// TODO
		return new SpriteEntry(se_Mark);
	} else if (!strncmp(blockType, SETF, 4)) {
		// TODO: set flag?
		uint32 unknown = _stream->readUint32LE();
		assert(unknown <= 4); // 0, 1, 2, 3 or 4

		return new SpriteEntry(se_None); // XXX
	} else if (!strncmp(blockType, RAND, 4)) {
		// wait for a random time
		uint32 upper = _stream->readUint32LE();
		uint32 lower = _stream->readUint32LE();
		if (lower > upper) SWAP(upper, lower);
		return new SpriteEntryRandomWait(lower, upper);
	} else if (!strncmp(blockType, JUMP, 4)) {
		// a jump to an animation
		uint32 target = _stream->readUint32LE();
		return new SpriteEntryJump(target);
	} else if (!strncmp(blockType, SCOM, 4)) {
		// compressed image data representing speech
		SpriteEntrySprite *img = new SpriteEntrySprite;
		img->type = se_SpeechSprite;
		readCompressedImage(size, img);
		return img;
	} else if (!strncmp(blockType, DIGI, 4)) {
		// TODO: audio?!
		_stream->skip(size); // TODO
		return new SpriteEntry(se_None); // XXX
	} else if (!strncmp(blockType, SNDW, 4)) {
		// TODO
		return new SpriteEntry(se_None); // XXX
	} else if (!strncmp(blockType, SNDF, 4)) {
		// TODO: unknown is always 75, 95 or 100. volume?
		uint32 unknown = _stream->readUint32LE();
		uint32 empty = _stream->readUint32LE();
		assert(empty == 0);

		char name[16];
		_stream->read(name, 16);
		return new SpriteEntry(se_None); // XXX
	} else if (!strncmp(blockType, PLAY, 4)) {
		// TODO
		return new SpriteEntry(se_None); // XXX
	} else if (!strncmp(blockType, MASK, 4)) {
		// TODO
		return new SpriteEntry(se_None); // XXX
	} else if (!strncmp(blockType, RPOS, 4)) {
		// relative position change(?)
		int32 adjustx = _stream->readSint32LE();
		int32 adjusty = _stream->readSint32LE();

		return new SpriteEntryRelPos(adjustx, adjusty);
	} else if (!strncmp(blockType, MPOS, 4)) {
		// mouth position (relative to parent)
		int32 adjustx = _stream->readSint32LE();
		int32 adjusty = _stream->readSint32LE();

		return new SpriteEntryMouthPos(adjustx, adjusty);
	} else if (!strncmp(blockType, SILE, 4)) {
		// TODO
		return new SpriteEntry(se_None); // XXX
	} else if (!strncmp(blockType, OBJS, 4)) {
		// TODO
		uint32 unknown = _stream->readUint32LE();
		assert(unknown == 1 || (unknown >= 3 && unknown <= 7)); // always 1, 3, 4, 5, 6 or 7
		return new SpriteEntry(se_None); // XXX
	} else if (!strncmp(blockType, BSON, 4)) {
		// used only in legaleze.spr
		return new SpriteEntry(se_None); // XXX
	} else {
		error("unknown sprite block type %c%c%c%c", blockType[3], blockType[2], blockType[1], blockType[0]);
	}
	return NULL; // TODO: discarding data
}

#define NEXT_BITS() i = bitoffset / 8; shift = bitoffset % 8; \
		x = buf[i] << shift; \
		if (shift > 0 && i + 1 < size) x += (buf[i + 1] >> (8 - shift));

void Sprite::readCompressedImage(uint32 size, SpriteEntrySprite *img) {
	img->width = _stream->readUint32LE();
	img->height = _stream->readUint32LE();
	img->data = NULL;

	uint16 unknown3 = _stream->readUint16LE();
	uint16 unknown4 = _stream->readUint16LE();
	uint32 old_pos = 0;
	if (unknown4 == 0x3) {
		assert(size == 16);
		// reference to the sprite block which is ref bytes behind the start of this block
		uint32 ref = _stream->readUint32LE();

		// TODO: XXX: replace this hack with something that shares sprite data
		old_pos = _stream->pos();
		// 4 for ref field, 12 for header, 4 to catch size
		_stream->seek(-(int)ref - 4 - 12 - 4, SEEK_CUR);
		uint32 new_size = _stream->readUint32LE();
		readCompressedImage(new_size - 8, img);
		_stream->seek(old_pos);
		return;
	}
	assert(unknown4 == 0x1 || unknown4 == 0x2);
	if (unknown3 == 0x0) {
		// TODO: unknown3 == 0x0 is unknown method!
		// this isn't used by any images i care about right now
		warning("unknown compressed format");
		_stream->skip(size - 12);
		return;
	}
	// legaleze.spr uses 0xed
	assert(unknown3 == 0xd || unknown3 == 0xed);

	//printf("compressed image, size 0x%x x 0x%x (%d), actual size %d, param1 0x%x, param2 0x%x\n",
	//	width, height, width * height, size - 12, unknown3, unknown4);

	uint32 targetsize = img->width * img->height;
	img->data = new byte[targetsize + 2]; // TODO: +2 is stupid hack for overruns

	byte *buf = new byte[size - 12];
	_stream->read(buf, size - 12);

	// so, unknown3 == 0xd, unknown4 is 0x1 or 0x2: time to decode the image
	if (unknown4 == 0x1) {
		decodeSpriteTypeOne(buf, size - 12, img->data, img->width, img->height);
	} else {
		decodeSpriteTypeTwo(buf, size - 12, img->data, targetsize);
	}

	delete[] buf;
}

void Sprite::decodeSpriteTypeOne(byte *buf, unsigned int size, byte *data, unsigned int width, unsigned int height) {
	/*
	 * unknown4 == 0x1 method:
	 * image split into blocks, some variable number of bits for type, then per-type data
         * bit 0 set: single pixel: 3 bit colour offset
         * bit 1 set: 2 bit colour offset, 3 bit length (+1)
         * bit 2 set: 7 bit colour (+128 for global palette), 1 bit length (+1)
	 * bit 3 set: 5 bit length (+1), use previous colour
         * bit 4 set: 7 bit colour (+128 for global palette), 4 bit length (+1)
	 * bit 5 set: 8 bit colour, 8 bit length
         * otherwise: do a pixel run of blank(?) for a whole width, then drop 1 bit
	 */

	unsigned int bitoffset = 0;
	unsigned int bytesout = 0;
	unsigned char last_colour = 0;
	while (bitoffset < 8 * size) {
		unsigned int i, shift; unsigned char x;
		NEXT_BITS();

		unsigned int decodetype = (x >> 2);
		if ((decodetype & 0x20) == 0) {
			bitoffset += 1;

			NEXT_BITS();
			unsigned int colour = (x >> 5);
			bitoffset += 3;

			if ((colour & 0x4) == 0) {
				colour = last_colour + 1 + colour;
			} else {
				colour = last_colour - 1 - (colour & 0x3);
			}
			last_colour = colour;
			data[bytesout] = colour;
			bytesout++;
		} else if ((decodetype & 0x10) == 0) {
			bitoffset += 2;

			NEXT_BITS();
			unsigned int colour = (x >> 6);
			bitoffset += 2;

			NEXT_BITS();
			unsigned int length = (x >> 5) + 1;
			bitoffset += 3;

			if ((colour & 0x2) == 0) {
				colour = last_colour + 1 + colour;
			} else {
				colour = last_colour + 1 - colour;
			}
			last_colour = colour;
			for (unsigned int j = 0; j < length; j++) {
				data[bytesout] = colour;
				bytesout++;
			}
		} else if ((decodetype & 0x8) == 0) {
			bitoffset += 3;

			NEXT_BITS();
			unsigned int colour = (x >> 1) + 128;
			bitoffset += 7;

			NEXT_BITS();
			unsigned int length = (x >> 7) + 1;
			bitoffset += 1;

			last_colour = colour;
			for (unsigned int j = 0; j < length; j++) {
				data[bytesout] = colour;
				bytesout++;
			}
		} else if ((decodetype & 0x4) == 0) {
			bitoffset += 4;

			NEXT_BITS();
			unsigned int length = (x >> 3) + 1;
			bitoffset += 5;

			last_colour = COLOUR_BLANK;
			for (unsigned int j = 0; j < length; j++) {
				data[bytesout] = COLOUR_BLANK;
				bytesout++;
			}
		} else if ((decodetype & 0x2) == 0) {
			bitoffset += 5;

			NEXT_BITS();
			unsigned int colour = (x >> 1) + 128;
			bitoffset += 7;

			NEXT_BITS();
			unsigned int length = (x >> 4) + 1;
			bitoffset += 4;

			last_colour = colour;
			for (unsigned int j = 0; j < length; j++) {
				data[bytesout] = colour;
				bytesout++;
			}
		} else if ((decodetype & 0x1) == 0) {
			bitoffset += 6;

			NEXT_BITS();
			unsigned int colour = x;
			bitoffset += 8;

			NEXT_BITS();
			unsigned int length = x + 1;
			bitoffset += 8;

			last_colour = colour;
			for (unsigned int j = 0; j < length; j++) {
				data[bytesout] = colour;
				bytesout++;
			}
		} else {
			bitoffset += 7;

			last_colour = COLOUR_BLANK;
			for (unsigned int j = 0; j < width; j++) {
				data[bytesout] = COLOUR_BLANK;
				bytesout++;
			}
		}
	}

	//printf("tried for %d, got %d\n", width * height, bytesout);

	// TODO: why are we overrunning? not detecting the end?
	assert(bytesout >= width * height);
	assert(bytesout < 3 + width * height);
}

void Sprite::decodeSpriteTypeTwo(byte *buf, unsigned int size, byte *data, unsigned int targetsize) {
	/*
	 * unknown4 == 0x2 method:
	 * the image is split into blocks, 2 bits for type followed by a per-type number of bits
	 * type 00: the next 8 bits are a number n, run of (n + 1) blank(?) pixels
	 * type 01: the next 8 bits represent data for a single pixel
	 * type 02: 8 bits pixel data, followed by 3 bits n, run of (n + 1) of those pixels
	 * type 03: 8 bits pixel data, followed by 8 bits n, run of (n + 1) of those pixels
	 */

	unsigned int bitoffset = 0;
	unsigned int bytesout = 0;
	while (bitoffset < 8 * size) {
		unsigned int i = bitoffset / 8; unsigned int shift = bitoffset % 8;
		unsigned char x = buf[i] << shift;
		if (shift > 0 && i + 1 < size) x += (buf[i + 1] >> (8 - shift));
		unsigned int decodetype = (x >> 6);
		bitoffset += 2;

		if (decodetype == 0) {
			// read next 8 bits, output a run of that length
			i = bitoffset / 8; shift = bitoffset % 8;
			x = buf[i] << shift;
			if (shift > 0 && i + 1 < size) x += (buf[i + 1] >> (8 - shift));
			unsigned int length = x + 1;
			if (shift == 0 || i + 1 < size) {
				if (bytesout + length <= targetsize) {
					for (unsigned int j = 0; j < length; j++) {
						data[bytesout] = COLOUR_BLANK;
						bytesout++;
					}
				} else {
					// XXX: why do we go off the end sometimes?
					assert(bytesout == targetsize);
					assert(i + 2 > size);
				}
			}
			bitoffset += 8;
		} else if (decodetype == 1) {
			// read next 8 bits: one byte of data
			i = bitoffset / 8; shift = bitoffset % 8;
			x = buf[i] << shift;
			if (shift > 0 && i + 1 < size) x += (buf[i + 1] >> (8 - shift));
			data[bytesout] = x;
			bytesout += 1;
			bitoffset += 8;
		} else if (decodetype == 2) {
			// 8 bit colour + 3-bit length
			i = bitoffset / 8; shift = bitoffset % 8;
			x = buf[i] << shift;
			if (shift > 0 && i + 1 < size) x += (buf[i + 1] >> (8 - shift));
			char colour = x;
			bitoffset += 8;
			i = bitoffset / 8; shift = bitoffset % 8;
			x = buf[i] << shift;
			if (shift > 0 && i + 1 < size) x += (buf[i + 1] >> (8 - shift));
			unsigned int length = (x >> 5) + 1;
			for (unsigned int j = 0; j < length; j++) {
				data[bytesout] = colour;
				bytesout++;
			}
			bitoffset += 3;
		} else {
			// 8 bit colour + 8-bit length
			i = bitoffset / 8; shift = bitoffset % 8;
			x = buf[i] << shift;
			if (shift > 0 && i + 1 < size) x += (buf[i + 1] >> (8 - shift));
			char colour = x;
			bitoffset += 8;
			i = bitoffset / 8; shift = bitoffset % 8;
			x = buf[i] << shift;
			if (shift > 0 && i + 1 < size) x += (buf[i + 1] >> (8 - shift));
			unsigned int length = x + 1;
			for (unsigned int j = 0; j < length; j++) {
				data[bytesout] = colour;
				bytesout++;
			}
			bitoffset += 8;
		}
	}
	assert(bitoffset >= 8 * size);
	assert(bytesout == targetsize);

	bool dump = false;
	if (dump) {
		/*for (unsigned int i = 0; i < size - 12; i++) {
		  if (i % 16 == 0) printf("\n");
		  printf("%02x ", buf[i]);
		  }
		  printf("\n, as binary:");*/
		for (unsigned int j = bitoffset / 8; j < size; j++) {
			if (j % 4 == 0) printf("\n");
			char n = buf[j];
			unsigned int i;
			i = 1<<(sizeof(n) * 8 - 1);

			while (i > 0) {
				if (bitoffset % 8)
					bitoffset--;
				else if (n & i)
					printf("1");
				else
					printf("0");
				i >>= 1;
			}
		}
		printf("\n\n");
	}
}

} // Unity

