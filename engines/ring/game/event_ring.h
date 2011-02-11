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

#include "ring/base/event.h"

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
	virtual void onSound(Id id, SoundType type, uint32 a3);
	virtual void onSetup(Zone zone, uint32 a2);
	virtual void onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType);
	virtual void onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType);

private:
	Application *_app;

	bool         _controlNotPressed;

	// Data
	uint32 _dword_4A1C00;

	//////////////////////////////////////////////////////////////////////////
	// Event Handling
	//////////////////////////////////////////////////////////////////////////
	void onMouseLeftButtonUp(Common::Event &evt);


	void onKeyDownZoneSY(Common::KeyCode keycode);

	//////////////////////////////////////////////////////////////////////////
	// Setup
	//////////////////////////////////////////////////////////////////////////
	void onSetupZoneNI(uint32 a1);
	void onSetupZoneRH(uint32 a1);
	void onSetupZoneFO(uint32 a1);
	void onSetupZoneRO(uint32 a1);
	void onSetupZoneWA(uint32 a1);
	void onSetupZoneAS(uint32 a1);
	void onSetupZoneN2(uint32 a1);

	void onSetupLoadTimers(Common::String zoneName, Id testId1, Id puzzleRotationId, Id testId2);

	//////////////////////////////////////////////////////////////////////////
	// Timer
	//////////////////////////////////////////////////////////////////////////
	void onTimerZoneNI(TimerId id);
	void onTimerZoneRH(TimerId id);
	void onTimerZoneFO(TimerId id);
	void onTimerZoneRO(TimerId id);
	void onTimerZoneAS(TimerId id);
	void onTimerZoneN2(TimerId id);

	//////////////////////////////////////////////////////////////////////////
	// Rides
	//////////////////////////////////////////////////////////////////////////
	void onBeforeRideZoneNI(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType);
	void onBeforeRideZoneRH(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType);
	void onBeforeRideZoneFO(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType);
	void onBeforeRideZoneRO(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType);
	void onBeforeRideZoneWA(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType);
	void onBeforeRideZoneAS(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType);
	void onBeforeRideZoneN2(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType);

	void onAfterRideZoneNI(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType);
	void onAfterRideZoneRH(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType);
	void onAfterRideZoneFO(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType);
	void onAfterRideZoneRO(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType);
	void onAfterRideZoneWA(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType);
	void onAfterRideZoneAS(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType);
	void onAfterRideZoneN2(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType);

	//////////////////////////////////////////////////////////////////////////
	// Sound
	//////////////////////////////////////////////////////////////////////////
	void onSoundZoneSY(Id id, SoundType type, uint32 a3, uint32 a4);
	void onSoundZoneNI(Id id, SoundType type, uint32 a3, uint32 a4);
	void onSoundZoneRH(Id id, SoundType type, uint32 a3, uint32 a4);
	void onSoundZoneFO(Id id, SoundType type, uint32 a3, uint32 a4);
	void onSoundZoneRO(Id id, SoundType type, uint32 a3, uint32 a4);
	void onSoundZoneWA(Id id, SoundType type, uint32 a3, uint32 a4);
	void onSoundZoneAS(Id id, SoundType type, uint32 a3, uint32 a4);
	void onSoundZoneN2(Id id, SoundType type, uint32 a3, uint32 a4);
};

} // End of namespace Ring

#endif // RING_EVENTHANDLER_RING_H
