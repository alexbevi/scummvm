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
#include "graphics/video/video_player.h"
#include "graphics/video/flic_player.h"
#include "graphics/video/smk_player.h"

#include "kom/screen.h"
#include "kom/video_player.h"

namespace Kom {

VideoPlayer::VideoPlayer(KomEngine *vm) : _vm(vm), 
	_smk(vm->_mixer) {
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

bool VideoPlayer::playVideo(const char *filename) {
	_skipVideo = false;
	byte backupPalette[256 * 4];

	// Figure out which player to use, based on extension
	_player = &_smk;

	if (!_player->loadFile(filename)) {
		warning("TODO: Could not play video %s. Try flc", filename);
		return false;
	}

	// Backup the palette
	_vm->_system->grabPalette(backupPalette, 0, 256);

	_vm->_system->fillScreen(0);

	while (_player->getCurFrame() < _player->getFrameCount() && !_skipVideo && !_vm->shouldQuit()) {
		processEvents();
		processFrame();
	}

	_player->closeFile();

	_vm->_system->fillScreen(0);
	_vm->_system->updateScreen();

	// Restore the palette
	_vm->_system->setPalette(backupPalette, 0, 256);

	return true;
}

void VideoPlayer::processFrame() {
	_player->decodeNextFrame();

	Graphics::Surface *screen = _vm->_system->lockScreen();
	_player->copyFrameToBuffer((byte *)screen->pixels, 0, 0, SCREEN_W);
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
