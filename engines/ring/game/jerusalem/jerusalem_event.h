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

#ifndef RING_JERUSALEM_EVENT_H
#define RING_JERUSALEM_EVENT_H

#include "ring/base/event.h"

namespace Ring {

class ApplicationJerusalem;

class EventAnimationJerusalem;
class EventBagJerusalem;
class EventButtonJerusalem;
class EventInitJerusalem;
class EventInputJerusalem;
class EventRideJerusalem;
class EventSetupJerusalem;
class EventSoundJerusalem;
class EventTimerJerusalem;
class EventVisualJerusalem;
class EventZoneJerusalem;

class EventHandlerJerusalem : public EventHandler {
public:
	EventHandlerJerusalem(ApplicationJerusalem *application);
	virtual ~EventHandlerJerusalem();

	virtual void onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed);
	virtual void onMouseLeftButtonDown(const Common::Event &evt);
	virtual void onMouseRightButtonUp(const Common::Event &evt);
	virtual void onKeyDown(Common::Event &evt);
	virtual void onTimer(TimerId id);
	virtual void onInitZone(ZoneId zone);

private:
	ApplicationJerusalem *_app;

	// Event handlers
	EventAnimationJerusalem *_eventAnimation;
	EventBagJerusalem       *_eventBag;
	EventButtonJerusalem    *_eventButton;
	EventInitJerusalem      *_eventInit;
	EventInputJerusalem     *_eventInput;
	EventRideJerusalem      *_eventRide;
	EventSetupJerusalem     *_eventSetup;
	EventSoundJerusalem     *_eventSound;
	EventTimerJerusalem     *_eventTimer;
	EventVisualJerusalem    *_eventVisual;
	EventZoneJerusalem      *_eventZone;
};

} // End of namespace Ring

#endif // RING_JERUSALEM_EVENT_H
