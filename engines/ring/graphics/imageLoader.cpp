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

#include "ring/graphics/imageLoader.h"

#include "ring/base/stream.h"

#include "ring/graphics/image.h"
#include "ring/graphics/movie.h"

#include "ring/helpers.h"

#include "common/archive.h"

#include "graphics/imagedec.h"

namespace Ring {

#pragma region BMP

bool ImageLoaderBMP::load(Image *image, ArchiveType, ZoneId, LoadFrom, DrawType) {
	if (!image)
		error("[ImageLoaderBMP::load] Invalid image pointer!");

	// Get image stream
	Common::SeekableReadStream *stream = SearchMan.createReadStreamForMember(image->getName());
	if (!stream) {
		warning("[ImageLoaderBMP::load] Cannot load image (%s)", image->getName().c_str());
		return false;
	}

	// Get image surface
	Graphics::Surface *surface = Graphics::ImageDecoder::loadFile(*stream, g_system->getScreenFormat());
	if (!surface) {
		warning("[ImageLoaderBMP::load] Cannot decode image (%s)", image->getName().c_str());
		delete stream;
		return false;
	}

	image->setSurface(surface);

	delete stream;

	return true;
}

#pragma endregion

#pragma region BMA

ImageLoaderBMA::ImageLoaderBMA() {
	_stream = NULL;
	memset(&_header, 0, sizeof(_header));
	_coreSize = 0;
	_seqSize = 0;
	_blockSize = 0;
}

ImageLoaderBMA::~ImageLoaderBMA() {
	deinit();
}

bool ImageLoaderBMA::load(Image *image, ArchiveType type, ZoneId zone, LoadFrom loadFrom, DrawType) {
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
		warning("[ImageLoaderBMA::load] Error reading header (%s)", _filename.c_str());
		goto cleanup;
	}

	// Read image data
	if (!readImage(image)) {
		warning("[ImageLoaderBMA::load] Error reading image (%s)", _filename.c_str());
		goto cleanup;
	}

	deinit();

	return true;

cleanup:
	deinit();
	return false;
}

bool ImageLoaderBMA::init(ArchiveType type, ZoneId zone, LoadFrom loadFrom) {
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
	if (!_stream)
		error("[ImageLoaderBMA::readHeader] Stream not initialized");

	Common::SeekableReadStream *data = _stream->getCompressedStream();
	if (!data) {
		warning("[ImageLoaderBMA::readHeader] Cannot get compressed stream (%s)", _filename.c_str());
		return false;
	}

	// Read Seq size
	_seqSize = data->readUint32LE();

	// Read header
	_header.coreWidth = data->readUint16LE();
	_header.coreHeight = data->readUint16LE();
	_header.seqWidth = data->readUint32LE();
	_header.seqHeight = data->readUint32LE();
	_header.field_C = data->readUint32LE();
	_header.field_10 = data->readUint16LE();

	// Read core size
	data->seek(_seqSize + 76);
	_coreSize = data->readUint32LE();

	// Compute block size
	_blockSize = 0;
	uint32 val = _header.coreWidth;
	if (val) {
		do {
			val >>= 1;
			++_blockSize;
		} while (val);
	}

	return true;
}

bool ImageLoaderBMA::readImage(Image *image) {
	if (!_stream)
		error("[ImageLoaderBMA::readImage] Stream not initialized");

	Common::MemoryReadStream *imageData = _stream->decompressIndexed(_blockSize,
	                                                                 _seqSize,  (2 * _header.seqWidth * _header.seqHeight) / _header.coreHeight,
	                                                                 _coreSize, 2 * _header.coreWidth * _header.coreHeight,
	                                                                 _header.seqWidth * _header.seqHeight * 16,
	                                                                 2 * _header.seqWidth * _header.seqHeight - 6,
	                                                                 _header.field_C, _header.field_10);

	// Create surface to hold the data
	Graphics::PixelFormat format = g_system->getScreenFormat();
	format.bytesPerPixel = 2;

	Graphics::Surface *surface = new Graphics::Surface();
	surface->create((uint16)_header.seqWidth, (uint16)_header.seqHeight, format); // FIXME: Always 16bpp BMPs?

	// Read from compressed stream
	imageData->read(surface->pixels, _header.seqWidth * _header.seqHeight * 2);
	delete imageData;

	image->setSurface(surface);

	return true;
}

#pragma endregion

#pragma region TGC

ImageLoaderTGC::ImageLoaderTGC() {
	_stream = NULL;
}

ImageLoaderTGC::~ImageLoaderTGC() {
	deinit();
}

bool ImageLoaderTGC::load(Image *image, ArchiveType type, ZoneId zone, LoadFrom loadFrom, DrawType) {
	if (!image)
		error("[ImageLoaderTGC::load] Invalid image pointer!");

	_stream = NULL;
	_filename = image->getName();

	Common::SeekableReadStream *decompressedData = init(type, zone, loadFrom);
	if (!decompressedData){
		warning("[ImageLoaderTGC::load] Error opening image file (%s)", _filename.c_str());
		goto cleanup;
	}

	// Read header
	if (!readHeader(decompressedData)) {
		warning("[ImageLoaderTGC::load] Error reading header (%s)", _filename.c_str());
		goto cleanup;
	}

	// Read image data
	if (!readImage(decompressedData, image)) {
		warning("[ImageLoaderTGC::load] Error reading image (%s)", _filename.c_str());
		goto cleanup;
	}

	deinit();

	return true;

cleanup:
	deinit();
	return false;
}

Common::SeekableReadStream *ImageLoaderTGC::init(ArchiveType type, ZoneId zone, LoadFrom loadFrom) {
	_stream = new CompressedStream();

	// Initialize stream
	switch (type) {
	default:
		warning("[ImageLoaderTGC::init] Invalid archive type (%d)!", type);
		return NULL;

	case kArchiveFile:
		if (!_stream->init(_filename, 1, 0))
			return NULL;
		break;

	case kArchiveArt:
		if (!_stream->initArt(_filename, zone, loadFrom))
			return NULL;
		break;
	}

	// TGC are compressed as chunks of up to 64kb
	Common::SeekableReadStream *stream = _stream->getCompressedStream();
	uint32 chunks = stream->readUint32LE();
	uint32 size = stream->readUint32LE();

	return _stream->decompressChuncks(chunks, size);
}

void ImageLoaderTGC::deinit() {
	SAFE_DELETE(_stream);
}

#pragma endregion

#pragma region TGA

bool ImageLoaderTGA::load(Image *image, ArchiveType, ZoneId, LoadFrom, DrawType) {
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
		warning("[ImageLoaderTGA::load] Error reading header (%s)", _filename.c_str());
		goto cleanup;
	}

	// Read image data
	if (!readImage(stream, image)) {
		warning("[ImageLoaderTGA::load] Error reading image (%s)", _filename.c_str());
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

	Graphics::PixelFormat format = g_system->getScreenFormat();
	format.bytesPerPixel = _header.bits / 8;

	Graphics::Surface *surface = new Graphics::Surface();
	surface->create(_header.width, _header.height, format);

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

ImageLoaderCIN::ImageLoaderCIN() {
	_cinematic = NULL;
	memset(&_header, 0, sizeof(_header));
	_stride = 0;
	_widthAndPadding = 0;
	_width = 0;
	_height = 0;
}


ImageLoaderCIN::~ImageLoaderCIN() {
	deinit();
}

bool ImageLoaderCIN::load(Image *image, ArchiveType, ZoneId, LoadFrom, DrawType) {
	if (!image)
		error("[ImageLoaderCNM::load] Invalid image pointer!");

	byte format = 0;
	_filename = image->getName();

	if (!init(_filename)){
		warning("[ImageLoaderCIN::load] Error initializing image reader (%s)", _filename.c_str());
		goto cleanup;
	}

	// Check cinematic format
	format = _cinematic->readByte();
	if (_cinematic->eos() || _cinematic->err() || format != CINEMATIC_FORMAT) {
		warning("[ImageLoaderCIN::load] Wrong cinematic format for %s (was: %d, valid: %d)", _filename.c_str(), format, CINEMATIC_FORMAT);
		goto cleanup;
	}

	// Read image data
	if (!readImage(image)) {
		warning("[ImageLoaderCIN::load] Error reading image (%s)", _filename.c_str());
		goto cleanup;
	}

	deinit();

	return true;

cleanup:
	deinit();
	return false;
}

bool ImageLoaderCIN::init(Common::String filename) {
	_cinematic = new Cinematic();

	if (!_cinematic->init(filename)) {
		warning("[ImageLoaderCIN::init] Error initializing cinematic (%s)", _filename.c_str());
		return false;
	}

	if (!readHeader()) {
		warning("[ImageLoaderCIN::init] Error reading header (%s)", _filename.c_str());
		return false;
	}

	return true;
}

void ImageLoaderCIN::deinit() {
	SAFE_DELETE(_cinematic);
}

bool ImageLoaderCIN::readHeader() {
	if (!_cinematic)
		error("[ImageLoaderCIN::readHeader] Cinematic not initialized properly");

	memset(&_header, 0, sizeof(_header));

	// Read header (size: 0x40)
	_header.field_0    = _cinematic->readUint32LE();
	_header.field_4    = _cinematic->readUint32LE();
	_header.field_8    = _cinematic->readByte();
	_header.field_9    = _cinematic->readByte();
	_header.field_A    = _cinematic->readByte();
	_header.field_B    = _cinematic->readByte();
	_header.field_C    = _cinematic->readUint16LE();
	_header.chunkCount = _cinematic->readUint32LE();
	_header.field_12   = _cinematic->readUint16LE();
	_header.field_16   = _cinematic->readByte();
	_header.width      = _cinematic->readUint32LE();
	_header.height     = _cinematic->readUint32LE();
	_header.field_1F   = _cinematic->readByte();
	_header.field_20   = _cinematic->readUint32LE();
	_header.field_24   = _cinematic->readUint32LE();
	_header.field_28   = _cinematic->readUint32LE();
	_header.field_2C   = _cinematic->readUint32LE();
	_header.field_30   = _cinematic->readUint32LE();
	_header.field_34   = _cinematic->readUint32LE();
	_header.field_38   = _cinematic->readUint32LE();
	_header.field_3C   = _cinematic->readUint32LE();

	// Update width and height
	_width = _header.width;
	_height = _header.height;

	return true;
}

bool ImageLoaderCIN::readImage(Image *image) {
	if (!_cinematic)
		error("[ImageLoaderCIN::readImage] Cinematic not initialized properly");

	if (!image)
		error("[ImageLoaderCNM::readImage] Invalid image pointer!");

	if (!image->isInitialized())
		error("[ImageLoaderCNM::readImage] Invalid image!");

	Graphics::PixelFormat format = g_system->getScreenFormat();
	format.bytesPerPixel = 2;

	// TODO Create image instead
	// Create surface to hold the data
	Graphics::Surface *surface = new Graphics::Surface();
	surface->create((uint16)_width, (uint16)_height, format);

	// Compute stride
	_widthAndPadding = _width + 3;
	_stride = _widthAndPadding * 3;

	if (!_cinematic->sControl((byte *)surface->pixels))
		error("[ImageLoaderCIN::readImage] Cannot read image");

	delete surface;

	return true;
}

#pragma endregion

#pragma region CI2

bool ImageLoaderCI2::SoundTable::read(Common::SeekableReadStream *stream) {
	_field_0 = stream->readByte();
	_field_1 = stream->readByte();
	_field_4 = stream->readUint32LE();
	_field_6 = stream->readUint16LE();
	_field_8 = stream->readUint32LE();
	_field_C = stream->readUint32LE();

	return (!stream->err() && !stream->eos());
}

ImageLoaderCI2::ImageLoaderCI2() {
	_cinematic = NULL;
	memset(&_header, 0, sizeof(_header));
	memset(_soundTables, 0, ARRAYSIZE(_soundTables));
	_controlTable = NULL;
	_widthAndPadding = 0;
	_stride = 0;
	_width = 0;
	_height = 0;
}

ImageLoaderCI2::~ImageLoaderCI2() {
	deinit();
}

bool ImageLoaderCI2::load(Image *image, ArchiveType archiveType, ZoneId zone, LoadFrom loadFrom, DrawType drawType) {
	if (!image)
		error("[ImageLoaderCI2::load] Invalid image pointer");

	byte format = 0;
	_filename = image->getName();

	if (!init(_filename, archiveType, zone, loadFrom)) {
		warning("[ImageLoaderCI2::load] Error initializing image reader (%s)", _filename.c_str());
		goto cleanup;
	}

	// Check cinematic format
	format = _cinematic->readByte();
	if (_cinematic->eos() || _cinematic->err() || format != CINEMATIC_FORMAT) {
		warning("[ImageLoaderCI2::load] Wrong cinematic format for %s (was: %d, valid: %d)", _filename.c_str(), format, CINEMATIC_FORMAT);
		goto cleanup;
	}

	// Read image data
	if (!readImage(image, 32, drawType)) {
		warning("[ImageLoaderCI2::load] Error reading image (%s)", _filename.c_str());
		goto cleanup;
	}

	deinit();

	return true;

cleanup:
	deinit();
	return false;
}

bool ImageLoaderCI2::init(Common::String filename, ArchiveType archiveType, ZoneId zone, LoadFrom loadFrom) {
	_controlTable = NULL;
	_cinematic = new Cinematic2();

	if (!_cinematic->init(filename, archiveType, zone, loadFrom)) {
		warning("[ImageLoaderCI2::init] Error initializing cinematic (%s)", _filename.c_str());
		return false;
	}

	if (!readHeader()) {
		warning("[ImageLoaderCI2::init] Error reading header (%s)", _filename.c_str());
		return false;
	}

	if (!_cinematic->allocBuffer(_header.width * _header.height + 128)) {
		warning("[ImageLoaderCIN::init] Error allocating cinematic buffer (%s)", _filename.c_str());
		return false;
	}

	if (_header.field_1A)
		_cinematic->setField5404C();

	return true;
}

bool ImageLoaderCI2::readHeader() {
	if (!_cinematic)
		error("[ImageLoaderCI2::readHeader] Cinematic not initialized properly");

	memset(&_header, 0, sizeof(_header));

	// Read header (size: 0xC0)
	_header.field_0           = _cinematic->readUint32LE();
	_header.field_4           = _cinematic->readUint32LE();
	_header.field_8           = _cinematic->readUint32LE();
	_header.field_C           = _cinematic->readByte();
	_header.field_D           = _cinematic->readUint32LE();
	_header.width             = _cinematic->readUint32LE();
	_header.height            = _cinematic->readUint32LE();
	_header.field_19          = _cinematic->readByte();
	_header.field_1A          = _cinematic->readByte();
	_header.soundChannelCount = _cinematic->readByte();
	_header.controlTableSize  = _cinematic->readUint32LE();
	_header.field_20          = _cinematic->readUint32LE();
	_header.field_24          = _cinematic->readUint32LE();
	_header.field_28          = _cinematic->readUint32LE();
	_header.field_2C          = _cinematic->readUint32LE();
	_header.field_30          = _cinematic->readUint32LE();
	_header.field_34          = _cinematic->readUint32LE();
	_header.field_38          = _cinematic->readUint32LE();
	_header.field_3C          = _cinematic->readUint32LE();
	_header.field_40          = _cinematic->readUint32LE();
	_header.field_44          = _cinematic->readUint32LE();
	_header.field_48          = _cinematic->readUint32LE();
	_header.field_4C          = _cinematic->readUint32LE();
	_header.field_50          = _cinematic->readUint32LE();
	_header.field_54          = _cinematic->readUint32LE();
	_header.field_58          = _cinematic->readUint32LE();
	_header.field_5C          = _cinematic->readUint32LE();
	_header.field_60          = _cinematic->readUint32LE();
	_header.field_64          = _cinematic->readUint32LE();
	_header.field_68          = _cinematic->readUint32LE();
	_header.field_6C          = _cinematic->readUint32LE();
	_header.field_70          = _cinematic->readUint32LE();
	_header.field_74          = _cinematic->readUint32LE();
	_header.field_78          = _cinematic->readUint32LE();
	_header.field_7C          = _cinematic->readUint32LE();
	_header.field_80          = _cinematic->readUint32LE();
	_header.field_84          = _cinematic->readUint32LE();
	_header.field_88          = _cinematic->readUint32LE();
	_header.field_8C          = _cinematic->readUint32LE();
	_header.field_90          = _cinematic->readUint32LE();
	_header.field_94          = _cinematic->readUint32LE();
	_header.field_98          = _cinematic->readUint32LE();
	_header.field_9C          = _cinematic->readUint32LE();
	_header.field_A0          = _cinematic->readUint32LE();
	_header.field_A4          = _cinematic->readUint32LE();
	_header.field_A8          = _cinematic->readUint32LE();
	_header.field_AC          = _cinematic->readUint32LE();
	_header.field_B0          = _cinematic->readUint32LE();
	_header.field_B4          = _cinematic->readUint32LE();
	_header.field_B8          = _cinematic->readUint32LE();
	_header.field_BC          = _cinematic->readUint32LE();

	// Update width and height
	_width = _header.width;
	_height = _header.height;

	// Check width and height
	if (_width > 4096 || _height == 0) {
		warning("[ImageLoaderCIN::init] Invalid size for image %s (width:%d, height:%d)", _filename.c_str(), _width, _height);
		return false;
	}

	// Check channel count
	if (_header.soundChannelCount > 4) {
		warning("[ImageLoaderCIN::init] Invalid sound channel count for image %s (count: %d)", _filename.c_str(), _header.soundChannelCount);
		return false;
	}

	// Read sound tables
	for (uint32 i = 0; i < _header.soundChannelCount; i++)
		_soundTables[i].read(_cinematic);

	// Allocate control table
	uint32 tableSize = 8 * _header.controlTableSize;
	_controlTable = malloc(tableSize + 64);
	if (!_controlTable) {
		warning("[ImageLoaderCIN::init] Cannot allocate control table for image %s", _filename.c_str());
		return false;
	}

	// Read control table
	uint32 readData = _cinematic->read(_controlTable, tableSize);
	if (readData != tableSize || _cinematic->err() || _cinematic->eos()) {
		warning("[ImageLoaderCIN::init] Cannot read control table for image %s", _filename.c_str());
		return false;
	}

	return true;
}

bool ImageLoaderCI2::readImage(Image *image, byte bitdepth, DrawType drawType) {
	if (!_cinematic)
		error("[ImageLoaderCI2::readImage] Cinematic not initialized properly");

	if (!image)
		error("[ImageLoaderCI2::readImage] Invalid image pointer");

	if (!image->isInitialized())
		image->create(bitdepth, 2, _width, _height);

	// Compute stride
	_widthAndPadding = _width + 3;
	_stride = _widthAndPadding * 3;

	if (drawType == kDrawType3) {
		// Create buffer to hold the data
		byte *buffer = (byte *)malloc(_width * _stride + 64);
		if (!buffer) {
			warning("[ImageLoaderCI2::readImage] Cannot create buffer for image %s", _filename.c_str());
			return false;
		}

		if (!_cinematic->sControl(buffer, bitdepth)) {
			free(buffer);

			warning("[ImageLoaderCI2::readImage] Error when decompressing image %s", _filename.c_str());
			return false;
		}

		// TODO Copy to image
		warning("[ImageLoaderCI2::readImage] image copy for kDrawType3 not implemented");

		free(buffer);
	} else {
		if (!_cinematic->sControl((byte *)image->getSurface()->pixels, bitdepth)) {
			warning("[ImageLoaderCI2::readImage] Error when decompressing image %s", _filename.c_str());
			return false;
		}
	}

	return true;
}

void ImageLoaderCI2::deinit() {
	SAFE_DELETE(_cinematic);
	free(_controlTable);
	_controlTable = NULL;
}

#pragma endregion

} // End of namespace Ring
