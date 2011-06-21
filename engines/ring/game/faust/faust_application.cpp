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

#include "ring/game/faust/faust_application.h"

#include "ring/base/art.h"
#include "ring/base/bag.h"
#include "ring/base/cursor.h"
#include "ring/base/dialog.h"
#include "ring/base/preferences.h"
#include "ring/base/puzzle.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"
#include "ring/base/timer.h"

#include "ring/game/faust/faust_shared.h"
#include "ring/game/faust/faust_zonesystem.h"
#include "ring/game/faust/faust_zone2.h"
#include "ring/game/faust/faust_zone3.h"
#include "ring/game/faust/faust_zone4.h"
#include "ring/game/faust/faust_zone5.h"
#include "ring/game/faust/faust_zone6.h"
#include "ring/game/faust/faust_zone7.h"
#include "ring/game/faust/faust_zone8.h"
#include "ring/game/faust/faust_zone9.h"
#include "ring/game/faust/faust_zone10.h"
#include "ring/game/faust/faust_zone11.h"
#include "ring/game/faust/faust_zone12.h"
#include "ring/game/faust/faust_zone13.h"
#include "ring/game/faust/faust_zone14.h"
#include "ring/game/faust/faust_zone15.h"
#include "ring/game/faust/faust_zone16.h"
#include "ring/game/faust/faust_zone17.h"

#include "ring/graphics/image.h"
#include "ring/graphics/screen.h"

#include "ring/sound/soundhandler.h"

#include "ring/debug.h"
#include "ring/helpers.h"
#include "ring/ring.h"

using namespace FaustGame;

namespace Ring {

#pragma region Screen lists

// List of credits screens
static const struct {
	Common::String filename;
	uint32 ticksWait;
	LoadFrom loadFrom;
	ArchiveType archiveType;
} creditsScreens[7] = {
	{"cre1.bmp",    0, kLoadFromDisk, kArchiveArt},
	{"cre2.bmp",    0, kLoadFromDisk, kArchiveArt},
	{"cre3.bmp",    0, kLoadFromDisk, kArchiveArt},
	{"cre4.bmp",    0, kLoadFromDisk, kArchiveArt},
	{"cre5.bmp",    0, kLoadFromDisk, kArchiveArt},
	{"cre6.bmp",    0, kLoadFromDisk, kArchiveArt},
	{"cre7.bmp", 5000, kLoadFromDisk, kArchiveArt},
};

#pragma endregion

ApplicationFaust::ApplicationFaust(RingEngine *engine) : Application(engine) {
	_slot = -1;
	_zone = kZoneNone;

	// Event handlers
	_zoneSystem = new ZoneSystemFaust(this);
	_zone2      = new Zone2Faust(this);
	_zone3      = new Zone3Faust(this);
	_zone4      = new Zone4Faust(this);
	_zone5      = new Zone5Faust(this);
	_zone6      = new Zone6Faust(this);
	_zone7      = new Zone7Faust(this);
	_zone8      = new Zone8Faust(this);
	_zone9      = new Zone9Faust(this);
	_zone10     = new Zone10Faust(this);
	_zone11     = new Zone11Faust(this);
	_zone12     = new Zone12Faust(this);
	_zone13     = new Zone13Faust(this);
	_zone14     = new Zone14Faust(this);
	_zone15     = new Zone15Faust(this);
	_zone16     = new Zone16Faust(this);
	_zone17     = new Zone17Faust(this);
}

ApplicationFaust::~ApplicationFaust() {
	SAFE_DELETE(_zoneSystem);
	SAFE_DELETE(_zone2);
	SAFE_DELETE(_zone3);
	SAFE_DELETE(_zone4);
	SAFE_DELETE(_zone5);
	SAFE_DELETE(_zone6);
	SAFE_DELETE(_zone7);
	SAFE_DELETE(_zone8);
	SAFE_DELETE(_zone9);
	SAFE_DELETE(_zone10);
	SAFE_DELETE(_zone11);
	SAFE_DELETE(_zone12);
	SAFE_DELETE(_zone13);
	SAFE_DELETE(_zone14);
	SAFE_DELETE(_zone15);
	SAFE_DELETE(_zone16);
	SAFE_DELETE(_zone17);
}

#pragma region Game setup

void ApplicationFaust::initLanguages() {
	languageAdd(kLanguageEnglish, "ENG", "ENG", 1);
	languageAdd(kLanguageFrench,  "FRA", "FRA", 2);
	languageAdd(kLanguageGerman,  "GER", "GER", 3);
	languageAdd(kLanguageItalian, "ITA", "ITA", 1);
	languageAdd(kLanguageSpanish, "SPA", "SPA", 2);
	languageAdd(kLanguageDutch,   "HOL", "HOL", 3);
	languageAdd(kLanguageSwedish, "SWE", "SWE", 4);
}

void ApplicationFaust::initFont() {
	// Original asks for size 12, but the font only contains size 9
	fontAdd(kFontDefault, "arxrin3.fon", "HelveticaLight", 9, true, false, false, false, getCurrentLanguage());
}

void ApplicationFaust::setup() {
	// Add the list of episodes
	addEpisode(kZone2, "One",   2);
	addEpisode(kZone3, "Two",   2);
	addEpisode(kZone4, "Three", 4);
	addEpisode(kZone5, "Four",  3);
	addEpisode(kZone6, "Five",  3);
	addEpisode(kZone7, "Six",   1);
	addEpisode(kZone8, "Seven", 4);
	addEpisode(kZone9, "Eight", 5);

	// Add the list of zones
	addZone(kZone2,  "A01S01", "a01s01", kArchiveArt, kLoadFromCd);
	addZone(kZone3,  "A01S02", "a01s02", kArchiveArt, kLoadFromCd);
	addZone(kZone4,  "A01S03", "a01s03", kArchiveArt, kLoadFromCd);
	addZone(kZone5,  "A01S07", "a01s07", kArchiveArt, kLoadFromCd);
	addZone(kZone6,  "A03S01", "a03s01", kArchiveArt, kLoadFromCd);
	addZone(kZone7,  "A04S01", "a04s01", kArchiveArt, kLoadFromCd);
	addZone(kZone8,  "A05S03", "a05s03", kArchiveArt, kLoadFromCd);
	addZone(kZone9,  "A06S01", "a06s01", kArchiveArt, kLoadFromCd);
	addZone(kZone10, "A07S01", "a07s01", kArchiveArt, kLoadFromCd);
	addZone(kZone11, "A07S02", "a07s02", kArchiveArt, kLoadFromDisk);
	addZone(kZone12, "A08S01", "a08s01", kArchiveArt, kLoadFromCd);
	addZone(kZone13, "A09S01", "a09s01", kArchiveArt, kLoadFromCd);
	addZone(kZone14, "A11S01", "a11s01", kArchiveArt, kLoadFromCd);
	addZone(kZone15, "A12S01", "a12s01", kArchiveArt, kLoadFromCd);
	addZone(kZone16, "A13S01", "a13s01", kArchiveArt, kLoadFromCd);
	addZone(kZone17, "A14S01", "a14s01", kArchiveArt, kLoadFromCd);

	// Setup system zone
	setupZone(kZoneSY, kSetupTypeNone);
	setSpace(kZoneSY);

	// Setup cursors
	ArchiveType archiveType = (_configuration.artCURSOR ? kArchiveArt : kArchiveFile);

	cursorAdd(kCursorDefault,       "",                 kCursorTypeNormal,   1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorBusy,          "cur_busy",         kCursorTypeImage,    1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorIdle,          "cur_menuidle",     kCursorTypeNormal,   1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorMove,          "cur_muv",          kCursorTypeAnimated, 1, 18, 15.0f, 4, kLoadFromCursor, archiveType);
	cursorAdd(kCursorBack,          "cur_back",         kCursorTypeAnimated, 1, 15, 12.5f, 4, kLoadFromCursor, archiveType);
	cursorAdd(kCursorMenuIdle,      "sy_right",         kCursorTypeImage,    1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorMenuActive,    "cur_menuactive",   kCursorTypeImage,    1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorShoot,         "cur_shoot",        kCursorTypeImage,    1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorZoom,          "cur_zoom",         kCursorTypeAnimated, 1, 15, 12.5f, 4, kLoadFromCursor, archiveType);
	cursorAdd(kCursorTake,          "cur_take",         kCursorTypeAnimated, 1, 20, 15.0f, 4, kLoadFromCursor, archiveType);
	cursorAdd(kCursorAction,        "cur_action",       kCursorTypeImage,    1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorActionObject,  "cur_actionobject", kCursorTypeImage,    1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorDragDrop,      "cur_drag_dp",      kCursorTypeImage,    1, kLoadFromCursor, archiveType);

	// Adjust offsets
	cursorSetOffset(kCursorBusy,         Common::Point(15, 15));
	cursorSetOffset(kCursorIdle,         Common::Point(5, 3));
	cursorSetOffset(kCursorMove,         Common::Point(20, 20));
	cursorSetOffset(kCursorBack,         Common::Point(10, 20));
	cursorSetOffset(kCursorMenuIdle,     Common::Point(5, 3));
	cursorSetOffset(kCursorMenuActive,   Common::Point(5, 3));
	cursorSetOffset(kCursorZoom,         Common::Point(20, 20));
	cursorSetOffset(kCursorTake,         Common::Point(14, 14));
	cursorSetOffset(kCursorAction,       Common::Point(14, 14));
	cursorSetOffset(kCursorActionObject, Common::Point(14, 14));
	cursorSetOffset(kCursorDragDrop,     Common::Point(14, 14));

	// Setup subtitles
	subtitleSetColor(Color(255, 255, 255));
	subtitleSetBackgroundColor(Color(50, 50, 50));
}

void ApplicationFaust::initBag() {
	_bag->setOrigin(Common::Point(0, 0));
	_bag->sub_417D40(15, 26, 40, 61);
	_bag->setBackgroundOffset(Common::Point(0, 0));
	_bag->sub_417DD0(10);
	_bag->sub_417D80(0, 0, 30, 448);
	_bag->sub_417DA0(610, 0, 30, 448);
	_bag->sub_4192A0(6, 12);
	_bag->sub_4192C0(622, 12);
	_bag->sub_417DE0(99, 0);
	_bag->sub_419280(500);
	_bag->loadBackground("bagbgr.tga", "", "", "", "", "", "", "bag_hl.bmp", _archiveType);
	_bag->initHotspots();
}

#pragma endregion

#pragma region Startup

void ApplicationFaust::showStartupScreen() {
	setCurrentZone(kZoneSY);

	playMovie("1003");
	fadeOut(15, Color(0, 0, 0), 0);

	playMovie("1004");
	fadeOut(15, Color(0, 0, 0), 0);

	playMovie("1005");
	fadeOut(15, Color(0, 0, 0), 0);
}

void ApplicationFaust::startMenu(bool savegame) {
	// TODO 3 functions
	initMenuSave(savegame);
	initMenu2();
	initMenu3();

	if (_currentGameZone)
		return;

	if (savegame) {
		cursorSelect(kCursorBusy);
		_vm->setFlag(false);
		_bag->reset();

		byte slot = (byte)varGetByte(98001);
		if (!_saveManager->loadSave(slot, kLoadSaveWrite))
			error("[ApplicationRing::startMenu] Cannot save game in slot %d", slot);

		// Save a copy of the screen surface for savegame
		SAFE_DELETE(_thumbnail);

		_thumbnail = new Image();
		_thumbnail->create(24, 2, 640, 480);

		// Save a copy of the screen to our image
		_screenManager->copySurface(_thumbnail, 0, 0);
	}

	_currentGameZone = getCurrentEpisode();

	soundStopAll(4);
	timerStopAll();
	setupZone(kZoneSY, kSetupTypeNone);
	setSpace(kZoneSY);
	puzzleSetActive(kPuzzleGeneralMenu, true, true);
	puzzleSetMod(kPuzzleMenu, 1, 0);

	for (uint32 i = 1; i < 25; i++) {
		objectSetAccessibilityOff(i);
		objectPresentationHideAndRemove(i);
	}

	Bag *bag = getBag();
	if (bag && bag->isInitialized())
		bag->reset();

	cursorDelete();
}

void ApplicationFaust::showMenu(ZoneId zone, MenuAction menuAction) {
	if (zone != kZoneSY) {
		exitZone();
		initZones();
	}

	switch (zone) {
	default:
		break;

	case kZoneSY:
		switch (menuAction) {
		default:
			break;

		case kMenuAction0:
			loadAndStartLogin();
			break;

		case kMenuAction1:
			loadAndInitZone();
			break;

		case kMenuAction3:
			initZone();
			break;

		case kMenuAction10:
			exitZone();
			initZones();
			startLogin();
			break;
		}
		break;

	case kZone2:
		if (menuAction == kMenuAction1) {
			setCurrentZone(kZoneSY);
			_slot = -1;
			startMenu(false);
		}
		break;

	case kZone3:
		if (menuAction == kMenuAction0)
			setupZone(kZone2, kSetupType2);
		break;

	case kZone4:
		if (menuAction == kMenuAction0)
			setProgressAndShowMenu(kProgressState7);
		break;

	case kZone6:
		if (menuAction == kMenuAction0)
			setProgressAndShowMenu(kProgressState2);
		break;

	case kZone7:
		if (menuAction == kMenuAction0)
			setupZone(kZone2, kSetupType3);
		break;

	case kZone8:
		switch (menuAction) {
		default:
			break;

		case kMenuAction0:
			setCurrentZone(kZoneSY);
			varSetByte(98001, kProgressState8);
			setCurrentZone(kZoneSY);
			_slot = -1;
			startMenu(false);
			break;

		case kMenuAction2:
			setupZone(kZone8, kSetupType2);
			break;

		case kMenuAction3:
			varSetByte(98001, kProgressState8);
			setCurrentZone(kZoneSY);
			_slot = -1;
			startMenu(false);
			break;
		}
		break;

	case kZone9:
		if (menuAction == kMenuAction0)
			setupZone(kZone6, kSetupType3);
		break;

	case kZone10:
		switch (menuAction) {
		default:
			break;

		case kMenuAction2:
			setupZone(kZone6, kSetupType2);
			break;

		case kMenuAction4:
			setSpace(kZone14);
			playMovie("1001");
			fadeOut(15, Color(0, 0, 0), 0);

			setProgressAndShowMenu(kProgressState3);
			break;
		}
		break;

	case kZone11:
		switch (menuAction) {
		default:
			break;

		case kMenuAction1:
			_saveManager->loadSave("Rabbit", kLoadSaveRead);
			puzzleSetMod(kPuzzleMenu, 1, 0);
			break;

		case kMenuAction4:
			setProgressAndShowMenu(kProgressState4);
			break;
		}
		break;

	case kZone12:
		if (menuAction == kMenuAction0)
			setupZone(kZone4, kSetupType3);
		break;

	case kZone13:
		if (menuAction == kMenuAction0)
			setProgressAndShowMenu(kProgressState4);
		break;

	case kZone14:
		switch (menuAction) {
		default:
			break;

		case kMenuAction0:
			setupZone(kZone3, kSetupType3);
			break;

		case kMenuAction1:
		case kMenuAction2:
			playMovie("1002");
			break;
		}
		break;

	case kZone15:
		if (menuAction == kMenuAction0)
			setupZone(kZone5, kSetupType2);
		break;

	case kZone16:
		if (menuAction == kMenuAction0)
			setProgressAndShowMenu(kProgressState5);
		break;

	case kZone17:
		if (menuAction == kMenuAction0)
			setProgressAndShowMenu(kProgressState6);
		break;
	}
}

void ApplicationFaust::setProgressAndShowMenu(ProgressState progress) {
	if (getSaveManager()->getData()->progressState < progress) {
		getSaveManager()->getData()->progressState = progress;
		getSaveManager()->saveProgress(progress);
	}

	varSetByte(98001, progress);
	setCurrentZone(kZoneSY);
	_slot = -1;
	startMenu(false);
}

void ApplicationFaust::loadAndStartLogin() {
	// Get clicked object
	ObjectId objectId = visualListGetObjectIdClicked(4, kPuzzleGeneralMenu);
	if (!objectId)
		error("[ApplicationFaust::loadAndStartLogin] No object clicked");

	// Get icon name
	Common::String iconName = getObject(objectId)->getName();
	if (iconName.empty())
		error("[ApplicationFaust::loadAndStartLogin] Object does not have an icon name (%d)", objectId.id());

	exitZone();
	initZones();

	if (getSaveManager()->loadSave(iconName, kLoadSaveRead)) {
		varSetByte(98003, 0);
	} else {
		// Reload autosave
		Common::String filename = Common::String::format("Savegame_%d", varGetByte(98001));

		// Try reloading autosave
		if (!getSaveManager()->loadSave(filename, kLoadSaveRead)) {
			warning("[ApplicationFaust::loadAndStartLogin] Cannot reload autosave after failed load (%s)", filename.c_str());

			exitZone();
			initZones();
			startLogin();
		}

		messageFormat("CanNotLoadGame", filename);
		messageShowWarning(0);
	}
}

void ApplicationFaust::loadAndInitZone() {
	// Set current zone
	ZoneId zone = (ZoneId)varGetByte(98001);
	getSaveManager()->getData()->zone = zone;

	if (getSaveManager()->hasZoneSavegame()) {
		exitZone();
		initZones();

		Common::String filename = Common::String::format("Savegame_%d", zone);
		if (!getSaveManager()->loadSave(filename, kLoadSaveRead)) {
			warning("[ApplicationFaust::loadAndInitZone] Can not load game (%s)", filename.c_str());

			exitZone();
			initZones();
			startLogin();

			messageFormat("CanNotLoadGame", filename);
			messageShowWarning(0);

			return;
		}

		varSetByte(99000, -1);
		varSetByte(98001, zone);

		return;
	}

	// No savegame, setup the zone
	switch (zone) {
	default:
		break;

	case kZoneSY:
		exitZone();
		initZones();
		setupZone(kZone2, kSetupType1);
		break;

	case kZone2:
		exitZone();
		initZones();
		setupZone(kZone3, kSetupType1);
		break;

	case kZone3:
		exitZone();
		initZones();
		setupZone(kZone4, kSetupType1);
		break;

	case kZone4:
		exitZone();
		initZones();
		setupZone(kZone5, kSetupType1);
		break;

	case kZone5:
		exitZone();
		initZones();
		setupZone(kZone6, kSetupType1);
		break;

	case kZone6:
		exitZone();
		initZones();
		setupZone(kZone7, kSetupType1);
		break;

	case kZone7:
		exitZone();
		initZones();
		setupZone(kZone8, kSetupType1);
		break;
	}

	// Save zone
	varSetByte(99000, -1);
	varSetByte(98001, zone);
}

void ApplicationFaust::initZone() {
	bool resetZones = true;

	switch (varGetByte(98001)) {
	default:
		resetZones = false;
		break;

	case kZoneSY:
		setupZone(kZone2, kSetupType1);
		break;

	case kZone2:
		setupZone(kZone3, kSetupType1);
		break;

	case kZone3:
		setupZone(kZone4, kSetupType1);
		break;

	case kZone4:
		setupZone(kZone5, kSetupType1);
		break;

	case kZone5:
		setupZone(kZone6, kSetupType1);
		break;

	case kZone6:
		setupZone(kZone7, kSetupType1);
		break;

	case kZone7:
		setupZone(kZone8, kSetupType1);
		break;
	}

	if (resetZones) {
		exitZone();
		initZones();
	}

	varSetByte(98003, 0);
	varSetByte(98002, 1);
	varSetByte(98001, _zone);
}


void ApplicationFaust::initMenuSave(bool savegame)  {
	int8 slot = varGetByte(98001);

	varSetByte(99000, 0);
	varSetByte(99001, 1);
	varSetByte(99002, 1);
	varSetByte(99003, 0);
	varSetByte(99004, 0);
	varSetByte(99005, 0);
	varSetByte(99006, 0);
	varSetByte(99007, 0);
	varSetByte(99008, 0);
	varSetByte(99009, 0);

	if (slot <= 0) {
		if (slot == 0) {
			varSetByte(99005, 1);
			varSetByte(99006, 1);
			varSetByte(99007, 1);
			varSetByte(99008, 1);
			varSetByte(99009, 1);
		}
	} else {
		varSetByte(99005, 0);

		if (slot == getSaveManager()->getData()->progressState) {
			varSetByte(99001, 0);
		} else if (slot < getSaveManager()->getData()->progressState) {
			varSetByte(99000, 1);
			varSetByte(99001, 0);
		} else {
			varSetByte(99000, 1);
		}
	}

	if (savegame) {
		_slot = slot;
		varSetByte(98089, 0);
	}

	if (_slot == slot) {
		varSetByte(99000, 0);
		varSetByte(99002, 0);
	} else {
		varSetByte(99002, 1);
	}

	for (uint32 i = 1; i < (uint32)getSaveManager()->getData()->progressState; i++) {
		if (i < 9)
			objectPresentationShow(99600, i - 1);
	}

	if (varGetByte(98012) == getSaveManager()->getData()->progressState)
		if (varGetByte(98012) > 0)
			objectPresentationShow(99600, getSaveManager()->getData()->progressState - 1);

	if (getSaveManager()->getData()->progressState > 1 && getSaveManager()->getData()->progressState >= slot) {
		objectPresentationShow(99600, 7);
		objectSetAccessibilityOn(14, 2, 2);

		switch (slot) {
		default:
			break;

		case 1:
		case 3:
		case 5:
		case 7:
			if (getSaveManager()->getData()->progressState > slot)
				objectSetAccessibilityOn(99100);
			break;
		}
	}

	puzzleSetMod(kPuzzleMenu, 1, 0);
	objectSetAccessibilityOff(kObjectPipe);
	objectSetAccessibilityOff(kObject22);
	objectSetAccessibilityOff(kObject20);
	objectSetAccessibilityOff(kObject114503);
}

void ApplicationFaust::initMenu2()  {
	int8 slot = varGetByte(98001);

	objectPresentationShow(99012, 0);
	objectPresentationHide(99011);

	if (slot >= 8)
		objectPresentationShow(99011, 7);
	else
		objectPresentationShow(99011, (byte)slot);

	for (uint32 i = 99000; i < 99010; i++) {
		objectPresentationHide(i, 0);
		objectPresentationHide(i, 1);

		if (varGetByte(i) > 0)
			objectPresentationShow(i, 1);
	}

	objectPresentationShow(99010, 0);
	objectPresentationShow(99010, 1);
	objectPresentationShow(99010, 3);
}

void ApplicationFaust::initMenu3() {
	for (uint32 i = 99000; i < 99010; i++) {
		objectSetAccessibilityOn(i);

		if (varGetByte(i) > 0)
			objectSetAccessibilityOff(i);
	}
}

void ApplicationFaust::showCredits() {
	soundStopAll(1024);
	setCurrentZone(kZoneSY);
	soundSetVolume(52000, 100);
	soundPlay(52000, kSoundLoop);

	// Scroll credits
	for (uint i = 0; i < ARRAYSIZE(creditsScreens); i++) {
		if (scrollImage(creditsScreens[i].filename, creditsScreens[i].ticksWait, creditsScreens[i].loadFrom, creditsScreens[i].archiveType))
			break;
	}

	fadeOut(15, Color(0, 0, 0), 0);
	soundStop(52000, 1024);
}

void ApplicationFaust::loadPreferences() {
	error("[ApplicationFaust::loadPreferences] Not implemented");
}

#pragma endregion

#pragma region Drawing

void ApplicationFaust::draw() {
	// Update our screen
	_screenManager->updateScreen();

	// Update engine state
	switch (_state){
	default:
		break;

	case kStateNone:
		return;

	case kStateUpdateRotation:
		if (!_rotation)
			error("[ApplicationRing::draw] No active rotation!");

		if (!checkEscape()) {
			if (_rotation->getField28() != 0) {
				if (_rotation->getField28() == 3) {
					_rotation->destroyImage();
					_rotation->setField28(0);
				}
			} else {
				if (_rotation->hasImage())
					_rotation->setField28(3);
			}
		}

		if (_rotation->getField28() != 0) {
			if (_rotation->getField28() == 3) {
				_rotation->loadImage();
				_rotation->drawImage();
				_rotation->drawText();
			}
		} else {
			_rotation->alloc();
			_rotation->update();
			Common::Point point = _vm->getCoordinates();
			_rotation->setCoordinates(&point, Common::KEYCODE_INVALID);
			_rotation->draw();
			_rotation->drawText();
		}

		break;

	case kStateUpdatePuzzle:
		_puzzle->alloc();
		_puzzle->update();
		break;

	case kStateDrawBag:
		getBag()->draw();
		break;

	case kStateShowMenu:
		showMenu(getCurrentZone(), _menuAction);
		return;
	}

	// Update menu puzzle
	if (_puzzles.has(kPuzzleMenu)) {
		Puzzle *puzzleMenu = _puzzles.get(kPuzzleMenu);

		puzzleMenu->alloc();
		puzzleMenu->update();
	}

	// Draw bag
	Bag *bag = getBag();
	if (bag && bag->isInitialized())
		bag->draw();

	// Open bag
	if (_vm->isMouseButtonPressed())
		updateBag(_vm->getCoordinates());

	// Update bag, puzzle & rotation
	if (_vm->getFlag())
		update(_vm->getCoordinates());
	else
		_vm->setFlag(true);

	// Play dialogs
	if (_dialogHandler)
		_dialogHandler->play();

	// Update animated cursors
	if (_cursorHandler->getType() == kCursorTypeImage || _cursorHandler->getType() == kCursorTypeAnimated)
		_cursorHandler->draw();
}

#pragma endregion

#pragma region Visual

Visual *ApplicationFaust::createVisual(Id visualId, uint32 a3, uint32 a4, uint32 left, uint32 top, uint32 offsetY, uint32 height, uint32 progressMultiplier, uint32 progressColor) {
	error("[ApplicationFaust::createVisual] Engine state update not implemented!");
}

#pragma endregion

#pragma region Message

void ApplicationFaust::messageInsertCd(ZoneId zone) {
	setupZone(kZoneSY, kSetupTypeNone);
	setSpace(kZoneSY);

	for (uint32 i = 0; i <= 25; i++) {
		objectSetAccessibilityOff(i);
		objectPresentationHideAndRemove(i);
	}

	objectPresentationSetTextToPuzzle(kObject55, 1, 0, _messageType);
	objectPresentationSetTextCoordinatesToPuzzle(kObject55, 1, 0, Common::Point(225, 193));
	objectPresentationSetTextToPuzzle(kObject55, 1, 1, _message);
	objectPresentationSetTextCoordinatesToPuzzle(kObject55, 1, 1, Common::Point(225, 213));
	objectPresentationShow(kObject55, 0);
	objectSetAccessibilityOff(kObject55);
	objectSetAccessibilityOn(kObject55, zone, zone);
	objectSetAccessibilityOn(kObject55, 10, 10);
	puzzleSetActive(kPuzzleInsertCd);
}

#pragma endregion

#pragma region Zone setup

void ApplicationFaust::initZones() {
	debugC(kRingDebugLogic, "Init zone data");

	_loadFrom = kLoadFromDisk;

	drawZoneName(kZoneSY);
	_archiveType = getZoneArchiveType(kZoneSY);
	_zoneSystem->onInit();

	_loadFrom = kLoadFromCd;

	drawZoneName(kZone2);
	_archiveType = getZoneArchiveType(kZone2);
	_zone2->onInit();

	drawZoneName(kZone2);
	_archiveType = getZoneArchiveType(kZone3);
	_zone3->onInit();

	drawZoneName(kZone4);
	_archiveType = getZoneArchiveType(kZone4);
	_zone4->onInit();

	drawZoneName(kZone5);
	_archiveType = getZoneArchiveType(kZone5);
	_zone5->onInit();

	drawZoneName(kZone6);
	_archiveType = getZoneArchiveType(kZone6);
	_zone6->onInit();

	drawZoneName(kZone7);
	_archiveType = getZoneArchiveType(kZone7);
	_zone7->onInit();

	drawZoneName(kZone8);
	_archiveType = getZoneArchiveType(kZone8);
	_zone8->onInit();

	drawZoneName(kZone9);
	_archiveType = getZoneArchiveType(kZone9);
	_zone9->onInit();

	drawZoneName(kZone10);
	_archiveType = getZoneArchiveType(kZone10);
	_zone10->onInit();

	_loadFrom = kLoadFromDisk;

	drawZoneName(kZone11);
	_archiveType = getZoneArchiveType(kZone11);
	_zone11->onInit();

	_loadFrom = kLoadFromCd;

	drawZoneName(kZone12);
	_archiveType = getZoneArchiveType(kZone12);
	_zone12->onInit();

	drawZoneName(kZone13);
	_archiveType = getZoneArchiveType(kZone13);
	_zone13->onInit();

	drawZoneName(kZone14);
	_archiveType = getZoneArchiveType(kZone14);
	_zone14->onInit();

	drawZoneName(kZone15);
	_archiveType = getZoneArchiveType(kZone15);
	_zone15->onInit();

	drawZoneName(kZone16);
	_archiveType = getZoneArchiveType(kZone16);
	_zone16->onInit();

	drawZoneName(kZone17);
	_archiveType = getZoneArchiveType(kZone17);
	_zone17->onInit();

	_archiveType = getZoneArchiveType(getCurrentZone());

	setState(kStateNone);

	// Clear screen
	_screenManager->clear();
	g_system->updateScreen();
}

void ApplicationFaust::setupZone(ZoneId zone, SetupType type) {
	debugC(kRingDebugLogic, "Setting up zone %s", getZoneName(zone).c_str());

	bool hasLanguage = true;
	bool hasData = false;

	if (zone == kZoneSY) {
		hasData = true;
	} else {
		// The original checks for the correct CD,
		// we should instead check that the zone folder
		// has been copied properly
		warning("[ApplicationFaust::setupZone] Zone CD check not implemented");

		hasLanguage = hasLanguagePack();
	}

	reset();
	soundStopAll(8);

	if (_soundHandler)
		_soundHandler->reset();

	if (zone != kZoneSY)
		_artHandler->reset();

	if (hasData && hasLanguage) {
		setCurrentEpisode(zone);
		setZone(zone, type);
	} else {
		_saveManager->setSetupType(type);

		if (hasData)
			messageGet("TurnDVD");
		else {
			messageFormat("InsertCD", Common::String::format("%d", getEpisodeCd(zone)));
		}

		messageInsertCd(zone);
	}
}

#pragma endregion

#pragma region Event handling

void ApplicationFaust::onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed) {
	error("[ApplicationFaust::onMouseLeftButtonUp] Not implemented (evt: %d, CTRL pressed: %d)", evt.type, isControlPressed);
}

void ApplicationFaust::onMouseLeftButtonDown(const Common::Event &evt) {
	error("[ApplicationFaust::onMouseLeftButtonDown] Not implemented (evt: %d)", evt.type);
}

void ApplicationFaust::onMouseRightButtonUp(const Common::Event &evt) {
	error("[ApplicationFaust::onMouseRightButtonUp] Not implemented (evt: %d)", evt.type);
}

void ApplicationFaust::onKeyDown(Common::Event &evt) {
	error("[ApplicationFaust::onKeyDown] Not implemented (evt: %d)", evt.type);
}

void ApplicationFaust::onButtonDown(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	debugC(kRingDebugLogic, "onButtonDown (ObjectId: %d, coords: (%d, %d))", id.id(), point.x, point.y);

	if (puzzleRotationId == 1 && a4 == 1) {
		_zoneSystem->onButtonDown(id, target, puzzleRotationId, a4, point);
		return;
	}

	switch (getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_zoneSystem->onButtonDown(id, target, puzzleRotationId, a4, point);
		break;

	case kZone5:
		_zone5->onButtonDown(id, target, puzzleRotationId, a4, point);
		break;
	}
}

void ApplicationFaust::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	debugC(kRingDebugLogic, "onButtonUp (ObjectId: %d, target: %d, coords: (%d, %d))", id.id(), target, point.x, point.y);

	if (puzzleRotationId == 1 && a4 == 1) {
		_zoneSystem->onButtonUp(id, target, puzzleRotationId, a4, point);
		return;
	}

	switch (getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_zoneSystem->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZone2:
		_zone2->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZone3:
		_zone3->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZone4:
		_zone4->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZone5:
		_zone5->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZone6:
		_zone6->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZone7:
		_zone7->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZone8:
		_zone8->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZone9:
		_zone9->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZone10:
		_zone10->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZone11:
		_zone11->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZone12:
		_zone12->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZone13:
		_zone13->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZone14:
		_zone14->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZone15:
		_zone15->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZone16:
		_zone16->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZone17:
		_zone17->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;
	}
}

void ApplicationFaust::onSetup(ZoneId zone, SetupType type) {
	switch (zone) {
	default:
		break;

	case kZone2:
		_zone2->onSetup(type);
		break;

	case kZone3:
		_zone3->onSetup(type);
		break;

	case kZone4:
		_zone4->onSetup(type);
		break;

	case kZone5:
		_zone5->onSetup(type);
		break;

	case kZone6:
		_zone6->onSetup(type);
		break;

	case kZone7:
		_zone7->onSetup(type);
		break;

	case kZone8:
		_zone8->onSetup(type);
		break;
	}
}

void ApplicationFaust::onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	if (movabilityIndex == 1 || a4 == 1) {
		_zoneSystem->onUpdateBefore(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		return;
	}

	switch (getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_zoneSystem->onUpdateBefore(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone3:
		_zone3->onUpdateBefore(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone4:
		_zone4->onUpdateBefore(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone6:
		_zone6->onUpdateBefore(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone8:
		_zone8->onUpdateBefore(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone12:
		_zone12->onUpdateBefore(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone14:
		_zone14->onUpdateBefore(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone15:
		_zone15->onUpdateBefore(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone16:
		_zone16->onUpdateBefore(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;
	}
}

void ApplicationFaust::onTimer(TimerId timerId) {
	switch (getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_zoneSystem->onTimer(timerId);
		break;

	case kZone2:
		_zone2->onTimer(timerId);
		break;

	case kZone3:
		_zone3->onTimer(timerId);
		break;

	case kZone4:
		_zone4->onTimer(timerId);
		break;

	case kZone5:
		_zone5->onTimer(timerId);
		break;

	case kZone6:
		_zone6->onTimer(timerId);
		break;

	case kZone8:
		_zone8->onTimer(timerId);
		break;

	case kZone9:
		_zone9->onTimer(timerId);
		break;

	case kZone10:
		_zone10->onTimer(timerId);
		break;

	case kZone11:
		_zone11->onTimer(timerId);
		break;

	case kZone12:
		_zone12->onTimer(timerId);
		break;

	case kZone13:
		_zone13->onTimer(timerId);
		break;

	case kZone14:
		_zone14->onTimer(timerId);
		break;

	case kZone15:
		_zone15->onTimer(timerId);
		break;

	case kZone16:
		_zone16->onTimer(timerId);
		break;
	}

	if (getState() != kStateShowMenu)
		getTimerHandler()->incrementFiredCount(timerId);
}

void ApplicationFaust::onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	debugC(kRingDebugLogic, "onBag (object: %d)", id.id());

	if (puzzleRotationId == 1 && a4 == 1) {
		_zoneSystem->onBag(id, target, 1, 1, dragControl, type);
		return;
	}

	switch (getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_zoneSystem->onBag(id, target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone3:
		_zone3->onBag(id, target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone4:
		_zone4->onBag(id, target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone5:
		_zone5->onBag(id, target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone8:
		_zone8->onBag(id, target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone12:
		_zone12->onBag(id, target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone13:
		_zone13->onBag(id, target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone14:
		_zone14->onBag(id, target, puzzleRotationId, a4, dragControl, type);
		break;
	}
}

void ApplicationFaust::onUpdateBag(const Common::Point &point) {
	switch (getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_zoneSystem->onUpdateBag(point);
		break;

	case kZone3:
		_zone3->onUpdateBag(point);
		break;

	case kZone4:
		_zone4->onUpdateBag(point);
		break;

	case kZone6:
		_zone6->onUpdateBag(point);
		break;

	case kZone12:
		_zone12->onUpdateBag(point);
		break;

	case kZone14:
		_zone14->onUpdateBag(point);
		break;

	case kZone15:
		_zone15->onUpdateBag(point);
		break;

	case kZone16:
		_zone16->onUpdateBag(point);
		break;
	}
}

void ApplicationFaust::onBagClickedObject(ObjectId id) {
	switch (getCurrentZone()) {
	default:
		break;

	case kZone2:
		_zone2->onBagClickedObject(id);
		break;

	case kZone3:
		_zone3->onBagClickedObject(id);
		break;

	case kZone4:
		_zone4->onBagClickedObject(id);
		break;

	case kZone8:
		_zone8->onBagClickedObject(id);
		break;

	case kZone14:
		_zone14->onBagClickedObject(id);
		break;

	case kZone15:
		_zone15->onBagClickedObject(id);
		break;
	}
}

void ApplicationFaust::onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {
	if (movabilityFrom == 1)
		return;

	switch (getCurrentZone()) {
	default:
	case kZoneSY:
		break;

	case kZone2:
		_zone2->onBeforeRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZone4:
		_zone4->onBeforeRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZone5:
		_zone5->onBeforeRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZone8:
		_zone8->onBeforeRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZone14:
		_zone14->onBeforeRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZone15:
		_zone15->onBeforeRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;
	}
}

void ApplicationFaust::onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {
	if (movabilityTo == 1)
		return;

	switch (getCurrentZone()) {
	default:
	case kZoneSY:
		break;

	case kZone2:
		_zone2->onAfterRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZone3:
		_zone3->onAfterRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZone4:
		_zone4->onAfterRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZone6:
		_zone6->onAfterRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZone8:
		_zone8->onAfterRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZone10:
		_zone10->onAfterRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZone12:
		_zone12->onAfterRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZone13:
		_zone13->onAfterRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZone14:
		_zone14->onAfterRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZone15:
		_zone15->onAfterRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZone16:
		_zone16->onAfterRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZone17:
		_zone17->onAfterRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;
	}
}

void ApplicationFaust::onSound(Id id, SoundType type, uint32 a3) {
	debugC(kRingDebugLogic, "onSound (id: %d, type: %d)", id, type);

	bool process = (a3 & 0x1000) != 0;
	a3 &= 239;

	switch (getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_zoneSystem->onSound(id, type, a3, process);
		break;

	case kZone3:
		_zone3->onSound(id, type, a3, process);
		break;

	case kZone4:
		_zone4->onSound(id, type, a3, process);
		break;

	case kZone5:
		_zone5->onSound(id, type, a3, process);
		break;

	case kZone8:
		_zone8->onSound(id, type, a3, process);
		break;

	case kZone12:
		_zone12->onSound(id, type, a3, process);
		break;

	case kZone14:
		_zone14->onSound(id, type, a3, process);
		break;

	case kZone15:
		_zone15->onSound(id, type, a3, process);
		break;

	case kZone17:
		_zone17->onSound(id, type, a3, process);
		break;
	}
}

void ApplicationFaust::onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	switch (getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_zoneSystem->onAnimationNextFrame(animationId, name, frame, frameCount);
		break;

	case kZone3:
		_zone3->onAnimationNextFrame(animationId, name, frame, frameCount);
		break;

	case kZone4:
		_zone4->onAnimationNextFrame(animationId, name, frame, frameCount);
		break;

	case kZone5:
		_zone5->onAnimationNextFrame(animationId, name, frame, frameCount);
		break;

	case kZone6:
		_zone6->onAnimationNextFrame(animationId, name, frame, frameCount);
		break;

	case kZone7:
		_zone7->onAnimationNextFrame(animationId, name, frame, frameCount);
		break;

	case kZone8:
		_zone8->onAnimationNextFrame(animationId, name, frame, frameCount);
		break;

	case kZone9:
		_zone9->onAnimationNextFrame(animationId, name, frame, frameCount);
		break;

	case kZone10:
		_zone10->onAnimationNextFrame(animationId, name, frame, frameCount);
		break;

	case kZone11:
		_zone11->onAnimationNextFrame(animationId, name, frame, frameCount);
		break;

	case kZone12:
		_zone12->onAnimationNextFrame(animationId, name, frame, frameCount);
		break;

	case kZone13:
		_zone13->onAnimationNextFrame(animationId, name, frame, frameCount);
		break;

	case kZone14:
		_zone14->onAnimationNextFrame(animationId, name, frame, frameCount);
		break;

	case kZone15:
		_zone15->onAnimationNextFrame(animationId, name, frame, frameCount);
		break;
	}
}

void ApplicationFaust::onVisualList(Id id, uint32 type, const Common::Point &point) {
	if (getCurrentZone() == kZoneSY)
		_zoneSystem->onVisualList(id, type, point);
}

#pragma endregion

#pragma region Helpers

void ApplicationFaust::setupRotation(uint32 type) {
	switch (type) {
	default:
		break;

	case 2:
		soundStop(17019, 1024);
		setSpace(kZone10);
		rotationSetAlp(71009, 64.0f);
		rotationSetBet(71009, 0);
		rotationSetRan(71009, 87.0f);
		rotationSetActive(71009);
		break;

	case 3:
		setSpace(kZone9);
		soundPlay(61306, kSoundLoop);
		rotationSetAlp(61200, 0.0f);
		rotationSetBet(61200, 0);
		rotationSetRan(61200, 87.0f);
		rotationSetActive(61200);
		break;

	case 4:
		setSpace(kZone10);
		soundStop(61306, 1024);
		rotationSetAlp(71010, 270.0f);
		rotationSetBet(71010, 0);
		rotationSetRan(71010, 87.0f);
		rotationSetActive(71010);
		objectPresentationHide(kObject5);
		break;
	}
}

void ApplicationFaust::sub_45FF30() {
	if (soundIsPlaying(112016)) {
		objectPresentationHide(kObject12, 1);
		objectSetAccessibilityOff(kObject22);
		soundStop(112016, 1024);
		varSetByte(114100, 0);
	}
}

void ApplicationFaust::sub_468290(uint32 a1) {
	error("[ApplicationFaust::sub_468290] Not implemented");
}

void ApplicationFaust::sub_4A2CF0() {
	error("[ApplicationFaust::sub_4A2CF0] Not implemented");
}

#pragma endregion

} // End of namespace Ring
