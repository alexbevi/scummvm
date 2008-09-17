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

#ifndef INNOCENT_ROOM_H
#define INNOCENT_ROOM_H

#include <vector>

#include "common/list.h"
#include "common/rect.h"

#include "innocent/actor.h"
#include "innocent/debug.h"

namespace Innocent {
//

class Exit;
class Logic;

class Room : public StaticInspectable {
//
public:
	class Rect {
	public:
		Rect() : _zindex(999) {}
		Rect(int16 z, Common::Rect r) : _zindex(z), _rect(r) {}

	private:
		int16 _zindex;
		Common::Rect _rect;
	};

	const Common::List<Exit *> &exits() const { return _exits; }

	void addActorFrame(Common::Point p, std::vector<byte> nexts);
	Actor::Frame getFrame(uint16 index) const {
		if (index >= _actorFrames.size() || index == 0)
			return Actor::Frame();
		else
			return _actorFrames[index-1];
	}

	void addRect(const Room::Rect &f) { _rects.push_back(f); }

	friend class Logic;

private:
	Room(Logic *l);

	// just in case, we'll explicitly add those if needed
	Room();
	Room(const Room &);
	Room &operator=(const Room &);

	Common::List<Exit *> _exits;
	Logic *_logic;

	std::vector<Actor::Frame> _actorFrames;
	Common::List<Rect> _rects;

	DEBUG_INFO
};

}

#endif
