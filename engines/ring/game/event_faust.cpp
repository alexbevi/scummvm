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

#include "ring/game/event_faust.h"

#include "ring/base/application.h"

//using namespace FaustGame;

namespace Ring {

EventHandlerFaust::EventHandlerFaust(Application *application) : _app(application) {
}

EventHandlerFaust::~EventHandlerFaust() {
	// Zero-out passed pointers
	_app = NULL;
}

#pragma region Event handling

void EventHandlerFaust::onMouseLeftButtonUp(Common::Event &evt, bool isControlPressed) {
	error("[EventHandlerFaust::onMouseLeftButtonUp] Not implemented");
}

void EventHandlerFaust::onMouseLeftButtonDown(Common::Event &evt) {
	error("[EventHandlerFaust::onMouseLeftButtonDown] Not implemented");
}

void EventHandlerFaust::onMouseRightButtonUp(Common::Event &evt) {
	error("[EventHandlerFaust::onMouseRightButtonUp] Not implemented");
}

void EventHandlerFaust::onKeyDown(Common::Event &evt) {
	error("[EventHandlerFaust::onKeyDown] Not implemented");
}

#pragma endregion

#pragma region Zone Setup

void EventHandlerFaust::onSetup(Zone zone, SetupType type) {
	error("[EventHandlerFaust::onSetup] Not implemented");
}

#pragma endregion

#pragma region Timer

void EventHandlerFaust::onTimer(TimerId timerId) {
	error("[EventHandlerFaust::onTimer] Not implemented");
}

#pragma endregion

#pragma region Sound

void EventHandlerFaust::onSound(Id id, SoundType type, uint32 a3) {
	error("[EventHandlerFaust::onSound] Not implemented");
}

#pragma endregion

} // End of namespace Ring
