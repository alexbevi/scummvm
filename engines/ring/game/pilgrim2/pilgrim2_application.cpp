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

#include "ring/game/pilgrim2/pilgrim2_application.h"

#include "ring/base/art.h"
#include "ring/base/bag.h"
#include "ring/base/preferences.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"

#include "ring/game/pilgrim2/pilgrim2_shared.h"
#include "ring/game/pilgrim2/pilgrim2_zonesystem.h"
#include "ring/game/pilgrim2/pilgrim2_zone1.h"
#include "ring/game/pilgrim2/pilgrim2_zone2.h"
#include "ring/game/pilgrim2/pilgrim2_zone3.h"
#include "ring/game/pilgrim2/pilgrim2_zone4.h"
#include "ring/game/pilgrim2/pilgrim2_zone5.h"

#include "ring/graphics/screen.h"

#include "ring/sound/soundhandler.h"

#include "ring/debug.h"
#include "ring/helpers.h"
#include "ring/ring.h"

//using namespace Pilgrim2Game;

namespace Ring {

ApplicationPilgrim2::ApplicationPilgrim2(RingEngine *engine) : Application(engine) {
	_systemZone = kZone100;

	// Event handlers
	_zoneSystem = new ZoneSystemPilgrim2(this);
	_zone1      = new Zone1Pilgrim2(this);
	_zone2      = new Zone2Pilgrim2(this);
	_zone3      = new Zone3Pilgrim2(this);
	_zone4      = new Zone4Pilgrim2(this);
	_zone5      = new Zone5Pilgrim2(this);
}

ApplicationPilgrim2::~ApplicationPilgrim2() {
	SAFE_DELETE(_zoneSystem);
	SAFE_DELETE(_zone1);
	SAFE_DELETE(_zone2);
	SAFE_DELETE(_zone3);
	SAFE_DELETE(_zone4);
	SAFE_DELETE(_zone5);
}

#pragma region Game setup

void ApplicationPilgrim2::initFont() {
	// Original asks for size 12, but the font only contains size 8
	fontAdd(kFontDefault, "legend.fon", "Arxel1", 8, true, false, false, false, getCurrentLanguage());
}

void ApplicationPilgrim2::setup() {
	// Add the list of episodes
	addEpisode(kZone2, "One",   1);
	addEpisode(kZone3, "Two",   2);
	addEpisode(kZone4, "Three", 2);
	addEpisode(kZone5, "Four",  2);
	addEpisode(kZone6, "Five",  1);

	// Add the list of zones
	addZone(kZone1, "A01", "a01", kArchiveArt, kLoadFromCd);
	addZone(kZone2, "A02", "a02", kArchiveArt, kLoadFromCd);
	addZone(kZone3, "A03", "a03", kArchiveArt, kLoadFromCd);
	addZone(kZone4, "A04", "a04", kArchiveArt, kLoadFromCd);
	addZone(kZone5, "A05", "a05", kArchiveArt, kLoadFromCd);

	// Setup system zone
	setupZone(kZoneSY, kSetupTypeNone);
	setSpace(kZone100);

	// Setup cursors
	ArchiveType archiveType = (_configuration.artCURSOR ? kArchiveArt : kArchiveFile);

	cursorAdd(kCursorDefault,                 "cur_no", kCursorTypeImage, 1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorBusy,                  "cur_busy", kCursorTypeImage, 1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorIdle,                  "cur_idle", kCursorTypeImage, 1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorMove,                   "cur_muv", kCursorTypeImage, 1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorBack,                  "cur_back", kCursorTypeImage, 1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorMenuIdle,          "cur_menuIdle", kCursorTypeImage, 1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorMenuActive,      "cur_menuActive", kCursorTypeImage, 1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorZoom,                  "cur_zoom", kCursorTypeImage, 1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorTake,                  "cur_take", kCursorTypeImage, 1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorAction,              "cur_action", kCursorTypeImage, 1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorActionObject,  "cur_actionObject", kCursorTypeImage, 1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorDragDrop,              "cur_drag", kCursorTypeImage, 1, kLoadFromCursor, archiveType);

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

void ApplicationPilgrim2::initData() {
	_field_74 = true;
	_field_75 = true;
	_field_76 = true;
	_field_77 = false;
	_field_78 = true;
}

void ApplicationPilgrim2::initBag() {
	_bag->setOrigin(Common::Point(0, 0));
	_bag->sub_417D40(15, 26, 40, 61);
	_bag->setBackgroundOffset(Common::Point(0, 0));
	_bag->sub_417DD0(10);
	_bag->sub_417D80(0, 0, 30, 448);
	_bag->sub_417DA0(610, 0, 30, 448);
	_bag->sub_4192A0(6, 12);
	_bag->sub_4192C0(622, 12);
	_bag->sub_417DE0(506, 0);
	_bag->sub_419280(500);
	_bag->loadBackground("bagbgr.tga", "", "", "", "", "", "", "bag_h.bmp", _archiveType);
	_bag->initHotspots();
}

#pragma endregion

#pragma region Startup

void ApplicationPilgrim2::showStartupScreen() {
	error("[ApplicationPilgrim2::showStartupScreen] Not implemented");
}

void ApplicationPilgrim2::startMenu(bool savegame) {
	error("[ApplicationPilgrim2::startMenu] Not implemented");
}

void ApplicationPilgrim2::showMenu(ZoneId zone, MenuAction menuAction) {
	error("[ApplicationPilgrim2::showMenu] Not implemented");
}

void ApplicationPilgrim2::showCredits() {
	error("[ApplicationPilgrim2::showCredits] Not implemented");
}

void ApplicationPilgrim2::loadPreferences() {
	error("[ApplicationPilgrim2::loadPreferences] Not implemented");
}

#pragma endregion

#pragma region Drawing

void ApplicationPilgrim2::draw() {
	// Update our screen
	_screenManager->updateScreen();

	// Update engine state
	error("[ApplicationPilgrim2::draw] Engine state update not implemented!");
}

#pragma endregion

#pragma region Visual

Visual *ApplicationPilgrim2::createVisual(Id visualId, uint32 a3, uint32 a4, uint32 left, uint32 top, uint32 offsetY, uint32 height, uint32 progressMultiplier, uint32 progressColor) {
	error("[ApplicationPilgrim2::createVisual] Engine state update not implemented!");
}

#pragma endregion

#pragma region Zone initialization

void ApplicationPilgrim2::initZones() {
	debugC(kRingDebugLogic, "Init zone data");

	_loadFrom = kLoadFromDisk;

	drawZoneName(kZone100);
	_archiveType = getZoneArchiveType(kZone100);
	_zoneSystem->onInit();

	_loadFrom = kLoadFromCd;

	drawZoneName(kZone1);
	_archiveType = getZoneArchiveType(kZone1);
	_zone1->onInit();

	drawZoneName(kZone2);
	_archiveType = getZoneArchiveType(kZone2);
	_zone2->onInit();

	drawZoneName(kZone3);
	_archiveType = getZoneArchiveType(kZone3);
	_zone3->onInit();

	drawZoneName(kZone4);
	_archiveType = getZoneArchiveType(kZone4);
	_zone4->onInit();

	drawZoneName(kZone5);
	_archiveType = getZoneArchiveType(kZone5);
	_zone5->onInit();

	_archiveType = getZoneArchiveType(getCurrentZone());

	setState(kStateNone);

	// Clear screen
	_screenManager->clear();
	g_system->updateScreen();
}

#pragma endregion

#pragma region Event handling

void ApplicationPilgrim2::onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed) {
	error("[ApplicationPilgrim2::onMouseLeftButtonUp] Not implemented (evt: %d, CTRL pressed: %d)", evt.type, isControlPressed);
}

void ApplicationPilgrim2::onMouseLeftButtonDown(const Common::Event &evt) {
	error("[ApplicationPilgrim2::onMouseLeftButtonDown] Not implemented (evt: %d)", evt.type);
}

void ApplicationPilgrim2::onMouseRightButtonUp(const Common::Event &evt) {
	error("[ApplicationPilgrim2::onMouseRightButtonUp] Not implemented (evt: %d)", evt.type);
}

void ApplicationPilgrim2::onKeyDown(Common::Event &evt) {
	error("[ApplicationPilgrim2::onKeyDown] Not implemented (evt: %d)", evt.type);
}

void ApplicationPilgrim2::onTimer(TimerId timerId) {
	error("[ApplicationPilgrim2::onTimer] Not implemented (id: %d)", timerId);
}

#pragma endregion

} // End of namespace Ring
