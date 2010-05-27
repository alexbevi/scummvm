#include "sprite.h"

#include "common/system.h"

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

Sprite::Sprite(Common::SeekableReadStream *_str) : _stream(_str) {
	assert(_stream);

	_isSprite = false;

	readBlock();
	assert(_isSprite);

	// make sure we read everything
	assert(_str->pos() == _str->size() - 1);
}

Sprite::~Sprite() {
	delete _stream;	
}

void Sprite::readBlock() {
	assert(!_stream->eos());

	uint32 start = _stream->pos();

	// each block is in format <4-byte type> <32-bit size, including type+size> <data>
	char buf[4];
	_stream->read(buf, 4);
	uint32 size = _stream->readUint32LE();
	assert(size >= 8);
	parseBlock(buf, size - 8);

	assert((uint32)_stream->pos() == start + size);
}

void Sprite::parseBlock(char blockType[4], uint32 size) {
	//printf("sprite parser: trying block type %c%c%c%c\n", blockType[3], blockType[2], blockType[1], blockType[0]);
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
		while (num_entries--)
			uint32 unknown = _stream->readUint32LE(); // TODO
		while ((uint32)_stream->pos() < start + size) {
			readBlock();
		}
	} else if (!strncmp(blockType, TIME, 4)) {
		uint32 unknown = _stream->readUint32LE(); // TODO
	} else if (!strncmp(blockType, COMP, 4)) {
		// compressed image data
		readCompressedImage(size);
	} else if (!strncmp(blockType, RGBP, 4)) {
		// palette (256*3 bytes, each 0-63)
		_stream->skip(size); // TODO
	} else if (!strncmp(blockType, POSN, 4)) {
		// TODO: guesses!
		uint32 xpos = _stream->readUint32LE();
		uint32 ypos = _stream->readUint32LE();
	} else if (!strncmp(blockType, STAT, 4)) {
		return;
	} else if (!strncmp(blockType, PAUS, 4)) {
		return;
	} else if (!strncmp(blockType, EXIT, 4)) {
		return;
	} else if (!strncmp(blockType, MARK, 4)) {
		return;
	} else if (!strncmp(blockType, SETF, 4)) {
		// set flag?
		uint32 unknown = _stream->readUint32LE(); // TODO
	} else if (!strncmp(blockType, RAND, 4)) {
		// TODO: pick between two blocks randomly or something?
		uint32 unknown1 = _stream->readUint32LE(); // TODO
		uint32 unknown2 = _stream->readUint32LE(); // TODO
	} else if (!strncmp(blockType, JUMP, 4)) {
		uint32 unknown = _stream->readUint32LE(); // TODO
	} else if (!strncmp(blockType, SCOM, 4)) {
		// TODO: ???
		_stream->skip(size); // TODO
	} else if (!strncmp(blockType, DIGI, 4)) {
		// TODO: audio?!
		_stream->skip(size); // TODO
	} else if (!strncmp(blockType, SNDW, 4)) {
		return;
	} else if (!strncmp(blockType, SNDF, 4)) {
		uint32 unknown1 = _stream->readUint32LE(); // TODO
		uint32 unknown2 = _stream->readUint32LE(); // TODO
		assert(unknown2 == 0);
		char name[16];
		_stream->read(name, 16);
		//printf("name '%s'\n", name);
	} else if (!strncmp(blockType, PLAY, 4)) {
		return;
	} else if (!strncmp(blockType, MASK, 4)) {
		return;
	} else if (!strncmp(blockType, RPOS, 4)) {
		uint32 unknown1 = _stream->readUint32LE(); // TODO
		uint32 unknown2 = _stream->readUint32LE(); // TODO
	} else if (!strncmp(blockType, MPOS, 4)) {
		uint32 unknown1 = _stream->readUint32LE(); // TODO
		uint32 unknown2 = _stream->readUint32LE(); // TODO
	} else if (!strncmp(blockType, SILE, 4)) {
		return;
	} else if (!strncmp(blockType, OBJS, 4)) {
		uint32 unknown = _stream->readUint32LE(); // TODO
	} else {
		error("unknown sprite block type %c%c%c%c", blockType[3], blockType[2], blockType[1], blockType[0]);
	}
}

void Sprite::readCompressedImage(uint32 size) {
	uint32 width = _stream->readUint32LE();
	uint32 height = _stream->readUint32LE();

	uint16 unknown3 = _stream->readUint16LE();
	uint16 unknown4 = _stream->readUint16LE();
	if (unknown4 == 0x3) {
		// 4 bytes - reference to elsewhere?
		assert(size == 16);
		_stream->skip(size - 12);
		return;
	}
	assert(unknown4 == 0x1 || unknown4 == 0x2);
	if (unknown3 == 0x0) {
		// TODO: unknown3 == 0x0 is unknown method!
		// this isn't used by any images i care about right now
		printf("unknown compressed format!\n");
		_stream->skip(size - 12);
		return;
	}
	assert(unknown3 == 0xd);

	printf("compressed image, size 0x%x x 0x%x (%d), actual size %d, param1 0x%x, param2 0x%x\n",
		width, height, width * height, size - 12, unknown3, unknown4);

	uint32 targetsize = width * height;
	byte *data = new byte[targetsize];

	byte *buf = new byte[size - 12];
	_stream->read(buf, size - 12);

	// so, unknown3 == 0xd, unknown4 is 0x1 or 0x2: time to decode the image
	if (unknown4 == 0x1) {
		delete[] buf;
		return; // XXX
		decodeSpriteTypeOne(buf, size - 12, data, width, height);
	} else {
		decodeSpriteTypeTwo(buf, size - 12, data, targetsize);
	}

	delete[] buf;
	sprites.push_back(data);
	widths.push_back(width);
	heights.push_back(height);
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
						data[bytesout] = 0; // XXX: is zero good?
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

