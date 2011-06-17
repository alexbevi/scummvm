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

#include "ring/game/ring/ring_zonesystem.h"

#include "ring/base/preferences.h"
#include "ring/base/saveload.h"

#include "ring/game/ring/ring_application.h"
#include "ring/game/ring/ring_shared.h"

#include "ring/graphics/dragControl.h"
#include "ring/graphics/image.h"

#include "ring/debug.h"
#include "ring/ring.h"
#include "ring/helpers.h"

using namespace RingGame;

namespace Ring {

ZoneSystemRing::ZoneSystemRing(ApplicationRing *application) : _app(application) {
	// Button
	_presentationIndexSY = 0;
	_prefsVolume = 0;
	_prefsSubtitles = false;
	_prefsVolumeDialog = 0;
	_object1Visible = false;

	// Bag
	_offsetX = 0;
	_offsetX0 = 0;
	_coordX = 0;
}

ZoneSystemRing::~ZoneSystemRing() {
	// Zero-out passed pointers
	_app = NULL;
}

void ZoneSystemRing::onInit() {
	debugC(1, kRingDebugLogic, "Initializing System zone (SY)...");

	_app->setZoneAndEnableBag(kZoneSY);

	//////////////////////////////////////////////////////////////////////////
	// Setup system and dialog boxes
	_app->puzzleAdd(kPuzzleMenu);

	_app->objectAdd(kObject1, "", "", 1);
	_app->objectAddPresentation(kObject1);
	_app->objectPresentationAddTextToPuzzle(kObject1, 0, kPuzzleMenu, "", Common::Point(1, 16), kFontDefault, Color(0, -1, 0), Color(-1, -1, -1));

	_app->objectAdd(kObject6, "", "", 1);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 0, kPuzzleMenu, "FO_WOT1_W_A.tga", Common::Point(0, 96), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 1, kPuzzleMenu, "FO_WOT1_W_B.tga", Common::Point(0, 176), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 2, kPuzzleMenu, "FO_WOT1_W_C.tga", Common::Point(0, 126), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 3, kPuzzleMenu, "FO_WOT1_W_D.tga", Common::Point(0, 208), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 4, kPuzzleMenu, "FO_WOT1_W_E.tga", Common::Point(0, 88), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 5, kPuzzleMenu, "FO_WOT1_B_A.tga", Common::Point(415, 281), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 6, kPuzzleMenu, "FO_WOT1_B_C.tga", Common::Point(488, 180), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 7, kPuzzleMenu, "FO_WOT1_B_E.tga", Common::Point(195, 94), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 8, kPuzzleMenu, "FO_BRU1_W_A.tga", Common::Point(0, 45), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 9, kPuzzleMenu, "FO_BRU1_B_A.tga", Common::Point(440, 66), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 10, kPuzzleMenu, "FO_BRU1_B_B.tga", Common::Point(490, 208), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 11, kPuzzleMenu, "FO_WOT2_W_A.tga", Common::Point(0, 28), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 12, kPuzzleMenu, "FO_WOT2_W_B.tga", Common::Point(0, 22), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 13, kPuzzleMenu, "FO_WOT2_W_C.tga", Common::Point(0, 67), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 14, kPuzzleMenu, "FO_WOT2_W_D.tga", Common::Point(0, 194), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 15, kPuzzleMenu, "FO_WOT2_B_A.tga", Common::Point(477, 200), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 16, kPuzzleMenu, "FO_WOT2_B_C.tga", Common::Point(496, 251), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 17, kPuzzleMenu, "FO_BRU2_W_A.tga", Common::Point(0, 186), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 18, kPuzzleMenu, "FO_BRU2_B_A.tga", Common::Point(443, 222), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 19, kPuzzleMenu, "FO_BRU2_B_B.tga", Common::Point(493, 219), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 20, kPuzzleMenu, "FO_WOT3_W_A.tga", Common::Point(0, 16), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 21, kPuzzleMenu, "FO_WOT3_W_B.tga", Common::Point(0, 107), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 22, kPuzzleMenu, "FO_WOT3_W_C.tga", Common::Point(0, 46), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 23, kPuzzleMenu, "FO_WOT3_W_D.tga", Common::Point(0, 26), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 24, kPuzzleMenu, "FO_WOT3_B_A.tga", Common::Point(508, 238), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 25, kPuzzleMenu, "FO_WOT3_B_B.tga", Common::Point(381, 216), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject6);
	_app->objectPresentationAddImageToPuzzle(kObject6, 26, kPuzzleMenu, "FO_WOT3_B_D.tga", Common::Point(408, 246), true, kDrawType3, 1000);

	_app->objectAdd(kObject7, "", "", 1);
	_app->objectAddPresentation(kObject7);
	_app->objectPresentationAddImageToPuzzle(kObject7, 0, kPuzzleMenu, "SY_ISHA.bmp", Common::Point(0, 16), true, kDrawType1, 1000);

	_app->objectAdd(kObject2, "", "", 1);
	_app->objectAddPresentation(kObject2);
	_app->objectPresentationAddImageToPuzzle(kObject2, 0, kPuzzleMenu, "Exit.bmp", Common::Point(160, 165), true, kDrawType1, 1000);
	_app->objectAddPuzzleAccessibility(kObject2, kPuzzleMenu, Common::Rect(262, 270, 321, 306), false, kCursorMenuActive, 1);
	_app->objectAddPuzzleAccessibility(kObject2, kPuzzleMenu, Common::Rect(310, 270, 370, 306), false, kCursorMenuActive, 0);
	_app->objectSetPuzzleAccessibilityKey(kObject2, 0, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject2, 1, Common::KEYCODE_ESCAPE);
	_app->objectAddPresentation(kObject2);
	_app->objectPresentationAddImageToPuzzle(kObject2, 1, kPuzzleMenu, "ex_yes.bmp", Common::Point(261, 279), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject2);
	_app->objectPresentationAddImageToPuzzle(kObject2, 2, kPuzzleMenu, "ex_no.bmp", Common::Point(318, 279), true, kDrawType1, 1000);

	_app->objectAdd(kObject3, "", "", 1);
	_app->objectAddPresentation(kObject3);
	_app->objectPresentationAddImageToPuzzle(kObject3, 0, kPuzzleMenu, "Warning.bmp", Common::Point(160, 165), true, kDrawType1, 1000);
	_app->objectPresentationAddTextToPuzzle(kObject3, 0, kPuzzleMenu, "", Common::Point(200, 200), kFontDefault, Color(255, 95, 0), Color(-1, -1, -1));
	_app->objectPresentationAddTextToPuzzle(kObject3, 0, kPuzzleMenu, "", Common::Point(300, 280), kFontDefault, Color(255, 95, 0), Color(-1, -1, -1));
	_app->objectAddPresentation(kObject3);
	_app->objectPresentationAddImageToPuzzle(kObject3, 1, kPuzzleMenu, "wr_ok.tga", Common::Point(313, 281), true, kDrawType3, 1000);
	_app->objectAddPuzzleAccessibility(kObject3, kPuzzleMenu, Common::Rect(286, 269, 363, 307), false, kCursorMenuActive, 0);
	_app->objectSetPuzzleAccessibilityKey(kObject3, 0, Common::KEYCODE_RETURN);

	_app->objectAdd(kObject4, "", "", 1);
	_app->objectAddPresentation(kObject4);
	_app->objectPresentationAddImageToPuzzle(kObject4, 0, kPuzzleMenu, "Question.bmp", Common::Point(160, 165), true, kDrawType1, 1000);
	_app->objectPresentationAddTextToPuzzle(kObject4, 0, kPuzzleMenu, "", Common::Point(200, 200), kFontDefault, Color(255, 95, 0), Color(-1, -1, -1));
	_app->objectPresentationAddTextToPuzzle(kObject4, 0, kPuzzleMenu, "", Common::Point(200, 280), kFontDefault, Color(255, 95, 0), Color(-1, -1, -1));
	_app->objectAddPresentation(kObject4);
	_app->objectPresentationAddImageToPuzzle(kObject4, 1, kPuzzleMenu, "g_ok.tga", Common::Point(181, 257), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject4);
	_app->objectPresentationAddImageToPuzzle(kObject4, 2, kPuzzleMenu, "qu_cancel.tga", Common::Point(181, 282), true, kDrawType3, 1000);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(277, 300, 347, 340), false, kCursorMenuActive, 0);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(350, 300, 380, 340), false, kCursorMenuActive, 1);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 0, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 1, Common::KEYCODE_ESCAPE);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(180, 250, 250, 281), false, kCursorMenuActive, 2);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(180, 283, 250, 309), false, kCursorMenuActive, 3);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 2, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 3, Common::KEYCODE_ESCAPE);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(180, 250, 250, 281), false, kCursorMenuActive, 4);
	_app->objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(180, 283, 250, 309), false, kCursorMenuActive, 5);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 4, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject4, 5, Common::KEYCODE_ESCAPE);

	_app->objectAdd(kObject5, "", "", 1);
	_app->puzzleAdd(kPuzzleInsertCd);
	_app->puzzleAddBackgroundImage(kPuzzleInsertCd, "insertcd.bmp", Common::Point(0, 16), true);

	_app->objectAdd(kObject90912, "", "", 1);
	_app->objectAddPresentation(kObject90912);
	_app->objectPresentationAddImageToPuzzle(kObject90912, 0, kPuzzleInsertCd, "g_ok.tga", Common::Point(306, 274), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObject90912);
	_app->objectPresentationAddTextToPuzzle(kObject90912, 1, kPuzzleInsertCd, "", Common::Point(320, 240), kFontDefault, Color(255, 95, 0), Color(-1, -1, -1));
	_app->objectPresentationAddTextToPuzzle(kObject90912, 1, kPuzzleInsertCd, "", Common::Point(320, 260), kFontDefault, Color(255, 95, 0), Color(-1, -1, -1));
	_app->objectPresentationShow(kObject90912, 1);
	_app->objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), true, kCursorMenuActive, 0);
	_app->objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), true, kCursorMenuActive, 1);
	_app->objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), true, kCursorMenuActive, 2);
	_app->objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), true, kCursorMenuActive, 3);
	_app->objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), true, kCursorMenuActive, 4);
	_app->objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), true, kCursorMenuActive, 5);
	_app->objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), true, kCursorMenuActive, 6);
	_app->objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), true, kCursorMenuActive, 7);
	_app->objectAddPuzzleAccessibility(kObject90912, kPuzzleInsertCd, Common::Rect(266, 268, 388, 311), true, kCursorMenuActive, 8);
	_app->objectSetPuzzleAccessibilityKey(kObject90912, 0, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject90912, 1, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject90912, 2, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject90912, 3, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject90912, 4, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject90912, 5, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject90912, 6, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject90912, 7, Common::KEYCODE_RETURN);
	_app->objectSetPuzzleAccessibilityKey(kObject90912, 8, Common::KEYCODE_RETURN);

	//////////////////////////////////////////////////////////////////////////
	// General Menu
	_app->puzzleAdd(kPuzzleGeneralMenu);
	_app->puzzleAddBackgroundImage(kPuzzleGeneralMenu, "GenMen.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzlePreferences);
	_app->puzzleAddBackgroundImage(kPuzzlePreferences, "Preferences.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzleSave);
	_app->puzzleAddBackgroundImage(kPuzzleSave, "Save.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzleLoad);
	_app->puzzleAddBackgroundImage(kPuzzleLoad, "Load.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzleGameStatus);
	_app->puzzleAddBackgroundImage(kPuzzleGameStatus, "GameStat.bmp", Common::Point(0, 16), true);

	// General Menu objects
	_app->objectAdd(kObjectMenuNewGame, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectMenuNewGame, kPuzzleGeneralMenu, Common::Rect(148, 69, 500, 99), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObjectMenuNewGame);
	_app->objectPresentationAddImageToPuzzle(kObjectMenuNewGame, 0, kPuzzleGeneralMenu, "gm_new.bmp", Common::Point(148, 85), true, kDrawType1, 1000);
	_app->objectAdd(kObjectMenuPreferences, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectMenuPreferences, kPuzzleGeneralMenu, Common::Rect(148, 105, 500, 135), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObjectMenuPreferences);
	_app->objectPresentationAddImageToPuzzle(kObjectMenuPreferences, 0, kPuzzleGeneralMenu, "gm_pre.bmp", Common::Point(148, 121), true, kDrawType1, 1000);
	_app->objectAdd(kObjectMenuLoad, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectMenuLoad, kPuzzleGeneralMenu, Common::Rect(148, 168, 500, 198), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObjectMenuLoad);
	_app->objectPresentationAddImageToPuzzle(kObjectMenuLoad, 0, kPuzzleGeneralMenu, "gm_loa.bmp", Common::Point(148, 184), true, kDrawType1, 1000);
	_app->objectAdd(kObjectMenuSave, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectMenuSave, kPuzzleGeneralMenu, Common::Rect(148, 236, 500, 266), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObjectMenuSave);
	_app->objectPresentationAddImageToPuzzle(kObjectMenuSave, 0, kPuzzleGeneralMenu, "gm_sav.bmp", Common::Point(148, 252), true, kDrawType1, 1000);
	_app->objectAdd(kObjectMenuContinue, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectMenuContinue, kPuzzleGeneralMenu, Common::Rect(148, 303, 500, 333), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObjectMenuContinue);
	_app->objectPresentationAddImageToPuzzle(kObjectMenuContinue, 0, kPuzzleGeneralMenu, "gm_con.bmp", Common::Point(148, 319), true, kDrawType1, 1000);
	_app->objectAdd(kObjectMenuStatus, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectMenuStatus, kPuzzleGeneralMenu, Common::Rect(148, 342, 500, 372), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObjectMenuStatus);
	_app->objectPresentationAddImageToPuzzle(kObjectMenuStatus, 0, kPuzzleGeneralMenu, "gm_sta.bmp", Common::Point(148, 358), true, kDrawType1, 1000);
	_app->objectAdd(kObjectMenuExit, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectMenuExit, kPuzzleGeneralMenu, Common::Rect(148, 380, 500, 410), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObjectMenuExit);
	_app->objectPresentationAddImageToPuzzle(kObjectMenuExit, 0, kPuzzleGeneralMenu, "gm_exi.bmp", Common::Point(148, 396), true, kDrawType1, 1000);

	// Preferences objects
	_app->objectAdd(kObjectPreferencesCancel, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectPreferencesCancel, kPuzzlePreferences, Common::Rect(410, 420, 490, 445), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObjectPreferencesCancel);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferencesCancel, 0, kPuzzlePreferences, "g_cancel.tga", Common::Point(407, 421), true, kDrawType3, 1000);
	_app->objectSetPuzzleAccessibilityKey(kObjectPreferencesCancel, 0, Common::KEYCODE_ESCAPE);
	_app->objectAdd(kObjectPreferencesOk, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectPreferencesOk, kPuzzlePreferences, Common::Rect(320, 420, 370, 445), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObjectPreferencesOk);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferencesOk, 0, kPuzzlePreferences, "g_ok.tga", Common::Point(328, 421), true, kDrawType3, 1000);
	_app->objectSetPuzzleAccessibilityKey(kObjectPreferencesOk, 0, Common::KEYCODE_RETURN);
	_app->objectAdd(kObjectPreferencesSubtitles, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectPreferencesSubtitles, kPuzzlePreferences, Common::Rect(310, 315, 370, 350), true, kCursorMenuActive, 0);
	_app->objectAddPuzzleAccessibility(kObjectPreferencesSubtitles, kPuzzlePreferences, Common::Rect(400, 315, 460, 350), true, kCursorMenuActive, 1);
	_app->objectAddPresentation(kObjectPreferencesSubtitles);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferencesSubtitles, 0, kPuzzlePreferences, "pr_on.bmp", Common::Point(317, 326), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectPreferencesSubtitles);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferencesSubtitles, 1, kPuzzlePreferences, "pr_off.bmp", Common::Point(402, 326), true, kDrawType1, 1000);
	_app->objectAdd(kObjectPreferences3dSound, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectPreferences3dSound, kPuzzlePreferences, Common::Rect(355, 260, 420, 295), true, kCursorMenuActive, 1);
	_app->objectAddPresentation(kObjectPreferences3dSound);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferences3dSound, 0, kPuzzlePreferences, "pr_left.tga", Common::Point(336, 288), true, kDrawType3, 1000);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferences3dSound, 0, kPuzzlePreferences, "pr_right.tga", Common::Point(428, 288), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObjectPreferences3dSound);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferences3dSound, 1, kPuzzlePreferences, "pr_right.tga", Common::Point(336, 288), true, kDrawType3, 1000);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferences3dSound, 1, kPuzzlePreferences, "pr_left.tga", Common::Point(428, 288), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObjectPreferences3dSound);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferences3dSound, 2, kPuzzlePreferences, "pr_3ds.tga", Common::Point(356, 281), true, kDrawType3, 1000);
	_app->objectAdd(kObjectPreferencesSliderVolume, "", "ni_handsel", 4);
	_app->objectAddPuzzleAccessibility(kObjectPreferencesSliderVolume, kPuzzlePreferences, Common::Rect(300, 140, 600, 180), true, kCursorMenuActive, 1);
	_app->objectAddPresentation(kObjectPreferencesSliderVolume);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferencesSliderVolume, 0, kPuzzlePreferences, "pr_slider.tga", Common::Point(314, 155), true, kDrawType3, 1000);
	_app->objectPresentationShow(kObjectPreferencesSliderVolume);
	_app->objectSetActiveDrawCursor(kObjectPreferencesSliderVolume, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObjectPreferencesSliderVolume, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectPreferencesSliderDialog, "", "ni_handsel", 4);
	_app->objectAddPuzzleAccessibility(kObjectPreferencesSliderDialog, kPuzzlePreferences, Common::Rect(300, 197, 600, 237), true, kCursorMenuActive, 1);
	_app->objectAddPresentation(kObjectPreferencesSliderDialog);
	_app->objectPresentationAddImageToPuzzle(kObjectPreferencesSliderDialog, 0, kPuzzlePreferences, "pr_slider.tga", Common::Point(314, 212), true, kDrawType3, 1000);
	_app->objectPresentationShow(kObjectPreferencesSliderDialog);
	_app->objectSetActiveDrawCursor(kObjectPreferencesSliderDialog, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectSetPassiveDrawCursor(kObjectPreferencesSliderDialog, Common::Point(15, 15), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectCredits, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectCredits, kPuzzlePreferences, Common::Rect(0, 448, 20, 640), true, kCursorMenuActive, 1);

	// Loading screen objects
	_app->objectAdd(kObjectLoadOk, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectLoadOk, kPuzzleLoad, Common::Rect(325, 418, 375, 461), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObjectLoadOk);
	_app->objectPresentationAddImageToPuzzle(kObjectLoadOk, 0, kPuzzleLoad, "g_ok.tga", Common::Point(328, 421), true, kDrawType3, 1000);
	_app->objectSetPuzzleAccessibilityKey(kObjectLoadOk, 0, Common::KEYCODE_RETURN);
	_app->objectAdd(kObjectLoadCancel, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectLoadCancel, kPuzzleLoad, Common::Rect(416, 418, 498, 461), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObjectLoadCancel);
	_app->objectPresentationAddImageToPuzzle(kObjectLoadCancel, 0, kPuzzleLoad, "g_cancel.tga", Common::Point(407, 421), true, kDrawType3, 1000);
	_app->objectSetPuzzleAccessibilityKey(kObjectLoadCancel, 0, Common::KEYCODE_ESCAPE);

	// Saving screen objects
	_app->objectAdd(kObjectSaveOk, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectSaveOk, kPuzzleSave, Common::Rect(325, 418, 375, 461), true, kCursorMenuActive, 0);
	_app->objectAdd(kObjectSaveCancel, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectSaveCancel, kPuzzleSave, Common::Rect(416, 418, 498, 461), true, kCursorMenuActive, 0);
	_app->objectSetPuzzleAccessibilityKey(kObjectSaveCancel, 0, Common::KEYCODE_ESCAPE);
	_app->objectAddPresentation(kObjectSaveOk);
	_app->objectPresentationAddImageToPuzzle(kObjectSaveOk, 0, kPuzzleSave, "g_ok.tga", Common::Point(328, 421), true, kDrawType3, 1000);
	_app->objectAddPresentation(kObjectSaveCancel);
	_app->objectPresentationAddImageToPuzzle(kObjectSaveCancel, 0, kPuzzleSave, "g_cancel.tga", Common::Point(407, 421), true, kDrawType3, 1000);
	_app->objectSetPuzzleAccessibilityKey(kObjectSaveOk, 0, Common::KEYCODE_RETURN);
	_app->objectAdd(kObjectSaveName, "", "", 1);
	_app->objectAddPresentation(kObjectSaveName);
	_app->objectPresentationAddTextToPuzzle(kObjectSaveName, 0, kPuzzleSave, "", Common::Point(344, 181), kFontDefault, Color(255, 95, 0), Color(-1, -1, -1));
	_app->objectPresentationAddTextToPuzzle(kObjectSaveName, 0, kPuzzleSave, "", Common::Point(0, 0),     kFontDefault, Color(255, 95, 0), Color(-1, -1, -1));
	_app->objectPresentationAddAnimationToPuzzle(kObjectSaveName, 0, kPuzzleSave, "kybcur", kImageTypeBMP, Common::Point(0, 0), kDrawType1, 1000, 6, 12.5f, 16);
	_app->objectPresentationSetAnimationCoordinatesOnPuzzle(kObjectSaveName, 0, Common::Point(346, 181));

	_app->setArchiveType(kArchiveFile);
	_app->objectPresentationAddImageToPuzzle(kObjectSaveName, 0, kPuzzleSave, "osc.bmp", Common::Point(0, 0), true, kDrawType1, 1000);
	if (_app->getConfiguration().artSY )
		_app->setArchiveType(kArchiveArt);

	_app->objectPresentationShow(kObjectSaveName, 0);

	_app->visualListAddToPuzzle(1, kPuzzleLoad, 65,
	                            "", "" /* save folder */, "", "up_gun.tga", "up_gur.tga", "", "up_gua.tga", "down_gun.tga", "down_gur.tga", "",
	                            "down_gua.tga", "load_gun.tga", "load_gua.tga",
	                            kDrawType3, Common::Point(0, 0), Common::Point(0, 0), 335, 127, 300, 35, 45,
	                            3, Common::Point(330, 349), 320, 339, 40, 40, Common::Point(330, 380), 320,
	                            370, 40, 40, Common::Point(0, 0), kImageTypeBMP, kDrawType1, 311, 137, 4,
	                            Color(255, 95, 0), Color(245, 235, 50), Color(-1, -1, -1), kFontDefault,
	                            kArchiveFile);

	// Game status screen
	_app->objectAdd(kObjectStatusOk, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectStatusOk, kPuzzleGameStatus, Common::Rect(28, 79, 107, 109), true, kCursorMenuActive, 0);
	_app->objectAddPresentation(kObjectStatusOk);
	_app->objectPresentationAddImageToPuzzle(kObjectStatusOk, 0, kPuzzleGameStatus, "g_ok.tga", Common::Point(46, 95), true, kDrawType3, 1000);
	_app->objectSetPuzzleAccessibilityKey(kObjectStatusOk, 0, Common::KEYCODE_RETURN);
	_app->visualAddShowToPuzzle(2, kPuzzleGameStatus, 1, 4, 295, 343, 28, 4, 300, 38655);
	_app->objectAdd(kObjectStatusProgress, "", "", 1);
	_app->objectAddPresentation(kObjectStatusProgress);
	_app->objectPresentationAddTextToPuzzle(kObjectStatusProgress, 0, kPuzzleGameStatus, "", Common::Point(600, 327), kFontDefault, Color(255, -106, 0), Color(-1, -1, -1));
	_app->objectPresentationAddTextToPuzzle(kObjectStatusProgress, 0, kPuzzleGameStatus, "", Common::Point(600, 356), kFontDefault, Color(255, -106, 0), Color(-1, -1, -1));
	_app->objectPresentationAddTextToPuzzle(kObjectStatusProgress, 0, kPuzzleGameStatus, "", Common::Point(600, 384), kFontDefault, Color(255, -106, 0), Color(-1, -1, -1));
	_app->objectPresentationAddTextToPuzzle(kObjectStatusProgress, 0, kPuzzleGameStatus, "", Common::Point(600, 410), kFontDefault, Color(255, -106, 0), Color(-1, -1, -1));
	_app->objectPresentationShow(kObjectStatusProgress);

	// Sounds
	_app->soundAdd(90001, kSoundTypeDialog, "1844.wac", kLoadFromDisk, 2, _app->getConfiguration().dialog.soundChunck);

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

void ZoneSystemRing::onKeyDown(const Common::KeyState &keyState) {
	if (!_app->hasCurrentPuzzle())
		return;

	PuzzleId puzzleId = _app->getCurrentPuzzleId();
	switch (puzzleId) {
	default:
		break;

	case kPuzzleLoad:
		if (keyState.keycode == Common::KEYCODE_DELETE) {
			_app->messageGet("DoYouWantToDeleteSavedGame");
			_app->messageShowQuestion(4);
		}
		break;

	case kPuzzleSave:
		switch (keyState.keycode) {
		default:
			if (_app->objectPresentationGetTextWidth(kObjectSaveName, 0, 0) >= 280)
				return;

			*_app->getSaveManager()->getName() += (char)keyState.ascii;
			break;

		case Common::KEYCODE_ESCAPE:
			_app->getSaveManager()->getName()->clear();
			break;

		case Common::KEYCODE_BACKSPACE:
			if (_app->getSaveManager()->getName()->size() != 1)
				_app->getSaveManager()->getName()->deleteLastChar();
			break;

		case Common::KEYCODE_RETURN:
			// Do nothing
			return;
		}

		_app->objectPresentationSetTextToPuzzle(kObjectSaveName, 0, 0, *_app->getSaveManager()->getName());
		_app->objectPresentationSetTextCoordinatesToPuzzle(kObjectSaveName, 0, 0, Common::Point(344, 181));
		_app->objectPresentationSetAnimationCoordinatesOnPuzzle(kObjectSaveName, 0, Common::Point(346 + (int16)_app->objectPresentationGetTextWidth(kObjectSaveName, 0, 0), 181));
		break;
	}
}

void ZoneSystemRing::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	RingEngine *engine = ((RingEngine *)g_engine);

	switch (id) {
	default:
		break;

	case kObject1:
		if (!_app->bagHasClickedObject()) {
			if (_object1Visible) {
				_app->objectPresentationHideAndRemove(kObject1);
				_object1Visible = false;
			} else {
				_app->objectPresentationShow(kObject1);
				_app->rotationSet3DSoundOn(50004, 1);
				_app->puzzleSet3DSoundOn(kPuzzle50001, 1);
				_object1Visible = true;
			}
		}
		break;

	case kObject2:
		switch (target) {
		default:
			break;

		case 0:
			_app->objectPresentationHideAndRemove(kObject2);
			_app->objectSetAccessibilityOff(kObject2);
			_app->puzzleSetMod(kPuzzleMenu, 1, 0);
			break;

		case 1:
			Engine::quitGame();
			break;
		}
		break;

	case kObject3:
		_app->messageHideWarning(target);
		break;

	case kObject4:
		switch (target) {
		default:
		case 0:
		case 1:
			_app->messageHideQuestion(0);
			break;

		case 2:
			_app->cursorSelect(kCursorBusy);
			unsetFlag();
			handleEvents();
			_app->exitZone();
			_app->initZones();
			_app->loadPreferences();
			_app->messageHideQuestion(2);
			break;

		case 3:
			_app->messageHideQuestion(2);
			break;

		// Delete savegame
		case 4: {
			int32 imageIndex  = _app->visualListGetImageIndexClicked(1, 90002);
			int32 objectIndex = _app->visualListGetObjectIndexClicked(1, kPuzzleLoad);

			if (!objectIndex) {
				_app->messageHideQuestion(4);
				_app->messageGet("SelectGame");
				_app->messageShowWarning(0);
				_app->messageHideQuestion(4);
				break;
			}

			// Delete selected savegame
			uint32 slot = _app->visualListGetItemCount(1, kObjectMenuLoad) - ((uint32)imageIndex + 1);
			if (!_app->getSaveManager()->remove(slot)
			 || !removeSavedTimers(slot)) {
				_app->messageHideQuestion(4);
				_app->messageGet("CanNotDeleteSavedGame");
				_app->messageShowWarning(0);
				_app->messageHideQuestion(4);
				break;
			}

			// Hide delete question
			_app->messageHideQuestion(4);
			}
			break;

		case 5:
			_app->messageHideQuestion(4);
			break;
		}
		break;

	case kObjectMenuNewGame:
		_app->messageGet("DoYouWantToStartNewGame");
		_app->messageShowQuestion(2);
		break;

	case kObjectMenuPreferences:
		_app->puzzleSetActive(kPuzzlePreferences);

		_prefsVolume = _app->getPreferenceHandler()->getVolume();
		_prefsVolumeDialog = _app->getPreferenceHandler()->getVolumeDialog();
		_presentationIndexSY = _app->getPreferenceHandler()->getReverseStereo() == 1;
		_prefsSubtitles = _app->getPreferenceHandler()->getShowSubtitles();

		// Original forces subtitles on if no sound is available

		if (_prefsSubtitles) {
			_app->objectPresentationShow(kObjectPreferencesSubtitles, 0);
			_app->objectPresentationHide(kObjectPreferencesSubtitles, 1);
		} else {
			_app->objectPresentationHide(kObjectPreferencesSubtitles, 0);
			_app->objectPresentationShow(kObjectPreferencesSubtitles, 1);
		}

		_app->objectPresentationSetImageCoordinatesOnPuzzle(kObjectPreferencesSliderVolume, 0, 0, Common::Point((int16)(5 * _prefsVolume + 84), 155));
		_app->objectPresentationSetImageCoordinatesOnPuzzle(kObjectPreferencesSliderDialog, 0, 0, Common::Point((int16)(5 * _prefsVolumeDialog + 84), 212));
		_app->objectPresentationHide(kObjectPreferences3dSound);
		_app->objectPresentationShow(kObjectPreferences3dSound, _presentationIndexSY);

		break;

	case kObjectPreferences3dSound:
		_presentationIndexSY = (_presentationIndexSY != 1) ? 1 : 0;
		_app->objectPresentationHide(kObjectPreferences3dSound);
		_app->objectPresentationShow(kObjectPreferences3dSound, _presentationIndexSY);
		break;

	case kObjectPreferencesSubtitles:
		if (target) {
			_app->objectPresentationHide(kObjectPreferencesSubtitles, 0);
			_app->objectPresentationShow(kObjectPreferencesSubtitles, 1);
			_prefsSubtitles = false;
		} else {
			_app->objectPresentationShow(kObjectPreferencesSubtitles, 0);
			_app->objectPresentationHide(kObjectPreferencesSubtitles, 1);
			_prefsSubtitles = true;
		}
		break;

	case kObjectPreferencesOk:
		_app->puzzleSetActive(kPuzzleGeneralMenu);
		_app->getPreferenceHandler()->save(_prefsVolume, _prefsVolumeDialog, _presentationIndexSY != 0 ? 2 : 0, _prefsSubtitles);
		break;

	case kObjectCredits:
		_app->showCredits();
		break;

	case kObjectMenuLoad: {
		// Get the list of games
		SaveStateList list = engine->listSaves(engine->getGameDescription()->desc.gameid);

		for (uint32 i = 0; i < list.size(); i++) {
			SaveStateDescriptor desc = list[i];

			_app->objectAdd((ObjectId)(90500 + i), Common::String::format("#%d", i), desc.description(), 1);
			_app->visualListAdd(1, kPuzzleLoad, (ObjectId)(90500 + i));
		}
		}
		break;

	case kObjectMenuSave: {
		_app->cursorSelect(kCursorBusy);

		unsetFlag();
		handleEvents();

		// Reset name and description
		_app->getSaveManager()->getName()->clear();

		_app->objectPresentationSetTextToPuzzle(kObjectSaveName, 0, 0, *_app->getSaveManager()->getName());
		_app->objectPresentationSetTextCoordinatesToPuzzle(kObjectSaveName, 0, 0, Common::Point(344, 181));
		_app->objectPresentationSetAnimationCoordinatesOnPuzzle(kObjectSaveName, 0, Common::Point(346, 181));

		// Prepare savegame description
		TimeDate date;
		g_system->getTimeAndDate(date);
		Common::String description = Common::String::format("%s %02d:%02d:%02d %02d/%02d/%d",
		                                                    _app->getZoneName(_app->getCurrentGameZone()).c_str(),
		                                                    date.tm_hour, date.tm_min, date.tm_sec,
		                                                    date.tm_mon, date.tm_mday, date.tm_year + 1900);

		// We only save the current zone here
		_app->getSaveManager()->setDescription(_app->getZoneName(_app->getCurrentGameZone()));

		_app->objectPresentationSetTextToPuzzle(kObjectSaveName, 0, 1, description);
		_app->objectPresentationSetTextCoordinatesToPuzzle(kObjectSaveName, 0, 1, Common::Point(344, 155));

		// Store the thumbnail image
		if (_app->getThumbnail() != NULL) {
			Image *thumbnail = _app->getThumbnail()->zoom(0.40645f, 1.0f);
			_app->getSaveManager()->setThumbnail(thumbnail);
			delete thumbnail;
		}

		_app->puzzleSetActive(kPuzzleSave);
		}
		break;

	case kObjectMenuContinue:
		_app->cursorSelect(kCursorBusy);

		unsetFlag();
		handleEvents();

		_app->exitZone();
		_app->initZones();

		if (!_app->getSaveManager()->loadSave(0, kLoadSaveRead)) {
			_app->exitZone();
			_app->initZones();
			_app->loadPreferences();
			_app->messageGet("CanNotCountineGame"); // Typo is normal :S
			_app->messageShowWarning(0);
		}
		break;

	case kObjectLoadOk: {
		_app->cursorSelect(kCursorBusy);

		unsetFlag();
		handleEvents();

		int32 imageIndex  = _app->visualListGetImageIndexClicked(1, 90002);
		if (imageIndex == -1) {
			_app->messageGet("SelectGame");
			_app->messageShowWarning(0);
			break;
		}

		// Compute slot
		uint32 slot = _app->visualListGetItemCount(1, kObjectMenuLoad) - ((uint32)imageIndex + 1);

		// Get a list of saves
		SaveStateList list = engine->listSaves(engine->getGameDescription()->desc.gameid);

		// Prepare loading of game
		_app->visualListRemove(1, kPuzzleLoad, true);
		_app->exitZone();
		_app->initZones();

		// Get info about game
		Common::String name = list[slot].description();

		// Try loading the game, and fallback to autosave if it doesn't work
		if (_app->getSaveManager()->loadSave(slot, kLoadSaveRead)) {
			// Try to reload auto-save
			if (!_app->getSaveManager()->loadSave(0, kLoadSaveRead)) {
				_app->exitZone();
				_app->initZones();
				_app->loadPreferences();
			}

			_app->messageFormat("CanNotLoadGame", name);
			_app->messageShowWarning(0);
		}
		}
		break;

	case kObjectSaveOk: {
		_app->cursorSelect(kCursorBusy);

		unsetFlag();
		handleEvents();

		// Compute the next slot to save to
		uint32 slot = _app->getSaveManager()->getNextSlot();

		Common::SaveFileManager *saveFileMan = g_system->getSavefileManager();
		if (!saveFileMan->copySavefile(SaveManager::getSavegameFile(0), SaveManager::getSavegameFile(slot))
		 || !copySavedTimers(slot)) {
			_app->exitZone();
			_app->initZones();

			// Try reloading autosave
			if (_app->getSaveManager()->loadSave(0, kLoadSaveRead))
				break;

			_app->exitZone();
			_app->initZones();
			_app->loadPreferences();

			_app->messageFormat("CanNotSaveGame", *_app->getSaveManager()->getName());
			_app->messageShowWarning(0);
		}
		}
		break;

	case kObjectLoadCancel:
		_app->puzzleSetActive(kPuzzleGeneralMenu);
		_app->visualListRemove(1, kPuzzleLoad, true);
		break;

	case kObjectMenuStatus:
		_app->puzzleSetActive(kPuzzleGameStatus);
		break;

	case kObjectStatusOk:
	case kObjectSaveCancel:
	case kObjectPreferencesCancel:
		_app->puzzleSetActive(kPuzzleGeneralMenu);
		break;

	case kObjectMenuExit:
		if (_app->puzzleSetMod(kPuzzleMenu, 2, 2)) {
			_app->objectPresentationShow(kObject2, 0);
			_app->objectSetAccessibilityOn(kObject2);
		}
		break;

	// Original also handles clicks on Object 90911 (that doesn't exist)
	case kObject90912:
		// Check current Cd
		if (target > 0 && target <=8) {
			// Original checks the cd number in data/cd.ini

			_app->setZoneAndEnableBag((ZoneId)target);
			_app->setZone((ZoneId)target, _app->getSaveManager()->getSetupType());
		}
		break;
	}
}

void ZoneSystemRing::onSound(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	if (id == 90001) {
		_app->objectPresentationHideAndRemove(kObject7, 0);
		_app->showCredits();
		_app->startMenu(false);
	}
}

void ZoneSystemRing::onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	switch (id) {
	default:
		break;

	case kObjectPreferencesSliderVolume:
	case kObjectPreferencesSliderDialog:
		switch (type) {
		default:
			break;

		case 1:
			_app->dragControlSetField45(2);

			if (id == kObjectPreferencesSliderVolume)
				_app->dragControlSetHotspot(Common::Rect(310, 140, 600, 180));
			else
				_app->dragControlSetHotspot(Common::Rect(310, 197, 600, 237));

			_offsetX = (_app->getDragControl()->getCurrentCoords().x - 314) / 5;
			if (_offsetX <= 54) {
				if (_offsetX < 0)
					_offsetX = 0;
			} else {
				_offsetX = 54;
			}

			_coordX = (int16)(5 * _offsetX + 314);
			_app->objectPresentationSetImageCoordinatesOnPuzzle(kObjectPreferencesSliderVolume, 0, 0, Common::Point(_coordX, (id == kObjectPreferencesSliderVolume) ? 155 : 212));
			break;

		case 2:
			_offsetX = (_offsetX0 + _coordX - 314) / 5;
			if (_offsetX <= 54) {
				if (_offsetX < 0)
					_offsetX = 0;

				_prefsVolume = _offsetX + 46;
			} else {
				_offsetX = 54;
				_prefsVolume = 100;
			}

			_coordX = (int16)(5 * _offsetX + 314);
			_app->objectPresentationSetImageCoordinatesOnPuzzle(kObjectPreferencesSliderVolume, 0, 0, Common::Point(_coordX, (id == kObjectPreferencesSliderVolume) ? 155 : 212));
			break;

		case 3:
			_offsetX0 = (int16)_app->dragControlGetOffsetX0();
			if (_app->dragControlXLower0())
				_offsetX0 = -(int16)_app->dragControlGetOffsetX0();

			_offsetX = (_offsetX0 + _coordX - 314) / 5;
			if (_offsetX <= 54) {
				if (_offsetX < 0)
					_offsetX = 0;
			} else {
				_offsetX = 54;
			}

			_app->objectPresentationSetImageCoordinatesOnPuzzle(kObjectPreferencesSliderVolume, 0, 0, Common::Point(_offsetX0 + _coordX, (id == kObjectPreferencesSliderVolume) ? 155 : 212));
			break;
		}
		break;
	}
}

void ZoneSystemRing::onUpdateBag(const Common::Point &point) {
	_app->objectPresentationHide(kObjectMenuNewGame, 0);
	_app->objectPresentationHide(kObjectMenuPreferences, 0);
	_app->objectPresentationHide(kObjectMenuLoad, 0);
	_app->objectPresentationHide(kObjectMenuSave, 0);
	_app->objectPresentationHide(kObjectMenuContinue, 0);
	_app->objectPresentationHide(kObjectMenuExit, 0);
	_app->objectPresentationHide(kObjectPreferencesCancel, 0);
	_app->objectPresentationHide(kObjectPreferencesOk, 0);
	_app->objectPresentationHide(kObjectPreferences3dSound, 2);
	_app->objectPresentationHide(kObjectLoadOk, 0);
	_app->objectPresentationHide(kObjectLoadCancel, 0);
	_app->objectPresentationHide(kObjectSaveOk, 0);
	_app->objectPresentationHide(kObjectSaveCancel, 0);
	_app->objectPresentationHide(kObjectMenuStatus, 0);
	_app->objectPresentationHide(kObjectStatusOk, 0);
	_app->objectPresentationHide(kObject2, 1);
	_app->objectPresentationHide(kObject2, 2);
	_app->objectPresentationHide(kObject4, 1);
	_app->objectPresentationHide(kObject4, 2);
	_app->objectPresentationHide(kObject3, 1);
	_app->objectPresentationHide(kObject90912, 0);

	_app->cursorSelect(kCursorMenuIdle);
}

void ZoneSystemRing::onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, const Common::Point &point) {
	switch (movabilityFrom) {
	default:
		break;

	case 2:
		switch (movabilityTo) {
		default:
			break;

		case 0:
			_app->objectPresentationHide(kObject2, 1);
			_app->objectPresentationShow(kObject2, 2);
			break;

		case 1:
			_app->objectPresentationHide(kObject2, 2);
			_app->objectPresentationShow(kObject2, 1);
			break;
		}
		break;

	case 3:
		_app->objectPresentationShow(kObject3, 1);
		break;

	case 4: {
		int32 val = (uint32)(movabilityTo) & 0x80000001;
		bool state = (val < 0 ? ((int32)((val - 1) | 0xFFFFFFFE)) == -1 : val == 0);

		if (state) {
			_app->objectPresentationHide(kObject4, 2);
			_app->objectPresentationShow(kObject4, 1);
		} else {
			_app->objectPresentationHide(kObject4, 1);
			_app->objectPresentationShow(kObject4, 2);
		}
		}
		break;

	case 90000:
		_app->objectPresentationHide(kObjectMenuNewGame, 0);
		_app->objectPresentationHide(kObjectMenuPreferences, 0);
		_app->objectPresentationHide(kObjectMenuLoad, 0);
		_app->objectPresentationHide(kObjectMenuSave, 0);
		_app->objectPresentationHide(kObjectMenuContinue, 0);
		_app->objectPresentationHide(kObjectMenuExit, 0);
		_app->objectPresentationShow(kObjectMenuNewGame, 0);
		break;

	case 90001:
		_app->objectPresentationHide(kObjectMenuNewGame, 0);
		_app->objectPresentationHide(kObjectMenuPreferences, 0);
		_app->objectPresentationHide(kObjectMenuLoad, 0);
		_app->objectPresentationHide(kObjectMenuSave, 0);
		_app->objectPresentationHide(kObjectMenuContinue, 0);
		_app->objectPresentationHide(kObjectMenuStatus, 0);
		_app->objectPresentationHide(kObjectMenuExit, 0);
		_app->objectPresentationShow(kObjectMenuPreferences, 0);
		break;

	case 90002:
		_app->objectPresentationHide(kObjectMenuNewGame, 0);
		_app->objectPresentationHide(kObjectMenuPreferences, 0);
		_app->objectPresentationHide(kObjectMenuLoad, 0);
		_app->objectPresentationHide(kObjectMenuSave, 0);
		_app->objectPresentationHide(kObjectMenuContinue, 0);
		_app->objectPresentationHide(kObjectMenuStatus, 0);
		_app->objectPresentationHide(kObjectMenuExit, 0);
		_app->objectPresentationShow(kObjectMenuLoad, 0);
		break;

	case 90003:
		_app->objectPresentationHide(kObjectMenuNewGame, 0);
		_app->objectPresentationHide(kObjectMenuPreferences, 0);
		_app->objectPresentationHide(kObjectMenuLoad, 0);
		_app->objectPresentationHide(kObjectMenuSave, 0);
		_app->objectPresentationHide(kObjectMenuContinue, 0);
		_app->objectPresentationHide(kObjectMenuStatus, 0);
		_app->objectPresentationHide(kObjectMenuExit, 0);
		_app->objectPresentationShow(kObjectMenuSave, 0);
		break;

	case 90005:
		_app->objectPresentationHide(kObjectMenuNewGame, 0);
		_app->objectPresentationHide(kObjectMenuPreferences, 0);
		_app->objectPresentationHide(kObjectMenuLoad, 0);
		_app->objectPresentationHide(kObjectMenuSave, 0);
		_app->objectPresentationHide(kObjectMenuContinue, 0);
		_app->objectPresentationHide(kObjectMenuStatus, 0);
		_app->objectPresentationHide(kObjectMenuExit, 0);
		_app->objectPresentationShow(kObjectMenuStatus, 0);
		break;

	case 90004:
		_app->objectPresentationHide(kObjectMenuNewGame, 0);
		_app->objectPresentationHide(kObjectMenuPreferences, 0);
		_app->objectPresentationHide(kObjectMenuLoad, 0);
		_app->objectPresentationHide(kObjectMenuSave, 0);
		_app->objectPresentationHide(kObjectMenuContinue, 0);
		_app->objectPresentationHide(kObjectMenuStatus, 0);
		_app->objectPresentationHide(kObjectMenuExit, 0);
		_app->objectPresentationShow(kObjectMenuContinue, 0);
		break;

	case 90006:
		_app->objectPresentationHide(kObjectMenuNewGame, 0);
		_app->objectPresentationHide(kObjectMenuPreferences, 0);
		_app->objectPresentationHide(kObjectMenuLoad, 0);
		_app->objectPresentationHide(kObjectMenuSave, 0);
		_app->objectPresentationHide(kObjectMenuContinue, 0);
		_app->objectPresentationHide(kObjectMenuStatus, 0);
		_app->objectPresentationHide(kObjectMenuExit, 0);
		_app->objectPresentationShow(kObjectMenuExit, 0);
		break;

	case 90101:
		_app->objectPresentationShow(kObjectPreferencesCancel, 0);
		_app->objectPresentationHide(kObjectPreferencesOk, 0);
		break;

	case 90102:
		_app->objectPresentationHide(kObjectPreferencesCancel, 0);
		_app->objectPresentationShow(kObjectPreferencesOk, 0);
		break;

	case 90104:
		_app->objectPresentationShow(kObjectPreferences3dSound, 2);
		break;

	case 90207:
		_app->objectPresentationHide(kObjectLoadOk, 0);
		_app->objectPresentationShow(kObjectLoadCancel, 0);
		break;

	case 90208:
		_app->objectPresentationShow(kObjectLoadOk, 0);
		_app->objectPresentationHide(kObjectLoadCancel, 0);
		break;

	case 90401:
		_app->objectPresentationShow(kObjectStatusOk, 0);
		break;

	case 90309:
		_app->objectPresentationShow(kObjectSaveOk, 0);
		_app->objectPresentationHide(kObjectSaveCancel, 0);
		break;

	case 90310:
		_app->objectPresentationHide(kObjectSaveOk, 0);
		_app->objectPresentationShow(kObjectSaveCancel, 0);
		break;

	case 90912:
		_app->objectPresentationShow(kObject90912, 0);
		break;
	}
}

void ZoneSystemRing::onVisualList(Id id, uint32 type, const Common::Point &point) {
	if (id == 1) {
		if (type > 0 && type <= 3) {
			_app->objectPresentationHide(kObjectLoadOk, 0);
			_app->objectPresentationHide(kObjectLoadCancel, 0);
		}
	}
}

#pragma region Helper functions

bool ZoneSystemRing::copySavedTimers(uint32 slot) const {
	Common::SaveFileManager *saveFileMan = g_system->getSavefileManager();
	if (!saveFileMan->copySavefile(SaveManager::getTimerFile("ALB", 0), SaveManager::getTimerFile("ALB", slot)))
		return false;

	if (!saveFileMan->copySavefile(SaveManager::getTimerFile("LOG", 0), SaveManager::getTimerFile("LOG", slot)))
		return false;

	if (!saveFileMan->copySavefile(SaveManager::getTimerFile("SIE", 0), SaveManager::getTimerFile("SIE", slot)))
		return false;

	if (!saveFileMan->copySavefile(SaveManager::getTimerFile("BRU", 0), SaveManager::getTimerFile("BRU", slot)))
		return false;

	return true;
}

bool ZoneSystemRing::removeSavedTimers(uint32 slot) const {
	Common::SaveFileManager *saveFileMan = g_system->getSavefileManager();
	if (!saveFileMan->removeSavefile(SaveManager::getTimerFile("ALB", slot)))
		return false;

	if (!saveFileMan->removeSavefile(SaveManager::getTimerFile("LOG", slot)))
		return false;

	if (!saveFileMan->removeSavefile(SaveManager::getTimerFile("SIE", slot)))
		return false;

	if (!saveFileMan->removeSavefile(SaveManager::getTimerFile("BRU", slot)))
		return false;


	return true;
}

#pragma endregion

} // End of namespace Ring
