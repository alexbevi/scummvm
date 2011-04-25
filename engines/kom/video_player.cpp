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

#include "common/textconsole.h"

#include "graphics/surface.h"
#include "graphics/palette.h"
#include "video/flic_decoder.h"
#include "video/smk_decoder.h"

#include "kom/screen.h"
#include "kom/video_player.h"

namespace Kom {

VideoPlayer::VideoPlayer(KomEngine *vm) : _vm(vm), 
	_smk(vm->_mixer), _background(0) {
	_eventMan = _vm->_system->getEventManager();
}

void VideoPlayer::processEvents() {
	Common::Event curEvent;

	while (_eventMan->pollEvent(curEvent)) {
		switch (curEvent.type) {
		case Common::EVENT_KEYDOWN:
			if (curEvent.kbd.keycode == Common::KEYCODE_SPACE)
				_skipVideo = true;
			break;
		case Common::EVENT_RBUTTONDOWN:
			_skipVideo = true;
			break;

		case Common::EVENT_RTL:
		case Common::EVENT_QUIT:
			_skipVideo = true;
			_vm->quitGame();
			break;
		default:
			break;
		}
	}
}

bool VideoPlayer::playVideo(char *filename) {
	_skipVideo = false;
	byte backupPalette[256 * 3];

	// Backup the palette
	_vm->_system->getPaletteManager()->grabPalette(backupPalette, 0, 256);

	// Figure out which player to use, based on extension
	int length = strlen(filename);

	if (filename[length - 3] == 's' &&
		filename[length - 2] == 'm' &&
		filename[length - 1] == 'k') {

		_player = &_smk;

		if (!_player->loadFile(filename))
			error("Could not load video file: %s\n", filename);

	// Flic files have associated color set and audio files
	} else {
		_player = &_flic;

		if (!_player->loadFile(filename))
			error("Could not load video file: %s\n", filename);

		filename[length - 3] = 'c';
		filename[length - 2] = 'l';
		filename[length - 1] = '\0';
		ColorSet *cs = new ColorSet(filename);
		_vm->screen()->useColorSet(cs, 0);
		delete cs;

		filename[length - 3] = 'r';
		filename[length - 2] = 'a';
		filename[length - 1] = 'w';
		_vm->sound()->playFileSFX(filename, &_soundHandle);

		_background = _vm->screen()->createZoomBlur(160, 100);
	}

	_vm->_system->fillScreen(0);

	while (!_player->endOfVideo() && !_skipVideo && !_vm->shouldQuit()) {
		processEvents();
		processFrame();
	}

	// Sound sample should continue playing after video is done
	// (for example, when talking to the pixie)
	if (_skipVideo)
		_vm->sound()->stopHandle(_soundHandle);

	_player->close();

	if (_background) {
		delete[] _background;
		_background = 0;
	}

	_vm->_system->fillScreen(0);
	_vm->_system->updateScreen();

	// Restore the palette
	_vm->_system->getPaletteManager()->setPalette(backupPalette, 0, 256);

	return true;
}

void VideoPlayer::processFrame() {
	const Graphics::Surface *frame = _player->decodeNextFrame();

	Graphics::Surface *screen = _vm->_system->lockScreen();

	if (_player->hasDirtyPalette() && _player == &_smk) {
		_player->setSystemPalette();
	}

	if (!_background) {
		for (uint16 y = 0; y < frame->h; y++)
			memcpy((byte *)screen->pixels + y * screen->pitch, (byte *)frame->pixels + y * frame->pitch, frame->w);

	} else {
		memcpy((byte *)screen->pixels, _background, SCREEN_W * (SCREEN_H - PANEL_H));
		for (int i = 0; i < SCREEN_W * (SCREEN_H - PANEL_H); i++) {
			byte color = ((byte *)frame->pixels)[i];
			if (color != 0)
				((byte *)screen->pixels)[i] = color;
		}
	}

	_vm->_system->unlockScreen();

	// Update the screen
	_vm->_system->updateScreen();

	// Wait before showing the next frame
	_vm->_system->delayMillis(_player->getTimeToNextFrame());
}

} // End of namespace Kom
