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

#include "ring/game/pilgrim3/pilgrim3_animation.h"
#include "ring/game/pilgrim3/pilgrim3_application.h"
#include "ring/game/pilgrim3/pilgrim3_bag.h"
#include "ring/game/pilgrim3/pilgrim3_button.h"
#include "ring/game/pilgrim3/pilgrim3_init.h"
#include "ring/game/pilgrim3/pilgrim3_input.h"
#include "ring/game/pilgrim3/pilgrim3_ride.h"
#include "ring/game/pilgrim3/pilgrim3_setup.h"
#include "ring/game/pilgrim3/pilgrim3_shared.h"
#include "ring/game/pilgrim3/pilgrim3_sound.h"
#include "ring/game/pilgrim3/pilgrim3_timer.h"
#include "ring/game/pilgrim3/pilgrim3_visual.h"
#include "ring/game/pilgrim3/pilgrim3_zone.h"

#include "ring/debug.h"
#include "ring/helpers.h"
#include "ring/ring.h"

//using namespace Pilgrim3Game;

namespace Ring {

EventHandlerPilgrim3::EventHandlerPilgrim3(ApplicationPilgrim3 *application) : _app(application) {
	// Event handlers
	_eventAnimation = new EventAnimationPilgrim3(application);
	_eventBag       = new EventBagPilgrim3(application);
	_eventButton    = new EventButtonPilgrim3(application);
	_eventInit      = new EventInitPilgrim3(application);
	_eventInput     = new EventInputPilgrim3(application);
	_eventRide      = new EventRidePilgrim3(application);
	_eventSetup     = new EventSetupPilgrim3(application);
	_eventSound     = new EventSoundPilgrim3(application);
	_eventTimer     = new EventTimerPilgrim3(application);
	_eventVisual    = new EventVisualPilgrim3(application);
	_eventZone      = new EventZonePilgrim3(application);
}

EventHandlerPilgrim3::~EventHandlerPilgrim3() {
	SAFE_DELETE(_eventAnimation);
	SAFE_DELETE(_eventBag);
	SAFE_DELETE(_eventButton);
	SAFE_DELETE(_eventInit);
	SAFE_DELETE(_eventInput);
	SAFE_DELETE(_eventRide);
	SAFE_DELETE(_eventSetup);
	SAFE_DELETE(_eventSound);
	SAFE_DELETE(_eventTimer);
	SAFE_DELETE(_eventVisual);
	SAFE_DELETE(_eventZone);

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
