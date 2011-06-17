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

#include "ring/game/pompeii/pompeii_application.h"

#include "ring/base/art.h"
#include "ring/base/bag.h"
#include "ring/base/preferences.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"

#include "ring/game/pompeii/pompeii_animation.h"
#include "ring/game/pompeii/pompeii_bag.h"
#include "ring/game/pompeii/pompeii_button.h"
#include "ring/game/pompeii/pompeii_init.h"
#include "ring/game/pompeii/pompeii_input.h"
#include "ring/game/pompeii/pompeii_ride.h"
#include "ring/game/pompeii/pompeii_setup.h"
#include "ring/game/pompeii/pompeii_shared.h"
#include "ring/game/pompeii/pompeii_sound.h"
#include "ring/game/pompeii/pompeii_timer.h"
#include "ring/game/pompeii/pompeii_visual.h"
#include "ring/game/pompeii/pompeii_zone.h"

#include "ring/graphics/screen.h"

#include "ring/sound/soundhandler.h"

#include "ring/debug.h"
#include "ring/helpers.h"
#include "ring/ring.h"

//using namespace PompeiiGame;

namespace Ring {

ApplicationPompeii::ApplicationPompeii(RingEngine *engine) : Application(engine) {
	_systemZone = kZone100;

	// Event handlers
	_eventAnimation = new EventAnimationPompeii(this);
	_eventBag       = new EventBagPompeii(this);
	_eventButton    = new EventButtonPompeii(this);
	_eventInit      = new EventInitPompeii(this);
	_eventInput     = new EventInputPompeii(this);
	_eventRide      = new EventRidePompeii(this);
	_eventSetup     = new EventSetupPompeii(this);
	_eventSound     = new EventSoundPompeii(this);
	_eventTimer     = new EventTimerPompeii(this);
	_eventVisual    = new EventVisualPompeii(this);
	_eventZone      = new EventZonePompeii(this);
}

ApplicationPompeii::~ApplicationPompeii() {
	SAFE_DELETE(_eventAnimation);
	SAFE_DELETE(_eventBag);
	SAFE_DELETE(_eventButton);
	SAFE_DELETE(_eventInit);
	SAFE_DELETE(_eventInput);
	SAFE_DELETE(_eventRide);
	SAFE_DELETE(_eventSetup);
	SAFE_DELETE(_eventSound);
	SAFE_DELETE(_eventTimer);
	SAFE_DELETE(_eventVisual);
	SAFE_DELETE(_eventZone);
}

#pragma region Game setup

void ApplicationPompeii::initFont() {
	// Original asks for size 12, but the font only contains size 8
	fontAdd(kFontDefault, "pompei.fon", "Arxel1", 8, true, false, false, false, getCurrentLanguage());
}

void ApplicationPompeii::setup() {
	// Add the list of episodes
	addEpisode(kZone2, "One",   2);
	addEpisode(kZone3, "Two",   1);
	addEpisode(kZone4, "Three", 2);
	addEpisode(kZone5, "Four",  1);
	addEpisode(kZone6, "Five",  1);

	// Add the list of zones
	addZone(kZone1,  "S00", "S00", kArchiveArt, kLoadFromCd);
	addZone(kZone2,  "S01", "S01", kArchiveArt, kLoadFromCd);
	addZone(kZone3,  "S02", "S02", kArchiveArt, kLoadFromCd);
	addZone(kZone4,  "S03", "S03", kArchiveArt, kLoadFromCd);
	addZone(kZone5,  "S04", "S04", kArchiveArt, kLoadFromCd);
	addZone(kZone6,  "S05", "S05", kArchiveArt, kLoadFromCd);
	addZone(kZone7,  "S06", "S06", kArchiveArt, kLoadFromCd);
	addZone(kZone8,  "S07", "S07", kArchiveArt, kLoadFromCd);
	addZone(kZone9,  "S08", "S08", kArchiveArt, kLoadFromCd);
	addZone(kZone10, "S09", "S09", kArchiveArt, kLoadFromCd);
	addZone(kZone11, "S10", "S10", kArchiveArt, kLoadFromCd);
	addZone(kZone12, "S11", "S11", kArchiveArt, kLoadFromCd);

	// Setup system zone
	setupZone(kZoneSY, kSetupTypeNone);
	setSpace(kZone100);

	// Setup cursors
	ArchiveType archiveType = (_configuration.artCURSOR ? kArchiveArt : kArchiveFile);

	cursorAdd(kCursorDefault,               "", kCursorTypeNormal, 1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorBusy,          "cur_busy", kCursorTypeImage,  1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorIdle,          "cur_idle", kCursorTypeImage,  1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorMove,           "cur_muv", kCursorTypeImage,  1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorBack,          "cur_back", kCursorTypeImage,  1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorMenuIdle,      "cur_me-1", kCursorTypeImage,  1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorMenuActive,    "cur_me-2", kCursorTypeImage,  1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorZoom,          "cur_zoom", kCursorTypeImage,  1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorTake,          "cur_take", kCursorTypeImage,  1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorAction,        "cur_ac-1", kCursorTypeImage,  1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorActionObject,  "cur_ac-2", kCursorTypeImage,  1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorDragDrop,         "adrag", kCursorTypeImage,  1, kLoadFromCursor, archiveType);

	cursorSetOffset(kCursorBusy,         Common::Point(8, 8));
	cursorSetOffset(kCursorIdle,         Common::Point(17, 1));
	cursorSetOffset(kCursorMove,         Common::Point(17, 1));
	cursorSetOffset(kCursorBack,         Common::Point(17, 1));
	cursorSetOffset(kCursorMenuIdle,     Common::Point(8, 8));
	cursorSetOffset(kCursorMenuActive,   Common::Point(8, 8));
	cursorSetOffset(kCursorZoom,         Common::Point(17, 1));
	cursorSetOffset(kCursorTake,         Common::Point(17, 1));
	cursorSetOffset(kCursorAction,       Common::Point(17, 1));
	cursorSetOffset(kCursorActionObject, Common::Point(17, 1));
	cursorSetOffset(kCursorDragDrop,     Common::Point(8, 8));

	// Setup subtitles
	subtitleSetColor(Color(255, 255, 255));
	subtitleSetBackgroundColor(Color(50, 50, 50));
}

void ApplicationPompeii::initBag() {
	_bag->setOrigin(Common::Point(0, 0));
	_bag->sub_417D40(15, 26, 40, 61);
	_bag->setBackgroundOffset(Common::Point(0, 0));
	_bag->sub_417DD0(10);
	_bag->sub_417D80(0, 0, 30, 448);
	_bag->sub_417DA0(610, 0, 30, 448);
	_bag->sub_4192A0(6, 12);
	_bag->sub_4192C0(622, 12);
	_bag->sub_417DE0(291, 4);
	_bag->sub_419280(500);
	_bag->loadBackground("bagbgr.tga", "", "", "", "", "", "", "bag_h.bmp", _archiveType);
	_bag->initHotspots();
}

#pragma endregion

#pragma region Startup

void ApplicationPompeii::showStartupScreen() {
	error("[ApplicationPompeii::showStartupScreen] Not implemented");
}

void ApplicationPompeii::startMenu(bool savegame) {
	error("[ApplicationPompeii::startMenu] Not implemented");
}

void ApplicationPompeii::showMenu(ZoneId zone, MenuAction menuAction) {
	error("[ApplicationPompeii::showMenu] Not implemented");
}

void ApplicationPompeii::showCredits() {
	error("[ApplicationPompeii::showCredits] Not implemented");
}

void ApplicationPompeii::loadPreferences() {
	error("[ApplicationPompeii::loadPreferences] Not implemented");
}

#pragma endregion

#pragma region Drawing

void ApplicationPompeii::draw() {
	// Update our screen
	_screenManager->updateScreen();

	// Update engine state
	error("[ApplicationPompeii::draw] Engine state update not implemented!");
}

#pragma endregion

#pragma region Visual

Visual *ApplicationPompeii::createVisual(Id visualId, uint32 a3, uint32 a4, uint32 left, uint32 top, uint32 offsetY, uint32 height, uint32 progressMultiplier, uint32 progressColor) {
	error("[ApplicationPompeii::createVisual] Engine state update not implemented!");
}

#pragma endregion

#pragma region Zone initialization

void ApplicationPompeii::initZones() {
	debugC(kRingDebugLogic, "Init zone data");

	_loadFrom = kLoadFromDisk;

	drawZoneName(kZone100);
	_archiveType = getZoneArchiveType(kZone100);
	onInitZone(kZone100);

	_loadFrom = kLoadFromCd;

	drawZoneName(kZone1);
	_archiveType = getZoneArchiveType(kZone2);
	onInitZone(kZone1);

	drawZoneName(kZone2);
	_archiveType = getZoneArchiveType(kZone2);
	onInitZone(kZone2);

	drawZoneName(kZone2);
	_archiveType = getZoneArchiveType(kZone3);
	onInitZone(kZone3);

	drawZoneName(kZone4);
	_archiveType = getZoneArchiveType(kZone4);
	onInitZone(kZone4);

	drawZoneName(kZone5);
	_archiveType = getZoneArchiveType(kZone5);
	onInitZone(kZone5);

	drawZoneName(kZone6);
	_archiveType = getZoneArchiveType(kZone6);
	onInitZone(kZone6);

	drawZoneName(kZone7);
	_archiveType = getZoneArchiveType(kZone7);
	onInitZone(kZone7);

	drawZoneName(kZone8);
	_archiveType = getZoneArchiveType(kZone8);
	onInitZone(kZone8);

	drawZoneName(kZone9);
	_archiveType = getZoneArchiveType(kZone9);
	onInitZone(kZone9);

	drawZoneName(kZone10);
	_archiveType = getZoneArchiveType(kZone10);
	onInitZone(kZone10);

	_loadFrom = kLoadFromDisk;

	drawZoneName(kZone11);
	_archiveType = getZoneArchiveType(kZone11);
	onInitZone(kZone11);

	drawZoneName(kZone12);
	_archiveType = getZoneArchiveType(kZone12);
	onInitZone(kZone12);

	_archiveType = getZoneArchiveType(getCurrentZone());

	setState(kStateNone);

	// Clear screen
	_screenManager->clear();
	g_system->updateScreen();
}

#pragma endregion

#pragma region Event handling

void ApplicationPompeii::onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed) {
	error("[ApplicationPompeii::onMouseLeftButtonUp] Not implemented (evt: %d, CTRL pressed: %d)", evt.type, isControlPressed);
}

void ApplicationPompeii::onMouseLeftButtonDown(const Common::Event &evt) {
	error("[ApplicationPompeii::onMouseLeftButtonDown] Not implemented (evt: %d)", evt.type);
}

void ApplicationPompeii::onMouseRightButtonUp(const Common::Event &evt) {
	error("[ApplicationPompeii::onMouseRightButtonUp] Not implemented (evt: %d)", evt.type);
}

void ApplicationPompeii::onKeyDown(Common::Event &evt) {
	error("[ApplicationPompeii::onKeyDown] Not implemented (evt: %d)", evt.type);
}

void ApplicationPompeii::onInitZone(ZoneId zone) {
	error("[ApplicationPompeii::onInitZone] Not implemented (zone: %d)", zone);
}

void ApplicationPompeii::onTimer(TimerId timerId) {
	error("[ApplicationPompeii::onTimer] Not implemented (id: %d)", timerId);
}

void ApplicationPompeii::onSetup(ZoneId zone, SetupType type) {
	debugC(kRingDebugLogic, "onSetup (zone: %s, type: %d)", getZoneFolder(zone).c_str(), type);

	switch (zone) {
	default:
		break;

	case kZone2:
		_eventSetup->onSetupZone2(type);
		break;

	case kZone3:
		_eventSetup->onSetupZone3(type);
		break;

	case kZone4:
		_eventSetup->onSetupZone4(type);
		break;
	}
}

#pragma endregion

} // End of namespace Ring
