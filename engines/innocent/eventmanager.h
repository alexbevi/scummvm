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

#ifndef INNOCENT_EVENTMANAGER_H
#define INNOCENT_EVENTMANAGER_H

#include "common/list.h"
#include "common/rect.h"
#include "common/singleton.h"

#include "innocent/types.h"

namespace Innocent {
//

class EventManager;
class Debugger;

/**
 * Abstract base class of everything clickable. Automatically registers itself with the event manager.
 */
class Clickable {
public:
	Clickable();
	virtual ~Clickable();

	virtual void clicked() = 0;

	virtual Common::Rect area() const = 0;
	virtual byte zIndex() const = 0;

private:
	friend class EventManager;
};

class EventManager : public Common::Singleton<EventManager>, public Paintable {
public:
	EventManager() : _debug(false) {}
	void clicked(Common::Point pos);

	void paint(Graphics *g) const;
	byte zIndex() const { return 0; }

	friend class Debugger;

private:
	void toggleDebug();

	friend class Clickable;
	void push(Clickable *c);
	void pop(Clickable *c);

	Common::List<Clickable *> _handlers;
	bool _debug;
};

}

#endif
