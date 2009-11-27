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

#include "graphics/video/avi_decoder.h"

#include "engines/darkseed2/movie.h"
#include "engines/darkseed2/graphics.h"

namespace DarkSeed2 {

Movie::Movie(Audio::Mixer &mixer, Graphics &graphics) {
	_mixer    = &mixer;
	_graphics = &graphics;

	_aviDecoder = new ::Graphics::AviDecoder(_mixer);
}

Movie::~Movie() {
	delete _aviDecoder;
}

bool Movie::play(const Common::String &avi) {
	if (!_aviDecoder->loadFile((avi + ".avi").c_str()))
		return false;

	while (_aviDecoder->decodeNextFrame()) {
		int32 frameDelay = _aviDecoder->getFrameDelay();

		g_system->delayMillis(frameDelay);
	}

	return true;
}

} // End of namespace DarkSeed2
