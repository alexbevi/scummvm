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

#include "ring/game/pompeii/pompeii_event.h"

#include "ring/graphics/screen.h"

#include "ring/sound/soundhandler.h"

#include "ring/debug.h"

#include "common/textconsole.h"

//using namespace PompeiiGame;

namespace Ring {

ApplicationPompeii::ApplicationPompeii(RingEngine *engine) : Application(engine) {
	_eventHandler = new EventHandlerPompeii(this);

	_systemZone = kZone100;
}

ApplicationPompeii::~ApplicationPompeii() {
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
	initZoneSystem();

	_loadFrom = kLoadFromCd;

	drawZoneName(kZone1);
	_archiveType = getZoneArchiveType(kZone2);
	initZone1();

	drawZoneName(kZone2);
	_archiveType = getZoneArchiveType(kZone2);
	initZone2();

	drawZoneName(kZone2);
	_archiveType = getZoneArchiveType(kZone3);
	initZone3();

	drawZoneName(kZone4);
	_archiveType = getZoneArchiveType(kZone4);
	initZone4();

	drawZoneName(kZone5);
	_archiveType = getZoneArchiveType(kZone5);
	initZone5();

	drawZoneName(kZone6);
	_archiveType = getZoneArchiveType(kZone6);
	initZone6();

	drawZoneName(kZone7);
	_archiveType = getZoneArchiveType(kZone7);
	initZone7();

	drawZoneName(kZone8);
	_archiveType = getZoneArchiveType(kZone8);
	initZone8();

	drawZoneName(kZone9);
	_archiveType = getZoneArchiveType(kZone9);
	initZone9();

	drawZoneName(kZone10);
	_archiveType = getZoneArchiveType(kZone10);
	initZone10();

	_loadFrom = kLoadFromDisk;

	drawZoneName(kZone11);
	_archiveType = getZoneArchiveType(kZone11);
	initZone11();

	drawZoneName(kZone12);
	_archiveType = getZoneArchiveType(kZone12);
	initZone12();

	_archiveType = getZoneArchiveType(getCurrentZone());

	setState(kStateNone);

	// Clear screen
	_screenManager->clear();
	g_system->updateScreen();
}

#pragma endregion

#pragma region Zone initialization

////////////////////////////////////////////////////////////////////////////
// Zone initialization
//////////////////////////////////////////////////////////////////////////
void ApplicationPompeii::initZoneSystem() {
	error("[ApplicationPompeii::initZoneSystem] Not implemented");
}

void ApplicationPompeii::initZone1() {
	error("[ApplicationPompeii::initZone1] Not implemented");
}

void ApplicationPompeii::initZone2() {
	error("[ApplicationPompeii::initZone2] Not implemented");
}

void ApplicationPompeii::initZone3() {
	error("[ApplicationPompeii::initZone3] Not implemented");
}

void ApplicationPompeii::initZone4() {
	error("[ApplicationPompeii::initZone4] Not implemented");
}

void ApplicationPompeii::initZone5() {
	error("[ApplicationPompeii::initZone5] Not implemented");
}

void ApplicationPompeii::initZone6() {
	error("[ApplicationPompeii::initZone6] Not implemented");
}

void ApplicationPompeii::initZone7() {
	error("[ApplicationPompeii::initZone7] Not implemented");
}

void ApplicationPompeii::initZone8() {
	error("[ApplicationPompeii::initZone8] Not implemented");
}

void ApplicationPompeii::initZone9() {
	error("[ApplicationPompeii::initZone9] Not implemented");
}

void ApplicationPompeii::initZone10() {
	error("[ApplicationPompeii::initZone10] Not implemented");
}

void ApplicationPompeii::initZone11() {
	error("[ApplicationPompeii::initZone11] Not implemented");
}

void ApplicationPompeii::initZone12() {
	error("[ApplicationPompeii::initZone12] Not implemented");
}

#pragma endregion

} // End of namespace Ring
