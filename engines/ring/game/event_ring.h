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

class Accessibility;
class ApplicationRing;
class Movability;

class EventHandlerRing : public EventHandler {
public:
	EventHandlerRing(ApplicationRing *application);
	virtual ~EventHandlerRing();

	virtual void onMouseLeftButtonUp(Common::Event &evt, bool isControlPressed);
	virtual void onMouseLeftButtonDown(Common::Event &evt);
	virtual void onMouseRightButtonUp(Common::Event &evt);
	virtual void onKeyDown(Common::Event &evt);
	virtual void onTimer(TimerId id);
	virtual void onSound(Id id, SoundType type, uint32 a3);
	virtual void onSetup(Zone zone, SetupType type);
	virtual void onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	virtual void onBagClickedObject(ObjectId id);
	virtual void onBagZoneSwitch();
	virtual void onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	virtual void onUpdateAfter(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType, const Common::Point &point) {}
	virtual void onUpdateBag(const Common::Point &point);
	virtual void onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType);
	virtual void onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType);
	virtual void onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	virtual void onAnimation(uint32 type, Id animationId, const Common::String &name, uint32 frame, uint32 a5);
	virtual void onVisualList(Id id, uint32 type, const Common::Point &point);

private:
	ApplicationRing *_app;

	// Data & State
	bool         _controlNotPressed;
	uint32       _dword_4A1C00;
	bool         _disableTimerRH;
	uint32       _frameNumberTimerRH;

	// RO
	uint32       _presentationIndexRO;
	float        _presentationIndexBagRO;
	uint32       _presentationIndexBagRO2;

	// SY
	uint32       _presentationIndexSY;
	bool         _prefsSubtitles;
	uint32       _prefsVolume;
	uint32       _prefsVolumeDialog;
	bool         _object1Visible;

	//////////////////////////////////////////////////////////////////////////
	// Event Handling
	//////////////////////////////////////////////////////////////////////////
	void onMouseLeftButtonUp(Common::Event &evt);
	bool handleLeftButtonDown(Accessibility *accessibility, uint32 index, Id id, const Common::Point &point);

	bool handleLeftButtonUp(Accessibility *accessibility, Id id, const Common::Point &point);
	bool handleLeftButtonUp(Movability *movability, uint32 index, Id id, bool isRotation = false);

	//////////////////////////////////////////////////////////////////////////
	// Left button down
	//////////////////////////////////////////////////////////////////////////
	void onButtonDown(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonDownZoneNI(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonDownZoneRO(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonDownZoneN2(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);

	void onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZoneSY(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZoneNI(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZoneRH(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZoneFO(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZoneRO(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZoneWA(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZoneAS(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZoneN2(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);

	void onButtonUp2(ObjectId id, uint32 index, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUp2ZoneWA(ObjectId id, uint32 index, Id puzzleRotationId, uint32 a4, const Common::Point &point);

	//////////////////////////////////////////////////////////////////////////
	// Key down
	//////////////////////////////////////////////////////////////////////////
	void onKeyDownZone(Common::KeyState keyState);
	void onKeyDownZoneSY(Common::KeyState keyState);

	//////////////////////////////////////////////////////////////////////////
	// Setup
	//////////////////////////////////////////////////////////////////////////
	void onSetupZoneNI(SetupType type);
	void onSetupZoneRH(SetupType type);
	void onSetupZoneFO(SetupType type);
	void onSetupZoneRO(SetupType type);
	void onSetupZoneWA(SetupType type);
	void onSetupZoneAS(SetupType type);
	void onSetupZoneN2(SetupType type);

	void onSetupLoadTimers(Common::String zoneName, Id testId1, Id puzzleRotationId, Id testId2);

	//////////////////////////////////////////////////////////////////////////
	// Update
	//////////////////////////////////////////////////////////////////////////
	void onUpdateBeforeZoneSY(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	void onUpdateBeforeZoneNI(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	void onUpdateBeforeZoneN2(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	void onUpdateBagZoneSY(const Common::Point &point);

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
	// Bag
	//////////////////////////////////////////////////////////////////////////
	void onBagZoneSY(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZoneNI(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZoneFO(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZoneRO(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZoneN2(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);

	void onBagClickedObjectZoneFO(ObjectId id);

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
	void onSoundZoneSY(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZoneNI(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZoneRH(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZoneFO(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZoneRO(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZoneWA(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZoneAS(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZoneN2(Id id, SoundType type, uint32 a3, bool process);

	//////////////////////////////////////////////////////////////////////////
	// Animation
	//////////////////////////////////////////////////////////////////////////
	void onAnimationNextFrameZoneNI(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZoneRH(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZoneFO(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZoneRO(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZoneWA(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZoneAS(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZoneN2(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);

	void onAnimationZoneWA(uint32 type, Id animationId, const Common::String &name, uint32 frame, uint32 a5);

	//////////////////////////////////////////////////////////////////////////
	// Zone switching
	//////////////////////////////////////////////////////////////////////////
	void onSwitchZoneNI(uint32 type);
	void onSwitchZoneRH(uint32 type);
	void onSwitchZoneFO(uint32 type);
	void onSwitchZoneRO(uint32 type);
	void onSwitchZoneWA(uint32 type);
	void onSwitchZoneAS(uint32 type);
	void onSwitchZoneN2(uint32 type);

	//////////////////////////////////////////////////////////////////////////
	// VisualList
	//////////////////////////////////////////////////////////////////////////
	void onVisualListZoneSY(Id id, uint32 a2, const Common::Point &point);

	//////////////////////////////////////////////////////////////////////////
	// Helper functions
	//////////////////////////////////////////////////////////////////////////
	void sub_433EE0();
	void sub_433FA0();
	void sub_4340C0();

	// NI
	void sub_445930();
	void sub_445A10();

	void waitTicks(uint32 ticks);
};

} // End of namespace Ring

#endif // RING_EVENTHANDLER_RING_H
