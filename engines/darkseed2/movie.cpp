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

#include "common/events.h"

#include "graphics/cursorman.h"

#include "graphics/video/avi_decoder.h"

#include "engines/darkseed2/movie.h"
#include "engines/darkseed2/palette.h"
#include "engines/darkseed2/graphics.h"

namespace DarkSeed2 {

Movie::Movie(Audio::Mixer &mixer, Graphics &graphics) {
	_mixer    = &mixer;
	_graphics = &graphics;

	_aviDecoder = new ::Graphics::AviDecoder(_mixer, Audio::Mixer::kSFXSoundType);
}

Movie::~Movie() {
	delete _aviDecoder;
}

bool Movie::play(const Common::String &avi, uint32 x, uint32 y) {
	debugC(-1, kDebugMovie, "Playing movie \"%s\"", avi.c_str());

	if (!_aviDecoder->loadFile((avi + ".avi").c_str()))
		return false;

	_buffer.create(_aviDecoder->getWidth(), _aviDecoder->getHeight());

	_abort = false;

	_graphics->enterMovieMode();

	bool doubl = false;
	if (_doubleHalfSizedVideos) {
		if ((_aviDecoder->getWidth() == 320) && (_aviDecoder->getHeight() == 240)) {
			doubl = true;
			x = 0;
			y = 0;
		}
	}

	// Switching off the cursor
	bool cursorVisible = CursorMan.isVisible();
	CursorMan.showMouse(false);

	while (!_abort && (_aviDecoder->getCurFrame() <= _aviDecoder->getFrameCount())) {
		_aviDecoder->decodeNextFrame();
		_aviDecoder->copyFrameToBuffer(_buffer.getData(), 0, 0, _buffer.getWidth());

		_graphics->assertPalette0();

		if (doubl)
			_graphics->blitToScreenDouble(_buffer, x, y, false);
		else
			_graphics->blitToScreen(_buffer, x, y, false);

		_graphics->retrace();

		int32 waitTime = _aviDecoder->getFrameWaitTime();
		if (waitTime > 0)
			g_system->delayMillis(waitTime);

		handleInput();
	}

	// Restoring the cursor visibility
	CursorMan.showMouse(cursorVisible);

	_buffer.clear();

	_aviDecoder->closeFile();

	_graphics->leaveMovieMode();

	return true;
}

void Movie::handleInput() {
	Common::Event event;

	while (g_system->getEventManager()->pollEvent(event)) {
		switch (event.type) {
		case Common::EVENT_KEYDOWN:
			if (event.kbd.keycode == Common::KEYCODE_ESCAPE)
				_abort = true;
			break;

		case Common::EVENT_QUIT:
			_abort = true;
			break;

		default:
			break;

		}
	}
}

} // End of namespace DarkSeed2
