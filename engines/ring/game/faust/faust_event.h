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
	virtual void onInitZone(ZoneId zone);

	virtual void onSound(Id id, SoundType type, uint32 a3);
	virtual void onSetup(ZoneId zone, SetupType type);
	virtual void onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	virtual void onBagClickedObject(ObjectId id);
	virtual void onUpdateBag(const Common::Point &point);
	virtual void onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	virtual void onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType);
	virtual void onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType);
	virtual void onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);\
	virtual void onVisualList(Id id, uint32 type, const Common::Point &point);

private:
	Application *_app;

	//////////////////////////////////////////////////////////////////////////
	// Button down
	//////////////////////////////////////////////////////////////////////////
	void onButtonDown(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonDownZoneSY(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonDownZone5(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);

	void onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZoneSY(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone2(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone3(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone4(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone5(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone6(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone7(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone8(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone9(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone10(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone11(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone12(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone13(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone14(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone15(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone16(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone17(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);

	//////////////////////////////////////////////////////////////////////////
	// Key down
	//////////////////////////////////////////////////////////////////////////
	void onKeyDownZone(const Common::KeyState &keyState);

	//////////////////////////////////////////////////////////////////////////
	// Update
	//////////////////////////////////////////////////////////////////////////
	void onUpdateBeforeZoneSY(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	void onUpdateBeforeZone3(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	void onUpdateBeforeZone4(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	void onUpdateBeforeZone6(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	void onUpdateBeforeZone8(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	void onUpdateBeforeZone12(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	void onUpdateBeforeZone14(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	void onUpdateBeforeZone15(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	void onUpdateBeforeZone16(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);

	void onUpdateBagZoneSY(const Common::Point &point);
	void onUpdateBagZone3(const Common::Point &point);
	void onUpdateBagZone4(const Common::Point &point);
	void onUpdateBagZone6(const Common::Point &point);
	void onUpdateBagZone12(const Common::Point &point);
	void onUpdateBagZone14(const Common::Point &point);
	void onUpdateBagZone15(const Common::Point &point);
	void onUpdateBagZone16(const Common::Point &point);

	//////////////////////////////////////////////////////////////////////////
	// Bag
	//////////////////////////////////////////////////////////////////////////
	void onBagZoneSY(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZone3(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZone4(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZone5(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZone8(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZone12(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZone13(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZone14(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);

	void onBagClickedObjectZone2(ObjectId id);
	void onBagClickedObjectZone3(ObjectId id);
	void onBagClickedObjectZone4(ObjectId id);
	void onBagClickedObjectZone8(ObjectId id);
	void onBagClickedObjectZone14(ObjectId id);
	void onBagClickedObjectZone15(ObjectId id);

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

	//////////////////////////////////////////////////////////////////////////
	// Rides
	//////////////////////////////////////////////////////////////////////////
	void onBeforeRideZone2(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onBeforeRideZone4(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onBeforeRideZone5(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onBeforeRideZone8(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onBeforeRideZone14(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onBeforeRideZone15(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);

	void onAfterRideZone2(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone3(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone4(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone6(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone8(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone10(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone12(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone13(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone14(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone15(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone16(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone17(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);

	//////////////////////////////////////////////////////////////////////////
	// Sound
	//////////////////////////////////////////////////////////////////////////
	void onSoundZoneSY(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZone3(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZone4(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZone5(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZone8(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZone12(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZone14(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZone15(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZone17(Id id, SoundType type, uint32 a3, bool process);

	//////////////////////////////////////////////////////////////////////////
	// Animation
	//////////////////////////////////////////////////////////////////////////
	void onAnimationNextFrameZoneSY(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone3(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone4(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone5(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone6(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone7(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone8(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone9(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone10(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone11(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone12(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone13(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone14(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone15(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);

	//////////////////////////////////////////////////////////////////////////
	// VisualList
	//////////////////////////////////////////////////////////////////////////
	void onVisualListZoneSY(Id id, uint32 a2, const Common::Point &point);

	//////////////////////////////////////////////////////////////////////////
	// Helpers
	//////////////////////////////////////////////////////////////////////////
	void sub_45FF30();
	void sub_468290(uint32 a1);
	void sub_4A2CF0();
	void setupRotation(uint32 type);
};

} // End of namespace Ring

#endif // RING_EVENTHANDLER_FAUST_H
