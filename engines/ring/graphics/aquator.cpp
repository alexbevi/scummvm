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
 */

#include "ring/graphics/aquator.h"

#include "ring/base/stream.h"

#include "ring/helpers.h"

#include "common/archive.h"

namespace Ring {

#pragma region Pixel

void Pixel::set(int *pixel, int *val) {
	*pixel = *val;
}

void Pixel::set(int *val, int *pixel, int offset) {
	*pixel = *val - offset;
}

void Pixel::set(int *pixel, int val1, int val2, int val3) {
	if (val1 < val2) {
		*pixel = val2;
	} else if (val1 > val3) {
		*pixel = val3;
	} else {
		*pixel = val1;
	}
}

void Pixel::set(PixelTriplet *triplet, PixelData *from, float a1, float a2, float a3) {
	triplet->reset();

	// Compute pixels
	PixelData pixel1, pixel2;

	float val = from->a1 * a1 + from->a2 * a2 + from->a3 * a3;
	pixel2.a1 = a1 - val * from->a1;
	pixel2.a2 = a2 - val * from->a2;
	pixel2.a3 = a3 - val * from->a3;

	divide(&pixel2);

	pixel1.a1 = pixel2.a2 * from->a3 - pixel2.a3 * from->a2;
	pixel1.a2 = pixel2.a3 * from->a1 - pixel2.a1 * from->a3;
	pixel1.a3 = pixel2.a1 * from->a2 - pixel2.a2 * from->a1;

	// Update triplet
	triplet->p3 = *from;
	triplet->p2 = pixel2;
	triplet->p1 = pixel1;
}

void Pixel::set(PixelTriplet *from, PixelData *pixel, PixelData *pixel2) {
	pixel->a1 = (pixel2->a1 * from->p1.a1) + (pixel2->a2 * from->p2.a1) + (pixel2->a3 * from->p3.a1);
	pixel->a2 = (pixel2->a1 * from->p1.a2) + (pixel2->a2 * from->p2.a2) + (pixel2->a3 * from->p3.a2);
	pixel->a3 = (pixel2->a1 * from->p1.a3) + (pixel2->a2 * from->p2.a3) + (pixel2->a3 * from->p3.a3);
}

void Pixel::add(int *pixel, int val) {
	*pixel += val;
}

void Pixel::add(PixelData *from, PixelData *pixel, float a1, float a2, float a3) {
	pixel->a1 = from->a1 + a1;
	pixel->a2 = from->a2 + a2;
	pixel->a3 = from->a3 + a3;
}

void Pixel::substract(int *pixel, int val) {
	*pixel -= val;
}

void Pixel::multiply(int *pixel, double val) {
	*pixel = (int)(val * 65536.0);
}

void Pixel::multiply(PixelData *from, PixelData *pixel, float val) {
	pixel->a1 = from->a1 * val;
	pixel->a2 = from->a2 * val;
	pixel->a3 = from->a3 * val;
}

void Pixel::divide(PixelData *pixel) {
	float val = sqrt(pixel->a1 * pixel->a1 + pixel->a2 * pixel->a2 + pixel->a3 * pixel->a3);
	if (val == 0.0f)
		return;

	pixel->a1 = pixel->a1 / val;
	pixel->a2 = pixel->a2 / val;
	pixel->a3 = pixel->a3 / val;
}

#pragma endregion

#pragma region ImageHeaderEntry

#define IMAGEHEADER_BUFFER_SIZE 129600

ImageHeaderEntry::Header::Header() {
	field_0  = 0;
	field_4  = 0;
	field_8  = 0;
	field_C  = 0;
	field_10 = 0;
	field_14 = 0;
	field_18 = 0;
	field_1C = 0;
	field_20 = 0;
	field_24 = 0;
	field_28 = 0;
	field_2C = 0;
	field_30 = 0;
}

void ImageHeaderEntry::Header::update(const Header &header) {
	field_0  = header.field_0;
	field_4  = header.field_4;
	field_8  = header.field_8;
	field_C  = header.field_C;
	field_10 = header.field_10;
	field_14 = header.field_14;
	field_18 = header.field_18;
	field_1C = header.field_1C;
	field_20 = header.field_20;
	field_24 = header.field_24;
	field_28 = header.field_28;
	field_2C = header.field_2C;
	field_30 = header.field_30;
}

void ImageHeaderEntry::Header::load(Common::SeekableReadStream *stream) {
	field_0  = stream->readUint32LE();
	field_4  = stream->readUint32LE();
	field_8  = stream->readUint32LE();
	field_C  = stream->readUint32LE();
	field_10 = stream->readUint32LE();
	field_14 = stream->readUint32LE();
	field_18 = stream->readUint32LE();
	field_1C = stream->readUint32LE();
	field_20 = stream->readUint32LE();
	field_24 = stream->readUint32LE();
	field_28 = stream->readUint32LE();
	field_2C = stream->readUint32LE();
	field_30 = stream->readUint32LE();
}

void ImageHeaderEntry::InternalData::init(float a1, float a2, float a3, float a4, float a5, float a6) {
	field_0 = a3;
	field_4 = a4;
	field_8 = a5;
	field_C = a6;
	field_10 = a1 - 1;
	field_14 = a2 - 1;
	field_18 = 0;
	field_1C = 0;
}

ImageHeaderEntry::ImageHeaderEntry() {
	_buffer      = NULL;
	_bufferData  = NULL;
	_hasAdditionnalData = false;
}

ImageHeaderEntry::~ImageHeaderEntry() {
	free(_buffer);
	_buffer = NULL;

	_bufferData = NULL;
}

void ImageHeaderEntry::reset() {
	free(_buffer);
	_buffer = NULL;

	_bufferData = NULL;
}

void ImageHeaderEntry::init(Common::SeekableReadStream *stream, bool hasAdditionnalData) {
	reset();

	// Read entry header
	_header.load(stream);
	initData();

	// Allocate buffer and setup pointers
	_buffer = allocBuffer(hasAdditionnalData);
	if (hasAdditionnalData)
		_bufferData = (byte *)_buffer + IMAGEHEADER_BUFFER_SIZE;

	// Read buffers
	stream->read(_bufferData, _header.field_2C / 4);

	if (hasAdditionnalData)
		stream->read(_buffer, IMAGEHEADER_BUFFER_SIZE);

	_hasAdditionnalData = hasAdditionnalData;
}

void ImageHeaderEntry::init(ImageHeaderEntry *entry) {
	reset();

	// Copy header information
	_header.update(entry->getHeader());
	initData();

	// ALlocate new buffer
	_buffer = allocBuffer(true);
	_bufferData = (byte *)_buffer + IMAGEHEADER_BUFFER_SIZE;
	_hasAdditionnalData = true;
}

void ImageHeaderEntry::initData() {
	if (!_header.field_20)
		_header.field_20 = _header.field_0;

	if (!_header.field_28)
		_header.field_28 = _header.field_4;

	_header.field_30 = _header.field_8  * (_header.field_20 - _header.field_1C);
	_header.field_2C = (uint32)(_header.field_30 * (_header.field_28 - _header.field_24));
}

void ImageHeaderEntry::update(ImageHeaderEntry *entry, bool updateCaller) {
	error("[ImageHeaderEntry::update] Not implemented");
}

void ImageHeaderEntry::prepareBuffer() {
	error("[ImageHeaderEntry::prepareBuffer] Not implemented");
}

void ImageHeaderEntry::drawBuffer() {
	error("[ImageHeaderEntry::drawBuffer] Not implemented");
}

void ImageHeaderEntry::computeCoordinates(Common::Point *point) {
	int32 *buffer = (int32 *)_bufferData;
	if (!buffer)
		error("[ImageHeaderEntry::updateBuffer] Buffer not initialized properly");

	// Compute coordinates values
	Common::Point pointDiv = *point;
	pointDiv.x /= 16;
	pointDiv.y /= 16;

	Common::Point pointMod = *point;
	pointMod.x %= 16;
	pointMod.y %= 16;

	// Adjust coordinates
	if (point->x < 0)
		point->x = 0;

	if (point->x >= (int16)buffer[8200])
		point->x = buffer[8200] - 1;

	if (point->y < 0)
		point->y = 0;

	if (point->y >= (int16)buffer[8201])
		point->y = buffer[8201] - 1;

	int offset = (pointDiv.y << 6) + pointDiv.x;
	float dist = 2048.0f * 0.5f;

	float a1 = (float)buffer[offset + 2]  * 0.000015258789f;
	float a2 = (float)buffer[offset + 3]  * 0.000015258789f;
	float a3 = (float)buffer[offset + 66] * 0.000015258789f;
	float a4 = (float)buffer[offset + 67] * 0.000015258789f;

	error("[ImageHeaderEntry::updateBuffer] Missing code");

	if (true /* testing an unknown var related to x / y */) {
		if (a3 - a1 < dist) {
			if (a4 - a2 >= dist) {
				a2 += 2048.0f;
			} else if (a3 - a4 >= dist) {
				a4 += 2048.0f;
			}
		} else {
			a1 += 2048.0f;
			a2 += 2048.0f;
			a4 += 2048.0f;
		}
	} else {
		if (a3 - a4 < dist) {
			a2 += 2048.0f;

			if (true /* testing an unknown var related to x / y */) {
				a4 += 2048.0f;
				a3 += 2048.0f;
			}
		} else {
			a4 += 2048.0f;
			a2 += 2048.0f;
		}
	}

	float a5 = (float)buffer[offset + 4098] * 0.000015258789f;
	float a6 = (float)buffer[offset + 4099] * 0.000015258789f;
	float a7 = (float)buffer[offset + 4162]  * 0.000015258789f;
	float a8 = (float)buffer[offset + 4163]  * 0.000015258789f;

	float a9  = (float)(16 - pointMod.x);
	float a10 = (float)(16 - pointMod.y);

	// Update coordinates
	point->x = ((a9 * a3 + pointMod.x * a4) * pointMod.y + (a9 * a1 + pointMod.x * a2) * a10       ) * 0.0625f * 0.0625f;
	point->y = ((a9 * a5 + pointMod.x * a6) * a10        + (a9 * a7 + pointMod.x * a8) * pointMod.y) * 0.0625f * 0.0625f;

	point->x %= 2048;
	point->y %= buffer[8229];
}

void ImageHeaderEntry::adjustCoordinates(Common::Point *point) {
	uint32 *buffer = (uint32 *)_bufferData;
	if (!buffer)
		error("[ImageHeaderEntry::updateCoordinates] Buffer not initialized properly");

	point->x = (int16)((point->x * (float)buffer[8227] * 10.0f) / 2048.0f);
	point->y = (int16)((point->y - (float)buffer[8230] * 0.5f) * (float)buffer[8225] / (float)buffer[8230] + (float)buffer[8226] + 10.0f);
}

void *ImageHeaderEntry::allocBuffer(bool hasAdditionnalData) const {
	uint32 size = _header.field_2C / 4 + (hasAdditionnalData ? IMAGEHEADER_BUFFER_SIZE : 0);

	void *buffer = malloc(size + 1024);
	if (!buffer)
		error("[ImageHeaderEntry::allocBuffer] Cannot allocate buffer of size %d", size + 1024);

	return buffer;
}

#pragma endregion

#pragma region ImageHeader

ImageHeader::ImageHeader() {
	_field_4 = 0;
	_current = NULL;
	_field_4C = -1;
}

ImageHeader::~ImageHeader() {
	reset();
}

void ImageHeader::reset() {
	CLEAR_ARRAY(ImageHeaderEntry, _entries);
	SAFE_DELETE(_current);
}

void ImageHeader::init(Common::SeekableReadStream *stream) {
	reset();

	uint32 count = stream->readUint32LE();
	_field_4  = stream->readUint32LE();
	_field_4C = stream->readUint32LE();

	// Create entries
	for (uint32 i = 0; i < count; i++)
		_entries.push_back(new ImageHeaderEntry());

	if (count == 1)
		_field_4C = -1;

	for (uint32 i = 0; i < count; i++)
		_entries[i]->init(stream, false);
}

void ImageHeader::update(ImageHeaderEntry* entry) {
	if (!_current)
		error("[ImageHeader::update] entry not initialized properly");

	_current->init(_entries[0]);
	entry->update(_current, false);
}

ImageHeaderEntry *ImageHeader::get(uint32 index) {
	if (index >= _entries.size())
		error("[ImageHeader::get] Invalid index (was:%d, max:%d)", index, _entries.size() - 1);

	return _entries[index];
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
	SAFE_DELETE(_entry);
}

void AquatorStream::alloc(bool isCompressed, const Graphics::PixelFormat &format, uint32 size) {
	if (isCompressed) {
		Common::String filename = Common::String::format("%s.aqi", _path.c_str());

		// Decompress node
		CompressedStream *stream = new CompressedStream();
		if (!stream->init(filename, 2, size))
			error("[AquatorStream::alloc] Cannot init compressed stream for aquator file (%s)", filename.c_str());

		initNode(stream->decompressNode(), format);

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

		initNode(archive, format);

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

void AquatorStream::initNode(Common::SeekableReadStream *stream, const Graphics::PixelFormat &format) {
	_entry->init(stream, true);

	SAFE_DELETE(stream);

	// Init entry buffer
	int16 *buffer = (int16 *)_entry->getBuffer();

	// Process buffer
	for (uint32 i = 0; i < 64800; i++) {
		int16 green = (buffer[0] >> 3) & 252;
		int16 blue  = (buffer[0] >> 3) & 31;
		int16 red   = (buffer[0] >> 8) & 248;

		green >>= format.gBits();
		blue  >>= format.bBits();
		red   >>= format.rBits();

		green <<= format.gShift;
		blue  <<= format.bShift;
		red   <<= format.rShift;

		buffer[0] = green | red | blue;

		++buffer;
	}
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

void AquatorStream::updateEntries(float timeOffset) {
	for (Common::Array<AquatorImageHeader *>::iterator it = _headers.begin(); it != _headers.end(); it++) {
		AquatorImageHeader *header = (*it);

		if (!header->getHeader()->hasEntries())
			continue;

		if (header->getField4()) {
			_entry->update(header->getChannel() ? header->getHeader()->get(header->getField8() * 64) : header->getHeader()->getCurrent());
			header->setField4(0);
		}
	}
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
