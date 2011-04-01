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

#ifndef DGDS_MOVIE_H_
#define DGDS_MOVIE_H_

#include "common/str-array.h"

#include "dgds/resource.h"

namespace Dgds {

struct MovieChunk {
	uint16 code;
	Common::String name;
	Common::Array<int32> data;
};

enum {
	kChunkSaveBackground = 0x0020,
	kChunkDrawBackground = 0x0080,
	// kChunk0x00C0 = 0x00C0,
	kChunkPurgeSavedImages = 0x0110,
	// kChunk0500 = 0x0500,
	// kChunk0510 = 0x0510,
	kChunkUpdate = 0x0FF0,
	kChunkDelay = 0x1020,
	kChunkSelectImage = 0x1050,
	kChunkSelectPalette = 0x1060,
	// kChunk1070 = 0x1070,
	// kChunk1100 = 0x1100,
	kChunkSetScene = 0x1110,
	//kChunk1200 = 0x1200,
	kChunkSetFrame_1 = 0x2000,
	kChunkSetFrame_2 = 0x2010,
	// kChunk2300 = 0x2300,
	// kChunk2310 = 0x2310,
	// kChunk2320 = 0x2320,
	// kChunk2400 = 0x2400,
	kChunkSetWindow_1 =0x4000,
	kChunkFadeOut = 0x4110,
	kChunkFadeIn = 0x4120,
	kChunkSaveImage_1 = 0x4200,
	kChunkSaveImage_2 = 0x4210,
	// kChunkA010 = 0xA010,
	// kChunkA030 = 0xA030,
	// kChunkA090 = 0xA090,
	// kChunkA0B0 = 0xA0B0,
	kChunkSetWindow_2 = 0xA100,
	kChunkDrawSprite_1 = 0xA500,
	kChunkDrawSprite_2 = 0xA510,
	kChunkDrawSprite_3 = 0XA520,
	kChunkDrawSprite_4 = 0xA530,
	// kChunkA5A0 = 0xA5A0,
	// kChunkA600 = 0xA600,
	kChunkDrawScreen = 0xB600,
	kChunkLoadSound = 0xC020,
	kChunkSelectSound = 0xC030,
	kChunkDisableSound = 0xC040,
	kChunkPlaySound = 0xC050,
	kChunkStopSound = 0xC060,
	kChunkLoadScreen = 0xF010,
	kChunkLoadImage = 0xF020,
	// kChunkF040 = 0xF040,
	kChunkLoadPalette = 0xF050
};

class Movie {
public:
	Movie(Resource *res);
	virtual ~Movie();

	void play();

	Common::String getVersion();
	uint16 getPages() const;
	void outputChunks();

private:
	void loadVER(Resource *res);
	void loadTT3(Resource *res);
	void loadPAG(Resource *res);

	Common::String _version;
	uint16 _pages;
	Common::Array<MovieChunk *> _chunks;
};

}

#endif
