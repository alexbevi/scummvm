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

#include "ring/game/pilgrim2/pilgrim2_event.h"

#include "ring/game/pilgrim2/pilgrim2_animation.h"
#include "ring/game/pilgrim2/pilgrim2_application.h"
#include "ring/game/pilgrim2/pilgrim2_bag.h"
#include "ring/game/pilgrim2/pilgrim2_button.h"
#include "ring/game/pilgrim2/pilgrim2_init.h"
#include "ring/game/pilgrim2/pilgrim2_input.h"
#include "ring/game/pilgrim2/pilgrim2_ride.h"
#include "ring/game/pilgrim2/pilgrim2_setup.h"
#include "ring/game/pilgrim2/pilgrim2_shared.h"
#include "ring/game/pilgrim2/pilgrim2_sound.h"
#include "ring/game/pilgrim2/pilgrim2_timer.h"
#include "ring/game/pilgrim2/pilgrim2_visual.h"
#include "ring/game/pilgrim2/pilgrim2_zone.h"

#include "ring/debug.h"
#include "ring/helpers.h"
#include "ring/ring.h"

//using namespace Pilgrim2Game;

namespace Ring {

EventHandlerPilgrim2::EventHandlerPilgrim2(ApplicationPilgrim2 *application) : _app(application) {
	// Event handlers
	_eventAnimation = new EventAnimationPilgrim2(application);
	_eventBag       = new EventBagPilgrim2(application);
	_eventButton    = new EventButtonPilgrim2(application);
	_eventInit      = new EventInitPilgrim2(application);
	_eventInput     = new EventInputPilgrim2(application);
	_eventRide      = new EventRidePilgrim2(application);
	_eventSetup     = new EventSetupPilgrim2(application);
	_eventSound     = new EventSoundPilgrim2(application);
	_eventTimer     = new EventTimerPilgrim2(application);
	_eventVisual    = new EventVisualPilgrim2(application);
	_eventZone      = new EventZonePilgrim2(application);
}

EventHandlerPilgrim2::~EventHandlerPilgrim2() {
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

void EventHandlerPilgrim2::onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed) {
	error("[EventHandlerPilgrim2::onMouseLeftButtonUp] Not implemented (evt: %d, CTRL pressed: %d)", evt.type, isControlPressed);
}

void EventHandlerPilgrim2::onMouseLeftButtonDown(const Common::Event &evt) {
	error("[EventHandlerPilgrim2::onMouseLeftButtonDown] Not implemented (evt: %d)", evt.type);
}

void EventHandlerPilgrim2::onMouseRightButtonUp(const Common::Event &evt) {
	error("[EventHandlerPilgrim2::onMouseRightButtonUp] Not implemented (evt: %d)", evt.type);
}

void EventHandlerPilgrim2::onKeyDown(Common::Event &evt) {
	error("[EventHandlerPilgrim2::onKeyDown] Not implemented (evt: %d)", evt.type);
}

#pragma endregion

#pragma region Zone

void EventHandlerPilgrim2::onInitZone(ZoneId zone) {
	error("[EventHandlerPilgrim2::onInitZone] Not implemented (zone: %d)", zone);
}

#pragma endregion

#pragma region Timer

void EventHandlerPilgrim2::onTimer(TimerId timerId) {
	error("[EventHandlerPilgrim2::onTimer] Not implemented (id: %d)", timerId);
}

#pragma endregion

} // End of namespace Ring
