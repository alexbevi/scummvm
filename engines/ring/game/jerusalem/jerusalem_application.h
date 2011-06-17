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

#ifndef RING_JERUSALEM_APPLICATION_H
#define RING_JERUSALEM_APPLICATION_H

#include "ring/base/application.h"

namespace Ring {

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

class ApplicationJerusalem : public Application {
public:
	ApplicationJerusalem(RingEngine *engine);
	~ApplicationJerusalem();

	//////////////////////////////////////////////////////////////////////////
	// Initialization
	virtual void initFont();
	virtual void setup();
	virtual void initData();
	virtual void initZones();
	virtual void initBag();

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

#endif // RING_JERUSALEM_APPLICATION_H
