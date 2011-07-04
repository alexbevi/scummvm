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
 */

#ifndef NEVERHOOD_SCREEN_H
#define NEVERHOOD_SCREEN_H

#include "graphics/surface.h"
#include "neverhood/neverhood.h"
#include "neverhood/graphics.h"

namespace Neverhood {

class Screen {
public:
	Screen(NeverhoodEngine *vm);
	~Screen();
	void update();
	void wait();
	void setFps(int fps);
	int getFps();
	void setPaletteData(byte *paletteData);
	void unsetPaletteData(byte *paletteData);
	void testPalette(byte *paletteData);
	void updatePalette();
	void clear();
	void drawSurface2(Graphics::Surface *surface, NDrawRect &drawRect, NRect &clipRect);
protected:
	NeverhoodEngine *_vm;
	Graphics::Surface *_backScreen;
	uint32 _ticks;
	uint32 _frameDelay;
	byte *_paletteData;
	bool _paletteChanged;
};

} // End of namespace Neverhood

#endif /* NEVERHOOD_SCREEN_H */
