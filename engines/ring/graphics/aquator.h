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

#ifndef RING_AQUATOR_H
#define RING_AQUATOR_H

#include "ring/shared.h"

namespace Ring {

class ImageHeaderEntry {
public:
	ImageHeaderEntry();
	~ImageHeaderEntry();

	bool isInitialized() { return _buffer != NULL; }

private:
	uint32  _field_0;
	uint32  _field_4;
	uint32  _field_8;
	uint32  _field_1C;
	uint32  _field_20;
	uint32  _field_24;
	uint32  _field_28;
	uint32 *_buffer;
	uint32  _field_38;
	uint32  _field_3C;
};

class ImageHeader {
public:
	ImageHeader();
	~ImageHeader();

	void init(Common::SeekableReadStream *stream);
	void update(ImageHeaderEntry* entry);

	uint32 getCount() { return _headers.size(); }
	uint32 getField4() { return _field_4; }

private:
	// uint32 _field_0; // count
	uint32 _field_4;
	Common::Array<ImageHeaderEntry *> _headers;
	ImageHeaderEntry *_current;
	int32 _field_4C;
};

class AquatorImageHeader {
public:
	AquatorImageHeader();
	~AquatorImageHeader();

	void setField0(bool state) { _field_0 = state; }
	void setField4(uint32 val) { _field_4 = val; }
	void setField8(uint32 val) { _field_8 = val; }
	void setChannel(uint32 val);

	bool getField0() { return _field_0; }
	uint32 getField8() { return _field_8; }
	ImageHeader *getHeader() { return _header; }
	uint32 getChannel() { return _channel; }

private:
	bool _field_0;
	uint32 _field_4;
	uint32 _field_8;
	ImageHeader *_header;
	uint32 _channel;
};

class AquatorStream {
public:
	AquatorStream(uint32 count, Common::String path);
	~AquatorStream();

	void alloc(bool isCompressed, byte a2, int a3, byte a4, int a5, byte a6, uint32 a7, uint32 size);
	void dealloc();

	uint32 sub_410F50(uint32 index);
	uint32 getCount() { return _headers.size(); }
	void sub_4114C0(float timeOffset);
	void sub_411530(uint32 index, uint32 a2);
	void setChannel(uint32 index, uint32 channel);
	uint32 getChannel(uint32 index);

	bool isInitialized() { return _entry->isInitialized(); }

private:
	Common::Array<AquatorImageHeader *> _headers;
	Common::String _path;
	ImageHeaderEntry *_entry;

	void initNode(Common::SeekableReadStream *stream, byte a2, int a3, byte a4, int a5, byte a6, uint32 a7);
	void initChannel(Common::SeekableReadStream *stream, uint32 index);

	// Decompressed aquator
	void initStream(uint32 index);
};

} // End of namespace Ring

#endif // RING_AQUATOR_H
