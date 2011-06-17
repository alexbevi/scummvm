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
 */

#include "ring/game/faust/faust_input.h"

#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_shared.h"

using namespace FaustGame;

namespace Ring {

EventInputFaust::EventInputFaust(ApplicationFaust *application) : _app(application) {
}

EventInputFaust::~EventInputFaust() {
	// Zero-out passed pointers
	_app   = NULL;
}

void EventInputFaust::onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed) {
	error("[EventInputFaust::onMouseLeftButtonUp] Not implemented (evt: %d, CTRL pressed: %d)", evt.type, isControlPressed);
}

void EventInputFaust::onMouseLeftButtonDown(const Common::Event &evt) {
	error("[EventInputFaust::onMouseLeftButtonDown] Not implemented (evt: %d)", evt.type);
}

void EventInputFaust::onMouseRightButtonUp(const Common::Event &evt) {
	error("[EventInputFaust::onMouseRightButtonUp] Not implemented (evt: %d)", evt.type);
}

void EventInputFaust::onKeyDown(Common::Event &evt) {
	error("[EventInputFaust::onKeyDown] Not implemented (evt: %d)", evt.type);
}

void EventInputFaust::onKeyDownZone(const Common::KeyState &keyState) {
	error("[EventInputFaust::onKeyDownZone] Not implemented");

	// Original goes into switch for all zones if state is not kStateShowMenu, but all functions are empty
}

} // End of namespace Ring
