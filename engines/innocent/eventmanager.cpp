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

#include "innocent/eventmanager.h"

#include "innocent/debug.h"
#include "innocent/graphics.h"
#include "innocent/util.h"

using namespace std;

namespace Common {
	DECLARE_SINGLETON(Innocent::EventManager);
}

namespace Innocent {
//

Clickable::Clickable() {
	EventManager::instance().push(this);
}

Clickable::~Clickable() {
	EventManager::instance().pop(this);
}

void EventManager::clicked(Common::Point pos) {
	Clickable *handler = 0;

	foreach(Clickable *, _handlers)
		if ((*it)->area().contains(pos))
			if (!handler || handler->zIndex() > (*it)->zIndex())
				handler = *it;

	if (handler)
		handler->clicked();
}

void EventManager::push(Clickable *c) {
	_handlers.push_back(c);
}

void EventManager::pop(Clickable *c) {
	_handlers.remove(c);
}

void EventManager::paint(Graphics *g) const {
	debugC(3, kDebugLevelEvents | kDebugLevelGraphics, "EventManager got paint event");
	if (!_debug)
		return;
	debugC(3, kDebugLevelEvents | kDebugLevelGraphics, "EventManager paints clickable areas");

	foreach_const(Clickable *, _handlers)
		g->paintRect((*it)->area());
}

void EventManager::toggleDebug() {
	_debug = !_debug;
	debugC(3, kDebugLevelEvents, "EventManager toggled debug mode to %s", _debug ? "on" : "off");

	if (_debug)
		Graphics::instance().push(this);
	else
		Graphics::instance().pop(this);
}

}
