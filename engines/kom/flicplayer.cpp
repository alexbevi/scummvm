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

#include "common/fs.h"
#include "common/file.h"
#include "common/list.h"
#include "common/rect.h"

#include "kom/flicplayer.h"

namespace Kom {

using Common::File;
using Common::Rect;

FlicPlayer::FlicPlayer(Common::FilesystemNode flicNode) : _paletteDirty(false), _offscreen(0), _currFrame(0) {
	File f;
	f.open(flicNode);
	_flicData = _flicDataStart = new uint8[f.size()];
	f.read(_flicDataStart, f.size());
	f.close();

	_flicInfo.size = READ_LE_UINT32(_flicData);      _flicData += 4;;
	_flicInfo.type = READ_LE_UINT16(_flicData);      _flicData += 2;;
	_flicInfo.numFrames = READ_LE_UINT16(_flicData); _flicData += 2;;
	_flicInfo.width = READ_LE_UINT16(_flicData);     _flicData += 2;;
	_flicInfo.height = READ_LE_UINT16(_flicData);    _flicData += 2;;
	_flicData += 4;
	_flicInfo.speed = READ_LE_UINT32(_flicData);    _flicData += 2;;

	_flicData = _flicDataStart + 80;
	_flicInfo.offsetFrame1 = READ_LE_UINT32(_flicData); _flicData += 4;;
	_flicInfo.offsetFrame2 = READ_LE_UINT32(_flicData); _flicData += 4;;

	// Check FLC magic number
	if (_flicInfo.type != 0xAF12) {
		error("FlicPlayer::FlicPlayer(): attempted to load non-FLC data (type = 0x%04X)", _flicInfo.type);
	}

	_offscreen = new uint8[_flicInfo.width * _flicInfo.height];
	memset(_palette, 0, sizeof(_palette));

	// Seek to the first frame
	_flicData = _flicDataStart + _flicInfo.offsetFrame1;

	_dirtyRects = new Common::List<Rect>;
}

FlicPlayer::~FlicPlayer() { 
	delete[] _flicDataStart;
	delete[] _offscreen;
	delete _dirtyRects;
}

ChunkHeader FlicPlayer::readChunkHeader() {
	ChunkHeader head;
	head.size = READ_LE_UINT32(_flicData); _flicData += 4;;
	head.type = READ_LE_UINT16(_flicData); _flicData += 2;;

	/* XXX: You'll want to read the rest of the chunk here as well! */
	
	return head;
}

FrameTypeChunkHeader FlicPlayer::readFrameTypeChunkHeader(ChunkHeader chunkHead) {
	FrameTypeChunkHeader head;
	
	head.header = chunkHead;
	head.numChunks = READ_LE_UINT16(_flicData); _flicData += 2;;
	head.delay = READ_LE_UINT16(_flicData); _flicData += 2;;
	head.reserved = READ_LE_UINT16(_flicData); _flicData += 2;;
	head.widthOverride = READ_LE_UINT16(_flicData); _flicData += 2;;
	head.heightOverride = READ_LE_UINT16(_flicData); _flicData += 2;;
	
	return head;
}

void FlicPlayer::decodeByteRun(uint8 *data) {
	uint8 *ptr = (uint8 *)_offscreen;
	while((ptr - _offscreen) < (_flicInfo.width * _flicInfo.height)) {
		uint8 chunks = *data++;
		while (chunks--) {
			int8 count = *data++;
			if (count > 0) {
				while (count--) {
					*ptr++ = *data;	
				}
				data++;
			} else {
				uint8 copyBytes = -count;
				memcpy(ptr, data, copyBytes);
				ptr += copyBytes;
				data += copyBytes;
			}
		}
	}
	_dirtyRects->clear();
	_dirtyRects->push_back(Rect(0, 0, _flicInfo.width, _flicInfo.height));
}

#define OP_PACKETCOUNT   0
#define OP_UNDEFINED     1
#define OP_LASTPIXEL     2
#define OP_LINESKIPCOUNT 3

void FlicPlayer::decodeDeltaFLC(uint8 *data) {
	uint16 linesInChunk = READ_LE_UINT16(data); data += 2;
	uint16 currentLine = 0;
	uint16 packetCount = 0;

	while (linesInChunk--) {
		uint16 opcode; 

		// First process all the opcodes.
		do {
			opcode = READ_LE_UINT16(data); data += 2;

			switch ((opcode >> 14) & 3) {
			case OP_PACKETCOUNT:
				packetCount = opcode;
				break;
			case OP_UNDEFINED:
				break;
			case OP_LASTPIXEL:
				*(uint8 *)(_offscreen + (currentLine * _flicInfo.width) + (_flicInfo.width - 1)) = (opcode & 0xFF);
				_dirtyRects->push_back(Rect(_flicInfo.width - 1, currentLine, _flicInfo.width, currentLine + 1));
				break;
			case OP_LINESKIPCOUNT:
				currentLine += -(int16)opcode;
				break;
			}
		} while (((opcode >> 14) & 3) != OP_PACKETCOUNT);

		uint16 column = 0;

		// Now interpret the RLE data
		while (packetCount--) {
			column += *data++;
			int8 rleCount = (int8)*data++;

			if (rleCount > 0) {
				memcpy((void *)(_offscreen + (currentLine * _flicInfo.width) + column), data, rleCount * 2);
				_dirtyRects->push_back(Rect(column, currentLine, column + (rleCount * 2), currentLine + 1));
				data += rleCount * 2;
				column += rleCount * 2;
			} else if (rleCount < 0) {
				uint16 dataWord = *(uint16 *)data; data += 2;
				for (int i = 0; i < -(int16)rleCount; ++i) { 
					*(uint16 *)(_offscreen + (currentLine * _flicInfo.width) + column + (i * 2)) = dataWord;
				}
				_dirtyRects->push_back(Rect(column, currentLine, column + (-(int16)rleCount * 2), currentLine + 1));

				column += (-(int16)rleCount) * 2;
			} else { // End of cutscene ?
				return;
			}
		}

		currentLine++;
	}
}

#define COLOR_256  4
#define FLI_SS2    7
#define FLI_BRUN   15
#define PSTAMP     18
#define FRAME_TYPE 0xF1FA

bool FlicPlayer::decodeFrame() {
	FrameTypeChunkHeader frameHeader;

	_dirtyRects->clear();

	// Read chunk
	ChunkHeader cHeader = readChunkHeader();
	switch (cHeader.type) {
	case FRAME_TYPE:
		frameHeader = readFrameTypeChunkHeader(cHeader);
		_currFrame++;
		break;
	default:
		error("FlicPlayer::decodeFrame(): unknown main chunk type (type = 0x%02X)", cHeader.type);
		break;
	 }

	// Read subchunks
	if (cHeader.type == FRAME_TYPE) {
		for (int i = 0; i < frameHeader.numChunks; ++i) {

			cHeader = readChunkHeader();
			switch (cHeader.type) {
			case COLOR_256:
				setPalette(_flicData);
				_paletteDirty = true;
				break;
			case FLI_SS2:
				decodeDeltaFLC(_flicData);
				break;
			case FLI_BRUN:
				decodeByteRun(_flicData);
				break;
			case PSTAMP:
				/* PSTAMP - skip for now */
				break;
			default:
				error("FlicPlayer::decodeFrame(): unknown subchunk type (type = 0x%02X)", cHeader.type);
				break;
			 }

			_flicData += cHeader.size - 6;
		}
	}

	// If we just processed the ring frame, set the next frame
	if (_currFrame == _flicInfo.numFrames + 1) {
		_currFrame = 1;
		_flicData = _flicDataStart + _flicInfo.offsetFrame2;
	}

	return true;
}

void FlicPlayer::setPalette(uint8 *mem) {
	uint16 numPackets = READ_LE_UINT16(mem); mem += 2;

	if (0 == READ_LE_UINT16(mem)) { //special case
		mem += 2;
		for (int i = 0; i < 256; ++i) {
			for (int j = 0; j < 3; ++j)
				_palette[i * 4 + j] = (mem[i * 3 + j]);
			_palette[i * 4 + 3] = 0;
		}
	} else {
		uint8 palPos = 0;

		while (numPackets--) {
			palPos += *mem++;
			uint8 change = *mem++;

			for (int i = 0; i < change; ++i) {
				for (int j = 0; j < 3; ++j)
					_palette[(palPos + i) * 4 + j] = (mem[i * 3 + j]);
				_palette[(palPos + i) * 4 + 3] = 0;
			}

			palPos += change;
			mem += (change * 3);
		}
	}
}

} // End of namespace Kom
