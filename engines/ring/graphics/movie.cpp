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

#include "ring/graphics/movie.h"

#include "ring/graphics/screen.h"

#include "ring/helpers.h"

namespace Ring {

#pragma region MovieData

MovieData::MovieData() {
	// buffer = NULL;
	// buffer2 = NULL;
	_field_8 = 0;
	// backbufferGlobal = NULL;
	// backBuffer = NULL;
	_field_11 = 0;
	_field_12 = 0;
	_field_16 = 0;
	_field_1A = 0;
	// CinTControlKosiGlobal = NULL;
	// CinTControlKosi = NULL;
	// cacheBufferGlobal = NULL;
	// cacheBuffer = NULL;
	_field_2E = 0;
	_field_32 = 0;
	_field_36 = 0;
	_field_3A = 0;
	// event
	_isStreaming = false;
	_field_46 = 0;
	_field_4A = 0;
	_field_4E = 1.0f;
	_isSoundInitialized = false;
	_field_53 = 0;
	_framerate = 0.0f;
	_removeDialog = false;
	_channel = 0;
}

MovieData::~MovieData() {
	deinit();
}

void MovieData::deinit() {
	warning("[MovieData::deinit] Not implemented!");
}

#pragma endregion

#pragma region Movie

Movie::Movie(ScreenManager *screen) : _screen(screen) {
	_imageCIN = NULL;
	// bufferGlobal = NULL;

	_data = new MovieData();
}

Movie::~Movie() {
	deinit();

	SAFE_DELETE(_data);

	// Zero-out passed pointers
	_screen = NULL;
}

void Movie::init(Common::String path, Common::String filename, uint32 a3, uint32 a4) {
	warning("[Movie::init] Not implemented");
}

void Movie::deinit() {
	SAFE_DELETE(_imageCIN);

	_data->_isSoundInitialized = true;
	_data->_field_53 = true;
}

void Movie::play(uint32 a1, uint32 a2) {
	warning("[Movie::play] Not implemented");
}

#pragma endregion

} // End of namespace Ring
