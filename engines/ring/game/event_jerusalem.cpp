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

#include "ring/game/event_jerusalem.h"

#include "ring/base/application.h"

//using namespace JerusalemGame;

namespace Ring {

EventHandlerJerusalem::EventHandlerJerusalem(Application *application) : _app(application) {
}

EventHandlerJerusalem::~EventHandlerJerusalem() {
	// Zero-out passed pointers
	_app = NULL;
}

#pragma region Event handling

void EventHandlerJerusalem::onMouseLeftButtonUp(Common::Event &evt, bool isControlPressed) {
	error("[EventHandlerJerusalem::onMouseLeftButtonUp] Not implemented");
}

void EventHandlerJerusalem::onMouseLeftButtonDown(Common::Event &evt) {
	error("[EventHandlerJerusalem::onMouseLeftButtonDown] Not implemented");
}

void EventHandlerJerusalem::onMouseRightButtonUp(Common::Event &evt) {
	error("[EventHandlerJerusalem::onMouseRightButtonUp] Not implemented");
}

void EventHandlerJerusalem::onKeyDown(Common::Event &evt) {
	error("[EventHandlerJerusalem::onKeyDown] Not implemented");
}

#pragma endregion

#pragma region Zone Setup

void EventHandlerJerusalem::onSetup(Zone zone, SetupType type) {
	error("[EventHandlerJerusalem::onSetup] Not implemented");
}

#pragma endregion

#pragma region Timer

void EventHandlerJerusalem::onTimer(TimerId timerId) {
	error("[EventHandlerJerusalem::onTimer] Not implemented");
}

#pragma endregion

#pragma region Sound

void EventHandlerJerusalem::onSound(Id id, SoundType type, uint32 a3) {
	error("[EventHandlerJerusalem::onSound] Not implemented");
}

#pragma endregion

} // End of namespace Ring
