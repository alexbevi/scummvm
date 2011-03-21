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

#ifndef RING_IMAGELOADER_H
#define RING_IMAGELOADER_H

#include "ring/shared.h"

namespace Ring {

class Cinematic;
class CompressedStream;
class Image;

//////////////////////////////////////////////////////////////////////////
class ImageLoader {
public:
	virtual ~ImageLoader() {};

	virtual bool load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom) = 0;

protected:
	Common::String _filename;
};

//////////////////////////////////////////////////////////////////////////
class ImageLoaderBMP : public ImageLoader {
public:
	virtual ~ImageLoaderBMP() {};

	virtual bool load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom);
};

//////////////////////////////////////////////////////////////////////////
class ImageLoaderBMA : public ImageLoader {
public:
	virtual ~ImageLoaderBMA();

	virtual bool load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom);

private:
	struct Header {
		uint16 coreWidth;
		uint16 coreHeight;
		uint32 seqWidth;
		uint32 seqHeight;
		uint32 field_C;
		uint16 field_10;
	};

	Header _header;
	uint32 _coreSize;
	uint32 _seqSize;
	uint32 _blockSize;

	CompressedStream *_stream;

	bool init(ArchiveType type, Zone zone, LoadFrom loadFrom);
	void deinit();
	bool readHeader();
	bool readImage(Image *image);
};

//////////////////////////////////////////////////////////////////////////
class ImageLoaderTGA : public ImageLoader {
public:
	virtual ~ImageLoaderTGA() {};

	virtual bool load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom);

protected:
	enum ImageType {
		kImageTypeNone,
		kImageTypeIndexed,
		kImageTypeRGB,
		kImageTypeGrey,
		kImageTypeRLE
	};

	struct Header {
		byte   identsize;          ///< size of ID field that follows 18 byte header (0 usually)
		byte   colourmaptype;      ///< type of colour map 0=none, 1=has palette
		byte   imagetype;          ///< type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

		uint16 colourmapstart;     ///< first color map entry in palette
		uint16 colourmaplength;    ///< number of colors in palette
		byte   colourmapbits;      ///< number of bits per palette entry 15,16,24,32

		uint16 xstart;             ///< image x origin
		uint16 ystart;             ///< image y origin
		uint16 width;              ///< image width in pixels
		uint16 height;             ///< image height in pixels
		byte   bits;               ///< image bits per pixel 8,16,24,32
		byte   descriptor;         ///< image descriptor bits (vh flip bits)
	};

	Header _header;

	bool readHeader(Common::SeekableReadStream *stream);
	bool readImage(Common::SeekableReadStream *stream, Image *image);
};

//////////////////////////////////////////////////////////////////////////
class ImageLoaderTGC : public ImageLoaderTGA {
public:
	virtual ~ImageLoaderTGC();

	virtual bool load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom);

private:
	CompressedStream *_stream;

	Common::SeekableReadStream *init(ArchiveType type, Zone zone, LoadFrom loadFrom);
	void deinit();
};

//////////////////////////////////////////////////////////////////////////
#define CINEMATIC_FORMAT 83

class ImageLoaderCIN : public ImageLoader {
public:
	struct Header {
		uint32 field_0;
		uint32 field_4;
		byte   field_8;
		byte   field_9;
		byte   field_A;
		byte   field_B;
		uint16 field_C;
		uint32 chunkCount;
		uint16 field_12;
		byte   field_16;
		uint32 width;
		uint32 height;
		byte   field_1F;
		uint32 field_20;
		uint32 field_24;
		uint32 field_28;
		uint32 field_2C;
		uint32 field_30;
		uint32 field_34;
		uint32 field_38;
		uint32 field_3C;
	};

	virtual ~ImageLoaderCIN();

	virtual bool load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom);
	bool readImage(Image *image);

	bool init(Common::String filename);

	Header *getHeader() { return &_header; }

private:
	Cinematic *_cinematic;
	Header _header;
	uint32 _field_1084;
	uint32 _field_1088;
	uint32 _width;
	uint32 _height;

	void deinit();

	bool readHeader();
};

} // End of namespace Ring

#endif // RING_IMAGELOADER_H
