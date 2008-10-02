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

#ifndef KOM_FLICPLAYER_H
#define KOM_FLICPLAYER_H

#include "common/endian.h"
#include "common/fs.h"
#include "common/list.h"
#include "common/rect.h"

#include "kom/kom.h"

namespace Kom {

struct FlicHeader {
	uint32 size;
	uint16 type;
	uint16 numFrames;
	uint16 width;
	uint16 height;
	uint32 speed;
	uint16 offsetFrame1;
	uint16 offsetFrame2;
};

struct ChunkHeader {
	uint32 size;
	uint16 type;
};

struct FrameTypeChunkHeader {
	ChunkHeader header;
	uint16 numChunks;
	uint16 delay;
	uint16 reserved; // always zero
	uint16 widthOverride;
	uint16 heightOverride;
};

class FlicPlayer {
public:
	FlicPlayer(Common::FSNode flicNode);
	~FlicPlayer();

	bool decodeFrame();
	int width() const { return (_flicData ? _flicInfo.width : 0); }
	int height() const { return (_flicData ? _flicInfo.height : 0); }
	bool hasFrames() const { return (_flicData ? _flicInfo.numFrames > 0 : false); }
	uint32 speed() const { return _flicInfo.speed; }
	bool paletteDirty() const { return _paletteDirty; }
	const uint8 *getPalette() { _paletteDirty = false; return _palette; }
	const uint8 *getOffscreen() const { return _offscreen; }
	const Common::List<Common::Rect> *getDirtyRects() const { return _dirtyRects; }
	void clearDirtyRects() const { _dirtyRects->clear(); }

protected:
	ChunkHeader readChunkHeader();
	FrameTypeChunkHeader readFrameTypeChunkHeader(ChunkHeader chunkHead);
	void decodeByteRun(uint8 *data);
	void decodeDeltaFLC(uint8 *data);
	void setPalette(uint8 *mem);

	bool _paletteDirty;
	uint8 * _flicData;
	uint8 * _flicDataStart;
	uint8 *_offscreen;
	uint8 _palette[256 * 4];
	FlicHeader _flicInfo;
	uint16 _currFrame;
	uint32 _lastFrameTime;
	Common::List<Common::Rect> *_dirtyRects;
};

} // End of namespace Kom

#endif
