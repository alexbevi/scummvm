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

#include "ring/game/pompeii/pompeii_event.h"

#include "ring/game/pompeii/pompeii_animation.h"
#include "ring/game/pompeii/pompeii_application.h"
#include "ring/game/pompeii/pompeii_bag.h"
#include "ring/game/pompeii/pompeii_button.h"
#include "ring/game/pompeii/pompeii_init.h"
#include "ring/game/pompeii/pompeii_input.h"
#include "ring/game/pompeii/pompeii_ride.h"
#include "ring/game/pompeii/pompeii_setup.h"
#include "ring/game/pompeii/pompeii_shared.h"
#include "ring/game/pompeii/pompeii_sound.h"
#include "ring/game/pompeii/pompeii_timer.h"
#include "ring/game/pompeii/pompeii_visual.h"
#include "ring/game/pompeii/pompeii_zone.h"

#include "ring/debug.h"
#include "ring/helpers.h"
#include "ring/ring.h"

//using namespace PompeiiGame;

namespace Ring {

EventHandlerPompeii::EventHandlerPompeii(ApplicationPompeii *application) : _app(application) {
	// Event handlers
	_eventAnimation = new EventAnimationPompeii(application);
	_eventBag       = new EventBagPompeii(application);
	_eventButton    = new EventButtonPompeii(application);
	_eventInit      = new EventInitPompeii(application);
	_eventInput     = new EventInputPompeii(application);
	_eventRide      = new EventRidePompeii(application);
	_eventSetup     = new EventSetupPompeii(application);
	_eventSound     = new EventSoundPompeii(application);
	_eventTimer     = new EventTimerPompeii(application);
	_eventVisual    = new EventVisualPompeii(application);
	_eventZone      = new EventZonePompeii(application);
}

EventHandlerPompeii::~EventHandlerPompeii() {
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

void EventHandlerPompeii::onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed) {
	error("[EventHandlerPompeii::onMouseLeftButtonUp] Not implemented (evt: %d, CTRL pressed: %d)", evt.type, isControlPressed);
}

void EventHandlerPompeii::onMouseLeftButtonDown(const Common::Event &evt) {
	error("[EventHandlerPompeii::onMouseLeftButtonDown] Not implemented (evt: %d)", evt.type);
}

void EventHandlerPompeii::onMouseRightButtonUp(const Common::Event &evt) {
	error("[EventHandlerPompeii::onMouseRightButtonUp] Not implemented (evt: %d)", evt.type);
}

void EventHandlerPompeii::onKeyDown(Common::Event &evt) {
	error("[EventHandlerPompeii::onKeyDown] Not implemented (evt: %d)", evt.type);
}

#pragma endregion

#pragma region Zone

void EventHandlerPompeii::onInitZone(ZoneId zone) {
	error("[EventHandlerPompeii::onInitZone] Not implemented (zone: %d)", zone);
}

#pragma endregion

#pragma region Timer

void EventHandlerPompeii::onTimer(TimerId timerId) {
	error("[EventHandlerPompeii::onTimer] Not implemented (id: %d)", timerId);
}

#pragma endregion

#pragma region Zone Setup

void EventHandlerPompeii::onSetup(ZoneId zone, SetupType type) {
	debugC(kRingDebugLogic, "onSetup (zone: %s, type: %d)", _app->getZoneFolder(zone).c_str(), type);

	switch (zone) {
	default:
		break;

	case kZone2:
		_eventSetup->onSetupZone2(type);
		break;

	case kZone3:
		_eventSetup->onSetupZone3(type);
		break;

	case kZone4:
		_eventSetup->onSetupZone4(type);
		break;
	}
}

#pragma endregion

} // End of namespace Ring
