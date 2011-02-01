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

#include "ring/game/game.h"

#include "ring/base/art.h"
#include "ring/base/sound.h"

#include "ring/game/bag.h"

#include "ring/game/application.h"
#include "ring/game/saveload.h"

#include "ring/helpers.h"

#include "common/archive.h"

namespace Ring {

Game::Game(Application *application) : _app(application) {
	_saveLoad = new SaveLoad();
}

Game::~Game() {
	SAFE_DELETE(_saveLoad);
}

//////////////////////////////////////////////////////////////////////////
// Game setup
//////////////////////////////////////////////////////////////////////////
void Game::setup() {
	setupZone(kZoneSY, 0);

	// Setup cursors
	ArchiveType archiveType = (_app->getConfiguration().artCURSOR ? kArchiveArt : kArchiveFile);

	_app->cursorAdd(kCursorDefault,       "",                kCursorTypeNormal,   1, kImageCursor, archiveType);
	_app->cursorAdd(kCursorBusy,          "cur_busy",        kCursorTypeImage,    1, kImageCursor, archiveType);
	_app->cursorAdd(kCursorHandSelection, "ni_handsel",      kCursorTypeImage,    1, kImageCursor, archiveType);
	_app->cursorAdd(kCursorIdle,          "cur_idle",        kCursorTypeAnimated, 1, 15, 1095237632, 4, kImageCursor, archiveType);
	_app->cursorAdd(kCursorMove,          "cur_muv",         kCursorTypeAnimated, 1, 20, 1095237632, 4, kImageCursor, archiveType);
	_app->cursorAdd(kCursorHotspot,       "cur_hotspot",     kCursorTypeAnimated, 1, 19, 1095237632, 4, kImageCursor, archiveType);
	_app->cursorAdd(kCursorBack,          "cur_back",        kCursorTypeImage,    1, kImageCursor, archiveType);
	_app->cursorAdd(kCursorMenuIdle,      "cur_menuidle",    kCursorTypeNormal,   1, kImageCursor, archiveType);
	_app->cursorAdd(kCursorMenuActive,    "cur_menuactive",  kCursorTypeImage,    1, kImageCursor, archiveType);

	// Adjust offsets
	_app->cursorSetOffset(kCursorHandSelection, Common::Point(15, 15));
	_app->cursorSetOffset(kCursorIdle, Common::Point(10, 6));
	_app->cursorSetOffset(kCursorMove, Common::Point(10, 6));
	_app->cursorSetOffset(kCursorHotspot, Common::Point(10, 6));
	_app->cursorSetOffset(kCursorBack, Common::Point(10, 20));

	// Setup subtitles
	_app->subtitleSetColor(Color(255, 255, 255));
	_app->subtitleSetBackgroundColor(Color(50, 50, 50));
}

//////////////////////////////////////////////////////////////////////////
// Zone init and setup
//////////////////////////////////////////////////////////////////////////
void Game::setupZone(Zone zone, uint32 a2)  {
	bool load = _saveLoad->isLoaded(a2);

	// Check saved data for zone and/or puzzle id
	if (!load) {
		if (zone == kZoneSY) {
			load = true;
		} else {
			error("[Game::setupZone] Zone CD check not implemented");
		}
	}

	_app->puzzleReset();
	warning("[Game::setupZone] Missing function call");

	_app->getSoundHandler()->reset();

	if (zone != kZoneSY)
		_app->getArtHandler()->remove();

	if (load) {
		setZoneAndEnableBag(zone);
		setZone(zone, a2);
	} else {
		error("[Game::setupZone] CD check not implemented");
	}
}

void Game::setZoneAndEnableBag(Zone zone) {
	_app->setCurrentZone(zone);

	// Enable or disable bag
	if (zone == kZoneSY || zone == kZoneAS)
		_app->getBag()->disable();
	else
		_app->getBag()->enable();
}

void Game::setZone(Zone zone, uint32 a2) {
	bool load = _saveLoad->isLoaded(a2);

	if (zone != kZoneSY && !load) {
		if (_app->getReadFrom(zone) == kArchiveArt) {
			if (!_app->getArtHandler())
				error("[Game::setZone] Art handler is not initialized properly");

			_app->getArtHandler()->open(zone, kLoadFromCd);
		}
	}

	if (a2 == 1000) {
		error("[Game::setZone] Not implemented (a2 == 1000)");
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

void Game::initZones() {
	_app->setField5D(2);

	_app->setArchiveType(_app->getConfiguration().artSY ? kArchiveArt : kArchiveFile);
	initZoneSY();

	_app->setField5D(1);

	_app->setArchiveType(_app->getConfiguration().artAS ? kArchiveArt : kArchiveFile);
	initZoneAS();

	_app->setArchiveType(_app->getConfiguration().artNI ? kArchiveArt : kArchiveFile);
	initZoneNI();

	_app->setArchiveType(_app->getConfiguration().artN2 ? kArchiveArt : kArchiveFile);
	initZoneN2();

	_app->setArchiveType(_app->getConfiguration().artRO ? kArchiveArt : kArchiveFile);
	initZoneRO();

	_app->setArchiveType(_app->getConfiguration().artRH ? kArchiveArt : kArchiveFile);
	initZoneRH();

	_app->setArchiveType(_app->getConfiguration().artFO ? kArchiveArt : kArchiveFile);
	initZoneFO();

	_app->setArchiveType(_app->getConfiguration().artWA ? kArchiveArt : kArchiveFile);
	initZoneWA();

	if (_app->getConfiguration().artSY || _app->getConfiguration().artAS || _app->getConfiguration().artNI || _app->getConfiguration().artN2
	 || _app->getConfiguration().artRO || _app->getConfiguration().artRH || _app->getConfiguration().artFO || _app->getConfiguration().artWA)
		_app->setArchiveType(kArchiveArt);
	else
		_app->setArchiveType(kArchiveFile);

	_app->setField66(0);
}

void Game::setZoneNI(Zone zone, uint32 a2) {
	error("[Game::setZoneNI] Not implemented");
}

void Game::setZoneRH(Zone zone, uint32 a2) {
	error("[Game::setZoneRH] Not implemented");
}

void Game::setZoneFO(Zone zone, uint32 a2) {
	error("[Game::setZoneFO] Not implemented");
}

void Game::setZoneRO(Zone zone, uint32 a2) {
	error("[Game::setZoneRO] Not implemented");
}

void Game::setZoneWA(Zone zone, uint32 a2) {
	error("[Game::setZoneWA] Not implemented");
}

void Game::setZoneAS(Zone zone, uint32 a2) {
	error("[Game::setZoneAS] Not implemented");
}

void Game::setZoneN2(Zone zone, uint32 a2) {
	error("[Game::setZoneN2] Not implemented");
}

void Game::initZoneSY() {
	setZoneAndEnableBag(kZoneSY);

	//////////////////////////////////////////////////////////////////////////
	// Setup system and dialog boxes
	_app->puzzleAdd(kPuzzle1);

	_app->objectAdd(kObject1, "", "", 1);
	_app->objectAddPresentation(kObject1);
	_app->objectPresentationAddTextToPuzzle(kObject1, 0, kPuzzle1, "", 1, 16, 1, 0, -1, 0, -1, -1, -1);

	_app->objectAdd(kObject6, "", "", 1);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 0, kPuzzle1, "FO_WOT1_W_A.tga", 0, 96, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 1, kPuzzle1, "FO_WOT1_W_B.tga", 0, 176, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 2, kPuzzle1, "FO_WOT1_W_C.tga", 0, 126, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 3, kPuzzle1, "FO_WOT1_W_D.tga", 0, 208, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 4, kPuzzle1, "FO_WOT1_W_E.tga", 0, 88, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 5, kPuzzle1, "FO_WOT1_B_A.tga", 415, 281, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 6, kPuzzle1, "FO_WOT1_B_C.tga", 488, 180, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 7, kPuzzle1, "FO_WOT1_B_E.tga", 195, 94, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 8, kPuzzle1, "FO_BRU1_W_A.tga", 0, 45, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 9, kPuzzle1, "FO_BRU1_B_A.tga", 440, 66, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 10, kPuzzle1, "FO_BRU1_B_B.tga", 490, 208, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 11, kPuzzle1, "FO_WOT2_W_A.tga", 0, 28, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 12, kPuzzle1, "FO_WOT2_W_B.tga", 0, 22, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 13, kPuzzle1, "FO_WOT2_W_C.tga", 0, 67, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 14, kPuzzle1, "FO_WOT2_W_D.tga", 0, 194, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 15, kPuzzle1, "FO_WOT2_B_A.tga", 477, 200, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 16, kPuzzle1, "FO_WOT2_B_C.tga", 496, 251, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 17, kPuzzle1, "FO_BRU2_W_A.tga", 0, 186, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 18, kPuzzle1, "FO_BRU2_B_A.tga", 443, 222, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 19, kPuzzle1, "FO_BRU2_B_B.tga", 493, 219, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 20, kPuzzle1, "FO_WOT3_W_A.tga", 0, 16, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 21, kPuzzle1, "FO_WOT3_W_B.tga", 0, 107, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 22, kPuzzle1, "FO_WOT3_W_C.tga", 0, 46, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 23, kPuzzle1, "FO_WOT3_W_D.tga", 0, 26, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 24, kPuzzle1, "FO_WOT3_B_A.tga", 508, 238, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 25, kPuzzle1, "FO_WOT3_B_B.tga", 381, 216, 1, 3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 26, kPuzzle1, "FO_WOT3_B_D.tga", 408, 246, 1, 3, 1000);

	_app->objectAdd(kObject7, "", "", 1);
	_app->objectAddPresentation(kObject7);
	_app->objectPresentationAddImageToPuzzle(kObject7, 0, kPuzzle1, "SY_ISHA.bmp", 0, 16, 1, 1, 1000);

	_app->objectAdd(kObject2, "", "", 1);
	_app->objectAddPresentation(kObject2);
	_app->objectPresentationAddImageToPuzzle(kObject2, 0, kPuzzle1, "Exit.bmp", 160, 165, 1, 1, 1000);
	_app->objectAddPuzzleAccessibility(kObject2, kPuzzle1, Common::Rect(262, 270, 321, 306), 0, 57, 1);
	_app->objectAddPuzzleAccessibility(kObject2, kPuzzle1, Common::Rect(310, 270, 370, 306), 0, 57, 0);
	_app->objectSetPuzzleAccessibilityKey(kObject2, 0, 13);
	_app->objectSetPuzzleAccessibilityKey(kObject2, 1, 27);
	_app->objectAddPresentation(kObject2);
	_app->objectPresentationAddImageToPuzzle(kObject2, 1, kPuzzle1, "ex_yes.bmp", 261, 279, 1, 1, 1000);
	_app->objectAddPresentation(kObject2);
	_app->objectPresentationAddImageToPuzzle(kObject2, 2, kPuzzle1, "ex_no.bmp", 318, 279, 1, 1, 1000);

	_app->objectAdd(kObject3, "", "", 1);
	_app->objectAddPresentation(kObject3);
	_app->objectPresentationAddImageToPuzzle(kObject3, 0, kPuzzle1, "Warning.bmp", 160, 165, 1, 1, 1000);
	_app->objectPresentationAddTextToPuzzle(kObject3, 0, kPuzzle1, "", 200, 200, 1, 255u, 95, 0, -1, -1, -1);
	_app->objectPresentationAddTextToPuzzle(kObject3, 0, kPuzzle1, "", 300, 280, 1, 255u, 95, 0, -1, -1, -1);
	_app->objectAddPresentation(kObject3);
	_app->objectPresentationAddImageToPuzzle(kObject3, 1, kPuzzle1, "wr_ok.tga", 313, 281, 1, 3, 1000);
	_app->objectAddPuzzleAccessibility(kObject3, kPuzzle1, Common::Rect(286, 269, 363, 307), 0, 57, 0);
	_app->objectSetPuzzleAccessibilityKey(kObject3, 0, 13);

	_app->objectAdd(kObject4, "", "", 1);
	_app->objectAddPresentation(kObject4);
	_app->objectPresentationAddImageToPuzzle(kObject4, 0, kPuzzle1, "Question.bmp", 160, 165, 1, 1, 1000);
	_app->objectPresentationAddTextToPuzzle(kObject4, 0, kPuzzle1, "", 200, 200, 1, 255u, 95, 0, -1, -1, -1);
	_app->objectPresentationAddTextToPuzzle(kObject4, 0, kPuzzle1, "", 200, 280, 1, 255u, 95, 0, -1, -1, -1);
	_app->objectAddPresentation(kObject4);
	_app->objectPresentationAddImageToPuzzle(kObject4, 1, kPuzzle1, "g_ok.tga", 181, 257, 1, 3, 1000);
	_app->objectAddPresentation(kObject4);
	_app->objectPresentationAddImageToPuzzle(kObject4, 2, kPuzzle1, "qu_cancel.tga", 181, 282, 1, 3, 1000);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzle1, Common::Rect(277, 300, 347, 340), 0, 57, 0);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzle1, Common::Rect(350, 300, 380, 340), 0, 57, 1);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 0, 13);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 1, 27);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzle1, Common::Rect(180, 250, 250, 281), 0, 57, 2);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzle1, Common::Rect(180, 283, 250, 309), 0, 57, 3);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 2, 13);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 3, 27);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzle1, Common::Rect(180, 250, 250, 281), 0, 57, 4);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzle1, Common::Rect(180, 283, 250, 309), 0, 57, 5);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 4, 13);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 5, 27);

	_app->objectAdd(kObject5, "", "", 1);
	_app->puzzleAdd(kPuzzleInsertCd);
	_app->puzzleAddBackgroundImage(kPuzzleInsertCd, "insertcd.bmp", 0, 16, 1);

	_app->objectAdd(kObject90912, "", "", 1);
	_app->objectAddPresentation(kObject90912);
	_app->objectPresentationAddImageToPuzzle(kObject90912, 0, kPuzzleInsertCd, "g_ok.tga", 306, 274, 1, 3, 1000);
	_app->objectAddPresentation(kObject90912);
	_app->objectPresentationAddTextToPuzzle(kObject90912, 1, kPuzzleInsertCd, "", 320, 240, 1, 0xFFu, 95, 0, -1, -1, -1);
	_app->objectPresentationAddTextToPuzzle(kObject90912, 1, kPuzzleInsertCd, "", 320, 260, 1, 0xFFu, 95, 0, -1, -1, -1);
	_app->objectPresentationShow(kObject90912, 1);
	_app->objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), 1, 57, 0);
	_app->objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), 1, 57, 1);
	_app->objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), 1, 57, 2);
	_app->objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), 1, 57, 3);
	_app->objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), 1, 57, 4);
	_app->objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), 1, 57, 5);
	_app->objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), 1, 57, 6);
	_app->objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), 1, 57, 7);
	_app->objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), 1, 57, 8);
	_app->objectSetPuzzleAccessibilityKey(kObject90912, 0, 13);
	_app->objectSetPuzzleAccessibilityKey(kObject90912, 1, 13);
	_app->objectSetPuzzleAccessibilityKey(kObject90912, 2, 13);
	_app->objectSetPuzzleAccessibilityKey(kObject90912, 3, 13);
	_app->objectSetPuzzleAccessibilityKey(kObject90912, 4, 13);
	_app->objectSetPuzzleAccessibilityKey(kObject90912, 5, 13);
	_app->objectSetPuzzleAccessibilityKey(kObject90912, 6, 13);
	_app->objectSetPuzzleAccessibilityKey(kObject90912, 7, 13);
	_app->objectSetPuzzleAccessibilityKey(kObject90912, 8, 13);

	//////////////////////////////////////////////////////////////////////////
	// Menu
	_app->puzzleAdd(kPuzzleMenu);
	_app->puzzleAddBackgroundImage(kPuzzleMenu, "GenMen.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzlePreferences);
	_app->puzzleAddBackgroundImage(kPuzzlePreferences, "Preferences.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzleSave);
	_app->puzzleAddBackgroundImage(kPuzzleSave, "Save.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzleLoad);
	_app->puzzleAddBackgroundImage(kPuzzleLoad, "Load.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzleGameStatus);
	_app->puzzleAddBackgroundImage(kPuzzleGameStatus, "GameStat.bmp", 0, 16, 1);

	// Menu objects
	_app->objectAdd(kObjectMenuNewGame, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectMenuNewGame, kPuzzleMenu, Common::Rect(148, 69, 500, 99), 1, 57, 0);
	_app->objectAddPresentation(kObjectMenuNewGame);
	_app->objectPresentationAddImageToPuzzle(kObjectMenuNewGame, 0, kPuzzleMenu, "gm_new.bmp", 148, 85, 1, 1, 1000);
	_app->objectAdd(kObjectMenuPreferences, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectMenuPreferences, kPuzzleMenu, Common::Rect(148, 105, 500, 135), 1, 57, 0);
	_app->objectAddPresentation(kObjectMenuPreferences);
	_app->objectPresentationAddImageToPuzzle(kObjectMenuPreferences, 0, kPuzzleMenu, "gm_pre.bmp", 148, 121, 1, 1, 1000);
	_app->objectAdd(kObjectMenuLoad, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectMenuLoad, kPuzzleMenu, Common::Rect(148, 168, 500, 198), 1, 57, 0);
	_app->objectAddPresentation(kObjectMenuLoad);
	_app->objectPresentationAddImageToPuzzle(kObjectMenuLoad, 0, kPuzzleMenu, "gm_loa.bmp", 148, 184, 1, 1, 1000);
	_app->objectAdd(kObjectMenuSave, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectMenuSave, kPuzzleMenu, Common::Rect(148, 236, 500, 266), 1, 57, 0);
	_app->objectAddPresentation(kObjectMenuSave);
	_app->objectPresentationAddImageToPuzzle(kObjectMenuSave, 0, kPuzzleMenu, "gm_sav.bmp", 148, 252, 1, 1, 1000);
	_app->objectAdd(kObjectMenuContinue, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectMenuContinue, kPuzzleMenu, Common::Rect(148, 303, 500, 333), 1, 57, 0);
	_app->objectAddPresentation(kObjectMenuContinue);
	_app->objectPresentationAddImageToPuzzle(kObjectMenuContinue, 0, kPuzzleMenu, "gm_con.bmp", 148, 319, 1, 1, 1000);
	_app->objectAdd(kObjectMenuStatus, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectMenuStatus, kPuzzleMenu, Common::Rect(148, 342, 500, 372), 1, 57, 0);
	_app->objectAddPresentation(kObjectMenuStatus);
	_app->objectPresentationAddImageToPuzzle(kObjectMenuStatus, 0, kPuzzleMenu, "gm_sta.bmp", 148, 358, 1, 1, 1000);
	_app->objectAdd(kObjectMenuExit, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectMenuExit, kPuzzleMenu, Common::Rect(148, 380, 500, 410), 1, 57, 0);
	_app->objectAddPresentation(kObjectMenuExit);
	_app->objectPresentationAddImageToPuzzle(kObjectMenuExit, 0, kPuzzleMenu, "gm_exi.bmp", 148, 396, 1, 1, 1000);

	// Preferences objects
	_app->objectAdd(kObjectPreferencesCancel, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectPreferencesCancel, kPuzzlePreferences, Common::Rect(410, 420, 490, 445), 1, 57, 0);
	_app->objectAddPresentation(kObjectPreferencesCancel);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferencesCancel, 0, kPuzzlePreferences, "g_cancel.tga", 407, 421, 1, 3, 1000);
	_app->objectSetPuzzleAccessibilityKey(kObjectPreferencesCancel, 0, 27);
	_app->objectAdd(kObjectPreferencesOk, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectPreferencesOk, kPuzzlePreferences, Common::Rect(320, 420, 370, 445), 1, 57, 0);
	_app->objectAddPresentation(kObjectPreferencesOk);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferencesOk, 0, kPuzzlePreferences, "g_ok.tga", 328, 421, 1, 3, 1000);
	_app->objectSetPuzzleAccessibilityKey(kObjectPreferencesOk, 0, 13);
	_app->objectAdd(kObjectPreferencesSubtitles, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectPreferencesSubtitles, kPuzzlePreferences, Common::Rect(310, 315, 370, 350), 1, 57, 0);
	_app->objectAddPuzzleAccessibility(kObjectPreferencesSubtitles, kPuzzlePreferences, Common::Rect(400, 315, 460, 350), 1, 57, 1);
	_app->objectAddPresentation(kObjectPreferencesSubtitles);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferencesSubtitles, 0, kPuzzlePreferences, "pr_on.bmp", 317, 326, 1, 1, 1000);
	_app->objectAddPresentation(kObjectPreferencesSubtitles);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferencesSubtitles, 1, kPuzzlePreferences, "pr_off.bmp", 402, 326, 1, 1, 1000);
	_app->objectAdd(kObjectPreferences3dSound, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectPreferences3dSound, kPuzzlePreferences, Common::Rect(355, 260, 420, 295), 1, 57, 1);
	_app->objectAddPresentation(kObjectPreferences3dSound);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferences3dSound, 0, kPuzzlePreferences, "pr_left.tga", 336, 288, 1, 3, 1000);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferences3dSound, 0, kPuzzlePreferences, "pr_right.tga", 428, 288, 1, 3, 1000);
	_app->objectAddPresentation(kObjectPreferences3dSound);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferences3dSound, 1, kPuzzlePreferences, "pr_right.tga", 336, 288, 1, 3, 1000);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferences3dSound, 1, kPuzzlePreferences, "pr_left.tga", 428, 288, 1, 3, 1000);
	_app->objectAddPresentation(kObjectPreferences3dSound);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferences3dSound, 2, kPuzzlePreferences, "pr_3ds.tga", 356, 281, 1, 3, 1000);
	_app->objectAdd(kObjectPreferencesSliderVolume, "", "ni_handsel", 4);
	_app->objectAddPuzzleAccessibility(kObjectPreferencesSliderVolume, kPuzzlePreferences, Common::Rect(300, 140, 600, 180), 1, 57, 1);
	_app->objectAddPresentation(kObjectPreferencesSliderVolume);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferencesSliderVolume, 0, kPuzzlePreferences, "pr_slider.tga", 314, 155, 1, 3, 1000);
	_app->objectPresentationShow(kObjectPreferencesSliderVolume);
	_app->objectSetActiveDrawCursor(kObjectPreferencesSliderVolume, 15, 15, 0, 3, 0, 0, 3);
	_app->objectSetPassiveDrawCursor(kObjectPreferencesSliderVolume, 15, 15, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectPreferencesSliderDialog, "", "ni_handsel", 4);
	_app->objectAddPuzzleAccessibility(kObjectPreferencesSliderDialog, kPuzzlePreferences, Common::Rect(300, 197, 600, 237), 1, 57, 1);
	_app->objectAddPresentation(kObjectPreferencesSliderDialog);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferencesSliderDialog, 0, kPuzzlePreferences, "pr_slider.tga", 314, 212, 1, 3, 1000);
	_app->objectPresentationShow(kObjectPreferencesSliderDialog);
	_app->objectSetActiveDrawCursor(kObjectPreferencesSliderDialog, 15, 15, 0, 3, 0, 0, 3);
	_app->objectSetPassiveDrawCursor(kObjectPreferencesSliderDialog, 15, 15, 0, 3, 0, 0, 3);
	_app->objectAdd(kObject90107, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject90107, kPuzzlePreferences, Common::Rect(0, 448, 20, 640), 1, 57, 1);

	// Loading screen objects
	_app->objectAdd(kObjectLoadOk, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectLoadOk, kPuzzleLoad, Common::Rect(325, 418, 375, 461), 1, 57, 0);
	_app->objectAddPresentation(kObjectLoadOk);
	_app->objectPresentationAddImageToPuzzle(kObjectLoadOk, 0, kPuzzleLoad, "g_ok.tga", 328, 421, 1, 3, 1000);
	_app->objectSetPuzzleAccessibilityKey(kObjectLoadOk, 0, 13);
	_app->objectAdd(kObjectLoadCancel, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectLoadCancel, kPuzzleLoad, Common::Rect(416, 418, 498, 461), 1, 57, 0);
	_app->objectAddPresentation(kObjectLoadCancel);
	_app->objectPresentationAddImageToPuzzle(kObjectLoadCancel, 0, kPuzzleLoad, "g_cancel.tga", 407, 421, 1, 3, 1000);
	_app->objectSetPuzzleAccessibilityKey(kObjectLoadCancel, 0, 27);

	// Saving screen objects
	_app->objectAdd(kObjectSaveOk, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectSaveOk, kPuzzleSave, Common::Rect(325, 418, 375, 461), 1, 57, 0);
	_app->objectAdd(kObjectSaveCancel, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectSaveCancel, kPuzzleSave, Common::Rect(416, 418, 498, 461), 1, 57, 0);
	_app->objectSetPuzzleAccessibilityKey(kObjectSaveCancel, 0, 27);
	_app->objectAddPresentation(kObjectSaveOk);
	_app->objectPresentationAddImageToPuzzle(kObjectSaveOk, 0, kPuzzleSave, "g_ok.tga", 328, 421, 1, 3, 1000);
	_app->objectAddPresentation(kObjectSaveCancel);
	_app->objectPresentationAddImageToPuzzle(kObjectSaveCancel, 0, kPuzzleSave, "g_cancel.tga", 407, 421, 1, 3, 1000);
	_app->objectSetPuzzleAccessibilityKey(kObjectSaveOk, 0, 13);
	_app->objectAdd(kObjectSave90313, "", "", 1);
	_app->objectAddPresentation(kObjectSave90313);
	_app->objectPresentationAddTextToPuzzle(kObjectSave90313, 0, kPuzzleSave, "", 344, 181, 1, 0xFFu, 95, 0, -1, -1, -1);
	_app->objectPresentationAddTextToPuzzle(kObjectSave90313, 0, kPuzzleSave, "", 0, 0, 1, 0xFFu, 95, 0, -1, -1, -1);
	_app->objectPresentationAddAnimationToPuzzle(kObjectSave90313, 0, kPuzzleSave, "kybcur", 0, 0, 0, 1, 1000, 6, 1095237632, 16);
	_app->objectPresentationSetAnimationCoordinatesOnPuzzle(kObjectSave90313, 0, Common::Point(346, 181));
	_app->setArchiveType(kArchiveFile);
	_app->objectPresentationAddImageToPuzzle(kObjectSave90313, 0, kPuzzleSave, "osc.bmp", 0, 0, 1, 1, 1000);
	if ( _app->getConfiguration().artSY )
		_app->setArchiveType(kArchiveArt);
	_app->objectPresentationShow(kObjectSave90313, 0);

	_app->visualAddListToPuzzle(1, kPuzzleLoad, 65,
	                            "", "" /* save folder */, "", "up_gun.tga", "up_gur.tga", "", "up_gua.tga", "down_gun.tga", "down_gur.tga", "",
	                            "down_gua.tga", "load_gun.tga", "load_gua.tga",
	                            3, 0, 0, 0, 0, 335, 127, 300, 35, 45,
	                            3, 330, 349, 320, 339, 40, 40, 330, 380, 320,
	                            370, 40, 40, 0, 0, 0, 1, 311, 137, 4,
	                            255, 95, 0, 245, 235, 50, -1, -1,-1, 1,
	                            kArchiveFile);

	// Game status screen
	_app->objectAdd(kObjectStatusOk, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectStatusOk, kPuzzleGameStatus, Common::Rect(28, 79, 107, 109), 1, 57, 0);
	_app->objectAddPresentation(kObjectStatusOk);
	_app->objectPresentationAddImageToPuzzle(kObjectStatusOk, 0, kPuzzleGameStatus, "g_ok.tga", 46, 95, 1, 3, 1000);
	_app->objectSetPuzzleAccessibilityKey(kObjectStatusOk, 0, 13);
	_app->visualAddShowToPuzzle(2, kPuzzleGameStatus, 1, 4, 295, 343, 28, 4, 300, 38655);
	_app->objectAdd(kObjectStatusProgress, "", "", 1);
	_app->objectAddPresentation(kObjectStatusProgress);
	_app->objectPresentationAddTextToPuzzle(kObjectStatusProgress, 0, kPuzzleGameStatus, "", 600, 327, 1, 0xFFu, -106, 0, -1, -1, -1);
	_app->objectPresentationAddTextToPuzzle(kObjectStatusProgress, 0, kPuzzleGameStatus, "", 600, 356, 1, 0xFFu, -106, 0, -1, -1, -1);
	_app->objectPresentationAddTextToPuzzle(kObjectStatusProgress, 0, kPuzzleGameStatus, "", 600, 384, 1, 0xFFu, -106, 0, -1, -1, -1);
	_app->objectPresentationAddTextToPuzzle(kObjectStatusProgress, 0, kPuzzleGameStatus, "", 600, 410, 1, 0xFFu, -106, 0, -1, -1, -1);
	_app->objectPresentationShow(kObjectStatusProgress);

	// Sounds
	_app->soundAddEx(90001, 5, "1844.wac", 2, 2, _app->getConfiguration().dialog.soundChunck);

	// Variables
	_app->varDefineByte(90001, 0);
	_app->varDefineByte(90002, 0);
	_app->varDefineByte(90003, 0);
	_app->varDefineByte(90004, 0);
	_app->varDefineFloat(90005, 0);
	_app->varDefineFloat(90006, 0);
	_app->varDefineFloat(90007, 0);
	_app->varDefineFloat(90008, 0);
	_app->varDefineByte(90009, 0);
	_app->varDefineByte(90010, 0);
	_app->varDefineByte(90011, 0);
	_app->varDefineByte(90012, 0);
	_app->varDefineDword(90013, 0);
	_app->varDefineDword(90014, 0);
	_app->varDefineDword(90015, 0);
	_app->varDefineDword(90016, 0);
	_app->varDefineByte(90017, 0);
	_app->varDefineByte(90018, 0);
	_app->varDefineByte(90019, 0);
	_app->varDefineByte(90020, 0);
	_app->varDefineDword(90021, 0);
	_app->varDefineDword(90022, 0);
	_app->varDefineDword(90023, 0);
	_app->varDefineDword(90024, 0);
	_app->varDefineByte(90025, 0);
	_app->varDefineByte(90026, 0);
	_app->varDefineByte(90027, 0);
	_app->varDefineByte(90028, 0);




	error("[Game::initZoneSY] Not implemented");
}

void Game::initZoneNI() {
	error("[Game::initZoneNI] Not implemented");
}

void Game::initZoneRH() {
	error("[Game::initZoneRH] Not implemented");
}

void Game::initZoneFO() {
	error("[Game::initZoneFO] Not implemented");
}

void Game::initZoneRO() {
	error("[Game::initZoneRO] Not implemented");
}

void Game::initZoneWA() {
	error("[Game::initZoneWA] Not implemented");
}

void Game::initZoneAS() {
	error("[Game::initZoneAS] Not implemented");
}

void Game::initZoneN2() {
	error("[Game::initZoneN2] Not implemented");
}


} // End of namespace Ring
