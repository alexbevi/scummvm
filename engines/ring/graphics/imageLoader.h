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

class Image;

class ImageLoader {
public:
	virtual ~ImageLoader() {};

	virtual bool load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom) = 0;
};

class ImageLoaderBMP : public ImageLoader {
public:
	virtual ~ImageLoaderBMP() {};

	virtual bool load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom);
};

class ImageLoaderBMA : public ImageLoader {
public:
	virtual ~ImageLoaderBMA() {};

	virtual bool load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom);
};

class ImageLoaderTGC : public ImageLoader {
public:
	virtual ~ImageLoaderTGC() {};

	virtual bool load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom);
};

class ImageLoaderTGA : public ImageLoader {
public:
	virtual ~ImageLoaderTGA() {};

	virtual bool load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom);
};

class ImageLoaderCNM : public ImageLoader {
public:
	virtual ~ImageLoaderCNM() {};

	virtual bool load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom);
};

} // End of namespace Ring

#endif // RING_IMAGELOADER_H
