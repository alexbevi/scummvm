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

#include "common/serializer.h"

#include "video/avi_decoder.h"
#include "video/qt_decoder.h"
#include "video/segafilm_decoder.h"

#include "darkseed2/movie.h"
#include "darkseed2/resources.h"
#include "darkseed2/palette.h"
#include "darkseed2/graphics.h"
#include "darkseed2/cursors.h"
#include "darkseed2/sound.h"
#include "darkseed2/saveload.h"

namespace DarkSeed2 {

Movie::Movie(Audio::Mixer &mixer, Graphics &graphics, Cursors &cursors, Sound &sound, Common::Platform platform) {
	_mixer    = &mixer;
	_graphics = &graphics;
	_cursors  = &cursors;
	_sound    = &sound;
	_platform = platform;

	_doubling = false;
	_cursorVisible = false;

	_x = 0;
	_y = 0;

	_decoder = 0;
	//ecoder = new ::Graphics::AviDecoder(_mixer, Audio::Mixer::kSFXSoundType);
}

Movie::~Movie() {
	stop();
	delete _decoder;
}

bool Movie::isPlaying() const {
	return _decoder && _decoder->isVideoLoaded();
}

Video::VideoDecoder *Movie::createDecoder(const Common::String &file) const {
	Common::String realFile;
	Video::VideoDecoder *decoder = 0;

	switch (_platform) {
	case Common::kPlatformWindows:
		// The Windows port uses AVI videos
		realFile = Resources::addExtension(file, "AVI");
		decoder = new Video::AviDecoder(_mixer, Audio::Mixer::kSFXSoundType);
		break;
	case Common::kPlatformSaturn:
		// The Sega Saturn port uses Sega FILM videos
		realFile = Resources::addExtension(file, "CPK");
		decoder = new Video::SegaFILMDecoder(_mixer, Audio::Mixer::kSFXSoundType);
		break;
	case Common::kPlatformMacintosh:
		// The Macintosh port uses QuickTime videos
		realFile = Common::String("movies/") + Resources::addExtension(file, "MooV");
		decoder = new Video::QuickTimeDecoder();
		break;
	case Common::kPlatformPSX:
		// TODO: Probably PlayStation streams :P
		break;
	default:
		break;
	}

	if (decoder && decoder->loadFile(realFile))
		return decoder;

	delete decoder;
	return 0;
}

bool Movie::play(const Common::String &file, int32 x, int32 y) {
	// Sanity checks
	assert((x >= 0) && (y >= 0) && (x <= 0x7FFF) & (y <= 0x7FFF));

	debugC(-1, kDebugMovie, "Playing movie \"%s\"", file.c_str());

	stop();

	_sound->pauseAll(true);

	if (!(_decoder = createDecoder(file)))
		return false;

	_area = Common::Rect(_decoder->getWidth(), _decoder->getHeight());
	_screen.create(_decoder->getWidth(), _decoder->getHeight());

	_graphics->enterMovieMode();

	_x = x;
	_y = y;

	// Check for doubling
	_doubling = false;
	if (_doubleHalfSizedVideos)
		if ((_decoder->getWidth() == 320) && (_decoder->getHeight() == 240) &&
		    (g_system->getWidth() == 640) && (g_system->getHeight() == 480))
			_doubling = true;

	if (_doubling) {
		x = 0;
		y = 0;

		_screen.setScale(2 * FRAC_ONE);
		_area = Common::Rect(_screen.getWidth(), _screen.getHeight());
	} else
		_area.moveTo(x, y);

	_cursorVisible = _cursors->isVisible();
	_cursors->setVisible(false);

	_fileName = file;

	return true;
}

void Movie::updateStatus() {
	if (!isPlaying())
		return;

	if (_decoder->endOfVideo()) {
		stop();
		return;
	}

	const ::Graphics::Surface *frame = _decoder->decodeNextFrame();

	if (frame)
		_screen.copyFrom((byte *)frame->pixels, frame->bytesPerPixel, false);

	if (_decoder->hasDirtyPalette()) {
		Palette newPalette;
		newPalette.copyFrom(_decoder->getPalette(), 256);
		_screen.setPalette(newPalette);
	}

	_graphics->requestRedraw(_area);
}

void Movie::redraw(Sprite &sprite, Common::Rect area) {
	if (!_area.intersects(area))
		return;

	area.clip(_area);

	int32 x = area.left;
	int32 y = area.top;

	area.moveTo(area.left - _area.left, area.top - _area.top);

	sprite.blit(_screen, area, x, y, false);
}

uint32 Movie::getFrameWaitTime() const {
	if (!isPlaying())
		return 0;

	return _decoder->getTimeToNextFrame();
}

void Movie::stop() {
	if (!isPlaying())
		return;

	_fileName.clear();

	_sound->pauseAll(false);

	// Restoring the cursor visibility
	_cursors->setVisible(_cursorVisible);

	_screen.clear();

	_decoder->close();

	_graphics->leaveMovieMode();

	delete _decoder;
	_decoder = 0;
}

bool Movie::saveLoad(Common::Serializer &serializer, Resources &resources) {
	SaveLoad::sync(serializer, _fileName);
	SaveLoad::sync(serializer, _x);
	SaveLoad::sync(serializer, _y);
	return true;
}

bool Movie::loading(Resources &resources) {
	play(_fileName, _x, _y);
	return true;
}

} // End of namespace DarkSeed2
