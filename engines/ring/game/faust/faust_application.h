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

#ifndef RING_FAUST_APPLICATION_H
#define RING_FAUST_APPLICATION_H

#include "ring/base/application.h"

namespace Ring {

class ZoneSystemFaust;
class Zone2Faust;
class Zone3Faust;
class Zone4Faust;
class Zone5Faust;
class Zone6Faust;
class Zone7Faust;
class Zone8Faust;
class Zone9Faust;
class Zone10Faust;
class Zone11Faust;
class Zone12Faust;
class Zone13Faust;
class Zone14Faust;
class Zone15Faust;
class Zone16Faust;
class Zone17Faust;

class ApplicationFaust : public Application {
public:
	ApplicationFaust(RingEngine *engine);
	~ApplicationFaust();

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
	// Messages
	virtual void messageInsertCd(ZoneId zone);

	//////////////////////////////////////////////////////////////////////////
	// Zone
	virtual void setupZone(ZoneId zone, SetupType type);

	//////////////////////////////////////////////////////////////////////////
	// Visual
	virtual Visual *createVisual(Id visualId, uint32 a3, uint32 a4, uint32 left, uint32 top, uint32 offsetY, uint32 height, uint32 progressMultiplier, uint32 progressColor);

	//////////////////////////////////////////////////////////////////////////
	// Event handler
	virtual void onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed);
	virtual void onMouseLeftButtonDown(const Common::Event &evt);
	virtual void onMouseRightButtonUp(const Common::Event &evt);
	virtual void onKeyDown(Common::Event &evt);
	virtual void onTimer(TimerId id);
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

protected:
	//////////////////////////////////////////////////////////////////////////
	// Buttons
	//////////////////////////////////////////////////////////////////////////
	void onButtonDown(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);

	//////////////////////////////////////////////////////////////////////////
	// Zone switching
	//////////////////////////////////////////////////////////////////////////
	void onSwitchZone(ZoneId zone, uint32 type);

	//////////////////////////////////////////////////////////////////////////
	// Helper functions
	//////////////////////////////////////////////////////////////////////////
	void setupRotation(uint32 type);
	void sub_45FF30();
	void sub_468290(uint32 a1);
	void sub_4A2CF0();

private:
	// Event handlers
	ZoneSystemFaust *_zoneSystem;
	Zone2Faust      *_zone2;
	Zone3Faust      *_zone3;
	Zone4Faust      *_zone4;
	Zone5Faust      *_zone5;
	Zone6Faust      *_zone6;
	Zone7Faust      *_zone7;
	Zone8Faust      *_zone8;
	Zone9Faust      *_zone9;
	Zone10Faust     *_zone10;
	Zone11Faust     *_zone11;
	Zone12Faust     *_zone12;
	Zone13Faust     *_zone13;
	Zone14Faust     *_zone14;
	Zone15Faust     *_zone15;
	Zone16Faust     *_zone16;
	Zone17Faust     *_zone17;

	int32 _slot;
	ZoneId _zone;

	//////////////////////////////////////////////////////////////////////////
	// Start menu
	//////////////////////////////////////////////////////////////////////////
	void setProgressAndShowMenu(ProgressState progress);
	void loadAndStartLogin();
	void loadAndInitZone();
	void initZone();
	void initMenuSave(bool savegame);
	void initMenu2();
	void initMenu3();

	friend class ZoneSystemFaust;
	friend class Zone2Faust;
	friend class Zone3Faust;
	friend class Zone4Faust;
	friend class Zone5Faust;
	friend class Zone6Faust;
	friend class Zone7Faust;
	friend class Zone8Faust;
	friend class Zone9Faust;
	friend class Zone10Faust;
	friend class Zone11Faust;
	friend class Zone12Faust;
	friend class Zone13Faust;
	friend class Zone14Faust;
	friend class Zone15Faust;
	friend class Zone16Faust;
	friend class Zone17Faust;
};

} // End of namespace Ring

#endif // RING_FAUST_APPLICATION_H
