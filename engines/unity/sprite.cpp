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
		while ((uint32)_stream->pos() < start + size) {
			readBlock();
			// TODO: make sure this can only have one block
			assert((uint32)_stream->pos() == start + size);
		}
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
		// compressed sprite data
		uint32 width = _stream->readUint32LE();
		uint32 height = _stream->readUint32LE();

		// TODO
		uint16 unknown3 = _stream->readUint16LE();
		uint16 unknown4 = _stream->readUint16LE();
		if (unknown3 != 0x0 && unknown3 != 0xd) {
			error("unknown3: %d", unknown3);
		}
		if (unknown4 != 0x1 && unknown4 != 0x2 && unknown4 != 0x3) {
			error("unknown4: %d", unknown4);
		}
		if (unknown4 == 0x3) {
			// 4 bytes - reference to elsewhere?
			assert(size == 16);
			_stream->skip(size - 12);
			return;
		}
		if (size - 12 < 16 * 4 || unknown4 == 1) printf("compressed image, size 0x%x x 0x%x (%d), actual size %d, param1 0x%x, param2 0x%x",
			width, height, width * height, size - 12, unknown3, unknown4);
		byte buf[size - 12];
		_stream->read(buf, size - 12);
		unsigned int bitoffset = 0;
		unsigned int bytesout = 0;
		uint32 targetsize = width * height;
		// TODO: unknown3 == 0x0 is unknown method!
		// TODO: unknown4 == 0x1 doesn't work properly?
		//       maybe it means <8 bit something> <8 bit actual content>?
		while (unknown3 == 0xd && bitoffset < 8 * (size - 12)) {
			unsigned int i = bitoffset / 8; unsigned int shift = bitoffset % 8;
			unsigned char x = buf[i] << shift;
			bitoffset += 2;
			if (unknown4 == 1) {
				if ((x >> 6) == 0) break; // TODO
				i = bitoffset / 8; shift = bitoffset % 8;
				x = buf[i] << shift;
				if (shift == 0 || i + 1 < size - 12)
					if (bytesout < targetsize)
						bytesout += 1;
					else
						assert(bytesout == targetsize);
				bitoffset += 8;
			} else if ((x >> 6) == 0) {
				// read next 8 bits, output a run of that length
				i = bitoffset / 8; shift = bitoffset % 8;
				x = buf[i] << shift;
				if (shift > 0 && i + 1 < size - 12) x += (buf[i + 1] >> (8 - shift));
				if (shift == 0 || i + 1 < size - 12)
					if (bytesout < targetsize)
						bytesout += (int)x + 1;
					else
					//	{ bitoffset = 0; break; }
						assert(bytesout == targetsize);
				printf("read a 0 at offset of %d bits, shifted %d bits at %d to output %d bytes (total %d bytes)\n", bitoffset, shift, i, (int)x + 1, bytesout);
				bitoffset += 8;
			} else if ((x >> 6) == 1) {
				// read next 8 bits: one byte of data
				bytesout += 1;
				printf("read a 1 at offset of %d bits, output 1 byte (total %d bytes)\n", bitoffset, bytesout);
				bitoffset += 8;
			} else if ((x >> 6) == 2) {
				// TODO: this needs to output 4 bytes for '10' with a maximum of 3 zero bytes after :(
				i = bitoffset / 8; shift = bitoffset % 8;
				x = buf[i] << shift;
				if (shift > 0 && i + 1 < size - 12) x += (buf[i + 1] >> (8 - shift));
				printf("read a 2 with val %d at offset of %d bits (shift %d) (total %d bytes)\n", x, bitoffset, shift, bytesout);
				bitoffset -= 2;
				break;
			} else {
				// set the colour for the runs? 6 bits is 64 colours..
				printf("read a 3 at offset of %d bits (total %d bytes)\n", bitoffset, bytesout);
				bitoffset += 6;
			}
		}
		if (bitoffset >= 8 * (size - 12)) {
			printf("this was decoded! output %d bytes\n", bytesout);
			assert(bytesout == targetsize);
		} else if (size - 12 < 128 * 4) {
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
					if (n & i)
						printf("1");
					else
						printf("0");
					i >>= 1;
				}
			}
		}
		printf("\n\n");
	} else if (!strncmp(blockType, RGBP, 4)) {
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

} // Unity

