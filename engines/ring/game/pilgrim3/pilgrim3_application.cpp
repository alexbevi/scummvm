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

#include "ring/game/pilgrim3/pilgrim3_application.h"

#include "ring/base/art.h"
#include "ring/base/bag.h"
#include "ring/base/preferences.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"

#include "ring/game/pilgrim3/pilgrim3_event.h"

#include "ring/graphics/screen.h"

#include "ring/sound/soundhandler.h"

#include "ring/debug.h"

//using namespace Pilgrim3Game;

namespace Ring {

ApplicationPilgrim3::ApplicationPilgrim3(RingEngine *engine) : Application(engine) {
	_eventHandler = new EventHandlerPilgrim3(this);

	_systemZone = kZone100;
}

ApplicationPilgrim3::~ApplicationPilgrim3() {
}

#pragma region Game setup

void ApplicationPilgrim3::initFont() {
	// Original asks for size 12, but the font only contains size 8
	fontAdd(kFontDefault, "secrets.fon", "Arxel1", 8, true, false, false, false, getCurrentLanguage());
}

void ApplicationPilgrim3::setup() {
	// Add the list of episodes
	addEpisode(kZone7,  "Six",   1);
	addEpisode(kZone8,  "Seven", 1);
	addEpisode(kZone9,  "Eight", 2);
	addEpisode(kZone10, "Nine", 1);
	addEpisode(kZone11, "Ten", 2);

	// Add the list of zones
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

void ApplicationPilgrim3::initData() {
	_field_74 = true;
	_field_75 = true;
	_field_76 = true;
	_field_77 = false;
	_field_78 = true;
}

void ApplicationPilgrim3::initBag() {
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

void ApplicationPilgrim3::showStartupScreen() {
	error("[ApplicationPilgrim3::showStartupScreen] Not implemented");
}

void ApplicationPilgrim3::startMenu(bool savegame) {
	error("[ApplicationPilgrim3::startMenu] Not implemented");
}

void ApplicationPilgrim3::showMenu(ZoneId zone, MenuAction menuAction) {
	error("[ApplicationPilgrim3::showMenu] Not implemented");
}

void ApplicationPilgrim3::showCredits() {
	error("[ApplicationPilgrim3::showCredits] Not implemented");
}

void ApplicationPilgrim3::loadPreferences() {
	error("[ApplicationPilgrim3::loadPreferences] Not implemented");
}

#pragma endregion

#pragma region Drawing

void ApplicationPilgrim3::draw() {
	// Update our screen
	_screenManager->updateScreen();

	// Update engine state
	error("[ApplicationPilgrim3::draw] Engine state update not implemented!");
}

#pragma endregion

#pragma region Visual

Visual *ApplicationPilgrim3::createVisual(Id visualId, uint32 a3, uint32 a4, uint32 left, uint32 top, uint32 offsetY, uint32 height, uint32 progressMultiplier, uint32 progressColor) {
	error("[ApplicationPilgrim3::createVisual] Engine state update not implemented!");
}

#pragma endregion

#pragma region Zone initialization

void ApplicationPilgrim3::initZones() {
	debugC(kRingDebugLogic, "Init zone data");

	_loadFrom = kLoadFromDisk;

	drawZoneName(kZone100);
	_archiveType = getZoneArchiveType(kZone100);
	_eventHandler->onInitZone(kZone100);

	_loadFrom = kLoadFromCd;

	drawZoneName(kZone6);
	_archiveType = getZoneArchiveType(kZone6);
	_eventHandler->onInitZone(kZone6);

	drawZoneName(kZone7);
	_archiveType = getZoneArchiveType(kZone7);
	_eventHandler->onInitZone(kZone7);

	drawZoneName(kZone8);
	_archiveType = getZoneArchiveType(kZone8);
	_eventHandler->onInitZone(kZone8);

	drawZoneName(kZone9);
	_archiveType = getZoneArchiveType(kZone9);
	_eventHandler->onInitZone(kZone9);

	drawZoneName(kZone10);
	_archiveType = getZoneArchiveType(kZone10);
	_eventHandler->onInitZone(kZone10);

	_archiveType = getZoneArchiveType(getCurrentZone());

	setState(kStateNone);

	// Clear screen
	_screenManager->clear();
	g_system->updateScreen();
}

#pragma endregion

} // End of namespace Ring
