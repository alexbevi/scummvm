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

#include "ring/game/jerusalem/jerusalem_application.h"

#include "ring/base/art.h"
#include "ring/base/bag.h"
#include "ring/base/preferences.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"

#include "ring/game/jerusalem/jerusalem_animation.h"
#include "ring/game/jerusalem/jerusalem_bag.h"
#include "ring/game/jerusalem/jerusalem_button.h"
#include "ring/game/jerusalem/jerusalem_init.h"
#include "ring/game/jerusalem/jerusalem_input.h"
#include "ring/game/jerusalem/jerusalem_ride.h"
#include "ring/game/jerusalem/jerusalem_setup.h"
#include "ring/game/jerusalem/jerusalem_shared.h"
#include "ring/game/jerusalem/jerusalem_sound.h"
#include "ring/game/jerusalem/jerusalem_timer.h"
#include "ring/game/jerusalem/jerusalem_visual.h"
#include "ring/game/jerusalem/jerusalem_zone.h"

#include "ring/graphics/screen.h"

#include "ring/sound/soundhandler.h"

#include "ring/debug.h"
#include "ring/helpers.h"
#include "ring/ring.h"

//using namespace JerusalemGame;

namespace Ring {

ApplicationJerusalem::ApplicationJerusalem(RingEngine *engine) : Application(engine) {
	_systemZone = kZone100;

	// Event handlers
	_eventAnimation = new EventAnimationJerusalem(this);
	_eventBag       = new EventBagJerusalem(this);
	_eventButton    = new EventButtonJerusalem(this);
	_eventInit      = new EventInitJerusalem(this);
	_eventInput     = new EventInputJerusalem(this);
	_eventRide      = new EventRideJerusalem(this);
	_eventSetup     = new EventSetupJerusalem(this);
	_eventSound     = new EventSoundJerusalem(this);
	_eventTimer     = new EventTimerJerusalem(this);
	_eventVisual    = new EventVisualJerusalem(this);
	_eventZone      = new EventZoneJerusalem(this);
}

ApplicationJerusalem::~ApplicationJerusalem() {
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

void ApplicationJerusalem::initFont() {
	// Original asks for size 12, but the font only contains size 8
	fontAdd(kFontDefault, "Jerusalm.fon", "Arxel1", 8, true, false, false, false, getCurrentLanguage());
}

void ApplicationJerusalem::setup() {
	// Add the list of episodes
	addEpisode(kZone2,  "One",    1);
	addEpisode(kZone3,  "Two",    2);
	addEpisode(kZone4,  "Three",  2);
	addEpisode(kZone5,  "Four",   2);
	addEpisode(kZone6,  "Five",   2);
	addEpisode(kZone12, "Eleven", 0);

	// Add the list of zones
	addZone(kZone1,  "A01", "a01", kArchiveArt, kLoadFromCd);
	addZone(kZone2,  "A02", "a02", kArchiveArt, kLoadFromCd);
	addZone(kZone3,  "A03", "a03", kArchiveArt, kLoadFromCd);
	addZone(kZone4,  "A04", "a04", kArchiveArt, kLoadFromCd);
	addZone(kZone5,  "A05", "a05", kArchiveArt, kLoadFromCd);
	addZone(kZone6,  "A06", "a06", kArchiveArt, kLoadFromCd);
	addZone(kZone7,  "A07", "a07", kArchiveArt, kLoadFromCd);
	addZone(kZone8,  "A08", "a08", kArchiveArt, kLoadFromCd);
	addZone(kZone9,  "A09", "a09", kArchiveArt, kLoadFromCd);
	addZone(kZone10, "A10", "a10", kArchiveArt, kLoadFromCd);

	// Setup system zone
	setupZone(kZoneSY, kSetupTypeNone);
	setSpace(kZone100);

	// Setup cursors
	ArchiveType archiveType = (_configuration.artCURSOR ? kArchiveArt : kArchiveFile);

	cursorAdd(kCursorDefault,                 "cur_no", kCursorTypeImage, 1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorBusy,                  "cur_busy", kCursorTypeImage, 1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorIdle,                  "cur_idle", kCursorTypeImage, 1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorMove,                   "cur_muv", kCursorTypeAnimated, 1, 3, 5.0f, 16, kLoadFromCursor, archiveType);
	cursorAdd(kCursorBack,                  "cur_back", kCursorTypeImage, 1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorMenuIdle,          "cur_menuIdle", kCursorTypeImage, 1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorMenuActive,      "cur_menuActive", kCursorTypeImage, 1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorZoom,                  "cur_zoom", kCursorTypeAnimated, 1, 2, 5.0f, 16, kLoadFromCursor, archiveType);
	cursorAdd(kCursorTake,                  "cur_take", kCursorTypeImage, 1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorAction,              "cur_action", kCursorTypeImage, 1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorActionObject,  "cur_actionObject", kCursorTypeImage, 1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorDragDrop,              "cur_drag", kCursorTypeImage, 1, kLoadFromCursor, archiveType);

	// Adjust offsets
	cursorSetOffset(kCursorBusy,         Common::Point(8, 8));
	cursorSetOffset(kCursorIdle,         Common::Point(24, 2));
	cursorSetOffset(kCursorMove,         Common::Point(24, 2));
	cursorSetOffset(kCursorBack,         Common::Point(24, 2));
	cursorSetOffset(kCursorMenuIdle,     Common::Point(15, 2));
	cursorSetOffset(kCursorMenuActive,   Common::Point(15, 2));
	cursorSetOffset(kCursorZoom,         Common::Point(17, 14));
	cursorSetOffset(kCursorTake,         Common::Point(24, 2));
	cursorSetOffset(kCursorAction,       Common::Point(24, 2));
	cursorSetOffset(kCursorActionObject, Common::Point(24, 2));
	cursorSetOffset(kCursorDragDrop,     Common::Point(24, 2));

	// Setup subtitles
	subtitleSetColor(Color(255, 255, 255));
	subtitleSetBackgroundColor(Color(50, 50, 50));
}

void ApplicationJerusalem::initData() {
	_field_74 = true;
	_field_75 = true;
	_field_76 = true;
	_field_77 = false;
	_field_78 = true;
}

void ApplicationJerusalem::initBag() {
	_bag->setOrigin(Common::Point(0, 0));
	_bag->sub_417D40(20, 35, 40, 60);
	_bag->setBackgroundOffset(Common::Point(0, 0));
	_bag->sub_417DD0(10);
	_bag->sub_417D80(0, 0, 30, 448);
	_bag->sub_417DA0(610, 0, 30, 448);
	_bag->sub_4192A0(6, 12);
	_bag->sub_4192C0(622, 12);
	_bag->sub_417DE0(0, 0);
	_bag->sub_419280(500);
	_bag->loadBackground("bagbgr.tga", "", "", "", "", "", "", "bag_h.tga", _archiveType);
	_bag->initHotspots();
}

#pragma endregion

#pragma region Startup

void ApplicationJerusalem::showStartupScreen() {
	error("[ApplicationJerusalem::showStartupScreen] Not implemented");
}

void ApplicationJerusalem::startMenu(bool savegame) {
	error("[ApplicationJerusalem::startMenu] Not implemented");
}

void ApplicationJerusalem::showMenu(ZoneId zone, MenuAction menuAction) {
	error("[ApplicationJerusalem::showMenu] Not implemented");
}

void ApplicationJerusalem::showCredits() {
	error("[ApplicationJerusalem::showCredits] Not implemented");
}

void ApplicationJerusalem::loadPreferences() {
	error("[ApplicationJerusalem::loadPreferences] Not implemented");
}

#pragma endregion

#pragma region Drawing

void ApplicationJerusalem::draw() {
	// Update our screen
	_screenManager->updateScreen();

	// Update engine state
	error("[ApplicationJerusalem::draw] Engine state update not implemented!");
}

#pragma endregion

#pragma region Visual

Visual *ApplicationJerusalem::createVisual(Id visualId, uint32 a3, uint32 a4, uint32 left, uint32 top, uint32 offsetY, uint32 height, uint32 progressMultiplier, uint32 progressColor) {
	error("[ApplicationJerusalem::createVisual] Engine state update not implemented!");
}

#pragma endregion

#pragma region Zone initialization

void ApplicationJerusalem::initZones() {
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

	_archiveType = getZoneArchiveType(getCurrentZone());

	setState(kStateNone);

	// Clear screen
	_screenManager->clear();
	g_system->updateScreen();

	// Original loads preferences and stores the showSubtitles
	// value to the main app structure
}

#pragma endregion

#pragma region Event handling

void ApplicationJerusalem::onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed) {
	error("[ApplicationJerusalem::onMouseLeftButtonUp] Not implemented (evt: %d, CTRL pressed: %d)", evt.type, isControlPressed);
}

void ApplicationJerusalem::onMouseLeftButtonDown(const Common::Event &evt) {
	error("[ApplicationJerusalem::onMouseLeftButtonDown] Not implemented (evt: %d)", evt.type);
}

void ApplicationJerusalem::onMouseRightButtonUp(const Common::Event &evt) {
	error("[ApplicationJerusalem::onMouseRightButtonUp] Not implemented (evt: %d)", evt.type);
}

void ApplicationJerusalem::onKeyDown(Common::Event &evt) {
	error("[ApplicationJerusalem::onKeyDown] Not implemented (evt: %d)", evt.type);
}

void ApplicationJerusalem::onInitZone(ZoneId zone) {
	error("[ApplicationJerusalem::onInitZone] Not implemented (zone: %d)", zone);
}

void ApplicationJerusalem::onTimer(TimerId timerId) {
	error("[ApplicationJerusalem::onTimer] Not implemented (id: %d)", timerId);
}

#pragma endregion

} // End of namespace Ring
