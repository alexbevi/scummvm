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
	virtual ~ImageLoaderBMA() {};

	virtual bool load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom);
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

	bool init(ArchiveType type, Zone zone, LoadFrom loadFrom);
	void deinit();
};

//////////////////////////////////////////////////////////////////////////
class ImageLoaderCNM : public ImageLoader {
public:
	virtual ~ImageLoaderCNM() {};

	virtual bool load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom);
};

} // End of namespace Ring

#endif // RING_IMAGELOADER_H
