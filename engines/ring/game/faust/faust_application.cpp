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

#include "ring/game/faust/faust_animation.h"
#include "ring/game/faust/faust_bag.h"
#include "ring/game/faust/faust_button.h"
#include "ring/game/faust/faust_init.h"
#include "ring/game/faust/faust_input.h"
#include "ring/game/faust/faust_ride.h"
#include "ring/game/faust/faust_setup.h"
#include "ring/game/faust/faust_shared.h"
#include "ring/game/faust/faust_sound.h"
#include "ring/game/faust/faust_timer.h"
#include "ring/game/faust/faust_visual.h"
#include "ring/game/faust/faust_zone.h"

#include "ring/graphics/image.h"
#include "ring/graphics/screen.h"

#include "ring/sound/soundhandler.h"

#include "ring/debug.h"
#include "ring/helpers.h"
#include "ring/ring.h"

using namespace FaustGame;

namespace Ring {

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


ApplicationFaust::ApplicationFaust(RingEngine *engine) : Application(engine) {
	_slot = -1;
	_zone = kZoneNone;

	_eventAnimation = new EventAnimationFaust(this);
	_eventBag       = new EventBagFaust(this);
	_eventButton    = new EventButtonFaust(this);
	_eventInit      = new EventInitFaust(this);
	_eventInput     = new EventInputFaust(this);
	_eventRide      = new EventRideFaust(this);
	_eventSetup     = new EventSetupFaust(this);
	_eventSound     = new EventSoundFaust(this);
	_eventTimer     = new EventTimerFaust(this);
	_eventVisual    = new EventVisualFaust(this);
	_eventZone      = new EventZoneFaust(this);
}

ApplicationFaust::~ApplicationFaust() {
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
	onInitZone(kZoneSY);

	_loadFrom = kLoadFromCd;

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

	_loadFrom = kLoadFromCd;

	drawZoneName(kZone12);
	_archiveType = getZoneArchiveType(kZone12);
	onInitZone(kZone12);

	drawZoneName(kZone13);
	_archiveType = getZoneArchiveType(kZone13);
	onInitZone(kZone13);

	drawZoneName(kZone14);
	_archiveType = getZoneArchiveType(kZone14);
	onInitZone(kZone14);

	drawZoneName(kZone15);
	_archiveType = getZoneArchiveType(kZone15);
	onInitZone(kZone15);

	drawZoneName(kZone16);
	_archiveType = getZoneArchiveType(kZone16);
	onInitZone(kZone16);

	drawZoneName(kZone17);
	_archiveType = getZoneArchiveType(kZone17);
	onInitZone(kZone17);

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
	_eventInput->onMouseLeftButtonUp(evt, isControlPressed);
}

void ApplicationFaust::onMouseLeftButtonDown(const Common::Event &evt) {
	_eventInput->onMouseLeftButtonDown(evt);
}

void ApplicationFaust::onMouseRightButtonUp(const Common::Event &evt) {
	_eventInput->onMouseRightButtonUp(evt);
}

void ApplicationFaust::onKeyDown(Common::Event &evt) {
	_eventInput->onKeyDown(evt);
}

void ApplicationFaust::onButtonDown(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	debugC(kRingDebugLogic, "onButtonDown (ObjectId: %d, coords: (%d, %d))", id.id(), point.x, point.y);

	if (puzzleRotationId == 1 && a4 == 1) {
		_eventButton->onButtonDownZoneSY(id, (uint32)target, puzzleRotationId, a4, point);
		return;
	}

	switch (getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_eventButton->onButtonDownZoneSY(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone5:
		_eventButton->onButtonDownZone5(id, (uint32)target, puzzleRotationId, a4, point);
		break;
	}
}

void ApplicationFaust::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	debugC(kRingDebugLogic, "onButtonUp (ObjectId: %d, target: %d, coords: (%d, %d))", id.id(), target, point.x, point.y);

	if (puzzleRotationId == 1 && a4 == 1) {
		_eventButton->onButtonUpZoneSY(id, (uint32)target, puzzleRotationId, a4, point);
		return;
	}

	switch (getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_eventButton->onButtonUpZoneSY(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone2:
		_eventButton->onButtonUpZone2(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone3:
		_eventButton->onButtonUpZone3(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone4:
		_eventButton->onButtonUpZone4(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone5:
		_eventButton->onButtonUpZone5(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone6:
		_eventButton->onButtonUpZone6(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone7:
		_eventButton->onButtonUpZone7(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone8:
		_eventButton->onButtonUpZone8(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone9:
		_eventButton->onButtonUpZone9(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone10:
		_eventButton->onButtonUpZone10(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone11:
		_eventButton->onButtonUpZone11(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone12:
		_eventButton->onButtonUpZone12(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone13:
		_eventButton->onButtonUpZone13(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone14:
		_eventButton->onButtonUpZone14(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone15:
		_eventButton->onButtonUpZone15(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone16:
		_eventButton->onButtonUpZone16(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZone17:
		_eventButton->onButtonUpZone17(id, (uint32)target, puzzleRotationId, a4, point);
		break;
	}
}

void ApplicationFaust::onSetup(ZoneId zone, SetupType type) {
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

	case kZone5:
		_eventSetup->onSetupZone5(type);
		break;

	case kZone6:
		_eventSetup->onSetupZone6(type);
		break;

	case kZone7:
		_eventSetup->onSetupZone7(type);
		break;

	case kZone8:
		_eventSetup->onSetupZone8(type);
		break;
	}
}

void ApplicationFaust::onInitZone(ZoneId zone) {
	switch (zone) {
	default:
		break;

	case kZoneSY:
		_eventInit->initZoneSY();
		break;

	case kZone2:
		_eventInit->initZone2();
		break;

	case kZone3:
		_eventInit->initZone3();
		break;

	case kZone4:
		_eventInit->initZone4();
		break;

	case kZone5:
		_eventInit->initZone5();
		break;

	case kZone6:
		_eventInit->initZone6();
		break;

	case kZone7:
		_eventInit->initZone7();
		break;

	case kZone8:
		_eventInit->initZone8();
		break;

	case kZone9:
		_eventInit->initZone9();
		break;

	case kZone10:
		_eventInit->initZone10();
		break;

	case kZone11:
		_eventInit->initZone11();
		break;

	case kZone12:
		_eventInit->initZone12();
		break;

	case kZone13:
		_eventInit->initZone13();
		break;

	case kZone14:
		_eventInit->initZone14();
		break;

	case kZone15:
		_eventInit->initZone15();
		break;

	case kZone16:
		_eventInit->initZone16();
		break;

	case kZone17:
		_eventInit->initZone17();
		break;
	}
}

void ApplicationFaust::onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
	if (movabilityIndex == 1 || a4 == 1) {
		_eventZone->onUpdateBeforeZoneSY(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		return;
	}

	switch (getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_eventZone->onUpdateBeforeZoneSY(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone3:
		_eventZone->onUpdateBeforeZone3(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone4:
		_eventZone->onUpdateBeforeZone4(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone6:
		_eventZone->onUpdateBeforeZone6(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone8:
		_eventZone->onUpdateBeforeZone8(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone12:
		_eventZone->onUpdateBeforeZone12(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone14:
		_eventZone->onUpdateBeforeZone14(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone15:
		_eventZone->onUpdateBeforeZone15(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZone16:
		_eventZone->onUpdateBeforeZone16(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;
	}
}

void ApplicationFaust::onTimer(TimerId timerId) {
	switch (getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_eventTimer->onTimerZoneSY(timerId);
		break;

	case kZone2:
		_eventTimer->onTimerZone2(timerId);
		break;

	case kZone3:
		_eventTimer->onTimerZone3(timerId);
		break;

	case kZone4:
		_eventTimer->onTimerZone4(timerId);
		break;

	case kZone5:
		_eventTimer->onTimerZone5(timerId);
		break;

	case kZone6:
		_eventTimer->onTimerZone6(timerId);
		break;

	case kZone8:
		_eventTimer->onTimerZone8(timerId);
		break;

	case kZone9:
		_eventTimer->onTimerZone9(timerId);
		break;

	case kZone10:
		_eventTimer->onTimerZone10(timerId);
		break;

	case kZone11:
		_eventTimer->onTimerZone11(timerId);
		break;

	case kZone12:
		_eventTimer->onTimerZone12(timerId);
		break;

	case kZone13:
		_eventTimer->onTimerZone13(timerId);
		break;

	case kZone14:
		_eventTimer->onTimerZone14(timerId);
		break;

	case kZone15:
		_eventTimer->onTimerZone15(timerId);
		break;

	case kZone16:
		_eventTimer->onTimerZone16(timerId);
		break;
	}

	if (getState() != kStateShowMenu)
		getTimerHandler()->incrementFiredCount(timerId);
}

void ApplicationFaust::onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	debugC(kRingDebugLogic, "onBag (object: %d)", id.id());

	if (puzzleRotationId == 1 && a4 == 1) {
		_eventBag->onBagZoneSY(id, (uint32)target, 1, 1, dragControl, type);
		return;
	}

	switch (getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_eventBag->onBagZoneSY(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone3:
		_eventBag->onBagZone3(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone4:
		_eventBag->onBagZone4(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone5:
		_eventBag->onBagZone5(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone8:
		_eventBag->onBagZone8(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone12:
		_eventBag->onBagZone12(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone13:
		_eventBag->onBagZone13(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZone14:
		_eventBag->onBagZone14(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;
	}
}

void ApplicationFaust::onUpdateBag(const Common::Point &point) {
	switch (getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_eventBag->onUpdateBagZoneSY(point);
		break;

	case kZone3:
		_eventBag->onUpdateBagZone3(point);
		break;

	case kZone4:
		_eventBag->onUpdateBagZone4(point);
		break;

	case kZone6:
		_eventBag->onUpdateBagZone6(point);
		break;

	case kZone12:
		_eventBag->onUpdateBagZone12(point);
		break;

	case kZone14:
		_eventBag->onUpdateBagZone14(point);
		break;

	case kZone15:
		_eventBag->onUpdateBagZone15(point);
		break;

	case kZone16:
		_eventBag->onUpdateBagZone16(point);
		break;
	}
}

void ApplicationFaust::onBagClickedObject(ObjectId id) {
	switch (getCurrentZone()) {
	default:
		break;

	case kZone2:
		_eventBag->onBagClickedObjectZone2(id);
		break;

	case kZone3:
		_eventBag->onBagClickedObjectZone3(id);
		break;

	case kZone4:
		_eventBag->onBagClickedObjectZone4(id);
		break;

	case kZone8:
		_eventBag->onBagClickedObjectZone8(id);
		break;

	case kZone14:
		_eventBag->onBagClickedObjectZone14(id);
		break;

	case kZone15:
		_eventBag->onBagClickedObjectZone15(id);
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
		_eventRide->onBeforeRideZone2(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone4:
		_eventRide->onBeforeRideZone4(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone5:
		_eventRide->onBeforeRideZone5(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone8:
		_eventRide->onBeforeRideZone8(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone14:
		_eventRide->onBeforeRideZone14(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone15:
		_eventRide->onBeforeRideZone15(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
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
		_eventRide->onAfterRideZone2(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone3:
		_eventRide->onAfterRideZone3(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone4:
		_eventRide->onAfterRideZone4(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone6:
		_eventRide->onAfterRideZone6(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone8:
		_eventRide->onAfterRideZone8(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone10:
		_eventRide->onAfterRideZone10(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone12:
		_eventRide->onAfterRideZone12(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone13:
		_eventRide->onAfterRideZone13(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone14:
		_eventRide->onAfterRideZone14(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone15:
		_eventRide->onAfterRideZone15(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone16:
		_eventRide->onAfterRideZone16(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZone17:
		_eventRide->onAfterRideZone17(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
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
		_eventSound->onSoundZoneSY(id, type, a3, process);
		break;

	case kZone3:
		_eventSound->onSoundZone3(id, type, a3, process);
		break;

	case kZone4:
		_eventSound->onSoundZone4(id, type, a3, process);
		break;

	case kZone5:
		_eventSound->onSoundZone5(id, type, a3, process);
		break;

	case kZone8:
		_eventSound->onSoundZone8(id, type, a3, process);
		break;

	case kZone12:
		_eventSound->onSoundZone12(id, type, a3, process);
		break;

	case kZone14:
		_eventSound->onSoundZone14(id, type, a3, process);
		break;

	case kZone15:
		_eventSound->onSoundZone15(id, type, a3, process);
		break;

	case kZone17:
		_eventSound->onSoundZone17(id, type, a3, process);
		break;
	}
}

void ApplicationFaust::onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	switch (getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_eventAnimation->onAnimationNextFrameZoneSY(animationId, name, frame, frameCount);
		break;

	case kZone3:
		_eventAnimation->onAnimationNextFrameZone3(animationId, name, frame, frameCount);
		break;

	case kZone4:
		_eventAnimation->onAnimationNextFrameZone4(animationId, name, frame, frameCount);
		break;

	case kZone5:
		_eventAnimation->onAnimationNextFrameZone5(animationId, name, frame, frameCount);
		break;

	case kZone6:
		_eventAnimation->onAnimationNextFrameZone6(animationId, name, frame, frameCount);
		break;

	case kZone7:
		_eventAnimation->onAnimationNextFrameZone7(animationId, name, frame, frameCount);
		break;

	case kZone8:
		_eventAnimation->onAnimationNextFrameZone8(animationId, name, frame, frameCount);
		break;

	case kZone9:
		_eventAnimation->onAnimationNextFrameZone9(animationId, name, frame, frameCount);
		break;

	case kZone10:
		_eventAnimation->onAnimationNextFrameZone10(animationId, name, frame, frameCount);
		break;

	case kZone11:
		_eventAnimation->onAnimationNextFrameZone11(animationId, name, frame, frameCount);
		break;

	case kZone12:
		_eventAnimation->onAnimationNextFrameZone12(animationId, name, frame, frameCount);
		break;

	case kZone13:
		_eventAnimation->onAnimationNextFrameZone13(animationId, name, frame, frameCount);
		break;

	case kZone14:
		_eventAnimation->onAnimationNextFrameZone14(animationId, name, frame, frameCount);
		break;

	case kZone15:
		_eventAnimation->onAnimationNextFrameZone15(animationId, name, frame, frameCount);
		break;
	}
}

void ApplicationFaust::onVisualList(Id id, uint32 type, const Common::Point &point) {
	if (getCurrentZone() == kZoneSY)
		_eventVisual->onVisualListZoneSY(id, type, point);
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
