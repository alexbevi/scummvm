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

#include "ring/game/jerusalem/jerusalem_event.h"

#include "ring/game/jerusalem/jerusalem_animation.h"
#include "ring/game/jerusalem/jerusalem_application.h"
#include "ring/game/jerusalem/jerusalem_bag.h"
#include "ring/game/jerusalem/jerusalem_button.h"
#include "ring/game/jerusalem/jerusalem_init.h"
#include "ring/game/jerusalem/jerusalem_input.h"
#include "ring/game/jerusalem/jerusalem_ride.h"
#include "ring/game/jerusalem/jerusalem_setup.h"
#include "ring/game/jerusalem/jerusalem_shared.h"
#include "ring/game/jerusalem/jerusalem_sound.h"
#include "ring/game/jerusalem/jerusalem_timer.h"
#include "ring/game/jerusalem/jerusalem_visual.h"
#include "ring/game/jerusalem/jerusalem_zone.h"

#include "ring/helpers.h"
#include "ring/ring.h"

//using namespace JerusalemGame;

namespace Ring {

EventHandlerJerusalem::EventHandlerJerusalem(ApplicationJerusalem *application) : _app(application) {
	// Event handlers
	_eventAnimation = new EventAnimationJerusalem(application);
	_eventBag       = new EventBagJerusalem(application);
	_eventButton    = new EventButtonJerusalem(application);
	_eventInit      = new EventInitJerusalem(application);
	_eventInput     = new EventInputJerusalem(application);
	_eventRide      = new EventRideJerusalem(application);
	_eventSetup     = new EventSetupJerusalem(application);
	_eventSound     = new EventSoundJerusalem(application);
	_eventTimer     = new EventTimerJerusalem(application);
	_eventVisual    = new EventVisualJerusalem(application);
	_eventZone      = new EventZoneJerusalem(application);
}

EventHandlerJerusalem::~EventHandlerJerusalem() {
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

void EventHandlerJerusalem::onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed) {
	error("[EventHandlerJerusalem::onMouseLeftButtonUp] Not implemented (evt: %d, CTRL pressed: %d)", evt.type, isControlPressed);
}

void EventHandlerJerusalem::onMouseLeftButtonDown(const Common::Event &evt) {
	error("[EventHandlerJerusalem::onMouseLeftButtonDown] Not implemented (evt: %d)", evt.type);
}

void EventHandlerJerusalem::onMouseRightButtonUp(const Common::Event &evt) {
	error("[EventHandlerJerusalem::onMouseRightButtonUp] Not implemented (evt: %d)", evt.type);
}

void EventHandlerJerusalem::onKeyDown(Common::Event &evt) {
	error("[EventHandlerJerusalem::onKeyDown] Not implemented (evt: %d)", evt.type);
}

#pragma endregion

#pragma region Zone

void EventHandlerJerusalem::onInitZone(ZoneId zone) {
	error("[EventHandlerJerusalem::onInitZone] Not implemented (zone: %d)", zone);
}

#pragma endregion

#pragma region Timer

void EventHandlerJerusalem::onTimer(TimerId timerId) {
	error("[EventHandlerJerusalem::onTimer] Not implemented (id: %d)", timerId);
}

#pragma endregion

} // End of namespace Ring
