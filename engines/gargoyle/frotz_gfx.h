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

#ifndef FROTZ_GFX_H
#define FROTZ_GFX_H

#include "common/scummsys.h"
#include "common/file.h"
#include "gargoyle/frotz.h"
#include "gargoyle/gargoyle.h"

namespace Frotz {

extern uint32 DEFAULT_COLOUR_LIST[16];

struct ZCodePicFileHeader {
    uint8 fileno;
    uint8 flags;
    uint16 unused1;
    uint16 images;
    uint16 link;
    uint8 entry_size;
    uint8 padding;
    uint16 checksum;
    uint16 unused2;
    uint16 version;
};

enum FrotzGraphicsType {
	MODE_NONE = 0,
	MODE_CGA = 1,
	MODE_EGA = 2,
	MODE_MCGA = 3
};

class FrotzPictures {
private:
	Gargoyle::GargoyleEngine *_engine;
	Common::File _gfxFp;
	ZCodePicFileHeader _header;
	byte *_gfxInfo;
	int _fileNumber;
	Common::Point _screenSize;
	int _gfxMode;
	zbyte _gameVersion;

	bool openPictureFile();
	void copyRect(Graphics::Surface &s, int16 x, int16 y, int16 width, int16 height, int16 pitch,
		double scaleX, double scaleY, const byte *bmmask, const byte *bmcontents);
	void applyPalette(uint32 *palette);
public:
	FrotzPictures(Gargoyle::GargoyleEngine *engine, const Common::Point &screenSize, zbyte gameVersion);
	~FrotzPictures();
	static bool picturesExists(const Common::String &target);

	bool getPictureData(int picNumber, int16 *width, int16 *height, int32 *data, uint32 *colour, uint32 *flags, bool retScale);
	bool getPictureInfo(int picNumber, int16 *width, int16 *height);
	void drawPicture(int picNumber, Graphics::Surface &s, int x, int y);
};

} // End of namespace Frotz

#endif

