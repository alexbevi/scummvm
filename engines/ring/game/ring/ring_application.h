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

#ifndef RING_RING_APPLICATION_H
#define RING_RING_APPLICATION_H

#include "ring/base/application.h"

namespace Ring {

class Accessibility;
class Movability;

class ZoneSystemRing;
class ZoneNIRing;
class ZoneRHRing;
class ZoneFORing;
class ZoneRORing;
class ZoneWARing;
class ZoneASRing;
class ZoneN2Ring;

class ApplicationRing : public Application {
public:
	ApplicationRing(RingEngine *engine);
	~ApplicationRing();

	//////////////////////////////////////////////////////////////////////////
	// Initialization
	virtual void initLanguages();
	virtual void initFont();
	virtual void setup();
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
	// Setup
	virtual void setupZone(ZoneId zone, SetupType type);
	void setZone(ZoneId zone, SetupType type);
	void setZoneAndEnableBag(ZoneId zone);

	//////////////////////////////////////////////////////////////////////////
	// Messages
	virtual void messageInsertCd(ZoneId zone);

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
	virtual void onSound(Id id, SoundType type, uint32 a3);
	virtual void onSetup(ZoneId zone, SetupType type);
	virtual void onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	virtual void onBagClickedObject(ObjectId id);
	virtual void onBagZoneSwitch();
	virtual void onUpdateBag(const Common::Point &point);
	virtual void onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	virtual void onUpdateAfter(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType, const Common::Point &point) {}
	virtual void onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType);
	virtual void onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType);
	virtual void onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	virtual void onAnimation(uint32 type, Id animationId, const Common::String &name, uint32 frame, uint32 a5);
	virtual void onVisualList(Id id, uint32 type, const Common::Point &point);

protected:
	void sub_433EE0();
	void sub_445A10();

	void onSwitchZone(ZoneId zone, uint32 type);
	void onSetupLoadTimers(Common::String zoneName, Id testId1, Id puzzleRotationId, Id testId2);

private:
	// Event handlers
	ZoneSystemRing     *_zoneSystem;
	ZoneNIRing         *_zoneNI;
	ZoneRHRing         *_zoneRH;
	ZoneFORing         *_zoneFO;
	ZoneRORing         *_zoneRO;
	ZoneWARing         *_zoneWA;
	ZoneASRing         *_zoneAS;
	ZoneN2Ring         *_zoneN2;

	bool         _controlNotPressed;
	int32        _presentationIndexNI;

	void onButtonDown(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUp2(ObjectId id, uint32 index, Id puzzleRotationId, uint32 a4, const Common::Point &point);

	void onMouseLeftButtonUp(const Common::Event &evt);
	bool handleLeftButtonDown(Accessibility *accessibility, uint32 index, Id id, const Common::Point &point);
	bool handleLeftButtonUp(Accessibility *accessibility, Id id, const Common::Point &point);
	bool handleLeftButtonUp(Movability *movability, uint32 index, Id id, bool isRotation = false);
	void onKeyDownZone(const Common::KeyState &keyState);

	uint32 getCdForZone(ZoneId zone) const;
	bool isDataPresent(SetupType type);

	friend class ZoneSystemRing;
	friend class ZoneNIRing;
	friend class ZoneRHRing;
	friend class ZoneFORing;
	friend class ZoneRORing;
	friend class ZoneWARing;
	friend class ZoneASRing;
	friend class ZoneN2Ring;
};

} // End of namespace Ring

#endif // RING_RING_APPLICATION_H
