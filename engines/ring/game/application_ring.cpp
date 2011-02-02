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

#include "ring/game/application_ring.h"

#include "ring/base/art.h"
#include "ring/base/sound.h"

#include "ring/game/bag.h"
#include "ring/game/saveload.h"

#include "ring/helpers.h"
#include "ring/ring.h"

#include "common/archive.h"

namespace Ring {

ApplicationRing::ApplicationRing(RingEngine *engine) : Application(engine) {
}

ApplicationRing::~ApplicationRing() {

}

//////////////////////////////////////////////////////////////////////////
// Game setup
//////////////////////////////////////////////////////////////////////////
void ApplicationRing::setup() {
	setupZone(kZoneSY, 0);

	// Setup cursors
	ArchiveType archiveType = (_configuration.artCURSOR ? kArchiveArt : kArchiveFile);

	cursorAdd(kCursorDefault,       "",                kCursorTypeNormal,   1, kImageCursor, archiveType);
	cursorAdd(kCursorBusy,          "cur_busy",        kCursorTypeImage,    1, kImageCursor, archiveType);
	cursorAdd(kCursorHandSelection, "ni_handsel",      kCursorTypeImage,    1, kImageCursor, archiveType);
	cursorAdd(kCursorIdle,          "cur_idle",        kCursorTypeAnimated, 1, 15, 1095237632, 4, kImageCursor, archiveType);
	cursorAdd(kCursorMove,          "cur_muv",         kCursorTypeAnimated, 1, 20, 1095237632, 4, kImageCursor, archiveType);
	cursorAdd(kCursorHotspot,       "cur_hotspot",     kCursorTypeAnimated, 1, 19, 1095237632, 4, kImageCursor, archiveType);
	cursorAdd(kCursorBack,          "cur_back",        kCursorTypeImage,    1, kImageCursor, archiveType);
	cursorAdd(kCursorMenuIdle,      "cur_menuidle",    kCursorTypeNormal,   1, kImageCursor, archiveType);
	cursorAdd(kCursorMenuActive,    "cur_menuactive",  kCursorTypeImage,    1, kImageCursor, archiveType);

	// Adjust offsets
	cursorSetOffset(kCursorHandSelection, Common::Point(15, 15));
	cursorSetOffset(kCursorIdle, Common::Point(10, 6));
	cursorSetOffset(kCursorMove, Common::Point(10, 6));
	cursorSetOffset(kCursorHotspot, Common::Point(10, 6));
	cursorSetOffset(kCursorBack, Common::Point(10, 20));

	// Setup subtitles
	subtitleSetColor(Color(255, 255, 255));
	subtitleSetBackgroundColor(Color(50, 50, 50));
}

//////////////////////////////////////////////////////////////////////////
// Timer
//////////////////////////////////////////////////////////////////////////
void ApplicationRing::onZoneTimer(TimerId timerId) {
	switch (getCurrentZone()) {
	default:
	case kZoneSY:
	case kZoneWA:
		break;

	case kZoneNI:
		onZoneTimerNI(timerId);
		break;

	case kZoneRH:
		onZoneTimerRH(timerId);
		break;

	case kZoneFO:
		onZoneTimerFO(timerId);
		break;

	case kZoneRO:
		onZoneTimerRO(timerId);
		break;

	case kZoneAS:
		onZoneTimerAS(timerId);
		break;

	case kZoneN2:
		onZoneTimerN2(timerId);
		break;
	}
}

void ApplicationRing::onZoneTimerNI(TimerId id) {
	error("[ApplicationRing::onZoneTimerNI] Not implemented");
}

void ApplicationRing::onZoneTimerRH(TimerId id) {
	error("[ApplicationRing::onZoneTimerNI] Not implemented");
}

void ApplicationRing::onZoneTimerFO(TimerId id) {
	error("[ApplicationRing::onZoneTimerNI] Not implemented");
}

void ApplicationRing::onZoneTimerRO(TimerId id) {
	error("[ApplicationRing::onZoneTimerNI] Not implemented");
}

void ApplicationRing::onZoneTimerAS(TimerId id) {
	error("[ApplicationRing::onZoneTimerNI] Not implemented");
}

void ApplicationRing::onZoneTimerN2(TimerId id) {
	error("[ApplicationRing::onZoneTimerNI] Not implemented");
}


//////////////////////////////////////////////////////////////////////////
// Zone init and setup
//////////////////////////////////////////////////////////////////////////
void ApplicationRing::setupZone(Zone zone, uint32 a2)  {
	bool load = _saveLoad->isLoaded(a2);

	// Check saved data for zone and/or puzzle id
	if (!load) {
		if (zone == kZoneSY) {
			load = true;
		} else {
			error("[ApplicationRing::setupZone] Zone CD check not implemented");
		}
	}

	puzzleReset();
	warning("[ApplicationRing::setupZone] Missing function call");

	getSoundHandler()->reset();

	if (zone != kZoneSY)
		getArtHandler()->remove();

	if (load) {
		setZoneAndEnableBag(zone);
		setZone(zone, a2);
	} else {
		error("[ApplicationRing::setupZone] CD check not implemented");
	}
}

void ApplicationRing::setZoneAndEnableBag(Zone zone) {
	_zone = zone;
	_zoneString = getZoneString(zone);

	// Enable or disable bag
	if (zone == kZoneSY || zone == kZoneAS)
		getBag()->disable();
	else
		getBag()->enable();
}

void ApplicationRing::setZone(Zone zone, uint32 a2) {
	bool load = _saveLoad->isLoaded(a2);

	if (zone != kZoneSY && !load) {
		if (getReadFrom(zone) == kArchiveArt) {
			if (!getArtHandler())
				error("[ApplicationRing::setZone] Art handler is not initialized properly");

			getArtHandler()->open(zone, kLoadFromCd);
		}
	}

	if (a2 == 1000) {
		error("[ApplicationRing::setZone] Not implemented (a2 == 1000)");
	}

	// Set zone
	switch (zone) {
	default:
	case kZoneSY:
		break;

	case kZoneNI:
		setZoneNI(zone, a2);
		break;

	case kZoneRH:
		setZoneRH(zone, a2);
		break;

	case kZoneFO:
		setZoneFO(zone, a2);
		break;

	case kZoneRO:
		setZoneRO(zone, a2);
		break;

	case kZoneWA:
		setZoneWA(zone, a2);
		break;

	case kZoneAS:
		setZoneAS(zone, a2);
		break;

	case kZoneN2:
		setZoneN2(zone, a2);
		break;
	}
}

void ApplicationRing::setZoneNI(Zone zone, uint32 a2) {
	error("[ApplicationRing::setZoneNI] Not implemented");
}

void ApplicationRing::setZoneRH(Zone zone, uint32 a2) {
	error("[ApplicationRing::setZoneRH] Not implemented");
}

void ApplicationRing::setZoneFO(Zone zone, uint32 a2) {
	error("[ApplicationRing::setZoneFO] Not implemented");
}

void ApplicationRing::setZoneRO(Zone zone, uint32 a2) {
	error("[ApplicationRing::setZoneRO] Not implemented");
}

void ApplicationRing::setZoneWA(Zone zone, uint32 a2) {
	error("[ApplicationRing::setZoneWA] Not implemented");
}

void ApplicationRing::setZoneAS(Zone zone, uint32 a2) {
	error("[ApplicationRing::setZoneAS] Not implemented");
}

void ApplicationRing::setZoneN2(Zone zone, uint32 a2) {
	error("[ApplicationRing::setZoneN2] Not implemented");
}

//////////////////////////////////////////////////////////////////////////
// Zone full names, short string and ReadFrom
//////////////////////////////////////////////////////////////////////////
Common::String ApplicationRing::getZoneString(Zone zone) const {
	switch (zone) {
	default:
		break;

	case kZoneSY:
		return "sy";

	case kZoneNI:
		return "ni";

	case kZoneRH:
		return "rh";

	case kZoneFO:
		return "fo";

	case kZoneRO:
		return "ro";

	case kZoneWA:
		return "wa";

	case kZoneAS:
		return "as";

	case kZoneN2:
		return "n2";
	}

	error("[Application::getZone] Invalid zone (%d)", zone);
}

Common::String ApplicationRing::getZoneLongName(Zone zone) const {
	switch (zone) {
	default:
		break;

	case kZoneSY:
		return "";

	case kZoneNI:
	case kZoneRH:
		return "Alberich";

	case kZoneFO:
		return "Siegmund";

	case kZoneWA:
		return "Brnnnhilde";

	case kZoneAS:
		return "Dril";

	case kZoneRO:
	case kZoneN2:
		return "Loge";
	}

	error("[Application::getZoneName] Invalid zone (%d)", zone);
}

uint32 ApplicationRing::getReadFrom(Zone zone) const {
	if (_archiveType == kArchiveFile)
		return kArchiveFile;

	switch (zone) {
	default:
		break;

	case kZoneSY:
		return _configuration.artSY ? kArchiveArt : kArchiveFile;

	case kZoneNI:
		return _configuration.artNI ? kArchiveArt : kArchiveFile;

	case kZoneRH:
		return _configuration.artRH ? kArchiveArt : kArchiveFile;

	case kZoneFO:
		return _configuration.artFO ? kArchiveArt : kArchiveFile;

	case kZoneRO:
		return _configuration.artRO ? kArchiveArt : kArchiveFile;

	case kZoneWA:
		return _configuration.artWA ? kArchiveArt : kArchiveFile;

	case kZoneAS:
		return _configuration.artAS ? kArchiveArt : kArchiveFile;

	case kZoneN2:
		return _configuration.artN2 ? kArchiveArt : kArchiveFile;
	}

	error("[ApplicationRing::getReadFrom] Invalid zone (%d)", zone);
}

//////////////////////////////////////////////////////////////////////////
// Initialization
//////////////////////////////////////////////////////////////////////////
void ApplicationRing::initZones() {
	_loadFrom = kLoadFromDisk;

	_archiveType = _configuration.artSY ? kArchiveArt : kArchiveFile;
	initZoneSY();

	_loadFrom = kLoadFromCd;

	_archiveType = _configuration.artAS ? kArchiveArt : kArchiveFile;
	initZoneAS();

	_archiveType = _configuration.artNI ? kArchiveArt : kArchiveFile;
	initZoneNI();

	_archiveType = _configuration.artN2 ? kArchiveArt : kArchiveFile;
	initZoneN2();

	_archiveType = _configuration.artRO ? kArchiveArt : kArchiveFile;
	initZoneRO();

	_archiveType = _configuration.artRH ? kArchiveArt : kArchiveFile;
	initZoneRH();

	_archiveType = _configuration.artFO ? kArchiveArt : kArchiveFile;
	initZoneFO();

	_archiveType = _configuration.artWA ? kArchiveArt : kArchiveFile;
	initZoneWA();

	if (_configuration.artSY || _configuration.artAS || _configuration.artNI || _configuration.artN2
	 || _configuration.artRO || _configuration.artRH || _configuration.artFO || _configuration.artWA)
		_archiveType = kArchiveArt;
	else
		_archiveType = kArchiveFile;

	_field_66 = 0;
}

void ApplicationRing::initZoneSY() {
	setZoneAndEnableBag(kZoneSY);

	//////////////////////////////////////////////////////////////////////////
	// Setup system and dialog boxes
	puzzleAdd(kPuzzle1);

	objectAdd(kObject1, "", "", 1);
	objectAddPresentation(kObject1);
	objectPresentationAddTextToPuzzle(kObject1, 0, kPuzzle1, "", 1, 16, 1, 0, -1, 0, -1, -1, -1);

	objectAdd(kObject6, "", "", 1);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 0, kPuzzle1, "FO_WOT1_W_A.tga", 0, 96, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 1, kPuzzle1, "FO_WOT1_W_B.tga", 0, 176, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 2, kPuzzle1, "FO_WOT1_W_C.tga", 0, 126, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 3, kPuzzle1, "FO_WOT1_W_D.tga", 0, 208, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 4, kPuzzle1, "FO_WOT1_W_E.tga", 0, 88, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 5, kPuzzle1, "FO_WOT1_B_A.tga", 415, 281, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 6, kPuzzle1, "FO_WOT1_B_C.tga", 488, 180, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 7, kPuzzle1, "FO_WOT1_B_E.tga", 195, 94, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 8, kPuzzle1, "FO_BRU1_W_A.tga", 0, 45, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 9, kPuzzle1, "FO_BRU1_B_A.tga", 440, 66, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 10, kPuzzle1, "FO_BRU1_B_B.tga", 490, 208, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 11, kPuzzle1, "FO_WOT2_W_A.tga", 0, 28, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 12, kPuzzle1, "FO_WOT2_W_B.tga", 0, 22, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 13, kPuzzle1, "FO_WOT2_W_C.tga", 0, 67, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 14, kPuzzle1, "FO_WOT2_W_D.tga", 0, 194, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 15, kPuzzle1, "FO_WOT2_B_A.tga", 477, 200, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 16, kPuzzle1, "FO_WOT2_B_C.tga", 496, 251, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 17, kPuzzle1, "FO_BRU2_W_A.tga", 0, 186, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 18, kPuzzle1, "FO_BRU2_B_A.tga", 443, 222, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 19, kPuzzle1, "FO_BRU2_B_B.tga", 493, 219, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 20, kPuzzle1, "FO_WOT3_W_A.tga", 0, 16, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 21, kPuzzle1, "FO_WOT3_W_B.tga", 0, 107, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 22, kPuzzle1, "FO_WOT3_W_C.tga", 0, 46, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 23, kPuzzle1, "FO_WOT3_W_D.tga", 0, 26, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 24, kPuzzle1, "FO_WOT3_B_A.tga", 508, 238, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 25, kPuzzle1, "FO_WOT3_B_B.tga", 381, 216, 1, 3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 26, kPuzzle1, "FO_WOT3_B_D.tga", 408, 246, 1, 3, 1000);

	objectAdd(kObject7, "", "", 1);
	objectAddPresentation(kObject7);
	objectPresentationAddImageToPuzzle(kObject7, 0, kPuzzle1, "SY_ISHA.bmp", 0, 16, 1, 1, 1000);

	objectAdd(kObject2, "", "", 1);
	objectAddPresentation(kObject2);
	objectPresentationAddImageToPuzzle(kObject2, 0, kPuzzle1, "Exit.bmp", 160, 165, 1, 1, 1000);
	objectAddPuzzleAccessibility(kObject2, kPuzzle1, Common::Rect(262, 270, 321, 306), 0, 57, 1);
	objectAddPuzzleAccessibility(kObject2, kPuzzle1, Common::Rect(310, 270, 370, 306), 0, 57, 0);
	objectSetPuzzleAccessibilityKey(kObject2, 0, 13);
	objectSetPuzzleAccessibilityKey(kObject2, 1, 27);
	objectAddPresentation(kObject2);
	objectPresentationAddImageToPuzzle(kObject2, 1, kPuzzle1, "ex_yes.bmp", 261, 279, 1, 1, 1000);
	objectAddPresentation(kObject2);
	objectPresentationAddImageToPuzzle(kObject2, 2, kPuzzle1, "ex_no.bmp", 318, 279, 1, 1, 1000);

	objectAdd(kObject3, "", "", 1);
	objectAddPresentation(kObject3);
	objectPresentationAddImageToPuzzle(kObject3, 0, kPuzzle1, "Warning.bmp", 160, 165, 1, 1, 1000);
	objectPresentationAddTextToPuzzle(kObject3, 0, kPuzzle1, "", 200, 200, 1, 255u, 95, 0, -1, -1, -1);
	objectPresentationAddTextToPuzzle(kObject3, 0, kPuzzle1, "", 300, 280, 1, 255u, 95, 0, -1, -1, -1);
	objectAddPresentation(kObject3);
	objectPresentationAddImageToPuzzle(kObject3, 1, kPuzzle1, "wr_ok.tga", 313, 281, 1, 3, 1000);
	objectAddPuzzleAccessibility(kObject3, kPuzzle1, Common::Rect(286, 269, 363, 307), 0, 57, 0);
	objectSetPuzzleAccessibilityKey(kObject3, 0, 13);

	objectAdd(kObject4, "", "", 1);
	objectAddPresentation(kObject4);
	objectPresentationAddImageToPuzzle(kObject4, 0, kPuzzle1, "Question.bmp", 160, 165, 1, 1, 1000);
	objectPresentationAddTextToPuzzle(kObject4, 0, kPuzzle1, "", 200, 200, 1, 255u, 95, 0, -1, -1, -1);
	objectPresentationAddTextToPuzzle(kObject4, 0, kPuzzle1, "", 200, 280, 1, 255u, 95, 0, -1, -1, -1);
	objectAddPresentation(kObject4);
	objectPresentationAddImageToPuzzle(kObject4, 1, kPuzzle1, "g_ok.tga", 181, 257, 1, 3, 1000);
	objectAddPresentation(kObject4);
	objectPresentationAddImageToPuzzle(kObject4, 2, kPuzzle1, "qu_cancel.tga", 181, 282, 1, 3, 1000);
	objectAddPuzzleAccessibility(kObject4, kPuzzle1, Common::Rect(277, 300, 347, 340), 0, 57, 0);
	objectAddPuzzleAccessibility(kObject4, kPuzzle1, Common::Rect(350, 300, 380, 340), 0, 57, 1);
	objectSetPuzzleAccessibilityKey(kObject4, 0, 13);
	objectSetPuzzleAccessibilityKey(kObject4, 1, 27);
	objectAddPuzzleAccessibility(kObject4, kPuzzle1, Common::Rect(180, 250, 250, 281), 0, 57, 2);
	objectAddPuzzleAccessibility(kObject4, kPuzzle1, Common::Rect(180, 283, 250, 309), 0, 57, 3);
	objectSetPuzzleAccessibilityKey(kObject4, 2, 13);
	objectSetPuzzleAccessibilityKey(kObject4, 3, 27);
	objectAddPuzzleAccessibility(kObject4, kPuzzle1, Common::Rect(180, 250, 250, 281), 0, 57, 4);
	objectAddPuzzleAccessibility(kObject4, kPuzzle1, Common::Rect(180, 283, 250, 309), 0, 57, 5);
	objectSetPuzzleAccessibilityKey(kObject4, 4, 13);
	objectSetPuzzleAccessibilityKey(kObject4, 5, 27);

	objectAdd(kObject5, "", "", 1);
	puzzleAdd(kPuzzleInsertCd);
	puzzleAddBackgroundImage(kPuzzleInsertCd, "insertcd.bmp", 0, 16, 1);

	objectAdd(kObject90912, "", "", 1);
	objectAddPresentation(kObject90912);
	objectPresentationAddImageToPuzzle(kObject90912, 0, kPuzzleInsertCd, "g_ok.tga", 306, 274, 1, 3, 1000);
	objectAddPresentation(kObject90912);
	objectPresentationAddTextToPuzzle(kObject90912, 1, kPuzzleInsertCd, "", 320, 240, 1, 0xFFu, 95, 0, -1, -1, -1);
	objectPresentationAddTextToPuzzle(kObject90912, 1, kPuzzleInsertCd, "", 320, 260, 1, 0xFFu, 95, 0, -1, -1, -1);
	objectPresentationShow(kObject90912, 1);
	objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), 1, 57, 0);
	objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), 1, 57, 1);
	objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), 1, 57, 2);
	objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), 1, 57, 3);
	objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), 1, 57, 4);
	objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), 1, 57, 5);
	objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), 1, 57, 6);
	objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), 1, 57, 7);
	objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), 1, 57, 8);
	objectSetPuzzleAccessibilityKey(kObject90912, 0, 13);
	objectSetPuzzleAccessibilityKey(kObject90912, 1, 13);
	objectSetPuzzleAccessibilityKey(kObject90912, 2, 13);
	objectSetPuzzleAccessibilityKey(kObject90912, 3, 13);
	objectSetPuzzleAccessibilityKey(kObject90912, 4, 13);
	objectSetPuzzleAccessibilityKey(kObject90912, 5, 13);
	objectSetPuzzleAccessibilityKey(kObject90912, 6, 13);
	objectSetPuzzleAccessibilityKey(kObject90912, 7, 13);
	objectSetPuzzleAccessibilityKey(kObject90912, 8, 13);

	//////////////////////////////////////////////////////////////////////////
	// Menu
	puzzleAdd(kPuzzleMenu);
	puzzleAddBackgroundImage(kPuzzleMenu, "GenMen.bmp", 0, 16, 1);
	puzzleAdd(kPuzzlePreferences);
	puzzleAddBackgroundImage(kPuzzlePreferences, "Preferences.bmp", 0, 16, 1);
	puzzleAdd(kPuzzleSave);
	puzzleAddBackgroundImage(kPuzzleSave, "Save.bmp", 0, 16, 1);
	puzzleAdd(kPuzzleLoad);
	puzzleAddBackgroundImage(kPuzzleLoad, "Load.bmp", 0, 16, 1);
	puzzleAdd(kPuzzleGameStatus);
	puzzleAddBackgroundImage(kPuzzleGameStatus, "GameStat.bmp", 0, 16, 1);

	// Menu objects
	objectAdd(kObjectMenuNewGame, "", "", 1);
	objectAddPuzzleAccessibility(kObjectMenuNewGame, kPuzzleMenu, Common::Rect(148, 69, 500, 99), 1, 57, 0);
	objectAddPresentation(kObjectMenuNewGame);
	objectPresentationAddImageToPuzzle(kObjectMenuNewGame, 0, kPuzzleMenu, "gm_new.bmp", 148, 85, 1, 1, 1000);
	objectAdd(kObjectMenuPreferences, "", "", 1);
	objectAddPuzzleAccessibility(kObjectMenuPreferences, kPuzzleMenu, Common::Rect(148, 105, 500, 135), 1, 57, 0);
	objectAddPresentation(kObjectMenuPreferences);
	objectPresentationAddImageToPuzzle(kObjectMenuPreferences, 0, kPuzzleMenu, "gm_pre.bmp", 148, 121, 1, 1, 1000);
	objectAdd(kObjectMenuLoad, "", "", 1);
	objectAddPuzzleAccessibility(kObjectMenuLoad, kPuzzleMenu, Common::Rect(148, 168, 500, 198), 1, 57, 0);
	objectAddPresentation(kObjectMenuLoad);
	objectPresentationAddImageToPuzzle(kObjectMenuLoad, 0, kPuzzleMenu, "gm_loa.bmp", 148, 184, 1, 1, 1000);
	objectAdd(kObjectMenuSave, "", "", 1);
	objectAddPuzzleAccessibility(kObjectMenuSave, kPuzzleMenu, Common::Rect(148, 236, 500, 266), 1, 57, 0);
	objectAddPresentation(kObjectMenuSave);
	objectPresentationAddImageToPuzzle(kObjectMenuSave, 0, kPuzzleMenu, "gm_sav.bmp", 148, 252, 1, 1, 1000);
	objectAdd(kObjectMenuContinue, "", "", 1);
	objectAddPuzzleAccessibility(kObjectMenuContinue, kPuzzleMenu, Common::Rect(148, 303, 500, 333), 1, 57, 0);
	objectAddPresentation(kObjectMenuContinue);
	objectPresentationAddImageToPuzzle(kObjectMenuContinue, 0, kPuzzleMenu, "gm_con.bmp", 148, 319, 1, 1, 1000);
	objectAdd(kObjectMenuStatus, "", "", 1);
	objectAddPuzzleAccessibility(kObjectMenuStatus, kPuzzleMenu, Common::Rect(148, 342, 500, 372), 1, 57, 0);
	objectAddPresentation(kObjectMenuStatus);
	objectPresentationAddImageToPuzzle(kObjectMenuStatus, 0, kPuzzleMenu, "gm_sta.bmp", 148, 358, 1, 1, 1000);
	objectAdd(kObjectMenuExit, "", "", 1);
	objectAddPuzzleAccessibility(kObjectMenuExit, kPuzzleMenu, Common::Rect(148, 380, 500, 410), 1, 57, 0);
	objectAddPresentation(kObjectMenuExit);
	objectPresentationAddImageToPuzzle(kObjectMenuExit, 0, kPuzzleMenu, "gm_exi.bmp", 148, 396, 1, 1, 1000);

	// Preferences objects
	objectAdd(kObjectPreferencesCancel, "", "", 1);
	objectAddPuzzleAccessibility(kObjectPreferencesCancel, kPuzzlePreferences, Common::Rect(410, 420, 490, 445), 1, 57, 0);
	objectAddPresentation(kObjectPreferencesCancel);
	objectPresentationAddImageToPuzzle(kObjectPreferencesCancel, 0, kPuzzlePreferences, "g_cancel.tga", 407, 421, 1, 3, 1000);
	objectSetPuzzleAccessibilityKey(kObjectPreferencesCancel, 0, 27);
	objectAdd(kObjectPreferencesOk, "", "", 1);
	objectAddPuzzleAccessibility(kObjectPreferencesOk, kPuzzlePreferences, Common::Rect(320, 420, 370, 445), 1, 57, 0);
	objectAddPresentation(kObjectPreferencesOk);
	objectPresentationAddImageToPuzzle(kObjectPreferencesOk, 0, kPuzzlePreferences, "g_ok.tga", 328, 421, 1, 3, 1000);
	objectSetPuzzleAccessibilityKey(kObjectPreferencesOk, 0, 13);
	objectAdd(kObjectPreferencesSubtitles, "", "", 1);
	objectAddPuzzleAccessibility(kObjectPreferencesSubtitles, kPuzzlePreferences, Common::Rect(310, 315, 370, 350), 1, 57, 0);
	objectAddPuzzleAccessibility(kObjectPreferencesSubtitles, kPuzzlePreferences, Common::Rect(400, 315, 460, 350), 1, 57, 1);
	objectAddPresentation(kObjectPreferencesSubtitles);
	objectPresentationAddImageToPuzzle(kObjectPreferencesSubtitles, 0, kPuzzlePreferences, "pr_on.bmp", 317, 326, 1, 1, 1000);
	objectAddPresentation(kObjectPreferencesSubtitles);
	objectPresentationAddImageToPuzzle(kObjectPreferencesSubtitles, 1, kPuzzlePreferences, "pr_off.bmp", 402, 326, 1, 1, 1000);
	objectAdd(kObjectPreferences3dSound, "", "", 1);
	objectAddPuzzleAccessibility(kObjectPreferences3dSound, kPuzzlePreferences, Common::Rect(355, 260, 420, 295), 1, 57, 1);
	objectAddPresentation(kObjectPreferences3dSound);
	objectPresentationAddImageToPuzzle(kObjectPreferences3dSound, 0, kPuzzlePreferences, "pr_left.tga", 336, 288, 1, 3, 1000);
	objectPresentationAddImageToPuzzle(kObjectPreferences3dSound, 0, kPuzzlePreferences, "pr_right.tga", 428, 288, 1, 3, 1000);
	objectAddPresentation(kObjectPreferences3dSound);
	objectPresentationAddImageToPuzzle(kObjectPreferences3dSound, 1, kPuzzlePreferences, "pr_right.tga", 336, 288, 1, 3, 1000);
	objectPresentationAddImageToPuzzle(kObjectPreferences3dSound, 1, kPuzzlePreferences, "pr_left.tga", 428, 288, 1, 3, 1000);
	objectAddPresentation(kObjectPreferences3dSound);
	objectPresentationAddImageToPuzzle(kObjectPreferences3dSound, 2, kPuzzlePreferences, "pr_3ds.tga", 356, 281, 1, 3, 1000);
	objectAdd(kObjectPreferencesSliderVolume, "", "ni_handsel", 4);
	objectAddPuzzleAccessibility(kObjectPreferencesSliderVolume, kPuzzlePreferences, Common::Rect(300, 140, 600, 180), 1, 57, 1);
	objectAddPresentation(kObjectPreferencesSliderVolume);
	objectPresentationAddImageToPuzzle(kObjectPreferencesSliderVolume, 0, kPuzzlePreferences, "pr_slider.tga", 314, 155, 1, 3, 1000);
	objectPresentationShow(kObjectPreferencesSliderVolume);
	objectSetActiveDrawCursor(kObjectPreferencesSliderVolume, 15, 15, 0, 3, 0, 0, 3);
	objectSetPassiveDrawCursor(kObjectPreferencesSliderVolume, 15, 15, 0, 3, 0, 0, 3);
	objectAdd(kObjectPreferencesSliderDialog, "", "ni_handsel", 4);
	objectAddPuzzleAccessibility(kObjectPreferencesSliderDialog, kPuzzlePreferences, Common::Rect(300, 197, 600, 237), 1, 57, 1);
	objectAddPresentation(kObjectPreferencesSliderDialog);
	objectPresentationAddImageToPuzzle(kObjectPreferencesSliderDialog, 0, kPuzzlePreferences, "pr_slider.tga", 314, 212, 1, 3, 1000);
	objectPresentationShow(kObjectPreferencesSliderDialog);
	objectSetActiveDrawCursor(kObjectPreferencesSliderDialog, 15, 15, 0, 3, 0, 0, 3);
	objectSetPassiveDrawCursor(kObjectPreferencesSliderDialog, 15, 15, 0, 3, 0, 0, 3);
	objectAdd(kObject90107, "", "", 1);
	objectAddPuzzleAccessibility(kObject90107, kPuzzlePreferences, Common::Rect(0, 448, 20, 640), 1, 57, 1);

	// Loading screen objects
	objectAdd(kObjectLoadOk, "", "", 1);
	objectAddPuzzleAccessibility(kObjectLoadOk, kPuzzleLoad, Common::Rect(325, 418, 375, 461), 1, 57, 0);
	objectAddPresentation(kObjectLoadOk);
	objectPresentationAddImageToPuzzle(kObjectLoadOk, 0, kPuzzleLoad, "g_ok.tga", 328, 421, 1, 3, 1000);
	objectSetPuzzleAccessibilityKey(kObjectLoadOk, 0, 13);
	objectAdd(kObjectLoadCancel, "", "", 1);
	objectAddPuzzleAccessibility(kObjectLoadCancel, kPuzzleLoad, Common::Rect(416, 418, 498, 461), 1, 57, 0);
	objectAddPresentation(kObjectLoadCancel);
	objectPresentationAddImageToPuzzle(kObjectLoadCancel, 0, kPuzzleLoad, "g_cancel.tga", 407, 421, 1, 3, 1000);
	objectSetPuzzleAccessibilityKey(kObjectLoadCancel, 0, 27);

	// Saving screen objects
	objectAdd(kObjectSaveOk, "", "", 1);
	objectAddPuzzleAccessibility(kObjectSaveOk, kPuzzleSave, Common::Rect(325, 418, 375, 461), 1, 57, 0);
	objectAdd(kObjectSaveCancel, "", "", 1);
	objectAddPuzzleAccessibility(kObjectSaveCancel, kPuzzleSave, Common::Rect(416, 418, 498, 461), 1, 57, 0);
	objectSetPuzzleAccessibilityKey(kObjectSaveCancel, 0, 27);
	objectAddPresentation(kObjectSaveOk);
	objectPresentationAddImageToPuzzle(kObjectSaveOk, 0, kPuzzleSave, "g_ok.tga", 328, 421, 1, 3, 1000);
	objectAddPresentation(kObjectSaveCancel);
	objectPresentationAddImageToPuzzle(kObjectSaveCancel, 0, kPuzzleSave, "g_cancel.tga", 407, 421, 1, 3, 1000);
	objectSetPuzzleAccessibilityKey(kObjectSaveOk, 0, 13);
	objectAdd(kObjectSave90313, "", "", 1);
	objectAddPresentation(kObjectSave90313);
	objectPresentationAddTextToPuzzle(kObjectSave90313, 0, kPuzzleSave, "", 344, 181, 1, 0xFFu, 95, 0, -1, -1, -1);
	objectPresentationAddTextToPuzzle(kObjectSave90313, 0, kPuzzleSave, "", 0, 0, 1, 0xFFu, 95, 0, -1, -1, -1);
	objectPresentationAddAnimationToPuzzle(kObjectSave90313, 0, kPuzzleSave, "kybcur", 0, 0, 0, 1, 1000, 6, 1095237632, 16);
	objectPresentationSetAnimationCoordinatesOnPuzzle(kObjectSave90313, 0, Common::Point(346, 181));

	_archiveType = kArchiveFile;
	objectPresentationAddImageToPuzzle(kObjectSave90313, 0, kPuzzleSave, "osc.bmp", 0, 0, 1, 1, 1000);
	if ( _configuration.artSY )
		_archiveType = kArchiveArt;

	objectPresentationShow(kObjectSave90313, 0);

	visualAddListToPuzzle(1, kPuzzleLoad, 65,
	                            "", "" /* save folder */, "", "up_gun.tga", "up_gur.tga", "", "up_gua.tga", "down_gun.tga", "down_gur.tga", "",
	                            "down_gua.tga", "load_gun.tga", "load_gua.tga",
	                            3, 0, 0, 0, 0, 335, 127, 300, 35, 45,
	                            3, 330, 349, 320, 339, 40, 40, 330, 380, 320,
	                            370, 40, 40, 0, 0, 0, 1, 311, 137, 4,
	                            255, 95, 0, 245, 235, 50, -1, -1,-1, 1,
	                            kArchiveFile);

	// Game status screen
	objectAdd(kObjectStatusOk, "", "", 1);
	objectAddPuzzleAccessibility(kObjectStatusOk, kPuzzleGameStatus, Common::Rect(28, 79, 107, 109), 1, 57, 0);
	objectAddPresentation(kObjectStatusOk);
	objectPresentationAddImageToPuzzle(kObjectStatusOk, 0, kPuzzleGameStatus, "g_ok.tga", 46, 95, 1, 3, 1000);
	objectSetPuzzleAccessibilityKey(kObjectStatusOk, 0, 13);
	visualAddShowToPuzzle(2, kPuzzleGameStatus, 1, 4, 295, 343, 28, 4, 300, 38655);
	objectAdd(kObjectStatusProgress, "", "", 1);
	objectAddPresentation(kObjectStatusProgress);
	objectPresentationAddTextToPuzzle(kObjectStatusProgress, 0, kPuzzleGameStatus, "", 600, 327, 1, 0xFFu, -106, 0, -1, -1, -1);
	objectPresentationAddTextToPuzzle(kObjectStatusProgress, 0, kPuzzleGameStatus, "", 600, 356, 1, 0xFFu, -106, 0, -1, -1, -1);
	objectPresentationAddTextToPuzzle(kObjectStatusProgress, 0, kPuzzleGameStatus, "", 600, 384, 1, 0xFFu, -106, 0, -1, -1, -1);
	objectPresentationAddTextToPuzzle(kObjectStatusProgress, 0, kPuzzleGameStatus, "", 600, 410, 1, 0xFFu, -106, 0, -1, -1, -1);
	objectPresentationShow(kObjectStatusProgress);

	// Sounds
	soundAdd(90001, 5, "1844.wac", kLoadFromDisk, 2, _configuration.dialog.soundChunck);

	// Variables
	varDefineByte(90001, 0);
	varDefineByte(90002, 0);
	varDefineByte(90003, 0);
	varDefineByte(90004, 0);
	varDefineFloat(90005, 0);
	varDefineFloat(90006, 0);
	varDefineFloat(90007, 0);
	varDefineFloat(90008, 0);
	varDefineByte(90009, 0);
	varDefineByte(90010, 0);
	varDefineByte(90011, 0);
	varDefineByte(90012, 0);
	varDefineDword(90013, 0);
	varDefineDword(90014, 0);
	varDefineDword(90015, 0);
	varDefineDword(90016, 0);
	varDefineByte(90017, 0);
	varDefineByte(90018, 0);
	varDefineByte(90019, 0);
	varDefineByte(90020, 0);
	varDefineDword(90021, 0);
	varDefineDword(90022, 0);
	varDefineDword(90023, 0);
	varDefineDword(90024, 0);
	varDefineByte(90025, 0);
	varDefineByte(90026, 0);
	varDefineByte(90027, 0);
	varDefineByte(90028, 0);
}

void ApplicationRing::initZoneNI() {
	setZoneAndEnableBag(kZoneNI);

	rotationAdd(10000, "NIS00N01", 0, 2);
	rotationAdd(10001, "NIS00N03", 0, 2);
	rotationAdd(10002, "NIS00N04", 0, 2);
	rotationSetComBufferLength(10002, 1300000);
	rotationAdd(10003, "NIS00N05", 0, 2);
	rotationAdd(10004, "NIS00N06", 0, 2);
	rotationAdd(10005, "NIS00N07", 0, 2);
	rotationAdd(10101, "NIS01N01", 0, 1);
	rotationSetComBufferLength(10101, 1300000);
	rotationAdd(10102, "NIS01N02", 0, 1);
	rotationAdd(10201, "NIS02N01", 0, 1);
	rotationAdd(10301, "NIS03N01", 0, 1);
	rotationAdd(10401, "NIS04N01", 0, 2);
	rotationAdd(10402, "NIS04N02", 0, 2);
	rotationSetComBufferLength(10402, 1300000);
	rotationAdd(10403, "NIS04N03", 0, 0);
	rotationAdd(10404, "NIS04N04", 0, 0);
	rotationAdd(10405, "NIS04N05", 0, 0);
	rotationAdd(10415, "NIS04N15", 0, 0);
	rotationAdd(10406, "NIS04N06", 0, 0);
	rotationSetJugOn(10406, 10.0, 1.0);
	rotationAdd(10501, "NIS05N01", 0, 0);
	rotationAdd(10601, "NIS06N01", 0, 0);
	puzzleAdd(kPuzzle12001);
	puzzleAddBackgroundImage(kPuzzle12001, "TR_NI_RH_BP01.BMP", 0, 16, 1);
	puzzleAdd(kPuzzle12002);
	puzzleAddBackgroundImage(kPuzzle12002, "TR_NI_RH_BP02.BMP", 0, 16, 1);
	puzzleAdd(kPuzzle12003);
	puzzleAddBackgroundImage(kPuzzle12003, "TR_NI_RH_BP03.BMP", 0, 16, 1);
	puzzleAdd(kPuzzle10000);
	puzzleAddBackgroundImage(kPuzzle10000, "NIS00N01P01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10001);
	puzzleAddBackgroundImage(kPuzzle10001, "NIS00N01P01L01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10002);
	puzzleAddBackgroundImage(kPuzzle10002, "NIS00N01P01L02.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10100);
	puzzleAddBackgroundImage(kPuzzle10100, "NIS01N01P01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10102);
	puzzleAddBackgroundImage(kPuzzle10102, "NIS01N01P03.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10200);
	puzzleAddBackgroundImage(kPuzzle10200, "NIS02N01P01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10201);
	puzzleAddBackgroundImage(kPuzzle10201, "NIS02N01P02.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10202);
	puzzleAddBackgroundImage(kPuzzle10202, "NIS02N01P03S01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10203);
	puzzleAddBackgroundImage(kPuzzle10203, "NIS02N01P02S01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10204);
	puzzleAddBackgroundImage(kPuzzle10204, "NIS02N01P02S02.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10205);
	puzzleAddBackgroundImage(kPuzzle10205, "NIS02N01P02S03.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10391);
	puzzleAddBackgroundImage(kPuzzle10391, "NIS03N01P01S01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10390);
	puzzleAddBackgroundImage(kPuzzle10390, "NIS03N01P01S02.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10392);
	puzzleAddBackgroundImage(kPuzzle10392, "NIS03N01P03S01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10300);
	puzzleAddBackgroundImage(kPuzzle10300, "NIS03N01P02.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10400);
	puzzleAddBackgroundImage(kPuzzle10400, "NIS04N02P01L01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10401);
	puzzleAddBackgroundImage(kPuzzle10401, "NIS04N02P02L01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10410);
	puzzleAddBackgroundImage(kPuzzle10410, "NIS04N03P01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10411);
	puzzleAddBackgroundImage(kPuzzle10411, "NIS04N03P03.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10500);
	puzzleAddBackgroundImage(kPuzzle10500, "NIS05N01P01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10501);
	puzzleAddBackgroundImage(kPuzzle10501, "NIS05N01P01L01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10510);
	puzzleAddBackgroundImage(kPuzzle10510, "NIS05N01P02.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10511);
	puzzleAddBackgroundImage(kPuzzle10511, "NIS05N01P02L01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10520);
	puzzleAddBackgroundImage(kPuzzle10520, "NIS05N01P03.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10521);
	puzzleAddBackgroundImage(kPuzzle10521, "NIS05N01P03L01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle10600);
	puzzleAddBackgroundImage(kPuzzle10600, "NIS06N01P01.0001.bmp", 0, 16, 1);
	rotationAddMovabilityToRotation(10000, 10001, "1551", 1211, -137, 1494, 151, 0, 53, 1);
	rotationSetMovabilityToRotation(10000, 0, 1119092736, 1050253722, 1118529126, 0, 0, 1119092736, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10000, 10401, "1552", 2847, -364, 3394, 97, 0, 53, 41);
	rotationSetMovabilityToRotation(10000, 1, 1132920832, 1050253722, 1118529126, 0, 0, 1127481344, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10000, 10201, "1553", 300, -244, 1000, 209, 0, 53, 21);
	rotationSetMovabilityToRotation(10000, 2, 1119092736, 1050253722, 1118529126, 0, 0, 1132920832, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10001, 10002, "1554", 1239, -116, 1485, 127, 0, 53, 1);
	rotationSetMovabilityToRotation(10001, 0, 1119092736, 1050253722, 1118529126, 0, 0, 1119092736, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10001, 10000, "1555", 2998, -212, 3315, 163, 0, 53, 2);
	rotationSetMovabilityToRotation(10001, 1, 1132920832, 1050253722, 1118529126, 0, 0, 1132920832, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10002, 10004, "1556", 1212, -132, 1500, 190, 0, 53, 1);
	rotationSetMovabilityToRotation(10002, 0, 1119092736, 1050253722, 1118529126, 0, 0, 1119092736, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10002, 10001, "1557", 2951, -167, 3327, 218, 0, 53, 2);
	rotationSetMovabilityToRotation(10002, 1, 1132920832, 1050253722, 1118529126, 0, 0, 1132920832, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10002, 10003, "1558", 1600, -214, 1923, 169, 0, 53, 5);
	rotationSetMovabilityToRotation(10002, 2, 1119092736, 1050253722, 1118529126, 0, 0, 1119092736, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10003, 10002, "1559", 0, -156, 492, 219, 0, 53, 2);
	rotationSetMovabilityToRotation(10003, 0, 1134831206, 1050253722, 1118529126, 0, 0, 1132920832, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10003, 10301, "1560", 2141, -190, 2666, 265, 1, 53, 110);
	rotationSetMovabilityToRotation(10003, 1, 1128464384, 1050253722, 1118529126, 0, 2, 1125515264, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10004, 10005, "1561", 1263, -47, 1610, 221, 0, 53, 1);
	rotationSetMovabilityToRotation(10004, 0, 1119092736, 1050253722, 1118529126, 0, 0, 1119092736, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10004, 10002, "1562", 3009, -146, 3301, 160, 0, 53, 2);
	rotationSetMovabilityToRotation(10004, 1, 1132920832, 1050253722, 1118529126, 0, 0, 1132920832, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10005, 10101, "1563", 1121, -195, 1606, 331, 0, 53, 4);
	rotationSetMovabilityToRotation(10005, 0, 1119092736, 1050253722, 1118529126, 0, 2, 1132920832, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10005, 10004, "1564", 3009, -319, 3487, 172, 0, 53, 2);
	rotationSetMovabilityToRotation(10005, 1, 1132920832, 1050253722, 1118529126, 0, 0, 1132920832, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10101, 10102, "1565", 1186, -143, 1534, 195, 1, 53, 0);
	rotationSetMovabilityToRotation(10101, 0, 1119092736, 1050253722, 1118529126, 0, 2, 1119092736, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10102, 10101, "1566", 2951, -125, 3350, 193, 1, 53, 0);
	rotationSetMovabilityToRotation(10102, 0, 1132920832, 1050253722, 1118529126, 0, 2, 1132920832, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10102, 10601, "1567", 1323, -57, 1587, 225, 1, 53, 16);
	rotationSetMovabilityToRotation(10102, 1, 1119092736, 1050253722, 1118529126, 0, 2, 1124204544, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10102, 10601, "1568", 1323, -57, 1587, 225, 0, 53, 0);
	rotationSetMovabilityToRotation(10102, 2, 1119092736, 1050253722, 1118529126, 0, 2, 1130364928, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10201, 10000, "1509", 1947, -158, 2418, 301, 0, 53, 2);
	rotationSetMovabilityToRotation(10201, 0, 1128143258, 1050253722, 1118529126, 0, 0, 1132920832, 1050253722, 1118529126);
	rotationAddMovabilityToPuzzle(10201, kPuzzle10200, "1569", 198, -34, 495, 366, 1, 52, 110);
	rotationSetMovabilityToPuzzle(10201, 1, 1135378432, 1097859072, 1117782016, 0, 2);
	rotationAddMovabilityToRotation(10301, 10003, "1570", 318, -202, 789, 226, 1, 53, 100);
	rotationSetMovabilityToRotation(10301, 0, 1094713344, 1050253722, 1118529126, 0, 2, 1119092736, 1050253722, 1118529126);
	rotationAddMovabilityToPuzzle(10301, kPuzzle10300, "", 2272, -8, 2418, 144, 1, 53, 0);
	rotationSetMovabilityToPuzzle(10301, 1, 1128136704, 1050253722, 1118529126, 0, 0);
	rotationAddMovabilityToRotation(10401, 10000, "1571", 1123, -153, 1599, 279, 0, 53, 1);
	rotationSetMovabilityToRotation(10401, 0, 1119092736, 1050253722, 1118529126, 0, 0, 1119092736, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10401, 10402, "1572", 2953, -223, 3359, 155, 1, 53, 110);
	rotationSetMovabilityToRotation(10401, 1, 1132920832, 1050253722, 1118529126, 0, 2, 1132920832, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10402, 10401, "1573", 1081, 13, 1657, 286, 1, 53, 100);
	rotationSetMovabilityToRotation(10402, 0, 1119092736, 1050253722, 1118529126, 0, 2, 1127481344, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10402, 10403, "1574", 3002, -139, 3306, 190, 1, 53, 0);
	rotationSetMovabilityToRotation(10402, 1, 1132920832, 1050253722, 1118529126, 0, 2, 1132920832, 1050253722, 1118529126);
	rotationAddMovabilityToPuzzle(10402, kPuzzle10400, "", 2213, 87, 2371, 202, 1, 52, 41);
	rotationSetMovabilityToPuzzle(10402, 2, 1127809024, 1097859072, 1118529126, 0, 2);
	rotationAddMovabilityToPuzzle(10402, kPuzzle10401, "", 326, 87, 485, 198, 1, 52, 42);
	rotationSetMovabilityToPuzzle(10402, 3, 0, 1097859072, 1118529126, 0, 2);
	rotationAddMovabilityToRotation(10403, 10402, "1575", 1146, -94, 1560, 221, 1, 53, 0);
	rotationSetMovabilityToRotation(10403, 0, 1119092736, 1050253722, 1118529126, 0, 2, 1119092736, 1050253722, 1118529126);
	rotationAddMovabilityToPuzzle(10403, kPuzzle10410, "", 3041, -265, 3290, 0, 1, 52, 0);
	rotationSetMovabilityToPuzzle(10403, 1, 1132920832, 1050253722, 1118529126, 0, 2);
	rotationAddMovabilityToRotation(10404, 10403, "1576", 207, 249, 564, 423, 1, 52, 0);
	rotationSetMovabilityToRotation(10404, 0, 1119092736, 0, 1118529126, 0, 2, 1119092736, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10405, 10404, "1577", 1165, -221, 1527, 284, 1, 53, 0);
	rotationSetMovabilityToRotation(10405, 0, 1119092736, 1050253722, 1118529126, 0, 2, 1119092736, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10415, 10404, "1577", 1165, -221, 1527, 284, 1, 53, 0);
	rotationSetMovabilityToRotation(10415, 0, 1119092736, 1050253722, 1118529126, 0, 2, 1119092736, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(10501, 10601, "1579", 546, -345, 980, 120, 1, 53, 0);
	rotationSetMovabilityToRotation(10501, 0, 1107820544, 1050253722, 1118529126, 0, 2, 1110441984, 1050253722, 1118529126);
	rotationAddMovabilityToPuzzle(10501, kPuzzle10500, "", 1139, -204, 1520, 156, 1, 52, 0);
	rotationSetMovabilityToPuzzle(10501, 1, 1119092736, 1050253722, 1118529126, 0, 2);
	rotationAddMovabilityToPuzzle(10501, kPuzzle10510, "", 1787, -143, 2033, 108, 1, 52, 0);
	rotationSetMovabilityToPuzzle(10501, 2, 1125187584, 1050253722, 1118529126, 0, 2);
	rotationAddMovabilityToPuzzle(10501, kPuzzle10520, "", 2228, -118, 2411, 80, 1, 52, 0);
	rotationSetMovabilityToPuzzle(10501, 3, 1127809024, 1050253722, 1118529126, 0, 2);
	rotationAddMovabilityToRotation(10601, 10102, "1580", 863, -211, 1123, 101, 1, 53, 0);
	rotationSetMovabilityToRotation(10601, 0, 1110441984, 1050253722, 1118529126, 0, 2, 1132920832, 1050253722, 1118529126);
	rotationAddMovabilityToPuzzle(10601, kPuzzle10600, "", 2436, -256, 2951, 284, 1, 52, 61);
	rotationSetMovabilityToPuzzle(10601, 1, 1130391142, 1050253722, 1118529126, 0, 0);
	puzzleAddMovabilityToPuzzle(kPuzzle10102, 10100, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleAddMovabilityToRotation(kPuzzle10200, 10201, "1549", Common::Rect(0, 420, 640, 464), 1, 55, 100);
	puzzleSetMovabilityToRotation(kPuzzle10200, 0, 1135378432, 1050253722, 1118529126);
	puzzleAddMovabilityToRotation(kPuzzle10300, 10301, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle10300, 0, 1128136704, 1050253722, 1118529126);
	puzzleAddMovabilityToRotation(kPuzzle10400, 10402, "", Common::Rect(0, 420, 640, 464), 1, 55, 41);
	puzzleSetMovabilityToRotation(kPuzzle10400, 0, 1128136704, 1050253722, 1118529126);
	puzzleAddMovabilityToRotation(kPuzzle10401, 10402, "", Common::Rect(0, 420, 640, 464), 1, 55, 42);
	puzzleSetMovabilityToRotation(kPuzzle10401, 0, 1092616192, 1050253722, 1118529126);
	puzzleAddMovabilityToPuzzle(kPuzzle10410, 10411, "", Common::Rect(250, 280, 320, 370), 0, 52, 0);
	puzzleAddMovabilityToRotation(kPuzzle10410, 10403, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle10410, 1, 1132920832, 1050253722, 1118529126);
	puzzleAddMovabilityToRotation(kPuzzle10410, 10404, "1581", Common::Rect(455, 185, 534, 230), 1, 52, 0);
	puzzleSetMovabilityToRotation(kPuzzle10410, 2, 1132920832, 1050253722, 1118529126);
	puzzleAddMovabilityToPuzzle(kPuzzle10411, 10410, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleAddMovabilityToRotation(kPuzzle10500, 10501, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle10500, 0, 1119092736, 1050253722, 1118529126);
	puzzleAddMovabilityToPuzzle(kPuzzle10500, 10501, "", Common::Rect(200, 100, 400, 300), 1, 52, 0);
	puzzleAddMovabilityToPuzzle(kPuzzle10501, 10500, "", Common::Rect(430, 48, 640, 434), 1, 52, 0);
	puzzleAddMovabilityToRotation(kPuzzle10510, 10501, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle10510, 0, 1125122048, 1050253722, 1118529126);
	puzzleAddMovabilityToPuzzle(kPuzzle10510, 10511, "", Common::Rect(200, 100, 400, 300), 1, 52, 0);
	puzzleAddMovabilityToPuzzle(kPuzzle10511, 10510, "", Common::Rect(430, 48, 640, 434), 1, 52, 0);
	puzzleAddMovabilityToRotation(kPuzzle10520, 10501, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle10520, 0, 1127743488, 1050253722, 1118529126);
	puzzleAddMovabilityToPuzzle(kPuzzle10520, 10521, "", Common::Rect(200, 100, 400, 300), 1, 52, 0);
	puzzleAddMovabilityToPuzzle(kPuzzle10521, 10520, "", Common::Rect(430, 48, 640, 434), 1, 52, 0);
	puzzleAddMovabilityToPuzzle(kPuzzle10521, 10521, "1582", Common::Rect(216, 124, 406, 327), 1, 52, 55);
	objectAdd(kObject12000, "", "", 1);
	objectAddPresentation(kObject12000);
	objectPresentationAddAnimationToPuzzle(kObject12000, 0, kPuzzle12001, "TR_NI_RH_BP01S01", 0, 239, 208, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject12000);
	objectPresentationAddAnimationToPuzzle(kObject12000, 1, kPuzzle12002, "TR_NI_RH_BP02S01", 0, 187, 94, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject12000);
	objectPresentationAddAnimationToPuzzle(kObject12000, 2, kPuzzle12003, "TR_NI_RH_BP03S01", 0, 207, 134, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObject10003, "Bike", "", 1);
	objectAddPresentation(kObject10003);
	objectPresentationAddImageToRotation(kObject10003, 0, 10003, 1);
	objectAddPresentation(kObject10003);
	objectPresentationAddImageToRotation(kObject10003, 1, 10201, 0);
	objectAddPresentation(kObject10003);
	objectAddPresentation(kObject10003);
	objectPresentationAddImageToRotation(kObject10003, 3, 10000, 0);
	objectAddPresentation(kObject10003);
	objectPresentationAddImageToRotation(kObject10003, 4, 10001, 0);
	objectAddPresentation(kObject10003);
	objectPresentationAddImageToRotation(kObject10003, 5, 10002, 0);
	objectAddPresentation(kObject10003);
	objectPresentationAddImageToRotation(kObject10003, 6, 10004, 0);
	objectAddPresentation(kObject10003);
	objectPresentationAddImageToRotation(kObject10003, 7, 10005, 0);
	objectAddPresentation(kObject10003);
	objectPresentationAddImageToRotation(kObject10003, 8, 10000, 1);
	objectAddPresentation(kObject10003);
	objectPresentationAddImageToRotation(kObject10003, 9, 10001, 1);
	objectAddPresentation(kObject10003);
	objectPresentationAddImageToRotation(kObject10003, 10, 10002, 1);
	objectAddPresentation(kObject10003);
	objectPresentationAddImageToRotation(kObject10003, 11, 10004, 1);
	objectAddPresentation(kObject10003);
	objectPresentationAddImageToRotation(kObject10003, 12, 10005, 1);
	objectPresentationShow(kObject10003, 0);
	objectPresentationShow(kObject10003, 1);
	objectAdd(kObjectBrutality, "SP/Brutality", "NI_Brutality", 1);
	objectAddBagAnimation(kObjectBrutality, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectBrutality, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectBrutality, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectMinerals, "Minerals", "NI_Minerals", 1);
	objectAddBagAnimation(kObjectMinerals, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectMinerals, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectMinerals, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectGlug, "Glug", "NI_Glug", 1);
	objectAddBagAnimation(kObjectGlug, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectGlug, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectGlug, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectGlug, kPuzzle10000, Common::Rect(227, 206, 410, 394), 1, 52, 0);
	objectAddPuzzleAccessibility(kObjectGlug, kPuzzle10001, Common::Rect(48, 40, 567, 416), 1, 52, 1);
	objectAddPuzzleAccessibility(kObjectGlug, kPuzzle10002, Common::Rect(246, 188, 383, 322), 1, 52, 2);
	objectAddRotationAccessibility(kObjectGlug, 10003, Common::Rect(1089, 390, 1664, 680), 1, 52, 3);
	objectAddRotationAccessibility(kObjectGlug, 10401, Common::Rect(1963, 390, 2615, 680), 1, 52, 3);
	objectAddRotationAccessibility(kObjectGlug, 10201, Common::Rect(2830, 390, 3505, 680), 1, 52, 3);
	objectAddPuzzleAccessibility(kObjectGlug, kPuzzle10000, Common::Rect(0, 420, 640, 464), 1, 55, 4);
	objectAddPuzzleAccessibility(kObjectGlug, kPuzzle10001, Common::Rect(0, 420, 640, 464), 1, 55, 4);
	objectAddPuzzleAccessibility(kObjectGlug, kPuzzle10002, Common::Rect(0, 420, 640, 464), 1, 55, 4);
	objectAddPresentation(kObjectGlug);
	objectPresentationAddAnimationToPuzzle(kObjectGlug, 0, kPuzzle10002, "NIS00N01P01S01", 0, 203, 191, 1, 1000, 5, 1095237632, 4);
	objectPresentationSetAnimationOnPuzzle(kObjectGlug, 0, 0, 10000);
	objectAdd(kObject10110, "", "", 1);
	objectAddPuzzleAccessibility(kObject10110, kPuzzle10100, Common::Rect(236, 79, 289, 118), 0, 52, 1);
	objectAddPuzzleAccessibility(kObject10110, kPuzzle10100, Common::Rect(171, 124, 229, 159), 0, 52, 2);
	objectAddPuzzleAccessibility(kObject10110, kPuzzle10100, Common::Rect(272, 126, 342, 165), 0, 52, 3);
	objectAddPuzzleAccessibility(kObject10110, kPuzzle10100, Common::Rect(195, 169, 259, 213), 0, 52, 4);
	objectAddPuzzleAccessibility(kObject10110, kPuzzle10100, Common::Rect(107, 204, 188, 285), 0, 52, 5);
	objectAdd(kObject10100, "Console", "", 1);
	objectAddPuzzleAccessibility(kObject10100, kPuzzle10100, Common::Rect(0, 16, 640, 66), 1, 53, 0);
	objectAddPuzzleAccessibility(kObject10100, kPuzzle10100, Common::Rect(0, 414, 640, 464), 1, 53, 0);
	objectAddPuzzleAccessibility(kObject10100, kPuzzle10100, Common::Rect(0, 66, 50, 414), 1, 53, 0);
	objectAddPuzzleAccessibility(kObject10100, kPuzzle10100, Common::Rect(610, 66, 640, 414), 1, 53, 0);
	objectAddRotationAccessibility(kObject10100, 10101, Common::Rect(3406, 26, 3541, 155), 1, 52, 1);
	objectAddPresentation(kObject10100);
	objectPresentationAddImageToPuzzle(kObject10100, 0, kPuzzle10100, "NIS01N01P01L01.0001.bmp", 101, 68, 1, 1, 1000);
	objectAddPresentation(kObject10100);
	objectPresentationAddImageToPuzzle(kObject10100, 1, kPuzzle10100, "NIS01N01P01L02.0001.bmp", 80, 63, 1, 1, 1000);
	objectAddPresentation(kObject10100);
	objectPresentationAddImageToPuzzle(kObject10100, 2, kPuzzle10100, "NIS01N01P01L03.0001.bmp", 116, 159, 1, 1, 1000);
	objectAdd(kObject10101, "CCButton1", "", 2);
	objectAddRotationAccessibility(kObject10101, 10101, Common::Rect(3208, 109, 3322, 184), 1, 52, 1);
	objectAddPuzzleAccessibility(kObject10101, kPuzzle10100, Common::Rect(359, 265, 440, 304), 0, 52, 0);
	objectAddPuzzleAccessibility(kObject10101, kPuzzle10100, Common::Rect(359, 265, 440, 304), 0, 52, 1);
	objectAddPresentation(kObject10101);
	objectPresentationAddImageToPuzzle(kObject10101, 0, kPuzzle10100, "NIS01N01P01S03.0001.bmp", 361, 259, 1, 1, 1000);
	objectAddPresentation(kObject10101);
	objectPresentationAddAnimationToPuzzle(kObject10101, 1, kPuzzle10100, "NIS01N01P01S01", 0, 85, 68, 1, 1000, 36, 1095237632, 6);
	objectPresentationSetAnimationOnPuzzle(kObject10101, 1, 0, 10101);
	objectAddPresentation(kObject10101);
	objectPresentationAddAnimationToPuzzle(kObject10101, 2, kPuzzle10100, "NIS01N01P01L02S01", 0, 80, 63, 1, 1000, 36, 1095237632, 10);
	objectPresentationSetAnimationOnPuzzle(kObject10101, 2, 0, 10103);
	objectAddPresentation(kObject10101);
	objectPresentationAddAnimationToPuzzle(kObject10101, 3, kPuzzle10100, "NIS01N01P01S01", 0, 85, 68, 1, 1000, 36, 1095237632, 10);
	objectPresentationSetAnimationOnPuzzle(kObject10101, 3, 0, 10100);
	objectAddPresentation(kObject10101);
	objectPresentationAddAnimationToPuzzle(kObject10101, 4, kPuzzle10100, "NIS01N01P01L02S01", 0, 80, 63, 1, 1000, 36, 1095237632, 6);
	objectPresentationSetAnimationOnPuzzle(kObject10101, 4, 0, 10102);
	objectAddPresentation(kObject10101);
	objectPresentationAddImageToPuzzle(kObject10101, 5, kPuzzle10100, "NIS01N01P01S03.0003.bmp", 361, 259, 1, 1, 1000);
	objectAddPresentation(kObject10101);
	objectPresentationAddImageToRotation(kObject10101, 6, 10101, 0);
	objectPresentationAddImageToRotation(kObject10101, 6, 10102, 0);
	objectAdd(kObject10102, "CCButton2", "", 2);
	objectAddRotationAccessibility(kObject10102, 10101, Common::Rect(3250, 195, 3357, 245), 1, 52, 1);
	objectAddPuzzleAccessibility(kObject10102, kPuzzle10100, Common::Rect(382, 308, 472, 352), 1, 52, 0);
	objectAddPuzzleAccessibility(kObject10102, kPuzzle10100, Common::Rect(382, 308, 472, 352), 1, 52, 1);
	objectAddPresentation(kObject10102);
	objectPresentationAddImageToPuzzle(kObject10102, 0, kPuzzle10100, "NIS01N01P01S03.0004.bmp", 385, 299, 1, 1, 1000);
	objectAddPresentation(kObject10102);
	objectPresentationAddAnimationToPuzzle(kObject10102, 1, kPuzzle10100, "NIS01N01P01S02", 0, 40, 65, 1, 1000, 46, 1095237632, 6);
	objectPresentationSetAnimationOnPuzzle(kObject10102, 1, 0, 10105);
	objectAddPresentation(kObject10102);
	objectPresentationAddAnimationToPuzzle(kObject10102, 2, kPuzzle10100, "NIS01N01P01S02", 0, 40, 65, 1, 1000, 23, 1095237632, 6);
	objectPresentationSetAnimationOnPuzzle(kObject10102, 2, 0, 10104);
	objectAddPresentation(kObject10102);
	objectPresentationAddAnimationToPuzzle(kObject10102, 3, kPuzzle10100, "NIS01N01P01S02", 0, 40, 65, 1, 1000, 23, 1095237632, 10);
	objectPresentationSetAnimationOnPuzzle(kObject10102, 3, 0, 10106);
	objectAddPresentation(kObject10102);
	objectPresentationAddImageToPuzzle(kObject10102, 4, kPuzzle10100, "NIS01N01P01S03.0002.bmp", 385, 299, 1, 1, 1000);
	objectAdd(kObject10103, "CCHandle", "NI_HandSel", 4);
	objectAddPuzzleAccessibility(kObject10103, kPuzzle10100, Common::Rect(497, 192, 543, 249), 1, 10000, 1);
	objectSetActiveDrawCursor(kObject10103, 15, 15, 0, 3, 0, 0, 3);
	objectSetPassiveDrawCursor(kObject10103, 15, 15, 0, 3, 0, 0, 3);

	for (uint32 i = 0; i < 14; i++) {
		objectAddPresentation(kObject10103);
		objectPresentationAddImageToPuzzle(kObject10103, i, kPuzzle10100, Common::String::format("NIS01N01P02S01.%04d.bmp", i + 1), 504, 194, 1, 1, 1000);
	}

	objectAdd(kObject10104, "CCTileHold", "", 1);
	objectAddPuzzleAccessibility(kObject10104, kPuzzle10100, Common::Rect(60, 175, 187, 320), 0, 52, 0);
	objectAdd(kObject10109, "CCMosaicBtns", "", 3);
	objectAddPuzzleAccessibility(kObject10109, kPuzzle10100, Common::Rect(267, 257, 284, 272), 0, 52, 0);
	objectAddPuzzleAccessibility(kObject10109, kPuzzle10100, Common::Rect(265, 205, 282, 220), 0, 52, 1);
	objectAddPuzzleAccessibility(kObject10109, kPuzzle10100, Common::Rect(244, 220, 261, 235), 0, 52, 2);
	objectAddPuzzleAccessibility(kObject10109, kPuzzle10100, Common::Rect(278, 220, 295, 235), 0, 52, 3);
	objectAddPuzzleAccessibility(kObject10109, kPuzzle10100, Common::Rect(256, 235, 273, 250), 0, 52, 4);
	objectAddPuzzleAccessibility(kObject10109, kPuzzle10100, Common::Rect(230, 257, 247, 272), 0, 52, 5);
	objectAddPresentation(kObject10109);
	objectPresentationAddImageToPuzzle(kObject10109, 0, kPuzzle10100, "NIS01N01P01L02S02.0006.bmp", 221, 196, 1, 1, 1000);
	objectAddPresentation(kObject10109);
	objectPresentationAddImageToPuzzle(kObject10109, 1, kPuzzle10100, "NIS01N01P01L02S02.0001.bmp", 221, 196, 1, 1, 1000);
	objectAddPresentation(kObject10109);
	objectPresentationAddImageToPuzzle(kObject10109, 2, kPuzzle10100, "NIS01N01P01L02S02.0002.bmp", 221, 196, 1, 1, 1000);
	objectAddPresentation(kObject10109);
	objectPresentationAddImageToPuzzle(kObject10109, 3, kPuzzle10100, "NIS01N01P01L02S02.0003.bmp", 221, 196, 1, 1, 1000);
	objectAddPresentation(kObject10109);
	objectPresentationAddImageToPuzzle(kObject10109, 4, kPuzzle10100, "NIS01N01P01L02S02.0004.bmp", 221, 196, 1, 1, 1000);
	objectAddPresentation(kObject10109);
	objectPresentationAddImageToPuzzle(kObject10109, 5, kPuzzle10100, "NIS01N01P01L02S02.0005.bmp", 221, 196, 1, 1, 1000);
	objectAdd(kObject10105, "CCHolo", "", 1);
	objectAddPuzzleAccessibility(kObject10105, kPuzzle10100, Common::Rect(77, 132, 340, 298), 0, 52, 0);
	objectAddPuzzleAccessibility(kObject10105, kPuzzle10100, Common::Rect(140, 170, 430, 380), 0, 52, 0);
	objectAdd(kObject10106, "CCHoloCross", "NI_HandSel", 4);
	objectAddPuzzleAccessibility(kObject10106, kPuzzle10102, Common::Rect(203, 236, 283, 316), 1, 10000, 0);
	objectSetActiveDrawCursor(kObject10106, 15, 15, 0, 3, 0, 0, 3);
	objectSetPassiveDrawCursor(kObject10106, 15, 15, 0, 3, 0, 0, 3);

	for (uint32 i = 0; i < 20; i++) {
		objectAddPresentation(kObject10106);
		objectPresentationAddImageToPuzzle(kObject10106, i, kPuzzle10102, Common::String::format("NIS01N01P03L01S01.%04d.bmp", i + 1), 213, 254, 1, 1, 1000);
	}

	for (uint32 i = 0; i < 20; i++) {
		objectAddPresentation(kObject10106);
		objectPresentationAddImageToPuzzle(kObject10106, i, kPuzzle10102, Common::String::format("NIS01N01P03L01S01_a.%04d.bmp", i + 1), 213, 254, 1, 1, 1000);
	}

	objectAddPresentation(kObject10106);
	objectPresentationAddImageToPuzzle(kObject10106, 38, kPuzzle10102, "NIS01N01P03L02.0001.bmp", 267, 283, 1, 1, 1000);
	objectAddPresentation(kObject10106);
	objectPresentationAddImageToPuzzle(kObject10106, 39, kPuzzle10102, "NIS01N01P03L01.0001.bmp", 56, 139, 1, 1, 1000);
	objectAdd(kObject10107, "CCHoloDam1", "NI_HandSel", 4);
	objectAddPuzzleAccessibility(kObject10107, kPuzzle10102, Common::Rect(295, 255, 345, 315), 1, 10000, 0);
	objectAddPuzzleAccessibility(kObject10107, kPuzzle10102, Common::Rect(295, 315, 345, 375), 0, 10000, 1);
	objectSetActiveDrawCursor(kObject10107, 15, 15, 0, 3, 0, 0, 3);
	objectSetPassiveDrawCursor(kObject10107, 15, 15, 0, 3, 0, 0, 3);

	for (uint32 i = 0; i < 15; i++) {
		objectAddPresentation(kObject10107);
		objectPresentationAddImageToPuzzle(kObject10107, i, kPuzzle10102, Common::String::format("NIS01N01P03L01S02.%04d.bmp", i + 1), 302, 264, 1, 1, 1000);
	}

	objectAdd(kObject10200, "Speaker", "", 1);
	objectAddPuzzleAccessibility(kObject10200, kPuzzle10200, Common::Rect(150, 110, 470, 200), 1, 52, 0);
	objectAddPresentation(kObject10200);
	objectPresentationAddAnimationToPuzzle(kObject10200, 0, kPuzzle10202, "NIS02N01P03S01", 0, 229, 188, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject10200);
	objectPresentationAddAnimationToPuzzle(kObject10200, 1, kPuzzle10203, "NIS02N01P02S01", 0, 197, 44, 1, 1000, 10, 1095237632, 4);
	objectPresentationSetAnimationOnPuzzle(kObject10200, 1, 0, 10200);
	objectAddPresentation(kObject10200);
	objectPresentationAddAnimationToPuzzle(kObject10200, 2, kPuzzle10204, "NIS02N01P02S02", 0, 311, 166, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject10200);
	objectPresentationAddAnimationToPuzzle(kObject10200, 3, kPuzzle10205, "NIS02N01P02S03", 0, 97, 185, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObject10201, "Handle", "NI_HandSel", 4);
	objectAddPuzzleAccessibility(kObject10201, kPuzzle10200, Common::Rect(300, 210, 430, 270), 0, 10000, 0);
	objectSetActiveDrawCursor(kObject10201, 15, 15, 0, 3, 0, 0, 3);
	objectSetPassiveDrawCursor(kObject10201, 15, 15, 0, 3, 0, 0, 3);

	for (uint32 i = 0; i < 15; i++) {
		objectAddPresentation(kObject10201);
		objectPresentationAddImageToPuzzle(kObject10201, i, kPuzzle10200, Common::String::format("NIS02N01P01S01.%04d.bmp", i + 1), 305, 213, 1, 1, 1000);
	}

	objectAdd(kObject10300, "Mime", "", 1);
	objectAddRotationAccessibility(kObject10300, 10301, Common::Rect(1937, -26, 2079, 88), 1, 52, 0);
	objectAddPresentation(kObject10300);
	objectPresentationAddAnimationToRotation(kObject10300, 0, 10301, 0, 25, 1092616192, 4);
	objectPresentationSetAnimationOnRotation(kObject10300, 0, 0, 10300);
	objectPresentationShow(kObject10300, 0);
	objectAddPresentation(kObject10300);
	objectPresentationAddAnimationToPuzzle(kObject10300, 1, kPuzzle10390, "NIS03N01P01S02", 0, 289, 79, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject10300);
	objectPresentationAddAnimationToPuzzle(kObject10300, 2, kPuzzle10391, "NIS03N01P01S01", 0, 260, 73, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject10300);
	objectPresentationAddAnimationToPuzzle(kObject10300, 3, kPuzzle10392, "NIS03N01P03S01", 0, 246, 183, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObjectLogeTear, "Loge's Tear", "NI_Tear", 1);
	objectAddBagAnimation(kObjectLogeTear, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectLogeTear, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectLogeTear, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectLogeTear, kPuzzle10300, Common::Rect(470, 254, 541, 328), 1, 52, 0);
	objectAddPresentation(kObjectLogeTear);
	objectPresentationAddImageToPuzzle(kObjectLogeTear, 0, kPuzzle10300, "NIS03N01P02L02.0001.bmp", 466, 249, 1, 1, 1000);
	objectAddPresentation(kObjectLogeTear);
	objectPresentationAddAnimationToPuzzle(kObjectLogeTear, 1, kPuzzle10300, "NIS03N01P02S01", 0, 466, 250, 1, 1000, 21, 1095237632, 4);
	objectPresentationShow(kObjectLogeTear, 1);
	objectAdd(kObjectGold, "Rhine Gold", "RhineGold", 1);
	objectAdd(kObjectTile, "Tile", "NI_Mosaic", 1);
	objectAddBagAnimation(kObjectTile, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectTile, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectTile, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectTile, kPuzzle10300, Common::Rect(247, 24, 337, 126), 1, 52, 0);
	objectAddPresentation(kObjectTile);
	objectPresentationAddImageToPuzzle(kObjectTile, 0, kPuzzle10300, "NIS03N01P02L01.0001.bmp", 251, 45, 1, 1, 1000);
	objectAdd(kObjectMagicFrog, "Frog", "NI_Frog", 1);
	objectAddBagAnimation(kObjectMagicFrog, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectMagicFrog, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectMagicFrog, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectMagicFrog, kPuzzle10300, Common::Rect(300, 230, 430, 430), 1, 52, 0);
	objectAdd(kObject10420, "LValve", "NI_HandSel", 4);
	objectAddPuzzleAccessibility(kObject10420, kPuzzle10400, Common::Rect(287, 223, 344, 252), 1, 10000, 0);
	objectAddPuzzleAccessibility(kObject10420, kPuzzle10400, Common::Rect(312, 274, 368, 300), 0, 10000, 1);
	objectSetActiveDrawCursor(kObject10420, 15, 15, 0, 3, 0, 0, 3);
	objectSetPassiveDrawCursor(kObject10420, 15, 15, 0, 3, 0, 0, 3);
	objectAddPresentation(kObject10420);
	objectPresentationAddImageToPuzzle(kObject10420, 0, kPuzzle10400, "NIS04N02P01L01S01.0012.bmp", 257, 211, 1, 1, 1000);

	for (uint32 i = 1; i < 13; i++) {
		objectAddPresentation(kObject10420);
		objectPresentationAddImageToPuzzle(kObject10420, i, kPuzzle10400, Common::String::format("NIS04N02P01L01S01.%04d.bmp", i), 257, 211, 1, 1, 1000);
	}

	objectAdd(kObject10421, "RValve", "NI_HandSel", 4);
	objectAddPuzzleAccessibility(kObject10421, kPuzzle10401, Common::Rect(291, 221, 357, 257), 1, 10000, 0);
	objectAddPuzzleAccessibility(kObject10421, kPuzzle10401, Common::Rect(271, 268, 336, 308), 0, 10000, 1);
	objectSetActiveDrawCursor(kObject10421, 15, 15, 0, 3, 0, 0, 3);
	objectSetPassiveDrawCursor(kObject10421, 15, 15, 0, 3, 0, 0, 3);
	objectAddPresentation(kObject10421);
	objectPresentationAddImageToPuzzle(kObject10421, 0, kPuzzle10401, "NIS04N02P02L01S01.0012.bmp", 227, 204, 1, 1, 1000);

	for (uint32 i = 1; i < 13; i++) {
		objectAddPresentation(kObject10421);
		objectPresentationAddImageToPuzzle(kObject10421, i, kPuzzle10401, Common::String::format("NIS04N02P02L01S01.%04d.bmp", i), 227, 204, 1, 1, 1000);
	}

	objectAdd(kObject10430, "TearCasing", "", 3);
	objectAddPuzzleAccessibility(kObject10430, kPuzzle10411, Common::Rect(189, 189, 362, 377), 1, 52, 0);
	objectAddPuzzleAccessibility(kObject10430, kPuzzle10411, Common::Rect(224, 90, 362, 209), 0, 52, 1);
	objectAddPuzzleAccessibility(kObject10430, kPuzzle10411, Common::Rect(293, 241, 475, 462), 0, 52, 2);
	objectAddPresentation(kObject10430);
	objectPresentationAddImageToPuzzle(kObject10430, 0, kPuzzle10411, "NIS04N03P03L01.0001.bmp", 214, 173, 1, 1, 1000);
	objectAddPresentation(kObject10430);
	objectPresentationAddImageToPuzzle(kObject10430, 1, kPuzzle10411, "NIS04N03P03L02.0001.bmp", 270, 116, 1, 1, 1000);
	objectAddPresentation(kObject10430);
	objectPresentationAddAnimationToPuzzle(kObject10430, 2, kPuzzle10411, "NIS04N03P03L01S01", 0, 217, 171, 1, 1000, 27, 1095237632, 10);
	objectPresentationSetAnimationOnPuzzle(kObject10430, 2, 0, 10422);
	objectAddPresentation(kObject10430);
	objectPresentationAddAnimationToPuzzle(kObject10430, 3, kPuzzle10411, "NIS04N03P03L01S01", 0, 217, 171, 1, 1000, 27, 1095237632, 6);
	objectPresentationSetAnimationOnPuzzle(kObject10430, 3, 0, 10423);
	objectAddPresentation(kObject10430);
	objectPresentationAddAnimationToPuzzle(kObject10430, 4, kPuzzle10411, "NIS04N03P03L02S01", 0, 215, 111, 1, 1000, 26, 1095237632, 10);
	objectPresentationSetAnimationOnPuzzle(kObject10430, 4, 0, 10424);
	objectAddPresentation(kObject10430);
	objectPresentationAddAnimationToPuzzle(kObject10430, 5, kPuzzle10411, "NIS04N03P03L02S01", 0, 215, 111, 1, 1000, 26, 1095237632, 6);
	objectPresentationSetAnimationOnPuzzle(kObject10430, 5, 0, 10425);
	objectAddPresentation(kObject10430);
	objectPresentationAddAnimationToPuzzle(kObject10430, 6, kPuzzle10411, "NIS04N03P03L02S02", 0, 275, 126, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObject10440, "Door", "", 1);
	objectAddRotationAccessibility(kObject10440, 10404, Common::Rect(2923, -370, 3549, 460), 1, 53, 0);
	objectAdd(kObject10450, "Water", "", 1);
	objectAddRotationAccessibility(kObject10450, 10405, Common::Rect(2000, 400, 3600, 680), 1, 52, 0);
	objectAddRotationAccessibility(kObject10450, 10415, Common::Rect(2000, 400, 3600, 680), 1, 52, 0);
	objectAdd(kObject10460, "Water", "", 1);
	objectAddRotationAccessibility(kObject10460, 10406, Common::Rect(0, -600, 3599, 600), 1, 53, 1);
	objectAdd(kObject10432, "", "", 1);
	objectAddPresentation(kObject10432);
	objectPresentationAddAnimationToPuzzle(kObject10432, 0, kPuzzle10410, "NIS04N03P01S02", 0, 171, 285, 1, 1000, 13, 1092616192, 32);
	objectPresentationAddAnimationToPuzzle(kObject10432, 0, kPuzzle10410, "NIS04N03P01S03", 0, 244, 268, 1, 1000, 12, 1092616192, 32);
	objectPresentationAddAnimationToPuzzle(kObject10432, 0, kPuzzle10410, "NIS04N03P01S04", 0, 317, 261, 1, 1000, 7, 1092616192, 32);
	objectAddPresentation(kObject10432);
	objectPresentationAddAnimationToRotation(kObject10432, 1, 10401, 0, 25, 1095237632, 6);
	objectPresentationAddAnimationToRotation(kObject10432, 1, 10402, 0, 26, 1095237632, 6);
	objectAddPresentation(kObject10432);
	objectPresentationAddAnimationToRotation(kObject10432, 2, 10401, 1, 25, 1095237632, 6);
	objectPresentationAddAnimationToRotation(kObject10432, 2, 10402, 1, 26, 1095237632, 6);
	objectAdd(kObject10431, "Temperat", "", 1);
	objectAddPresentation(kObject10431);

	for (uint32 i = 1; i < 13; i++) {
		objectAddPresentation(kObject10431);
		objectPresentationAddImageToPuzzle(kObject10431, i, kPuzzle10410, Common::String::format("NIS04N03P01S01.%04d.bmp", i + 1), 107, 155, 1, 1, 1000);
	}

	objectAdd(kObjectDivingHelmet2, "Helmet&Frog", "NI_HelmetFrog", 1);
	objectAddBagAnimation(kObjectDivingHelmet2, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectDivingHelmet2, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectDivingHelmet2, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectDivingHelmet, "Helmet", "NI_Helmet", 1);
	objectAddBagAnimation(kObjectDivingHelmet, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectDivingHelmet, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectDivingHelmet, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectDivingHelmet, kPuzzle10501, Common::Rect(218, 249, 419, 346), 1, 52, 0);
	objectAddPresentation(kObjectDivingHelmet);
	objectPresentationAddImageToPuzzle(kObjectDivingHelmet, 0, kPuzzle10501, "NIS05N01P01L02.0001.bmp", 229, 252, 1, 1, 1000);
	objectAdd(kObjectAntiGCells, "AGCells", "RH_AntiG", 1);
	objectAddBagAnimation(kObjectAntiGCells, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectAntiGCells, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectAntiGCells, 22, 22, 0, 3, 0, 0, 3);
	objectAddRotationAccessibility(kObjectAntiGCells, 10501, Common::Rect(2921, 68, 3185, 477), 1, 52, 0);
	objectAdd(kObject10600, "UTiles", "", 1);
	objectAddPuzzleAccessibility(kObject10600, kPuzzle10600, Common::Rect(0, 420, 640, 464), 1, 55, 0);
	objectAdd(kObject10601, "UTile1", "NI_HandSel", 4);
	objectAddPuzzleAccessibility(kObject10601, kPuzzle10600, Common::Rect(215, 73, 430, 207), 1, 10000, 0);
	objectSetActiveDrawCursor(kObject10601, 15, 15, 0, 3, 0, 0, 3);
	objectSetPassiveDrawCursor(kObject10601, 15, 15, 0, 3, 0, 0, 3);

	for (uint32 i = 0; i < 49; i++) {
		objectAddPresentation(kObject10601);
		objectPresentationAddImageToPuzzle(kObject10601, i, kPuzzle10600, Common::String::format("NIS06N01P01S01.%04d.bmp", i + 1), 203, 82, 1, 1, 1000);
	}

	objectAdd(kObject10602, "UTile2", "NI_HandSel", 4);
	objectAddPuzzleAccessibility(kObject10602, kPuzzle10600, Common::Rect(222, 211, 420, 266), 1, 10000, 0);
	objectSetActiveDrawCursor(kObject10602, 15, 15, 0, 3, 0, 0, 3);
	objectSetPassiveDrawCursor(kObject10602, 15, 15, 0, 3, 0, 0, 3);

	for (uint32 i = 0; i < 49; i++) {
		objectAddPresentation(kObject10602);
		objectPresentationAddImageToPuzzle(kObject10602, i, kPuzzle10600, Common::String::format("NIS06N01P01S02.%04d.bmp", i + 1), 245, 211, 1, 1, 1000);
	}

	objectAdd(kObject10603, "UTile3", "NI_HandSel", 4);
	objectAddPuzzleAccessibility(kObject10603, kPuzzle10600, Common::Rect(216, 272, 424, 398), 1, 10000, 0);
	objectSetActiveDrawCursor(kObject10603, 15, 15, 0, 3, 0, 0, 3);
	objectSetPassiveDrawCursor(kObject10603, 15, 15, 0, 3, 0, 0, 3);

	for (uint32 i = 0; i < 49; i++) {
		objectAddPresentation(kObject10603);
		objectPresentationAddImageToPuzzle(kObject10603, i, kPuzzle10600, Common::String::format("NIS06N01P01S03.%04d.bmp", i + 1), 199, 272, 1, 1, 1000);
	}

	soundAdd(14001, 1, "1583.was", _configuration.backgroundMusic.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(14002, 1, "1584.was", _configuration.backgroundMusic.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(14003, 1, "1585.was", _configuration.backgroundMusic.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundSetVolume(14003, 90);
	soundSetVolume(14001, 90);
	soundSetVolume(14002, 90);
	soundAdd(10101, 3, "1586.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(10300, 3, "1587.was", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(10301, 3, "1588.wav", _configuration.ambientEffect.loadFrom);
	soundAdd(10800, 3, "1589.wac", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(10801, 3, "1590.wac", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(10900, 3, "1591.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(10901, 3, "1592.was", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(10902, 3, "1593.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(10803, 3, "1594.wav", _configuration.ambientEffect.loadFrom);
	soundAdd(10804, 3, "1595.wav", _configuration.ambientEffect.loadFrom);
	soundAdd(10409, 3, "1596.was", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(10410, 3, "1597.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(10411, 3, "1598.was", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(10412, 3, "1599.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(10413, 3, "1600.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(10501, 3, "1601.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(10502, 3, "1602.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(10102, 3, "1603.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(10103, 3, "1604.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(10104, 3, "1605.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(10105, 3, "1606.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(13001, 3, "1607.wac", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(13002, 3, "1608.wac", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(13003, 3, "1609.wac", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(13004, 3, "1610.wac", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(13005, 3, "1611.wac", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(13006, 3, "1612.wac", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(13007, 3, "1613.wac", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(13008, 3, "1614.wac", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(13009, 3, "1615.wac", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(10415, 3, "1616.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(10106, 4, "1617.wav", _configuration.effect.loadFrom);
	soundAdd(10414, 4, "1618.wav", _configuration.effect.loadFrom);
	soundAdd(10401, 4, "1619.wav", _configuration.effect.loadFrom);
	soundAdd(10402, 4, "1620.wav", _configuration.effect.loadFrom);
	soundAdd(10403, 4, "1621.wav", _configuration.effect.loadFrom);
	soundAdd(10404, 4, "1616.wav", _configuration.effect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(10405, 4, "1622.wav", _configuration.effect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(10406, 4, "1623.wav", _configuration.effect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(10407, 4, "1624.wav", _configuration.effect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(10408, 4, "1625.wav", _configuration.effect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(12001, 5, "1626.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(12002, 5, "1627.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(12003, 5, "1628.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10001, 5, "1629.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10002, 5, "1630.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10003, 5, "1631.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10004, 5, "1632.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10005, 5, "1633.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10006, 5, "1634.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10007, 5, "1635.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10008, 5, "1636.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10009, 5, "1637.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10010, 5, "1638.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10011, 5, "1639.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10012, 5, "1640.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10013, 5, "1641.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10014, 5, "1642.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10015, 5, "1643.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10018, 5, "1644.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10016, 5, "1645.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10019, 5, "1646.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10017, 5, "1647.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10020, 5, "1648.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10021, 5, "1649.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10022, 5, "1650.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10023, 5, "1651.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10024, 5, "1652.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10025, 5, "1653.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10026, 5, "1654.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10027, 5, "1655.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10028, 5, "1656.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10029, 5, "1657.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10030, 5, "1658.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10031, 5, "1659.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10032, 5, "1660.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10037, 5, "1661.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10034, 5, "1662.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10035, 5, "1663.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10036, 5, "1664.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(10033, 5, "1665.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	rotationAdd3DSound(10301, 10301, 1, 1, 20, 100, 160.0, 20);
	rotationSet3DSoundOff(10301, 10301);
	rotationAdd3DSound(10301, 10300, 1, 1, 20, 100, 160.0, 5);
	rotationAdd3DSound(10003, 10300, 1, 1, 20, 95, 195.0, 10);
	puzzleAdd3DSound(kPuzzle10300, 10300, 1, 1, 2, 93, 200.0, 5);
	rotationAdd3DSound(10403, 10412, 1, 1, 20, 90, 270.0, 10);
	rotationSet3DSoundOff(10403, 10412);
	rotationAdd3DSound(10402, 10412, 1, 1, 20, 85, 270.0, 10);
	rotationSet3DSoundOff(10402, 10412);
	rotationAdd3DSound(10401, 10412, 1, 1, 20, 80, 270.0, 10);
	rotationSet3DSoundOff(10401, 10412);
	rotationAdd3DSound(10404, 10412, 1, 1, 20, 90, 270.0, 10);
	rotationSet3DSoundOff(10404, 10412);
	puzzleAdd3DSound(kPuzzle10410, 10412, 1, 1, 2, 93, 300.0, 5);
	puzzleSet3DSoundOff(kPuzzle10410, 10412);
	puzzleAdd3DSound(kPuzzle10411, 10412, 1, 1, 2, 95, 300.0, 5);
	puzzleSet3DSoundOff(kPuzzle10411, 10412);
	puzzleAdd3DSound(kPuzzle10400, 10412, 1, 1, 2, 95, 205.0, 5);
	puzzleSet3DSoundOff(kPuzzle10400, 10412);
	puzzleAdd3DSound(kPuzzle10401, 10412, 1, 1, 2, 95, 345.0, 5);
	puzzleSet3DSoundOff(kPuzzle10401, 10412);
	varDefineWord(10100, 0);
	varDefineWord(10101, 0);
	varDefineWord(10600, 12);
	varDefineWord(10601, 0);
	varDefineWord(10602, 24);
	varDefineDword(10000, 0);
	varDefineDword(10001, 0);
	varDefineByte(10432, 0);
	varDefineByte(10200, 0);
	varDefineByte(10000, 0);
	varDefineByte(10300, 0);
	varDefineByte(10301, 0);
	varDefineByte(10302, 0);
	varDefineByte(10303, 0);
	varDefineByte(10420, 0);
	varDefineByte(10421, 0);
	varDefineByte(10430, 0);
	varDefineByte(10431, 0);
	varDefineByte(10100, 0);
	varDefineByte(10101, 0);
	varDefineByte(10102, 0);
	varDefineByte(10103, 0);
	varDefineByte(10104, 0);
	varDefineByte(10105, 0);
	varDefineByte(10500, 0);
	varDefineByte(10501, 1);
	varDefineByte(10106, 0);
	varDefineByte(10107, 0);
	varDefineByte(10113, 0);
	varDefineByte(10108, 0);
	varDefineByte(10109, 0);
	varDefineByte(10110, 0);
	varDefineByte(10111, 0);
	varDefineByte(10112, 0);
}

void ApplicationRing::initZoneRH() {
	setZoneAndEnableBag(kZoneRH);

	puzzleAdd(kPuzzle22001);
	puzzleAddBackgroundImage(kPuzzle22001, "TR_NI_RH_BP01.BMP", 0, 16, 1);
	puzzleAdd(kPuzzle22002);
	puzzleAddBackgroundImage(kPuzzle22002, "TR_NI_RH_BP02.BMP", 0, 16, 1);
	puzzleAdd(kPuzzle22003);
	puzzleAddBackgroundImage(kPuzzle22003, "TR_NI_RH_BP03.BMP", 0, 16, 1);
	puzzleAdd(kPuzzle20000);
	puzzleAddBackgroundImage(kPuzzle20000, "RHS00N01P01S02.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle20011);
	puzzleAddBackgroundImage(kPuzzle20011, "RHS00N01P01L00.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle20021);
	puzzleAddBackgroundImage(kPuzzle20021, "RHS00N02P01L00.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle20031);
	puzzleAddBackgroundImage(kPuzzle20031, "RHS00N03P01L00.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle20202);
	puzzleAddBackgroundImage(kPuzzle20202, "RHS02N01P02.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle20203);
	puzzleAddBackgroundImage(kPuzzle20203, "RHS02N01P02DI01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle20204);
	puzzleAddBackgroundImage(kPuzzle20204, "RHS02N02P01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle20301);
	puzzleAddBackgroundImage(kPuzzle20301, "RHS03N01P01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle20302);
	puzzleAddBackgroundImage(kPuzzle20302, "RHS03N02P01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle20303);
	puzzleAddBackgroundImage(kPuzzle20303, "RHS03N03P01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle20401);
	puzzleAddBackgroundImage(kPuzzle20401, "RHS04N01P01L01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle20501);
	puzzleAddBackgroundImage(kPuzzle20501, "RHS05N01P01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle20502);
	puzzleAddBackgroundImage(kPuzzle20502, "RHS05N01P02.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle20503);
	puzzleAddBackgroundImage(kPuzzle20503, "RHS05N02P01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle20701);
	puzzleAddBackgroundImage(kPuzzle20701, "RHS07N01P01L01.bmp", 0, 16, 1);
	rotationAdd(20010, "RHS00N01", 0, 0);
	rotationAdd(20020, "RHS00N02", 0, 0);
	rotationAdd(20030, "RHS00N03", 0, 0);
	rotationAdd(20101, "RHS01N01", 0, 2);
	rotationSetComBufferLength(20101, 1300000);
	rotationAdd(20201, "RHS02N01", 0, 1);
	rotationAdd(20202, "RHS02N02", 0, 1);
	rotationAdd(20203, "RHS02N03", 0, 1);
	rotationAdd(20301, "RHS03N01", 0, 2);
	rotationSetComBufferLength(20301, 1300000);
	rotationAdd(20302, "RHS03N02", 0, 0);
	rotationAdd(20303, "RHS03N03", 0, 0);
	rotationAdd(20304, "RHS03N04", 0, 1);
	rotationAdd(20305, "RHS03N05", 0, 1);
	rotationAdd(20401, "RHS04N01", 0, 1);
	rotationAdd(20402, "RHS04N02", 0, 1);
	rotationAdd(20403, "RHS04N03", 0, 1);
	rotationAdd(20501, "RHS05N01", 0, 1);
	rotationAdd(20503, "RHS05N03", 0, 1);
	rotationAdd(20504, "RHS05N04", 0, 0);
	rotationAdd(20601, "RHS06N01", 0, 0);
	rotationAdd(20701, "RHS07N01", 0, 0);
	rotationSetJugOn(20701, 10.0, 1.0);
	rotationAddMovabilityToPuzzle(20010, kPuzzle20011, "1708", 274, -289, 836, 102, 1, 53, 0);
	rotationSetMovabilityToPuzzle(20010, 0, 1069547520, -1064724070, 1117703373, 0, 0);
	rotationAddMovabilityToRotation(20010, 20020, "1709", 274, -289, 836, 102, 0, 53, 1);
	rotationSetMovabilityToRotation(20010, 1, 1069547520, -1064724070, 1117703373, 0, 0, 1069547520, -1064724070, 1117703373);
	rotationAddMovabilityToPuzzle(20020, kPuzzle20021, "1713", 274, -289, 836, 102, 1, 53, 0);
	rotationSetMovabilityToPuzzle(20020, 0, 1069547520, -1064724070, 1117703373, 0, 0);
	rotationAddMovabilityToRotation(20020, 20030, "1709", 274, -289, 836, 102, 0, 53, 1);
	rotationSetMovabilityToRotation(20020, 1, 1069547520, -1064724070, 1117703373, 0, 0, 1069547520, -1064724070, 1117703373);
	rotationAddMovabilityToRotation(20020, 20010, "1709", 2074, -289, 2636, 102, 1, 53, 1);
	rotationSetMovabilityToRotation(20020, 2, 1127579648, -1064724070, 1117703373, 0, 0, 1127579648, -1064724070, 1117703373);
	rotationAddMovabilityToPuzzle(20030, kPuzzle20031, "1717", 274, -289, 836, 102, 1, 53, 0);
	rotationSetMovabilityToPuzzle(20030, 0, 1069547520, -1064724070, 1117703373, 0, 0);
	rotationAddMovabilityToRotation(20030, 20101, "1718", 0, -289, 836, 102, 0, 53, 1);
	rotationSetMovabilityToRotation(20030, 1, 1134723072, -1064724070, 1117703373, 0, 2, 1135280128, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20030, 20020, "1709", 2074, -289, 2636, 102, 1, 53, 1);
	rotationSetMovabilityToRotation(20030, 2, 1127579648, -1064724070, 1117703373, 0, 0, 1127579648, -1064724070, 1117703373);
	rotationAddMovabilityToRotation(20101, 20030, "1709", 2239, -62, 2902, 240, 1, 53, 1);
	rotationSetMovabilityToRotation(20101, 0, 1129775104, 1050253722, 1118529126, 0, 2, 1127579648, -1064724070, 1117703373);
	rotationAddMovabilityToRotation(20101, 20201, "1695", 82, -97, 362, 268, 0, 53, 1);
	rotationSetMovabilityToRotation(20101, 1, 1129775104, 1050253722, 1118529126, 0, 2, 0, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20201, 20202, "1722", 356, -162, 553, 238, 0, 53, 0);
	rotationSetMovabilityToRotation(20201, 0, 0, 1050253722, 1118529126, 0, 2, 0, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20201, 20101, "1723", 2098, -130, 2371, 221, 1, 53, 0);
	rotationSetMovabilityToRotation(20201, 1, 1127809024, 1050253722, 1118529126, 0, 0, 1127481344, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20202, 20203, "1724", 346, -88, 569, 191, 1, 53, 0);
	rotationSetMovabilityToRotation(20202, 0, 0, 1050253722, 1118529126, 0, 2, 1132920832, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20202, 20203, "1725", 346, -88, 569, 191, 0, 53, 0);
	rotationSetMovabilityToRotation(20202, 1, 0, 1050253722, 1118529126, 0, 2, 1132920832, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20202, 20201, "1726", 1921, -251, 2515, 450, 1, 53, 0);
	rotationSetMovabilityToRotation(20202, 2, 1127481344, 1050253722, 1118529126, 0, 0, 1127481344, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20203, 20202, "1727", 1218, -88, 1499, 204, 1, 53, 0);
	rotationSetMovabilityToRotation(20203, 0, 1119092736, 1050253722, 1118529126, 0, 2, 1127481344, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20203, 20202, "1728", 1218, -88, 1499, 204, 0, 53, 0);
	rotationSetMovabilityToRotation(20203, 1, 1119092736, 1050253722, 1118529126, 0, 2, 1127481344, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20203, 20301, "1707", 3300, -560, 3600, 600, 0, 53, 0);
	rotationSetMovabilityToRotation(20203, 2, 1132920832, 1050253722, 1118529126, 0, 2, 1109917696, 1102577664, 1118529126);
	rotationAddMovabilityToRotation(20203, 20301, "1707", 0, 560, 50, -600, 0, 53, 0);
	rotationSetMovabilityToRotation(20203, 3, 1132920832, 1050253722, 1118529126, 0, 2, 1109917696, 1102577664, 1118529126);
	rotationAddMovabilityToRotation(20304, 20305, "1729", 346, -88, 569, 191, 1, 53, 0);
	rotationSetMovabilityToRotation(20304, 0, 0, 1050253722, 1118529126, 0, 2, 1119092736, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20304, 20305, "1730", 346, -88, 569, 191, 0, 53, 0);
	rotationSetMovabilityToRotation(20304, 1, 0, 1050253722, 1118529126, 0, 2, 1119092736, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20304, 20303, "1731", 1933, -350, 2634, 430, 1, 53, 0);
	rotationSetMovabilityToRotation(20304, 2, 1127481344, 1050253722, 1118529126, 0, 0, 1127481344, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20305, 20304, "1732", 2009, -55, 2369, 320, 1, 53, 0);
	rotationSetMovabilityToRotation(20305, 0, 1119092736, 1050253722, 1118529126, 0, 2, 1127481344, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20305, 20304, "1733", 2009, -55, 2369, 320, 0, 53, 0);
	rotationSetMovabilityToRotation(20305, 1, 1119092736, 1050253722, 1118529126, 0, 2, 1127481344, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20305, 20401, "1734", 680, -270, 1430, 530, 0, 53, 0);
	rotationSetMovabilityToRotation(20305, 2, 1132920832, 1050253722, 1118529126, 0, 2, 1127481344, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20402, 20403, "1735", 346, -88, 569, 191, 1, 53, 0);
	rotationSetMovabilityToRotation(20402, 0, 1065353216, 1050253722, 1118529126, 0, 2, 1132920832, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20402, 20403, "1736", 346, -88, 569, 191, 0, 53, 0);
	rotationSetMovabilityToRotation(20402, 1, 1065353216, 1050253722, 1118529126, 0, 2, 1132920832, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20403, 20402, "1737", 1209, -68, 1474, 207, 1, 53, 0);
	rotationSetMovabilityToRotation(20403, 0, 1119092736, 1050253722, 1118529126, 0, 2, 1127481344, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20403, 20402, "1738", 1209, -68, 1474, 207, 0, 53, 0);
	rotationSetMovabilityToRotation(20403, 1, 1119092736, 1050253722, 1118529126, 0, 2, 1127481344, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20403, 20501, "1739", 3000, -600, 3170, 460, 0, 53, 0);
	rotationSetMovabilityToRotation(20403, 2, 1132920832, 1050253722, 1118529126, 0, 2, 1092616192, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20503, 20501, "1740", 1933, -350, 2634, 430, 1, 53, 0);
	rotationSetMovabilityToRotation(20503, 0, 1127481344, 1050253722, 1118529126, 0, 0, 1127481344, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20503, 20504, "1741", 346, -88, 569, 191, 1, 53, 0);
	rotationSetMovabilityToRotation(20503, 1, 0, 1050253722, 1118529126, 0, 2, 1127481344, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20504, 20503, "1742", 263, -186, 717, 218, 1, 53, 0);
	rotationSetMovabilityToRotation(20504, 0, 0, 1050253722, 1118529126, 0, 2, 1127481344, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20504, 20503, "1743", 263, -186, 717, 218, 0, 53, 0);
	rotationSetMovabilityToRotation(20504, 1, 0, 1050253722, 1118529126, 0, 2, 1127481344, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(20504, 20601, "1744", 2125, -151, 2355, 570, 1, 53, 0);
	rotationSetMovabilityToRotation(20504, 2, 1132920832, 1050253722, 1118529126, 0, 2, 0, 1050253722, 1118529126);
	objectAdd(kObject22000, "", "", 1);
	objectAddPresentation(kObject22000);
	objectPresentationAddAnimationToPuzzle(kObject22000, 0, kPuzzle22001, "TR_NI_RH_BP01S01", 0, 239, 208, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject22000);
	objectPresentationAddAnimationToPuzzle(kObject22000, 1, kPuzzle22002, "TR_NI_RH_BP02S01", 0, 187, 94, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject22000);
	objectPresentationAddAnimationToPuzzle(kObject22000, 2, kPuzzle22003, "TR_NI_RH_BP03S01", 0, 207, 134, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObject21003, "", "", 1);
	objectAddPresentation(kObject21003);
	objectPresentationAddImageToRotation(kObject21003, 0, 20203, 0);
	objectAddPresentation(kObject21003);
	objectPresentationAddImageToRotation(kObject21003, 1, 20305, 0);
	objectAddPresentation(kObject21003);
	objectPresentationAddImageToRotation(kObject21003, 2, 20403, 0);
	objectAdd(kObject21001, "", "", 1);
	objectAddPresentation(kObject21001);
	objectPresentationAddAnimationToPuzzle(kObject21001, 0, kPuzzle20000, "RHS00N01P01S02", 0, 215, 155, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObject20001, "", "", 1);
	objectAddPuzzleAccessibility(kObject20001, kPuzzle20011, Common::Rect(339, 22, 453, 396), 0, 52, 0);
	objectAddPuzzleAccessibility(kObject20001, kPuzzle20011, Common::Rect(0, 16, 639, 463), 0, 53, 1);
	objectAddPresentation(kObject20001);
	objectPresentationAddImageToPuzzle(kObject20001, 0, kPuzzle20011, "RHS00N01P01S01SD.0001.tga", 320, 16, 1, 3, 997);
	objectAddPresentation(kObject20001);
	objectPresentationAddAnimationToPuzzle(kObject20001, 1, kPuzzle20011, "RHS00N01P01s01sd", 0, 334, 16, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject20001);
	objectPresentationAddAnimationToPuzzle(kObject20001, 2, kPuzzle20011, "RHS00N01P01a01_a", 0, 334, 16, 1, 999, 5, 1095237632, 6);
	objectAddPresentation(kObject20001);
	objectPresentationAddAnimationToPuzzle(kObject20001, 3, kPuzzle20011, "RHS00N01P01a01_a", 0, 334, 16, 1, 999, 5, 1095237632, 10);
	objectAddPresentation(kObject20001);
	objectPresentationAddImageToPuzzle(kObject20001, 4, kPuzzle20011, "RHS00N01P01S01SD.0001.BMP", 334, 16, 1, 1, 998);
	objectAdd(kObject20002, "", "", 1);
	objectAddPuzzleAccessibility(kObject20002, kPuzzle20021, Common::Rect(204, 99, 314, 436), 0, 52, 0);
	objectAddPuzzleAccessibility(kObject20002, kPuzzle20021, Common::Rect(0, 16, 639, 463), 0, 53, 1);
	objectAddPresentation(kObject20002);
	objectPresentationAddImageToPuzzle(kObject20002, 0, kPuzzle20021, "RHS00N02P01S01SD.0001.tga", 170, 86, 1, 3, 997);
	objectAddPresentation(kObject20002);
	objectPresentationAddAnimationToPuzzle(kObject20002, 1, kPuzzle20021, "RHS00N02P01s01sd", 0, 199, 81, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject20002);
	objectPresentationAddAnimationToPuzzle(kObject20002, 2, kPuzzle20021, "RHS00N02P01a01_a", 0, 199, 81, 1, 999, 5, 1095237632, 6);
	objectAddPresentation(kObject20002);
	objectPresentationAddAnimationToPuzzle(kObject20002, 3, kPuzzle20021, "RHS00N02P01a01_a", 0, 199, 81, 1, 999, 5, 1095237632, 10);
	objectAddPresentation(kObject20002);
	objectPresentationAddImageToPuzzle(kObject20002, 4, kPuzzle20021, "RHS00N02P01S01SD.0001.BMP", 199, 81, 1, 1, 998);
	objectAdd(kObject20003, "", "", 1);
	objectAddPuzzleAccessibility(kObject20003, kPuzzle20031, Common::Rect(85, 347, 471, 420), 0, 52, 0);
	objectAddPuzzleAccessibility(kObject20003, kPuzzle20031, Common::Rect(0, 16, 639, 463), 0, 53, 1);
	objectAddPresentation(kObject20003);
	objectPresentationAddImageToPuzzle(kObject20003, 0, kPuzzle20031, "RHS00N03P01S01SD.0001.bmp", 0, 16, 1, 1, 997);
	objectAddPresentation(kObject20003);
	objectPresentationAddAnimationToPuzzle(kObject20003, 1, kPuzzle20031, "RHS00N03P01s01sd", 0, 62, 353, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject20003);
	objectPresentationAddAnimationToPuzzle(kObject20003, 2, kPuzzle20031, "RHS00N03P01a01_a", 0, 60, 351, 1, 999, 5, 1095237632, 6);
	objectAddPresentation(kObject20003);
	objectPresentationAddAnimationToPuzzle(kObject20003, 3, kPuzzle20031, "RHS00N03P01a01_a", 0, 60, 351, 1, 999, 5, 1095237632, 10);
	objectAddPresentation(kObject20003);
	objectPresentationAddImageToPuzzle(kObject20003, 4, kPuzzle20031, "RHS00N03P01S01SD1.0001.BMP", 62, 353, 1, 1, 998);
	objectAdd(kObjectKeyIndifference, "Indifference", "RH_Key1", 9);
	objectAddBagAnimation(kObjectKeyIndifference, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectKeyIndifference, 2, 43, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectKeyIndifference, 2, 43, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectKeyIndifference, kPuzzle20011, Common::Rect(288, 284, 362, 422), 0, 52, 0);
	objectAddPresentation(kObjectKeyIndifference);
	objectPresentationAddAnimationToPuzzle(kObjectKeyIndifference, 0, kPuzzle20011, "RHS00N01P01SA01", 1, 270, 286, 3, 1000, 20, 1095237632, 4);
	objectAdd(kObjectKeyMistrust, "Mistrust", "RH_Key1", 9);
	objectAddBagAnimation(kObjectKeyMistrust, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectKeyMistrust, 2, 43, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectKeyMistrust, 2, 43, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectKeyMistrust, kPuzzle20021, Common::Rect(288, 284, 362, 422), 0, 52, 0);
	objectAddPresentation(kObjectKeyMistrust);
	objectPresentationAddAnimationToPuzzle(kObjectKeyMistrust, 0, kPuzzle20021, "RHS00N01P01SA01", 1, 270, 286, 3, 1000, 20, 1095237632, 4);
	objectAdd(kObjectKeySelfishness, "Selfishness", "RH_Key1", 9);
	objectAddBagAnimation(kObjectKeySelfishness, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectKeySelfishness, 2, 43, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectKeySelfishness, 2, 43, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectKeySelfishness, kPuzzle20031, Common::Rect(288, 284, 362, 422), 0, 52, 0);
	objectAddPresentation(kObjectKeySelfishness);
	objectPresentationAddAnimationToPuzzle(kObjectKeySelfishness, 0, kPuzzle20031, "RHS00N01P01SA01", 1, 270, 286, 3, 1000, 20, 1095237632, 4);
	objectAdd(kObjectKeyDisgust, "Disgust", "RH_Key1", 9);
	objectAddBagAnimation(kObjectKeyDisgust, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectKeyDisgust, 2, 43, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectKeyDisgust, 2, 43, 0, 3, 0, 0, 3);
	objectAddRotationAccessibility(kObjectKeyDisgust, 20101, Common::Rect(3529, 151, 3600, 296), 1, 52, 0);
	objectAddRotationAccessibility(kObjectKeyDisgust, 20101, Common::Rect(0, 151, 52, 296), 1, 52, 0);
	objectAddPresentation(kObjectKeyDisgust);
	objectPresentationAddImageToRotation(kObjectKeyDisgust, 0, 20101, 0);
	objectAddPresentation(kObjectKeyDisgust);
	objectPresentationAddAnimationToRotation(kObjectKeyDisgust, 1, 20101, 1, 34, 1095237632, 4);
	objectPresentationSetAnimationOnRotation(kObjectKeyDisgust, 1, 0, 20003);
	objectPresentationShow(kObjectKeyDisgust, 0);
	objectPresentationShow(kObjectKeyDisgust, 1);
	objectAdd(kObjectRedfish, "GoldFish", "RH_GoldFish", 9);
	objectAddBagAnimation(kObjectRedfish, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectRedfish, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectRedfish, 22, 22, 0, 3, 0, 0, 3);
	objectAddRotationAccessibility(kObjectRedfish, 20201, Common::Rect(792, 141, 1140, 291), 1, 52, 1);
	objectAddPresentation(kObjectRedfish);
	objectPresentationAddAnimationToRotation(kObjectRedfish, 0, 20201, 0, 30, 1095237632, 4);
	objectPresentationShow(kObjectRedfish, 0);
	objectAdd(kObjectDolphin, "", "", 1);
	objectAddPuzzleAccessibility(kObjectDolphin, kPuzzle20202, Common::Rect(374, 291, 480, 429), 1, 52, 0);
	objectAddPuzzleAccessibility(kObjectDolphin, kPuzzle20202, Common::Rect(374, 291, 480, 429), 0, 52, 0);
	objectAddPuzzleAccessibility(kObjectDolphin, kPuzzle20202, Common::Rect(0, 420, 640, 464), 1, 55, 9);
	objectAddRotationAccessibility(kObjectDolphin, 20201, Common::Rect(3524, 108, 3600, 247), 0, 52, 2);
	objectAddRotationAccessibility(kObjectDolphin, 20201, Common::Rect(0, 108, 50, 247), 0, 52, 2);
	objectAddRotationAccessibility(kObjectDolphin, 20201, Common::Rect(3104, -322, 3600, 485), 1, 53, 1);
	objectAddPresentation(kObjectDolphin);
	objectPresentationAddAnimationToPuzzle(kObjectDolphin, 0, kPuzzle20202, "RHS02N01P02S01", 0, 175, 25, 1, 1000, 13, 1095237632, 4);
	objectAddPresentation(kObjectDolphin);
	objectPresentationAddAnimationToPuzzle(kObjectDolphin, 1, kPuzzle20202, "RHS02N01P02S04", 0, 173, 21, 1, 1000, 13, 1095237632, 4);
	objectAddPresentation(kObjectDolphin);
	objectPresentationAddAnimationToPuzzle(kObjectDolphin, 2, kPuzzle20202, "RHS02N01P02S02", 0, 178, 23, 1, 1000, 13, 1095237632, 4);
	objectAddPresentation(kObjectDolphin);
	objectPresentationAddAnimationToPuzzle(kObjectDolphin, 3, kPuzzle20203, "RHS02N01P02S03SD", 0, 274, 94, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObjectMedallion, "Necklace", "RH_Medallion", 1);
	objectAddBagAnimation(kObjectMedallion, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectMedallion, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectMedallion, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObject20204, "", "", 1);
	objectAddRotationAccessibility(kObject20204, 20202, Common::Rect(2343, 326, 2487, 408), 1, 52, 0);
	objectAddPresentation(kObject20204);
	objectPresentationAddImageToRotation(kObject20204, 0, 20202, 0);
	objectAddPresentation(kObject20204);
	objectPresentationAddAnimationToPuzzle(kObject20204, 1, kPuzzle20204, "RHS02N02P01S01", 0, 226, 186, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObject20301, "", "", 1);
	objectAddRotationAccessibility(kObject20301, 20301, Common::Rect(377, -139, 611, 108), 1, 53, 0);
	objectAddRotationAccessibility(kObject20301, 20301, Common::Rect(641, 368, 1121, 551), 1, 52, 1);
	objectAddPuzzleAccessibility(kObject20301, kPuzzle20301, Common::Rect(240, 247, 386, 426), 1, 52, 2);
	objectAddPuzzleAccessibility(kObject20301, kPuzzle20301, Common::Rect(0, 420, 640, 464), 1, 55, 9);
	objectAddPresentation(kObject20301);
	objectPresentationAddAnimationToRotation(kObject20301, 0, 20301, 0, 20, 1095237632, 4);
	objectPresentationAddAnimationToRotation(kObject20301, 0, 20301, 1, 20, 1095237632, 4);
	objectPresentationShow(kObject20301, 0);
	objectAddPresentation(kObject20301);
	objectPresentationAddAnimationToPuzzle(kObject20301, 1, kPuzzle20301, "RHS03N01P01S01SD", 0, 280, 246, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObject20302, "", "", 1);
	objectAddRotationAccessibility(kObject20302, 20302, Common::Rect(170, -169, 558, 193), 1, 53, 0);
	objectAddRotationAccessibility(kObject20302, 20302, Common::Rect(3471, 343, 3600, 566), 1, 52, 1);
	objectAddRotationAccessibility(kObject20302, 20302, Common::Rect(0, 343, 298, 566), 1, 52, 1);
	objectAddPresentation(kObject20302);
	objectAddPresentation(kObject20302);
	objectPresentationAddAnimationToPuzzle(kObject20302, 1, kPuzzle20302, "RHS03N02P01S01SD", 0, 294, 243, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObject20303, "", "", 1);
	objectAddRotationAccessibility(kObject20303, 20303, Common::Rect(511, -235, 798, 298), 0, 53, 0);
	objectAddRotationAccessibility(kObject20303, 20303, Common::Rect(1694, 350, 2116, 566), 1, 53, 1);
	objectAddPuzzleAccessibility(kObject20303, kPuzzle20303, Common::Rect(243, 161, 376, 442), 1, 52, 2);
	objectAddPresentation(kObject20303);
	objectAddPresentation(kObject20303);
	objectPresentationAddAnimationToPuzzle(kObject20303, 1, kPuzzle20303, "RHS03N03P01S01SD", 0, 230, 124, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObject20304, "", "", 1);
	objectAddRotationAccessibility(kObject20304, 20304, Common::Rect(2343, 326, 2487, 408), 1, 52, 0);
	objectAddPresentation(kObject20304);
	objectPresentationAddImageToRotation(kObject20304, 0, 20304, 0);
	objectAddPresentation(kObject20304);
	objectPresentationAddAnimationToPuzzle(kObject20304, 1, kPuzzle20204, "RHS02N02P01S01", 0, 226, 186, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObject20401, "", "", 1);
	objectAddRotationAccessibility(kObject20401, 20401, Common::Rect(1114, -160, 1564, 390), 1, 52, 0);
	objectAddPresentation(kObject20401);
	objectPresentationAddAnimationToRotation(kObject20401, 0, 20401, 0, 28, 1095237632, 4);
	objectPresentationShow(kObject20401, 0);
	objectPresentationPauseAnimation(kObject20401, 0);
	objectAdd(kObject20402, "", "", 1);
	objectAddPresentation(kObject20402);
	objectPresentationAddAnimationToPuzzle(kObject20402, 0, kPuzzle20401, "RHS04N01P01S01SD", 0, 277, 124, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject20402);
	objectPresentationAddAnimationToPuzzle(kObject20402, 1, kPuzzle20401, "RHS04N01P01S02", 0, 216, 116, 1, 1000, 25, 1095237632, 4);
	objectPresentationShow(kObject20402, 1);
	objectAdd(kObjectAntiGCells2, "Anti gravitation cells", "RH_AntiG", 1);
	objectAddBagAnimation(kObjectAntiGCells2, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectAntiGCells2, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectAntiGCells2, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectAntiGCells2, kPuzzle20401, Common::Rect(250, 121, 345, 346), 1, 52, 0);
	objectAdd(kObject20404, "", "", 1);
	objectAddRotationAccessibility(kObject20404, 20402, Common::Rect(2315, 207, 2409, 272), 1, 52, 0);
	objectAddPresentation(kObject20404);
	objectPresentationAddImageToRotation(kObject20404, 0, 20402, 0);
	objectAddPresentation(kObject20404);
	objectPresentationAddAnimationToPuzzle(kObject20404, 1, kPuzzle20204, "RHS02N02P01S01", 0, 226, 186, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObject20501, "Daughter of the Rhine", "", 1);
	objectAddRotationAccessibility(kObject20501, 20501, Common::Rect(270, 317, 592, 516), 1, 52, 0);
	objectAddRotationAccessibility(kObject20501, 20501, Common::Rect(341, -122, 553, 200), 1, 53, 1);
	objectAddRotationAccessibility(kObject20501, 20501, Common::Rect(2053, -148, 2429, 324), 0, 53, 2);
	objectAddPuzzleAccessibility(kObject20501, kPuzzle20502, Common::Rect(226, 184, 462, 458), 0, 52, 3);
	objectAddPuzzleAccessibility(kObject20501, kPuzzle20502, Common::Rect(0, 420, 640, 464), 1, 55, 9);
	objectAddPresentation(kObject20501);
	objectPresentationAddImageToRotation(kObject20501, 0, 20501, 0);
	objectPresentationShow(kObject20501, 0);
	objectAddPresentation(kObject20501);
	objectPresentationAddAnimationToPuzzle(kObject20501, 1, kPuzzle20501, "RHS05N01P01S01SF", 0, 197, 116, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject20501);
	objectPresentationAddAnimationToPuzzle(kObject20501, 2, kPuzzle20502, "RHS05N01P02S01SD", 0, 247, 161, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject20501);
	objectPresentationAddAnimationToPuzzle(kObject20501, 3, kPuzzle20503, "RHS05N02P01S01", 0, 184, 226, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObject20502, "", "", 1);
	objectAddRotationAccessibility(kObject20502, 20503, Common::Rect(2343, 326, 2487, 408), 1, 52, 0);
	objectAddPresentation(kObject20502);
	objectPresentationAddImageToRotation(kObject20502, 0, 20503, 0);
	objectAddPresentation(kObject20502);
	objectPresentationAddAnimationToPuzzle(kObject20502, 1, kPuzzle20204, "RHS02N02P01S01", 0, 226, 186, 1, 1000, 20, 1095237632, 4);
	objectAddRotationAccessibility(kObjectDivingHelmet2, 20601, Common::Rect(337, -75, 557, 195), 1, 53, 0);
	objectAdd(kObject20700, "Rhine Gold", "RH_Gold", 1);
	objectAddBagAnimation(kObject20700, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObject20700, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObject20700, 22, 22, 0, 3, 0, 0, 3);
	objectAddRotationAccessibility(kObject20700, 20701, Common::Rect(381, 61, 530, 231), 1, 52, 0);
	soundAdd(23005, 2, "1746.was", _configuration.ambientMusic.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	rotationAddAmbientSound(20010, 23005, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(20020, 23005, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(20030, 23005, 100, 0, 1, 1, 10);
	soundAdd(23013, 2, "1747.was", _configuration.ambientMusic.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	puzzleAddAmbientSound(kPuzzle20000, 23013, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle20011, 23013, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle20021, 23013, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle20031, 23013, 100, 0, 1, 1, 10);
	soundAdd(23004, 2, "1748.was", _configuration.ambientMusic.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	rotationAddAmbientSound(20101, 23004, 90, 0, 1, 1, 10);
	soundAdd(23002, 2, "1749.was", _configuration.ambientMusic.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	rotationAddAmbientSound(20201, 23002, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(20202, 23002, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(20203, 23002, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle20202, 23002, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle20203, 23002, 100, 0, 1, 1, 10);
	soundAdd(23007, 2, "1750.was", _configuration.ambientMusic.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	rotationAddAmbientSound(20301, 23007, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(20302, 23007, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(20303, 23007, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(20304, 23007, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(20305, 23007, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle20301, 23007, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle20302, 23007, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle20303, 23007, 100, 0, 1, 1, 10);
	soundAdd(23003, 2, "1751.was", _configuration.ambientMusic.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	rotationAddAmbientSound(20401, 23003, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(20402, 23003, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(20403, 23003, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle20401, 23003, 100, 0, 1, 1, 10);
	soundAdd(23006, 2, "1752.was", _configuration.ambientMusic.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	rotationAddAmbientSound(20501, 23006, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(20503, 23006, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(20504, 23006, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle20501, 23006, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle20502, 23006, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle20503, 23006, 100, 0, 1, 1, 10);
	soundAdd(23001, 2, "1753.was", _configuration.ambientMusic.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	rotationAddAmbientSound(20601, 23001, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(20701, 23001, 100, 0, 1, 1, 10);
	soundAdd(23014, 3, "1754.wac", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	rotationAdd3DSound(20101, 23014, 1, 1, 20, 100, 228.0, 15);
	rotationSet3DSoundOff(20101, 23014);
	soundAdd(23009, 3, "1755.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(23010, 3, "1756.was", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(23011, 3, "1757.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(23012, 3, "1758.wac", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(22001, 5, "1626.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(22002, 5, "1627.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(22003, 5, "1628.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(20011, 5, "1759.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(20021, 5, "1760.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(20031, 5, "1761.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(20012, 5, "1762.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(20022, 5, "1763.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(20032, 5, "1764.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(20013, 5, "1765.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(20023, 5, "1766.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(20033, 5, "1767.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(20201, 5, "1768.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(20202, 5, "1769.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(20301, 5, "1770.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(20302, 5, "1771.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(20303, 5, "1772.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(20304, 5, "1773.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(20401, 5, "1774.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(20402, 5, "1775.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(20501, 5, "1776.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(20502, 5, "1777.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(20503, 5, "1778.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(20504, 5, "1779.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	varDefineByte(21000, 0);
	varDefineByte(21001, 0);
	varDefineByte(20200, 1);
	varDefineByte(20201, 1);
	varDefineByte(20202, 0);
	varDefineByte(20301, 0);
	varDefineByte(20500, 0);
	varDefineByte(20009, 0);
	varDefineWord(20000, 20000);
}

void ApplicationRing::initZoneFO() {
	setZoneAndEnableBag(kZoneFO);

	puzzleAdd(kPuzzle35001);
	puzzleAddBackgroundImage(kPuzzle35001, "FOS03N02P01S00.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle35002);
	puzzleAddBackgroundImage(kPuzzle35002, "FOS08N01P01.BMP", 0, 16, 1);
	puzzleAdd(kPuzzle35003);
	puzzleAddBackgroundImage(kPuzzle35003, "FOS06N01P06.BMP", 0, 16, 1);
	puzzleAdd(kPuzzle35004);
	puzzleAddBackgroundImage(kPuzzle35004, "FOS06N01P01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle35005);
	puzzleAddBackgroundImage(kPuzzle35005, "FOS06N01P05.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle35006);
	puzzleAddBackgroundImage(kPuzzle35006, "FOS06N01P02.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle35007);
	puzzleAddBackgroundImage(kPuzzle35007, "FOS06N01P03.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle35008);
	puzzleAddBackgroundImage(kPuzzle35008, "FOS06N01P04.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle35009);
	puzzleAddBackgroundImage(kPuzzle35009, "FOS06N02P01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle35010);
	puzzleAddBackgroundImage(kPuzzle35010, "FOS07N03P01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle35011);
	puzzleAddBackgroundImage(kPuzzle35011, "FOS03N01P01.BMP", 0, 16, 1);
	puzzleAdd(kPuzzle35019);
	puzzleAddBackgroundImage(kPuzzle35019, "FOS00N04P01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle35020);
	puzzleAddBackgroundImage(kPuzzle35020, "FOS07N04P01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle35100);
	puzzleAddBackgroundImage(kPuzzle35100, "FOS00N03P02.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle35101);
	puzzleAddBackgroundImage(kPuzzle35101, "FOS00N03P02L02.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle35103);
	puzzleAddBackgroundImage(kPuzzle35103, "FOS01N01P07.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle35104);
	puzzleAddBackgroundImage(kPuzzle35104, "FOS01N01P03.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle35105);
	puzzleAddBackgroundImage(kPuzzle35105, "FOS01N01P04.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle35110);
	puzzleAddBackgroundImage(kPuzzle35110, "FOS01N01P09.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle35106);
	puzzleAddBackgroundImage(kPuzzle35106, "FOS01N01P05.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle35107);
	puzzleAddBackgroundImage(kPuzzle35107, "FOS01N01P06.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle35108);
	puzzleAddBackgroundImage(kPuzzle35108, "FOS01N01P10.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle35111);
	puzzleAddBackgroundImage(kPuzzle35111, "FOS01N01P02.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle35109);
	puzzleAddBackgroundImage(kPuzzle35109, "FOS01N01P11.bmp", 0, 16, 1);
	rotationAdd(30001, "FOS00N01", 0, 1);
	rotationAdd(30002, "FOS00N02", 0, 0);
	rotationAdd(30003, "FOS00N03", 0, 1);
	rotationSetComBufferLength(30003, 1400000);
	rotationAdd(30004, "FOS00N04", 0, 0);
	rotationAdd(30005, "FOS00N05", 0, 0);
	rotationAdd(30006, "FOS00N06", 0, 0);
	rotationAdd(30008, "FOS00N08", 0, 1);
	rotationAdd(30009, "FOS00N09", 0, 1);
	rotationAdd(30010, "FOS00N10", 0, 1);
	rotationAdd(30011, "FOS00N11", 0, 3);
	rotationSetComBufferLength(30011, 1500000);
	rotationAdd(30012, "FOS00N12", 0, 0);
	rotationAdd(30101, "FOS01N01", 0, 4);
	rotationSetComBufferLength(30101, 2000000);
	rotationAdd(30301, "FOS03N01", 0, 1);
	rotationAdd(30302, "FOS03N02", 0, 0);
	rotationAdd(30303, "FOS03N03", 0, 0);
	rotationAdd(30401, "FOS04N01", 0, 0);
	rotationAdd(30402, "FOS04N02", 0, 1);
	rotationAdd(30501, "FOS05N01", 0, 3);
	rotationAdd(30601, "FOS06N01", 0, 1);
	rotationAdd(30602, "FOS06N02", 0, 1);
	rotationSetComBufferLength(30602, 1750000);
	rotationAdd(30701, "FOS07N01", 0, 0);
	rotationAdd(30702, "FOS07N02", 0, 1);
	rotationAdd(30703, "FOS07N03", 0, 1);
	rotationAdd(30704, "FOS07N04", 0, 0);
	rotationAdd(30801, "FOS08N01", 0, 0);
	rotationAddMovabilityToRotation(30001, 30003, "1219", 2787, -143, 3113, 169, 1, 53, 0);
	rotationSetMovabilityToRotation(30001, 0, 1131872256, 0, 1118476698, 0, 2, 1131872256, 0, 1118476698);
	rotationAddMovabilityToRotation(30001, 30004, "1219", 3406, -125, 3599, 73, 1, 53, 0);
	rotationSetMovabilityToRotation(30001, 1, 1134297088, 0, 1118476698, 0, 2, 1134297088, 0, 1118476698);
	rotationAddMovabilityToRotation(30001, 30004, "1219", 0, -125, 72, 73, 1, 53, 0);
	rotationSetMovabilityToRotation(30001, 2, 1134297088, 0, 1118476698, 0, 2, 1134297088, 0, 1118476698);
	rotationAddMovabilityToRotation(30002, 30003, "1220", 272, -81, 437, 118, 1, 53, 0);
	rotationSetMovabilityToRotation(30002, 0, 1135738880, 0, 1118476698, 0, 2, 1135738880, 0, 1118476698);
	rotationAddMovabilityToRotation(30002, 30004, "1220", 89, -104, 254, 45, 1, 53, 0);
	rotationSetMovabilityToRotation(30002, 1, 1135149056, 0, 1118476698, 0, 2, 1135149056, 0, 1118476698);
	rotationAddMovabilityToRotation(30003, 30001, "1221", 954, -129, 1320, 190, 1, 53, 0);
	rotationSetMovabilityToRotation(30003, 0, 1117126656, 0, 1118476698, 0, 2, 1117126656, 0, 1118476698);
	rotationAddMovabilityToRotation(30003, 30002, "1222", 2002, -97, 2364, 156, 1, 53, 0);
	rotationSetMovabilityToRotation(30003, 1, 1126629376, 0, 1118476698, 0, 2, 1126629376, 0, 1118476698);
	rotationAddMovabilityToRotation(30003, 30004, "1223", 45, -104, 261, 136, 1, 53, 0);
	rotationSetMovabilityToRotation(30003, 2, 1134886912, 0, 1118476698, 0, 2, 1134886912, 0, 1118476698);
	rotationAddMovabilityToRotation(30004, 30001, "1224", 1620, -75, 1773, 52, 1, 53, 0);
	rotationSetMovabilityToRotation(30004, 0, 1124073472, 0, 1118476698, 0, 2, 1124073472, 0, 1118476698);
	rotationAddMovabilityToRotation(30004, 30002, "1224", 1928, -48, 2037, 38, 1, 53, 0);
	rotationSetMovabilityToRotation(30004, 1, 1125449728, 0, 1118476698, 0, 2, 1125449728, 0, 1118476698);
	rotationAddMovabilityToRotation(30004, 30003, "1224", 1791, -54, 1900, 73, 1, 53, 0);
	rotationSetMovabilityToRotation(30004, 2, 1124728832, 0, 1118476698, 0, 2, 1124728832, 0, 1118476698);
	rotationAddMovabilityToRotation(30004, 30005, "1225", 40, -115, 274, 146, 1, 53, 0);
	rotationSetMovabilityToRotation(30004, 3, 1134919680, 0, 1118476698, 0, 2, 1134919680, 0, 1118476698);
	rotationAddMovabilityToRotation(30004, 30009, "1226", 402, -116, 601, 75, 1, 53, 0);
	rotationSetMovabilityToRotation(30004, 4, 0, 0, 1118476698, 0, 2, 0, 0, 1118476698);
	rotationAddMovabilityToPuzzle(30004, kPuzzle35019, "", 1033, -116, 1233, 120, 1, 52, 0);
	rotationSetMovabilityToPuzzle(30004, 5, 1116471296, 0, 1118476698, 0, 2);
	rotationAddMovabilityToRotation(30005, 30004, "1227", 1587, -134, 1829, 87, 1, 53, 0);
	rotationSetMovabilityToRotation(30005, 0, 1123418112, 0, 1118476698, 0, 2, 1123418112, 0, 1118476698);
	rotationAddMovabilityToRotation(30005, 30006, "1228", 3311, -127, 3513, 113, 1, 53, 0);
	rotationSetMovabilityToRotation(30005, 1, 1133641728, 0, 1118476698, 0, 2, 1135738880, -1049624576, 1118476698);
	rotationAddMovabilityToRotation(30006, 30005, "1229", 1817, -179, 2149, 144, 1, 53, 0);
	rotationSetMovabilityToRotation(30006, 0, 1125646336, 0, 1118476698, 0, 2, 1125646336, 0, 1118476698);
	rotationAddMovabilityToRotation(30006, 30008, "1230", 249, -385, 580, 26, 1, 53, 0);
	rotationSetMovabilityToRotation(30006, 1, 1135738880, -1049624576, 1118476698, 0, 2, 1135738880, 0, 1118476698);
	rotationAddMovabilityToRotation(30008, 30006, "1231", 2037, 130, 2455, 362, 1, 53, 0);
	rotationSetMovabilityToRotation(30008, 0, 1127481344, 1097859072, 1118476698, 0, 2, 1127481344, 0, 1118476698);
	rotationAddMovabilityToRotation(30008, 30301, "1232", 358, -43, 567, 113, 1, 53, 0);
	rotationSetMovabilityToRotation(30008, 1, 0, 0, 1118476698, 0, 2, 0, 0, 1118476698);
	rotationAddMovabilityToRotation(30009, 30004, "1233", 2536, -68, 2698, 87, 1, 53, 0);
	rotationSetMovabilityToRotation(30009, 0, 1129840640, 0, 1118476698, 0, 2, 1129840640, 0, 1118476698);
	rotationAddMovabilityToRotation(30009, 30010, "1234", 924, -136, 1121, 123, 1, 53, 0);
	rotationSetMovabilityToRotation(30009, 1, 1114112000, 0, 1118476698, 0, 2, 1114112000, 0, 1118476698);
	rotationAddMovabilityToRotation(30009, 30005, "1233", 2726, -88, 2940, 90, 1, 53, 0);
	rotationSetMovabilityToRotation(30009, 2, 1133903872, 0, 1118476698, 0, 2, 1114112000, 0, 1118476698);
	rotationAddMovabilityToRotation(30010, 30009, "1235", 2583, -137, 2845, 160, 1, 53, 0);
	rotationSetMovabilityToRotation(30010, 0, 1130758144, 0, 1118476698, 0, 2, 1130758144, 0, 1118476698);
	rotationAddMovabilityToRotation(30010, 30011, "1236", 1469, -47, 1668, 134, 1, 53, 0);
	rotationSetMovabilityToRotation(30010, 1, 1121189888, 0, 1118476698, 0, 2, 1121189888, 0, 1118476698);
	rotationAddMovabilityToRotation(30010, 30012, "1237", 573, -52, 703, 108, 1, 53, 0);
	rotationSetMovabilityToRotation(30010, 2, 1101529088, 0, 1118476698, 0, 2, 1101529088, 0, 1118476698);
	rotationAddMovabilityToRotation(30011, 30010, "1238", 3107, -45, 3309, 163, 1, 53, 0);
	rotationSetMovabilityToRotation(30011, 0, 1132756992, 0, 1118476698, 0, 2, 1132756992, 0, 1118476698);
	rotationAddMovabilityToRotation(30011, 30401, "1239", 3517, 195, 3600, 418, 1, 53, 0);
	rotationSetMovabilityToRotation(30011, 1, 1135345664, 1097859072, 1118476698, 0, 2, 1134886912, 0, 1118476698);
	rotationAddMovabilityToRotation(30011, 30401, "1239", 0, 195, 769, 418, 1, 53, 0);
	rotationSetMovabilityToRotation(30011, 2, 1135345664, 1097859072, 1118476698, 0, 2, 1134886912, 0, 1118476698);
	rotationAddMovabilityToRotation(30012, 30010, "1240", 2290, -64, 2599, 111, 1, 53, 0);
	rotationSetMovabilityToRotation(30012, 0, 1129250816, 0, 1118476698, 0, 2, 1129250816, 0, 1118476698);
	rotationAddMovabilityToRotation(30012, 30501, "1241", 543, -83, 768, 329, 1, 53, 0);
	rotationSetMovabilityToRotation(30012, 1, 1101529088, 1086324736, 1118476698, 0, 2, 1101529088, 0, 1118476698);
	rotationAddMovabilityToRotation(30101, 30003, "1242", 3594, -76, 3599, 423, 1, 53, 0);
	rotationSetMovabilityToRotation(30101, 0, 1134788608, 1097859072, 1118476698, 0, 2, 0, 0, 1118476698);
	rotationAddMovabilityToRotation(30101, 30003, "1242", 0, -76, 469, 423, 1, 53, 0);
	rotationSetMovabilityToRotation(30101, 1, 1134788608, 1097859072, 1118476698, 0, 2, 0, 0, 1118476698);
	rotationAddMovabilityToRotation(30301, 30008, "1243", 1979, -71, 2418, 214, 1, 53, 0);
	rotationSetMovabilityToRotation(30301, 0, 1127481344, 1097859072, 1118476698, 0, 2, 1127481344, 1097859072, 1118476698);
	rotationAddMovabilityToPuzzle(30301, kPuzzle35011, "", 1907, -127, 1965, 13, 1, 52, 0);
	rotationSetMovabilityToPuzzle(30301, 1, 1125515264, 1077936128, 1118476698, 0, 2);
	rotationAddMovabilityToPuzzle(30303, kPuzzle35001, "", 2023, 62, 2255, 252, 1, 52, 0);
	rotationSetMovabilityToPuzzle(30303, 0, 1126825984, 1097859072, 1118476698, 0, 2);
	rotationAddMovabilityToRotation(30401, 30402, "1244", 442, -134, 782, 153, 1, 53, 0);
	rotationSetMovabilityToRotation(30401, 0, 1135673344, 0, 1118476698, 0, 2, 1132625920, 0, 1118476698);
	rotationAddMovabilityToRotation(30401, 30011, "1245", 2074, 141, 2808, 345, 1, 53, 0);
	rotationSetMovabilityToRotation(30401, 1, 1128792064, 1097859072, 1118476698, 0, 2, 1128136704, 0, 1118476698);
	rotationAddMovabilityToRotation(30401, 30801, "1246", 1014, -104, 1239, 41, 1, 53, 0);
	rotationSetMovabilityToRotation(30401, 2, 1116471296, 0, 1118476698, 0, 2, 1116864512, 0, 1118476698);
	rotationAddMovabilityToRotation(30402, 30401, "1247", 1745, -59, 2100, 104, 1, 53, 0);
	rotationSetMovabilityToRotation(30402, 0, 1126825984, 0, 1118476698, 0, 2, 1127874560, 0, 1118476698);
	rotationAddMovabilityToRotation(30402, 30801, "1248", 1318, -92, 1504, 17, 1, 53, 0);
	rotationSetMovabilityToRotation(30402, 1, 1119354880, 0, 1118476698, 0, 2, 1117782016, 0, 1118476698);
	rotationAddMovabilityToRotation(30402, 30501, "1249", 2967, -102, 3237, 190, 1, 53, 0);
	rotationSetMovabilityToRotation(30402, 2, 1132625920, 0, 1118476698, 0, 2, 1106247680, 1097859072, 1118476698);
	rotationSetMovabilityOnOrOff(30402, 2, 2);
	rotationAddMovabilityToRotation(30501, 30012, "1250", 1981, -64, 2218, 324, 1, 53, 0);
	rotationSetMovabilityToRotation(30501, 0, 1126498304, 1090519040, 1118476698, 0, 2, 1129250816, 0, 1118476698);
	rotationAddMovabilityToRotation(30501, 30601, "1251", 390, -247, 766, 179, 1, 53, 0);
	rotationSetMovabilityToRotation(30501, 1, 1073741824, -1049624576, 1118476698, 0, 2, 1129250816, 0, 1118476698);
	rotationAddMovabilityToRotation(30601, 30602, "1253", 1571, -68, 1916, 132, 1, 53, 0);
	rotationSetMovabilityToRotation(30601, 0, 1124990976, -1049624576, 1118476698, 0, 2, 1132462080, -1049624576, 1118476698);
	rotationAddMovabilityToRotation(30601, 30602, "1253", 1274, 73, 1778, 525, 1, 53, 0);
	rotationSetMovabilityToRotation(30601, 1, 1124990976, -1049624576, 1118476698, 0, 2, 1132462080, -1049624576, 1118476698);
	rotationAddMovabilityToRotation(30601, 30501, "1254", 2654, 191, 3427, 436, 1, 53, 0);
	rotationSetMovabilityToRotation(30601, 2, 1132593152, 1097859072, 1118476698, 0, 2, 1117913088, 1097859072, 1118476698);
	rotationAddMovabilityToPuzzle(30601, kPuzzle35003, "", 3529, -177, 3599, 108, 1, 52, 0);
	rotationSetMovabilityToPuzzle(30601, 3, 1134395392, 1065353216, 1118476698, 0, 2);
	rotationAddMovabilityToPuzzle(30601, kPuzzle35003, "", 0, -177, 96, 108, 1, 52, 0);
	rotationSetMovabilityToPuzzle(30601, 4, 1134395392, 1065353216, 1118476698, 0, 2);
	rotationAddMovabilityToPuzzle(30601, kPuzzle35004, "", 2077, -43, 2466, 251, 1, 52, 0);
	rotationSetMovabilityToPuzzle(30601, 5, 1127481344, 1095761920, 1118476698, 0, 2);
	rotationAddMovabilityToPuzzle(30601, kPuzzle35005, "", 530, -193, 703, 102, 1, 52, 0);
	rotationSetMovabilityToPuzzle(30601, 6, 1099431936, 0, 1118476698, 0, 2);
	rotationAddMovabilityToPuzzle(30601, kPuzzle35006, "", 174, -111, 451, 52, 1, 52, 0);
	rotationSetMovabilityToPuzzle(30601, 7, 1135443968, 0, 1118476698, 0, 2);
	rotationAddMovabilityToPuzzle(30601, kPuzzle35007, "", 1775, 127, 2065, 375, 1, 52, 0);
	rotationSetMovabilityToPuzzle(30601, 8, 1125187584, 1104674816, 1118476698, 0, 2);
	rotationAddMovabilityToPuzzle(30601, kPuzzle35008, "", 251, 123, 305, 181, 1, 52, 0);
	rotationSetMovabilityToPuzzle(30601, 9, 1135312896, 1096810496, 1118476698, 0, 2);
	rotationAddMovabilityToRotation(30602, 30601, "1255", 2536, 303, 3188, 511, 1, 53, 0);
	rotationSetMovabilityToRotation(30602, 0, 1131741184, 1097859072, 1118476698, 0, 2, 0, 0, 1118476698);
	rotationAddMovabilityToRotation(30602, 30701, "1256", 1423, -125, 1617, 139, 1, 53, 0);
	rotationSetMovabilityToRotation(30602, 1, 1121714176, 0, 1118476698, 0, 2, 1118961664, 0, 1118476698);
	rotationAddMovabilityToRotation(30701, 30702, "1257", 1269, -69, 1436, 261, 1, 53, 0);
	rotationSetMovabilityToRotation(30701, 0, 1118699520, 0, 1118476698, 0, 2, 1119092736, 0, 1118476698);
	rotationAddMovabilityToRotation(30701, 30602, "1258", 2990, -174, 3348, 225, 1, 53, 0);
	rotationSetMovabilityToRotation(30701, 1, 1132855296, 0, 1118476698, 0, 2, 1132331008, 0, 1118476698);
	rotationAddMovabilityToRotation(30702, 30703, "1259", 1451, 144, 1773, 439, 1, 53, 0);
	rotationSetMovabilityToRotation(30702, 0, 1122500608, 1097859072, 1118476698, 0, 2, 0, 0, 1118476698);
	rotationAddMovabilityToRotation(30702, 30701, "1260", 3091, -71, 3202, 209, 1, 53, 0);
	rotationSetMovabilityToRotation(30702, 1, 1132953600, 0, 1118476698, 0, 2, 1132920832, 0, 1118476698);
	rotationAddMovabilityToRotation(30702, 30704, "1261", 1265, -92, 1455, 219, 1, 53, 0);
	rotationSetMovabilityToRotation(30702, 2, 1119092736, 0, 1118476698, 0, 2, 1119092736, 0, 1118476698);
	rotationAddMovabilityToRotation(30703, 30702, "1262", 219, -530, 639, -404, 1, 53, 0);
	rotationSetMovabilityToRotation(30703, 0, 1133379584, -1049624576, 1118476698, 0, 2, 1133379584, 0, 1118476698);
	rotationAddMovabilityToPuzzle(30703, kPuzzle35010, "", 105, 266, 857, 456, 1, 52, 0);
	rotationSetMovabilityToPuzzle(30703, 1, 0, 1097859072, 1118476698, 0, 2);
	rotationAddMovabilityToRotation(30704, 30702, "1263", 3070, -68, 3253, 225, 1, 53, 0);
	rotationSetMovabilityToRotation(30704, 0, 1133019136, 0, 1118476698, 0, 2, 1132920832, 0, 1118476698);
	rotationAddMovabilityToPuzzle(30704, kPuzzle35020, "", 1214, -8, 1462, 176, 1, 52, 0);
	rotationSetMovabilityToPuzzle(30704, 1, 1118961664, 1088421888, 1118476698, 0, 2);
	rotationAddMovabilityToRotation(30801, 30401, "1264", 2847, -75, 2991, 17, 1, 53, 0);
	rotationSetMovabilityToRotation(30801, 0, 1132134400, 0, 1118476698, 0, 2, 1127874560, 0, 1118476698);
	rotationAddMovabilityToRotation(30801, 30402, "1265", 3183, -85, 3280, 5, 1, 53, 0);
	rotationSetMovabilityToRotation(30801, 1, 1133248512, 0, 1118476698, 0, 2, 1132625920, 0, 1118476698);
	rotationAddMovabilityToPuzzle(30801, kPuzzle35002, "", 984, -69, 1840, 256, 1, 52, 0);
	rotationSetMovabilityToPuzzle(30801, 2, 1119485952, 1097859072, 1118476698, 0, 2);
	puzzleAddMovabilityToRotation(kPuzzle35001, 30302, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle35001, 0, 1126825984, 1097859072, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle35002, 30801, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle35002, 0, 1119485952, 1097859072, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle35003, 30601, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle35003, 0, 1134395392, 1065353216, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle35004, 30601, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle35004, 0, 1127481344, 1095761920, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle35005, 30601, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle35005, 0, 1099431936, 0, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle35006, 30601, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle35006, 0, 1135443968, 0, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle35007, 30601, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle35007, 0, 1125187584, 1097859072, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle35008, 30601, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle35008, 0, 1135312896, 0, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle35009, 30602, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle35009, 0, 1127153664, -1044905984, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle35010, 30703, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle35010, 0, 0, 1097859072, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle35011, 30301, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle35011, 0, 1125515264, 1077936128, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle35019, 30004, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle35019, 0, 1116471296, 0, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle35020, 30704, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle35020, 0, 1118961664, 1088421888, 1118476698);
	puzzleAddMovabilityToPuzzle(kPuzzle35109, 35111, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleAddMovabilityToRotation(kPuzzle35100, 30003, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	objectAdd(kObjectWolfInstinct, "Wolf Vision", "FO_WolfVision", 1);
	objectAddBagAnimation(kObjectWolfInstinct, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectWolfInstinct, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectWolfInstinct, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectIngot, "Panel", "FO_Panel01_", 1);
	objectAddBagAnimation(kObjectIngot, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectIngot, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectIngot, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectIngot2, "Panel", "FO_Panel01_", 1);
	objectAddBagAnimation(kObjectIngot2, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectIngot2, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectIngot2, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectIngot3, "Panel", "FO_Panel01_", 1);
	objectAddBagAnimation(kObjectIngot3, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectIngot3, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectIngot3, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectLogeTear2, "Loge's Tear", "NI_Tear", 1);
	objectAddBagAnimation(kObjectLogeTear2, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectLogeTear2, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectLogeTear2, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObject30001, "", "", 1);
	objectAdd(kObjectWolfBadge, "Medallion", "FO_WolfLogo", 1);
	objectAddBagAnimation(kObjectWolfBadge, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectWolfBadge, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectWolfBadge, 22, 22, 0, 3, 0, 0, 3);
	objectAddPresentation(kObject30001);
	objectPresentationAddImageToPuzzle(kObject30001, 0, kPuzzle35011, "FOS03N01P01L01.bmp", 170, 133, 1, 1, 1000);
	objectAddPresentation(kObject30001);
	objectPresentationAddImageToPuzzle(kObject30001, 1, kPuzzle35011, "FOS03N01P01L02.bmp", 266, 178, 1, 1, 1000);
	objectAddPresentation(kObject30001);
	objectPresentationAddImageToPuzzle(kObject30001, 2, kPuzzle35011, "FOS03N01P01L03.bmp", 268, 211, 1, 1, 1000);
	objectAddPresentation(kObject30001);
	objectPresentationAddImageToPuzzle(kObject30001, 3, kPuzzle35011, "FOS03N01P01L04.bmp", 266, 249, 1, 1, 1000);
	objectAddPresentation(kObject30001);
	objectPresentationAddImageToPuzzle(kObject30001, 4, kPuzzle35011, "FOS03N01P01L05.bmp", 293, 332, 1, 1, 1000);
	objectAddPuzzleAccessibility(kObject30001, kPuzzle35011, Common::Rect(260, 171, 376, 374), 1, 52, 0);
	objectAddPuzzleAccessibility(kObject30001, kPuzzle35011, Common::Rect(273, 182, 361, 202), 1, 52, 1);
	objectAddPuzzleAccessibility(kObject30001, kPuzzle35011, Common::Rect(276, 218, 359, 238), 1, 52, 2);
	objectAddPuzzleAccessibility(kObject30001, kPuzzle35011, Common::Rect(275, 257, 361, 312), 1, 52, 3);
	objectAddPuzzleAccessibility(kObject30001, kPuzzle35011, Common::Rect(292, 333, 345, 388), 1, 52, 4);
	objectSetAccessibilityOnOrOff(kObject30001, 1, 4);
	objectAdd(kObjectPatience, "Saturn", "FO_Scroll01_", 1);
	objectAddBagAnimation(kObjectPatience, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectPatience, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectPatience, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectMovementAndIntuition, "Mercury", "FO_Scroll01_", 1);
	objectAddBagAnimation(kObjectMovementAndIntuition, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectMovementAndIntuition, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectMovementAndIntuition, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectLove, "Venus", "FO_Scroll01_", 1);
	objectAddBagAnimation(kObjectLove, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectLove, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectLove, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectImagination, "Moon", "FO_Scroll01_", 1);
	objectAddBagAnimation(kObjectImagination, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectImagination, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectImagination, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectDestruction, "Mars", "FO_Scroll01_", 1);
	objectAddBagAnimation(kObjectDestruction, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectDestruction, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectDestruction, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectJudgementAndDirection, "Jupiter", "FO_Scroll01_", 1);
	objectAddBagAnimation(kObjectJudgementAndDirection, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectJudgementAndDirection, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectJudgementAndDirection, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectWill, "Sun", "FO_Scroll01_", 1);
	objectAddBagAnimation(kObjectWill, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectWill, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectWill, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObject30002, "", "", 1);
	objectAddRotationAccessibility(kObject30002, 30301, Common::Rect(1858, 280, 2021, 366), 1, 52, 0);
	objectAdd(kObject30003, "", "", 1);
	objectAddRotationAccessibility(kObject30003, 30301, Common::Rect(2371, 280, 2540, 366), 1, 52, 0);
	objectAdd(kObject30004, "", "", 1);
	objectAddRotationAccessibility(kObject30004, 30301, Common::Rect(2884, 280, 3048, 366), 1, 52, 0);
	objectAdd(kObject30005, "", "", 1);
	objectAddRotationAccessibility(kObject30005, 30301, Common::Rect(3401, 280, 3561, 366), 1, 52, 0);
	objectAdd(kObject30006, "", "", 1);
	objectAddRotationAccessibility(kObject30006, 30301, Common::Rect(311, 280, 481, 366), 1, 52, 0);
	objectAdd(kObject30007, "", "", 1);
	objectAddRotationAccessibility(kObject30007, 30301, Common::Rect(827, 280, 998, 366), 1, 52, 0);
	objectAdd(kObject30008, "", "", 1);
	objectAddRotationAccessibility(kObject30008, 30301, Common::Rect(1337, 280, 1509, 366), 1, 52, 0);
	objectAdd(kObject30027, "", "", 1);
	objectAddPuzzleAccessibility(kObject30027, kPuzzle35001, Common::Rect(410, 230, 466, 273), 1, 53, 0);
	objectAdd(kObject30016, "", "", 4);
	objectSetPassiveDrawCursor(kObject30016, 20, 20, 0, 3, 0, 0, 3);
	objectSetActiveDrawCursor(kObject30016, 20, 20, 0, 3, 0, 0, 3);

	for (uint32 i = 0; i < 51; i++) {
		objectAddPresentation(kObject30016);
		objectPresentationAddImageToPuzzle(kObject30016, i, kPuzzle35001, Common::String::format("FOS03N02P01S01.%04d.bmp", i + 1), 341, 150, 1, 1, 1000);
	}

	objectPresentationShow(kObject30016, 25);
	objectAddPuzzleAccessibility(kObject30016, kPuzzle35001, Common::Rect(349, 164, 532, 332), 1, 10000, 0);
	objectAdd(kObjectBerries, "Berries", "FO_Berries", 1);
	objectAddBagAnimation(kObjectBerries, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectBerries, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectBerries, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectSleepingBerries, "Poison berries", "FO_BerriesPoison", 1);
	objectAddBagAnimation(kObjectSleepingBerries, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectSleepingBerries, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectSleepingBerries, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectBerries, kPuzzle35002, Common::Rect(232, 322, 346, 436), 1, 52, 0);
	objectAddPuzzleAccessibility(kObjectBerries, kPuzzle35002, Common::Rect(110, 120, 492, 305), 1, 52, 1);
	objectAddPuzzleAccessibility(kObjectBerries, kPuzzle35002, Common::Rect(116, 296, 205, 426), 1, 52, 2);
	objectAddPuzzleAccessibility(kObjectBerries, kPuzzle35002, Common::Rect(348, 251, 636, 414), 1, 52, 3);
	objectAddPuzzleAccessibility(kObjectBerries, kPuzzle35002, Common::Rect(505, 194, 556, 238), 1, 52, 4);
	objectAddPresentation(kObjectBerries);
	objectPresentationAddImageToPuzzle(kObjectBerries, 0, kPuzzle35002, "FOS08N01P01L01.BMP", 508, 197, 1, 1, 1000);
	objectAddPresentation(kObjectBerries);
	objectPresentationAddImageToPuzzle(kObjectBerries, 1, kPuzzle35002, "FOS08N01P01L02.BMP", 0, 16, 1, 1, 1000);
	objectAddPresentation(kObjectBerries);
	objectPresentationAddImageToPuzzle(kObjectBerries, 2, kPuzzle35002, "FOS08N01P01L03.BMP", 510, 198, 1, 1, 1000);
	objectAddPresentation(kObjectBerries);
	objectPresentationAddImageToPuzzle(kObjectBerries, 3, kPuzzle35002, "FOS08N01P01L04.BMP", 209, 319, 1, 1, 1000);
	objectAdd(kObjectBerriesJuice, "Berries juice", "FO_BerriesJuice", 1);
	objectAddBagAnimation(kObjectBerriesJuice, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectBerriesJuice, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectBerriesJuice, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectSleepingPotion2, "Poison berries juice", "FO_BerriesPoisonJuice", 1);
	objectAddBagAnimation(kObjectSleepingPotion2, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectSleepingPotion2, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectSleepingPotion2, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectBerries, kPuzzle35005, Common::Rect(155, 283, 407, 447), 1, 52, 5);
	objectAddPresentation(kObjectBerries);
	objectPresentationAddImageToPuzzle(kObjectBerries, 4, kPuzzle35005, "FOS06N01P05L01.BMP", 209, 281, 1, 1, 1000);
	objectAddPresentation(kObjectBerries);
	objectPresentationAddImageToPuzzle(kObjectBerries, 5, kPuzzle35005, "FOS06N01P05L02.BMP", 243, 402, 1, 1, 1000);
	objectAdd(kObjectSleepingPotion, "", "", 1);
	objectAddPresentation(kObjectSleepingPotion);
	objectPresentationAddAnimationToRotation(kObjectSleepingPotion, 0, 30003, 0, 26, 1101004800, 4);
	objectPresentationSetAnimationOnRotation(kObjectSleepingPotion, 0, 0, 30006);
	objectPresentationShow(kObjectSleepingPotion, 0);
	objectAddRotationAccessibility(kObjectSleepingPotion, 30003, Common::Rect(1931, 76, 2112, 263), 1, 52, 0);
	objectAddRotationAccessibility(kObjectSleepingPotion, 30003, Common::Rect(1476, -50, 1741, 245), 1, 53, 1);
	objectAdd(kObjectHare, "Rabbit hare", "FO_Hares", 1);
	objectAddBagAnimation(kObjectHare, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectHare, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectHare, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectBow, "Hunting bow", "FO_HuntingBow", 1);
	objectAddBagAnimation(kObjectBow, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectBow, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectBow, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectBow, kPuzzle35003, Common::Rect(240, 200, 400, 410), 1, 52, 0);
	objectAddPuzzleAccessibility(kObjectBow, kPuzzle35003, Common::Rect(230, 219, 278, 275), 1, 52, 1);
	objectAddPuzzleAccessibility(kObjectBow, kPuzzle35003, Common::Rect(237, 280, 273, 334), 1, 52, 2);
	objectAddPuzzleAccessibility(kObjectBow, kPuzzle35003, Common::Rect(353, 228, 387, 283), 1, 52, 3);
	objectAddPuzzleAccessibility(kObjectBow, kPuzzle35003, Common::Rect(384, 226, 415, 281), 1, 52, 4);
	objectAddPuzzleAccessibility(kObjectBow, kPuzzle35003, Common::Rect(328, 286, 366, 452), 1, 52, 5);
	objectAddPuzzleAccessibility(kObjectBow, kPuzzle35003, Common::Rect(277, 174, 326, 447), 1, 52, 6);
	objectAddPresentation(kObjectBow);
	objectPresentationAddImageToPuzzle(kObjectBow, 0, kPuzzle35003, "FOS06N01P06L01.bmp", 72, 16, 1, 1, 1000);
	objectAddPresentation(kObjectBow);
	objectPresentationAddImageToPuzzle(kObjectBow, 1, kPuzzle35003, "FOS06N01P06L02.bmp", 233, 219, 1, 1, 1000);
	objectAddPresentation(kObjectBow);
	objectPresentationAddImageToPuzzle(kObjectBow, 2, kPuzzle35003, "FOS06N01P06L03.bmp", 241, 273, 1, 1, 1000);
	objectAddPresentation(kObjectBow);
	objectPresentationAddImageToPuzzle(kObjectBow, 3, kPuzzle35003, "FOS06N01P06L04.bmp", 357, 221, 1, 1, 1000);
	objectAddPresentation(kObjectBow);
	objectPresentationAddImageToPuzzle(kObjectBow, 4, kPuzzle35003, "FOS06N01P06L05.bmp", 387, 222, 1, 1, 1000);
	objectAddPresentation(kObjectBow);
	objectPresentationAddImageToPuzzle(kObjectBow, 5, kPuzzle35003, "FOS06N01P06L06.bmp", 326, 282, 1, 1, 1000);
	objectAddPresentation(kObjectBow);
	objectPresentationAddImageToPuzzle(kObjectBow, 6, kPuzzle35003, "FOS06N01P06L07.bmp", 274, 170, 1, 1, 1000);
	objectAdd(kObject30028, "", "", 1);
	objectAddPuzzleAccessibility(kObject30028, kPuzzle35004, Common::Rect(450, 265, 512, 340), 1, 52, 0);
	objectAddPuzzleAccessibility(kObject30028, kPuzzle35004, Common::Rect(70, 350, 240, 415), 1, 52, 1);
	objectAddPresentation(kObject30028);
	objectPresentationAddImageToPuzzle(kObject30028, 0, kPuzzle35004, "FOS06N01P01L01.bmp", 0, 339, 1, 1, 1000);
	objectAddPresentation(kObject30028);
	objectPresentationAddAnimationToPuzzle(kObject30028, 1, kPuzzle35004, "FOS06N01P01LS01", 0, 1, 250, 1, 1000, 13, 1095237632, 4);
	objectAddPresentation(kObject30028);
	objectPresentationAddAnimationToPuzzle(kObject30028, 2, kPuzzle35004, "FOS06N01P01LS02", 0, 1, 242, 1, 1000, 13, 1095237632, 4);
	objectAddPresentation(kObject30028);
	objectPresentationAddImageToRotation(kObject30028, 3, 30601, 0);
	objectAdd(kObject30040, "", "", 1);
	objectAdd(kObjectMetals, "Ignots", "FO_Ignots", 1);
	objectAddBagAnimation(kObjectMetals, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectMetals, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectMetals, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectMould, "Pan", "FO_Pan", 1);
	objectAddBagAnimation(kObjectMould, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectMould, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectMould, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectMeltedGold, "Gold", "FO_IgnotGld", 1);
	objectAddBagAnimation(kObjectMeltedGold, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectMeltedGold, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectMeltedGold, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectMeltedSilver, "Silver", "FO_IgnotSil", 1);
	objectAddBagAnimation(kObjectMeltedSilver, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectMeltedSilver, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectMeltedSilver, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectMeltedCopper, "Copper", "FO_IgnotCop", 1);
	objectAddBagAnimation(kObjectMeltedCopper, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectMeltedCopper, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectMeltedCopper, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectMeltedLead, "Lead", "FO_IgnotLed", 1);
	objectAddBagAnimation(kObjectMeltedLead, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectMeltedLead, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectMeltedLead, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectMeltedSteel, "Steel", "FO_IgnotSte", 1);
	objectAddBagAnimation(kObjectMeltedSteel, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectMeltedSteel, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectMeltedSteel, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectMeltedTin, "Tin", "FO_IgnotTin", 1);
	objectAddBagAnimation(kObjectMeltedTin, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectMeltedTin, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectMeltedTin, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectQuicksilver, "Mercury", "FO_IgnotMer", 1);
	objectAddBagAnimation(kObjectQuicksilver, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectQuicksilver, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectQuicksilver, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObject30040, kPuzzle35006, Common::Rect(293, 132, 351, 173), 1, 52, 0);
	objectAddPuzzleAccessibility(kObject30040, kPuzzle35006, Common::Rect(285, 207, 354, 245), 1, 52, 1);
	objectAddPuzzleAccessibility(kObject30040, kPuzzle35006, Common::Rect(173, 229, 244, 271), 1, 52, 2);
	objectAddPuzzleAccessibility(kObject30040, kPuzzle35006, Common::Rect(201, 289, 284, 329), 1, 52, 3);
	objectAddPuzzleAccessibility(kObject30040, kPuzzle35006, Common::Rect(214, 135, 364, 189), 1, 52, 4);
	objectAddPuzzleAccessibility(kObject30040, kPuzzle35006, Common::Rect(222, 210, 353, 277), 1, 52, 5);
	objectSetAccessibilityOnOrOff(kObject30040, 2, 2);
	objectSetAccessibilityOnOrOff(kObject30040, 3, 3);
	objectSetAccessibilityOnOrOff(kObject30040, 4, 4);
	objectSetAccessibilityOnOrOff(kObject30040, 5, 5);
	objectAddPresentation(kObject30040);
	objectPresentationAddImageToPuzzle(kObject30040, 0, kPuzzle35006, "FOS06N01P02L02.bmp", 1, 44, 1, 1, 1000);
	objectAddPresentation(kObject30040);
	objectPresentationAddImageToPuzzle(kObject30040, 1, kPuzzle35006, "FOS06N01P02L01.bmp", 2, 45, 1, 1, 1000);
	objectAddPresentation(kObject30040);
	objectPresentationAddImageToPuzzle(kObject30040, 2, kPuzzle35006, "FOS06N01P02L03.bmp", 58, 151, 1, 1, 1000);
	objectAddPresentation(kObject30040);
	objectPresentationAddImageToPuzzle(kObject30040, 3, kPuzzle35006, "FOS06N01P02L04.bmp", 54, 132, 1, 1, 1000);
	objectAddPresentation(kObject30040);
	objectPresentationAddAnimationToPuzzle(kObject30040, 4, kPuzzle35006, "FOS06N01P02LS01", 0, 157, 168, 1, 1000, 25, 1095237632, 6);
	objectPresentationSetAnimationOnPuzzle(kObject30040, 4, 0, 30008);
	objectAdd(kObject30042, "", "", 1);
	objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(248, 261, 287, 278), 1, 52, 0);
	objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(405, 79, 453, 115), 1, 52, 1);
	objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(337, 47, 379, 77), 1, 52, 2);
	objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(453, 199, 492, 228), 1, 52, 3);
	objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(245, 48, 285, 74), 1, 52, 4);
	objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(285, 46, 330, 73), 1, 52, 5);
	objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(419, 120, 457, 151), 1, 52, 6);
	objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(441, 266, 476, 294), 1, 52, 7);
	objectSetAccessibilityOnOrOff(kObject30042, 1, 7);
	objectAddPresentation(kObject30042);
	objectPresentationAddImageToPuzzle(kObject30042, 0, kPuzzle35007, "FOS06N01P03L02.bmp", 0, 16, 1, 1, 1000);
	objectAddPresentation(kObject30042);
	objectPresentationAddImageToPuzzle(kObject30042, 1, kPuzzle35007, "FOS06N01P03L03_03.bmp", 334, 200, 1, 1, 1000);
	objectAddPresentation(kObject30042);
	objectPresentationAddImageToPuzzle(kObject30042, 2, kPuzzle35007, "FOS06N01P03L03_02.bmp", 328, 182, 1, 1, 1000);
	objectAddPresentation(kObject30042);
	objectPresentationAddImageToPuzzle(kObject30042, 3, kPuzzle35007, "FOS06N01P03L03_05.bmp", 335, 229, 1, 1, 1000);
	objectAddPresentation(kObject30042);
	objectPresentationAddImageToPuzzle(kObject30042, 4, kPuzzle35007, "FOS06N01P03L03_06.bmp", 326, 252, 1, 1, 1000);
	objectAddPresentation(kObject30042);
	objectPresentationAddImageToPuzzle(kObject30042, 5, kPuzzle35007, "FOS06N01P03L03_01.bmp", 294, 221, 1, 1, 1000);
	objectAddPresentation(kObject30042);
	objectPresentationAddImageToPuzzle(kObject30042, 6, kPuzzle35007, "FOS06N01P03L03_04.bmp", 368, 229, 1, 1, 1000);
	objectAddPresentation(kObject30042);
	objectPresentationAddImageToPuzzle(kObject30042, 7, kPuzzle35007, "FOS06N01P03L03_07.bmp", 312, 307, 1, 1, 1000);
	objectAddPresentation(kObject30042);
	objectPresentationAddImageToPuzzle(kObject30042, 8, kPuzzle35007, "FOS06N01P03L01.bmp", 0, 16, 1, 1, 1000);
	objectAdd(kObjectGolem, "Golem", "FO_Golem", 1);
	objectAddBagAnimation(kObjectGolem, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectGolem, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectGolem, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObject30044, "", "", 1);
	objectAddPuzzleAccessibility(kObject30044, kPuzzle35008, Common::Rect(253, 206, 333, 300), 1, 52, 0);
	objectAddPuzzleAccessibility(kObject30044, kPuzzle35008, Common::Rect(203, 262, 300, 379), 1, 52, 1);
	objectSetAccessibilityOnOrOff(kObject30044, 1, 1);
	objectAddPresentation(kObject30044);
	objectPresentationAddImageToPuzzle(kObject30044, 0, kPuzzle35008, "FOS06N01P04L01.bmp", 189, 229, 1, 1, 1000);
	objectAdd(kObject30045, "", "", 1);
	objectAddPuzzleAccessibility(kObject30045, kPuzzle35009, Common::Rect(283, 238, 347, 299), 1, 52, 0);
	objectAddPuzzleAccessibility(kObject30045, kPuzzle35009, Common::Rect(273, 315, 306, 352), 1, 52, 1);
	objectAddPuzzleAccessibility(kObject30045, kPuzzle35009, Common::Rect(293, 198, 330, 236), 1, 52, 2);
	objectAddPuzzleAccessibility(kObject30045, kPuzzle35009, Common::Rect(342, 217, 387, 265), 1, 52, 3);
	objectAddPuzzleAccessibility(kObject30045, kPuzzle35009, Common::Rect(225, 241, 261, 281), 1, 52, 4);
	objectAddPuzzleAccessibility(kObject30045, kPuzzle35009, Common::Rect(250, 268, 288, 306), 1, 52, 5);
	objectAddPuzzleAccessibility(kObject30045, kPuzzle35009, Common::Rect(351, 267, 403, 325), 1, 52, 6);
	objectAddRotationAccessibility(kObject30045, 30602, Common::Rect(1856, -381, 2445, -20), 1, 52, 7);
	objectAddPresentation(kObject30045);
	objectPresentationAddImageToPuzzle(kObject30045, 0, kPuzzle35009, "FOS06N02P01L01.bmp", 195, 34, 1, 1, 1000);
	objectAddPresentation(kObject30045);
	objectPresentationAddImageToPuzzle(kObject30045, 1, kPuzzle35009, "FOS06N02P01L02.bmp", 193, 32, 1, 1, 1000);
	objectAddPresentation(kObject30045);
	objectPresentationAddImageToPuzzle(kObject30045, 2, kPuzzle35009, "FOS06N02P01L03.bmp", 148, 16, 1, 1, 1000);
	objectAddPresentation(kObject30045);
	objectPresentationAddImageToPuzzle(kObject30045, 3, kPuzzle35009, "FOS06N02P01L06.bmp", 174, 16, 1, 1, 1000);
	objectAddPresentation(kObject30045);
	objectPresentationAddImageToPuzzle(kObject30045, 4, kPuzzle35009, "FOS06N02P01L05.bmp", 203, 16, 1, 1, 1000);
	objectAddPresentation(kObject30045);
	objectPresentationAddImageToPuzzle(kObject30045, 5, kPuzzle35009, "FOS06N02P01L04.bmp", 171, 22, 1, 1, 1000);
	objectAddPresentation(kObject30045);
	objectPresentationAddImageToPuzzle(kObject30045, 6, kPuzzle35009, "FOS06N02P01L07.bmp", 161, 37, 1, 1, 1000);
	objectAddPresentation(kObject30045);
	objectPresentationAddAnimationToRotation(kObject30045, 7, 30602, 0, 75, 1106247680, 4);
	objectPresentationSetAnimationOnRotation(kObject30045, 7, 0, 30000);
	objectPresentationShow(kObject30045, 7);
	objectAdd(kObjectFishingRod, "Fishing pole", "FO_Fishing", 1);
	objectAddBagAnimation(kObjectFishingRod, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectFishingRod, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectFishingRod, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectFish, "Fish", "FO_Fish", 1);
	objectAddBagAnimation(kObjectFish, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectFish, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectFish, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectFishingRodWithWorms, "Fishing pole and worms", "FO_FishingWorms", 1);
	objectAddBagAnimation(kObjectFishingRodWithWorms, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectFishingRodWithWorms, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectFishingRodWithWorms, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectKey, "Key form the fish", "FO_FishKey", 1);
	objectAddBagAnimation(kObjectKey, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectKey, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectKey, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectFishingRod, kPuzzle35010, Common::Rect(0, 208, 639, 384), 1, 52, 0);
	objectAddPuzzleAccessibility(kObjectFishingRod, kPuzzle35010, Common::Rect(0, 208, 639, 384), 1, 52, 1);
	objectAddPuzzleAccessibility(kObjectFishingRod, kPuzzle35010, Common::Rect(0, 63, 639, 197), 1, 52, 2);
	objectSetAccessibilityOnOrOff(kObjectFishingRod, 1, 2);
	objectAddRotationAccessibility(kObjectFishingRod, 30703, Common::Rect(1126, 141, 3373, 523), 1, 52, 3);
	objectAddPresentation(kObjectFishingRod);
	objectPresentationAddImageToPuzzle(kObjectFishingRod, 0, kPuzzle35010, "FOS07N03P01L01.bmp", 0, 16, 1, 1, 1000);
	objectAddPresentation(kObjectFishingRod);
	objectPresentationAddImageToPuzzle(kObjectFishingRod, 1, kPuzzle35010, "FOS07N03P01L02.bmp", 0, 16, 1, 1, 1000);
	objectAdd(kObjectWorms, "Worms", "FO_Worms", 1);
	objectAddBagAnimation(kObjectWorms, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectWorms, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectWorms, 22, 22, 0, 3, 0, 0, 3);
	objectAddPresentation(kObjectWorms);
	objectPresentationAddImageToPuzzle(kObjectWorms, 0, kPuzzle35020, "FOS07N04P01L01.bmp", 0, 16, 1, 1, 1000);
	objectAddPresentation(kObjectWorms);
	objectPresentationAddAnimationToPuzzle(kObjectWorms, 1, kPuzzle35020, "FOS07N04P01S01", 0, 312, 209, 1, 1000, 12, 1095237632, 4);
	objectAddPuzzleAccessibility(kObjectWorms, kPuzzle35020, Common::Rect(285, 196, 391, 330), 0, 52, 0);
	objectAdd(kObject30050, "", "", 1);
	objectAddRotationAccessibility(kObject30050, 30501, Common::Rect(1177, 298, 1263, 530), 1, 52, 0);
	objectAddPresentation(kObject30050);
	objectPresentationAddImageToRotation(kObject30050, 0, 30501, 0);
	objectAddPresentation(kObject30050);
	objectPresentationAddAnimationToRotation(kObject30050, 1, 30501, 1, 13, 1095237632, 6);
	objectPresentationSetAnimationOnRotation(kObject30050, 1, 0, 30009);
	objectAddPresentation(kObject30050);
	objectPresentationAddImageToRotation(kObject30050, 2, 30402, 0);
	objectAddPresentation(kObject30050);
	objectPresentationAddImageToRotation(kObject30050, 3, 30501, 2);
	objectAdd(kObject30051, "", "", 1);
	objectAdd(kObject30052, "", "", 1);
	objectAdd(kObjectBurningArrow, "Inflamed arrow and bow", "FO_BowFire", 1);
	objectAddBagAnimation(kObjectBurningArrow, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectBurningArrow, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectBurningArrow, 22, 22, 0, 3, 0, 0, 3);
	objectAddRotationAccessibility(kObject30051, 30011, Common::Rect(411, 0, 511, 57), 0, 52, 0);
	objectAddRotationAccessibility(kObject30051, 30011, Common::Rect(958, 102, 1105, 333), 1, 52, 1);
	objectAddRotationAccessibility(kObject30051, 30011, Common::Rect(0, 0, 0, 0), 1, 52, 2);
	objectAddRotationAccessibility(kObject30051, 30011, Common::Rect(421, 15, 764, 170), 0, 52, 3);
	objectAddPresentation(kObject30051);
	objectPresentationAddImageToRotation(kObject30051, 0, 30011, 0);
	objectAddPresentation(kObject30051);
	objectPresentationAddAnimationToRotation(kObject30051, 1, 30011, 1, 22, 1101004800, 4);
	objectAddPresentation(kObject30051);
	objectPresentationAddImageToRotation(kObject30051, 2, 30011, 2);
	objectPresentationShow(kObject30051, 1);
	objectAdd(kObject30100, "", "", 1);
	objectAddPresentation(kObject30100);
	objectPresentationAddImageToPuzzle(kObject30100, 0, kPuzzle35100, "FOS00N03P02L01.bmp", 117, 70, 1, 1, 1000);
	objectAddPuzzleAccessibility(kObject30100, kPuzzle35100, Common::Rect(114, 106, 406, 463), 1, 52, 0);
	objectAddPuzzleAccessibility(kObject30100, kPuzzle35100, Common::Rect(311, 249, 397, 463), 0, 52, 1);
	objectAdd(kObjectWolfBrooch, "Sieglinde's Medallion", "FO_WolfLogo", 1);
	objectAddBagAnimation(kObjectWolfBrooch, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectWolfBrooch, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectWolfBrooch, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObject30101, "", "", 1);
	objectAddPresentation(kObject30101);
	objectPresentationAddAnimationToPuzzle(kObject30101, 0, kPuzzle35101, "FOS00N03P02S02", 0, 297, 209, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObject30102, "", "", 1);
	objectAdd(kObject30103, "", "", 1);
	objectAddPresentation(kObject30103);
	objectPresentationAddAnimationToPuzzle(kObject30103, 0, kPuzzle35103, "FOS01N01P07", 0, 380, 143, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObject30104, "", "", 1);
	objectAddPresentation(kObject30104);
	objectPresentationAddAnimationToPuzzle(kObject30104, 0, kPuzzle35104, "FOS01N01P03", 0, 165, 129, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObject30105, "", "", 1);
	objectAddPresentation(kObject30105);
	objectPresentationAddAnimationToPuzzle(kObject30105, 0, kPuzzle35105, "FOS01N01P04", 0, 383, 171, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObject30106, "", "", 1);
	objectAddPresentation(kObject30106);
	objectPresentationAddAnimationToPuzzle(kObject30106, 0, kPuzzle35106, "FOS01N01P05", 0, 443, 94, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObject30107, "", "", 1);
	objectAddPresentation(kObject30107);
	objectPresentationAddAnimationToPuzzle(kObject30107, 0, kPuzzle35107, "FOS01N01P06", 0, 303, 161, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObject30109, "", "", 1);
	objectAddPresentation(kObject30109);
	objectPresentationAddAnimationToPuzzle(kObject30109, 0, kPuzzle35111, "FOS01N01P02S01", 0, 190, 141, 1, 1000, 202, 1095237632, 4);
	objectPresentationSetAnimationOnPuzzle(kObject30109, 0, 0, 30007);
	objectAddPuzzleAccessibility(kObject30109, kPuzzle35111, Common::Rect(0, 334, 463, 418), 0, 52, 0);
	objectAdd(kObject30108, "", "", 1);
	objectAddPuzzleAccessibility(kObject30108, kPuzzle35109, Common::Rect(181, 165, 256, 259), 1, 52, 0);
	objectAddRotationAccessibility(kObject30102, 30101, Common::Rect(0, 0, 0, 0), 1, 52, 0);
	objectAddRotationAccessibility(kObject30102, 30101, Common::Rect(1741, 134, 1826, 221), 1, 52, 1);
	objectAdd(kObject30110, "", "", 1);
	objectAddPresentation(kObject30110);
	objectPresentationAddImageToRotation(kObject30110, 0, 30101, 0);
	objectAddPresentation(kObject30110);
	objectPresentationAddAnimationToRotation(kObject30110, 1, 30101, 1, 200, 1095237632, 4);
	objectPresentationSetAnimationOnRotation(kObject30110, 1, 0, 30001);
	objectAddPresentation(kObject30110);
	objectPresentationAddImageToRotation(kObject30110, 2, 30101, 2);
	objectAddPresentation(kObject30110);
	objectPresentationAddImageToRotation(kObject30110, 3, 30101, 3);
	objectAdd(kObjectNotung, "Sword Notung", "", 1);
	objectAddPuzzleAccessibility(kObjectNotung, kPuzzle35019, Common::Rect(290, 167, 363, 232), 0, 52, 0);
	objectAddPresentation(kObjectNotung);
	objectPresentationAddImageToPuzzle(kObjectNotung, 0, kPuzzle35019, "FOS00N04P01L01.bmp", 0, 16, 1, 1, 1000);
	objectAddPresentation(kObjectNotung);
	objectPresentationAddImageToPuzzle(kObjectNotung, 1, kPuzzle35019, "FOS00N04P01L02.bmp", 303, 192, 1, 1, 1000);
	objectAddPresentation(kObjectNotung);
	objectPresentationAddImageToPuzzle(kObjectNotung, 2, kPuzzle35019, "FOS00N04P01L03.bmp", 252, 41, 1, 1, 1000);
	objectAddPresentation(kObjectNotung);
	objectPresentationAddImageToPuzzle(kObjectNotung, 3, kPuzzle35019, "FOS00N04P01L04.bmp", 284, 193, 1, 1, 1000);
	objectAdd(kObject30059, "", "", 1);
	objectAddRotationAccessibility(kObject30059, 30302, Common::Rect(2183, -268, 2752, 33), 1, 52, 0);
	objectAddRotationAccessibility(kObject30059, 30303, Common::Rect(2183, -268, 2752, 33), 1, 52, 1);
	objectAdd(kObject30061, "", "", 1);
	objectAddPresentation(kObject30061);
	objectPresentationAddImageToRotation(kObject30061, 0, 30001, 0);
	objectAddPresentation(kObject30061);
	objectPresentationAddImageToRotation(kObject30061, 1, 30010, 0);
	objectAddPresentation(kObject30061);
	objectPresentationAddImageToRotation(kObject30061, 2, 30301, 0);
	objectAddPresentation(kObject30061);
	objectPresentationAddImageToRotation(kObject30061, 3, 30008, 0);
	objectAddPresentation(kObject30061);
	objectPresentationAddImageToRotation(kObject30061, 4, 30009, 0);
	objectAddPresentation(kObject30061);
	objectPresentationAddImageToRotation(kObject30061, 5, 30702, 0);
	objectAddPresentation(kObject30061);
	objectPresentationAddImageToRotation(kObject30061, 6, 30703, 0);
	objectAdd(kObject30200, "", "", 1);
	objectAddRotationAccessibility(kObject30200, 30301, Common::Rect(3452, 64, 3505, 109), 1, 52, 0);
	objectAddRotationAccessibility(kObject30200, 30301, Common::Rect(2939, 57, 2995, 99), 1, 52, 1);
	objectAddRotationAccessibility(kObject30200, 30301, Common::Rect(873, 68, 949, 120), 1, 52, 2);
	objectAddRotationAccessibility(kObject30200, 30301, Common::Rect(2425, 71, 2478, 118), 1, 52, 3);
	objectAddRotationAccessibility(kObject30200, 30301, Common::Rect(1390, 62, 1451, 108), 1, 52, 4);
	objectAddRotationAccessibility(kObject30200, 30301, Common::Rect(1912, 54, 1959, 118), 1, 52, 5);
	objectAddRotationAccessibility(kObject30200, 30301, Common::Rect(367, 62, 425, 111), 1, 52, 6);
	soundAdd(30007, 1, "1267.was", _configuration.backgroundMusic.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30000, 2, "1269.was", _configuration.backgroundMusic.loadFrom, 2, 18);
	soundAdd(30001, 2, "1270.was", _configuration.backgroundMusic.loadFrom, 2, 18);
	soundAdd(30002, 2, "1271.was", _configuration.backgroundMusic.loadFrom, 2, 18);
	soundAdd(30003, 2, "1272.was", _configuration.backgroundMusic.loadFrom, 2, 18);
	soundAdd(30004, 2, "1273.was", _configuration.backgroundMusic.loadFrom, 2, 18);
	soundAdd(30005, 2, "1274.was", _configuration.backgroundMusic.loadFrom, 2, 18);
	soundAdd(30006, 2, "1275.was", _configuration.backgroundMusic.loadFrom, 2, 18);
	soundSetVolume(30005, 90);
	soundSetVolume(30002, 95);
	rotationAddAmbientSound(30001, 30000, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30002, 30000, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30003, 30000, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30004, 30000, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30005, 30000, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30006, 30000, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30008, 30000, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30009, 30000, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30010, 30000, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30011, 30000, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30012, 30000, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35019, 30000, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35100, 30000, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30101, 30003, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35101, 30003, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35103, 30003, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35104, 30003, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35105, 30003, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35106, 30003, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35107, 30003, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35108, 30003, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35109, 30003, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35110, 30003, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35111, 30003, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30301, 30004, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30302, 30004, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30303, 30004, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35001, 30004, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35011, 30004, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30401, 30005, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30402, 30005, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30801, 30005, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30401, 30006, 90, 0, 1, 1, 10);
	rotationAddAmbientSound(30402, 30006, 90, 0, 1, 1, 10);
	rotationAddAmbientSound(30801, 30006, 90, 0, 1, 1, 10);
	rotationAddAmbientSound(30011, 30006, 87, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35002, 30006, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30501, 30002, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30601, 30002, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30602, 30002, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35009, 30002, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35005, 30002, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35003, 30002, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35004, 30002, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35006, 30002, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35007, 30002, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35008, 30002, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30701, 30001, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30702, 30001, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30703, 30001, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(30704, 30001, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35010, 30001, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle35020, 30001, 100, 0, 1, 1, 10);
	soundAdd(30201, 3, "1276.wav", _configuration.ambientEffect.loadFrom);
	soundAdd(30202, 3, "1277.wav", _configuration.ambientEffect.loadFrom);
	soundAdd(30300, 3, "1278.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30301, 3, "1279.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30302, 3, "1280.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30303, 3, "1281.wav", _configuration.ambientEffect.loadFrom);
	soundAdd(30304, 3, "1282.wav", _configuration.ambientEffect.loadFrom);
	soundAdd(30305, 3, "1283.wav", _configuration.ambientEffect.loadFrom);
	soundAdd(30306, 3, "1284.wav", _configuration.ambientEffect.loadFrom);
	soundAdd(30307, 3, "1285.wav", _configuration.ambientEffect.loadFrom);
	soundAdd(30308, 3, "1286.wav", _configuration.ambientEffect.loadFrom);
	soundAdd(30309, 3, "1287.wav", _configuration.ambientEffect.loadFrom);
	soundAdd(30310, 3, "1288.wav", _configuration.ambientEffect.loadFrom);
	soundAdd(30311, 3, "1289.wav", _configuration.ambientEffect.loadFrom);
	soundAdd(30312, 3, "1290.wav", _configuration.ambientEffect.loadFrom);
	soundAdd(30313, 3, "1291.wav", _configuration.ambientEffect.loadFrom);
	soundAdd(30314, 3, "1292.wav", _configuration.ambientEffect.loadFrom);
	soundAdd(30501, 3, "1293.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30502, 3, "1294.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30315, 3, "1295.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30316, 3, "1296.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30317, 3, "1297.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30318, 3, "1298.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30319, 3, "1299.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30320, 3, "1300.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30321, 3, "1301.was", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30322, 3, "1302.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30323, 3, "1303.was", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30324, 3, "1304.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30325, 3, "1305.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30326, 3, "1306.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30503, 3, "1307.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30504, 3, "1308.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30505, 3, "1309.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30506, 3, "1310.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30508, 3, "1311.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30509, 3, "1312.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30510, 3, "1313.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30511, 3, "1314.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30512, 3, "1315.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30513, 3, "1316.was", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30514, 3, "1317.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30500, 4, "1320.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(30100, 5, "1322.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30101, 5, "1323.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30102, 5, "1324.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30103, 5, "1325.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30104, 5, "1326.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30105, 5, "1327.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30106, 5, "1328.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30107, 5, "1329.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30108, 5, "1330.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30161, 5, "1331.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30109, 5, "1332.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30118, 5, "1333.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30111, 5, "1334.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30112, 5, "1335.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30113, 5, "1336.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30114, 5, "1337.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30115, 5, "1338.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30116, 5, "1340.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30110, 5, "1339.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30117, 5, "1341.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30120, 5, "1342.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30121, 5, "1343.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30122, 5, "1344.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30123, 5, "1345.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30124, 5, "1346.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30125, 5, "1347.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30126, 5, "1348.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30127, 5, "1349.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30128, 5, "1350.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30129, 5, "1351.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30130, 5, "1352.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30131, 5, "1353.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30132, 5, "1354.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30133, 5, "1355.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30134, 5, "1356.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30135, 5, "1357.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30136, 5, "1358.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30150, 5, "1359.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30151, 5, "1360.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30152, 5, "1361.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30153, 5, "1362.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30154, 5, "1363.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30155, 5, "1364.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30156, 5, "1365.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30162, 5, "1366.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30163, 5, "1367.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30164, 5, "1368.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30165, 5, "1369.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30166, 5, "1370.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30167, 5, "1371.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(30168, 5, "1372.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	rotationAdd3DSound(30602, 30202, 1, 1, 15, 100, 174.0, 10);
	rotationAdd3DSound(30601, 30202, 1, 1, 15, 80, 140.0, 10);
	rotationAdd3DSound(30001, 30501, 1, 1, 20, 75, 244.0, 3);
	rotationAdd3DSound(30002, 30501, 1, 1, 20, 75, 347.0, 6);
	rotationAdd3DSound(30003, 30501, 1, 1, 20, 85, 158.0, 3);
	rotationAdd3DSound(30004, 30501, 1, 1, 20, 70, 142.0, 2);
	rotationAdd3DSound(30011, 30513, 1, 1, 40, 90, 56.0, 10);
	varDefineByte(30009, 2);
	varDefineByte(30010, 1);
	varDefineByte(30011, 3);
	varDefineByte(30012, 4);
	varDefineByte(30013, 5);
	varDefineByte(30014, 6);
	varDefineByte(30015, 7);
	varDefineByte(30016, 25);
	varDefineByte(30017, 0);
	varDefineByte(30050, 0);
	varDefineByte(30051, 0);
	varDefineByte(30052, 0);
	varDefineByte(30053, 0);
	varDefineByte(30054, 0);
	varDefineByte(30055, 0);
	varDefineByte(30019, 0);
	varDefineByte(30020, 0);
	varDefineByte(30021, 0);
	varDefineByte(30022, 0);
	varDefineByte(30023, 0);
	varDefineByte(30024, 0);
	varDefineByte(30025, 0);
	varDefineByte(30026, 0);
	varDefineByte(30027, 0);
	varDefineByte(30028, 0);
	varDefineByte(30029, 0);
	varDefineByte(30030, 0);
	varDefineByte(30031, 0);
	varDefineByte(30032, 0);
	varDefineByte(30033, 0);
	varDefineByte(30038, 0);
	varDefineByte(30034, 0);
	varDefineByte(30035, 0);
	varDefineByte(30036, 0);
	varDefineByte(30037, 0);
	varDefineByte(30039, 0);
	varDefineByte(30040, 0);
	varDefineByte(30041, 0);
	varDefineByte(30042, 0);
	varDefineByte(30043, 0);
	varDefineByte(30044, 0);
	varDefineByte(30045, 0);
	varDefineByte(30047, 0);
	varDefineByte(30056, 1);
	varDefineByte(30060, 0);
	varDefineByte(30061, 0);
	varDefineByte(30062, 0);
	varDefineByte(30063, 0);
	varDefineByte(30064, 0);
	varDefineByte(30065, 0);
	varDefineByte(30070, 0);
	varDefineByte(30071, 0);
	varDefineByte(30072, 0);
	varDefineByte(30066, 0);
	varDefineByte(30067, 0);
	varDefineByte(30068, 0);
	varDefineByte(30069, 1);
	varDefineByte(30073, 0);
	varDefineByte(30074, 0);
	varDefineByte(30075, 0);
	varDefineByte(30076, 0);
	varDefineByte(30077, 0);
	varDefineByte(30078, 0);
	varDefineByte(30201, 0);
	varDefineByte(30203, 0);
	varDefineByte(30204, 0);
	varDefineByte(30205, 0);
	varDefineByte(30206, 0);
	varDefineByte(30207, 0);
	varDefineByte(30208, 0);
	varDefineByte(30209, 0);
	varDefineByte(30210, 0);
	varDefineByte(30211, 0);
	varDefineFloat(30046, -10.0);
	varDefineFloat(30042, 1.0);
}

void ApplicationRing::initZoneRO() {
	setZoneAndEnableBag(kZoneRO);

	puzzleAdd(kPuzzle40010);
	puzzleAddBackgroundImage(kPuzzle40010, "ROS00N01P01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle40011);
	puzzleAddBackgroundImage(kPuzzle40011, "ROS00N01P01L02.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle40013);
	puzzleAddBackgroundImage(kPuzzle40013, "ROS00N01P01L01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle40012);
	puzzleAddBackgroundImage(kPuzzle40012, "ROS00N01P01L03.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle40060);
	puzzleAddBackgroundImage(kPuzzle40060, "ROS00N06P01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle40100);
	puzzleAddBackgroundImage(kPuzzle40100, "ROS00N01A01_A.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle40101);
	puzzleAddBackgroundImage(kPuzzle40101, "ROS00N01A01_B.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle40102);
	puzzleAddBackgroundImage(kPuzzle40102, "ROS00N01A01_C.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle40103);
	puzzleAddBackgroundImage(kPuzzle40103, "ROS00N01A01_D.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle40104);
	puzzleAddBackgroundImage(kPuzzle40104, "RO_ERDA.bmp", 0, 16, 1);
	rotationAdd(40000, "ROS00N01", 0, 1);
	rotationAdd(40001, "ROS00N02", 0, 1);
	rotationAdd(40002, "ROS00N03", 0, 1);
	rotationAdd(40003, "ROS00N04", 0, 1);
	rotationAdd(40004, "ROS00N05", 0, 1);
	rotationAdd(40005, "ROS00N06", 0, 0);
	rotationAddMovabilityToRotation(40000, 40001, "1789", 300, -250, 600, 300, 1, 53, 0);
	rotationSetMovabilityToRotation(40000, 0, 0, 0, 1118529126, 0, 0, 0, 0, 1118529126);
	rotationAddMovabilityToPuzzle(40000, kPuzzle40010, "", 3192, -104, 3378, 90, 1, 52, 0);
	rotationSetMovabilityToPuzzle(40000, 1, 1133576192, 0, 1118437376, 0, 0);
	rotationAddMovabilityToRotation(40001, 40004, "1790", 300, -250, 600, 300, 1, 53, 0);
	rotationSetMovabilityToRotation(40001, 0, 0, 0, 1118529126, 0, 0, 0, 0, 1118529126);
	rotationAddMovabilityToRotation(40001, 40003, "1791", 1200, -250, 1500, 300, 1, 53, 0);
	rotationSetMovabilityToRotation(40001, 1, 1119092736, 0, 1118529126, 0, 0, 1119092736, 0, 1118529126);
	rotationAddMovabilityToRotation(40001, 40000, "1792", 2100, -250, 2400, 300, 1, 53, 0);
	rotationSetMovabilityToRotation(40001, 2, 1127481344, 0, 1118529126, 0, 0, 1127481344, 0, 1118529126);
	rotationAddMovabilityToRotation(40001, 40002, "1793", 3000, -250, 3300, 300, 1, 53, 0);
	rotationSetMovabilityToRotation(40001, 3, 1132920832, 0, 1118529126, 0, 0, 1132920832, 0, 1118529126);
	rotationAddMovabilityToRotation(40002, 40001, "1794", 1200, -250, 1500, 300, 1, 53, 0);
	rotationSetMovabilityToRotation(40002, 0, 1119092736, 0, 1118529126, 0, 0, 1119092736, 0, 1118529126);
	rotationAddMovabilityToRotation(40003, 40001, "1795", 3000, -250, 3300, 300, 1, 53, 0);
	rotationSetMovabilityToRotation(40003, 0, 1132920832, 0, 1118529126, 0, 0, 1132920832, 0, 1118529126);
	rotationAddMovabilityToRotation(40004, 40005, "1796", 300, -250, 600, 300, 0, 53, 0);
	rotationSetMovabilityToRotation(40004, 0, 0, 0, 1118529126, 0, 0, 0, 0, 1118529126);
	rotationAddMovabilityToRotation(40004, 40001, "1797", 2100, -250, 2400, 300, 1, 53, 0);
	rotationSetMovabilityToRotation(40004, 1, 1127481344, 0, 1118529126, 0, 0, 1127481344, 0, 1118529126);
	rotationAddMovabilityToRotation(40005, 40004, "1798", 2100, -250, 2400, 300, 1, 53, 0);
	rotationSetMovabilityToRotation(40005, 0, 1127481344, 0, 1118529126, 0, 0, 1127481344, 0, 1118529126);
	rotationAddMovabilityToPuzzle(40005, kPuzzle40060, "", 246, -90, 715, 376, 1, 52, 0);
	rotationSetMovabilityToPuzzle(40005, 1, 0, 1102053376, 1118529126, 0, 0);
	puzzleAddMovabilityToRotation(kPuzzle40010, 40000, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle40010, 0, 1133477888, 1092616192, 1118529126);
	puzzleAddMovabilityToRotation(kPuzzle40011, 40000, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle40011, 0, 1133477888, 1092616192, 1118529126);
	puzzleAddMovabilityToRotation(kPuzzle40012, 40000, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle40012, 0, 1133477888, 1092616192, 1118529126);
	puzzleAddMovabilityToRotation(kPuzzle40013, 40000, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle40013, 0, 1133477888, 1092616192, 1118529126);
	puzzleAddMovabilityToRotation(kPuzzle40060, 40005, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle40060, 0, 0, 1102053376, 1118529126);
	objectAdd(kObjectFirePower, "Fire Power", "RO_Fire_", 1);
	objectAddBagAnimation(kObjectFirePower, 1, 3, 13, 1095237632, 4);
	objectSetActiveCursor(kObjectFirePower, 22, 22, 13, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectFirePower, 22, 22, 1, 4, 1065353216, 4, 4);
	objectAdd(kObjectRing, "Ring", "RO_Ring", 1);
	objectAddBagAnimation(kObjectRing, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectRing, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectRing, 22, 22, 1, 4, 1065353216, 4, 4);
	objectAdd(kObjectCrown, "Crown", "RO_Crown", 1);
	objectAddBagAnimation(kObjectCrown, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectCrown, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectCrown, 22, 22, 1, 4, 1065353216, 4, 4);
	objectAdd(kObject40010, "The Egg", "RO_Egg", 1);
	objectAddPuzzleAccessibility(kObject40010, kPuzzle40010, Common::Rect(200, 80, 440, 350), 1, 52, 0);
	objectAddPuzzleAccessibility(kObject40010, kPuzzle40011, Common::Rect(200, 80, 440, 350), 0, 52, 1);
	objectAddPuzzleAccessibility(kObject40010, kPuzzle40012, Common::Rect(219, 189, 430, 352), 1, 52, 2);
	objectAddPresentation(kObject40010);
	objectPresentationAddAnimationToPuzzle(kObject40010, 0, kPuzzle40012, "ROS00N01P01s03", 0, 217, 192, 1, 1000, 69, 1103626240, 4);
	objectAddPresentation(kObject40010);
	objectPresentationAddAnimationToPuzzle(kObject40010, 1, kPuzzle40012, "ROS00N01P01s04", 0, 211, 180, 1, 1000, 69, 1103626240, 4);
	objectAdd(kObject40203, "", "", 1);
	objectAddPresentation(kObject40203);
	objectPresentationAddImageToRotation(kObject40203, 0, 40000, 0);
	objectAddPresentation(kObject40203);
	objectPresentationAddImageToRotation(kObject40203, 1, 40001, 0);
	objectAddPresentation(kObject40203);
	objectPresentationAddImageToRotation(kObject40203, 2, 40002, 0);
	objectAddPresentation(kObject40203);
	objectPresentationAddImageToRotation(kObject40203, 3, 40003, 0);
	objectAddPresentation(kObject40203);
	objectPresentationAddImageToRotation(kObject40203, 4, 40004, 0);
	objectAdd(kObject40101, "", "NI_HandSel", 4);
	objectSetActiveDrawCursor(kObject40101, 15, 15, 0, 3, 0, 0, 3);
	objectSetPassiveDrawCursor(kObject40101, 15, 15, 0, 3, 0, 0, 3);

	for (uint32 i = 0; i < 99; i++) {
		objectAddPresentation(kObject40101);
		objectPresentationAddImageToPuzzle(kObject40101, i, kPuzzle40013, Common::String::format("ROS00N01P01S02CF1.%04d.bmp", i + 1), 232, 203, 1, 1, 1000);
	}

	objectAdd(kObject40102, "", "NI_HandSel", 4);
	objectSetActiveDrawCursor(kObject40102, 15, 15, 0, 3, 0, 0, 3);
	objectSetPassiveDrawCursor(kObject40102, 15, 15, 0, 3, 0, 0, 3);

	for (uint32 i = 0; i < 99; i++) {
		objectAddPresentation(kObject40102);
		objectPresentationAddImageToPuzzle(kObject40102, i, kPuzzle40013, Common::String::format("ROS00N01P01S02CF2.%04d.bmp", i + 1), 265, 204, 1, 1, 1000);
	}

	objectAdd(kObject40103, "", "NI_HandSel", 4);
	objectSetActiveDrawCursor(kObject40103, 15, 15, 0, 3, 0, 0, 3);
	objectSetPassiveDrawCursor(kObject40103, 15, 15, 0, 3, 0, 0, 3);

	for (uint32 i = 0; i < 99; i++) {
		objectAddPresentation(kObject40103);
		objectPresentationAddImageToPuzzle(kObject40103, i, kPuzzle40013, Common::String::format("ROS00N01P01S02CF3.%04d.bmp", i + 1), 295, 203, 1, 1, 1000);
	}

	objectAdd(kObject40104, "", "NI_HandSel", 4);
	objectSetActiveDrawCursor(kObject40104, 15, 15, 0, 3, 0, 0, 3);
	objectSetPassiveDrawCursor(kObject40104, 15, 15, 0, 3, 0, 0, 3);

	for (uint32 i = 0; i < 99; i++) {
		objectAddPresentation(kObject40104);
		objectPresentationAddImageToPuzzle(kObject40104, i, kPuzzle40013, Common::String::format("ROS00N01P01S02CF4.%04d.bmp", i + 1), 326, 203, 1, 1, 1000);
	}

	objectAdd(kObject40105, "", "NI_HandSel", 4);
	objectSetActiveDrawCursor(kObject40105, 15, 15, 0, 3, 0, 0, 3);
	objectSetPassiveDrawCursor(kObject40105, 15, 15, 0, 3, 0, 0, 3);

	for (uint32 i = 0; i < 99; i++) {
		objectAddPresentation(kObject40105);
		objectPresentationAddImageToPuzzle(kObject40105, i, kPuzzle40013, Common::String::format("ROS00N01P01S02CF5.%04d.bmp", i + 1), 356, 204, 1, 1, 1000);
	}

	objectAddPuzzleAccessibility(kObject40101, kPuzzle40013, Common::Rect(0, 0, 0, 0), 1, 52, 0);
	objectAddPuzzleAccessibility(kObject40101, kPuzzle40013, Common::Rect(0, 0, 0, 0), 1, 52, 1);
	objectAddPuzzleAccessibility(kObject40101, kPuzzle40013, Common::Rect(298, 202, 328, 245), 1, 52, 2);
	objectAddPuzzleAccessibility(kObject40101, kPuzzle40013, Common::Rect(329, 202, 359, 245), 1, 52, 3);
	objectAddPuzzleAccessibility(kObject40101, kPuzzle40013, Common::Rect(360, 202, 390, 245), 1, 52, 4);
	objectAdd(kObject40011, "", "", 1);
	objectAddPresentation(kObject40011);

	for (uint32 i = 0; i < 7; i++) {
		for (uint32 j = 0; j < 7; j++) {
			varDefineByte(40501 + j * 10 + i, 99);
		}
	}

	objectAddPuzzleAccessibility(kObject40011, kPuzzle40011, Common::Rect(263, 71, 319, 127), 1, 52, 12);
	varSetByte(40513, 0);

	for (uint32 i = 0; i < 3; i++) {
		for (uint32 j = 0; j < 14; j++) {
			objectPresentationAddImageToPuzzle(kObject40011, i, kPuzzle40011, Common::String::format("L01T%d%d.bmp", i, j), 207 + j * 56, 127 + 56 * i, 1, 1, 1000);
			objectAddPuzzleAccessibility(kObject40011, kPuzzle40011, Common::Rect(207 + j * 56, 127 + 56 * i, 207 + (j + 1) * 56, 127 + 56 * (i + 1)), 1, 52, (21 + i * 10 + j));
			varSetByte((21 + i * 10 + j) + 40501, (j + 1) + 10 * i);
		}
	}

	objectPresentationShow(kObject40011, 0);
	objectPresentationShow(kObject40011, 1);
	objectPresentationShow(kObject40011, 2);
	objectPresentationShow(kObject40011, 3);
	objectAdd(kObject40060, "", "NI_HandSel", 4);
	objectSetActiveDrawCursor(kObject40060, 15, 15, 0, 3, 0, 0, 3);
	objectSetPassiveDrawCursor(kObject40060, 15, 15, 0, 3, 0, 0, 3);

	for (uint32 i = 0; i < 73; i++) {
		objectAddPresentation(kObject40060);
		objectPresentationAddImageToPuzzle(kObject40060, i, kPuzzle40060, Common::String::format("ROS00N06P01S02.%04d.bmp", i + 1), 67, 34, 1, 1, 1000);
	}

	objectAddPuzzleAccessibility(kObject40060, kPuzzle40060, Common::Rect(531, 112, 576, 135), 1, 52, 0);
	objectAddPuzzleAccessibility(kObject40060, kPuzzle40060, Common::Rect(466, 110, 511, 135), 0, 52, 1);
	objectAddPuzzleAccessibility(kObject40060, kPuzzle40060, Common::Rect(404, 110, 449, 135), 0, 52, 2);
	objectAddPuzzleAccessibility(kObject40060, kPuzzle40060, Common::Rect(343, 110, 387, 135), 0, 52, 3);
	objectAddPuzzleAccessibility(kObject40060, kPuzzle40060, Common::Rect(282, 110, 325, 135), 0, 52, 4);
	objectAddPuzzleAccessibility(kObject40060, kPuzzle40060, Common::Rect(218, 110, 263, 135), 0, 52, 5);
	objectAddPuzzleAccessibility(kObject40060, kPuzzle40060, Common::Rect(157, 110, 201, 135), 0, 52, 6);
	objectAddPuzzleAccessibility(kObject40060, kPuzzle40060, Common::Rect(93, 110, 140, 135), 0, 52, 7);
	objectAdd(kObject40201, "", "", 1);
	objectAddPuzzleAccessibility(kObject40201, kPuzzle40060, Common::Rect(461, 63, 488, 103), 1, 52, 0);
	objectAddPuzzleAccessibility(kObject40201, kPuzzle40060, Common::Rect(409, 63, 436, 103), 1, 52, 1);
	objectAddPuzzleAccessibility(kObject40201, kPuzzle40060, Common::Rect(357, 63, 383, 103), 1, 52, 2);
	objectAddPuzzleAccessibility(kObject40201, kPuzzle40060, Common::Rect(307, 63, 331, 103), 1, 52, 3);
	objectAddPuzzleAccessibility(kObject40201, kPuzzle40060, Common::Rect(253, 63, 280, 103), 1, 52, 4);
	objectAddPuzzleAccessibility(kObject40201, kPuzzle40060, Common::Rect(200, 63, 228, 103), 1, 52, 5);
	objectAddPuzzleAccessibility(kObject40201, kPuzzle40060, Common::Rect(150, 63, 176, 103), 1, 52, 6);
	objectAddPresentation(kObject40201);
	objectPresentationAddAnimationToPuzzle(kObject40201, 0, kPuzzle40060, "ROS00N06P01S03", 0, 457, 17, 1, 1000, 70, 1103626240, 6);
	objectPresentationSetAnimationOnPuzzle(kObject40201, 0, 0, 40100);
	objectAddPresentation(kObject40201);
	objectPresentationAddAnimationToPuzzle(kObject40201, 1, kPuzzle40060, "ROS00N06P01S04", 0, 412, 19, 1, 1000, 70, 1103626240, 6);
	objectPresentationSetAnimationOnPuzzle(kObject40201, 1, 0, 40101);
	objectAddPresentation(kObject40201);
	objectPresentationAddAnimationToPuzzle(kObject40201, 2, kPuzzle40060, "ROS00N06P01S05", 0, 359, 20, 1, 1000, 70, 1103626240, 6);
	objectPresentationSetAnimationOnPuzzle(kObject40201, 2, 0, 40102);
	objectAddPresentation(kObject40201);
	objectPresentationAddAnimationToPuzzle(kObject40201, 3, kPuzzle40060, "ROS00N06P01S06", 0, 298, 23, 1, 1000, 70, 1103626240, 6);
	objectPresentationSetAnimationOnPuzzle(kObject40201, 3, 0, 40103);
	objectAddPresentation(kObject40201);
	objectPresentationAddAnimationToPuzzle(kObject40201, 4, kPuzzle40060, "ROS00N06P01S07", 0, 239, 20, 1, 1000, 70, 1103626240, 6);
	objectPresentationSetAnimationOnPuzzle(kObject40201, 4, 0, 40104);
	objectAddPresentation(kObject40201);
	objectPresentationAddAnimationToPuzzle(kObject40201, 5, kPuzzle40060, "ROS00N06P01S08", 0, 173, 22, 1, 1000, 70, 1103626240, 6);
	objectPresentationSetAnimationOnPuzzle(kObject40201, 5, 0, 40105);
	objectAddPresentation(kObject40201);
	objectPresentationAddAnimationToPuzzle(kObject40201, 6, kPuzzle40060, "ROS00N06P01S09", 0, 109, 22, 1, 1000, 70, 1103626240, 6);
	objectPresentationSetAnimationOnPuzzle(kObject40201, 6, 0, 40106);
	objectAddPresentation(kObject40201);
	objectPresentationAddAnimationToPuzzle(kObject40201, 7, kPuzzle40060, "ROS00N06P02S01", 0, 454, 61, 1, 1000, 26, 1103626240, 6);
	objectPresentationSetAnimationOnPuzzle(kObject40201, 7, 0, 40201);
	objectAddPresentation(kObject40201);
	objectPresentationAddAnimationToPuzzle(kObject40201, 8, kPuzzle40060, "ROS00N06P02S02", 0, 400, 62, 1, 1000, 26, 1103626240, 6);
	objectPresentationSetAnimationOnPuzzle(kObject40201, 8, 0, 40202);
	objectAddPresentation(kObject40201);
	objectPresentationAddAnimationToPuzzle(kObject40201, 9, kPuzzle40060, "ROS00N06P02S03", 0, 349, 61, 1, 1000, 26, 1103626240, 6);
	objectPresentationSetAnimationOnPuzzle(kObject40201, 9, 0, 40203);
	objectAddPresentation(kObject40201);
	objectPresentationAddAnimationToPuzzle(kObject40201, 10, kPuzzle40060, "ROS00N06P02S04", 0, 297, 63, 1, 1000, 26, 1103626240, 6);
	objectPresentationSetAnimationOnPuzzle(kObject40201, 10, 0, 40204);
	objectAddPresentation(kObject40201);
	objectPresentationAddAnimationToPuzzle(kObject40201, 11, kPuzzle40060, "ROS00N06P02S05", 0, 247, 62, 1, 1000, 26, 1103626240, 6);
	objectPresentationSetAnimationOnPuzzle(kObject40201, 11, 0, 40205);
	objectAddPresentation(kObject40201);
	objectPresentationAddAnimationToPuzzle(kObject40201, 12, kPuzzle40060, "ROS00N06P02S06", 0, 195, 63, 1, 1000, 26, 1103626240, 6);
	objectPresentationSetAnimationOnPuzzle(kObject40201, 12, 0, 40206);
	objectAddPresentation(kObject40201);
	objectPresentationAddAnimationToPuzzle(kObject40201, 13, kPuzzle40060, "ROS00N06P02S07", 0, 142, 62, 1, 1000, 26, 1103626240, 6);
	objectPresentationSetAnimationOnPuzzle(kObject40201, 13, 0, 40207);
	objectAdd(kObject40202, "", "", 3);
	objectAddPresentation(kObject40202);
	objectPresentationAddImageToPuzzle(kObject40202, 0, kPuzzle40060, "ROS00N06P01L01.0001.bmp", 0, 16, 1, 1, 1000);
	objectAddPresentation(kObject40202);
	objectPresentationAddImageToPuzzle(kObject40202, 1, kPuzzle40060, "ROS00N06P01L01.0002.bmp", 57, 360, 1, 1, 1000);
	objectAddPresentation(kObject40202);
	objectPresentationAddImageToPuzzle(kObject40202, 2, kPuzzle40060, "ROS00N06P01L01.0003.bmp", 100, 353, 1, 1, 1000);
	objectAddPresentation(kObject40202);
	objectPresentationAddImageToPuzzle(kObject40202, 3, kPuzzle40060, "ROS00N06P01L01.0004.bmp", 127, 353, 1, 1, 1000);
	objectAddPresentation(kObject40202);
	objectPresentationAddImageToPuzzle(kObject40202, 4, kPuzzle40060, "ROS00N06P01L01.0005.bmp", 165, 355, 1, 1, 1000);
	objectAddPresentation(kObject40202);
	objectPresentationAddImageToPuzzle(kObject40202, 5, kPuzzle40060, "ROS00N06P01L01.0006.bmp", 197, 355, 1, 1, 1000);
	objectAddPresentation(kObject40202);
	objectPresentationAddImageToPuzzle(kObject40202, 6, kPuzzle40060, "ROS00N06P01L01.0007.bmp", 234, 352, 1, 1, 1000);
	objectAddPresentation(kObject40202);
	objectPresentationAddImageToPuzzle(kObject40202, 7, kPuzzle40060, "ROS00N06P01L01.0008.bmp", 260, 342, 1, 1, 1000);
	objectAddPresentation(kObject40202);
	objectPresentationAddImageToPuzzle(kObject40202, 8, kPuzzle40060, "ROS00N06P01L01.0009.bmp", 299, 331, 1, 1, 1000);
	objectAddPresentation(kObject40202);
	objectPresentationAddImageToPuzzle(kObject40202, 9, kPuzzle40060, "ROS00N06P01L01.0010.bmp", 330, 349, 1, 1, 1000);
	objectAddPresentation(kObject40202);
	objectPresentationAddImageToPuzzle(kObject40202, 10, kPuzzle40060, "ROS00N06P01L01.0011.bmp", 364, 335, 1, 1, 1000);
	objectAddPresentation(kObject40202);
	objectPresentationAddImageToPuzzle(kObject40202, 11, kPuzzle40060, "ROS00N06P01L01.0012.bmp", 393, 354, 1, 1, 1000);
	objectAddPresentation(kObject40202);
	objectPresentationAddImageToPuzzle(kObject40202, 12, kPuzzle40060, "ROS00N06P01L01.0013.bmp", 421, 353, 1, 1, 1000);
	objectAddPresentation(kObject40202);
	objectPresentationAddImageToPuzzle(kObject40202, 13, kPuzzle40060, "ROS00N06P01L01.0014.bmp", 454, 358, 1, 1, 1000);
	objectAddPresentation(kObject40202);
	objectPresentationAddImageToPuzzle(kObject40202, 14, kPuzzle40060, "ROS00N06P01L01.0015.bmp", 484, 345, 1, 1, 1000);
	objectAddPresentation(kObject40202);
	objectPresentationAddImageToPuzzle(kObject40202, 15, kPuzzle40060, "ROS00N06P01L01.0016.bmp", 516, 349, 1, 1, 1000);
	objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(67, 384, 100, 438), 0, 52, 0);
	objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(101, 384, 133, 438), 0, 52, 1);
	objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(136, 384, 164, 438), 0, 52, 2);
	objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(169, 384, 200, 438), 0, 52, 3);
	objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(203, 384, 234, 438), 0, 52, 4);
	objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(236, 384, 267, 438), 0, 52, 5);
	objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(269, 384, 299, 438), 0, 52, 6);
	objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(303, 384, 335, 438), 0, 52, 7);
	objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(336, 384, 368, 438), 0, 52, 8);
	objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(368, 384, 400, 438), 0, 52, 9);
	objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(402, 384, 436, 438), 0, 52, 10);
	objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(435, 384, 466, 438), 0, 52, 11);
	objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(466, 384, 499, 438), 0, 52, 12);
	objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(503, 384, 534, 438), 0, 52, 13);
	objectAddPuzzleAccessibility(kObject40202, kPuzzle40060, Common::Rect(535, 384, 571, 438), 0, 52, 14);
	objectAdd(kObject40300, "", "", 1);
	objectAddPresentation(kObject40300);
	objectPresentationAddAnimationToPuzzle(kObject40300, 0, kPuzzle40101, "ROS00N01A01_B", 0, 301, 217, 1, 1000, 19, 1095237632, 4);
	soundAdd(40001, 2, "1799.was", _configuration.ambientMusic.loadFrom, 2, _configuration.ambientMusic.soundChunck);
	soundAdd(40604, 2, "1800.was", _configuration.ambientMusic.loadFrom, 2, _configuration.ambientMusic.soundChunck);
	rotationAddAmbientSound(40000, 40001, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(40001, 40001, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(40002, 40001, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(40003, 40001, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(40004, 40001, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(40005, 40001, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle40010, 40001, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle40011, 40001, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle40012, 40001, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle40013, 40001, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle40100, 40001, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle40101, 40001, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle40102, 40001, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle40103, 40001, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle40104, 40001, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(40000, 40604, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(40001, 40604, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(40002, 40604, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(40003, 40604, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(40004, 40604, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(40005, 40604, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle40060, 40604, 100, 0, 1, 1, 10);
	rotationSetAmbiantSoundOff(40000, 40604);
	rotationSetAmbiantSoundOff(40001, 40604);
	rotationSetAmbiantSoundOff(40002, 40604);
	rotationSetAmbiantSoundOff(40003, 40604);
	rotationSetAmbiantSoundOff(40004, 40604);
	rotationSetAmbiantSoundOff(40005, 40604);
	puzzleSetAmbientSoundOff(kPuzzle40060, 40604);
	soundAdd(40002, 3, "1802.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(40003, 3, "1803.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(40102, 4, "1318.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundSetVolume(40102, 80);
	soundAdd(40103, 4, "1804.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundSetVolume(40103, 80);
	soundAdd(40500, 4, "1805.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(40501, 4, "1806.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(40502, 4, "1807.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(40503, 4, "1808.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(40504, 4, "1809.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(40505, 4, "1810.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(40506, 4, "1811.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(40507, 4, "1805.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(40508, 4, "1806.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(40509, 4, "1807.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(40510, 4, "1811.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(40511, 4, "1812.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(40512, 4, "1810.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(40513, 4, "1809.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(40514, 4, "1808.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(40600, 4, "1813.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(40601, 4, "1814.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(40602, 4, "1815.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(40603, 4, "1816.was", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(40400, 5, "1817.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40200, 5, "1818.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40201, 5, "1819.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40202, 5, "1820.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40203, 5, "1821.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40204, 5, "1822.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40205, 5, "1823.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40206, 5, "1824.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40300, 5, "1825.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40301, 5, "1826.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40302, 5, "1827.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40303, 5, "1828.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40304, 5, "1829.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40305, 5, "1830.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40306, 5, "1831.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40700, 5, "1832.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40701, 5, "1833.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40706, 5, "1834.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40707, 5, "1835.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40708, 5, "1836.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40709, 5, "1837.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40710, 5, "1838.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40711, 5, "1839.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40702, 5, "1840.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40703, 5, "1841.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40704, 5, "1842.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(40705, 5, "1843.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	rotationAdd3DSound(40000, 40002, 1, 1, 10, 87, 290.0, 10);
	rotationAdd3DSound(40001, 40002, 1, 1, 10, 85, 194.0, 5);
	rotationAdd3DSound(40002, 40002, 1, 1, 10, 83, 141.0, 5);
	rotationAdd3DSound(40003, 40002, 1, 1, 10, 83, 233.0, 5);
	rotationAdd3DSound(40004, 40002, 1, 1, 10, 80, 189.0, 2);
	rotationAdd3DSound(40005, 40002, 1, 1, 10, 77, 187.0, 2);
	puzzleAdd3DSound(kPuzzle40010, 40002, 1, 1, 10, 93, 290.0, 2);
	puzzleAdd3DSound(kPuzzle40011, 40002, 1, 1, 10, 95, 290.0, 2);
	puzzleAdd3DSound(kPuzzle40012, 40002, 1, 1, 10, 100, 290.0, 2);
	puzzleAdd3DSound(kPuzzle40013, 40002, 1, 1, 10, 97, 290.0, 2);
	puzzleAdd3DSound(kPuzzle40100, 40002, 1, 1, 10, 90, 290.0, 2);
	puzzleAdd3DSound(kPuzzle40101, 40002, 1, 1, 10, 90, 290.0, 2);
	puzzleAdd3DSound(kPuzzle40102, 40002, 1, 1, 10, 90, 290.0, 2);
	puzzleAdd3DSound(kPuzzle40103, 40002, 1, 1, 10, 90, 290.0, 2);
	puzzleAdd3DSound(kPuzzle40104, 40002, 1, 1, 10, 90, 290.0, 2);
	rotationAdd3DSound(40000, 40003, 1, 1, 10, 70, 0.0, 2);
	rotationAdd3DSound(40001, 40003, 1, 1, 10, 75, 0.0, 5);
	rotationAdd3DSound(40002, 40003, 1, 1, 10, 72, 25.0, 5);
	rotationAdd3DSound(40003, 40003, 1, 1, 10, 72, 333.0, 5);
	rotationAdd3DSound(40004, 40003, 1, 1, 10, 76, 0.0, 10);
	rotationAdd3DSound(40005, 40003, 1, 1, 10, 82, 0.0, 10);
	varDefineByte(40000, 0);
	varDefineByte(40601, 0);
	varDefineByte(40602, 0);
	varDefineByte(40603, 0);
	varDefineByte(40604, 0);
	varDefineByte(40605, 0);
	varDefineByte(40701, 0);
	varDefineByte(40702, 0);
	varDefineByte(40703, 0);
	varDefineByte(40801, 0);
	varDefineByte(40200, 1);
	varDefineByte(40201, 0);
	varDefineByte(40202, 1);
	varDefineByte(40203, 0);
	varDefineByte(40204, 0);
	varDefineByte(40205, 1);
	varDefineByte(40206, 0);
	varDefineByte(40802, 0);
	varDefineByte(40803, 0);
	varDefineByte(40804, 0);
	varDefineByte(40805, 56);
	varDefineByte(40806, 0);
	varDefineByte(40807, 0);
	varDefineByte(40901, 26);
	varDefineByte(40902, 26);
	varDefineByte(40903, 26);
	varDefineByte(40904, 26);
	varDefineByte(40905, 26);
	varDefineByte(40906, 26);
	varDefineByte(40907, 26);
	varDefineByte(40911, 70);
	varDefineByte(40912, 70);
	varDefineByte(40913, 70);
	varDefineByte(40914, 70);
	varDefineByte(40915, 70);
	varDefineByte(40916, 70);
	varDefineByte(40917, 70);
	varDefineString(40901, "0000000");
	varDefineString(40902, "00000000");
}

void ApplicationRing::initZoneWA() {
	setZoneAndEnableBag(kZoneWA);

	puzzleAdd(kPuzzle51001);
	puzzleAddBackgroundImage(kPuzzle51001, "TR_WA_A06.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle51002);
	puzzleAddBackgroundImage(kPuzzle51002, "TR_WA_A09.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle51003);
	puzzleAddBackgroundImage(kPuzzle51003, "TR_WA_AS.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle51004);
	puzzleAddBackgroundImage(kPuzzle51004, "TR_WA_BS.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle51005);
	puzzleAddBackgroundImage(kPuzzle51005, "TR_WA_CS.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle51006);
	puzzleAddBackgroundImage(kPuzzle51006, "TR_WA_DS.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle51007);
	puzzleAddBackgroundImage(kPuzzle51007, "TR_WA_ES.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle51008);
	puzzleAddBackgroundImage(kPuzzle51008, "TR_WA_FS.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle51009);
	puzzleAddBackgroundImage(kPuzzle51009, "TR_WA_GS.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle51010);
	puzzleAddBackgroundImage(kPuzzle51010, "TR_WA_HS.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle51011);
	puzzleAddBackgroundImage(kPuzzle51011, "TR_WA_IS.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle51012);
	puzzleAddBackgroundImage(kPuzzle51012, "TR_WA_JS.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle51013);
	puzzleAddBackgroundImage(kPuzzle51013, "TR_WA_KS.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle50100);
	puzzleAddBackgroundImage(kPuzzle50100, "WAS01N08P01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle50601);
	puzzleAddBackgroundImage(kPuzzle50601, "WAS06N01P01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle50701);
	puzzleAddBackgroundImage(kPuzzle50701, "WAS07N01P01S01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle50702);
	puzzleAddBackgroundImage(kPuzzle50702, "WAS07N01P01S02.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle50703);
	puzzleAddBackgroundImage(kPuzzle50703, "WAS07N01P01S03.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle50602);
	puzzleAddBackgroundImage(kPuzzle50602, "WAS06N01P02.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle50001);
	puzzleAddBackgroundImage(kPuzzle50001, "WAS00N01P01L01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle50002);
	puzzleAddBackgroundImage(kPuzzle50002, "WAS00N01P01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle50303);
	puzzleAddBackgroundImage(kPuzzle50303, "WAS03N03P01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle50304);
	puzzleAddBackgroundImage(kPuzzle50304, "WAS03N04P01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle50400);
	puzzleAddBackgroundImage(kPuzzle50400, "WAS04N02P01L01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle50401);
	puzzleAddBackgroundImage(kPuzzle50401, "WAS04N02P02.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle50501);
	puzzleAddBackgroundImage(kPuzzle50501, "WAS05N02P01.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle50502);
	puzzleAddBackgroundImage(kPuzzle50502, "WAS05N02P02.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle50503);
	puzzleAddBackgroundImage(kPuzzle50503, "WAS05N02P03.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle50504);
	puzzleAddBackgroundImage(kPuzzle50504, "WAS05N02P04.bmp", 0, 16, 1);
	rotationAdd(50001, "WAS00N01", 0, 0);
	rotationAdd(50101, "WAS01N01", 0, 0);
	rotationAdd(50102, "WAS01N02", 0, 0);
	rotationAdd(50103, "WAS01N03", 0, 2);
	rotationAdd(50104, "WAS01N04", 0, 0);
	rotationAdd(50105, "WAS01N05", 0, 0);
	rotationAdd(50106, "WAS01N06", 0, 0);
	rotationAdd(50107, "WAS01N07", 0, 0);
	rotationAdd(50108, "WAS01N08", 0, 0);
	rotationAdd(50201, "WAS02N01", 0, 3);
	rotationAdd(50202, "WAS02N02", 0, 3);
	rotationAdd(50301, "WAS03N01", 0, 1);
	rotationAdd(50302, "WAS03N02", 0, 1);
	rotationAdd(50303, "WAS03N03", 0, 1);
	rotationAdd(50304, "WAS03N04", 0, 6);
	rotationSetComBufferLength(50304, 1400000);
	rotationAdd(50401, "WAS04N01", 0, 1);
	rotationAdd(50402, "WAS04N02", 0, 2);
	rotationAdd(50501, "WAS05N01", 0, 2);
	rotationAdd(50502, "WAS05N02", 0, 7);
	rotationAdd(50601, "WAS06N01", 0, 0);
	rotationAdd(50602, "WAS06N02", 0, 0);
	rotationAdd(50701, "WAS07N01", 0, 2);
	rotationSetComBufferLength(50701, 1600000);
	rotationAddMovabilityToRotation(50001, 50101, "1883", 3341, -249, 3599, 242, 1, 53, 0);
	rotationSetMovabilityToRotation(50001, 0, 1134297088, 1050253722, 1118476698, 0, 0, 1135673344, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50001, 50101, "1883", 0, -249, 145, 242, 1, 53, 0);
	rotationSetMovabilityToRotation(50001, 1, 1134297088, 1050253722, 1118476698, 0, 2, 1135673344, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50101, 50102, "1884", 284, -170, 525, 127, 1, 53, 0);
	rotationSetMovabilityToRotation(50101, 0, 1135771648, 1050253722, 1118476698, 0, 2, 1135771648, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50101, 50001, "1885", 1747, -202, 1961, 179, 1, 53, 0);
	rotationSetMovabilityToRotation(50101, 1, 1124859904, 1050253722, 1118476698, 0, 2, 1124859904, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50102, 50103, "1886", 33, -280, 377, 52, 1, 53, 0);
	rotationSetMovabilityToRotation(50102, 0, 1134854144, 1050253722, 1118476698, 0, 2, 1132331008, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50102, 50101, "1887", 2049, -165, 2378, 198, 1, 53, 0);
	rotationSetMovabilityToRotation(50102, 1, 1126891520, 1050253722, 1118476698, 0, 2, 1124990976, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50102, 50201, "1888", 1158, -263, 1524, 240, 1, 53, 0);
	rotationSetMovabilityToRotation(50102, 2, 1119092736, 1050253722, 1118476698, 0, 0, 1119092736, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50103, 50104, "1889", 2729, -247, 3072, 81, 1, 53, 0);
	rotationSetMovabilityToRotation(50103, 0, 1131216896, 1050253722, 1118476698, 0, 2, 1126432768, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50103, 50102, "1890", 1453, -76, 1738, 293, 1, 53, 0);
	rotationSetMovabilityToRotation(50103, 1, 1123287040, 1050253722, 1118476698, 0, 2, 1127088128, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50103, 50701, "1891", 2033, -111, 2416, 340, 1, 53, 0);
	rotationSetMovabilityToRotation(50103, 2, 1127481344, 1050253722, 1118476698, 0, 0, 1127481344, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50103, 50701, "", 2033, -111, 2416, 340, 0, 53, 0);
	rotationSetMovabilityToRotation(50103, 3, 1127481344, 1050253722, 1118476698, 0, 0, 1127481344, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50104, 50105, "1892", 1882, -291, 2163, 109, 1, 53, 0);
	rotationSetMovabilityToRotation(50104, 0, 1125384192, 1050253722, 1118476698, 0, 2, 1116471296, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50104, 50103, "1893", 446, -85, 782, 301, 1, 53, 0);
	rotationSetMovabilityToRotation(50104, 1, 1103626240, 1050253722, 1118476698, 0, 2, 1122631680, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50104, 50301, "1894", 3005, -310, 3350, 221, 1, 53, 0);
	rotationSetMovabilityToRotation(50104, 2, 1133051904, 1050253722, 1118476698, 0, 0, 1132920832, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50105, 50106, "1895", 1000, -242, 1311, 115, 1, 53, 0);
	rotationSetMovabilityToRotation(50105, 0, 1116209152, 1050253722, 1118476698, 0, 2, 1135050752, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50105, 50104, "1896", 3248, -80, 3510, 272, 1, 53, 0);
	rotationSetMovabilityToRotation(50105, 1, 1133707264, 1050253722, 1118476698, 0, 2, 1096810496, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50105, 50401, "1897", 2056, -268, 2425, 244, 1, 53, 0);
	rotationSetMovabilityToRotation(50105, 2, 1127481344, 1050253722, 1118476698, 0, 0, 1127481344, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50106, 50107, "1898", 82, -261, 355, 59, 1, 53, 0);
	rotationSetMovabilityToRotation(50106, 0, 1134886912, 1050253722, 1118476698, 0, 2, 1132068864, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50106, 50105, "1899", 2337, -87, 2612, 268, 1, 53, 0);
	rotationSetMovabilityToRotation(50106, 1, 1129185280, 1050253722, 1118476698, 0, 2, 1133740032, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50106, 50501, "1900", 1175, -313, 1546, 223, 1, 53, 0);
	rotationSetMovabilityToRotation(50106, 2, 1119092736, 1050253722, 1118476698, 0, 0, 1119092736, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50107, 50108, "1901", 2715, -132, 3090, 104, 1, 53, 0);
	rotationSetMovabilityToRotation(50107, 0, 1132068864, 1050253722, 1118476698, 0, 2, 1132068864, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50107, 50106, "1902", 1281, -81, 1546, 277, 1, 53, 0);
	rotationSetMovabilityToRotation(50107, 1, 1120403456, 1050253722, 1118476698, 0, 2, 1128792064, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50108, 50107, "1904", 987, -216, 1350, 129, 1, 53, 0);
	rotationSetMovabilityToRotation(50108, 0, 1116995584, 1050253722, 1118476698, 0, 2, 1116995584, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50201, 50102, "1905", 2768, -516, 3506, 516, 1, 53, 0);
	rotationSetMovabilityToRotation(50201, 0, 1132920832, 1050253722, 1118476698, 0, 0, 1132920832, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50201, 50202, "1906", 1146, -118, 1437, 172, 1, 53, 0);
	rotationSetMovabilityToRotation(50201, 1, 1119092736, 1050253722, 1118476698, 0, 0, 1119092736, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50202, 50201, "1907", 2898, -209, 3267, 132, 1, 53, 0);
	rotationSetMovabilityToRotation(50202, 0, 1132756992, 1050253722, 1118476698, 0, 2, 1132756992, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50301, 50104, "1908", 975, -520, 1699, 520, 1, 53, 0);
	rotationSetMovabilityToRotation(50301, 0, 1119092736, 1050253722, 1118476698, 0, 0, 1119092736, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50301, 50302, "1909", 2896, -146, 3529, 231, 1, 53, 0);
	rotationSetMovabilityToRotation(50301, 1, 1132986368, 1050253722, 1118476698, 0, 2, 1132986368, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50301, 50104, "1910", 975, -520, 1699, 520, 1, 53, 0);
	rotationSetMovabilityToRotation(50301, 2, 1119092736, 1050253722, 1118476698, 0, 0, 1119092736, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50301, 50302, "1911", 2896, -146, 3529, 231, 0, 53, 0);
	rotationSetMovabilityToRotation(50301, 3, 1132986368, 1050253722, 1118476698, 0, 2, 1132986368, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50302, 50301, "1912", 1225, -209, 1634, 143, 1, 53, 0);
	rotationSetMovabilityToRotation(50302, 0, 1120010240, 1050253722, 1118476698, 0, 2, 1120010240, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50302, 50303, "1913", 548, -237, 762, 195, 1, 53, 0);
	rotationSetMovabilityToRotation(50302, 1, 1102577664, 1050253722, 1118476698, 0, 2, 1132756992, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50302, 50304, "1914", 2112, -78, 2288, 137, 1, 53, 0);
	rotationSetMovabilityToRotation(50302, 2, 1126957056, 1050253722, 1118476698, 0, 2, 1126957056, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50302, 50301, "1915", 1225, -209, 1634, 143, 0, 53, 0);
	rotationSetMovabilityToRotation(50302, 3, 1120010240, 1050253722, 1118476698, 0, 2, 1120010240, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50302, 50303, "1916", 548, -237, 762, 195, 0, 53, 0);
	rotationSetMovabilityToRotation(50302, 4, 1102577664, 1050253722, 1118476698, 0, 2, 1132756992, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50302, 50304, "1917", 2112, -78, 2288, 137, 0, 53, 0);
	rotationSetMovabilityToRotation(50302, 5, 1126957056, 1050253722, 1118476698, 0, 2, 1126957056, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50303, 50302, "1918", 2221, 216, 2647, 518, 1, 53, 0);
	rotationSetMovabilityToRotation(50303, 0, 1128202240, 1050253722, 1118476698, 0, 2, 1128202240, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50303, 50302, "1919", 2221, 216, 2647, 518, 0, 53, 0);
	rotationSetMovabilityToRotation(50303, 1, 1128202240, 1050253722, 1118476698, 0, 2, 1128202240, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50304, 50302, "", 217, -87, 506, 301, 1, 53, 0);
	rotationSetMovabilityToRotation(50304, 0, 1135378432, 1050253722, 1118476698, 0, 2, 1135378432, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50304, 50302, "", 217, -87, 506, 301, 0, 53, 0);
	rotationSetMovabilityToRotation(50304, 0, 1135378432, 1050253722, 1118476698, 0, 2, 1135378432, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50401, 50105, "1920", 123, -510, 866, 510, 1, 53, 0);
	rotationSetMovabilityToRotation(50401, 0, 0, 1050253722, 1118476698, 0, 0, 0, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50401, 50402, "1921", 2148, -275, 2360, 106, 1, 53, 0);
	rotationSetMovabilityToRotation(50401, 1, 1127481344, 1050253722, 1118476698, 0, 2, 1127481344, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50401, 50402, "1922", 2148, -275, 2360, 106, 0, 53, 0);
	rotationSetMovabilityToRotation(50401, 2, 1127481344, 1050253722, 1118476698, 0, 2, 1127481344, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50402, 50401, "1923", 386, 122, 662, 331, 1, 53, 0);
	rotationSetMovabilityToRotation(50402, 0, 0, 1050253722, 1118476698, 0, 0, 0, 1050253722, 1118476698);
	rotationAddMovabilityToPuzzle(50402, kPuzzle50400, "1924", 2191, 296, 2445, 481, 1, 52, 0);
	rotationSetMovabilityToPuzzle(50402, 1, 1127481344, 1104150528, 1118476698, 0, 2);
	rotationAddMovabilityToPuzzle(50402, kPuzzle50401, "", 2169, 47, 2392, 200, 0, 52, 0);
	rotationSetMovabilityToPuzzle(50402, 2, 1127481344, 1104150528, 1118476698, 0, 2);
	rotationAddMovabilityToRotation(50501, 50106, "1925", 2837, -480, 3327, 355, 1, 53, 0);
	rotationSetMovabilityToRotation(50501, 0, 1132920832, 1050253722, 1118476698, 0, 0, 1132920832, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50501, 50502, "1926", 1100, -202, 1420, 174, 1, 53, 0);
	rotationSetMovabilityToRotation(50501, 1, 1119092736, 1050253722, 1118476698, 0, 2, 1119092736, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50502, 50501, "1927", 2916, -277, 3179, 174, 1, 53, 0);
	rotationSetMovabilityToRotation(50502, 0, 1132920832, 1050253722, 1118476698, 0, 0, 1132920832, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50601, 50108, "1928", 907, -273, 1406, 247, 1, 53, 0);
	rotationSetMovabilityToRotation(50601, 0, 1116471296, 1050253722, 1118476698, 0, 2, 1116471296, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50601, 50602, "1929", 3387, -102, 3599, 348, 1, 53, 0);
	rotationSetMovabilityToRotation(50601, 1, 1133772800, 1050253722, 1118476698, 0, 2, 1133772800, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50601, 50602, "1929", 0, -102, 195, 348, 1, 53, 0);
	rotationSetMovabilityToRotation(50601, 2, 1133772800, 1050253722, 1118476698, 0, 2, 1133772800, 1050253722, 1118476698);
	rotationAddMovabilityToPuzzle(50601, kPuzzle50601, "", 2782, 317, 3250, 552, 1, 52, 0);
	rotationSetMovabilityToPuzzle(50601, 3, 1127481344, 1104150528, 1118476698, 0, 2);
	rotationAddMovabilityToPuzzle(50601, kPuzzle50602, "", 2643, -80, 3306, 177, 1, 52, 0);
	rotationSetMovabilityToPuzzle(50601, 4, 1127481344, 1104150528, 1118476698, 0, 2);
	rotationAddMovabilityToRotation(50602, 50601, "1930", 1408, -181, 1666, 237, 1, 53, 0);
	rotationSetMovabilityToRotation(50602, 0, 1122631680, 1050253722, 1118476698, 0, 2, 1122631680, 1050253722, 1118476698);
	rotationAddMovabilityToRotation(50701, 50103, "1931", 225, -136, 701, 378, 1, 53, 0);
	rotationSetMovabilityToRotation(50701, 0, 0, 1050253722, 1118476698, 0, 0, 0, 1050253722, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle50400, 50402, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle50400, 0, 1127481344, 1104150528, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle50401, 50402, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle50401, 0, 1127481344, 1104150528, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle50303, 50303, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle50303, 0, 1132756992, 1050253722, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle50304, 50304, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle50304, 0, 1124204544, 1050253722, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle50601, 50601, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle50601, 0, 1132068864, 1104150528, 1118476698);
	puzzleAddMovabilityToPuzzle(kPuzzle50601, 50602, "", Common::Rect(0, 0, 640, 32), 1, 55, 0);
	puzzleAddMovabilityToRotation(kPuzzle50602, 50601, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle50602, 0, 1132068864, 1050253722, 1118476698);
	puzzleAddMovabilityToPuzzle(kPuzzle50602, 50601, "", Common::Rect(0, 0, 640, 32), 1, 55, 0);
	puzzleAddMovabilityToRotation(kPuzzle50501, 50502, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle50501, 0, 1119092736, 1050253722, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle50502, 50502, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle50502, 0, 1119092736, 1050253722, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle50503, 50502, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle50503, 0, 1119092736, 1050253722, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle50504, 50502, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle50504, 0, 1119092736, 1050253722, 1118476698);
	objectAdd(kObject51000, "", "", 1);
	objectAddPresentation(kObject51000);
	objectPresentationAddAnimationToPuzzle(kObject51000, 0, kPuzzle51001, "TR_WA_A06", 0, 262, 151, 1, 1000, 19, 1095237632, 4);
	objectAddPresentation(kObject51000);
	objectPresentationAddAnimationToPuzzle(kObject51000, 1, kPuzzle51002, "TR_WA_A09", 0, 139, 94, 1, 1000, 19, 1095237632, 4);
	objectAddPresentation(kObject51000);
	objectPresentationAddAnimationToPuzzle(kObject51000, 2, kPuzzle51004, "TR_WA_BS", 0, 264, 189, 1, 1000, 19, 1095237632, 4);
	objectAddPresentation(kObject51000);
	objectPresentationAddAnimationToPuzzle(kObject51000, 3, kPuzzle51007, "TR_WA_ES", 0, 218, 18, 1, 1000, 19, 1095237632, 4);
	objectAddPresentation(kObject51000);
	objectPresentationAddAnimationToPuzzle(kObject51000, 4, kPuzzle51009, "TR_WA_GS", 0, 228, 82, 1, 1000, 19, 1095237632, 4);
	objectAddPresentation(kObject51000);
	objectPresentationAddAnimationToPuzzle(kObject51000, 5, kPuzzle51011, "TR_WA_IS", 0, 323, 184, 1, 1000, 19, 1095237632, 4);
	objectAddPresentation(kObject51000);
	objectPresentationAddAnimationToPuzzle(kObject51000, 6, kPuzzle51012, "TR_WA_JS", 0, 223, 16, 1, 1000, 19, 1095237632, 4);
	objectAddPresentation(kObject51000);
	objectPresentationAddAnimationToPuzzle(kObject51000, 7, kPuzzle51013, "TR_WA_KS", 0, 210, 70, 1, 1000, 19, 1095237632, 4);
	objectAdd(kObjectMagicLance, "Beam of light", "WA_Lance", 1);
	objectAddBagAnimation(kObjectMagicLance, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectMagicLance, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectMagicLance, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectFeather, "Feather", "WA_Feather", 1);
	objectAddBagAnimation(kObjectFeather, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectFeather, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectFeather, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectDeadLeaf, "DeadLeaf", "WA_ItemLeaf", 1);
	objectAddBagAnimation(kObjectDeadLeaf, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectDeadLeaf, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectDeadLeaf, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectInk, "Ink", "WA_Ink", 8);
	objectAddBagAnimation(kObjectInk, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectInk, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectInk, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectInk, kPuzzle50100, Common::Rect(446, 399, 488, 457), 1, 52, 0);
	objectAdd(kObjectPaper, "Paper", "WA_Paper", 8);
	objectAddBagAnimation(kObjectPaper, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectPaper, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectPaper, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectPaper, kPuzzle50100, Common::Rect(487, 386, 526, 453), 1, 52, 1);
	objectAdd(kObjectStylet, "Stylet", "WA_Staylet", 8);
	objectAddBagAnimation(kObjectStylet, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectStylet, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectStylet, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectStylet, kPuzzle50100, Common::Rect(526, 414, 556, 450), 1, 52, 2);
	objectAddPuzzleAccessibility(kObjectStylet, kPuzzle50100, Common::Rect(220, 395, 283, 456), 1, 52, 8);
	objectAdd(kObjectInkedStylet, "Ink & Stylet", "WA_StyletInk", 8);
	objectAddBagAnimation(kObjectInkedStylet, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectInkedStylet, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectInkedStylet, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectInkedStylet, kPuzzle50100, Common::Rect(326, 399, 374, 446), 1, 52, 9);
	objectAdd(kObject50105, "Ashes", "", 1);
	objectSetActiveCursor(kObject50105, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObject50105, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObject50105, kPuzzle50100, Common::Rect(318, 31, 379, 102), 1, 52, 0);
	objectAdd(kObject50100, "", "", 1);
	objectAddRotationAccessibility(kObject50100, 50108, Common::Rect(2701, -231, 3260, 345), 1, 52, 0);
	objectAddPresentation(kObject50100);
	objectPresentationAddImageToPuzzle(kObject50100, 0, kPuzzle50100, "WAS01N08P01L01_A.bmp", 450, 401, 1, 1, 1000);
	objectAddPresentation(kObject50100);
	objectPresentationAddImageToPuzzle(kObject50100, 1, kPuzzle50100, "WAS01N08P01L01_B.bmp", 490, 388, 1, 1, 1000);
	objectAddPresentation(kObject50100);
	objectPresentationAddImageToPuzzle(kObject50100, 2, kPuzzle50100, "WAS01N08P01L01_C.bmp", 526, 414, 1, 1, 1000);
	objectAddPresentation(kObject50100);
	objectPresentationAddImageToPuzzle(kObject50100, 3, kPuzzle50100, "WAS01N08P01L03.bmp", 221, 397, 1, 1, 1000);
	objectAddPresentation(kObject50100);
	objectPresentationAddImageToPuzzle(kObject50100, 4, kPuzzle50100, "WAS01N08P01L02.bmp", 326, 401, 1, 1, 1000);
	objectAddPresentation(kObject50100);
	objectPresentationAddImageToPuzzle(kObject50100, 5, kPuzzle50100, "WAS01N08P01L05.bmp", 218, 398, 1, 1, 1000);
	objectAddPresentation(kObject50100);
	objectPresentationAddImageToPuzzle(kObject50100, 6, kPuzzle50100, "WAS01N08P01L04.bmp", 325, 400, 1, 1, 1000);
	objectAddPresentation(kObject50100);
	objectPresentationAddAnimationToPuzzle(kObject50100, 7, kPuzzle50100, "WAS01N08P01S01", 0, 270, 101, 1, 1000, 50, 1095237632, 6);
	objectPresentationAnimationSetStartFrame(kObject50100, 7, 1);
	objectPresentationSetAnimationOnPuzzle(kObject50100, 7, 0, 50003);
	objectAddPresentation(kObject50100);
	objectPresentationAddAnimationToPuzzle(kObject50100, 8, kPuzzle50100, "WAS01N08P01S02", 0, 316, 30, 1, 1000, 50, 1095237632, 6);
	objectPresentationSetAnimationOnPuzzle(kObject50100, 8, 0, 50004);
	objectPresentationAddImageToPuzzle(kObject50100, 8, kPuzzle50100, "WAS01N08P01S01.bmp", 270, 101, 1, 1, 1000);
	objectPresentationShow(kObject50100, 0);
	objectPresentationShow(kObject50100, 1);
	objectPresentationShow(kObject50100, 2);
	objectAdd(kObject50600, "", "", 1);
	objectAddPresentation(kObject50600);
	objectPresentationAddAnimationToPuzzle(kObject50600, 0, kPuzzle50601, "WAS06N01P01S01", 0, 171, 182, 1, 1000, 50, 1095237632, 4);
	objectPresentationSetAnimationOnPuzzle(kObject50600, 0, 0, 50001);
	objectAddPresentation(kObject50600);
	objectPresentationAddAnimationToPuzzle(kObject50600, 1, kPuzzle50602, "WAS06N01P02S01", 0, 77, 134, 1, 1000, 30, 1095237632, 4);
	objectPresentationSetAnimationOnPuzzle(kObject50600, 1, 0, 50002);
	objectAddPresentation(kObject50600);
	objectPresentationAddImageToPuzzle(kObject50600, 2, kPuzzle50601, "WAS06N01P01L01.bmp", 206, 149, 1, 1, 1000);
	objectAddPresentation(kObject50600);
	objectPresentationAddImageToPuzzle(kObject50600, 3, kPuzzle50601, "WAS06N01P01L02.bmp", 383, 146, 1, 1, 1000);
	objectAddPresentation(kObject50600);
	objectPresentationAddImageToPuzzle(kObject50600, 4, kPuzzle50601, "WAS06N01P01L03.bmp", 225, 282, 1, 1, 1000);
	objectAddPresentation(kObject50600);
	objectPresentationAddImageToPuzzle(kObject50600, 5, kPuzzle50601, "WAS06N01P01L04.bmp", 380, 294, 1, 1, 1000);
	objectAddPresentation(kObject50600);
	objectPresentationAddImageToPuzzle(kObject50600, 6, kPuzzle50602, "WAS06N01P02L01.bmp", 115, 178, 1, 1, 1000);
	objectAddPresentation(kObject50600);
	objectPresentationAddImageToPuzzle(kObject50600, 7, kPuzzle50602, "WAS06N01P02L02.bmp", 114, 158, 1, 1, 1000);
	objectPresentationShow(kObject50600, 0);
	objectPresentationShow(kObject50600, 1);
	objectAddPuzzleAccessibility(kObject50600, kPuzzle50601, Common::Rect(208, 148, 237, 177), 1, 52, 0);
	objectAddPuzzleAccessibility(kObject50600, kPuzzle50601, Common::Rect(384, 148, 411, 172), 1, 52, 1);
	objectAddPuzzleAccessibility(kObject50600, kPuzzle50601, Common::Rect(227, 281, 253, 308), 1, 52, 2);
	objectAddPuzzleAccessibility(kObject50600, kPuzzle50601, Common::Rect(381, 290, 410, 322), 1, 52, 3);
	objectAdd(kObject50001, "", "", 1);
	objectAddPresentation(kObject50001);
	objectPresentationAddAnimationToPuzzle(kObject50001, 0, kPuzzle50002, "WAS00N01P01S01", 0, 414, 63, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject50001);
	objectPresentationAddAnimationToPuzzle(kObject50001, 1, kPuzzle50001, "WAS00N01P01S02", 0, 256, 263, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObjectGolem1, "Golem", "FO_Golem", 1);
	objectAddBagAnimation(kObjectGolem1, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectGolem1, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectGolem1, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectGolem1, kPuzzle50400, Common::Rect(232, 168, 422, 403), 1, 52, 0);
	objectAddPresentation(kObjectGolem1);
	objectPresentationAddImageToRotation(kObjectGolem1, 0, 50402, 0);
	objectPresentationAddImageToRotation(kObjectGolem1, 0, 50401, 0);
	objectAddPresentation(kObjectGolem1);
	objectPresentationAddImageToPuzzle(kObjectGolem1, 1, kPuzzle50400, "WAS04N02P01L02.bmp", 212, 178, 1, 1, 998);
	objectAdd(kObjectFronthead, "Head front", "WA_GolemHeadF", 8);
	objectAddBagAnimation(kObjectFronthead, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectFronthead, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectFronthead, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectFronthead, kPuzzle50400, Common::Rect(298, 159, 355, 233), 0, 52, 0);
	objectAddPuzzleAccessibility(kObjectFronthead, kPuzzle50400, Common::Rect(215, 92, 269, 160), 0, 52, 1);
	objectAddPresentation(kObjectFronthead);
	objectPresentationAddImageToPuzzle(kObjectFronthead, 0, kPuzzle50400, "WAS04N02P01L09.bmp", 296, 160, 1, 1, 1000);
	objectAddPresentation(kObjectFronthead);
	objectPresentationAddImageToPuzzle(kObjectFronthead, 1, kPuzzle50400, "WAS04N02P01L03.bmp", 209, 87, 1, 1, 1000);
	objectAdd(kObjectBackhead, "Head back", "WA_GolemHeadB", 8);
	objectAddBagAnimation(kObjectBackhead, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectBackhead, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectBackhead, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectBackhead, kPuzzle50400, Common::Rect(298, 159, 355, 233), 0, 52, 0);
	objectAddPuzzleAccessibility(kObjectBackhead, kPuzzle50400, Common::Rect(399, 93, 450, 158), 0, 52, 20);
	objectAddPresentation(kObjectBackhead);
	objectPresentationAddImageToPuzzle(kObjectBackhead, 0, kPuzzle50400, "WAS04N02P01L21.bmp", 301, 157, 1, 1, 999);
	objectAddPresentation(kObjectBackhead);
	objectPresentationAddImageToPuzzle(kObjectBackhead, 1, kPuzzle50400, "WAS04N02P01L06.bmp", 403, 89, 1, 1, 1000);
	objectAdd(kObjectBelly, "Belly", "WA_GolemStomak", 8);
	objectAddBagAnimation(kObjectBelly, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectBelly, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectBelly, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectBelly, kPuzzle50400, Common::Rect(298, 257, 358, 299), 0, 52, 0);
	objectAddPuzzleAccessibility(kObjectBelly, kPuzzle50400, Common::Rect(135, 301, 212, 358), 0, 52, 300);
	objectAddPresentation(kObjectBelly);
	objectPresentationAddImageToPuzzle(kObjectBelly, 0, kPuzzle50400, "WAS04N02P01L13.bmp", 292, 252, 1, 1, 1000);
	objectPresentationAddImageToPuzzle(kObjectBelly, 0, kPuzzle50400, "WAS04N02P01L14.bmp", 302, 293, 1, 1, 1000);
	objectAddPresentation(kObjectBelly);
	objectPresentationAddImageToPuzzle(kObjectBelly, 1, kPuzzle50400, "WAS04N02P01L07.bmp", 142, 302, 1, 1, 1000);
	objectAdd(kObjectRightArm, "Arm right", "WA_GolemArmR", 8);
	objectAddBagAnimation(kObjectRightArm, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectRightArm, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectRightArm, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectRightArm, kPuzzle50400, Common::Rect(241, 213, 276, 253), 0, 52, 0);
	objectAddPuzzleAccessibility(kObjectRightArm, kPuzzle50400, Common::Rect(151, 206, 200, 255), 0, 52, 4000);
	objectAddPresentation(kObjectRightArm);
	objectPresentationAddImageToPuzzle(kObjectRightArm, 0, kPuzzle50400, "WAS04N02P01L10.bmp", 224, 203, 1, 1, 1000);
	objectAddPresentation(kObjectRightArm);
	objectPresentationAddImageToPuzzle(kObjectRightArm, 1, kPuzzle50400, "WAS04N02P01L04.bmp", 147, 206, 1, 1, 1000);
	objectAdd(kObjectLeftArm, "Arm left", "WA_GolemArmL", 8);
	objectAddBagAnimation(kObjectLeftArm, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectLeftArm, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectLeftArm, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectLeftArm, kPuzzle50400, Common::Rect(380, 217, 414, 249), 0, 52, 0);
	objectAddPuzzleAccessibility(kObjectLeftArm, kPuzzle50400, Common::Rect(443, 211, 488, 253), 0, 52, 50000);
	objectAddPresentation(kObjectLeftArm);
	objectPresentationAddImageToPuzzle(kObjectLeftArm, 0, kPuzzle50400, "WAS04N02P01L11.bmp", 375, 210, 1, 1, 1000);
	objectAddPresentation(kObjectLeftArm);
	objectPresentationAddImageToPuzzle(kObjectLeftArm, 1, kPuzzle50400, "WAS04N02P01L05.bmp", 440, 208, 1, 1, 1000);
	objectAdd(kObjectLegs, "Legs", "WA_GolemLeg", 8);
	objectAddBagAnimation(kObjectLegs, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectLegs, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectLegs, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectLegs, kPuzzle50400, Common::Rect(268, 306, 384, 391), 0, 52, 0);
	objectAddPuzzleAccessibility(kObjectLegs, kPuzzle50400, Common::Rect(445, 297, 534, 349), 0, 52, 600000);
	objectAddPresentation(kObjectLegs);
	objectPresentationAddImageToPuzzle(kObjectLegs, 0, kPuzzle50400, "WAS04N02P01L15.bmp", 269, 282, 1, 1, 1000);
	objectPresentationAddImageToPuzzle(kObjectLegs, 0, kPuzzle50400, "WAS04N02P01L16.bmp", 268, 299, 1, 1, 1000);
	objectPresentationAddImageToPuzzle(kObjectLegs, 0, kPuzzle50400, "WAS04N02P01L17.bmp", 263, 307, 1, 1, 1000);
	objectPresentationAddImageToPuzzle(kObjectLegs, 0, kPuzzle50400, "WAS04N02P01L18.bmp", 365, 285, 1, 1, 1000);
	objectPresentationAddImageToPuzzle(kObjectLegs, 0, kPuzzle50400, "WAS04N02P01L19.bmp", 348, 301, 1, 1, 1000);
	objectPresentationAddImageToPuzzle(kObjectLegs, 0, kPuzzle50400, "WAS04N02P01L20.bmp", 334, 309, 1, 1, 1000);
	objectAddPresentation(kObjectLegs);
	objectPresentationAddImageToPuzzle(kObjectLegs, 1, kPuzzle50400, "WAS04N02P01L08.bmp", 440, 301, 1, 1, 1000);
	objectAdd(kObjectHeart, "Heart", "WA_GolemHeart", 8);
	objectAddBagAnimation(kObjectHeart, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectHeart, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectHeart, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectHeart, kPuzzle50400, Common::Rect(342, 231, 363, 251), 0, 52, 0);
	objectAddPuzzleAccessibility(kObjectHeart, kPuzzle50400, Common::Rect(325, 50, 340, 95), 0, 52, 7000000);
	objectAddPresentation(kObjectHeart);
	objectPresentationAddImageToPuzzle(kObjectHeart, 0, kPuzzle50400, "WAS04N02P01L12.bmp", 343, 231, 1, 1, 1000);
	objectAddPresentation(kObjectHeart);
	objectPresentationAddImageToPuzzle(kObjectHeart, 1, kPuzzle50400, "WAS04N02P01L22.bmp", 315, 74, 1, 1, 1000);
	objectAdd(kObject50451, "", "WA_glava", 8);
	objectAdd(kObject50452, "", "WA_droka", 8);
	objectAdd(kObject50453, "", "WA_lroka", 8);
	objectAdd(kObject50454, "", "WA_trup", 8);
	objectAdd(kObject50455, "", "WA_lnoga", 8);
	objectAdd(kObject50456, "", "WA_dnoga", 8);
	objectAdd(kObject50457, "", "WA_srce", 8);

	uint32 val = 115;
	for (uint32 i = 0; i < 7; i++) {
		objectSetActiveCursor((ObjectId)(kObject50451 + i), 22, 22, 0, 3, 0, 0, 3);
		objectSetPassiveCursor((ObjectId)(kObject50451 + i), 22, 22, 0, 3, 0, 0, 3);
		objectAddPuzzleAccessibility((ObjectId)(kObject50451 + i), kPuzzle50401, Common::Rect(125, val, 160, i >= 6 ? val + 35 : val), i >= 6 ? 0 : 1, 52, 0);
		objectAddPresentation((ObjectId)(kObject50451 + i));
		objectPresentationAddImageToPuzzle((ObjectId)(kObject50451 + i), 0, kPuzzle50401, Common::String::format("WAS04N02P02_L%d.tga", i + 1), 125, val, 1, 3, 1000);
		objectPresentationShow((ObjectId)(kObject50451 + i));
		val += 37;
	}

	for (uint32 i = 0; i < 7; i++) {
		uint32 l = 1;
		for (uint32 j = 115; j < (115 + 7 * 37); j += 37) {
			for (uint32 k = 162; k < (162 + 7 * 53); k += 53) {
				objectAddPuzzleAccessibility((ObjectId)(kObject50451 + i), kPuzzle50401, Common::Rect(k, j, k + 53, j + 37), 0, 52, l * 10);
			}
			l++;
		}
	}

	objectAdd(kObjectMagnet, "", "", 8);

	uint32 l = 1;
	for (uint32 j = 115; j < (115 + 7 * 37); j += 37) {
		for (uint32 k = 162; k < (162 + 7 * 53); k += 53) {
			objectAddPuzzleAccessibility(kObjectMagnet, kPuzzle50401, Common::Rect(k, j, k + 53, j + 37), 0, 52, l * 10);
		}
		l++;
	}

	objectAdd(kObjectThread, "Rope", "WA_Rope", 1);
	objectAddBagAnimation(kObjectThread, 1, 3, 1, 1095237632, 4);
	objectSetActiveCursor(kObjectThread, 22, 22, 1, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectThread, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObjectTree, "", "", 1);
	objectAddPresentation(kObjectTree);
	objectPresentationAddImageToRotation(kObjectTree, 0, 50502, 0);
	objectPresentationAddImageToRotation(kObjectTree, 0, 50501, 0);
	objectAddPresentation(kObjectTree);
	objectPresentationAddImageToRotation(kObjectTree, 1, 50502, 1);
	objectPresentationAddImageToPuzzle(kObjectTree, 1, kPuzzle50501, "WAS05N02P01L01.BMP", 281, 164, 1, 1, 1000);
	objectAddPresentation(kObjectTree);
	objectPresentationAddImageToRotation(kObjectTree, 2, 50502, 2);
	objectPresentationAddImageToPuzzle(kObjectTree, 2, kPuzzle50502, "WAS05N02P02L01.BMP", 226, 189, 1, 1, 1000);
	objectAddPresentation(kObjectTree);
	objectPresentationAddImageToRotation(kObjectTree, 3, 50502, 3);
	objectPresentationAddImageToPuzzle(kObjectTree, 3, kPuzzle50503, "WAS05N02P03L01.BMP", 215, 200, 1, 1, 1000);
	objectAddPresentation(kObjectTree);
	objectPresentationAddImageToRotation(kObjectTree, 4, 50502, 4);
	objectPresentationAddImageToPuzzle(kObjectTree, 4, kPuzzle50504, "WAS05N02P04L01.BMP", 213, 140, 1, 1, 1000);
	objectAddPresentation(kObjectTree);
	objectPresentationAddImageToRotation(kObjectTree, 5, 50502, 5);
	objectPresentationShow(kObjectTree, 5);
	objectAddPresentation(kObjectTree);
	objectPresentationAddImageToRotation(kObjectTree, 6, 50502, 6);
	objectPresentationAddImageToRotation(kObjectTree, 6, 50501, 1);
	objectAdd(kObject50700, "", "", 1);
	objectAddRotationAccessibility(kObject50700, 50701, Common::Rect(1975, -568, 2378, -240), 1, 52, 0);
	objectAddRotationAccessibility(kObject50700, 50701, Common::Rect(2033, -172, 2411, 289), 0, 52, 1);
	objectAddRotationAccessibility(kObject50700, 50701, Common::Rect(2079, 211, 2429, 537), 0, 52, 2);
	objectAddPresentation(kObject50700);
	objectPresentationAddImageToRotation(kObject50700, 0, 50103, 0);
	objectPresentationAddImageToRotation(kObject50700, 0, 50701, 0);
	objectAddPresentation(kObject50700);
	objectPresentationAddImageToRotation(kObject50700, 1, 50103, 1);
	objectPresentationAddImageToRotation(kObject50700, 1, 50701, 1);
	objectAddPresentation(kObject50700);
	objectPresentationAddAnimationToPuzzle(kObject50700, 2, kPuzzle50701, "WAS07N01P01S01", 0, 293, 187, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject50700);
	objectPresentationAddAnimationToPuzzle(kObject50700, 3, kPuzzle50702, "WAS07N01P01S02", 0, 304, 65, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObjectCounch1, "Conch", "WA_Conch", 1);
	objectAddBagAnimation(kObjectCounch1, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectCounch1, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectCounch1, 22, 22, 0, 3, 0, 0, 3);
	objectAddRotationAccessibility(kObjectCounch1, 50304, Common::Rect(1582, -54, 2009, 247), 1, 52, 0);
	objectAddRotationAccessibility(kObjectCounch1, 50304, Common::Rect(2424, -10, 2612, 136), 0, 52, 1);
	objectAddPuzzleAccessibility(kObjectCounch1, kPuzzle50304, Common::Rect(184, 191, 389, 391), 1, 52, 0);
	objectAddPresentation(kObjectCounch1);
	objectPresentationAddImageToRotation(kObjectCounch1, 0, 50304, 0);
	objectAddPresentation(kObjectCounch1);
	objectPresentationAddImageToRotation(kObjectCounch1, 1, 50304, 1);
	objectAddPresentation(kObjectCounch1);
	objectPresentationAddImageToRotation(kObjectCounch1, 2, 50304, 2);
	objectAddPresentation(kObjectCounch1);
	objectPresentationAddImageToRotation(kObjectCounch1, 3, 50304, 3);
	objectAddPresentation(kObjectCounch1);
	objectPresentationAddImageToRotation(kObjectCounch1, 4, 50304, 4);
	objectAddPresentation(kObjectCounch1);
	objectPresentationAddImageToRotation(kObjectCounch1, 5, 50304, 5);
	objectAdd(kObjectPhoenix, "", "", 1);
	objectAddRotationAccessibility(kObjectPhoenix, 50303, Common::Rect(2896, -146, 3529, 231), 1, 52, 0);
	objectAddPuzzleAccessibility(kObjectPhoenix, kPuzzle50303, Common::Rect(178, 89, 443, 384), 1, 52, 0);
	objectAddPresentation(kObjectPhoenix);
	objectPresentationAddImageToRotation(kObjectPhoenix, 0, 50301, 0);
	objectPresentationAddImageToRotation(kObjectPhoenix, 0, 50302, 0);
	objectPresentationAddImageToRotation(kObjectPhoenix, 0, 50303, 0);
	objectAdd(kObjectRopes, "", "", 1);
	objectAddRotationAccessibility(kObjectRopes, 50202, Common::Rect(653, -270, 775, 125), 1, 52, 0);
	objectAddRotationAccessibility(kObjectRopes, 50202, Common::Rect(1001, -298, 1023, 130), 1, 52, 1);
	objectAddRotationAccessibility(kObjectRopes, 50202, Common::Rect(1536, -287, 1633, 120), 1, 52, 2);
	objectAddRotationAccessibility(kObjectRopes, 50202, Common::Rect(1745, -387, 1965, 211), 1, 52, 3);
	objectAddPresentation(kObjectRopes);
	objectPresentationAddImageToRotation(kObjectRopes, 0, 50201, 0);
	objectPresentationAddImageToRotation(kObjectRopes, 0, 50202, 0);
	objectAddPresentation(kObjectRopes);
	objectPresentationAddImageToRotation(kObjectRopes, 1, 50201, 1);
	objectPresentationAddImageToRotation(kObjectRopes, 1, 50202, 1);
	objectAddPresentation(kObjectRopes);
	objectPresentationAddImageToRotation(kObjectRopes, 2, 50201, 2);
	objectPresentationAddImageToRotation(kObjectRopes, 2, 50202, 2);
	objectAdd(kObjectCloud, "", "", 1);
	objectAddRotationAccessibility(kObjectCloud, 50202, Common::Rect(0, -600, 1689, -219), 1, 52, 0);
	objectAdd(kObjectCounch, "", "", 1);
	objectAdd(kObjectBark, "Bark", "WA_ItemBark", 9);
	objectAddBagAnimation(kObjectBark, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectBark, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectBark, 22, 22, 0, 3, 0, 0, 3);
	objectAddRotationAccessibility(kObjectBark, 50302, Common::Rect(2858, 251, 3011, 512), 0, 52, 0);
	objectAdd(kObjectFlower, "Flower", "WA_ItemFlower", 9);
	objectAddBagAnimation(kObjectFlower, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectFlower, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectFlower, 22, 22, 0, 3, 0, 0, 3);
	objectAddRotationAccessibility(kObjectFlower, 50402, Common::Rect(2689, 190, 2937, 301), 1, 52, 0);
	objectAddPresentation(kObjectFlower);
	objectPresentationAddImageToRotation(kObjectFlower, 0, 50402, 1);
	objectPresentationShow(kObjectFlower);
	objectAdd(kObjectDragonSword, "Sword", "WA_Sword", 1);
	objectAddBagAnimation(kObjectDragonSword, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectDragonSword, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectDragonSword, 22, 22, 0, 3, 0, 0, 3);
	objectAddRotationAccessibility(kObjectDragonSword, 50502, Common::Rect(1093, 176, 1383, 381), 1, 52, 0);
	objectAdd(kObjectApple, "Apple", "WA_ItemFruit", 1);
	objectAddBagAnimation(kObjectApple, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectApple, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectApple, 22, 22, 0, 3, 0, 0, 3);
	objectAddRotationAccessibility(kObjectApple, 50502, Common::Rect(407, 190, 513, 289), 1, 52, 0);
	objectAdd(kObjectTotems, "", "", 1);
	objectAddPuzzleAccessibility(kObjectTotems, kPuzzle50501, Common::Rect(223, 134, 430, 409), 1, 52, 0);
	objectAddPuzzleAccessibility(kObjectTotems, kPuzzle50502, Common::Rect(195, 120, 388, 392), 1, 52, 1);
	objectAddPuzzleAccessibility(kObjectTotems, kPuzzle50503, Common::Rect(217, 92, 423, 405), 1, 52, 2);
	objectAddPuzzleAccessibility(kObjectTotems, kPuzzle50504, Common::Rect(196, 115, 413, 441), 1, 52, 3);
	objectAddRotationAccessibility(kObjectTotems, 50502, Common::Rect(947, -20, 1014, 43), 1, 52, 10);
	objectAddRotationAccessibility(kObjectTotems, 50502, Common::Rect(1091, -29, 1154, 48), 1, 52, 11);
	objectAddRotationAccessibility(kObjectTotems, 50502, Common::Rect(1469, -31, 1527, 45), 1, 52, 12);
	objectAddRotationAccessibility(kObjectTotems, 50502, Common::Rect(1597, -24, 1650, 50), 1, 52, 13);
	objectAdd(kObject50601, "", "", 1);
	objectAddRotationAccessibility(kObject50601, 50602, Common::Rect(3225, 17, 3364, 144), 1, 52, 0);
	objectAddRotationAccessibility(kObject50601, 50602, Common::Rect(423, -205, 667, 8), 1, 52, 1);
	objectAddRotationAccessibility(kObject50601, 50602, Common::Rect(868, 0, 996, 95), 1, 52, 2);
	soundAdd(50017, 2, "1932.wav", _configuration.ambientMusic.loadFrom, 2, _configuration.ambientMusic.soundChunck);
	puzzleAddAmbientSound(kPuzzle50602, 50017, 100, 0, 1, 1, 10);
	soundSetVolume(50017, 90);
	soundAdd(51002, 2, "1938.was", _configuration.ambientMusic.loadFrom, 2, _configuration.ambientMusic.soundChunck);
	soundSetVolume(51002, 95);
	soundAdd(51004, 2, "1939.was", _configuration.ambientMusic.loadFrom, 2, _configuration.ambientMusic.soundChunck);
	soundSetVolume(51004, 100);
	soundAdd(51003, 2, "1940.was", _configuration.ambientMusic.loadFrom, 2, _configuration.ambientMusic.soundChunck);
	soundSetVolume(51003, 95);
	soundAdd(51001, 2, "1941.was", _configuration.ambientMusic.loadFrom, 2, _configuration.ambientMusic.soundChunck);
	soundSetVolume(51001, 95);
	soundAdd(51006, 2, "1942.wav", _configuration.ambientMusic.loadFrom, 2, _configuration.ambientMusic.soundChunck);
	soundSetVolume(51006, 95);
	soundAdd(51007, 2, "1943.was", _configuration.ambientMusic.loadFrom, 2, _configuration.ambientMusic.soundChunck);
	soundSetVolume(51007, 90);
	soundAdd(51008, 2, "1944.was", _configuration.ambientMusic.loadFrom, 2, _configuration.ambientMusic.soundChunck);
	soundSetVolume(51008, 95);
	soundAdd(51010, 2, "1945.was", _configuration.ambientMusic.loadFrom, 2, _configuration.ambientMusic.soundChunck);
	soundAdd(51011, 2, "1946.was", _configuration.ambientMusic.loadFrom, 2, _configuration.ambientMusic.soundChunck);
	soundAdd(51012, 2, "1947.was", _configuration.ambientMusic.loadFrom, 2, _configuration.ambientMusic.soundChunck);
	soundAdd(51013, 2, "1948.was", _configuration.ambientMusic.loadFrom, 2, _configuration.ambientMusic.soundChunck);
	soundAdd(50018, 3, "1949.was", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(50023, 5, "1933.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50024, 5, "1934.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50025, 5, "1935.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50001, 5, "1950.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50002, 5, "1951.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50003, 5, "1952.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50009, 5, "1953.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50010, 5, "1954.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50011, 5, "1955.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50012, 5, "1956.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50013, 5, "1957.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50014, 5, "1958.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50015, 5, "1959.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50016, 5, "1960.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50021, 5, "1961.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50022, 5, "1962.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50026, 5, "1963.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50027, 5, "1964.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50028, 5, "1965.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50029, 5, "1966.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50030, 5, "1967.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50031, 5, "1968.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50032, 5, "1969.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50033, 5, "1970.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50034, 5, "1971.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50035, 5, "1972.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(50036, 5, "1973.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	varDefineByte(50001, 0);
	varDefineByte(50003, 0);
	varDefineWord(50000, 0);
	varDefineByte(50004, 0);
	varDefineByte(50005, 0);
	varDefineDword(50000, 0);
	varDefineWord(50101, 0);
	varDefineWord(50102, 0);
	varDefineWord(50103, 0);
	varDefineWord(50104, 0);
	varDefineWord(50105, 0);
	varDefineByte(50006, 0);
	varDefineByte(50007, 0);
	varDefineByte(50008, 0);
	varDefineByte(50009, 0);
	varDefineByte(50010, 0);
	varDefineByte(50011, 0);
	varDefineByte(50012, 0);

	for (uint32 i = 0; i < 78; i++)
		varDefineDword(51000 + i, 0);
}

void ApplicationRing::initZoneAS() {
	setZoneAndEnableBag(kZoneAS);

	_loadFrom = kLoadFromDisk;
	_archiveType = kArchiveFile;

	puzzleAdd(kPuzzle80001);
	puzzleAddBackgroundImage(kPuzzle80001, "Old_Ish.bma", 0, 16, 1);
	puzzleAdd(kPuzzle80011);
	puzzleAddBackgroundImage(kPuzzle80011, "DRIL_02.bma", 0, 16, 1);
	puzzleAdd(kPuzzle80012);
	puzzleAddBackgroundImage(kPuzzle80012, "DRIL_03.bma", 0, 16, 1);
	puzzleAdd(kPuzzle80013);
	puzzleAddBackgroundImage(kPuzzle80013, "DRIL_05.bma", 0, 16, 1);
	puzzleAdd(kPuzzle80014);
	puzzleAddBackgroundImage(kPuzzle80014, "DRIL_06.bma", 0, 16, 1);
	puzzleAdd(kPuzzle80002);
	puzzleAddBackgroundImage(kPuzzle80002, "ASV01.bma", 0, 16, 1);
	puzzleAdd(kPuzzle80003);
	puzzleAddBackgroundImage(kPuzzle80003, "ASV02.bma", 0, 16, 1);
	puzzleAdd(kPuzzle80004);
	puzzleAddBackgroundImage(kPuzzle80004, "ASV03.bma", 0, 16, 1);
	puzzleAdd(kPuzzle80005);
	puzzleAddBackgroundImage(kPuzzle80005, "ASV04.bma", 0, 16, 1);
	puzzleAdd(kPuzzle80008);
	puzzleAddBackgroundImage(kPuzzle80008, "ASP04L01.bma", 0, 16, 1);
	puzzleAdd(kPuzzle80010);
	puzzleAddBackgroundImage(kPuzzle80010, "ASP05L01.bma", 0, 16, 1);
	puzzleAdd(kPuzzle80009);
	puzzleAddBackgroundImage(kPuzzle80009, "ASP03L01.bma", 0, 16, 1);
	puzzleAdd(kPuzzle80006);
	puzzleAddBackgroundImage(kPuzzle80006, "ASP01L01.bma", 0, 16, 1);
	puzzleAdd(kPuzzle80007);
	puzzleAddBackgroundImage(kPuzzle80007, "ASP02L01.bma", 0, 16, 1);

	_loadFrom = kLoadFromCd;
	if (_configuration.artAS)
		_archiveType = kArchiveArt;

	rotationAdd(80001, "ASS00N01", 0, 1);
	rotationAdd(80002, "ASS00N02", 0, 1);
	rotationAdd(80003, "ASS00N03", 0, 1);
	rotationAdd(80004, "ASS00N04", 0, 1);
	rotationAdd(80005, "ASS00N05", 0, 1);
	rotationAdd(80006, "ASS00N06", 0, 1);
	rotationAdd(80007, "ASS00N07", 0, 1);
	rotationAdd(80008, "ASS00N08", 0, 1);
	rotationAdd(80009, "ASS00N09", 0, 1);
	rotationAdd(80010, "ASS00N10", 0, 1);
	rotationAdd(80011, "ASS00N11", 0, 1);
	rotationAdd(80013, "ASS00N13", 0, 1);
	rotationAdd(80014, "ASS00N14", 0, 1);
	rotationAdd(80015, "ASS00N15", 0, 1);

	_loadFrom = kLoadFromDisk;
	rotationAdd(80101, "ASS01N01", 0, 6);
	_loadFrom = kLoadFromCd;

	rotationSetComBufferLength(80101, 1800000);
	rotationAddMovabilityToRotation(80001, 80002, "1001", 757, -71, 928, 85, 1, 53, 0);
	rotationSetMovabilityToRotation(80001, 0, 1112539136, 0, 1118476698, 0, 0, 1112539136, 0, 1118476698);
	rotationAddMovabilityToRotation(80001, 80003, "1002", 1251, -64, 1444, 95, 1, 53, 0);
	rotationSetMovabilityToRotation(80001, 1, 1119092736, 0, 1118476698, 0, 0, 1119092736, 0, 1118476698);
	rotationAddMovabilityToRotation(80001, 80004, "1003", 1784, -62, 1982, 90, 1, 53, 0);
	rotationSetMovabilityToRotation(80001, 2, 1124073472, 0, 1118476698, 0, 0, 1124073472, 0, 1118476698);
	rotationAddMovabilityToRotation(80002, 80001, "1004", 2696, -167, 2956, 81, 1, 53, 0);
	rotationSetMovabilityToRotation(80002, 0, 1130889216, 0, 1118476698, 0, 0, 1130889216, 0, 1118476698);
	rotationAddMovabilityToRotation(80002, 80003, "1005", 2130, -78, 2366, 130, 1, 53, 0);
	rotationSetMovabilityToRotation(80002, 1, 1127612416, 0, 1118476698, 0, 0, 1127612416, 0, 1118476698);
	rotationAddMovabilityToRotation(80002, 80005, "1006", 1634, -64, 1910, 87, 1, 53, 0);
	rotationSetMovabilityToRotation(80002, 2, 1124270080, 0, 1118476698, 0, 0, 1124270080, 0, 1118476698);
	rotationAddMovabilityToRotation(80003, 80002, "1007", 321, -69, 548, 108, 1, 53, 0);
	rotationSetMovabilityToRotation(80003, 0, 1073741824, 0, 1118476698, 0, 0, 1073741824, 0, 1118476698);
	rotationAddMovabilityToRotation(80003, 80005, "1008", 1168, -69, 1538, 106, 1, 53, 0);
	rotationSetMovabilityToRotation(80003, 1, 1119092736, 0, 1118476698, 0, 0, 1119092736, 0, 1118476698);
	rotationAddMovabilityToRotation(80003, 80004, "1009", 2137, -66, 2394, 99, 1, 53, 0);
	rotationSetMovabilityToRotation(80003, 2, 1127350272, 0, 1118476698, 0, 0, 1127350272, 0, 1118476698);
	rotationAddMovabilityToRotation(80003, 80013, "1010", 2721, -317, 2882, -198, 1, 53, 0);
	rotationSetMovabilityToRotation(80003, 3, 1130823680, -1043333120, 1118476698, 0, 0, 1132920832, 0, 1118476698);
	rotationAddMovabilityToRotation(80003, 80014, "1011", 3406, -319, 3578, -204, 1, 53, 0);
	rotationSetMovabilityToRotation(80003, 4, 1134198784, -1043333120, 1118476698, 0, 0, 1132920832, 0, 1118476698);
	rotationAddMovabilityToRotation(80003, 80001, "1012", 3053, -97, 3246, 57, 1, 53, 0);
	rotationSetMovabilityToRotation(80003, 5, 1132920832, 0, 1118476698, 0, 0, 1132920832, 0, 1118476698);
	rotationAddMovabilityToRotation(80003, 80101, "1013", 952, 326, 1798, 590, 1, 53, 0);
	rotationSetMovabilityToRotation(80003, 6, 1119092736, 1104150528, 1118476698, 0, 0, 1119092736, 0, 1118476698);
	rotationAddMovabilityToRotation(80003, 80015, "1014", 1306, -270, 1397, -162, 1, 53, 0);
	rotationSetMovabilityToRotation(80003, 7, 1119092736, -1044381696, 1118476698, 0, 0, 1119092736, 0, 1118476698);
	rotationAddMovabilityToRotation(80004, 80001, "1015", 3387, -95, 3596, 69, 1, 53, 0);
	rotationSetMovabilityToRotation(80004, 0, 1134166016, 0, 1118476698, 0, 0, 1134166016, 0, 1118476698);
	rotationAddMovabilityToRotation(80004, 80003, "1016", 374, -22, 541, 104, 1, 53, 0);
	rotationSetMovabilityToRotation(80004, 1, 1135804416, 0, 1118476698, 0, 0, 1135804416, 0, 1118476698);
	rotationAddMovabilityToRotation(80004, 80005, "1017", 810, -27, 1089, 73, 1, 53, 0);
	rotationSetMovabilityToRotation(80004, 2, 1111752704, 0, 1118476698, 0, 0, 1111752704, 0, 1118476698);
	rotationAddMovabilityToRotation(80005, 80003, "1018", 3053, -153, 3244, -6, 1, 53, 0);
	rotationSetMovabilityToRotation(80005, 0, 1132920832, 0, 1118476698, 0, 0, 1132920832, 0, 1118476698);
	rotationAddMovabilityToRotation(80005, 80003, "1018", 2984, -24, 3090, 172, 1, 53, 0);
	rotationSetMovabilityToRotation(80005, 1, 1132920832, 0, 1118476698, 0, 0, 1132920832, 0, 1118476698);
	rotationAddMovabilityToRotation(80005, 80003, "1018", 3230, -24, 3336, 172, 1, 53, 0);
	rotationSetMovabilityToRotation(80005, 2, 1132920832, 0, 1118476698, 0, 0, 1132920832, 0, 1118476698);
	rotationAddMovabilityToRotation(80005, 80002, "1019", 3408, -115, 3600, 102, 1, 53, 0);
	rotationSetMovabilityToRotation(80005, 3, 1134264320, 0, 1118476698, 0, 0, 1134264320, 0, 1118476698);
	rotationAddMovabilityToRotation(80005, 80002, "1019", 0, -115, 135, 102, 1, 53, 0);
	rotationSetMovabilityToRotation(80005, 4, 1134264320, 0, 1118476698, 0, 0, 1134264320, 0, 1118476698);
	rotationAddMovabilityToRotation(80005, 80004, "1020", 2617, -71, 2870, 83, 1, 53, 0);
	rotationSetMovabilityToRotation(80005, 5, 1130692608, 0, 1118476698, 0, 0, 1130692608, 0, 1118476698);
	rotationAddMovabilityToRotation(80005, 80008, "1021", 1209, -109, 1313, 181, 1, 53, 0);
	rotationSetMovabilityToRotation(80005, 6, 1119092736, 0, 1118476698, 0, 0, 1119092736, 0, 1118476698);
	rotationAddMovabilityToRotation(80005, 80008, "1021", 1397, -130, 1509, 181, 1, 53, 0);
	rotationSetMovabilityToRotation(80005, 7, 1119092736, 0, 1118476698, 0, 0, 1119092736, 0, 1118476698);
	rotationAddMovabilityToRotation(80006, 80007, "1022", 2026, 413, 2387, 586, 1, 53, 0);
	rotationSetMovabilityToRotation(80006, 0, 1127481344, 1104150528, 1118476698, 0, 0, 1127481344, 0, 1118476698);
	rotationAddMovabilityToRotation(80006, 80011, "1023", 1740, -33, 1875, 102, 1, 53, 0);
	rotationSetMovabilityToRotation(80006, 1, 1124663296, 0, 1118476698, 0, 0, 1119092736, 0, 1118476698);
	rotationAddMovabilityToRotation(80007, 80006, "1024", 29, -540, 639, -388, 1, 53, 0);
	rotationSetMovabilityToRotation(80007, 0, 0, -1043333120, 1118476698, 0, 0, 0, 0, 1118476698);
	rotationAddMovabilityToRotation(80007, 80008, "1025", 2047, -34, 2427, 163, 1, 53, 0);
	rotationSetMovabilityToRotation(80007, 1, 1127481344, 0, 1118476698, 0, 0, 1127481344, 0, 1118476698);
	rotationAddMovabilityToRotation(80008, 80007, "1026", 305, -48, 622, 122, 1, 53, 0);
	rotationSetMovabilityToRotation(80008, 0, 0, 0, 1118476698, 0, 0, 0, 0, 1118476698);
	rotationAddMovabilityToRotation(80008, 80011, "1027", 1274, -272, 1429, 50, 1, 53, 0);
	rotationSetMovabilityToRotation(80008, 1, 1119092736, -1043333120, 1118476698, 0, 0, 1119092736, 0, 1118476698);
	rotationAddMovabilityToRotation(80008, 80009, "1028", 2102, -71, 2431, 136, 1, 53, 0);
	rotationSetMovabilityToRotation(80008, 2, 1127481344, 0, 1118476698, 0, 0, 1127481344, 0, 1118476698);
	rotationAddMovabilityToRotation(80008, 80005, "1029", 3000, -73, 3100, 218, 1, 53, 0);
	rotationSetMovabilityToRotation(80008, 3, 1132920832, 0, 1118476698, 0, 0, 1132920832, 0, 1118476698);
	rotationAddMovabilityToRotation(80008, 80005, "1029", 3200, -73, 3300, 218, 1, 53, 0);
	rotationSetMovabilityToRotation(80008, 4, 1132920832, 0, 1118476698, 0, 0, 1132920832, 0, 1118476698);
	rotationAddMovabilityToRotation(80008, 80005, "1029", 3100, -73, 3200, -34, 1, 53, 0);
	rotationSetMovabilityToRotation(80008, 5, 1132920832, 0, 1118476698, 0, 0, 1132920832, 0, 1118476698);
	rotationAddMovabilityToRotation(80009, 80008, "1031", 333, -33, 678, 176, 1, 53, 0);
	rotationSetMovabilityToRotation(80009, 0, 0, 0, 1118476698, 0, 0, 0, 0, 1118476698);
	rotationAddMovabilityToRotation(80009, 80010, "1032", 1745, -495, 2478, -369, 1, 53, 0);
	rotationSetMovabilityToRotation(80009, 1, 1127481344, -1043333120, 1118476698, 0, 0, 1127481344, 0, 1118476698);
	rotationAddMovabilityToRotation(80010, 80009, "1033", 182, 422, 745, 552, 1, 53, 0);
	rotationSetMovabilityToRotation(80010, 0, 0, 1104150528, 1118476698, 0, 0, 0, 0, 1118476698);
	rotationAddMovabilityToRotation(80010, 80011, "1034", 849, -83, 1091, 104, 1, 53, 0);
	rotationSetMovabilityToRotation(80010, 1, 1110179840, 0, 1118476698, 0, 0, 1119092736, 0, 1118476698);
	rotationAddMovabilityToRotation(80011, 80006, "1035", 3531, -47, 3600, 95, 1, 53, 0);
	rotationSetMovabilityToRotation(80011, 0, 1134460928, 0, 1118476698, 0, 0, 1134460928, 0, 1118476698);
	rotationAddMovabilityToRotation(80011, 80006, "1035", 0, -47, 249, 95, 1, 53, 0);
	rotationSetMovabilityToRotation(80011, 1, 1134460928, 0, 1118476698, 0, 0, 1134460928, 0, 1118476698);
	rotationAddMovabilityToRotation(80011, 80008, "1036", 3009, 280, 3320, 469, 1, 53, 0);
	rotationSetMovabilityToRotation(80011, 2, 1132920832, 1104150528, 1118476698, 0, 0, 1132920832, 0, 1118476698);
	rotationAddMovabilityToRotation(80011, 80010, "1037", 2571, -29, 2784, 80, 1, 53, 0);
	rotationSetMovabilityToRotation(80011, 3, 1130299392, 0, 1118476698, 0, 0, 1130299392, 0, 1118476698);
	rotationAddMovabilityToRotation(80013, 80014, "1038", 342, -55, 444, 47, 1, 53, 0);
	rotationSetMovabilityToRotation(80013, 0, 0, 0, 1118476698, 0, 0, 0, 0, 1118476698);
	rotationAddMovabilityToRotation(80013, 80015, "1039", 1149, -31, 1225, 27, 1, 53, 0);
	rotationSetMovabilityToRotation(80013, 1, 1116864512, 0, 1118476698, 0, 0, 1116864512, 0, 1118476698);
	rotationAddMovabilityToRotation(80013, 80003, "1040", 892, 244, 1045, 348, 1, 53, 0);
	rotationSetMovabilityToRotation(80013, 2, 1112276992, 1104150528, 1118476698, 0, 0, 1112276992, 0, 1118476698);
	rotationAddMovabilityToRotation(80014, 80013, "1041", 2253, -57, 2366, 52, 1, 53, 0);
	rotationSetMovabilityToRotation(80014, 0, 1127481344, 0, 1118476698, 0, 0, 1127481344, 0, 1118476698);
	rotationAddMovabilityToRotation(80014, 80015, "1042", 1478, -41, 1550, 34, 1, 53, 0);
	rotationSetMovabilityToRotation(80014, 1, 1121320960, 0, 1118476698, 0, 0, 1121320960, 0, 1118476698);
	rotationAddMovabilityToRotation(80014, 80003, "1043", 1668, 238, 1814, 359, 1, 53, 0);
	rotationSetMovabilityToRotation(80014, 2, 1124139008, 1104150528, 1118476698, 0, 0, 1124139008, 0, 1118476698);
	rotationAddMovabilityToRotation(80015, 80013, "1044", 2951, -36, 3019, 34, 1, 53, 0);
	rotationSetMovabilityToRotation(80015, 0, 1132265472, 0, 1118476698, 0, 0, 1132265472, 0, 1118476698);
	rotationAddMovabilityToRotation(80015, 80014, "1045", 3276, -29, 3355, 29, 1, 53, 0);
	rotationSetMovabilityToRotation(80015, 1, 1133477888, 0, 1118476698, 0, 0, 1133477888, 0, 1118476698);
	rotationAddMovabilityToRotation(80015, 80003, "1046", 3079, 212, 3236, 345, 1, 53, 0);
	rotationSetMovabilityToRotation(80015, 2, 1132920832, 1104150528, 1118476698, 0, 0, 1132920832, 0, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle80002, 80101, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle80002, 0, 1112276992, 0, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle80003, 80101, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle80003, 0, 1117126656, 0, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle80004, 80101, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle80004, 0, 1120665600, 0, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle80005, 80101, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle80005, 0, 1124204544, 0, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle80006, 80101, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle80006, 0, 1119092736, 1099431936, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle80007, 80101, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle80007, 0, 1119092736, 1099431936, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle80008, 80101, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle80008, 0, 1119092736, 1099431936, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle80009, 80101, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle80009, 0, 1119092736, 1099431936, 1118476698);
	puzzleAddMovabilityToRotation(kPuzzle80010, 80101, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle80010, 0, 1119092736, 1099431936, 1118476698);
	objectAdd(kObject80017, "", "", 1);
	objectAdd(kObject80007, "", "", 1);
	objectAddRotationAccessibility(kObject80007, 80101, Common::Rect(1314, 183, 1397, 226), 1, 52, 0);
	objectAdd(kObjectDeath, "Death", "AS_Nor_d", 1);
	objectAddBagAnimation(kObjectDeath, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectDeath, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectDeath, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObject80012, "", "", 1);
	objectAddRotationAccessibility(kObject80012, 80005, Common::Rect(3097, -1, 3197, 176), 1, 52, 0);
	objectAddRotationAccessibility(kObject80012, 80005, Common::Rect(1318, -66, 1381, 191), 1, 52, 1);
	objectAddRotationAccessibility(kObject80012, 80008, Common::Rect(3095, -27, 3215, 73), 1, 52, 2);
	objectAddRotationAccessibility(kObject80012, 80005, Common::Rect(384, -61, 478, 197), 1, 52, 3);
	objectAddRotationAccessibility(kObject80012, 80005, Common::Rect(2225, -59, 2308, 190), 1, 52, 4);
	objectAddRotationAccessibility(kObject80012, 80005, Common::Rect(467, -195, 606, 3), 1, 52, 5);
	objectAddRotationAccessibility(kObject80012, 80005, Common::Rect(2088, -167, 2235, 8), 1, 52, 6);
	objectAddRotationAccessibility(kObject80012, 80011, Common::Rect(1202, -156, 1552, 355), 1, 52, 7);
	objectAdd(kObject80016, "", "", 1);
	objectAddPresentation(kObject80016);
	objectPresentationAddImageToRotation(kObject80016, 0, 80001, 0);
	objectAddPresentation(kObject80016);
	objectPresentationAddImageToRotation(kObject80016, 1, 80002, 0);
	objectAddPresentation(kObject80016);
	objectPresentationAddImageToRotation(kObject80016, 2, 80003, 0);
	objectAddPresentation(kObject80016);
	objectPresentationAddImageToRotation(kObject80016, 3, 80004, 0);
	objectAddPresentation(kObject80016);
	objectPresentationAddImageToRotation(kObject80016, 4, 80005, 0);
	objectAddPresentation(kObject80016);
	objectPresentationAddImageToRotation(kObject80016, 5, 80006, 0);
	objectAddPresentation(kObject80016);
	objectPresentationAddImageToRotation(kObject80016, 6, 80007, 0);
	objectAddPresentation(kObject80016);
	objectPresentationAddImageToRotation(kObject80016, 7, 80008, 0);
	objectAddPresentation(kObject80016);
	objectPresentationAddImageToRotation(kObject80016, 8, 80009, 0);
	objectAddPresentation(kObject80016);
	objectPresentationAddImageToRotation(kObject80016, 9, 80010, 0);
	objectAddPresentation(kObject80016);
	objectPresentationAddImageToRotation(kObject80016, 10, 80011, 0);
	objectAddPresentation(kObject80016);
	objectPresentationAddImageToRotation(kObject80016, 11, 80013, 0);
	objectAddPresentation(kObject80016);
	objectPresentationAddImageToRotation(kObject80016, 12, 80014, 0);
	objectAddPresentation(kObject80016);
	objectPresentationAddImageToRotation(kObject80016, 13, 80015, 0);
	objectAdd(kObject80018, "", "", 1);
	objectAddRotationAccessibility(kObject80018, 80101, Common::Rect(843, -66, 1008, 188), 0, 52, 0);
	objectAddRotationAccessibility(kObject80018, 80101, Common::Rect(1147, -104, 1277, 136), 0, 52, 1);
	objectAddRotationAccessibility(kObject80018, 80101, Common::Rect(1418, -97, 1562, 130), 0, 52, 2);
	objectAddRotationAccessibility(kObject80018, 80101, Common::Rect(1698, -99, 1849, 186), 0, 52, 3);
	objectAddPresentation(kObject80018);
	objectPresentationAddImageToRotation(kObject80018, 0, 80101, 0);
	objectAddPresentation(kObject80018);
	objectPresentationAddAnimationToRotation(kObject80018, 1, 80101, 1, 49, 1095237632, 4);
	objectPresentationSetAnimationOnRotation(kObject80018, 1, 0, 80001);
	objectPresentationShow(kObject80018, 1);
	objectPresentationPauseAnimation(kObject80018, 1);
	objectAddPresentation(kObject80018);
	objectPresentationAddImageToRotation(kObject80018, 2, 80101, 2);
	objectAddPresentation(kObject80018);
	objectPresentationAddImageToRotation(kObject80018, 3, 80101, 3);
	objectAddPresentation(kObject80018);
	objectPresentationAddImageToRotation(kObject80018, 4, 80101, 4);
	objectAddPresentation(kObject80018);
	objectPresentationAddImageToRotation(kObject80018, 5, 80101, 5);
	objectAdd(kObject80019, "", "", 1);
	objectAddPuzzleAccessibility(kObject80019, kPuzzle80006, Common::Rect(159, 161, 477, 393), 1, 52, 0);
	objectAddPuzzleAccessibility(kObject80019, kPuzzle80006, Common::Rect(159, 161, 477, 393), 0, 52, 1);
	objectAddPuzzleAccessibility(kObject80019, kPuzzle80007, Common::Rect(159, 161, 477, 393), 1, 52, 2);
	objectAddPuzzleAccessibility(kObject80019, kPuzzle80008, Common::Rect(159, 161, 477, 393), 1, 52, 3);
	objectAddPuzzleAccessibility(kObject80019, kPuzzle80009, Common::Rect(159, 161, 477, 393), 1, 52, 4);
	objectAddPuzzleAccessibility(kObject80019, kPuzzle80010, Common::Rect(159, 161, 477, 393), 1, 52, 5);
	objectSetAccessibilityOnOrOff(kObject80019, 1, 1);
	objectAdd(kObject80021, "", "", 1);
	objectAddRotationAccessibility(kObject80021, 80101, Common::Rect(1248, 235, 1466, 348), 1, 52, 0);
	objectAddRotationAccessibility(kObject80021, 80101, Common::Rect(1089, 170, 1232, 268), 1, 52, 1);
	objectAddRotationAccessibility(kObject80021, 80101, Common::Rect(1181, 125, 1341, 190), 1, 52, 2);
	objectAddRotationAccessibility(kObject80021, 80101, Common::Rect(1353, 130, 1552, 183), 1, 52, 3);
	objectAddRotationAccessibility(kObject80021, 80101, Common::Rect(1444, 183, 1620, 249), 1, 52, 4);
	objectAddPresentation(kObject80019);

	_loadFrom = kLoadFromDisk;
	_archiveType = kArchiveFile;

	objectPresentationAddImageToPuzzle(kObject80019, 0, kPuzzle80006, "ASP01L02.bma", 0, 16, 1, 1, 1000);

	_loadFrom = kLoadFromCd;
	if (_configuration.artAS)
		_archiveType = kArchiveArt;

	objectAdd(kObject80022, "", "", 1);
	objectAddRotationAccessibility(kObject80022, 80101, Common::Rect(0, -580, 3600, -279), 1, 53, 0);
	soundAdd(80200, 2, "1048.was", _configuration.ambientMusic.loadFrom, 2, _configuration.ambientMusic.soundChunck);
	soundAdd(80201, 2, "1049.was", _configuration.ambientMusic.loadFrom, 2, _configuration.ambientMusic.soundChunck);
	soundAdd(80203, 2, "1050.was", _configuration.ambientMusic.loadFrom, 2, _configuration.ambientMusic.soundChunck);
	soundAdd(80204, 2, "1051.was", _configuration.ambientMusic.loadFrom, 2, _configuration.ambientMusic.soundChunck);
	soundAdd(80205, 2, "1052.was", _configuration.ambientMusic.loadFrom, 2, _configuration.ambientMusic.soundChunck);
	soundAdd(80207, 2, "1053.wav", _configuration.ambientMusic.loadFrom, 2, _configuration.ambientMusic.soundChunck);
	rotationAddAmbientSound(80001, 80200, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(80002, 80200, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(80003, 80200, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(80004, 80200, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(80005, 80200, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(80006, 80200, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(80007, 80200, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(80008, 80200, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(80009, 80200, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(80010, 80200, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(80011, 80200, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(80013, 80200, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(80014, 80200, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(80015, 80200, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(80101, 80200, 100, 0, 1, 1, 10);
	rotationAddAmbientSound(80101, 80207, 100, 0, 1, 1, 20);
	puzzleAddAmbientSound(kPuzzle80002, 80201, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle80003, 80203, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle80004, 80204, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle80005, 80205, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle80006, 80207, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle80007, 80207, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle80008, 80207, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle80010, 80207, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle80009, 80207, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle80006, 80200, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle80007, 80200, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle80008, 80200, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle80010, 80200, 100, 0, 1, 1, 10);
	puzzleAddAmbientSound(kPuzzle80009, 80200, 100, 0, 1, 1, 10);
	soundAdd(80206, 3, "1053.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(80018, 3, "1054.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(80019, 3, "1055.was", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(80020, 3, "1056.was", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(80208, 3, "1057.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(80209, 3, "1058.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(80210, 3, "1059.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundSetVolume(80210, 90);
	soundAdd(80004, 5, "1060.was", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80005, 5, "1061.was", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80006, 5, "1062.was", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80007, 5, "1063.was", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80008, 5, "1064.was", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80009, 5, "1065.was", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80010, 5, "1066.was", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80011, 5, "1067.was", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80012, 5, "1068.was", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80013, 5, "1069.was", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80014, 5, "1070.was", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80015, 5, "1071.was", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80040, 5, "1072.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80041, 5, "1073.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80042, 5, "1074.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80043, 5, "1075.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80044, 5, "1076.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80045, 5, "1077.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80046, 5, "1078.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80047, 5, "1079.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80048, 5, "1080.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80049, 5, "1081.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80050, 5, "1082.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80051, 5, "1083.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80052, 5, "1084.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80053, 5, "1085.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80054, 5, "1086.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80055, 5, "1087.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80056, 5, "1088.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80057, 5, "1089.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80058, 5, "1090.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80059, 5, "1091.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80060, 5, "1092.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80061, 5, "1093.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80062, 5, "1094.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80063, 5, "1095.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80064, 5, "1096.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80065, 5, "1097.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80066, 5, "1098.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80067, 5, "1099.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80068, 5, "1100.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80069, 5, "1101.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80070, 5, "1102.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80071, 5, "1103.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80072, 5, "1104.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80073, 5, "1105.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80074, 5, "1106.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80075, 5, "1107.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80076, 5, "1108.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80077, 5, "1109.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80078, 5, "1110.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80079, 5, "1111.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80080, 5, "1112.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80021, 5, "1125.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80022, 5, "1126.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80023, 5, "1127.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80024, 5, "1128.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80025, 5, "1129.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80026, 5, "1130.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80027, 5, "1131.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80028, 5, "1132.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80100, 5, "1133.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80101, 5, "1134.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80102, 5, "1135.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80103, 5, "1136.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80104, 5, "1137.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80105, 5, "1138.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80106, 5, "1139.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(80107, 5, "1140.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	rotationAdd3DSound(80001, 80206, 1, 1, 10, 90, 270.0, 20);
	varDefineByte(80000, 0);
	varDefineByte(80001, 0);
	varDefineByte(80002, 1);
	varDefineByte(80003, 0);
	varDefineByte(80004, 1);
	varDefineByte(80005, 0);
	varDefineFloat(80001, 1.0);
	varDefineFloat(80002, 2.0);
	varDefineFloat(80003, 100.0);
}

void ApplicationRing::initZoneN2() {
	setZoneAndEnableBag(kZoneN2);

	rotationAdd(70000, "N2S00N01", 0, 1);
	rotationAdd(70001, "N2S00N02", 0, 3);
	rotationAdd(70100, "N2S01N01", 0, 1);
	rotationAdd(70101, "N2S01N02", 0, 1);
	rotationAdd(70300, "N2S03N01", 0, 2);
	rotationAdd(70400, "N2S04N01", 0, 0);
	rotationAdd(70500, "N2S05N01", 0, 2);
	rotationAdd(70600, "N2S06N01", 0, 0);
	rotationAdd(70200, "N2S07N01", 0, 1);
	puzzleAdd(kPuzzle72001);
	puzzleAddBackgroundImage(kPuzzle72001, "TR_AS_RO_BS.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle72002);
	puzzleAddBackgroundImage(kPuzzle72002, "TR_AS_RO_CS.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle72003);
	puzzleAddBackgroundImage(kPuzzle72003, "TR_AS_RO_DS.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle72004);
	puzzleAddBackgroundImage(kPuzzle72004, "TR_AS_RO_ES.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle72005);
	puzzleAddBackgroundImage(kPuzzle72005, "TR_AS_RO_FS.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle72006);
	puzzleAddBackgroundImage(kPuzzle72006, "TR_AS_RO_HS.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle72007);
	puzzleAddBackgroundImage(kPuzzle72007, "TR_AS_RO_IS.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle72008);
	puzzleAddBackgroundImage(kPuzzle72008, "TR_AS_RO_JS.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle72009);
	puzzleAddBackgroundImage(kPuzzle72009, "TR_AS_RO_KS.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle72010);
	puzzleAddBackgroundImage(kPuzzle72010, "TR_AS_RO_MS.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle70000);
	puzzleAddBackgroundImage(kPuzzle70000, "N2EXTRA_1A.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle70100);
	puzzleAddBackgroundImage(kPuzzle70100, "N2S01N01P02.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle70102);
	puzzleAddBackgroundImage(kPuzzle70102, "N2S01N01P03.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle70300);
	puzzleAddBackgroundImage(kPuzzle70300, "N2S03N01P01S01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle70301);
	puzzleAddBackgroundImage(kPuzzle70301, "N2S03N01P01S02.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle70302);
	puzzleAddBackgroundImage(kPuzzle70302, "N2S03N01P01S04.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle70303);
	puzzleAddBackgroundImage(kPuzzle70303, "N2S02N01P02S02.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle70304);
	puzzleAddBackgroundImage(kPuzzle70304, "N2S02N01P02S03.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle70305);
	puzzleAddBackgroundImage(kPuzzle70305, "N2S00N01P01S01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle70306);
	puzzleAddBackgroundImage(kPuzzle70306, "N2S03N01P01S03.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle70410);
	puzzleAddBackgroundImage(kPuzzle70410, "N2S04N03P01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle70411);
	puzzleAddBackgroundImage(kPuzzle70411, "N2S04N03P03.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle70500);
	puzzleAddBackgroundImage(kPuzzle70500, "N2S05N01P01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle70501);
	puzzleAddBackgroundImage(kPuzzle70501, "N2S05N01P01L01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle70510);
	puzzleAddBackgroundImage(kPuzzle70510, "N2S05N01P02.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle70511);
	puzzleAddBackgroundImage(kPuzzle70511, "N2S05N01P02L01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle70520);
	puzzleAddBackgroundImage(kPuzzle70520, "N2S05N01P03.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle70521);
	puzzleAddBackgroundImage(kPuzzle70521, "N2S05N01P03L01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle70600);
	puzzleAddBackgroundImage(kPuzzle70600, "N2S07N01P01S01.0001.bmp", 0, 16, 1);
	puzzleAdd(kPuzzle70601);
	puzzleAddBackgroundImage(kPuzzle70601, "N2S07N01P01S04.0001.bmp", 0, 16, 1);
	rotationAddMovabilityToRotation(70000, 70001, "1373", 1585, -94, 1815, 111, 1, 53, 7);
	rotationSetMovabilityToRotation(70000, 0, 0, 0, 1118529126, 0, 2, 1119092736, 0, 1118529126);
	rotationAddMovabilityToRotation(70000, 70400, "1374", 2515, -165, 2796, 106, 1, 53, 3);
	rotationSetMovabilityToRotation(70000, 1, 0, 0, 1118529126, 0, 2, 1132920832, 0, 1118529126);
	rotationAddMovabilityToRotation(70000, 70200, "1375", 314, -261, 817, 167, 1, 53, 0);
	rotationSetMovabilityToRotation(70000, 2, 0, 0, 1118529126, 0, 2, 1127809024, 0, 1118529126);
	rotationAddMovabilityToRotation(70001, 70000, "1376", 3397, -81, 3600, 129, 1, 53, 7);
	rotationSetMovabilityToRotation(70001, 0, 0, 0, 1118529126, 0, 2, 1132920832, 0, 1118529126);
	rotationAddMovabilityToRotation(70001, 70300, "1377", 2132, -198, 2640, 211, 1, 53, 0);
	rotationSetMovabilityToRotation(70001, 1, 0, 0, 1118529126, 0, 2, 1125515264, 0, 1118529126);
	rotationAddMovabilityToRotation(70001, 70100, "1378", 1008, 31, 1218, 230, 1, 53, 0);
	rotationSetMovabilityToRotation(70001, 2, 0, 0, 1118529126, 0, 2, 1132920832, 0, 1118529126);
	rotationAddMovabilityToRotation(70001, 70400, "1379", 3227, -76, 3364, 61, 1, 53, 0);
	rotationSetMovabilityToRotation(70001, 3, 0, 0, 1118529126, 0, 2, 1132920832, 0, 1118529126);
	rotationAddMovabilityToRotation(70100, 70101, "1380", 1186, -143, 1534, 195, 1, 53, 0);
	rotationSetMovabilityToRotation(70100, 0, 0, 1050253722, 1118529126, 0, 2, 1119092736, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(70101, 70100, "1381", 2951, -125, 3350, 193, 1, 53, 0);
	rotationSetMovabilityToRotation(70101, 0, 0, 1050253722, 1118529126, 0, 2, 1132920832, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(70101, 70600, "1382", 1323, -57, 1587, 225, 1, 53, 16);
	rotationSetMovabilityToRotation(70101, 1, 0, 1050253722, 1118529126, 0, 2, 1124204544, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(70101, 70600, "1383", 1323, -57, 1587, 225, 0, 53, 0);
	rotationSetMovabilityToRotation(70101, 2, 0, 1050253722, 1118529126, 0, 2, 1130364928, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(70200, 70000, "1384", 108, -160, 578, 156, 1, 53, 7);
	rotationSetMovabilityToRotation(70200, 0, 0, 1050253722, 1118529126, 0, 2, 1127481344, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(70300, 70001, "1385", 318, -202, 789, 226, 1, 53, 7);
	rotationSetMovabilityToRotation(70300, 0, 0, 1050253722, 1118529126, 0, 2, 1134559232, 1050253722, 1118529126);
	rotationAddMovabilityToRotation(70400, 70000, "1386", 1200, -127, 1508, 143, 1, 53, 7);
	rotationSetMovabilityToRotation(70400, 0, 0, 0, 1118529126, 0, 2, 1119092736, 0, 1118529126);
	rotationAddMovabilityToPuzzle(70400, kPuzzle70410, "", 3011, -211, 3280, 29, 1, 53, 0);
	rotationSetMovabilityToPuzzle(70400, 1, 0, 0, 1118529126, 0, 2);
	rotationAddMovabilityToRotation(70500, 70600, "1387", 546, -345, 980, 120, 1, 53, 0);
	rotationSetMovabilityToRotation(70500, 0, 0, 1050253722, 1118529126, 0, 2, 1110441984, 1050253722, 1118529126);
	rotationAddMovabilityToPuzzle(70500, kPuzzle70500, "", 1139, -204, 1520, 156, 1, 52, 0);
	rotationSetMovabilityToPuzzle(70500, 1, 0, 1050253722, 1118529126, 0, 2);
	rotationAddMovabilityToPuzzle(70500, kPuzzle70510, "", 1787, -143, 2033, 108, 1, 52, 0);
	rotationSetMovabilityToPuzzle(70500, 2, 0, 1050253722, 1118529126, 0, 2);
	rotationAddMovabilityToPuzzle(70500, kPuzzle70520, "", 2228, -118, 2411, 80, 1, 52, 0);
	rotationSetMovabilityToPuzzle(70500, 3, 0, 1050253722, 1118529126, 0, 2);
	rotationAddMovabilityToRotation(70600, 70101, "1388", 863, -211, 1123, 101, 1, 53, 0);
	rotationSetMovabilityToRotation(70600, 0, 0, 1050253722, 1118529126, 0, 2, 1132920832, 1050253722, 1118529126);
	puzzleAddMovabilityToPuzzle(kPuzzle70102, 70100, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleAddMovabilityToPuzzle(kPuzzle70410, 70411, "", Common::Rect(250, 280, 320, 370), 1, 52, 0);
	puzzleAddMovabilityToRotation(kPuzzle70410, 70400, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle70410, 1, 1132920832, 1050253722, 1118529126);
	puzzleAddMovabilityToPuzzle(kPuzzle70411, 70410, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleAddMovabilityToRotation(kPuzzle70500, 70500, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle70500, 0, 1119092736, 1050253722, 1118529126);
	puzzleAddMovabilityToPuzzle(kPuzzle70500, 70501, "", Common::Rect(180, 80, 470, 370), 1, 52, 0);
	puzzleAddMovabilityToPuzzle(kPuzzle70501, 70500, "", Common::Rect(430, 48, 640, 434), 1, 52, 0);
	puzzleAddMovabilityToRotation(kPuzzle70510, 70500, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle70510, 0, 1125122048, 1050253722, 1118529126);
	puzzleAddMovabilityToPuzzle(kPuzzle70510, 70511, "", Common::Rect(180, 80, 470, 370), 1, 52, 0);
	puzzleAddMovabilityToPuzzle(kPuzzle70511, 70510, "", Common::Rect(430, 48, 640, 434), 1, 52, 0);
	puzzleAddMovabilityToRotation(kPuzzle70520, 70500, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	puzzleSetMovabilityToRotation(kPuzzle70520, 0, 1127743488, 1050253722, 1118529126);
	puzzleAddMovabilityToPuzzle(kPuzzle70520, 70521, "", Common::Rect(180, 80, 470, 370), 1, 52, 0);
	puzzleAddMovabilityToPuzzle(kPuzzle70521, 70520, "", Common::Rect(430, 48, 640, 434), 1, 52, 0);
	objectAdd(kObject72000, "", "", 1);
	objectAddPresentation(kObject72000);
	objectPresentationAddAnimationToPuzzle(kObject72000, 0, kPuzzle72001, "TR_AS_RO_BS", 0, 375, 203, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject72000);
	objectPresentationAddAnimationToPuzzle(kObject72000, 1, kPuzzle72002, "TR_AS_RO_CS", 0, 281, 154, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject72000);
	objectPresentationAddAnimationToPuzzle(kObject72000, 2, kPuzzle72003, "TR_AS_RO_DS", 0, 284, 163, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject72000);
	objectPresentationAddAnimationToPuzzle(kObject72000, 3, kPuzzle72004, "TR_AS_RO_ES", 0, 147, 140, 1, 1000, 23, 1095237632, 4);
	objectAddPresentation(kObject72000);
	objectPresentationAddAnimationToPuzzle(kObject72000, 4, kPuzzle72005, "TR_AS_RO_FS", 0, 388, 244, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject72000);
	objectPresentationAddAnimationToPuzzle(kObject72000, 5, kPuzzle72006, "TR_AS_RO_HS", 0, 146, 156, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject72000);
	objectPresentationAddAnimationToPuzzle(kObject72000, 6, kPuzzle72007, "TR_AS_RO_IS", 0, 308, 221, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject72000);
	objectPresentationAddAnimationToPuzzle(kObject72000, 7, kPuzzle72008, "TR_AS_RO_JS", 0, 250, 102, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject72000);
	objectPresentationAddAnimationToPuzzle(kObject72000, 8, kPuzzle72009, "TR_AS_RO_KS", 0, 224, 262, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject72000);
	objectPresentationAddAnimationToPuzzle(kObject72000, 9, kPuzzle72010, "TR_AS_RO_MS", 0, 173, 127, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObject70001, "Alberich", "", 1);
	objectAddPresentation(kObject70001);
	objectPresentationAddAnimationToPuzzle(kObject70001, 0, kPuzzle70000, "N2EXTRA_1B", 0, 1, 153, 1, 1000, 53, 1092616192, 4);
	objectAddPresentation(kObject70001);
	objectPresentationAddAnimationToPuzzle(kObject70001, 1, kPuzzle70000, "N2EXTRA_2B", 0, 0, 16, 1, 1000, 37, 1092616192, 4);
	objectAddPresentation(kObject70001);
	objectPresentationAddAnimationToPuzzle(kObject70001, 2, kPuzzle70000, "N2EXTRA_3B", 0, 49, 38, 1, 1000, 53, 1092616192, 4);
	objectAddPresentation(kObject70001);
	objectPresentationAddAnimationToRotation(kObject70001, 3, 70001, 2, 16, 1095237632, 4);
	objectPresentationSetAnimationOnRotation(kObject70001, 3, 0, 70000);
	objectPresentationShow(kObject70001, 3);
	objectAdd(kObjectFire, "Fire", "N2_Fire", 1);
	objectAddBagAnimation(kObjectFire, 1, 3, 13, 1095237632, 4);
	objectSetActiveCursor(kObjectFire, 22, 22, 13, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectFire, 22, 22, 0, 3, 0, 0, 3);
	objectAddRotationAccessibility(kObjectFire, 70600, Common::Rect(2413, -256, 2970, 256), 1, 52, 0);
	objectAddPresentation(kObjectFire);
	objectPresentationAddAnimationToRotation(kObjectFire, 0, 70000, 0, 25, 1092616192, 4);
	objectPresentationAddAnimationToRotation(kObjectFire, 0, 70001, 0, 25, 1092616192, 4);
	objectPresentationAddAnimationToRotation(kObjectFire, 0, 70001, 1, 25, 1092616192, 4);
	objectPresentationShow(kObjectFire, 0);
	objectAdd(kObject70100, "Console", "", 1);
	objectAddPuzzleAccessibility(kObject70100, kPuzzle70100, Common::Rect(0, 16, 640, 66), 1, 53, 0);
	objectAddPuzzleAccessibility(kObject70100, kPuzzle70100, Common::Rect(0, 414, 640, 464), 1, 53, 0);
	objectAddPuzzleAccessibility(kObject70100, kPuzzle70100, Common::Rect(0, 66, 50, 414), 1, 53, 0);
	objectAddPuzzleAccessibility(kObject70100, kPuzzle70100, Common::Rect(610, 66, 640, 414), 1, 53, 0);
	objectAddRotationAccessibility(kObject70100, 70100, Common::Rect(3406, 26, 3541, 155), 1, 52, 1);
	objectAddPresentation(kObject70100);
	objectPresentationAddImageToPuzzle(kObject70100, 0, kPuzzle70100, "N2S01N01P01L01.0001.bmp", 101, 68, 1, 1, 1000);
	objectAddPresentation(kObject70100);
	objectPresentationAddImageToPuzzle(kObject70100, 1, kPuzzle70100, "N2S01N01P01L02.0001.bmp", 80, 63, 1, 1, 1000);
	objectAddPresentation(kObject70100);
	objectPresentationAddImageToPuzzle(kObject70100, 2, kPuzzle70100, "N2S01N01P01L03.0001.bmp", 116, 159, 1, 1, 1000);
	objectAdd(kObject70101, "CCButton1", "", 2);
	objectAddRotationAccessibility(kObject70101, 70100, Common::Rect(3208, 109, 3322, 184), 1, 52, 1);
	objectAddPuzzleAccessibility(kObject70101, kPuzzle70100, Common::Rect(359, 265, 440, 304), 0, 52, 0);
	objectAddPuzzleAccessibility(kObject70101, kPuzzle70100, Common::Rect(359, 265, 440, 304), 0, 52, 1);
	objectAddPresentation(kObject70101);
	objectPresentationAddImageToPuzzle(kObject70101, 0, kPuzzle70100, "N2S01N01P01S03.0001.bmp", 361, 259, 1, 1, 1000);
	objectAddPresentation(kObject70101);
	objectPresentationAddAnimationToPuzzle(kObject70101, 1, kPuzzle70100, "N2S01N01P01S01", 0, 85, 68, 1, 1000, 36, 1095237632, 6);
	objectPresentationSetAnimationOnPuzzle(kObject70101, 1, 0, 70101);
	objectAddPresentation(kObject70101);
	objectPresentationAddAnimationToPuzzle(kObject70101, 2, kPuzzle70100, "N2S01N01P01L02S01", 0, 80, 63, 1, 1000, 36, 1095237632, 10);
	objectPresentationSetAnimationOnPuzzle(kObject70101, 2, 0, 70103);
	objectAddPresentation(kObject70101);
	objectPresentationAddAnimationToPuzzle(kObject70101, 3, kPuzzle70100, "N2S01N01P01S01", 0, 85, 68, 1, 1000, 36, 1095237632, 10);
	objectPresentationSetAnimationOnPuzzle(kObject70101, 3, 0, 70100);
	objectAddPresentation(kObject70101);
	objectPresentationAddAnimationToPuzzle(kObject70101, 4, kPuzzle70100, "N2S01N01P01L02S01", 0, 80, 63, 1, 1000, 36, 1095237632, 6);
	objectPresentationSetAnimationOnPuzzle(kObject70101, 4, 0, 70102);
	objectAddPresentation(kObject70101);
	objectPresentationAddImageToPuzzle(kObject70101, 5, kPuzzle70100, "N2S01N01P01S03.0003.bmp", 361, 259, 1, 1, 1000);
	objectAddPresentation(kObject70101);
	objectPresentationAddImageToRotation(kObject70101, 6, 70100, 0);
	objectPresentationAddImageToRotation(kObject70101, 6, 70101, 0);
	objectPresentationShow(kObject70101, 6);
	objectAdd(kObject70102, "CCButton2", "", 2);
	objectAddRotationAccessibility(kObject70102, 70100, Common::Rect(3250, 195, 3357, 245), 1, 52, 1);
	objectAddPuzzleAccessibility(kObject70102, kPuzzle70100, Common::Rect(382, 308, 472, 352), 1, 52, 0);
	objectAddPuzzleAccessibility(kObject70102, kPuzzle70100, Common::Rect(382, 308, 472, 352), 1, 52, 1);
	objectAddPresentation(kObject70102);
	objectPresentationAddImageToPuzzle(kObject70102, 0, kPuzzle70100, "N2S01N01P01S03.0004.bmp", 385, 299, 1, 1, 1000);
	objectAddPresentation(kObject70102);
	objectPresentationAddAnimationToPuzzle(kObject70102, 1, kPuzzle70100, "N2S01N01P01S02", 0, 40, 65, 1, 1000, 46, 1095237632, 6);
	objectPresentationSetAnimationOnPuzzle(kObject70102, 1, 0, 70105);
	objectAddPresentation(kObject70102);
	objectPresentationAddAnimationToPuzzle(kObject70102, 2, kPuzzle70100, "N2S01N01P01S02", 0, 40, 65, 1, 1000, 23, 1095237632, 6);
	objectPresentationSetAnimationOnPuzzle(kObject70102, 2, 0, 70104);
	objectAddPresentation(kObject70102);
	objectPresentationAddAnimationToPuzzle(kObject70102, 3, kPuzzle70100, "N2S01N01P01S02", 0, 40, 65, 1, 1000, 23, 1095237632, 10);
	objectPresentationSetAnimationOnPuzzle(kObject70102, 3, 0, 70106);
	objectAddPresentation(kObject70102);
	objectPresentationAddImageToPuzzle(kObject70102, 4, kPuzzle70100, "N2S01N01P01S03.0002.bmp", 385, 299, 1, 1, 1000);
	objectAdd(kObject70103, "CCHandle", "NI_HandSel", 4);
	objectAddRotationAccessibility(kObject70103, 70100, Common::Rect(3406, 26, 3541, 155), 1, 52, 0);
	objectAddPuzzleAccessibility(kObject70103, kPuzzle70100, Common::Rect(497, 192, 543, 249), 1, 10000, 1);
	objectSetActiveDrawCursor(kObject70103, 5, 5, 0, 3, 0, 0, 3);
	objectSetPassiveDrawCursor(kObject70103, 5, 5, 0, 3, 0, 0, 3);

	for (uint32 i = 0; i < 14; i++) {
		objectAddPresentation(kObject70103);
		objectPresentationAddImageToPuzzle(kObject70103, i, kPuzzle70100, Common::String::format("N2S01N01P02S01.%04d.bmp", i + 1), 504, 194, 1, 1, 1000);
	}

	objectAdd(kObject70105, "CCHolo", "", 1);
	objectAddPuzzleAccessibility(kObject70105, kPuzzle70100, Common::Rect(77, 132, 340, 298), 0, 52, 0);
	objectAddPuzzleAccessibility(kObject70105, kPuzzle70100, Common::Rect(140, 170, 430, 380), 0, 52, 0);
	objectAdd(kObject70106, "CCHoloCross", "NI_HandSel", 4);
	objectAddPuzzleAccessibility(kObject70106, kPuzzle70102, Common::Rect(203, 236, 283, 316), 1, 10000, 0);
	objectSetActiveDrawCursor(kObject70106, 5, 5, 0, 3, 0, 0, 3);
	objectSetPassiveDrawCursor(kObject70106, 5, 5, 0, 3, 0, 0, 3);

	for (uint32 i = 0; i < 20; i++) {
		objectAddPresentation(kObject70106);
		objectPresentationAddImageToPuzzle(kObject70106, i, kPuzzle70102, Common::String::format("N2S01N01P03L01S01.%04d.bmp", i + 1), 213, 254, 1, 1, 1000);
	}

	objectAdd(kObject70107, "CCHoloDam1", "NI_HandSel", 4);
	objectAdd(kObject70108, "CCHoloDam2", "NI_HandSel", 4);
	objectAddPuzzleAccessibility(kObject70108, kPuzzle70102, Common::Rect(389, 270, 434, 330), 1, 10000, 0);
	objectAddPuzzleAccessibility(kObject70108, kPuzzle70102, Common::Rect(389, 330, 434, 390), 0, 10000, 1);
	objectSetActiveDrawCursor(kObject70108, 5, 5, 0, 3, 0, 0, 3);
	objectSetPassiveDrawCursor(kObject70108, 5, 5, 0, 3, 0, 0, 3);

	for (uint32 i = 0; i < 15; i++) {
		objectAddPresentation(kObject70108);
		objectPresentationAddImageToPuzzle(kObject70108, i, kPuzzle70102, Common::String::format("N2S01N01P03L01S03.%04d.bmp", i + 1), 390, 271, 1, 1, 999);
	}

	objectAddPresentation(kObject70108);
	objectPresentationAddImageToPuzzle(kObject70108, 14, kPuzzle70102, "N2S01N01P03L01S03A.bmp", 340, 237, 1, 1, 1000);
	objectPresentationAddImageToPuzzle(kObject70108, 14, kPuzzle70102, "N2S01N01P03L01S03B.bmp", 311, 179, 1, 1, 1000);
	objectAdd(kObjectLogeTear3, "Loge's Tear", "NI_Tear", 1);
	objectAddBagAnimation(kObjectLogeTear3, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectLogeTear3, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectLogeTear3, 22, 22, 0, 3, 0, 0, 3);
	objectAdd(kObject70300, "Mime", "", 1);
	objectAddRotationAccessibility(kObject70300, 70300, Common::Rect(1937, -26, 2079, 88), 1, 52, 0);
	objectAddPresentation(kObject70300);
	objectPresentationAddAnimationToRotation(kObject70300, 0, 70300, 0, 25, 1092616192, 4);
	objectPresentationSetAnimationOnRotation(kObject70300, 0, 0, 70300);
	objectPresentationShow(kObject70300, 0);
	objectAddPresentation(kObject70300);
	objectPresentationAddAnimationToPuzzle(kObject70300, 1, kPuzzle70300, "N2S03N01P01S01", 0, 260, 73, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject70300);
	objectPresentationAddAnimationToPuzzle(kObject70300, 2, kPuzzle70301, "N2S03N01P01S02", 0, 289, 79, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject70300);
	objectPresentationAddAnimationToPuzzle(kObject70300, 3, kPuzzle70302, "N2S03N01P01S04", 0, 305, 117, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject70300);
	objectPresentationAddAnimationToPuzzle(kObject70300, 4, kPuzzle70303, "N2S02N01P02S02", 0, 311, 166, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject70300);
	objectPresentationAddAnimationToPuzzle(kObject70300, 5, kPuzzle70304, "N2S02N01P02S03", 0, 97, 185, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject70300);
	objectAddPresentation(kObject70300);
	objectPresentationAddAnimationToPuzzle(kObject70300, 7, kPuzzle70306, "N2S03N01P01S03", 0, 270, 135, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObjectCage, "Cage", "N2_Cage", 1);
	objectAddBagAnimation(kObjectCage, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectCage, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectCage, 22, 22, 0, 3, 0, 0, 3);
	objectAddPresentation(kObjectCage);
	objectPresentationAddImageToRotation(kObjectCage, 0, 70300, 1);
	objectAdd(kObject70404, "TearCasing", "", 3);
	objectAddPuzzleAccessibility(kObject70404, kPuzzle70411, Common::Rect(189, 189, 362, 377), 1, 52, 0);
	objectAddPuzzleAccessibility(kObject70404, kPuzzle70411, Common::Rect(224, 90, 362, 209), 0, 52, 1);
	objectAddPuzzleAccessibility(kObject70404, kPuzzle70411, Common::Rect(293, 241, 475, 462), 0, 52, 2);
	objectAddPresentation(kObject70404);
	objectPresentationAddImageToPuzzle(kObject70404, 0, kPuzzle70411, "N2S04N03P03L01.0001.bmp", 214, 173, 1, 1, 1000);
	objectAddPresentation(kObject70404);
	objectPresentationAddImageToPuzzle(kObject70404, 1, kPuzzle70411, "N2S04N03P03L02.0001.bmp", 270, 116, 1, 1, 1000);
	objectAddPresentation(kObject70404);
	objectPresentationAddAnimationToPuzzle(kObject70404, 2, kPuzzle70411, "N2S04N03P03L01S01", 0, 217, 171, 1, 1000, 27, 1095237632, 10);
	objectPresentationSetAnimationOnPuzzle(kObject70404, 2, 0, 70422);
	objectAddPresentation(kObject70404);
	objectPresentationAddAnimationToPuzzle(kObject70404, 3, kPuzzle70411, "N2S04N03P03L01S01", 0, 217, 171, 1, 1000, 27, 1095237632, 6);
	objectPresentationSetAnimationOnPuzzle(kObject70404, 3, 0, 70423);
	objectAddPresentation(kObject70404);
	objectPresentationAddAnimationToPuzzle(kObject70404, 4, kPuzzle70411, "N2S04N03P03L02S01", 0, 215, 111, 1, 1000, 26, 1095237632, 10);
	objectPresentationSetAnimationOnPuzzle(kObject70404, 4, 0, 70424);
	objectAddPresentation(kObject70404);
	objectPresentationAddAnimationToPuzzle(kObject70404, 5, kPuzzle70411, "N2S04N03P03L02S01", 0, 215, 111, 1, 1000, 26, 1095237632, 6);
	objectPresentationSetAnimationOnPuzzle(kObject70404, 5, 0, 70425);
	objectAddPresentation(kObject70404);
	objectPresentationAddAnimationToPuzzle(kObject70404, 6, kPuzzle70411, "N2S04N03P03L02S02", 0, 275, 126, 1, 1000, 20, 1095237632, 4);
	objectAdd(kObject70406, "", "", 1);
	objectAddPresentation(kObject70406);
	objectPresentationAddAnimationToPuzzle(kObject70406, 0, kPuzzle70410, "N2S04N03P01S02", 0, 171, 285, 1, 1000, 13, 1092616192, 32);
	objectPresentationAddAnimationToPuzzle(kObject70406, 0, kPuzzle70410, "N2S04N03P01S03", 0, 244, 268, 1, 1000, 12, 1092616192, 32);
	objectPresentationAddAnimationToPuzzle(kObject70406, 0, kPuzzle70410, "N2S04N03P01S04", 0, 317, 261, 1, 1000, 7, 1092616192, 32);
	objectPresentationShow(kObject70406, 0);
	objectAdd(kObject70405, "Temperat", "", 1);
	objectAddPresentation(kObject70405);

	for (uint32 i = 0; i < 13; i++) {
		objectAddPresentation(kObject70405);
		objectPresentationAddImageToPuzzle(kObject70405, i, kPuzzle70410, Common::String::format("N2S04N03P01S01.%04d.bmp", i + 1), 107, 155, 1, 1, 1000);
	}

	objectAdd(kObjectCentaur, "Centaur", "N2_Centaur", 1);
	objectAddBagAnimation(kObjectCentaur, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectCentaur, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectCentaur, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectCentaur, kPuzzle70501, Common::Rect(270, 250, 364, 363), 1, 52, 0);
	objectAddPuzzleAccessibility(kObjectCentaur, kPuzzle70000, Common::Rect(320, 16, 640, 464), 1, 52, 1);
	objectAddPresentation(kObjectCentaur);
	objectPresentationAddImageToPuzzle(kObjectCentaur, 0, kPuzzle70501, "N2S05N01P01L02.0001.bmp", 278, 258, 1, 1, 1000);
	objectPresentationShow(kObjectCentaur);
	objectAdd(kObjectDragon, "Dragon", "N2_Dragon", 1);
	objectAddBagAnimation(kObjectDragon, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectDragon, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectDragon, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectDragon, kPuzzle70511, Common::Rect(290, 260, 380, 371), 1, 52, 0);
	objectAddPuzzleAccessibility(kObjectDragon, kPuzzle70000, Common::Rect(320, 16, 640, 464), 1, 52, 1);
	objectAddPresentation(kObjectDragon);
	objectPresentationAddImageToPuzzle(kObjectDragon, 0, kPuzzle70511, "N2S05N01P02L02.0001.bmp", 291, 263, 1, 1, 1000);
	objectPresentationShow(kObjectDragon);
	objectAdd(kObjectPhoenix1, "Phoenix", "N2_Phoenix", 1);
	objectAddBagAnimation(kObjectPhoenix1, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectPhoenix1, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectPhoenix1, 22, 22, 0, 3, 0, 0, 3);
	objectAddPuzzleAccessibility(kObjectPhoenix1, kPuzzle70521, Common::Rect(265, 255, 356, 358), 1, 52, 0);
	objectAddPuzzleAccessibility(kObjectPhoenix1, kPuzzle70000, Common::Rect(320, 16, 640, 464), 1, 52, 1);
	objectAddPresentation(kObjectPhoenix1);
	objectPresentationAddImageToPuzzle(kObjectPhoenix1, 0, kPuzzle70521, "N2S05N01P03L02.0001.bmp", 268, 255, 1, 1, 1000);
	objectPresentationShow(kObjectPhoenix1);
	objectAdd(kObjectChrysoberyl, "Chrysoberyl", "N2_Beryllium", 1);
	objectAddBagAnimation(kObjectChrysoberyl, 1, 3, 20, 1095237632, 4);
	objectSetActiveCursor(kObjectChrysoberyl, 22, 22, 20, 4, 1095237632, 4, 4);
	objectSetPassiveCursor(kObjectChrysoberyl, 22, 22, 0, 3, 0, 0, 3);
	objectAddRotationAccessibility(kObjectChrysoberyl, 70500, Common::Rect(3483, -22, 3600, 155), 1, 52, 0);
	objectAddRotationAccessibility(kObjectChrysoberyl, 70500, Common::Rect(0, -22, 103, 155), 1, 52, 0);
	objectAddPresentation(kObjectChrysoberyl);
	objectPresentationAddImageToRotation(kObjectChrysoberyl, 0, 70500, 0);
	objectPresentationAddImageToRotation(kObjectChrysoberyl, 0, 70500, 1);
	objectAdd(kObject70700, "Nib", "", 1);
	objectAddRotationAccessibility(kObject70700, 70200, Common::Rect(3188, -62, 3331, 123), 1, 52, 0);
	objectAddRotationAccessibility(kObject70700, 70200, Common::Rect(3188, -62, 3331, 123), 0, 52, 1);
	objectAddPresentation(kObject70700);
	objectPresentationAddAnimationToRotation(kObject70700, 0, 70200, 0, 30, 1095237632, 4);
	objectAddPresentation(kObject70700);
	objectPresentationAddAnimationToPuzzle(kObject70700, 1, kPuzzle70600, "N2S07N01P01S01", 0, 93, 16, 1, 1000, 20, 1095237632, 4);
	objectAddPresentation(kObject70700);
	objectPresentationAddAnimationToPuzzle(kObject70700, 2, kPuzzle70601, "N2S07N01P01S04", 0, 286, 160, 1, 1000, 20, 1095237632, 4);
	objectPresentationShow(kObject70700, 0);
	soundAdd(70000, 1, "1392.was", _configuration.backgroundMusic.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(70101, 3, "1394.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(70300, 3, "1395.was", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(70301, 3, "1396.wav", _configuration.ambientEffect.loadFrom);
	soundAdd(70412, 3, "1397.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(70501, 3, "1399.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(70502, 3, "1400.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(70102, 3, "1401.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(70103, 3, "1402.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(70104, 3, "1403.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(70105, 3, "1404.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(70107, 3, "1405.was", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(70701, 3, "1406.was", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundAdd(70405, 4, "1412.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(70406, 4, "1413.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(70407, 4, "1414.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(70408, 4, "1415.wav", _configuration.effect.loadFrom, 2, _configuration.effect.soundChunck);
	soundAdd(72001, 5, "1417.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(72002, 5, "1418.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(72003, 5, "1419.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(72004, 5, "1420.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(72005, 5, "1421.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(72006, 5, "1422.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(72007, 5, "1423.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(72008, 5, "1424.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(72009, 5, "1425.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(72010, 5, "1426.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70001, 5, "1427.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70002, 5, "1428.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70003, 5, "1429.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70004, 5, "1430.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70005, 5, "1431.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70006, 5, "1432.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70007, 5, "1433.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70008, 5, "1434.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70009, 5, "1435.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70010, 5, "1436.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70011, 5, "1437.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70012, 5, "1438.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70013, 5, "1439.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70014, 5, "1440.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70015, 5, "1441.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70016, 5, "1442.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70017, 5, "1443.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70018, 5, "1444.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70019, 5, "1445.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70020, 5, "1446.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70021, 5, "1447.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70022, 5, "1448.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70023, 5, "1449.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70024, 5, "1450.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70025, 5, "1451.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70026, 5, "1452.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70027, 5, "1453.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(71001, 5, "1454.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(71010, 5, "1455.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(71002, 5, "1456.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(71011, 5, "1457.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(71003, 5, "1458.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(71012, 5, "1459.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(71100, 5, "1460.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(71101, 5, "1461.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(71102, 5, "1462.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(71111, 5, "1463.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(71110, 5, "1464.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(71112, 5, "1465.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(71122, 5, "1466.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(71120, 5, "1467.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(71121, 5, "1468.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70043, 5, "1469.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70044, 5, "1470.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70045, 5, "1471.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70046, 5, "1472.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70047, 5, "1473.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70048, 5, "1474.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70049, 5, "1475.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70050, 5, "1476.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70051, 5, "1477.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70052, 5, "1478.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70053, 5, "1479.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70054, 5, "1480.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70055, 5, "1481.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70056, 5, "1482.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70057, 5, "1483.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70058, 5, "1484.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70059, 5, "1485.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70060, 5, "1486.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70061, 5, "1487.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70062, 5, "1488.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70063, 5, "1489.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70064, 5, "1490.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70065, 5, "1491.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(70066, 5, "1492.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	rotationAdd3DSound(70300, 70301, 1, 1, 20, 100, 160.0, 20);
	rotationSet3DSoundOff(70300, 70301);
	rotationAdd3DSound(70300, 70300, 1, 1, 20, 100, 160.0, 5);
	rotationAdd3DSound(70400, 70412, 1, 1, 20, 90, 270.0, 10);
	puzzleAdd3DSound(kPuzzle70410, 70412, 1, 1, 2, 93, 300.0, 5);
	puzzleAdd3DSound(kPuzzle70411, 70412, 1, 1, 2, 95, 300.0, 5);
	rotationAdd3DSound(70000, 70107, 1, 1, 50, 100, 135.0, 8);
	rotationAdd3DSound(70001, 70107, 1, 1, 50, 100, 315.0, 8);
	rotationAdd3DSound(70400, 70107, 1, 1, 10, 93, 90.0, 20);
	rotationAdd3DSound(70100, 70107, 1, 1, 10, 93, 270.0, 5);
	rotationAdd3DSound(70101, 70107, 1, 1, 10, 88, 270.0, 5);
	puzzleAdd3DSound(kPuzzle70100, 70107, 1, 1, 2, 93, 27.0, 5);
	puzzleAdd3DSound(kPuzzle70102, 70107, 1, 1, 2, 93, 27.0, 5);
	rotationAdd3DSound(70200, 70701, 1, 1, 20, 100, 285.0, 5);
	varDefineByte(70001, 1);
	varDefineByte(70005, 1);
	varDefineByte(70010, 0);
	varDefineByte(70011, 0);
	varDefineByte(70012, 0);
	varDefineByte(70013, 0);
	varDefineByte(70014, 1);
	varDefineByte(70015, 97);
	varDefineWord(70016, 0);
	varDefineString(70099, "3231323131 76766     34        020212121212        776000    ");
}

} // End of namespace Ring
