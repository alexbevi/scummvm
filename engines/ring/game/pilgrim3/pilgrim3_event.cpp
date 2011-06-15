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

#include "ring/game/pilgrim3/pilgrim3_event.h"

#include "ring/base/application.h"

//using namespace Pilgrim3Game;

namespace Ring {

EventHandlerPilgrim3::EventHandlerPilgrim3(Application *application) : _app(application) {
}

EventHandlerPilgrim3::~EventHandlerPilgrim3() {
	// Zero-out passed pointers
	_app = NULL;
}

#pragma region Event handling

void EventHandlerPilgrim3::onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed) {
	error("[EventHandlerPilgrim3::onMouseLeftButtonUp] Not implemented (evt: %d, CTRL pressed: %d)", evt.type, isControlPressed);
}

void EventHandlerPilgrim3::onMouseLeftButtonDown(const Common::Event &evt) {
	error("[EventHandlerPilgrim3::onMouseLeftButtonDown] Not implemented (evt: %d)", evt.type);
}

void EventHandlerPilgrim3::onMouseRightButtonUp(const Common::Event &evt) {
	error("[EventHandlerPilgrim3::onMouseRightButtonUp] Not implemented (evt: %d)", evt.type);
}

void EventHandlerPilgrim3::onKeyDown(Common::Event &evt) {
	error("[EventHandlerPilgrim3::onKeyDown] Not implemented (evt: %d)", evt.type);
}

#pragma endregion

#pragma region Zone

void EventHandlerPilgrim3::onInitZone(ZoneId zone) {
	error("[EventHandlerPilgrim3::onInitZone] Not implemented (zone: %d)", zone);
}

#pragma endregion

#pragma region Timer

void EventHandlerPilgrim3::onTimer(TimerId timerId) {
	error("[EventHandlerPilgrim3::onTimer] Not implemented (id: %d)", timerId);
}

#pragma endregion

} // End of namespace Ring
