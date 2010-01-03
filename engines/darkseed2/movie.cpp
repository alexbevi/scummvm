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
#include "engines/darkseed2/sound.h"

namespace DarkSeed2 {

Movie::Movie(Audio::Mixer &mixer, Graphics &graphics, Sound &sound) {
	_mixer    = &mixer;
	_graphics = &graphics;
	_sound    = &sound;

	_doubling = false;
	_cursorVisible = false;

	_aviDecoder = new ::Graphics::AviDecoder(_mixer, Audio::Mixer::kSFXSoundType);
}

Movie::~Movie() {
	stop();
	delete _aviDecoder;
}

bool Movie::isPlaying() const {
	return _aviDecoder->isVideoLoaded();
}

bool Movie::play(const Common::String &avi, uint32 x, uint32 y) {
	debugC(-1, kDebugMovie, "Playing movie \"%s\"", avi.c_str());

	stop();

	_sound->pauseAll(true);

	if (!_aviDecoder->loadFile((avi + ".avi").c_str())) {
		stop();
		return false;
	}

	_area = Common::Rect(_aviDecoder->getWidth(), _aviDecoder->getHeight());
	_buffer.create(_aviDecoder->getWidth(), _aviDecoder->getHeight());

	_graphics->enterMovieMode();

	// Check for doubling
	_doubling = false;
	if (_doubleHalfSizedVideos)
		if ((_aviDecoder->getWidth() == 320) && (_aviDecoder->getHeight() == 240))
			_doubling = true;

	if (_doubling) {
		x = 0;
		y = 0;

		_buffer.setScale(2 * FRAC_ONE);
		_area = Common::Rect(_buffer.getWidth(), _buffer.getHeight());
	} else
		_area.moveTo(x, y);

	_cursorVisible = CursorMan.isVisible();
	CursorMan.showMouse(false);

	return true;
}

void Movie::updateStatus() {
	if (!isPlaying())
		return;

	if (_aviDecoder->getCurFrame() >= _aviDecoder->getFrameCount()) {
		stop();
		return;
	}

	_aviDecoder->decodeNextFrame();
	_aviDecoder->copyFrameToBuffer(_buffer.getData(), 0, 0, _buffer.getWidth(true));

	_graphics->requestRedraw(_area);
}

void Movie::redraw(Sprite &sprite, Common::Rect area) {
	if (!_area.intersects(area))
		return;

	area.clip(_area);

	uint32 x = area.left;
	uint32 y = area.top;

	area.moveTo(area.left - _area.left, area.top - _area.top);

	sprite.blit(_buffer, area, x, y, false);
}

uint32 Movie::getFrameWaitTime() const {
	if (!isPlaying())
		return 0;

	return _aviDecoder->getFrameWaitTime();
}

void Movie::stop() {
	if (!isPlaying())
		return;

	_sound->pauseAll(false);

	// Restoring the cursor visibility
	CursorMan.showMouse(_cursorVisible);

	_buffer.clear();

	_aviDecoder->closeFile();

	_graphics->leaveMovieMode();
}

} // End of namespace DarkSeed2
