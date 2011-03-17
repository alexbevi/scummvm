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

#include "ring/graphics/aquator.h"

#include "ring/base/stream.h"

#include "ring/helpers.h"

#include "common/archive.h"

namespace Ring {

#pragma region ImageHeaderEntry

ImageHeaderEntry::ImageHeaderEntry() {
	_field_0 = 0;
	_field_4 = 0;
	_field_8 = 0;
	_field_1C = 0;
	_field_20 = 0;
	_field_24 = 0;
	_field_28 = 0;
	_buffer = NULL;
	_field_38 = 0;
	_field_3C = 0;
}

ImageHeaderEntry::~ImageHeaderEntry() {
	SAFE_DELETE(_buffer);
}

#pragma endregion

#pragma region ImageHeader

ImageHeader::ImageHeader() {
	_field_4 = 0;
	_field_4C = -1;
}

ImageHeader::~ImageHeader() {
	CLEAR_ARRAY(ImageHeaderEntry, _headers);
}

void ImageHeader::init(Common::SeekableReadStream *stream) {
	error("[ImageHeader::init] Not implemented");
}

void ImageHeader::update(ImageHeaderEntry* entry) {
	error("[ImageHeader::update] Not implemented");
}

#pragma endregion

#pragma region AquatorImageHeader

AquatorImageHeader::AquatorImageHeader() {
	_field_0 = false;
	_field_4 = 0;
	_field_8 = 0;
	_channel = 0;

	_header = new ImageHeader();
}

AquatorImageHeader::~AquatorImageHeader() {
	delete _header;
}

void AquatorImageHeader::setChannel(uint32 channel) {
	if (_channel != channel) {
 		_channel = channel;
 		_field_4 = 1;
 	}
}

#pragma endregion

#pragma region AquatorStream

AquatorStream::AquatorStream(uint32 count, Common::String path) {
	_entry = new ImageHeaderEntry();

	_path = path;

	for (uint32 i = 0; i < count; i++)
		_headers.push_back(new AquatorImageHeader());
}

AquatorStream::~AquatorStream() {
	CLEAR_ARRAY(AquatorImageHeader, _headers);
}

void AquatorStream::alloc(bool isCompressed, byte a2, int a3, byte a4, int a5, byte a6, uint32 a7, uint32 size) {
	if (isCompressed) {
		Common::String filename = Common::String::format("%s.aqi", _path.c_str());

		// Decompress node
		CompressedStream *stream = new CompressedStream();
		if (!stream->init(filename, 2, size))
			error("[AquatorStream::alloc] Cannot init compressed stream for aquator file (%s)", filename.c_str());

		initNode(stream->decompressNode(), a2, a3, a4, a5, a6, a7);

		// Decompress each channel
		for (uint32 i = 0; i < _headers.size(); i++)
			initChannel(stream->decompressChannel(), i);

		delete stream;
	} else {
		// Open a stream to the uncompressed aquator file
		Common::String filename = Common::String::format("%s.aqi", _path.c_str());
		Common::SeekableReadStream *archive = SearchMan.createReadStreamForMember(filename);
		if (!archive)
			error("[AquatorStream::alloc] Error opening uncompressed aquator (%s)", filename.c_str());

		initNode(archive, a2, a3, a4, a5, a6, a7);

		// Init channels
		for (uint32 i = 0; i < _headers.size(); i++)
			initStream(i);
	}
}

void AquatorStream::dealloc() {
	CLEAR_ARRAY(AquatorImageHeader, _headers);

	delete _entry;
	_entry = new ImageHeaderEntry();
}

void AquatorStream::initNode(Common::SeekableReadStream *stream, byte a2, int a3, byte a4, int a5, byte a6, uint32 a7) {
	error("[AquatorStream::initNode] Not implemented");
}

void AquatorStream::initStream(uint32 index) {
	Common::String filename = Common::String::format("%s_%03i.aqc", _path.c_str(), index);
	Common::SeekableReadStream *archive = SearchMan.createReadStreamForMember(filename);
	if (!archive)
		error("[AquatorStream::initStream] Error opening aquator channel (%s)", filename.c_str());

	initChannel(archive, index);
}

void AquatorStream::initChannel(Common::SeekableReadStream *stream, uint32 index) {
	AquatorImageHeader *aquatorHeader = _headers[index];
	ImageHeader *imageHeader = aquatorHeader->getHeader();

	imageHeader->init(stream);

	// Cleanup
	delete stream;

	imageHeader->update(_entry);

	if (imageHeader->getField4() == 0)
		aquatorHeader->setField0(false);
	else
		aquatorHeader->setField0(true);

	aquatorHeader->setField4(aquatorHeader->getChannel());
}

uint32 AquatorStream::sub_410F50(uint32 index) {
	return _headers[index]->getField0();
}

void AquatorStream::sub_4114C0(float timeOffset) {
	error("[AquatorStream::sub_4114C0] Not implemented");
}

void AquatorStream::sub_411530(uint32 index, uint32 a2) {
	if (_headers[index]->getField8() != a2) {
		_headers[index]->setField8(a2);
		_headers[index]->setField4(_headers[index]->getChannel());
	}
}

void AquatorStream::setChannel(uint32 index, uint32 channel) {
	_headers[index]->setChannel(channel);
}

uint32 AquatorStream::getChannel(uint32 index) {
	return _headers[index]->getChannel();
}

#pragma endregion

} // End of namespace Ring
