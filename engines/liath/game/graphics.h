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

#ifndef LIATH_GRAPHICS_H
#define LIATH_GRAPHICS_H

#include "liath/shared.h"

namespace Liath {

class LiathEngine;

class GraphicsManager {
public:

	GraphicsManager(LiathEngine *engine);
	~GraphicsManager();

	void load(uint32 paletteOffset);
	void unload();

	// Opcodes
	OpcodeRet redraw(OpcodeParameters *parameters);
	OpcodeRet xscroll(OpcodeParameters *parameters);
	OpcodeRet yscroll(OpcodeParameters *parameters);
	OpcodeRet playAvs(OpcodeParameters *parameters, bool doStopMusic);
	OpcodeRet setBackground(OpcodeParameters *parameters);
	OpcodeRet setHeroRGB(OpcodeParameters *parameters);
	OpcodeRet look(OpcodeParameters *parameters);

private:
	struct PaletteEntry {
		byte red;
		byte green;
		byte blue;
		byte flags;
	};

	LiathEngine* _engine;

	// Palettes
	Common::Array<PaletteEntry *> _palette;
	void *_textPalette;
	void *_textPalette2;

	void *_colorTable;

	uint32 _shadow;

	void *_hMemBackgroundExt;

	void drawBMP(bool doSetBackground);

	void makeSoftTable(int16 maxSize);
	void clearSoftTable() { makeSoftTable(0); }

	void setPalette(Common::String paletteName);
	bool loadBackgroundPalette( Common::String paletteName );
};

} // End of namespace Liath

#endif // LIATH_GRAPHICS_H
