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

#include "ring/game/event_pompeii.h"

#include "ring/base/application.h"

//using namespace PompeiiGame;

namespace Ring {

EventHandlerPompeii::EventHandlerPompeii(Application *application) : _app(application) {
}

EventHandlerPompeii::~EventHandlerPompeii() {
	// Zero-out passed pointers
	_app = NULL;
}

#pragma region Event handling

void EventHandlerPompeii::onMouseLeftButtonUp(Common::Event &evt, bool isControlPressed) {
	error("[EventHandlerPompeii::onMouseLeftButtonUp] Not implemented");
}

void EventHandlerPompeii::onMouseLeftButtonDown(Common::Event &evt) {
	error("[EventHandlerPompeii::onMouseLeftButtonDown] Not implemented");
}

void EventHandlerPompeii::onMouseRightButtonUp(Common::Event &evt) {
	error("[EventHandlerPompeii::onMouseRightButtonUp] Not implemented");
}

void EventHandlerPompeii::onKeyDown(Common::Event &evt) {
	error("[EventHandlerPompeii::onKeyDown] Not implemented");
}

#pragma endregion

#pragma region Zone Setup

void EventHandlerPompeii::onSetup(Zone zone, uint32 a2) {
	error("[EventHandlerPompeii::onSetup] Not implemented");
}

#pragma endregion

#pragma region Timer

void EventHandlerPompeii::onTimer(TimerId timerId) {
	error("[EventHandlerPompeii::onTimer] Not implemented");
}

#pragma endregion

#pragma region Sound

void EventHandlerPompeii::onSound(Id id, SoundType type, uint32 a3) {
	error("[EventHandlerPompeii::onSound] Not implemented");
}

#pragma endregion

} // End of namespace Ring
