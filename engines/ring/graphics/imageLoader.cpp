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

#include "ring/graphics/imageLoader.h"

#include "ring/base/stream.h"

#include "ring/graphics/image.h"

#include "ring/helpers.h"

#include "common/archive.h"

#include "graphics/imagedec.h"
#include "graphics/surface.h"

namespace Ring {

#pragma region BMP

bool ImageLoaderBMP::load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom) {
	// Get image surface
	Graphics::Surface *surface = Graphics::ImageDecoder::loadFile(image->getName(), g_system->getScreenFormat());
	if (!surface)
		return false;

	image->setSurface(surface);

	return true;
}

#pragma endregion

#pragma region BMA

ImageLoaderBMA::~ImageLoaderBMA() {
	deinit();
}

bool ImageLoaderBMA::load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom) {
	if (!image)
		error("[ImageLoaderBMA::load] Invalid image pointer!");

	_stream = NULL;
	_filename = image->getName();

	if (!init(type, zone, loadFrom)){
		warning("[ImageLoaderBMA::load] Error opening image file (%s)", _filename.c_str());
		goto cleanup;
	}

	// Read header
	if (!readHeader()) {
		warning("[ImageLoaderBMA::load] Error reading header (%s)", image->getName().c_str());
		goto cleanup;
	}

	// Read image data
	if (!readImage(image)) {
		warning("[ImageLoaderBMA::load] Error reading header (%s)", image->getName().c_str());
		goto cleanup;
	}

	deinit();

	return true;

cleanup:
	deinit();
	return false;
}

bool ImageLoaderBMA::init(ArchiveType type, Zone zone, LoadFrom loadFrom) {
	_stream = new CompressedStream();

	// Initialize stream
	switch (type) {
	default:
		warning("[ImageLoaderBMA::init] Invalid archive type (%d)!", type);
		break;

	case kArchiveFile:
		return _stream->init(_filename, 1, 0);

	case kArchiveArt:
		return _stream->initArt(_filename, zone, loadFrom);
	}

	return false;
}

void ImageLoaderBMA::deinit() {
	SAFE_DELETE(_stream);
}

bool ImageLoaderBMA::readHeader() {
	Common::SeekableReadStream *data = _stream->getCompressedStream();
	if (!data) {
		warning("[ImageLoaderBMA::readHeader] Cannot get compressed stream (%s)", _filename.c_str());
		return false;
	}

	// Skip signature
	data->skip(4);

	_header.field_0 = data->readUint16LE();
	_header.field_2 = data->readUint16LE();
	_header.width = data->readUint32LE();
	_header.height = data->readUint32LE();
	_header.field_C = data->readUint32LE();
	_header.field_10 = data->readUint16LE();

	return true;
}

bool ImageLoaderBMA::readImage(Image *image) {
	// TODO implement decompression
	return false;
}

#pragma endregion

#pragma region TGC

ImageLoaderTGC::~ImageLoaderTGC() {
	deinit();
}

bool ImageLoaderTGC::load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom) {
	if (!image)
		error("[ImageLoaderTGC::load] Invalid image pointer!");

	_stream = NULL;
	_filename = image->getName();

	if (!init(type, zone, loadFrom)){
		warning("[ImageLoaderTGC::load] Error opening image file (%s)", _filename.c_str());
		goto cleanup;
	}

	// Read header
	if (!readHeader(_stream)) {
		warning("[ImageLoaderTGC::load] Error reading header (%s)", image->getName().c_str());
		goto cleanup;
	}

	// Read image data
	if (!readImage(_stream, image)) {
		warning("[ImageLoaderTGC::load] Error reading header (%s)", image->getName().c_str());
		goto cleanup;
	}

	deinit();

	return true;

cleanup:
	deinit();
	return false;
}

bool ImageLoaderTGC::init(ArchiveType type, Zone zone, LoadFrom loadFrom) {
	_stream = new CompressedStream();

	// Initialize stream
	switch (type) {
	default:
		warning("[ImageLoaderTGC::init] Invalid archive type (%d)!", type);
		return false;

	case kArchiveFile:
		if (!_stream->init(_filename, 1, 0))
			return false;
		break;

	case kArchiveArt:
		if (!_stream->initArt(_filename, zone, loadFrom))
			return false;
		break;
	}

	// TGC are compressed as chunks of up to 64kb
	Common::SeekableReadStream *stream = _stream->getCompressedStream();
	uint32 chunks = stream->readUint32LE();
	uint32 size = stream->readUint32LE();

	_stream->decompressChuncks(chunks, size);

	return true;
}

void ImageLoaderTGC::deinit() {
	SAFE_DELETE(_stream);
}

#pragma endregion

#pragma region TGA

bool ImageLoaderTGA::load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom) {
	if (!image)
		error("[ImageLoaderTGA::load] Invalid image pointer!");

	_filename = image->getName();

	// Open a stream
	Common::SeekableReadStream *stream = SearchMan.createReadStreamForMember(_filename);
	if (!stream) {
		warning("[ImageLoaderTGA::load] Error opening image file (%s)", _filename.c_str());
		goto cleanup;
	}

	// Read header
	if (!readHeader(stream)) {
		warning("[ImageLoaderTGA::load] Error reading header (%s)", image->getName().c_str());
		goto cleanup;
	}

	// Read image data
	if (!readImage(stream, image)) {
		warning("[ImageLoaderTGA::load] Error reading header (%s)", image->getName().c_str());
		goto cleanup;
	}

	delete stream;
	return true;

cleanup:
	delete stream;
	return false;
}

bool ImageLoaderTGA::readHeader(Common::SeekableReadStream *stream) {
	if (!stream) {
		warning("[ImageLoaderTGA::readHeader] Invalid stream (%s)", _filename.c_str());
		return false;
	}

memset(&_header, 0, sizeof(_header));

	_header.identsize       = stream->readByte();
	_header.colourmaptype   = stream->readByte();
	_header.imagetype       = stream->readByte();

	_header.colourmapstart  = stream->readUint16LE();
	_header.colourmaplength = stream->readUint16LE();
	_header.colourmapbits   = stream->readByte();

	_header.xstart          = stream->readUint16LE();
	_header.ystart          = stream->readUint16LE();
	_header.width           = stream->readUint16LE();
	_header.height          = stream->readUint16LE();
	_header.bits            = stream->readByte();
	_header.descriptor      = stream->readByte();

	// Check that we support the image format
	if (_header.imagetype != kImageTypeRGB) {
		warning("[ImageLoaderTGA::readHeader] Only RGB TGA files are supported (%s)", _filename.c_str());
		return false;
	}

	// Check bits per pixel
	if (_header.bits <= 16) {
		warning("[ImageLoaderTGA::readHeader] Only 24 and 32bpp TGA files are supported (%s)", _filename.c_str());
		return false;
	}

	// Skip id string
	stream->skip(_header.identsize);

	return true;
}

bool ImageLoaderTGA::readImage(Common::SeekableReadStream *stream, Image *image) {
	// Check that we are not called with an unsupported format
	if (_header.imagetype != kImageTypeRGB || _header.bits <= 16)
		error("[ImageLoaderTGA::readImage] Unsupported image type (%s)!", _filename.c_str());

	Graphics::Surface *surface = new Graphics::Surface();
	surface->create(_header.width, _header.height, _header.bits);

	uint32 size = _header.width * _header.height;

	switch (_header.bits) {
	default:
		warning("[ImageLoaderTGA::readImage] Unsupported pixel depth (%s)", _filename.c_str());
		surface->free();
		delete surface;
		return false;

	case 24:
		stream->read(surface->pixels, size * 3);
		break;

	case 32:
		stream->read(surface->pixels, size * 4);
		break;
	}

	image->setSurface(surface);

	return true;
}

#pragma endregion

#pragma region CNM

bool ImageLoaderCNM::load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom) {
	warning("[ImageLoaderCNM::load] Not implemented (%s)!", image->getName().c_str());

	return true;
}

#pragma endregion


} // End of namespace Ring
