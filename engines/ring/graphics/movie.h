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

#ifndef RING_MOVIE_H
#define RING_MOVIE_H

#include "ring/shared.h"

namespace Ring {

class Movie;
class ScreenManager;

class MovieData {
public:
	MovieData();
	~MovieData();

	void deinit();

private:
	// buffer
	// buffer 2
	byte   _field_8;
	// backbufferGlobal
	// backBuffer
	byte   _field_11;
	uint32 _field_12;
	uint32 _field_16;
	uint32 _field_1A;
	// CinTControlKosiGlobal
	// CinTControlKosi
	// cacheBufferGlobal
	// cacheBuffer
	uint32 _field_2E;
	uint32 _field_32;
	uint32 _field_36;
	uint32 _field_3A;
	// event
	bool _isStreaming; // uint32
	uint32 _field_46;
	uint32 _field_4A;
	float  _field_4E;
	bool   _isSoundInitialized;
	bool   _field_53;
	float  _framerate;
	bool   _removeDialog;
	uint32 _channel;

	friend Movie;
};

class Movie {
public:
	Movie(ScreenManager *screen);
	~Movie();

	void init(Common::String path, Common::String filename, uint32 a3, uint32 a4);
	void deinit();

	void play(uint32 a1, uint32 a2);

	void setFramerate(float rate) { _data->_framerate = rate; }

private:
	ScreenManager *_screen;

	uint32 *_imageCIN; // ImageCIN
	// global buffer

	// Data
	MovieData *_data;
};

} // End of namespace Ring

#endif // RING_MOVIE_H
