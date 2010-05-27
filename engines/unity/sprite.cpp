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
	if (unknown3 == 0x0) {
		// TODO: unknown3 == 0x0 is unknown method!
		// this isn't used by any images i care about right now
		printf("unknown compressed format!\n");
		_stream->skip(size - 12);
		return;
	}
	assert(unknown3 == 0xd);
	if (unknown4 == 0x3) {
		// 4 bytes - reference to elsewhere?
		assert(size == 16);
		_stream->skip(size - 12);
		return;
	}
	assert (unknown4 != 0x1 || unknown4 != 0x2);

	printf("compressed image, size 0x%x x 0x%x (%d), actual size %d, param1 0x%x, param2 0x%x\n",
		width, height, width * height, size - 12, unknown3, unknown4);

	// so, unknown3 == 0xd, unknown4 is 0x1 or 0x2: time to decode the image
	/*
	 * unknown4 == 0x2 method:
	 * the image is split into blocks, 2 bits for type followed by a per-type number of bits
	 * type 00: the next 8 bits are a number n, make a run of (n + 1) pixels, TODO
	 * type 01: the next 8 bits represent a single pixel
	 * type 02: something with 11 bits, TODO
	 * type 03: something with 16 bits, TODO
	 */
	byte buf[size - 12];
	_stream->read(buf, size - 12);
	unsigned int bitoffset = 0;
	unsigned int bytesout = 0;
	uint32 targetsize = width * height;
	bool dump = true;
	while (/*unknown4 != 0x1 &&*/ unknown3 == 0xd && bitoffset < 8 * (size - 12)) {
		unsigned int i = bitoffset / 8; unsigned int shift = bitoffset % 8;
		unsigned char x = buf[i] << shift;
		if (shift > 0 && i + 1 < size - 12) x += (buf[i + 1] >> (8 - shift));
		if (unknown4 == 1) {
			/*if ((x >> 6) == 0) {
			  printf("(don't know how to decode: input 0 with method 1)\n");
			  dump = true;
			  bitoffset -= 2;
			  break;
			  if (shift == 0 || i + 1 < size - 12) {
			  if (bytesout < targetsize)
			  bytesout += (int)x + 1;
			  else
			  assert(bytesout == targetsize);
			  }
			  bitoffset += 8;
			  continue;
			  }*/
			// skip 8 bits of data?
			//lastcol = x + (buf[i + 1] >> (8 - shift));
			bitoffset += 8;
			i = bitoffset / 8; shift = bitoffset % 8;
			if (i >= size - 12) { bytesout += 1; break; } // XXX
			x = buf[i] << shift;
			x += (buf[i + 1] >> (8 - shift));
			unsigned int decodeamt = (x >> 6);
			unsigned int old_decodeamt = decodeamt;
			bitoffset += 2;
			if (decodeamt == 0) {
				// was a single pixel
				decodeamt = 1;
			} else {
				// XXX: this is all terribly wrong
				i = bitoffset / 8; shift = bitoffset % 8;
				x = buf[i] << shift;
				x += (buf[i + 1] >> (8 - shift));

				decodeamt = (decodeamt << 2) + (x >> 6);
				// decodeamt = decodeamt + ((x >> 6) << 2) + 1;
				//printf("tweaking to %d bytes (with %d)\n", decodeamt, old_decodeamt);
				bitoffset += 2;
				//dump = true;
				//break;
			}
			//printf("had %d bytes, got %d bytes to add (making %d) at %d/%d to try reaching %d\n", bytesout, decodeamt, bytesout + decodeamt, i, size - 12, targetsize);
			/*for (unsigned int j = 0; j < decodeamt; j++) {
			  if (((bytesout + j) % width) == 0) printf("\n");
			  printf("%c", (lastcol / 10) + 'a');
			  }*/
			if (bytesout < targetsize)
				bytesout += decodeamt;
			else {
				assert(bytesout == targetsize);
				break;
			}
			continue;
		}
		bitoffset += 2;
		unsigned int decodetype = (x >> 6);
		if (decodetype == 0) {
			// read next 8 bits, output a run of that length
			i = bitoffset / 8; shift = bitoffset % 8;
			x = buf[i] << shift;
			if (shift > 0 && i + 1 < size - 12) x += (buf[i + 1] >> (8 - shift));
			printf("read a 0 at offset of %d bits, shifted %d bits at %d to output %d bytes (total %d bytes)\n", bitoffset, shift, i, (int)x + 1, bytesout);
			if (shift == 0 || i + 1 < size - 12) {
				if (bytesout < targetsize)
					bytesout += (int)x + 1;
				else
					assert(bytesout == targetsize);
			}
			bitoffset += 8;
		} else if (decodetype == 1) {
			// read next 8 bits: one byte of data
			printf("read a 1 at offset of %d bits, output 1 byte (total %d bytes)\n", bitoffset, bytesout);
			bytesout += 1;
			bitoffset += 8;
		} else if (decodetype == 2) {
			printf("read a 2 with val %d at offset of %d bits (shift %d) (total %d bytes)\n", x, bitoffset, shift, bytesout);
			i = bitoffset / 8; shift = bitoffset % 8;
			x = buf[i] << shift;
			if (shift > 0 && i + 1 < size - 12) x += (buf[i + 1] >> (8 - shift));
			bitoffset += 11;
			//printf("(don't know how to decode: input 2 with method 2)\n");
			//printf("(hit a problem)\n");
			//bitoffset -= 2;
			//break;
		} else {
			// set the colour for the runs? 6 bits is 64 colours..
			printf("read a 3 at offset of %d bits (total %d bytes)\n", bitoffset, bytesout);
			//bitoffset += 6;
			bitoffset += 16;
			i = bitoffset / 8; shift = bitoffset % 8;
			x = buf[i] << shift;
			if (shift > 0 && i + 1 < size - 12) x += (buf[i + 1] >> (8 - shift));
			printf("new colour?: %d\n", x >> 2);
		}
	}
	if (bitoffset >= 8 * (size - 12)) {
		printf("this was decoded! output %d bytes\n", bytesout);
		//assert(bytesout == targetsize);
		assert(bytesout <= targetsize);
	} else if (unknown3 == 0xd) {
		printf("decode failed\n");
	} else if (unknown3 == 0x0) {
		printf("weird encoding\n");
	}
	if (dump) {
		/*for (unsigned int i = 0; i < size - 12; i++) {
		  if (i % 16 == 0) printf("\n");
		  printf("%02x ", buf[i]);
		  }
		  printf("\n, as binary:");*/
		for (unsigned int i = bitoffset / 8; i < size - 12; i++) {
			if (i % 4 == 0) printf("\n");
			char n = buf[i];
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

