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
	_app->setLoadFrom(kLoadFromDisk);

	_app->setArchiveType(_app->getConfiguration().artSY ? kArchiveArt : kArchiveFile);
	initZoneSY();

	_app->setLoadFrom(kLoadFromCd);

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
	_app->soundAdd(90001, 5, "1844.wac", kLoadFromDisk, 2, _app->getConfiguration().dialog.soundChunck);

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
}

void Game::initZoneNI() {
	error("[Game::initZoneNI] Not implemented");
}

void Game::initZoneRH() {
	error("[Game::initZoneRH] Not implemented");
}

void Game::initZoneFO() {
	setZoneAndEnableBag(kZoneFO);

	_app->puzzleAdd(kPuzzle35001);
	_app->puzzleAddBackgroundImage(kPuzzle35001, "FOS03N02P01S00.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35002);
	_app->puzzleAddBackgroundImage(kPuzzle35002, "FOS08N01P01.BMP", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35003);
	_app->puzzleAddBackgroundImage(kPuzzle35003, "FOS06N01P06.BMP", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35004);
	_app->puzzleAddBackgroundImage(kPuzzle35004, "FOS06N01P01.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35005);
	_app->puzzleAddBackgroundImage(kPuzzle35005, "FOS06N01P05.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35006);
	_app->puzzleAddBackgroundImage(kPuzzle35006, "FOS06N01P02.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35007);
	_app->puzzleAddBackgroundImage(kPuzzle35007, "FOS06N01P03.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35008);
	_app->puzzleAddBackgroundImage(kPuzzle35008, "FOS06N01P04.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35009);
	_app->puzzleAddBackgroundImage(kPuzzle35009, "FOS06N02P01.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35010);
	_app->puzzleAddBackgroundImage(kPuzzle35010, "FOS07N03P01.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35011);
	_app->puzzleAddBackgroundImage(kPuzzle35011, "FOS03N01P01.BMP", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35019);
	_app->puzzleAddBackgroundImage(kPuzzle35019, "FOS00N04P01.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35020);
	_app->puzzleAddBackgroundImage(kPuzzle35020, "FOS07N04P01.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35100);
	_app->puzzleAddBackgroundImage(kPuzzle35100, "FOS00N03P02.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35101);
	_app->puzzleAddBackgroundImage(kPuzzle35101, "FOS00N03P02L02.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35103);
	_app->puzzleAddBackgroundImage(kPuzzle35103, "FOS01N01P07.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35104);
	_app->puzzleAddBackgroundImage(kPuzzle35104, "FOS01N01P03.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35105);
	_app->puzzleAddBackgroundImage(kPuzzle35105, "FOS01N01P04.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35110);
	_app->puzzleAddBackgroundImage(kPuzzle35110, "FOS01N01P09.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35106);
	_app->puzzleAddBackgroundImage(kPuzzle35106, "FOS01N01P05.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35107);
	_app->puzzleAddBackgroundImage(kPuzzle35107, "FOS01N01P06.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35108);
	_app->puzzleAddBackgroundImage(kPuzzle35108, "FOS01N01P10.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35111);
	_app->puzzleAddBackgroundImage(kPuzzle35111, "FOS01N01P02.bmp", 0, 16, 1);
	_app->puzzleAdd(kPuzzle35109);
	_app->puzzleAddBackgroundImage(kPuzzle35109, "FOS01N01P11.bmp", 0, 16, 1);
	_app->rotationAdd(30001, "FOS00N01", 0, 1);
	_app->rotationAdd(30002, "FOS00N02", 0, 0);
	_app->rotationAdd(30003, "FOS00N03", 0, 1);
	_app->rotationSetComBufferLength(30003, 1400000);
	_app->rotationAdd(30004, "FOS00N04", 0, 0);
	_app->rotationAdd(30005, "FOS00N05", 0, 0);
	_app->rotationAdd(30006, "FOS00N06", 0, 0);
	_app->rotationAdd(30008, "FOS00N08", 0, 1);
	_app->rotationAdd(30009, "FOS00N09", 0, 1);
	_app->rotationAdd(30010, "FOS00N10", 0, 1);
	_app->rotationAdd(30011, "FOS00N11", 0, 3);
	_app->rotationSetComBufferLength(30011, 1500000);
	_app->rotationAdd(30012, "FOS00N12", 0, 0);
	_app->rotationAdd(30101, "FOS01N01", 0, 4);
	_app->rotationSetComBufferLength(30101, 2000000);
	_app->rotationAdd(30301, "FOS03N01", 0, 1);
	_app->rotationAdd(30302, "FOS03N02", 0, 0);
	_app->rotationAdd(30303, "FOS03N03", 0, 0);
	_app->rotationAdd(30401, "FOS04N01", 0, 0);
	_app->rotationAdd(30402, "FOS04N02", 0, 1);
	_app->rotationAdd(30501, "FOS05N01", 0, 3);
	_app->rotationAdd(30601, "FOS06N01", 0, 1);
	_app->rotationAdd(30602, "FOS06N02", 0, 1);
	_app->rotationSetComBufferLength(30602, 1750000);
	_app->rotationAdd(30701, "FOS07N01", 0, 0);
	_app->rotationAdd(30702, "FOS07N02", 0, 1);
	_app->rotationAdd(30703, "FOS07N03", 0, 1);
	_app->rotationAdd(30704, "FOS07N04", 0, 0);
	_app->rotationAdd(30801, "FOS08N01", 0, 0);
	_app->rotationAddMovabilityToRotation(30001, 30003, "1219", 2787, -143, 3113, 169, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30001, 0, 1131872256, 0, 1118476698, 0, 2, 1131872256, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30001, 30004, "1219", 3406, -125, 3599, 73, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30001, 1, 1134297088, 0, 1118476698, 0, 2, 1134297088, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30001, 30004, "1219", 0, -125, 72, 73, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30001, 2, 1134297088, 0, 1118476698, 0, 2, 1134297088, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30002, 30003, "1220", 272, -81, 437, 118, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30002, 0, 1135738880, 0, 1118476698, 0, 2, 1135738880, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30002, 30004, "1220", 89, -104, 254, 45, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30002, 1, 1135149056, 0, 1118476698, 0, 2, 1135149056, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30003, 30001, "1221", 954, -129, 1320, 190, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30003, 0, 1117126656, 0, 1118476698, 0, 2, 1117126656, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30003, 30002, "1222", 2002, -97, 2364, 156, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30003, 1, 1126629376, 0, 1118476698, 0, 2, 1126629376, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30003, 30004, "1223", 45, -104, 261, 136, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30003, 2, 1134886912, 0, 1118476698, 0, 2, 1134886912, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30004, 30001, "1224", 1620, -75, 1773, 52, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30004, 0, 1124073472, 0, 1118476698, 0, 2, 1124073472, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30004, 30002, "1224", 1928, -48, 2037, 38, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30004, 1, 1125449728, 0, 1118476698, 0, 2, 1125449728, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30004, 30003, "1224", 1791, -54, 1900, 73, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30004, 2, 1124728832, 0, 1118476698, 0, 2, 1124728832, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30004, 30005, "1225", 40, -115, 274, 146, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30004, 3, 1134919680, 0, 1118476698, 0, 2, 1134919680, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30004, 30009, "1226", 402, -116, 601, 75, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30004, 4, 0, 0, 1118476698, 0, 2, 0, 0, 1118476698);
	_app->rotationAddMovabilityToPuzzle(30004, kPuzzle35019, "", 1033, -116, 1233, 120, 1, 52, 0);
	_app->rotationSetMovabilityToPuzzle(30004, 5, 1116471296, 0, 1118476698, 0, 2);
	_app->rotationAddMovabilityToRotation(30005, 30004, "1227", 1587, -134, 1829, 87, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30005, 0, 1123418112, 0, 1118476698, 0, 2, 1123418112, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30005, 30006, "1228", 3311, -127, 3513, 113, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30005, 1, 1133641728, 0, 1118476698, 0, 2, 1135738880, -1049624576, 1118476698);
	_app->rotationAddMovabilityToRotation(30006, 30005, "1229", 1817, -179, 2149, 144, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30006, 0, 1125646336, 0, 1118476698, 0, 2, 1125646336, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30006, 30008, "1230", 249, -385, 580, 26, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30006, 1, 1135738880, -1049624576, 1118476698, 0, 2, 1135738880, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30008, 30006, "1231", 2037, 130, 2455, 362, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30008, 0, 1127481344, 1097859072, 1118476698, 0, 2, 1127481344, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30008, 30301, "1232", 358, -43, 567, 113, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30008, 1, 0, 0, 1118476698, 0, 2, 0, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30009, 30004, "1233", 2536, -68, 2698, 87, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30009, 0, 1129840640, 0, 1118476698, 0, 2, 1129840640, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30009, 30010, "1234", 924, -136, 1121, 123, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30009, 1, 1114112000, 0, 1118476698, 0, 2, 1114112000, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30009, 30005, "1233", 2726, -88, 2940, 90, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30009, 2, 1133903872, 0, 1118476698, 0, 2, 1114112000, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30010, 30009, "1235", 2583, -137, 2845, 160, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30010, 0, 1130758144, 0, 1118476698, 0, 2, 1130758144, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30010, 30011, "1236", 1469, -47, 1668, 134, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30010, 1, 1121189888, 0, 1118476698, 0, 2, 1121189888, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30010, 30012, "1237", 573, -52, 703, 108, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30010, 2, 1101529088, 0, 1118476698, 0, 2, 1101529088, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30011, 30010, "1238", 3107, -45, 3309, 163, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30011, 0, 1132756992, 0, 1118476698, 0, 2, 1132756992, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30011, 30401, "1239", 3517, 195, 3600, 418, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30011, 1, 1135345664, 1097859072, 1118476698, 0, 2, 1134886912, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30011, 30401, "1239", 0, 195, 769, 418, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30011, 2, 1135345664, 1097859072, 1118476698, 0, 2, 1134886912, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30012, 30010, "1240", 2290, -64, 2599, 111, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30012, 0, 1129250816, 0, 1118476698, 0, 2, 1129250816, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30012, 30501, "1241", 543, -83, 768, 329, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30012, 1, 1101529088, 1086324736, 1118476698, 0, 2, 1101529088, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30101, 30003, "1242", 3594, -76, 3599, 423, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30101, 0, 1134788608, 1097859072, 1118476698, 0, 2, 0, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30101, 30003, "1242", 0, -76, 469, 423, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30101, 1, 1134788608, 1097859072, 1118476698, 0, 2, 0, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30301, 30008, "1243", 1979, -71, 2418, 214, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30301, 0, 1127481344, 1097859072, 1118476698, 0, 2, 1127481344, 1097859072, 1118476698);
	_app->rotationAddMovabilityToPuzzle(30301, kPuzzle35011, "", 1907, -127, 1965, 13, 1, 52, 0);
	_app->rotationSetMovabilityToPuzzle(30301, 1, 1125515264, 1077936128, 1118476698, 0, 2);
	_app->rotationAddMovabilityToPuzzle(30303, kPuzzle35001, "", 2023, 62, 2255, 252, 1, 52, 0);
	_app->rotationSetMovabilityToPuzzle(30303, 0, 1126825984, 1097859072, 1118476698, 0, 2);
	_app->rotationAddMovabilityToRotation(30401, 30402, "1244", 442, -134, 782, 153, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30401, 0, 1135673344, 0, 1118476698, 0, 2, 1132625920, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30401, 30011, "1245", 2074, 141, 2808, 345, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30401, 1, 1128792064, 1097859072, 1118476698, 0, 2, 1128136704, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30401, 30801, "1246", 1014, -104, 1239, 41, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30401, 2, 1116471296, 0, 1118476698, 0, 2, 1116864512, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30402, 30401, "1247", 1745, -59, 2100, 104, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30402, 0, 1126825984, 0, 1118476698, 0, 2, 1127874560, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30402, 30801, "1248", 1318, -92, 1504, 17, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30402, 1, 1119354880, 0, 1118476698, 0, 2, 1117782016, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30402, 30501, "1249", 2967, -102, 3237, 190, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30402, 2, 1132625920, 0, 1118476698, 0, 2, 1106247680, 1097859072, 1118476698);
	_app->rotationSetMovabilityOnOrOff(30402, 2, 2);
	_app->rotationAddMovabilityToRotation(30501, 30012, "1250", 1981, -64, 2218, 324, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30501, 0, 1126498304, 1090519040, 1118476698, 0, 2, 1129250816, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30501, 30601, "1251", 390, -247, 766, 179, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30501, 1, 1073741824, -1049624576, 1118476698, 0, 2, 1129250816, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30601, 30602, "1253", 1571, -68, 1916, 132, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30601, 0, 1124990976, -1049624576, 1118476698, 0, 2, 1132462080, -1049624576, 1118476698);
	_app->rotationAddMovabilityToRotation(30601, 30602, "1253", 1274, 73, 1778, 525, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30601, 1, 1124990976, -1049624576, 1118476698, 0, 2, 1132462080, -1049624576, 1118476698);
	_app->rotationAddMovabilityToRotation(30601, 30501, "1254", 2654, 191, 3427, 436, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30601, 2, 1132593152, 1097859072, 1118476698, 0, 2, 1117913088, 1097859072, 1118476698);
	_app->rotationAddMovabilityToPuzzle(30601, kPuzzle35003, "", 3529, -177, 3599, 108, 1, 52, 0);
	_app->rotationSetMovabilityToPuzzle(30601, 3, 1134395392, 1065353216, 1118476698, 0, 2);
	_app->rotationAddMovabilityToPuzzle(30601, kPuzzle35003, "", 0, -177, 96, 108, 1, 52, 0);
	_app->rotationSetMovabilityToPuzzle(30601, 4, 1134395392, 1065353216, 1118476698, 0, 2);
	_app->rotationAddMovabilityToPuzzle(30601, kPuzzle35004, "", 2077, -43, 2466, 251, 1, 52, 0);
	_app->rotationSetMovabilityToPuzzle(30601, 5, 1127481344, 1095761920, 1118476698, 0, 2);
	_app->rotationAddMovabilityToPuzzle(30601, kPuzzle35005, "", 530, -193, 703, 102, 1, 52, 0);
	_app->rotationSetMovabilityToPuzzle(30601, 6, 1099431936, 0, 1118476698, 0, 2);
	_app->rotationAddMovabilityToPuzzle(30601, kPuzzle35006, "", 174, -111, 451, 52, 1, 52, 0);
	_app->rotationSetMovabilityToPuzzle(30601, 7, 1135443968, 0, 1118476698, 0, 2);
	_app->rotationAddMovabilityToPuzzle(30601, kPuzzle35007, "", 1775, 127, 2065, 375, 1, 52, 0);
	_app->rotationSetMovabilityToPuzzle(30601, 8, 1125187584, 1104674816, 1118476698, 0, 2);
	_app->rotationAddMovabilityToPuzzle(30601, kPuzzle35008, "", 251, 123, 305, 181, 1, 52, 0);
	_app->rotationSetMovabilityToPuzzle(30601, 9, 1135312896, 1096810496, 1118476698, 0, 2);
	_app->rotationAddMovabilityToRotation(30602, 30601, "1255", 2536, 303, 3188, 511, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30602, 0, 1131741184, 1097859072, 1118476698, 0, 2, 0, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30602, 30701, "1256", 1423, -125, 1617, 139, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30602, 1, 1121714176, 0, 1118476698, 0, 2, 1118961664, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30701, 30702, "1257", 1269, -69, 1436, 261, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30701, 0, 1118699520, 0, 1118476698, 0, 2, 1119092736, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30701, 30602, "1258", 2990, -174, 3348, 225, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30701, 1, 1132855296, 0, 1118476698, 0, 2, 1132331008, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30702, 30703, "1259", 1451, 144, 1773, 439, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30702, 0, 1122500608, 1097859072, 1118476698, 0, 2, 0, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30702, 30701, "1260", 3091, -71, 3202, 209, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30702, 1, 1132953600, 0, 1118476698, 0, 2, 1132920832, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30702, 30704, "1261", 1265, -92, 1455, 219, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30702, 2, 1119092736, 0, 1118476698, 0, 2, 1119092736, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30703, 30702, "1262", 219, -530, 639, -404, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30703, 0, 1133379584, -1049624576, 1118476698, 0, 2, 1133379584, 0, 1118476698);
	_app->rotationAddMovabilityToPuzzle(30703, kPuzzle35010, "", 105, 266, 857, 456, 1, 52, 0);
	_app->rotationSetMovabilityToPuzzle(30703, 1, 0, 1097859072, 1118476698, 0, 2);
	_app->rotationAddMovabilityToRotation(30704, 30702, "1263", 3070, -68, 3253, 225, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30704, 0, 1133019136, 0, 1118476698, 0, 2, 1132920832, 0, 1118476698);
	_app->rotationAddMovabilityToPuzzle(30704, kPuzzle35020, "", 1214, -8, 1462, 176, 1, 52, 0);
	_app->rotationSetMovabilityToPuzzle(30704, 1, 1118961664, 1088421888, 1118476698, 0, 2);
	_app->rotationAddMovabilityToRotation(30801, 30401, "1264", 2847, -75, 2991, 17, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30801, 0, 1132134400, 0, 1118476698, 0, 2, 1127874560, 0, 1118476698);
	_app->rotationAddMovabilityToRotation(30801, 30402, "1265", 3183, -85, 3280, 5, 1, 53, 0);
	_app->rotationSetMovabilityToRotation(30801, 1, 1133248512, 0, 1118476698, 0, 2, 1132625920, 0, 1118476698);
	_app->rotationAddMovabilityToPuzzle(30801, kPuzzle35002, "", 984, -69, 1840, 256, 1, 52, 0);
	_app->rotationSetMovabilityToPuzzle(30801, 2, 1119485952, 1097859072, 1118476698, 0, 2);
	_app->puzzleAddMovabilityToRotation(kPuzzle35001, 30302, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35001, 0, 1126825984, 1097859072, 1118476698);
	_app->puzzleAddMovabilityToRotation(kPuzzle35002, 30801, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35002, 0, 1119485952, 1097859072, 1118476698);
	_app->puzzleAddMovabilityToRotation(kPuzzle35003, 30601, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35003, 0, 1134395392, 1065353216, 1118476698);
	_app->puzzleAddMovabilityToRotation(kPuzzle35004, 30601, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35004, 0, 1127481344, 1095761920, 1118476698);
	_app->puzzleAddMovabilityToRotation(kPuzzle35005, 30601, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35005, 0, 1099431936, 0, 1118476698);
	_app->puzzleAddMovabilityToRotation(kPuzzle35006, 30601, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35006, 0, 1135443968, 0, 1118476698);
	_app->puzzleAddMovabilityToRotation(kPuzzle35007, 30601, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35007, 0, 1125187584, 1097859072, 1118476698);
	_app->puzzleAddMovabilityToRotation(kPuzzle35008, 30601, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35008, 0, 1135312896, 0, 1118476698);
	_app->puzzleAddMovabilityToRotation(kPuzzle35009, 30602, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35009, 0, 1127153664, -1044905984, 1118476698);
	_app->puzzleAddMovabilityToRotation(kPuzzle35010, 30703, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35010, 0, 0, 1097859072, 1118476698);
	_app->puzzleAddMovabilityToRotation(kPuzzle35011, 30301, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35011, 0, 1125515264, 1077936128, 1118476698);
	_app->puzzleAddMovabilityToRotation(kPuzzle35019, 30004, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35019, 0, 1116471296, 0, 1118476698);
	_app->puzzleAddMovabilityToRotation(kPuzzle35020, 30704, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle35020, 0, 1118961664, 1088421888, 1118476698);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle35109, 35111, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle35100, 30003, "", Common::Rect(0, 420, 640, 464), 1, 55, 0);
	_app->objectAdd(kObjectWolfInstinct, "Wolf Vision", "FO_WolfVision", 1);
	_app->objectAddBagAnimation(kObjectWolfInstinct, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectWolfInstinct, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectWolfInstinct, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectIngot, "Panel", "FO_Panel01_", 1);
	_app->objectAddBagAnimation(kObjectIngot, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectIngot, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectIngot, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectIngot2, "Panel", "FO_Panel01_", 1);
	_app->objectAddBagAnimation(kObjectIngot2, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectIngot2, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectIngot2, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectIngot3, "Panel", "FO_Panel01_", 1);
	_app->objectAddBagAnimation(kObjectIngot3, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectIngot3, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectIngot3, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectLogeTear2, "Loge's Tear", "NI_Tear", 1);
	_app->objectAddBagAnimation(kObjectLogeTear2, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectLogeTear2, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectLogeTear2, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObject30001, "", "", 1);
	_app->objectAdd(kObjectWolfBadge, "Medallion", "FO_WolfLogo", 1);
	_app->objectAddBagAnimation(kObjectWolfBadge, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectWolfBadge, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectWolfBadge, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAddPresentation(kObject30001);
	_app->objectPresentationAddImageToPuzzle(kObject30001, 0, kPuzzle35011, "FOS03N01P01L01.bmp", 170, 133, 1, 1, 1000);
	_app->objectAddPresentation(kObject30001);
	_app->objectPresentationAddImageToPuzzle(kObject30001, 1, kPuzzle35011, "FOS03N01P01L02.bmp", 266, 178, 1, 1, 1000);
	_app->objectAddPresentation(kObject30001);
	_app->objectPresentationAddImageToPuzzle(kObject30001, 2, kPuzzle35011, "FOS03N01P01L03.bmp", 268, 211, 1, 1, 1000);
	_app->objectAddPresentation(kObject30001);
	_app->objectPresentationAddImageToPuzzle(kObject30001, 3, kPuzzle35011, "FOS03N01P01L04.bmp", 266, 249, 1, 1, 1000);
	_app->objectAddPresentation(kObject30001);
	_app->objectPresentationAddImageToPuzzle(kObject30001, 4, kPuzzle35011, "FOS03N01P01L05.bmp", 293, 332, 1, 1, 1000);
	_app->objectAddPuzzleAccessibility(kObject30001, kPuzzle35011, Common::Rect(260, 171, 376, 374), 1, 52, 0);
	_app->objectAddPuzzleAccessibility(kObject30001, kPuzzle35011, Common::Rect(273, 182, 361, 202), 1, 52, 1);
	_app->objectAddPuzzleAccessibility(kObject30001, kPuzzle35011, Common::Rect(276, 218, 359, 238), 1, 52, 2);
	_app->objectAddPuzzleAccessibility(kObject30001, kPuzzle35011, Common::Rect(275, 257, 361, 312), 1, 52, 3);
	_app->objectAddPuzzleAccessibility(kObject30001, kPuzzle35011, Common::Rect(292, 333, 345, 388), 1, 52, 4);
	_app->objectSetAccessibilityOnOrOff(kObject30001, 1, 4);
	_app->objectAdd(kObjectPatience, "Saturn", "FO_Scroll01_", 1);
	_app->objectAddBagAnimation(kObjectPatience, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectPatience, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectPatience, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectMovementAndIntuition, "Mercury", "FO_Scroll01_", 1);
	_app->objectAddBagAnimation(kObjectMovementAndIntuition, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectMovementAndIntuition, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectMovementAndIntuition, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectLove, "Venus", "FO_Scroll01_", 1);
	_app->objectAddBagAnimation(kObjectLove, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectLove, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectLove, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectImagination, "Moon", "FO_Scroll01_", 1);
	_app->objectAddBagAnimation(kObjectImagination, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectImagination, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectImagination, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectDestruction, "Mars", "FO_Scroll01_", 1);
	_app->objectAddBagAnimation(kObjectDestruction, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectDestruction, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectDestruction, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectJudgementAndDirection, "Jupiter", "FO_Scroll01_", 1);
	_app->objectAddBagAnimation(kObjectJudgementAndDirection, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectJudgementAndDirection, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectJudgementAndDirection, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectWill, "Sun", "FO_Scroll01_", 1);
	_app->objectAddBagAnimation(kObjectWill, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectWill, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectWill, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObject30002, "", "", 1);
	_app->objectAddRotationAccessibility(kObject30002, 30301, Common::Rect(1858, 280, 2021, 366), 1, 52, 0);
	_app->objectAdd(kObject30003, "", "", 1);
	_app->objectAddRotationAccessibility(kObject30003, 30301, Common::Rect(2371, 280, 2540, 366), 1, 52, 0);
	_app->objectAdd(kObject30004, "", "", 1);
	_app->objectAddRotationAccessibility(kObject30004, 30301, Common::Rect(2884, 280, 3048, 366), 1, 52, 0);
	_app->objectAdd(kObject30005, "", "", 1);
	_app->objectAddRotationAccessibility(kObject30005, 30301, Common::Rect(3401, 280, 3561, 366), 1, 52, 0);
	_app->objectAdd(kObject30006, "", "", 1);
	_app->objectAddRotationAccessibility(kObject30006, 30301, Common::Rect(311, 280, 481, 366), 1, 52, 0);
	_app->objectAdd(kObject30007, "", "", 1);
	_app->objectAddRotationAccessibility(kObject30007, 30301, Common::Rect(827, 280, 998, 366), 1, 52, 0);
	_app->objectAdd(kObject30008, "", "", 1);
	_app->objectAddRotationAccessibility(kObject30008, 30301, Common::Rect(1337, 280, 1509, 366), 1, 52, 0);
	_app->objectAdd(kObject30027, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject30027, kPuzzle35001, Common::Rect(410, 230, 466, 273), 1, 53, 0);
	_app->objectAdd(kObject30016, "", "", 4);
	_app->objectSetPassiveDrawCursor(kObject30016, 20, 20, 0, 3, 0, 0, 3);
	_app->objectSetActiveDrawCursor(kObject30016, 20, 20, 0, 3, 0, 0, 3);

	for (uint32 i = 0; i < 51; i++) {
		_app->objectAddPresentation(kObject30016);
		_app->objectPresentationAddImageToPuzzle(kObject30016, i, kPuzzle35001, Common::String::format("FOS03N02P01S01.%04d.bmp", i + 1), 341, 150, 1, 1, 1000);
	}

	_app->objectPresentationShow(kObject30016, 25);
	_app->objectAddPuzzleAccessibility(kObject30016, kPuzzle35001, Common::Rect(349, 164, 532, 332), 1, 10000, 0);
	_app->objectAdd(kObjectBerries, "Berries", "FO_Berries", 1);
	_app->objectAddBagAnimation(kObjectBerries, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectBerries, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectBerries, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectSleepingBerries, "Poison berries", "FO_BerriesPoison", 1);
	_app->objectAddBagAnimation(kObjectSleepingBerries, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectSleepingBerries, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectSleepingBerries, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAddPuzzleAccessibility(kObjectBerries, kPuzzle35002, Common::Rect(232, 322, 346, 436), 1, 52, 0);
	_app->objectAddPuzzleAccessibility(kObjectBerries, kPuzzle35002, Common::Rect(110, 120, 492, 305), 1, 52, 1);
	_app->objectAddPuzzleAccessibility(kObjectBerries, kPuzzle35002, Common::Rect(116, 296, 205, 426), 1, 52, 2);
	_app->objectAddPuzzleAccessibility(kObjectBerries, kPuzzle35002, Common::Rect(348, 251, 636, 414), 1, 52, 3);
	_app->objectAddPuzzleAccessibility(kObjectBerries, kPuzzle35002, Common::Rect(505, 194, 556, 238), 1, 52, 4);
	_app->objectAddPresentation(kObjectBerries);
	_app->objectPresentationAddImageToPuzzle(kObjectBerries, 0, kPuzzle35002, "FOS08N01P01L01.BMP", 508, 197, 1, 1, 1000);
	_app->objectAddPresentation(kObjectBerries);
	_app->objectPresentationAddImageToPuzzle(kObjectBerries, 1, kPuzzle35002, "FOS08N01P01L02.BMP", 0, 16, 1, 1, 1000);
	_app->objectAddPresentation(kObjectBerries);
	_app->objectPresentationAddImageToPuzzle(kObjectBerries, 2, kPuzzle35002, "FOS08N01P01L03.BMP", 510, 198, 1, 1, 1000);
	_app->objectAddPresentation(kObjectBerries);
	_app->objectPresentationAddImageToPuzzle(kObjectBerries, 3, kPuzzle35002, "FOS08N01P01L04.BMP", 209, 319, 1, 1, 1000);
	_app->objectAdd(kObjectBerriesJuice, "Berries juice", "FO_BerriesJuice", 1);
	_app->objectAddBagAnimation(kObjectBerriesJuice, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectBerriesJuice, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectBerriesJuice, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectSleepingPotion2, "Poison berries juice", "FO_BerriesPoisonJuice", 1);
	_app->objectAddBagAnimation(kObjectSleepingPotion2, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectSleepingPotion2, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectSleepingPotion2, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAddPuzzleAccessibility(kObjectBerries, kPuzzle35005, Common::Rect(155, 283, 407, 447), 1, 52, 5);
	_app->objectAddPresentation(kObjectBerries);
	_app->objectPresentationAddImageToPuzzle(kObjectBerries, 4, kPuzzle35005, "FOS06N01P05L01.BMP", 209, 281, 1, 1, 1000);
	_app->objectAddPresentation(kObjectBerries);
	_app->objectPresentationAddImageToPuzzle(kObjectBerries, 5, kPuzzle35005, "FOS06N01P05L02.BMP", 243, 402, 1, 1, 1000);
	_app->objectAdd(kObjectSleepingPotion, "", "", 1);
	_app->objectAddPresentation(kObjectSleepingPotion);
	_app->objectPresentationAddAnimationToRotation(kObjectSleepingPotion, 0, 30003, 0, 26, 1101004800, 4);
	_app->objectPresentationSetAnimationOnRotation(kObjectSleepingPotion, 0, 0, 30006);
	_app->objectPresentationShow(kObjectSleepingPotion, 0);
	_app->objectAddRotationAccessibility(kObjectSleepingPotion, 30003, Common::Rect(1931, 76, 2112, 263), 1, 52, 0);
	_app->objectAddRotationAccessibility(kObjectSleepingPotion, 30003, Common::Rect(1476, -50, 1741, 245), 1, 53, 1);
	_app->objectAdd(kObjectHare, "Rabbit hare", "FO_Hares", 1);
	_app->objectAddBagAnimation(kObjectHare, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectHare, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectHare, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectBow, "Hunting bow", "FO_HuntingBow", 1);
	_app->objectAddBagAnimation(kObjectBow, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectBow, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectBow, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAddPuzzleAccessibility(kObjectBow, kPuzzle35003, Common::Rect(240, 200, 400, 410), 1, 52, 0);
	_app->objectAddPuzzleAccessibility(kObjectBow, kPuzzle35003, Common::Rect(230, 219, 278, 275), 1, 52, 1);
	_app->objectAddPuzzleAccessibility(kObjectBow, kPuzzle35003, Common::Rect(237, 280, 273, 334), 1, 52, 2);
	_app->objectAddPuzzleAccessibility(kObjectBow, kPuzzle35003, Common::Rect(353, 228, 387, 283), 1, 52, 3);
	_app->objectAddPuzzleAccessibility(kObjectBow, kPuzzle35003, Common::Rect(384, 226, 415, 281), 1, 52, 4);
	_app->objectAddPuzzleAccessibility(kObjectBow, kPuzzle35003, Common::Rect(328, 286, 366, 452), 1, 52, 5);
	_app->objectAddPuzzleAccessibility(kObjectBow, kPuzzle35003, Common::Rect(277, 174, 326, 447), 1, 52, 6);
	_app->objectAddPresentation(kObjectBow);
	_app->objectPresentationAddImageToPuzzle(kObjectBow, 0, kPuzzle35003, "FOS06N01P06L01.bmp", 72, 16, 1, 1, 1000);
	_app->objectAddPresentation(kObjectBow);
	_app->objectPresentationAddImageToPuzzle(kObjectBow, 1, kPuzzle35003, "FOS06N01P06L02.bmp", 233, 219, 1, 1, 1000);
	_app->objectAddPresentation(kObjectBow);
	_app->objectPresentationAddImageToPuzzle(kObjectBow, 2, kPuzzle35003, "FOS06N01P06L03.bmp", 241, 273, 1, 1, 1000);
	_app->objectAddPresentation(kObjectBow);
	_app->objectPresentationAddImageToPuzzle(kObjectBow, 3, kPuzzle35003, "FOS06N01P06L04.bmp", 357, 221, 1, 1, 1000);
	_app->objectAddPresentation(kObjectBow);
	_app->objectPresentationAddImageToPuzzle(kObjectBow, 4, kPuzzle35003, "FOS06N01P06L05.bmp", 387, 222, 1, 1, 1000);
	_app->objectAddPresentation(kObjectBow);
	_app->objectPresentationAddImageToPuzzle(kObjectBow, 5, kPuzzle35003, "FOS06N01P06L06.bmp", 326, 282, 1, 1, 1000);
	_app->objectAddPresentation(kObjectBow);
	_app->objectPresentationAddImageToPuzzle(kObjectBow, 6, kPuzzle35003, "FOS06N01P06L07.bmp", 274, 170, 1, 1, 1000);
	_app->objectAdd(kObject30028, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject30028, kPuzzle35004, Common::Rect(450, 265, 512, 340), 1, 52, 0);
	_app->objectAddPuzzleAccessibility(kObject30028, kPuzzle35004, Common::Rect(70, 350, 240, 415), 1, 52, 1);
	_app->objectAddPresentation(kObject30028);
	_app->objectPresentationAddImageToPuzzle(kObject30028, 0, kPuzzle35004, "FOS06N01P01L01.bmp", 0, 339, 1, 1, 1000);
	_app->objectAddPresentation(kObject30028);
	_app->objectPresentationAddAnimationToPuzzle(kObject30028, 1, kPuzzle35004, "FOS06N01P01LS01", 0, 1, 250, 1, 1000, 13, 1095237632, 4);
	_app->objectAddPresentation(kObject30028);
	_app->objectPresentationAddAnimationToPuzzle(kObject30028, 2, kPuzzle35004, "FOS06N01P01LS02", 0, 1, 242, 1, 1000, 13, 1095237632, 4);
	_app->objectAddPresentation(kObject30028);
	_app->objectPresentationAddImageToRotation(kObject30028, 3, 30601, 0);
	_app->objectAdd(kObject30040, "", "", 1);
	_app->objectAdd(kObjectMetals, "Ignots", "FO_Ignots", 1);
	_app->objectAddBagAnimation(kObjectMetals, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectMetals, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectMetals, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectMould, "Pan", "FO_Pan", 1);
	_app->objectAddBagAnimation(kObjectMould, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectMould, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectMould, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectMeltedGold, "Gold", "FO_IgnotGld", 1);
	_app->objectAddBagAnimation(kObjectMeltedGold, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectMeltedGold, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectMeltedGold, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectMeltedSilver, "Silver", "FO_IgnotSil", 1);
	_app->objectAddBagAnimation(kObjectMeltedSilver, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectMeltedSilver, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectMeltedSilver, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectMeltedCopper, "Copper", "FO_IgnotCop", 1);
	_app->objectAddBagAnimation(kObjectMeltedCopper, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectMeltedCopper, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectMeltedCopper, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectMeltedLead, "Lead", "FO_IgnotLed", 1);
	_app->objectAddBagAnimation(kObjectMeltedLead, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectMeltedLead, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectMeltedLead, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectMeltedSteel, "Steel", "FO_IgnotSte", 1);
	_app->objectAddBagAnimation(kObjectMeltedSteel, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectMeltedSteel, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectMeltedSteel, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectMeltedTin, "Tin", "FO_IgnotTin", 1);
	_app->objectAddBagAnimation(kObjectMeltedTin, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectMeltedTin, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectMeltedTin, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectQuicksilver, "Mercury", "FO_IgnotMer", 1);
	_app->objectAddBagAnimation(kObjectQuicksilver, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectQuicksilver, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectQuicksilver, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAddPuzzleAccessibility(kObject30040, kPuzzle35006, Common::Rect(293, 132, 351, 173), 1, 52, 0);
	_app->objectAddPuzzleAccessibility(kObject30040, kPuzzle35006, Common::Rect(285, 207, 354, 245), 1, 52, 1);
	_app->objectAddPuzzleAccessibility(kObject30040, kPuzzle35006, Common::Rect(173, 229, 244, 271), 1, 52, 2);
	_app->objectAddPuzzleAccessibility(kObject30040, kPuzzle35006, Common::Rect(201, 289, 284, 329), 1, 52, 3);
	_app->objectAddPuzzleAccessibility(kObject30040, kPuzzle35006, Common::Rect(214, 135, 364, 189), 1, 52, 4);
	_app->objectAddPuzzleAccessibility(kObject30040, kPuzzle35006, Common::Rect(222, 210, 353, 277), 1, 52, 5);
	_app->objectSetAccessibilityOnOrOff(kObject30040, 2, 2);
	_app->objectSetAccessibilityOnOrOff(kObject30040, 3, 3);
	_app->objectSetAccessibilityOnOrOff(kObject30040, 4, 4);
	_app->objectSetAccessibilityOnOrOff(kObject30040, 5, 5);
	_app->objectAddPresentation(kObject30040);
	_app->objectPresentationAddImageToPuzzle(kObject30040, 0, kPuzzle35006, "FOS06N01P02L02.bmp", 1, 44, 1, 1, 1000);
	_app->objectAddPresentation(kObject30040);
	_app->objectPresentationAddImageToPuzzle(kObject30040, 1, kPuzzle35006, "FOS06N01P02L01.bmp", 2, 45, 1, 1, 1000);
	_app->objectAddPresentation(kObject30040);
	_app->objectPresentationAddImageToPuzzle(kObject30040, 2, kPuzzle35006, "FOS06N01P02L03.bmp", 58, 151, 1, 1, 1000);
	_app->objectAddPresentation(kObject30040);
	_app->objectPresentationAddImageToPuzzle(kObject30040, 3, kPuzzle35006, "FOS06N01P02L04.bmp", 54, 132, 1, 1, 1000);
	_app->objectAddPresentation(kObject30040);
	_app->objectPresentationAddAnimationToPuzzle(kObject30040, 4, kPuzzle35006, "FOS06N01P02LS01", 0, 157, 168, 1, 1000, 25, 1095237632, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject30040, 4, 0, 30008);
	_app->objectAdd(kObject30042, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(248, 261, 287, 278), 1, 52, 0);
	_app->objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(405, 79, 453, 115), 1, 52, 1);
	_app->objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(337, 47, 379, 77), 1, 52, 2);
	_app->objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(453, 199, 492, 228), 1, 52, 3);
	_app->objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(245, 48, 285, 74), 1, 52, 4);
	_app->objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(285, 46, 330, 73), 1, 52, 5);
	_app->objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(419, 120, 457, 151), 1, 52, 6);
	_app->objectAddPuzzleAccessibility(kObject30042, kPuzzle35007, Common::Rect(441, 266, 476, 294), 1, 52, 7);
	_app->objectSetAccessibilityOnOrOff(kObject30042, 1, 7);
	_app->objectAddPresentation(kObject30042);
	_app->objectPresentationAddImageToPuzzle(kObject30042, 0, kPuzzle35007, "FOS06N01P03L02.bmp", 0, 16, 1, 1, 1000);
	_app->objectAddPresentation(kObject30042);
	_app->objectPresentationAddImageToPuzzle(kObject30042, 1, kPuzzle35007, "FOS06N01P03L03_03.bmp", 334, 200, 1, 1, 1000);
	_app->objectAddPresentation(kObject30042);
	_app->objectPresentationAddImageToPuzzle(kObject30042, 2, kPuzzle35007, "FOS06N01P03L03_02.bmp", 328, 182, 1, 1, 1000);
	_app->objectAddPresentation(kObject30042);
	_app->objectPresentationAddImageToPuzzle(kObject30042, 3, kPuzzle35007, "FOS06N01P03L03_05.bmp", 335, 229, 1, 1, 1000);
	_app->objectAddPresentation(kObject30042);
	_app->objectPresentationAddImageToPuzzle(kObject30042, 4, kPuzzle35007, "FOS06N01P03L03_06.bmp", 326, 252, 1, 1, 1000);
	_app->objectAddPresentation(kObject30042);
	_app->objectPresentationAddImageToPuzzle(kObject30042, 5, kPuzzle35007, "FOS06N01P03L03_01.bmp", 294, 221, 1, 1, 1000);
	_app->objectAddPresentation(kObject30042);
	_app->objectPresentationAddImageToPuzzle(kObject30042, 6, kPuzzle35007, "FOS06N01P03L03_04.bmp", 368, 229, 1, 1, 1000);
	_app->objectAddPresentation(kObject30042);
	_app->objectPresentationAddImageToPuzzle(kObject30042, 7, kPuzzle35007, "FOS06N01P03L03_07.bmp", 312, 307, 1, 1, 1000);
	_app->objectAddPresentation(kObject30042);
	_app->objectPresentationAddImageToPuzzle(kObject30042, 8, kPuzzle35007, "FOS06N01P03L01.bmp", 0, 16, 1, 1, 1000);
	_app->objectAdd(kObjectGolem, "Golem", "FO_Golem", 1);
	_app->objectAddBagAnimation(kObjectGolem, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectGolem, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectGolem, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObject30044, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject30044, kPuzzle35008, Common::Rect(253, 206, 333, 300), 1, 52, 0);
	_app->objectAddPuzzleAccessibility(kObject30044, kPuzzle35008, Common::Rect(203, 262, 300, 379), 1, 52, 1);
	_app->objectSetAccessibilityOnOrOff(kObject30044, 1, 1);
	_app->objectAddPresentation(kObject30044);
	_app->objectPresentationAddImageToPuzzle(kObject30044, 0, kPuzzle35008, "FOS06N01P04L01.bmp", 189, 229, 1, 1, 1000);
	_app->objectAdd(kObject30045, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject30045, kPuzzle35009, Common::Rect(283, 238, 347, 299), 1, 52, 0);
	_app->objectAddPuzzleAccessibility(kObject30045, kPuzzle35009, Common::Rect(273, 315, 306, 352), 1, 52, 1);
	_app->objectAddPuzzleAccessibility(kObject30045, kPuzzle35009, Common::Rect(293, 198, 330, 236), 1, 52, 2);
	_app->objectAddPuzzleAccessibility(kObject30045, kPuzzle35009, Common::Rect(342, 217, 387, 265), 1, 52, 3);
	_app->objectAddPuzzleAccessibility(kObject30045, kPuzzle35009, Common::Rect(225, 241, 261, 281), 1, 52, 4);
	_app->objectAddPuzzleAccessibility(kObject30045, kPuzzle35009, Common::Rect(250, 268, 288, 306), 1, 52, 5);
	_app->objectAddPuzzleAccessibility(kObject30045, kPuzzle35009, Common::Rect(351, 267, 403, 325), 1, 52, 6);
	_app->objectAddRotationAccessibility(kObject30045, 30602, Common::Rect(1856, -381, 2445, -20), 1, 52, 7);
	_app->objectAddPresentation(kObject30045);
	_app->objectPresentationAddImageToPuzzle(kObject30045, 0, kPuzzle35009, "FOS06N02P01L01.bmp", 195, 34, 1, 1, 1000);
	_app->objectAddPresentation(kObject30045);
	_app->objectPresentationAddImageToPuzzle(kObject30045, 1, kPuzzle35009, "FOS06N02P01L02.bmp", 193, 32, 1, 1, 1000);
	_app->objectAddPresentation(kObject30045);
	_app->objectPresentationAddImageToPuzzle(kObject30045, 2, kPuzzle35009, "FOS06N02P01L03.bmp", 148, 16, 1, 1, 1000);
	_app->objectAddPresentation(kObject30045);
	_app->objectPresentationAddImageToPuzzle(kObject30045, 3, kPuzzle35009, "FOS06N02P01L06.bmp", 174, 16, 1, 1, 1000);
	_app->objectAddPresentation(kObject30045);
	_app->objectPresentationAddImageToPuzzle(kObject30045, 4, kPuzzle35009, "FOS06N02P01L05.bmp", 203, 16, 1, 1, 1000);
	_app->objectAddPresentation(kObject30045);
	_app->objectPresentationAddImageToPuzzle(kObject30045, 5, kPuzzle35009, "FOS06N02P01L04.bmp", 171, 22, 1, 1, 1000);
	_app->objectAddPresentation(kObject30045);
	_app->objectPresentationAddImageToPuzzle(kObject30045, 6, kPuzzle35009, "FOS06N02P01L07.bmp", 161, 37, 1, 1, 1000);
	_app->objectAddPresentation(kObject30045);
	_app->objectPresentationAddAnimationToRotation(kObject30045, 7, 30602, 0, 75, 1106247680, 4);
	_app->objectPresentationSetAnimationOnRotation(kObject30045, 7, 0, 30000);
	_app->objectPresentationShow(kObject30045, 7);
	_app->objectAdd(kObjectFishingRod, "Fishing pole", "FO_Fishing", 1);
	_app->objectAddBagAnimation(kObjectFishingRod, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectFishingRod, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectFishingRod, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectFish, "Fish", "FO_Fish", 1);
	_app->objectAddBagAnimation(kObjectFish, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectFish, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectFish, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectFishingRodWithWorms, "Fishing pole and worms", "FO_FishingWorms", 1);
	_app->objectAddBagAnimation(kObjectFishingRodWithWorms, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectFishingRodWithWorms, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectFishingRodWithWorms, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObjectKey, "Key form the fish", "FO_FishKey", 1);
	_app->objectAddBagAnimation(kObjectKey, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectKey, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectKey, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAddPuzzleAccessibility(kObjectFishingRod, kPuzzle35010, Common::Rect(0, 208, 639, 384), 1, 52, 0);
	_app->objectAddPuzzleAccessibility(kObjectFishingRod, kPuzzle35010, Common::Rect(0, 208, 639, 384), 1, 52, 1);
	_app->objectAddPuzzleAccessibility(kObjectFishingRod, kPuzzle35010, Common::Rect(0, 63, 639, 197), 1, 52, 2);
	_app->objectSetAccessibilityOnOrOff(kObjectFishingRod, 1, 2);
	_app->objectAddRotationAccessibility(kObjectFishingRod, 30703, Common::Rect(1126, 141, 3373, 523), 1, 52, 3);
	_app->objectAddPresentation(kObjectFishingRod);
	_app->objectPresentationAddImageToPuzzle(kObjectFishingRod, 0, kPuzzle35010, "FOS07N03P01L01.bmp", 0, 16, 1, 1, 1000);
	_app->objectAddPresentation(kObjectFishingRod);
	_app->objectPresentationAddImageToPuzzle(kObjectFishingRod, 1, kPuzzle35010, "FOS07N03P01L02.bmp", 0, 16, 1, 1, 1000);
	_app->objectAdd(kObjectWorms, "Worms", "FO_Worms", 1);
	_app->objectAddBagAnimation(kObjectWorms, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectWorms, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectWorms, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAddPresentation(kObjectWorms);
	_app->objectPresentationAddImageToPuzzle(kObjectWorms, 0, kPuzzle35020, "FOS07N04P01L01.bmp", 0, 16, 1, 1, 1000);
	_app->objectAddPresentation(kObjectWorms);
	_app->objectPresentationAddAnimationToPuzzle(kObjectWorms, 1, kPuzzle35020, "FOS07N04P01S01", 0, 312, 209, 1, 1000, 12, 1095237632, 4);
	_app->objectAddPuzzleAccessibility(kObjectWorms, kPuzzle35020, Common::Rect(285, 196, 391, 330), 0, 52, 0);
	_app->objectAdd(kObject30050, "", "", 1);
	_app->objectAddRotationAccessibility(kObject30050, 30501, Common::Rect(1177, 298, 1263, 530), 1, 52, 0);
	_app->objectAddPresentation(kObject30050);
	_app->objectPresentationAddImageToRotation(kObject30050, 0, 30501, 0);
	_app->objectAddPresentation(kObject30050);
	_app->objectPresentationAddAnimationToRotation(kObject30050, 1, 30501, 1, 13, 1095237632, 6);
	_app->objectPresentationSetAnimationOnRotation(kObject30050, 1, 0, 30009);
	_app->objectAddPresentation(kObject30050);
	_app->objectPresentationAddImageToRotation(kObject30050, 2, 30402, 0);
	_app->objectAddPresentation(kObject30050);
	_app->objectPresentationAddImageToRotation(kObject30050, 3, 30501, 2);
	_app->objectAdd(kObject30051, "", "", 1);
	_app->objectAdd(kObject30052, "", "", 1);
	_app->objectAdd(kObjectBurningArrow, "Inflamed arrow and bow", "FO_BowFire", 1);
	_app->objectAddBagAnimation(kObjectBurningArrow, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectBurningArrow, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectBurningArrow, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAddRotationAccessibility(kObject30051, 30011, Common::Rect(411, 0, 511, 57), 0, 52, 0);
	_app->objectAddRotationAccessibility(kObject30051, 30011, Common::Rect(958, 102, 1105, 333), 1, 52, 1);
	_app->objectAddRotationAccessibility(kObject30051, 30011, Common::Rect(0, 0, 0, 0), 1, 52, 2);
	_app->objectAddRotationAccessibility(kObject30051, 30011, Common::Rect(421, 15, 764, 170), 0, 52, 3);
	_app->objectAddPresentation(kObject30051);
	_app->objectPresentationAddImageToRotation(kObject30051, 0, 30011, 0);
	_app->objectAddPresentation(kObject30051);
	_app->objectPresentationAddAnimationToRotation(kObject30051, 1, 30011, 1, 22, 1101004800, 4);
	_app->objectAddPresentation(kObject30051);
	_app->objectPresentationAddImageToRotation(kObject30051, 2, 30011, 2);
	_app->objectPresentationShow(kObject30051, 1);
	_app->objectAdd(kObject30100, "", "", 1);
	_app->objectAddPresentation(kObject30100);
	_app->objectPresentationAddImageToPuzzle(kObject30100, 0, kPuzzle35100, "FOS00N03P02L01.bmp", 117, 70, 1, 1, 1000);
	_app->objectAddPuzzleAccessibility(kObject30100, kPuzzle35100, Common::Rect(114, 106, 406, 463), 1, 52, 0);
	_app->objectAddPuzzleAccessibility(kObject30100, kPuzzle35100, Common::Rect(311, 249, 397, 463), 0, 52, 1);
	_app->objectAdd(kObjectWolfBrooch, "Sieglinde's Medallion", "FO_WolfLogo", 1);
	_app->objectAddBagAnimation(kObjectWolfBrooch, 1, 3, 20, 1095237632, 4);
	_app->objectSetActiveCursor(kObjectWolfBrooch, 22, 22, 20, 4, 1095237632, 4, 4);
	_app->objectSetPassiveCursor(kObjectWolfBrooch, 22, 22, 0, 3, 0, 0, 3);
	_app->objectAdd(kObject30101, "", "", 1);
	_app->objectAddPresentation(kObject30101);
	_app->objectPresentationAddAnimationToPuzzle(kObject30101, 0, kPuzzle35101, "FOS00N03P02S02", 0, 297, 209, 1, 1000, 20, 1095237632, 4);
	_app->objectAdd(kObject30102, "", "", 1);
	_app->objectAdd(kObject30103, "", "", 1);
	_app->objectAddPresentation(kObject30103);
	_app->objectPresentationAddAnimationToPuzzle(kObject30103, 0, kPuzzle35103, "FOS01N01P07", 0, 380, 143, 1, 1000, 20, 1095237632, 4);
	_app->objectAdd(kObject30104, "", "", 1);
	_app->objectAddPresentation(kObject30104);
	_app->objectPresentationAddAnimationToPuzzle(kObject30104, 0, kPuzzle35104, "FOS01N01P03", 0, 165, 129, 1, 1000, 20, 1095237632, 4);
	_app->objectAdd(kObject30105, "", "", 1);
	_app->objectAddPresentation(kObject30105);
	_app->objectPresentationAddAnimationToPuzzle(kObject30105, 0, kPuzzle35105, "FOS01N01P04", 0, 383, 171, 1, 1000, 20, 1095237632, 4);
	_app->objectAdd(kObject30106, "", "", 1);
	_app->objectAddPresentation(kObject30106);
	_app->objectPresentationAddAnimationToPuzzle(kObject30106, 0, kPuzzle35106, "FOS01N01P05", 0, 443, 94, 1, 1000, 20, 1095237632, 4);
	_app->objectAdd(kObject30107, "", "", 1);
	_app->objectAddPresentation(kObject30107);
	_app->objectPresentationAddAnimationToPuzzle(kObject30107, 0, kPuzzle35107, "FOS01N01P06", 0, 303, 161, 1, 1000, 20, 1095237632, 4);
	_app->objectAdd(kObject30109, "", "", 1);
	_app->objectAddPresentation(kObject30109);
	_app->objectPresentationAddAnimationToPuzzle(kObject30109, 0, kPuzzle35111, "FOS01N01P02S01", 0, 190, 141, 1, 1000, 202, 1095237632, 4);
	_app->objectPresentationSetAnimationOnPuzzle(kObject30109, 0, 0, 30007);
	_app->objectAddPuzzleAccessibility(kObject30109, kPuzzle35111, Common::Rect(0, 334, 463, 418), 0, 52, 0);
	_app->objectAdd(kObject30108, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject30108, kPuzzle35109, Common::Rect(181, 165, 256, 259), 1, 52, 0);
	_app->objectAddRotationAccessibility(kObject30102, 30101, Common::Rect(0, 0, 0, 0), 1, 52, 0);
	_app->objectAddRotationAccessibility(kObject30102, 30101, Common::Rect(1741, 134, 1826, 221), 1, 52, 1);
	_app->objectAdd(kObject30110, "", "", 1);
	_app->objectAddPresentation(kObject30110);
	_app->objectPresentationAddImageToRotation(kObject30110, 0, 30101, 0);
	_app->objectAddPresentation(kObject30110);
	_app->objectPresentationAddAnimationToRotation(kObject30110, 1, 30101, 1, 200, 1095237632, 4);
	_app->objectPresentationSetAnimationOnRotation(kObject30110, 1, 0, 30001);
	_app->objectAddPresentation(kObject30110);
	_app->objectPresentationAddImageToRotation(kObject30110, 2, 30101, 2);
	_app->objectAddPresentation(kObject30110);
	_app->objectPresentationAddImageToRotation(kObject30110, 3, 30101, 3);
	_app->objectAdd(kObjectNotung, "Sword Notung", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectNotung, kPuzzle35019, Common::Rect(290, 167, 363, 232), 0, 52, 0);
	_app->objectAddPresentation(kObjectNotung);
	_app->objectPresentationAddImageToPuzzle(kObjectNotung, 0, kPuzzle35019, "FOS00N04P01L01.bmp", 0, 16, 1, 1, 1000);
	_app->objectAddPresentation(kObjectNotung);
	_app->objectPresentationAddImageToPuzzle(kObjectNotung, 1, kPuzzle35019, "FOS00N04P01L02.bmp", 303, 192, 1, 1, 1000);
	_app->objectAddPresentation(kObjectNotung);
	_app->objectPresentationAddImageToPuzzle(kObjectNotung, 2, kPuzzle35019, "FOS00N04P01L03.bmp", 252, 41, 1, 1, 1000);
	_app->objectAddPresentation(kObjectNotung);
	_app->objectPresentationAddImageToPuzzle(kObjectNotung, 3, kPuzzle35019, "FOS00N04P01L04.bmp", 284, 193, 1, 1, 1000);
	_app->objectAdd(kObject30059, "", "", 1);
	_app->objectAddRotationAccessibility(kObject30059, 30302, Common::Rect(2183, -268, 2752, 33), 1, 52, 0);
	_app->objectAddRotationAccessibility(kObject30059, 30303, Common::Rect(2183, -268, 2752, 33), 1, 52, 1);
	_app->objectAdd(kObject30061, "", "", 1);
	_app->objectAddPresentation(kObject30061);
	_app->objectPresentationAddImageToRotation(kObject30061, 0, 30001, 0);
	_app->objectAddPresentation(kObject30061);
	_app->objectPresentationAddImageToRotation(kObject30061, 1, 30010, 0);
	_app->objectAddPresentation(kObject30061);
	_app->objectPresentationAddImageToRotation(kObject30061, 2, 30301, 0);
	_app->objectAddPresentation(kObject30061);
	_app->objectPresentationAddImageToRotation(kObject30061, 3, 30008, 0);
	_app->objectAddPresentation(kObject30061);
	_app->objectPresentationAddImageToRotation(kObject30061, 4, 30009, 0);
	_app->objectAddPresentation(kObject30061);
	_app->objectPresentationAddImageToRotation(kObject30061, 5, 30702, 0);
	_app->objectAddPresentation(kObject30061);
	_app->objectPresentationAddImageToRotation(kObject30061, 6, 30703, 0);
	_app->objectAdd(kObject30200, "", "", 1);
	_app->objectAddRotationAccessibility(kObject30200, 30301, Common::Rect(3452, 64, 3505, 109), 1, 52, 0);
	_app->objectAddRotationAccessibility(kObject30200, 30301, Common::Rect(2939, 57, 2995, 99), 1, 52, 1);
	_app->objectAddRotationAccessibility(kObject30200, 30301, Common::Rect(873, 68, 949, 120), 1, 52, 2);
	_app->objectAddRotationAccessibility(kObject30200, 30301, Common::Rect(2425, 71, 2478, 118), 1, 52, 3);
	_app->objectAddRotationAccessibility(kObject30200, 30301, Common::Rect(1390, 62, 1451, 108), 1, 52, 4);
	_app->objectAddRotationAccessibility(kObject30200, 30301, Common::Rect(1912, 54, 1959, 118), 1, 52, 5);
	_app->objectAddRotationAccessibility(kObject30200, 30301, Common::Rect(367, 62, 425, 111), 1, 52, 6);
	_app->soundAdd(30007, 1, "1267.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30000, 2, "1269.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, 18);
	_app->soundAdd(30001, 2, "1270.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, 18);
	_app->soundAdd(30002, 2, "1271.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, 18);
	_app->soundAdd(30003, 2, "1272.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, 18);
	_app->soundAdd(30004, 2, "1273.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, 18);
	_app->soundAdd(30005, 2, "1274.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, 18);
	_app->soundAdd(30006, 2, "1275.was", _app->getConfiguration().backgroundMusic.loadFrom, 2, 18);
	_app->soundSetVolume(30005, 90);
	_app->soundSetVolume(30002, 95);
	_app->rotationAddAmbientSound(30001, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30002, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30003, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30004, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30005, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30006, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30008, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30009, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30010, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30011, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30012, 30000, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35019, 30000, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35100, 30000, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30101, 30003, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35101, 30003, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35103, 30003, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35104, 30003, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35105, 30003, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35106, 30003, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35107, 30003, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35108, 30003, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35109, 30003, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35110, 30003, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35111, 30003, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30301, 30004, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30302, 30004, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30303, 30004, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35001, 30004, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35011, 30004, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30401, 30005, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30402, 30005, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30801, 30005, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30401, 30006, 90, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30402, 30006, 90, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30801, 30006, 90, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30011, 30006, 87, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35002, 30006, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30501, 30002, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30601, 30002, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30602, 30002, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35009, 30002, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35005, 30002, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35003, 30002, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35004, 30002, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35006, 30002, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35007, 30002, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35008, 30002, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30701, 30001, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30702, 30001, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30703, 30001, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(30704, 30001, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35010, 30001, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle35020, 30001, 100, 0, 1, 1, 10);
	_app->soundAdd(30201, 3, "1276.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30202, 3, "1277.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30300, 3, "1278.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30301, 3, "1279.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30302, 3, "1280.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30303, 3, "1281.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30304, 3, "1282.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30305, 3, "1283.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30306, 3, "1284.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30307, 3, "1285.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30308, 3, "1286.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30309, 3, "1287.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30310, 3, "1288.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30311, 3, "1289.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30312, 3, "1290.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30313, 3, "1291.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30314, 3, "1292.wav", _app->getConfiguration().ambientEffect.loadFrom);
	_app->soundAdd(30501, 3, "1293.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30502, 3, "1294.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30315, 3, "1295.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30316, 3, "1296.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30317, 3, "1297.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30318, 3, "1298.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30319, 3, "1299.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30320, 3, "1300.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30321, 3, "1301.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30322, 3, "1302.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30323, 3, "1303.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30324, 3, "1304.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30325, 3, "1305.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30326, 3, "1306.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30503, 3, "1307.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30504, 3, "1308.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30505, 3, "1309.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30506, 3, "1310.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30508, 3, "1311.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30509, 3, "1312.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30510, 3, "1313.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30511, 3, "1314.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30512, 3, "1315.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30513, 3, "1316.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30514, 3, "1317.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30500, 4, "1320.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(30100, 5, "1322.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30101, 5, "1323.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30102, 5, "1324.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30103, 5, "1325.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30104, 5, "1326.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30105, 5, "1327.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30106, 5, "1328.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30107, 5, "1329.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30108, 5, "1330.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30161, 5, "1331.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30109, 5, "1332.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30118, 5, "1333.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30111, 5, "1334.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30112, 5, "1335.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30113, 5, "1336.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30114, 5, "1337.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30115, 5, "1338.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30116, 5, "1340.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30110, 5, "1339.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30117, 5, "1341.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30120, 5, "1342.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30121, 5, "1343.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30122, 5, "1344.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30123, 5, "1345.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30124, 5, "1346.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30125, 5, "1347.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30126, 5, "1348.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30127, 5, "1349.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30128, 5, "1350.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30129, 5, "1351.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30130, 5, "1352.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30131, 5, "1353.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30132, 5, "1354.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30133, 5, "1355.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30134, 5, "1356.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30135, 5, "1357.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30136, 5, "1358.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30150, 5, "1359.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30151, 5, "1360.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30152, 5, "1361.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30153, 5, "1362.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30154, 5, "1363.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30155, 5, "1364.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30156, 5, "1365.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30162, 5, "1366.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30163, 5, "1367.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30164, 5, "1368.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30165, 5, "1369.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30166, 5, "1370.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30167, 5, "1371.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(30168, 5, "1372.wav", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->rotationAdd3DSound(30602, 30202, 1, 1, 15, 100, 174.0, 10);
	_app->rotationAdd3DSound(30601, 30202, 1, 1, 15, 80, 140.0, 10);
	_app->rotationAdd3DSound(30001, 30501, 1, 1, 20, 75, 244.0, 3);
	_app->rotationAdd3DSound(30002, 30501, 1, 1, 20, 75, 347.0, 6);
	_app->rotationAdd3DSound(30003, 30501, 1, 1, 20, 85, 158.0, 3);
	_app->rotationAdd3DSound(30004, 30501, 1, 1, 20, 70, 142.0, 2);
	_app->rotationAdd3DSound(30011, 30513, 1, 1, 40, 90, 56.0, 10);
	_app->varDefineByte(30009, 2);
	_app->varDefineByte(30010, 1);
	_app->varDefineByte(30011, 3);
	_app->varDefineByte(30012, 4);
	_app->varDefineByte(30013, 5);
	_app->varDefineByte(30014, 6);
	_app->varDefineByte(30015, 7);
	_app->varDefineByte(30016, 25);
	_app->varDefineByte(30017, 0);
	_app->varDefineByte(30050, 0);
	_app->varDefineByte(30051, 0);
	_app->varDefineByte(30052, 0);
	_app->varDefineByte(30053, 0);
	_app->varDefineByte(30054, 0);
	_app->varDefineByte(30055, 0);
	_app->varDefineByte(30019, 0);
	_app->varDefineByte(30020, 0);
	_app->varDefineByte(30021, 0);
	_app->varDefineByte(30022, 0);
	_app->varDefineByte(30023, 0);
	_app->varDefineByte(30024, 0);
	_app->varDefineByte(30025, 0);
	_app->varDefineByte(30026, 0);
	_app->varDefineByte(30027, 0);
	_app->varDefineByte(30028, 0);
	_app->varDefineByte(30029, 0);
	_app->varDefineByte(30030, 0);
	_app->varDefineByte(30031, 0);
	_app->varDefineByte(30032, 0);
	_app->varDefineByte(30033, 0);
	_app->varDefineByte(30038, 0);
	_app->varDefineByte(30034, 0);
	_app->varDefineByte(30035, 0);
	_app->varDefineByte(30036, 0);
	_app->varDefineByte(30037, 0);
	_app->varDefineByte(30039, 0);
	_app->varDefineByte(30040, 0);
	_app->varDefineByte(30041, 0);
	_app->varDefineByte(30042, 0);
	_app->varDefineByte(30043, 0);
	_app->varDefineByte(30044, 0);
	_app->varDefineByte(30045, 0);
	_app->varDefineByte(30047, 0);
	_app->varDefineByte(30056, 1);
	_app->varDefineByte(30060, 0);
	_app->varDefineByte(30061, 0);
	_app->varDefineByte(30062, 0);
	_app->varDefineByte(30063, 0);
	_app->varDefineByte(30064, 0);
	_app->varDefineByte(30065, 0);
	_app->varDefineByte(30070, 0);
	_app->varDefineByte(30071, 0);
	_app->varDefineByte(30072, 0);
	_app->varDefineByte(30066, 0);
	_app->varDefineByte(30067, 0);
	_app->varDefineByte(30068, 0);
	_app->varDefineByte(30069, 1);
	_app->varDefineByte(30073, 0);
	_app->varDefineByte(30074, 0);
	_app->varDefineByte(30075, 0);
	_app->varDefineByte(30076, 0);
	_app->varDefineByte(30077, 0);
	_app->varDefineByte(30078, 0);
	_app->varDefineByte(30201, 0);
	_app->varDefineByte(30203, 0);
	_app->varDefineByte(30204, 0);
	_app->varDefineByte(30205, 0);
	_app->varDefineByte(30206, 0);
	_app->varDefineByte(30207, 0);
	_app->varDefineByte(30208, 0);
	_app->varDefineByte(30209, 0);
	_app->varDefineByte(30210, 0);
	_app->varDefineByte(30211, 0);
	_app->varDefineFloat(30046, -10.0);
	_app->varDefineFloat(30042, 1.0);
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
