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

#ifndef RING_EVENTHANDLER_RING_H
#define RING_EVENTHANDLER_RING_H

#include "ring/game/event.h"

namespace Ring {

class Application;

class EventHandlerRing : public EventHandler {
public:
	EventHandlerRing(Application *application);
	virtual ~EventHandlerRing();

	virtual void onMouseLeftButtonUp(Common::Event &evt, bool isControlPressed);
	virtual void onMouseLeftButtonDown(Common::Event &evt);
	virtual void onMouseRightButtonUp(Common::Event &evt);
	virtual void onKeyDown(Common::Event &evt);
	virtual void onTimer(TimerId id);

private:
	Application *_app;

	bool         _controlNotPressed;

	//////////////////////////////////////////////////////////////////////////
	// Event Handling
	//////////////////////////////////////////////////////////////////////////
	void onMouseLeftButtonUp(Common::Event &evt);

	//////////////////////////////////////////////////////////////////////////
	// Timer
	//////////////////////////////////////////////////////////////////////////
	void onZoneTimerNI(TimerId id);
	void onZoneTimerRH(TimerId id);
	void onZoneTimerFO(TimerId id);
	void onZoneTimerRO(TimerId id);
	void onZoneTimerAS(TimerId id);
	void onZoneTimerN2(TimerId id);
};

} // End of namespace Ring

#endif // RING_EVENTHANDLER_RING_H
