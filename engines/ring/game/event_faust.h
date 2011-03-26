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

#ifndef RING_EVENTHANDLER_FAUST_H
#define RING_EVENTHANDLER_FAUST_H

#include "ring/base/event.h"

namespace Ring {

class Application;

class EventHandlerFaust : public EventHandler {
public:
	EventHandlerFaust(Application *application);
	virtual ~EventHandlerFaust();

	virtual void onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed);
	virtual void onMouseLeftButtonDown(const Common::Event &evt);
	virtual void onMouseRightButtonUp(const Common::Event &evt);
	virtual void onKeyDown(Common::Event &evt);
	virtual void onTimer(TimerId id);

	//virtual void onSound(Id id, SoundType type, uint32 a3);
	virtual void onSetup(ZoneId zone, SetupType type);

private:
	Application *_app;

	//////////////////////////////////////////////////////////////////////////
	// Setup
	//////////////////////////////////////////////////////////////////////////
	void onSetupZone2(SetupType type);
	void onSetupZone3(SetupType type);
	void onSetupZone4(SetupType type);
	void onSetupZone5(SetupType type);
	void onSetupZone6(SetupType type);
	void onSetupZone7(SetupType type);
	void onSetupZone8(SetupType type);

	//////////////////////////////////////////////////////////////////////////
	// Timer
	//////////////////////////////////////////////////////////////////////////
	void onTimerZoneSY(TimerId id);
	void onTimerZone2(TimerId id);
	void onTimerZone3(TimerId id);
	void onTimerZone4(TimerId id);
	void onTimerZone5(TimerId id);
	void onTimerZone6(TimerId id);
	void onTimerZone8(TimerId id);
	void onTimerZone9(TimerId id);
	void onTimerZone10(TimerId id);
	void onTimerZone11(TimerId id);
	void onTimerZone12(TimerId id);
	void onTimerZone13(TimerId id);
	void onTimerZone14(TimerId id);
	void onTimerZone15(TimerId id);
	void onTimerZone16(TimerId id);
};

} // End of namespace Ring

#endif // RING_EVENTHANDLER_FAUST_H
