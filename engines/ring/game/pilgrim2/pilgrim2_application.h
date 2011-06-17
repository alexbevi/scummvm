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

#ifndef RING_PILGRIM2_APPLICATION_H
#define RING_PILGRIM2_APPLICATION_H

#include "ring/base/application.h"

namespace Ring {

class EventAnimationPilgrim2;
class EventBagPilgrim2;
class EventButtonPilgrim2;
class EventInitPilgrim2;
class EventInputPilgrim2;
class EventRidePilgrim2;
class EventSetupPilgrim2;
class EventSoundPilgrim2;
class EventTimerPilgrim2;
class EventVisualPilgrim2;
class EventZonePilgrim2;

class ApplicationPilgrim2 : public Application {
public:
	ApplicationPilgrim2(RingEngine *engine);
	~ApplicationPilgrim2();

	//////////////////////////////////////////////////////////////////////////
	// Initialization
	virtual void initFont();
	virtual void setup();
	virtual void initZones();
	virtual void initBag();
	virtual void initData();

	//////////////////////////////////////////////////////////////////////////
	// Startup & Menu
	virtual void showStartupScreen();
	virtual void startMenu(bool savegame);
	virtual void showMenu(ZoneId zone, MenuAction menuAction);
	virtual void showCredits();
	virtual void loadPreferences();

	//////////////////////////////////////////////////////////////////////////
	// Drawing
	virtual void draw();

	//////////////////////////////////////////////////////////////////////////
	// Visual
	virtual Visual *createVisual(Id visualId, uint32 a3, uint32 a4, uint32 left, uint32 top, uint32 offsetY, uint32 height, uint32 progressMultiplier, uint32 progressColor);

	//////////////////////////////////////////////////////////////////////////
	// Event handlers
	virtual void onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed);
	virtual void onMouseLeftButtonDown(const Common::Event &evt);
	virtual void onMouseRightButtonUp(const Common::Event &evt);
	virtual void onKeyDown(Common::Event &evt);
	virtual void onTimer(TimerId id);
	virtual void onInitZone(ZoneId zone);

private:
	// Event handlers
	EventAnimationPilgrim2 *_eventAnimation;
	EventBagPilgrim2       *_eventBag;
	EventButtonPilgrim2    *_eventButton;
	EventInitPilgrim2      *_eventInit;
	EventInputPilgrim2     *_eventInput;
	EventRidePilgrim2      *_eventRide;
	EventSetupPilgrim2     *_eventSetup;
	EventSoundPilgrim2     *_eventSound;
	EventTimerPilgrim2     *_eventTimer;
	EventVisualPilgrim2    *_eventVisual;
	EventZonePilgrim2      *_eventZone;
};

} // End of namespace Ring

#endif // RING_PILGRIM2_APPLICATION_H
