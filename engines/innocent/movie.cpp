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

#include "innocent/movie.h"

#include "common/file.h"
#include "common/system.h"

#include "innocent/debug.h"
#include "innocent/graphics.h"
#include "innocent/logic.h"
#include "innocent/innocent.h"
#include "innocent/resources.h"

namespace Innocent {
//

Movie *Movie::fromFile(const char *name) {
	Common::File *f = new Common::File;
	f->open(name);
	return new Movie(f);
}

Movie::Movie(Common::ReadStream *s) : _f(s) {}

Movie::~Movie() {
	delete _f;
}

void Movie::setFrameDelay(uint jiffies) {
	_delay = jiffies;
}

bool Movie::play() {
	_s.create(320, 200, ::Graphics::PixelFormat::createFormatCLUT8());

	debugC(4, kDebugLevelGraphics, "creating movie");
	while (findKeyFrame()) {
		loadKeyFrame();

		setPalette();
		showFrame();

		while (_iFrames) {
			debugC(3, kDebugLevelGraphics, "got %d iframes", _iFrames);
			loadIFrame();
			showFrame();
			delay();
			if ((Log.canSkipCutscene() && Eng.escapePressed()) || Eng.shouldQuit()) {
				Log.skipCutscene();
				return false;
			}
		}
	}
	return true;
}

bool Movie::findKeyFrame() {
	(void) _f->readUint32LE(); // size of block, we don't want that
	_iFrames = _f->readUint16LE();
	return !_f->eos();
}

void Movie::loadKeyFrame() {
	(void) _f->readUint16LE(); // no idea what that is

	uint16 w, h;
	w = _f->readUint16LE();
	h = _f->readUint16LE();
	assert (w == 320 && h == 200);

	Resources::decodeImage(_f, reinterpret_cast<byte *>(_s.pixels), w * h);

	(void) _f->readByte();
	Resources::readPalette(_f, _pal);
	_iFrames--;
}

void Movie::loadIFrame() {
	(void) _f->readUint16LE();

	byte skipB, skipW;
	skipB = _f->readByte();
	skipW = _f->readByte();

	assert(_s.pitch == 320);

	int left = 320 * 200;
	byte *dest = reinterpret_cast<byte *>(_s.pixels);

	while (left) {
		byte b = _f->readByte();

		uint16 s;
		if (b == skipB && (s = _f->readByte())) {
			dest += s;
			left -= s;
			continue;
		} else if (b == skipW && (s = _f->readUint16LE())) {
			dest += s;
			left -= s;
			continue;
		}

		*dest++ = b;
		left--;
	}

	_iFrames--;
}

void Movie::showFrame() {
	Engine::instance()._system->copyRectToScreen(
		reinterpret_cast<byte *>(_s.pixels), _s.pitch, 0, 0, _s.w, _s.h);

	Engine::instance()._system->updateScreen();
}

void Movie::setPalette() {
	Graf.setPalette(_pal, 0, 256);
}

void Movie::delay() {
	Engine::instance().delay(40 * (1+_delay));
}

} // end of namespace Innocent
