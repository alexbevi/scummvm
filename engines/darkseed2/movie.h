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

#ifndef DARKSEED2_MOVIE_H
#define DARKSEED2_MOVIE_H

#include "sound/mixer.h"

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/sprite.h"

namespace Common {
	class String;
}

namespace Graphics {
	class AviDecoder;
}

namespace DarkSeed2 {

class Graphics;

class Movie {
public:
	Movie(Audio::Mixer &mixer, Graphics &graphics);
	~Movie();

	/** Play that movie. */
	bool play(const Common::String &avi, uint32 x = 0, uint32 y = 0);

private:
	static const bool _doubleHalfSizedVideos = true;

	Audio::Mixer *_mixer;
	Graphics *_graphics;

	/** The AVI decoder. */
	::Graphics::AviDecoder *_aviDecoder;

	bool _abort; ///< Movie canceling requested?

	Sprite _buffer; ///< The current frame's buffer.

	/** Handle the user input. */
	void handleInput();
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_MOVIE_H
