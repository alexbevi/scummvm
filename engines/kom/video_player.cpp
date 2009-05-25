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

#include "graphics/surface.h"
#include "graphics/video/flic_decoder.h"
#include "graphics/video/smk_decoder.h"

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
		case Common::EVENT_RTL:
		case Common::EVENT_QUIT:
			_skipVideo = true;
			break;
		default:
			break;
		}
	}
}

bool VideoPlayer::playVideo(char *filename) {
	_skipVideo = false;
	byte backupPalette[256 * 4];

	// Figure out which player to use, based on extension
	_player = &_smk;

	// Backup the palette
	_vm->_system->grabPalette(backupPalette, 0, 256);

	if (!_player->loadFile(filename)) {
		ColorSet *cs;
		int length = strlen(filename);
		filename[length - 5] = '0';
		filename[length - 3] = 'f';
		filename[length - 2] = 'l';
		filename[length - 1] = 'c';
		_player = &_flic;
		if (!_player->loadFile(filename))
			error("Could not load flic file: %s\n", filename);

		filename[length - 3] = 'c';
		filename[length - 2] = 'l';
		filename[length - 1] = '\0';
		cs = new ColorSet(filename);
		_vm->screen()->useColorSet(cs, 0);
		delete cs;

		filename[length - 3] = 'r';
		filename[length - 2] = 'a';
		filename[length - 1] = 'w';
		_sample.loadFile(filename);
	}

	_vm->_system->fillScreen(0);

	// Sample should continue playing after player is done
	_vm->sound()->playSampleSFX(_sample, false);

	while (_player->getCurFrame() < _player->getFrameCount() && !_skipVideo && !_vm->shouldQuit()) {
		processEvents();
		processFrame();
	}

	_player->closeFile();

	if (_background) {
		delete[] _background;
		_background = 0;
	}

	_vm->_system->fillScreen(0);
	_vm->_system->updateScreen();

	// Restore the palette
	_vm->_system->setPalette(backupPalette, 0, 256);

	return true;
}

void VideoPlayer::processFrame() {
	_player->decodeNextFrame();

	Graphics::Surface *screen = _vm->_system->lockScreen();

	if (!_background) {
		_player->copyFrameToBuffer((byte *)screen->pixels, 0, 0, SCREEN_W);
	} else {
		memcpy((byte *)screen->pixels, _background, SCREEN_W * (SCREEN_H - PANEL_H));
		for (int i = 0; i < SCREEN_W * (SCREEN_H - PANEL_H); i++) {
			byte color = _player->getPixel(i);
			if (color != 0)
				((byte *)screen->pixels)[i] = color;
		}
	}

	_vm->_system->unlockScreen();

	uint32 waitTime = _player->getFrameWaitTime();

	if (!waitTime) {
		warning("dropped frame %i", _player->getCurFrame());
		return;
	}

	// Update the screen
	_vm->_system->updateScreen();

	// Wait before showing the next frame
	_vm->_system->delayMillis(waitTime);
}

} // End of namespace Kom
