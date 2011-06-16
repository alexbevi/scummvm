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

#ifndef RING_PILGRIM3_EVENT_H
#define RING_PILGRIM3_EVENT_H

#include "ring/base/event.h"

namespace Ring {

class ApplicationPilgrim3;

class EventAnimationPilgrim3;
class EventBagPilgrim3;
class EventButtonPilgrim3;
class EventInitPilgrim3;
class EventInputPilgrim3;
class EventRidePilgrim3;
class EventSetupPilgrim3;
class EventSoundPilgrim3;
class EventTimerPilgrim3;
class EventVisualPilgrim3;
class EventZonePilgrim3;

class EventHandlerPilgrim3 : public EventHandler {
public:
	EventHandlerPilgrim3(ApplicationPilgrim3 *application);
	virtual ~EventHandlerPilgrim3();

	virtual void onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed);
	virtual void onMouseLeftButtonDown(const Common::Event &evt);
	virtual void onMouseRightButtonUp(const Common::Event &evt);
	virtual void onKeyDown(Common::Event &evt);
	virtual void onTimer(TimerId id);
	virtual void onInitZone(ZoneId zone);

private:
	ApplicationPilgrim3 *_app;

	// Event handlers
	EventAnimationPilgrim3 *_eventAnimation;
	EventBagPilgrim3       *_eventBag;
	EventButtonPilgrim3    *_eventButton;
	EventInitPilgrim3      *_eventInit;
	EventInputPilgrim3     *_eventInput;
	EventRidePilgrim3      *_eventRide;
	EventSetupPilgrim3     *_eventSetup;
	EventSoundPilgrim3     *_eventSound;
	EventTimerPilgrim3     *_eventTimer;
	EventVisualPilgrim3    *_eventVisual;
	EventZonePilgrim3      *_eventZone;
};

} // End of namespace Ring

#endif // RING_PILGRIM3_EVENT_H
