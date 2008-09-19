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

#ifndef INNOCENT_MOVIE_H
#define INNOCENT_MOVIE_H

#include "common/stream.h"

#include "innocent/resources.h"

namespace Innocent {
//
class Movie {
public:
	virtual ~Movie();
	static Movie *fromFile(const char *name);
	void setFrameDelay(uint delay);
	bool play();

protected:
	virtual void showFrame();
	virtual void setPalette();
	virtual void delay();

	Surface _s;
	byte _pal[0x400];
private:
	Movie();
	Movie(Common::ReadStream *);
	Movie(const Movie &);
	Movie &operator=(const Movie &);

	bool findKeyFrame();
	void loadKeyFrame();
	void loadIFrame();

	int _delay, _iFrames;
	Common::ReadStream *_f;
};

}

#endif
