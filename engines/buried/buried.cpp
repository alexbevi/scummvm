/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * Additional copyright for this file:
 * Copyright (C) 1995 Presto Studios, Inc.
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

#include "common/scummsys.h"
#include "common/error.h"
#include "common/system.h"
#include "common/textconsole.h"
#include "engines/util.h"

#include "buried/buried.h"
#include "buried/database.h"
#include "buried/graphics.h"
#include "buried/message.h"
#include "buried/sound.h"
#include "buried/video_window.h"
#include "buried/window.h"

namespace Buried {

BuriedEngine::BuriedEngine(OSystem *syst, const BuriedGameDescription *gameDesc) : Engine(syst), _gameDescription(gameDesc) {
	_gfx = 0;
	_mainEXE = 0;
	_library = 0;
	_sound = 0;
	_timerSeed = 0;
	_mainWindow = 0;
}

BuriedEngine::~BuriedEngine() {
	delete _gfx;
	delete _mainEXE;
	delete _library;
	delete _sound;
	delete _mainWindow;
}

Common::Error BuriedEngine::run() {
	if (isTrueColor()) {
#ifndef USE_RGB_COLOR
		// Can't play 24bpp version without support
		return Common::kUnsupportedColorMode;
#else
		initGraphics(640, 480, true, 0);

		if (_system->getScreenFormat().bytesPerPixel == 1)
			return Common::kUnsupportedColorMode;
#endif
	} else {
		initGraphics(640, 480, true);
	}

	if (isWin95()) {
		error("TODO: Win95 version");
	} else if (isCompressed()) {
		_mainEXE = new DatabaseNECompressed();
		_library = new DatabaseNECompressed();
	} else {
		_mainEXE = new DatabaseNE();

		// Demo only uses the main EXE
		if (!isDemo())
			_library = new DatabaseNE();
	}

	if (!_mainEXE->load(getEXEName()))
		error("Failed to load main EXE '%s'", getEXEName().c_str());

	if (_library && !_library->load(getLibraryName()))
		error("Failed to load library DLL '%s'", getLibraryName().c_str());

	_gfx = new GraphicsManager(this);
	_sound = new SoundManager(this);

	// TODO: Event loop
	// - Dispatch window events
	// - Poll events
	// - Call UpdateWindow for the windows
	// - Update timers

	return Common::kNoError;
}

Common::String BuriedEngine::getString(uint32 stringID) {
	return _mainEXE->loadString(stringID);
}

Common::String BuriedEngine::getFilePath(uint32 stringID) {
	Common::String path = getString(stringID);
	Common::String output;

	if (path.empty())
		return output;

	uint i = 0;

	// The non-demo paths have CD info followed by a backspace
	// We ignore this
	if (!isDemo())
		i += 2;

	for (; i < path.size(); i++) {
		if (path[i] == '\\')
			output += '/';
		else
			output += path[i];
	}

	return output;
}

Common::SeekableReadStream *BuriedEngine::getBitmapStream(uint32 bitmapID) {
	// The demo's bitmaps are in the main EXE
	if (isDemo())
		return _mainEXE->getBitmapStream(bitmapID);

	// The rest in the database library
	return _library->getBitmapStream(bitmapID);
}

Common::SeekableReadStream *BuriedEngine::getNavData(uint32 resourceID) {
	return _mainEXE->getResourceStream("NAVDATA", resourceID);
}

Common::SeekableReadStream *BuriedEngine::getSndData(uint32 resourceID) {
	return _mainEXE->getResourceStream("SNDDATA", resourceID);
}

Common::SeekableReadStream *BuriedEngine::getAnimData(uint32 resourceID) {
	return _mainEXE->getResourceStream("ANIMDATA", resourceID);
}

Common::SeekableReadStream *BuriedEngine::getAIData(uint32 resourceID) {
	return _mainEXE->getResourceStream("AIDATA", resourceID);
}

Common::SeekableReadStream *BuriedEngine::getItemData(uint32 resourceID) {
	return _mainEXE->getResourceStream("ITEMDATA", resourceID);
}

Common::SeekableReadStream *BuriedEngine::getBookData(uint32 resourceID) {
	return _mainEXE->getResourceStream("BOOKDATA", resourceID);
}

Common::SeekableReadStream *BuriedEngine::getFileBCData(uint32 resourceID) {
	return _mainEXE->getResourceStream("FILEBCDATA", resourceID);
}

Common::SeekableReadStream *BuriedEngine::getINNData(uint32 resourceID) {
	return _mainEXE->getResourceStream("INNDATA", resourceID);
}

uint BuriedEngine::createTimer(Window *window, uint period) {
	uint timer = ++_timerSeed;

	Timer timerInfo;
	timerInfo.owner = window;
	timerInfo.period = period;
	timerInfo.nextTrigger = _system->getMillis() + period;

	_timers[timer] = timerInfo;
	return timer;
}

bool BuriedEngine::killTimer(uint timer) {
	TimerMap::iterator it = _timers.find(timer);

	if (it == _timers.end())
		return false;

	_timers.erase(it);
	return true;
}

void BuriedEngine::updateTimers() {
	for (TimerMap::iterator it = _timers.begin(); it != _timers.end(); it++) {
		if (g_system->getMillis() >= it->_value.nextTrigger) {
			it->_value.nextTrigger += it->_value.period;
			it->_value.owner->sendMessage(new TimerMessage(it->_key));
		}
	}
}

void BuriedEngine::addVideo(VideoWindow *window) {
	_videos.push_back(window);
}

void BuriedEngine::removeVideo(VideoWindow *window) {
	_videos.remove(window);
}

void BuriedEngine::updateVideos() {
	for (VideoList::iterator it = _videos.begin(); it != _videos.end(); it++)
		(*it)->updateVideo();
}

} // End of namespace Buried