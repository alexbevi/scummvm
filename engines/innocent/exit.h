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

#ifndef INNOCENT_EXIT_H
#define INNOCENT_EXIT_H

#include <memory>

#include "common/rect.h"

#include "innocent/debug.h"
#include "innocent/eventmanager.h"
#include "innocent/value.h"

namespace Innocent {
//
class Graphics;
class Sprite;

class Exit : public StaticInspectable, public Clickable {
	DEBUG_INFO
public:
	enum { Size = 0xe };

	uint16 room() const { return _room; }

	void paint(Graphics *g);

	virtual void clicked();

	virtual Common::Rect area() const;
	virtual byte zIndex() const;

	bool isEnabled() const { return _enabled; }
	void setEnabled(bool en) { _enabled = en; }

	friend class Program;
private:
	Exit(const CodePointer &code);

	Sprite *sprite() const { return _sprite.get(); }

	byte _zIndex;
	std::auto_ptr<Sprite> _sprite;
	Common::Point _position;
	Common::Rect _rect;
	uint16 _room;
	CodePointer _clickHandler;
	bool _enabled;
};

}

#endif
