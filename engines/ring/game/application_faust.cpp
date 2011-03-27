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

#include "ring/game/application_faust.h"

#include "ring/base/art.h"
#include "ring/base/bag.h"
#include "ring/base/preferences.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"
#include "ring/base/sound.h"

#include "ring/game/event_faust.h"

#include "ring/graphics/image.h"
#include "ring/graphics/screen.h"

#include "ring/ring.h"
#include "ring/debug.h"
#include "ring/shared_faust.h"

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
	_eventHandler = new EventHandlerFaust(this);

	_progressState = kProgressStateNone;
	_slot = -1;
}

ApplicationFaust::~ApplicationFaust() {
}

#pragma region Game setup

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
	if (_progressState < progress) {
		_progressState = progress;
		getSaveManager()->saveProgress(progress);
	}

	varSetByte(98001, progress);
	setCurrentZone(kZoneSY);
	_slot = -1;
	startMenu(false);
}

void ApplicationFaust::loadAndStartLogin() {
	error("[ApplicationFaust::loadAndStartLogin] Not implemented");
}

void ApplicationFaust::loadAndInitZone() {
	error("[ApplicationFaust::loadAndInitZone] Not implemented");
}

void ApplicationFaust::initZone() {
	error("[ApplicationFaust::initZone] Not implemented");
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

		if (slot == _progressState) {
			varSetByte(99001, 0);
		} else if (slot < _progressState) {
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

	for (uint32 i = 1; i < (uint32)_progressState; i++) {
		if (i < 9)
			objectPresentationShow(99600, i - 1);
	}

	if (varGetByte(98012) == _progressState)
		if (varGetByte(98012) > 0)
			objectPresentationShow(99600, _progressState - 1);

	if (_progressState > 1 && _progressState >= slot) {
		objectPresentationShow(99600, 7);
		objectSetAccessibilityOn(14, 2, 2);

		switch (slot) {
		default:
			break;

		case 1:
		case 3:
		case 5:
		case 7:
			if (_progressState > slot)
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
	error("[ApplicationFaust::draw] Engine state update not implemented!");
}

#pragma endregion

#pragma region Visual

Visual *ApplicationFaust::createVisual(Id visualId, uint32 a3, uint32 a4, uint32 left, uint32 top, uint32 offsetY, uint32 height, uint32 progressMultiplier, uint32 progressColor) {
	error("[ApplicationFaust::createVisual] Engine state update not implemented!");
}

#pragma endregion

#pragma region Zone initialization

void ApplicationFaust::initZones() {
	debugC(kRingDebugLogic, "Init zone data");

	_loadFrom = kLoadFromDisk;

	drawZoneName(kZoneSY);
	_archiveType = getZoneArchiveType(kZoneSY);
	initZoneSY();

	_loadFrom = kLoadFromCd;

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

	_loadFrom = kLoadFromCd;

	drawZoneName(kZone12);
	_archiveType = getZoneArchiveType(kZone12);
	initZone12();

	drawZoneName(kZone13);
	_archiveType = getZoneArchiveType(kZone13);
	initZone13();

	drawZoneName(kZone14);
	_archiveType = getZoneArchiveType(kZone14);
	initZone14();

	drawZoneName(kZone15);
	_archiveType = getZoneArchiveType(kZone15);
	initZone15();

	drawZoneName(kZone16);
	_archiveType = getZoneArchiveType(kZone16);
	initZone16();

	drawZoneName(kZone17);
	_archiveType = getZoneArchiveType(kZone17);
	initZone17();

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

bool ApplicationFaust::hasLanguagePack() {
	error("[ApplicationFaust::hasLanguagePack] Not implemented");
}

void ApplicationFaust::setZone(ZoneId zone, SetupType type) {

	// Finish loading savegame if needed
	if (type == kSetupTypeLoading) {
		SaveManager::SavegameData *data = getSaveManager()->getData();

		setSpace(data->zone);

		if (data->hasCurrentPuzzle)
			puzzleSetActive(data->puzzleId, false, true);

		if (data->hasCurrentRotation) {
			rotationSetActive(data->rotationId, false, true);

			getCurrentRotation()->setFreOnOff(data->rotationFre);
		}

		_loadFrom = data->loadFrom;
		_isRotationCompressed = data->isRotationCompressed;
		_archiveType = data->archiveType;

		getSaveManager()->loadSaveSounds();

		if (getSaveManager()->isSaving()) {
			_soundManager->playSounds();
		} else {
			_preferenceHandler->load();
			_soundManager->playSounds();
		}
	}

	// Setup zone
	_eventHandler->onSetup(zone, type);
}

#pragma endregion

#pragma region Zone initialization

void ApplicationFaust::initZoneSY() {
	setCurrentZone(kZoneSY);
	objectAdd(kObjectHomunculus, "Homunculus", "a08_Humunculus", 1);
	objectSetActiveCursor(kObjectHomunculus, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	objectSetPassiveCursor(kObjectHomunculus, Common::Point(11, 11), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	puzzleAdd(kPuzzleMenu);
	objectAdd(kObject1, "", "", 1);
	objectAddPuzzleAccessibility(kObject1, kPuzzleMenu, Common::Rect(0, 0, 640, 480), false, kCursorDefault, 0);
	objectAddPresentation(kObject1);
	objectPresentationAddTextToPuzzle(kObject1, 0, kPuzzleMenu, "", Common::Point(1, 16),  kFontDefault, Color(0, 0, 0), Color(255, 255, 255));
	objectPresentationAddTextToPuzzle(kObject1, 0, kPuzzleMenu, "", Common::Point(1, 32),  kFontDefault, Color(0, 0, 0), Color(255, 255, 255));
	objectPresentationAddTextToPuzzle(kObject1, 0, kPuzzleMenu, "", Common::Point(1, 48),  kFontDefault, Color(0, 0, 0), Color(255, 255, 255));
	objectPresentationAddTextToPuzzle(kObject1, 0, kPuzzleMenu, "", Common::Point(1, 64),  kFontDefault, Color(0, 0, 0), Color(255, 255, 255));
	objectPresentationAddTextToPuzzle(kObject1, 0, kPuzzleMenu, "", Common::Point(1, 80),  kFontDefault, Color(0, 0, 0), Color(255, 255, 255));
	objectPresentationAddTextToPuzzle(kObject1, 0, kPuzzleMenu, "", Common::Point(1, 96),  kFontDefault, Color(0, 0, 0), Color(255, 255, 255));
	objectPresentationAddTextToPuzzle(kObject1, 0, kPuzzleMenu, "", Common::Point(1, 112), kFontDefault, Color(0, 0, 0), Color(255, 255, 255));
	objectPresentationAddTextToPuzzle(kObject1, 0, kPuzzleMenu, "", Common::Point(1, 128), kFontDefault, Color(0, 0, 0), Color(255, 255, 255));
	objectPresentationAddTextToPuzzle(kObject1, 0, kPuzzleMenu, "", Common::Point(1, 144), kFontDefault, Color(0, 0, 0), Color(255, 255, 255));
	objectPresentationShow(kObject1, 0);
	puzzleAdd(kPuzzleGeneralMenu);
	puzzleAddBackgroundImage(kPuzzleGeneralMenu, "Menu.bmp", Common::Point(0, 0), true);
	puzzleAdd(kPuzzlePreferences);
	puzzleAddBackgroundImage(kPuzzlePreferences, "Login.bmp", Common::Point(0, 0), true);
	objectAdd(kObject6, "", "", 1);
	objectAddPuzzleAccessibility(kObject6, kPuzzleMenu, Common::Rect(498, 16, 528, 71), false, kCursorHotspot, 0);
	objectAddPuzzleAccessibility(kObject6, kPuzzleMenu, Common::Rect(0, 0, 640, 72), false, kCursorTake, 1);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 0, kPuzzleMenu, "a12_trak.tga", Common::Point(0, 16), true, kDrawType3, 1000);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 1, kPuzzleMenu, "a12_timer.tga", Common::Point(130, 16), true, kDrawType3, 1010);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 2, kPuzzleMenu, "a12_timer.tga", Common::Point(130, 46), true, kDrawType3, 1010);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 3, kPuzzleMenu, "a12_stairs.0001.tga", Common::Point(477, 16), true, kDrawType3, 1005);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 4, kPuzzleMenu, "a12_stairs.0002.tga", Common::Point(477, 16), true, kDrawType3, 1005);
	objectAddPresentation(kObject6);
	objectPresentationAddImageToPuzzle(kObject6, 5, kPuzzleMenu, "a12_humunculus.tga", Common::Point(502, 20), true, kDrawType3, 1020);
	objectAdd(kObject11, "", "", 1);
	objectAddPresentation(kObject11);
	objectPresentationAddImageToPuzzle(kObject11, 0, kPuzzleMenu, "a11s02n06p07.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	objectAddPresentation(kObject11);
	objectPresentationAddImageToPuzzle(kObject11, 1, kPuzzleMenu, "a11s02n05p03.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	objectAddPresentation(kObject11);
	objectPresentationAddImageToPuzzle(kObject11, 2, kPuzzleMenu, "a11s01n03p02l03.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	objectAddPresentation(kObject11);
	objectPresentationAddImageToPuzzle(kObject11, 3, kPuzzleMenu, "a11s01n08p01l04.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	objectAddPresentation(kObject11);
	objectPresentationAddImageToPuzzle(kObject11, 4, kPuzzleMenu, "a11s01n07p01l03.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	objectAddPresentation(kObject11);
	objectPresentationAddImageToPuzzle(kObject11, 5, kPuzzleMenu, "a11s01n06p01l05.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	objectAddPresentation(kObject11);
	objectPresentationAddImageToPuzzle(kObject11, 6, kPuzzleMenu, "a11s04n02p04l04.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	objectAddPresentation(kObject11);
	objectPresentationAddImageToPuzzle(kObject11, 7, kPuzzleMenu, "a11s04n02p03l04.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	objectAddPresentation(kObject11);
	objectPresentationAddImageToPuzzle(kObject11, 8, kPuzzleMenu, "a11s02n05p01.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	objectAddPresentation(kObject11);
	objectPresentationAddImageToPuzzle(kObject11, 9, kPuzzleMenu, "a11s02n06p10.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	objectAddPresentation(kObject11);
	objectPresentationAddImageToPuzzle(kObject11, 10, kPuzzleMenu, "a11s02n06p10l01.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	objectAddPresentation(kObject11);
	objectPresentationAddImageToPuzzle(kObject11, 11, kPuzzleMenu, "a11s02n06p11.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	objectAddPresentation(kObject11);
	objectPresentationAddImageToPuzzle(kObject11, 12, kPuzzleMenu, "a11s02n06p11l01.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	objectAddPresentation(kObject11);
	objectPresentationAddImageToPuzzle(kObject11, 13, kPuzzleMenu, "a11s02n06p12.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	objectAddPresentation(kObject11);
	objectPresentationAddImageToPuzzle(kObject11, 14, kPuzzleMenu, "a11s02n06p13.tga", Common::Point(163, 32), true, kDrawType3, 2000);
	objectAddPresentation(kObject11);
	objectPresentationAddImageToPuzzle(kObject11, 15, kPuzzleMenu, "a11s02n06p13l01.tga", Common::Point(163, 32), true, kDrawType3, 2000);
	objectAddPresentation(kObject11);
	objectPresentationAddImageToPuzzle(kObject11, 16, kPuzzleMenu, "a11s02n06p08.tga", Common::Point(0, 74), true, kDrawType3, 2000);
	objectAddPresentation(kObject11);
	objectPresentationAddImageToPuzzle(kObject11, 17, kPuzzleMenu, "a11s02n06p08l01.tga", Common::Point(0, 161), true, kDrawType3, 2000);
	objectAddPresentation(kObject11);
	objectPresentationAddImageToPuzzle(kObject11, 18, kPuzzleMenu, "a11s04n01p08l05.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	objectAdd(kObject12, "", "", 1);
	objectAddPresentation(kObject12);
	objectPresentationAddImageToPuzzle(kObject12, 0, kPuzzleMenu, "a11s04n01p11.tga", Common::Point(118, 130), true, kDrawType3, 1100);
	objectPresentationAddImageToPuzzle(kObject12, 0, kPuzzleMenu, "a11s04n01p11l01.tga", Common::Point(244, 210), true, kDrawType3, 1200);
	objectAddPresentation(kObject12);
	objectPresentationAddImageToPuzzle(kObject12, 1, kPuzzleMenu, "a11s04n01p11l02.tga", Common::Point(0, 20), true, kDrawType3, 1500);
	objectAddPresentation(kObject12);
	objectPresentationAddImageToPuzzle(kObject12, 2, kPuzzleMenu, "a11s04n01p11l01.tga", Common::Point(244, 210), true, kDrawType3, 2000);
	objectAddPresentation(kObject12);
	objectPresentationAddImageToPuzzle(kObject12, 3, kPuzzleMenu, "a11s04n01p11l03.tga", Common::Point(0, 16), true, kDrawType3, 2000);
	objectAdd(kObject23, "", "Cur_Drag", 5);
	objectSetActiveDrawCursor(kObject23, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	objectSetPassiveDrawCursor(kObject23, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	objectAddPuzzleAccessibility(kObject23, kPuzzleMenu, Common::Rect(220, 150, 420, 340), false, kCursorDragDrop, 0);
	objectAddPuzzleAccessibility(kObject23, kPuzzleMenu, Common::Rect(0, 0, 640, 480), false, kCursorBack, 99);
	objectAdd(kObjectCodingWheel, "Coding Wheel", "a11_CodeWheel", 1);
	objectSetActiveCursor(kObjectCodingWheel, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	objectSetPassiveCursor(kObjectCodingWheel, Common::Point(20, 20), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);

	for (uint32 i = 0; i < 36; i++) {
		objectAddPresentation(kObjectCodingWheel);
		objectPresentationAddImageToPuzzle(kObjectCodingWheel, i, kPuzzleMenu, Common::String::format("a11s04n01p16s01.%04d.tga", i + 1), Common::Point(220, 150), true, kDrawType3, 1000);
	}

	objectAdd(kObject13, "", "", 1);
	objectAddPresentation(kObject13);
	objectPresentationAddImageToPuzzle(kObject13, 0, kPuzzleMenu, "a11s02n05p02.tga", Common::Point(0, 360), true, kDrawType3, 900);
	objectAddPresentation(kObject13);
	objectPresentationAddImageToPuzzle(kObject13, 1, kPuzzleMenu, "a11s02n05p02l01.bmp", Common::Point(33, 395), true, kDrawType1, 1000);
	objectAddPresentation(kObject13);
	objectPresentationAddImageToPuzzle(kObject13, 2, kPuzzleMenu, "a11s02n05p02l02.bmp", Common::Point(56, 395), true, kDrawType1, 1000);
	objectAddPresentation(kObject13);
	objectPresentationAddImageToPuzzle(kObject13, 3, kPuzzleMenu, "a11s02n05p02l03.bmp", Common::Point(73, 392), true, kDrawType1, 1000);
	objectAddPresentation(kObject13);
	objectPresentationAddImageToPuzzle(kObject13, 4, kPuzzleMenu, "a11s02n05p02l04.bmp", Common::Point(93, 392), true, kDrawType1, 1000);
	objectAddPresentation(kObject13);
	objectPresentationAddImageToPuzzle(kObject13, 5, kPuzzleMenu, "a11s02n05p02l05.bmp", Common::Point(112, 390), true, kDrawType1, 1000);
	objectAddPresentation(kObject13);
	objectPresentationAddImageToPuzzle(kObject13, 6, kPuzzleMenu, "a11s02n05p02l06.bmp", Common::Point(24, 418), true, kDrawType1, 1000);
	objectAddPresentation(kObject13);
	objectPresentationAddImageToPuzzle(kObject13, 7, kPuzzleMenu, "a11s02n05p02l07.bmp", Common::Point(39, 414), true, kDrawType1, 1000);
	objectAddPresentation(kObject13);
	objectPresentationAddImageToPuzzle(kObject13, 8, kPuzzleMenu, "a11s02n05p02l08.bmp", Common::Point(55, 415), true, kDrawType1, 1000);
	objectAddPresentation(kObject13);
	objectPresentationAddImageToPuzzle(kObject13, 9, kPuzzleMenu, "a11s02n05p02l09.bmp", Common::Point(72, 414), true, kDrawType1, 1000);
	objectAddPresentation(kObject13);
	objectPresentationAddImageToPuzzle(kObject13, 10, kPuzzleMenu, "a11s02n05p02l10.bmp", Common::Point(104, 420), true, kDrawType1, 1000);
	objectAddPresentation(kObject13);
	objectPresentationAddImageToPuzzle(kObject13, 11, kPuzzleMenu, "a11s02n05p02l11.bmp", Common::Point(121, 419), true, kDrawType1, 1000);
	objectAdd(kObject7, "", "", 1);
	objectAddPresentation(kObject7);
	objectPresentationAddImageToPuzzle(kObject7, 0, kPuzzleMenu, "a12s07n01p01.tga", Common::Point(6, 350), true, kDrawType3, 1000);
	objectAddPresentation(kObject7);
	objectPresentationAddImageToPuzzle(kObject7, 1, kPuzzleMenu, "a12s07n01p01l01.tga", Common::Point(21, 387), true, kDrawType3, 1010);
	objectAddPresentation(kObject7);
	objectPresentationAddImageToPuzzle(kObject7, 2, kPuzzleMenu, "a12s07n01p01l02.tga", Common::Point(80, 387), true, kDrawType3, 1010);
	objectAddPresentation(kObject7);
	objectPresentationAddImageToPuzzle(kObject7, 3, kPuzzleMenu, "a12s07n01p01l03.tga", Common::Point(30, 360), true, kDrawType3, 1010);
	objectAddPresentation(kObject7);
	objectPresentationAddImageToPuzzle(kObject7, 4, kPuzzleMenu, "a12s07n01p01l04.tga", Common::Point(93, 360), true, kDrawType3, 1010);
	objectAdd(kObject8, "", "", 1);
	objectAddPresentation(kObject8);
	objectPresentationAddImageToPuzzle(kObject8, 0, kPuzzleMenu, "a07s01n14r01p01.tga", Common::Point(0, 16), true, kDrawType3, 1000);
	objectAddPresentation(kObject8);
	objectPresentationAddImageToPuzzle(kObject8, 1, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(10, 24), true, kDrawType3, 1100);
	objectAddPresentation(kObject8);
	objectPresentationAddImageToPuzzle(kObject8, 2, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(30, 24), true, kDrawType3, 1100);
	objectAddPresentation(kObject8);
	objectPresentationAddImageToPuzzle(kObject8, 3, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(50, 24), true, kDrawType3, 1100);
	objectAddPresentation(kObject8);
	objectPresentationAddImageToPuzzle(kObject8, 4, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(70, 24), true, kDrawType3, 1100);
	objectAddPresentation(kObject8);
	objectPresentationAddImageToPuzzle(kObject8, 5, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(90, 24), true, kDrawType3, 1100);
	objectAddPresentation(kObject8);
	objectPresentationAddImageToPuzzle(kObject8, 6, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(110, 24), true, kDrawType3, 1100);
	objectAddPresentation(kObject8);
	objectPresentationAddImageToPuzzle(kObject8, 7, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(130, 24), true, kDrawType3, 1100);
	objectAddPresentation(kObject8);
	objectPresentationAddImageToPuzzle(kObject8, 8, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(150, 24), true, kDrawType3, 1100);
	objectAddPresentation(kObject8);
	objectPresentationAddImageToPuzzle(kObject8, 9, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(170, 24), true, kDrawType3, 1100);
	objectAddPresentation(kObject8);
	objectPresentationAddImageToPuzzle(kObject8, 10, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(190, 24), true, kDrawType3, 1100);
	objectAddPresentation(kObject8);
	objectPresentationAddImageToPuzzle(kObject8, 11, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(210, 24), true, kDrawType3, 1100);
	objectAddPresentation(kObject8);
	objectPresentationAddImageToPuzzle(kObject8, 12, kPuzzleMenu, "a07s01n14r01p01l01.tga", Common::Point(230, 24), true, kDrawType3, 1100);
	objectAdd(kObject9, "", "", 1);
	objectAddPresentation(kObject9);
	objectPresentationAddImageToPuzzle(kObject9, 0, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(484, 22), true, kDrawType3, 1100);
	objectAddPresentation(kObject9);
	objectPresentationAddImageToPuzzle(kObject9, 1, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(494, 22), true, kDrawType3, 1100);
	objectAddPresentation(kObject9);
	objectPresentationAddImageToPuzzle(kObject9, 2, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(504, 22), true, kDrawType3, 1100);
	objectAddPresentation(kObject9);
	objectPresentationAddImageToPuzzle(kObject9, 3, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(514, 22), true, kDrawType3, 1100);
	objectAddPresentation(kObject9);
	objectPresentationAddImageToPuzzle(kObject9, 4, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(524, 22), true, kDrawType3, 1100);
	objectAddPresentation(kObject9);
	objectPresentationAddImageToPuzzle(kObject9, 5, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(534, 22), true, kDrawType3, 1100);
	objectAddPresentation(kObject9);
	objectPresentationAddImageToPuzzle(kObject9, 6, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(544, 22), true, kDrawType3, 1100);
	objectAddPresentation(kObject9);
	objectPresentationAddImageToPuzzle(kObject9, 7, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(554, 22), true, kDrawType3, 1100);
	objectAddPresentation(kObject9);
	objectPresentationAddImageToPuzzle(kObject9, 8, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(564, 22), true, kDrawType3, 1100);
	objectAddPresentation(kObject9);
	objectPresentationAddImageToPuzzle(kObject9, 9, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(574, 22), true, kDrawType3, 1100);
	objectAddPresentation(kObject9);
	objectPresentationAddImageToPuzzle(kObject9, 10, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(584, 22), true, kDrawType3, 1100);
	objectAddPresentation(kObject9);
	objectPresentationAddImageToPuzzle(kObject9, 11, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(594, 22), true, kDrawType3, 1100);
	objectAddPresentation(kObject9);
	objectPresentationAddImageToPuzzle(kObject9, 12, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(604, 22), true, kDrawType3, 1100);
	objectAddPresentation(kObject9);
	objectPresentationAddImageToPuzzle(kObject9, 13, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(614, 22), true, kDrawType3, 1100);
	objectAddPresentation(kObject9);
	objectPresentationAddImageToPuzzle(kObject9, 14, kPuzzleMenu, "a07s01n14r01p01l02.tga", Common::Point(624, 22), true, kDrawType3, 1100);
	objectAdd(kObject10, "", "", 1);
	objectAddPresentation(kObject10);
	objectPresentationAddImageToPuzzle(kObject10, 0, kPuzzleMenu, "a07s01n14r01p01l03.tga", Common::Point(270, 18), true, kDrawType3, 1100);
	objectAdd(kObject14, "", "", 1);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 0, kPuzzleMenu, "space.tga", Common::Point(0, 440), true, kDrawType3, 2000);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 1, kPuzzleMenu, "spacel_d.tga", Common::Point(0, 440), true, kDrawType3, 2100);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 2, kPuzzleMenu, "spacel_h.tga", Common::Point(0, 440), true, kDrawType3, 2100);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 3, kPuzzleMenu, "spacer_d.tga", Common::Point(440, 440), true, kDrawType3, 2100);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 4, kPuzzleMenu, "spacer_h.tga", Common::Point(440, 440), true, kDrawType3, 2100);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 5, kPuzzleMenu, "spacer_d.tga", Common::Point(440, 440), true, kDrawType3, 2100);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 6, kPuzzleMenu, "spacer_h.tga", Common::Point(440, 440), true, kDrawType3, 2100);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 7, kPuzzleMenu, "space_hP.tga", Common::Point(0, 264), true, kDrawType3, 2050);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 8, kPuzzleMenu, "space_hA.tga", Common::Point(0, 264), true, kDrawType3, 2050);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 9, kPuzzleMenu, "space_m.tga", Common::Point(440, 264), true, kDrawType3, 2050);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 10, kPuzzleMenu, "space_n.tga", Common::Point(440, 264), true, kDrawType3, 2050);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 11, kPuzzleMenu, "SY_map_A01.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 12, kPuzzleMenu, "SY_map_A01.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 13, kPuzzleMenu, "SY_map_A03.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 14, kPuzzleMenu, "SY_map_A05.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 15, kPuzzleMenu, "SY_map_A06.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 16, kPuzzleMenu, "SY_map_A07.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 17, kPuzzleMenu, "SY_map_A08.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 18, kPuzzleMenu, "SY_map_A09.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 19, kPuzzleMenu, "SY_map_A11.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 20, kPuzzleMenu, "SY_map_A12.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 21, kPuzzleMenu, "SY_map_A13.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 22, kPuzzleMenu, "SY_map_A14.bmp", Common::Point(0, 16), true, kDrawType1, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 23, kPuzzleMenu, "rol_l_j1.tga", Common::Point(0, 16), true, kDrawType3, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 24, kPuzzleMenu, "rol_l_j2.tga", Common::Point(0, 16), true, kDrawType3, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 25, kPuzzleMenu, "rol_nat1.tga", Common::Point(0, 16), true, kDrawType3, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 26, kPuzzleMenu, "rol_nat2.tga", Common::Point(0, 16), true, kDrawType3, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 27, kPuzzleMenu, "rol_fra1.tga", Common::Point(0, 16), true, kDrawType3, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 28, kPuzzleMenu, "rol_fra2.tga", Common::Point(0, 16), true, kDrawType3, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 29, kPuzzleMenu, "rol_kal1.tga", Common::Point(0, 16), true, kDrawType3, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 30, kPuzzleMenu, "rol_kal2.tga", Common::Point(0, 16), true, kDrawType3, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 31, kPuzzleMenu, "rol_han1.tga", Common::Point(0, 16), true, kDrawType3, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 32, kPuzzleMenu, "rol_han2.tga", Common::Point(0, 16), true, kDrawType3, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 33, kPuzzleMenu, "rol_tod1.tga", Common::Point(0, 16), true, kDrawType3, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 34, kPuzzleMenu, "rol_tod2.tga", Common::Point(0, 16), true, kDrawType3, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 35, kPuzzleMenu, "rol_gis1.tga", Common::Point(0, 16), true, kDrawType3, 900);
	objectAddPresentation(kObject14);
	objectPresentationAddImageToPuzzle(kObject14, 36, kPuzzleMenu, "rol_gis2.tga", Common::Point(0, 16), true, kDrawType3, 900);
	objectAddPuzzleAccessibility(kObject14, kPuzzleMenu, Common::Rect(7, 443, 169, 459), false, kCursorMenuActive, 0);
	objectAddPuzzleAccessibility(kObject14, kPuzzleMenu, Common::Rect(489, 441, 637, 462), false, kCursorMenuActive, 1);
	objectAddPuzzleAccessibility(kObject14, kPuzzleGeneralMenu, Common::Rect(469, 454, 637, 480), false, kCursorMenuActive, 2);
	objectAddPuzzleAccessibility(kObject14, kPuzzleMenu, Common::Rect(67, 329, 103, 411), false, kCursorMenuActive, 3);
	objectAddPuzzleAccessibility(kObject14, kPuzzleMenu, Common::Rect(0, 0, 1, 1), true, kCursorIdle, 4);
	objectSetPuzzleAccessibilityKey(kObject14, 4, Common::KEYCODE_ESCAPE);
	objectAddPuzzleAccessibility(kObject14, kPuzzleMenu, Common::Rect(346, 55, 585, 423), false, kCursorMenuActive, 5);
	objectAddPuzzleAccessibility(kObject14, kPuzzleMenu, Common::Rect( 81, 56, 288, 418), false, kCursorMenuActive, 6);
	objectAddPuzzleAccessibility(kObject14, kPuzzleGeneralMenu, Common::Rect(0, 420, 640, 480), false, kCursorBack, 99);
	objectAdd(kObject15, "", "", 1);
	objectAddPresentation(kObject15);
	objectPresentationAddAnimationToPuzzle(kObject15, 0, kPuzzleMenu, "limbo", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 2200, 183, 15.0f, 6);
	objectAdd(kObject16, "", "", 1);
	objectAddPresentation(kObject16);
	objectPresentationAddImageToPuzzle(kObject16, 0, kPuzzleMenu, "a01s02_mephisto.tga", Common::Point(80, 57), true, kDrawType3, 2100);
	objectAdd(kObject17, "", "", 1);
	objectAddPresentation(kObject17);
	objectPresentationAddImageToPuzzle(kObject17, 0, kPuzzleMenu, "a05s03n01p08.tga", Common::Point(0, 16), true, kDrawType3, 2100);
	objectAddPuzzleAccessibility(kObject17, kPuzzleMenu, Common::Rect(0, 0, 640, 480), false, kCursorBack, 0);
	objectAdd(kObject2, "", "", 1);
	objectAddPresentation(kObject2);
	objectPresentationAddImageToPuzzle(kObject2, 0, kPuzzleMenu, "Exit.tga", Common::Point(160, 165), true, kDrawType3, 2000);
	objectAddPuzzleAccessibility(kObject2, kPuzzleMenu, Common::Rect(290, 261, 370, 286), false, kCursorMenuActive, 1);
	objectAddPuzzleAccessibility(kObject2, kPuzzleMenu, Common::Rect(370, 261, 450, 286), false, kCursorMenuActive, 0);
	objectSetPuzzleAccessibilityKey(kObject2, 0, Common::KEYCODE_RETURN);
	objectSetPuzzleAccessibilityKey(kObject2, 1, Common::KEYCODE_ESCAPE);
	objectAddPresentation(kObject2);
	objectPresentationAddImageToPuzzle(kObject2, 1, kPuzzleMenu, "box_yes.tga", Common::Point(291, 261), true, kDrawType3, 2000);
	objectAddPresentation(kObject2);
	objectPresentationAddImageToPuzzle(kObject2, 2, kPuzzleMenu, "box_no.tga", Common::Point(371, 261), true, kDrawType3, 2000);
	objectAdd(kObject3, "", "", 1);
	objectAddPresentation(kObject3);
	objectPresentationAddImageToPuzzle(kObject3, 0, kPuzzleMenu, "Warning.tga", Common::Point(160, 165), true, kDrawType3, 1000);
	objectPresentationAddTextToPuzzle(kObject3, 0, kPuzzleMenu, "", Common::Point(200, 200), kFontDefault, Color(255, 95, 0), Color(-1, -1, -1));
	objectPresentationAddTextToPuzzle(kObject3, 0, kPuzzleMenu, "", Common::Point(300, 280), kFontDefault, Color(255, 95, 0), Color(-1, -1, -1));
	objectAddPresentation(kObject3);
	objectPresentationAddImageToPuzzle(kObject3, 1, kPuzzleMenu, "box_ok.tga", Common::Point(291, 261), true, kDrawType3, 1000);
	objectAddPuzzleAccessibility(kObject3, kPuzzleMenu, Common::Rect(286, 261, 363, 307), false, kCursorMenuActive, 0);
	objectSetPuzzleAccessibilityKey(kObject3, 0, Common::KEYCODE_RETURN);
	objectAdd(kObject4, "", "", 1);
	objectAddPresentation(kObject4);
	objectPresentationAddImageToPuzzle(kObject4, 0, kPuzzleMenu, "Question.tga", Common::Point(160, 165), true, kDrawType3, 1000);
	objectPresentationAddTextToPuzzle(kObject4, 0, kPuzzleMenu, "", Common::Point(200, 200), kFontDefault, Color(255, 95, 0), Color(-1, -1, -1));
	objectPresentationAddTextToPuzzle(kObject4, 0, kPuzzleMenu, "", Common::Point(200, 280), kFontDefault, Color(255, 95, 0), Color(-1, -1, -1));
	objectAddPresentation(kObject4);
	objectPresentationAddImageToPuzzle(kObject4, 1, kPuzzleMenu, "box_yes.tga", Common::Point(291, 261), true, kDrawType3, 1000);
	objectAddPresentation(kObject4);
	objectPresentationAddImageToPuzzle(kObject4, 2, kPuzzleMenu, "box_no.tga", Common::Point(371, 261), true, kDrawType3, 1000);
	objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(322, 254, 371, 288), false, kCursorMenuActive, 0);
	objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(372, 254, 420, 288), false, kCursorMenuActive, 1);
	objectSetPuzzleAccessibilityKey(kObject4, 0, Common::KEYCODE_RETURN);
	objectSetPuzzleAccessibilityKey(kObject4, 1, Common::KEYCODE_ESCAPE);
	objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(322, 254, 371, 288), false, kCursorMenuActive, 2);
	objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(372, 254, 420, 288), false, kCursorMenuActive, 3);
	objectSetPuzzleAccessibilityKey(kObject4, 2, Common::KEYCODE_RETURN);
	objectSetPuzzleAccessibilityKey(kObject4, 3, Common::KEYCODE_ESCAPE);
	objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(322, 254, 371, 288), false, kCursorMenuActive, 4);
	objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(372, 254, 420, 288), false, kCursorMenuActive, 5);
	objectSetPuzzleAccessibilityKey(kObject4, 4, Common::KEYCODE_RETURN);
	objectSetPuzzleAccessibilityKey(kObject4, 5, Common::KEYCODE_ESCAPE);
	objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(322, 254, 371, 288), false, kCursorMenuActive, 6);
	objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(372, 254, 420, 288), false, kCursorMenuActive, 7);
	objectSetPuzzleAccessibilityKey(kObject4, 6, Common::KEYCODE_RETURN);
	objectSetPuzzleAccessibilityKey(kObject4, 7, Common::KEYCODE_ESCAPE);
	objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(322, 254, 371, 288), false, kCursorMenuActive, 8);
	objectAddPuzzleAccessibility(kObject4, kPuzzleMenu, Common::Rect(372, 254, 420, 288), false, kCursorMenuActive, 9);
	objectSetPuzzleAccessibilityKey(kObject4, 8, Common::KEYCODE_RETURN);
	objectSetPuzzleAccessibilityKey(kObject4, 9, Common::KEYCODE_ESCAPE);
	objectAdd(kObject5, "", "", 1);
	puzzleAdd(kPuzzle90006);
	puzzleAddBackgroundImage(kPuzzle90006, "insertcd.bmp", Common::Point(0, 16), true);
	objectAdd(kObject55, "", "", 1);
	objectAddPresentation(kObject55);
	objectPresentationAddImageToPuzzle(kObject55, 0, kPuzzle90006, "box_yes.tga", Common::Point(291, 261), true, kDrawType3, 1000);
	objectAddPresentation(kObject55);
	objectPresentationAddTextToPuzzle(kObject55, 1, kPuzzle90006, "", Common::Point(320, 240), kFontDefault, Color(255, 95, 0), Color(-1, -1, -1));
	objectPresentationAddTextToPuzzle(kObject55, 1, kPuzzle90006, "", Common::Point(320, 260), kFontDefault, Color(255, 95, 0), Color(-1, -1, -1));
	objectAddPresentation(kObject55);
	objectPresentationAddImageToPuzzle(kObject55, 2, kPuzzle90006, "box_no.tga", Common::Point(371, 261), true, kDrawType3, 1000);
	objectPresentationShow(kObject55, 1);
	objectAddPuzzleAccessibility(kObject55, kPuzzle90006, Common::Rect(291, 261, 370, 311), true, kCursorMenuActive, 0);
	objectAddPuzzleAccessibility(kObject55, kPuzzle90006, Common::Rect(291, 261, 370, 311), true, kCursorMenuActive, 1);
	objectAddPuzzleAccessibility(kObject55, kPuzzle90006, Common::Rect(291, 261, 370, 311), true, kCursorMenuActive, 2);
	objectAddPuzzleAccessibility(kObject55, kPuzzle90006, Common::Rect(291, 261, 370, 311), true, kCursorMenuActive, 3);
	objectAddPuzzleAccessibility(kObject55, kPuzzle90006, Common::Rect(291, 261, 370, 311), true, kCursorMenuActive, 4);
	objectAddPuzzleAccessibility(kObject55, kPuzzle90006, Common::Rect(291, 261, 370, 311), true, kCursorMenuActive, 5);
	objectAddPuzzleAccessibility(kObject55, kPuzzle90006, Common::Rect(291, 261, 370, 311), true, kCursorMenuActive, 6);
	objectAddPuzzleAccessibility(kObject55, kPuzzle90006, Common::Rect(291, 261, 370, 311), true, kCursorMenuActive, 7);
	objectAddPuzzleAccessibility(kObject55, kPuzzle90006, Common::Rect(291, 261, 370, 311), true, kCursorMenuActive, 8);
	objectAddPuzzleAccessibility(kObject55, kPuzzle90006, Common::Rect(291, 261, 370, 311), true, kCursorMenuActive, 9);
	objectAddPuzzleAccessibility(kObject55, kPuzzle90006, Common::Rect(371, 261, 450, 311), true, kCursorMenuActive, 99);
	objectSetPuzzleAccessibilityKey(kObject55, 0, Common::KEYCODE_RETURN);
	objectSetPuzzleAccessibilityKey(kObject55, 1, Common::KEYCODE_RETURN);
	objectSetPuzzleAccessibilityKey(kObject55, 2, Common::KEYCODE_RETURN);
	objectSetPuzzleAccessibilityKey(kObject55, 3, Common::KEYCODE_RETURN);
	objectSetPuzzleAccessibilityKey(kObject55, 4, Common::KEYCODE_RETURN);
	objectSetPuzzleAccessibilityKey(kObject55, 5, Common::KEYCODE_RETURN);
	objectSetPuzzleAccessibilityKey(kObject55, 6, Common::KEYCODE_RETURN);
	objectSetPuzzleAccessibilityKey(kObject55, 7, Common::KEYCODE_RETURN);
	objectSetPuzzleAccessibilityKey(kObject55, 8, Common::KEYCODE_RETURN);
	objectSetPuzzleAccessibilityKey(kObject55, 9, Common::KEYCODE_RETURN);
	objectSetPuzzleAccessibilityKey(kObject55, 10, Common::KEYCODE_ESCAPE);
	objectAdd(kObject99500, "", "", 1);
	objectAddPuzzleAccessibility(kObject99500, kPuzzlePreferences, Common::Rect(132, 413, 252, 440), false, kCursorMenuActive, 1);
	objectAddPuzzleAccessibility(kObject99500, kPuzzlePreferences, Common::Rect(387, 413, 507, 440), false, kCursorMenuActive, 0);
	objectAddPresentation(kObject99500);
	objectPresentationAddImageToPuzzle(kObject99500, 0, kPuzzlePreferences, "log_l1h.bmp", Common::Point(132, 413), true, kDrawType1, 1100);
	objectAddPresentation(kObject99500);
	objectPresentationAddImageToPuzzle(kObject99500, 1, kPuzzlePreferences, "log_r1h.bmp", Common::Point(387, 413), true, kDrawType1, 1100);
	objectSetPuzzleAccessibilityKey(kObject99500, 0, Common::KEYCODE_RETURN);
	objectAdd(kObject99501, "", "", 1);
	objectAddPresentation(kObject99501);
	objectPresentationAddAnimationToPuzzle(kObject99501, 0, kPuzzlePreferences, "kybcur", kImageTypeBMP, Common::Point(0, 0), kDrawType1, 1000, 6, 12.5f, 16);
	objectPresentationSetAnimationCoordinatesOnPuzzle(kObject99501, 0, Common::Point(233, 334));
	objectPresentationAddTextToPuzzle(kObject99501, 0, kPuzzlePreferences, "", Common::Point(230, 334), kFontDefault, Color(255, 255, 255), Color(-1, -1, -1));
	objectPresentationShow(kObject99501);
	objectAdd(kObject99600, "", "", 1);
	objectAddPresentation(kObject99600);
	objectPresentationAddImageToPuzzle(kObject99600, 0, kPuzzleGeneralMenu, "Evidence_01.bmp", Common::Point(172, 452), true, kDrawType1, 6000);
	objectAddPresentation(kObject99600);
	objectPresentationAddImageToPuzzle(kObject99600, 1, kPuzzleGeneralMenu, "Evidence_02.bmp", Common::Point(213, 452), true, kDrawType1, 6000);
	objectAddPresentation(kObject99600);
	objectPresentationAddImageToPuzzle(kObject99600, 2, kPuzzleGeneralMenu, "Evidence_03.bmp", Common::Point(255, 452), true, kDrawType1, 6000);
	objectAddPresentation(kObject99600);
	objectPresentationAddImageToPuzzle(kObject99600, 3, kPuzzleGeneralMenu, "Evidence_04.bmp", Common::Point(297, 452), true, kDrawType1, 6000);
	objectAddPresentation(kObject99600);
	objectPresentationAddImageToPuzzle(kObject99600, 4, kPuzzleGeneralMenu, "Evidence_05.bmp", Common::Point(339, 452), true, kDrawType1, 6000);
	objectAddPresentation(kObject99600);
	objectPresentationAddImageToPuzzle(kObject99600, 5, kPuzzleGeneralMenu, "Evidence_06.bmp", Common::Point(381, 452), true, kDrawType1, 6000);
	objectAddPresentation(kObject99600);
	objectPresentationAddImageToPuzzle(kObject99600, 6, kPuzzleGeneralMenu, "Evidence_07.bmp", Common::Point(423, 452), true, kDrawType1, 6000);
	objectAddPresentation(kObject99600);
	objectPresentationAddImageToPuzzle(kObject99600, 7, kPuzzleGeneralMenu, "notebook.tga", Common::Point(456, 453), true, kDrawType3, 6000);
	objectAdd(kObject99010, "", "", 1);
	objectAddPuzzleAccessibility(kObject99010, kPuzzleGeneralMenu, Common::Rect(7, 19, 38, 49), true, kCursorMenuActive, 0);
	objectAddPuzzleAccessibility(kObject99010, kPuzzleGeneralMenu, Common::Rect(600, 19, 633, 50), true, kCursorMenuActive, 1);
	objectAddPresentation(kObject99010);
	objectPresentationAddAnimationToPuzzle(kObject99010, 0, kPuzzleGeneralMenu, "main", kImageTypeBMP, Common::Point(154, 50), kDrawType1, 2000, 63, 15.0f, 5);
	objectAddPresentation(kObject99010);
	objectPresentationAddImageToPuzzle(kObject99010, 1, kPuzzleGeneralMenu, "tit_ln.bmp", Common::Point(9, 22), true, kDrawType1, 2000);
	objectAddPresentation(kObject99010);
	objectPresentationAddImageToPuzzle(kObject99010, 2, kPuzzleGeneralMenu, "tit_lh.bmp", Common::Point(9, 22), true, kDrawType1, 2100);
	objectAddPresentation(kObject99010);
	objectPresentationAddImageToPuzzle(kObject99010, 3, kPuzzleGeneralMenu, "tit_rn.bmp", Common::Point(603, 22), true, kDrawType1, 2000);
	objectAddPresentation(kObject99010);
	objectPresentationAddImageToPuzzle(kObject99010, 4, kPuzzleGeneralMenu, "tit_rh.bmp", Common::Point(603, 22), true, kDrawType1, 2100);
	objectAdd(kObject99011, "", "", 3);
	objectAddPresentation(kObject99011);
	objectPresentationAddImageToPuzzle(kObject99011, 0, kPuzzleGeneralMenu, "tit_e00.bmp", Common::Point(48, 20), true, kDrawType1, 2000);
	objectAddPresentation(kObject99011);
	objectPresentationAddImageToPuzzle(kObject99011, 1, kPuzzleGeneralMenu, "tit_e01.bmp", Common::Point(48, 20), true, kDrawType1, 2000);
	objectAddPresentation(kObject99011);
	objectPresentationAddImageToPuzzle(kObject99011, 2, kPuzzleGeneralMenu, "tit_e02.bmp", Common::Point(48, 20), true, kDrawType1, 2000);
	objectAddPresentation(kObject99011);
	objectPresentationAddImageToPuzzle(kObject99011, 3, kPuzzleGeneralMenu, "tit_e03.bmp", Common::Point(48, 20), true, kDrawType1, 2000);
	objectAddPresentation(kObject99011);
	objectPresentationAddImageToPuzzle(kObject99011, 4, kPuzzleGeneralMenu, "tit_e04.bmp", Common::Point(48, 20), true, kDrawType1, 2000);
	objectAddPresentation(kObject99011);
	objectPresentationAddImageToPuzzle(kObject99011, 5, kPuzzleGeneralMenu, "tit_e05.bmp", Common::Point(48, 20), true, kDrawType1, 2000);
	objectAddPresentation(kObject99011);
	objectPresentationAddImageToPuzzle(kObject99011, 6, kPuzzleGeneralMenu, "tit_e06.bmp", Common::Point(48, 20), true, kDrawType1, 2000);
	objectAddPresentation(kObject99011);
	objectPresentationAddImageToPuzzle(kObject99011, 7, kPuzzleGeneralMenu, "tit_e07.bmp", Common::Point(48, 20), true, kDrawType1, 2000);
	objectPresentationShow(kObject99011, 0);
	objectAdd(kObject99012, "", "", 3);
	objectAddPresentation(kObject99012);
	objectPresentationAddImageToPuzzle(kObject99012, 0, kPuzzleGeneralMenu, "main.bmp", Common::Point(130, 50), true, kDrawType1, 1000);
	objectAdd(kObject99000, "", "", 1);
	objectAddPuzzleAccessibility(kObject99000, kPuzzleGeneralMenu, Common::Rect(0, 135, 120, 162), true, kCursorMenuActive, 0);
	objectAddPresentation(kObject99000);
	objectPresentationAddImageToPuzzle(kObject99000, 0, kPuzzleGeneralMenu, "But_l1h.bmp", Common::Point(0, 135), true, kDrawType1, 1000);
	objectAddPresentation(kObject99000);
	objectPresentationAddImageToPuzzle(kObject99000, 1, kPuzzleGeneralMenu, "But_l1d.bmp", Common::Point(0, 135), true, kDrawType1, 1000);
	objectAdd(kObject99001, "", "", 1);
	objectAddPuzzleAccessibility(kObject99001, kPuzzleGeneralMenu, Common::Rect(0, 185, 120, 212), true, kCursorMenuActive, 0);
	objectAddPresentation(kObject99001);
	objectPresentationAddImageToPuzzle(kObject99001, 0, kPuzzleGeneralMenu, "But_l2h.bmp", Common::Point(0, 185), true, kDrawType1, 1200);
	objectAddPresentation(kObject99001);
	objectPresentationAddImageToPuzzle(kObject99001, 1, kPuzzleGeneralMenu, "But_l2d.bmp", Common::Point(0, 185), true, kDrawType1, 1000);
	objectAddPresentation(kObject99001);
	objectPresentationAddImageToPuzzle(kObject99001, 2, kPuzzleGeneralMenu, "Load.bmp", Common::Point(130, 50), true, kDrawType1, 2000);
	objectAdd(kObject99002, "", "", 1);
	objectAddPuzzleAccessibility(kObject99002, kPuzzleGeneralMenu, Common::Rect(0, 235, 120, 262), true, kCursorMenuActive, 0);
	objectAddPresentation(kObject99002);
	objectPresentationAddImageToPuzzle(kObject99002, 0, kPuzzleGeneralMenu, "But_l3h.bmp", Common::Point(0, 235), true, kDrawType1, 1200);
	objectAddPresentation(kObject99002);
	objectPresentationAddImageToPuzzle(kObject99002, 1, kPuzzleGeneralMenu, "But_l3d.bmp", Common::Point(0, 235), true, kDrawType1, 1000);
	objectAddPresentation(kObject99002);
	objectPresentationAddImageToPuzzle(kObject99002, 2, kPuzzleGeneralMenu, "Save.bmp", Common::Point(130, 50), true, kDrawType1, 2000);
	objectAdd(kObject99003, "", "", 1);
	objectAddPuzzleAccessibility(kObject99003, kPuzzleGeneralMenu, Common::Rect(0, 285, 120, 312), true, kCursorMenuActive, 0);
	objectAddPresentation(kObject99003);
	objectPresentationAddImageToPuzzle(kObject99003, 0, kPuzzleGeneralMenu, "But_l4h.bmp", Common::Point(0, 285), true, kDrawType1, 1200);
	objectAddPresentation(kObject99003);
	objectPresentationAddImageToPuzzle(kObject99003, 1, kPuzzleGeneralMenu, "But_l4d.bmp", Common::Point(0, 285), true, kDrawType1, 1000);
	objectAddPresentation(kObject99003);
	objectPresentationAddImageToPuzzle(kObject99003, 2, kPuzzleGeneralMenu, "Preferences.bmp", Common::Point(130, 50), true, kDrawType1, 2000);
	objectAdd(kObject99004, "", "", 1);
	objectAddPuzzleAccessibility(kObject99004, kPuzzleGeneralMenu, Common::Rect(0, 335, 120, 362), true, kCursorMenuActive, 0);
	objectAddPresentation(kObject99004);
	objectPresentationAddImageToPuzzle(kObject99004, 0, kPuzzleGeneralMenu, "But_l5h.bmp", Common::Point(0, 335), true, kDrawType1, 1200);
	objectAddPresentation(kObject99004);
	objectPresentationAddImageToPuzzle(kObject99004, 1, kPuzzleGeneralMenu, "But_l5d.bmp", Common::Point(0, 335), true, kDrawType1, 1000);
	objectAddPresentation(kObject99004);
	objectPresentationAddTextToPuzzle(kObject99004, 2, kPuzzleGeneralMenu, "", Common::Point(1, 16), kFontDefault, Color(0, 0, 0), Color(255, 255, 255));
	objectAdd(kObject99005, "", "", 1);
	objectAddPuzzleAccessibility(kObject99005, kPuzzleGeneralMenu, Common::Rect(520, 135, 640, 162), true, kCursorMenuActive, 0);
	objectAddPresentation(kObject99005);
	objectPresentationAddImageToPuzzle(kObject99005, 0, kPuzzleGeneralMenu, "But_r1h.bmp", Common::Point(520, 135), true, kDrawType1, 1200);
	objectAddPresentation(kObject99005);
	objectPresentationAddImageToPuzzle(kObject99005, 1, kPuzzleGeneralMenu, "But_r1d.bmp", Common::Point(520, 135), true, kDrawType1, 1000);
	objectAddPresentation(kObject99005);
	objectPresentationAddImageToPuzzle(kObject99005, 2, kPuzzleGeneralMenu, "stars_e01.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	objectAddPresentation(kObject99005);
	objectPresentationAddImageToPuzzle(kObject99005, 3, kPuzzleGeneralMenu, "stars_e02.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	objectAddPresentation(kObject99005);
	objectPresentationAddImageToPuzzle(kObject99005, 4, kPuzzleGeneralMenu, "stars_e03.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	objectAddPresentation(kObject99005);
	objectPresentationAddImageToPuzzle(kObject99005, 5, kPuzzleGeneralMenu, "stars_e04.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	objectAddPresentation(kObject99005);
	objectPresentationAddImageToPuzzle(kObject99005, 6, kPuzzleGeneralMenu, "stars_e05.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	objectAddPresentation(kObject99005);
	objectPresentationAddImageToPuzzle(kObject99005, 7, kPuzzleGeneralMenu, "stars_e06.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	objectAddPresentation(kObject99005);
	objectPresentationAddImageToPuzzle(kObject99005, 8, kPuzzleGeneralMenu, "stars_e07.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	objectAdd(kObject99006, "", "", 1);
	objectAddPuzzleAccessibility(kObject99006, kPuzzleGeneralMenu, Common::Rect(520, 185, 640, 212), true, kCursorMenuActive, 0);
	objectAddPresentation(kObject99006);
	objectPresentationAddImageToPuzzle(kObject99006, 0, kPuzzleGeneralMenu, "But_r2h.bmp", Common::Point(520, 185), true, kDrawType1, 1200);
	objectAddPresentation(kObject99006);
	objectPresentationAddImageToPuzzle(kObject99006, 1, kPuzzleGeneralMenu, "But_r2d.bmp", Common::Point(520, 185), true, kDrawType1, 1000);
	objectAddPresentation(kObject99006);
	objectPresentationAddImageToPuzzle(kObject99006, 2, kPuzzleGeneralMenu, "Movies.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	objectAdd(kObject99007, "", "", 1);
	objectAddPuzzleAccessibility(kObject99007, kPuzzleGeneralMenu, Common::Rect(520, 235, 640, 262), true, kCursorMenuActive, 0);
	objectAddPresentation(kObject99007);
	objectPresentationAddImageToPuzzle(kObject99007, 0, kPuzzleGeneralMenu, "But_r3h.bmp", Common::Point(520, 235), true, kDrawType1, 1200);
	objectAddPresentation(kObject99007);
	objectPresentationAddImageToPuzzle(kObject99007, 1, kPuzzleGeneralMenu, "But_r3d.bmp", Common::Point(520, 235), true, kDrawType1, 1000);
	objectAddPresentation(kObject99007);
	objectPresentationAddImageToPuzzle(kObject99007, 2, kPuzzleGeneralMenu, "Music.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	objectAdd(kObject99008, "", "", 1);
	objectAddPuzzleAccessibility(kObject99008, kPuzzleGeneralMenu, Common::Rect(520, 285, 640, 312), true, kCursorMenuActive, 0);
	objectAddPresentation(kObject99008);
	objectPresentationAddImageToPuzzle(kObject99008, 0, kPuzzleGeneralMenu, "But_r4h.bmp", Common::Point(520, 285), true, kDrawType1, 1200);
	objectAddPresentation(kObject99008);
	objectPresentationAddImageToPuzzle(kObject99008, 1, kPuzzleGeneralMenu, "But_r4d.bmp", Common::Point(520, 285), true, kDrawType1, 1000);
	objectAddPresentation(kObject99008);
	objectPresentationAddImageToPuzzle(kObject99008, 2, kPuzzleGeneralMenu, "Status.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	objectAdd(kObject99009, "", "", 1);
	objectAddPuzzleAccessibility(kObject99009, kPuzzleGeneralMenu, Common::Rect(520, 335, 640, 362), true, kCursorMenuActive, 0);
	objectAddPresentation(kObject99009);
	objectPresentationAddImageToPuzzle(kObject99009, 0, kPuzzleGeneralMenu, "But_r5h.bmp", Common::Point(520, 335), true, kDrawType1, 1200);
	objectAddPresentation(kObject99009);
	objectPresentationAddImageToPuzzle(kObject99009, 1, kPuzzleGeneralMenu, "But_r5d.bmp", Common::Point(520, 335), true, kDrawType1, 1000);
	objectAddPresentation(kObject99009);
	objectPresentationAddImageToPuzzle(kObject99009, 2, kPuzzleGeneralMenu, "Attraction.bmp", Common::Point(130, 50), true, kDrawType1, 2200);
	objectAdd(kObject99021, "", "", 1);
	objectAddPuzzleAccessibility(kObject99021, kPuzzleGeneralMenu, Common::Rect(259, 413, 379, 440), false, kCursorMenuActive, 0);
	objectAddPresentation(kObject99021);
	objectPresentationAddImageToPuzzle(kObject99021, 0, kPuzzleGeneralMenu, "but_d2h.bmp", Common::Point(259, 413), true, kDrawType1, 2400);
	objectAddPresentation(kObject99021);
	objectPresentationAddImageToPuzzle(kObject99021, 1, kPuzzleGeneralMenu, "but_d2d.bmp", Common::Point(259, 413), true, kDrawType1, 2300);
	objectAddPresentation(kObject99021);
	objectPresentationAddImageToPuzzle(kObject99021, 2, kPuzzleGeneralMenu, "but_d2n.bmp", Common::Point(259, 413), true, kDrawType1, 2300);
	objectAdd(kObject99022, "", "", 1);
	objectAddPuzzleAccessibility(kObject99022, kPuzzleGeneralMenu, Common::Rect(386, 413, 506, 440), false, kCursorMenuActive, 0);
	objectAddPresentation(kObject99022);
	objectPresentationAddImageToPuzzle(kObject99022, 0, kPuzzleGeneralMenu, "but_d3h.bmp", Common::Point(386, 413), true, kDrawType1, 2400);
	objectAddPresentation(kObject99022);
	objectPresentationAddImageToPuzzle(kObject99022, 1, kPuzzleGeneralMenu, "but_d3d.bmp", Common::Point(386, 413), true, kDrawType1, 2300);
	objectAddPresentation(kObject99022);
	objectPresentationAddImageToPuzzle(kObject99022, 2, kPuzzleGeneralMenu, "but_d3n.bmp", Common::Point(386, 413), true, kDrawType1, 2300);
	objectSetPuzzleAccessibilityKey(kObject99022, 0, Common::KEYCODE_ESCAPE);
	objectAdd(kObject99023, "", "", 1);
	objectAddPuzzleAccessibility(kObject99023, kPuzzleGeneralMenu, Common::Rect(131, 413, 251, 440), false, kCursorMenuActive, 0);
	objectAddPresentation(kObject99023);
	objectPresentationAddImageToPuzzle(kObject99023, 0, kPuzzleGeneralMenu, "but_d1h.bmp", Common::Point(131, 413), true, kDrawType1, 2400);
	objectAddPresentation(kObject99023);
	objectPresentationAddImageToPuzzle(kObject99023, 1, kPuzzleGeneralMenu, "but_d1d.bmp", Common::Point(131, 413), true, kDrawType1, 2300);
	objectAddPresentation(kObject99023);
	objectPresentationAddImageToPuzzle(kObject99023, 2, kPuzzleGeneralMenu, "but_d1n.bmp", Common::Point(131, 413), true, kDrawType1, 2300);
	objectSetPuzzleAccessibilityKey(kObject99023, 0, Common::KEYCODE_RETURN);
	objectAdd(kObject99024, "", "", 1);
	objectAddPuzzleAccessibility(kObject99024, kPuzzleGeneralMenu, Common::Rect(386, 413, 506, 440), false, kCursorMenuActive, 0);
	objectAddPresentation(kObject99024);
	objectPresentationAddImageToPuzzle(kObject99024, 0, kPuzzleGeneralMenu, "but_d3h.bmp", Common::Point(386, 413), true, kDrawType1, 2400);
	objectAddPresentation(kObject99024);
	objectPresentationAddImageToPuzzle(kObject99024, 1, kPuzzleGeneralMenu, "but_d3d.bmp", Common::Point(386, 413), true, kDrawType1, 2300);
	objectAddPresentation(kObject99024);
	objectPresentationAddImageToPuzzle(kObject99024, 2, kPuzzleGeneralMenu, "but_d3n.bmp", Common::Point(386, 413), true, kDrawType1, 2300);
	objectSetPuzzleAccessibilityKey(kObject99024, 0, Common::KEYCODE_ESCAPE);
	objectAdd(kObject99025, "", "", 1);
	objectAddPuzzleAccessibility(kObject99025, kPuzzleGeneralMenu, Common::Rect(131, 413, 251, 440), false, kCursorMenuActive, 0);
	objectAddPresentation(kObject99025);
	objectPresentationAddImageToPuzzle(kObject99025, 0, kPuzzleGeneralMenu, "but_d1h.bmp", Common::Point(131, 413), true, kDrawType1, 2400);
	objectAddPresentation(kObject99025);
	objectPresentationAddImageToPuzzle(kObject99025, 1, kPuzzleGeneralMenu, "but_d1d.bmp", Common::Point(131, 413), true, kDrawType1, 2300);
	objectAddPresentation(kObject99025);
	objectPresentationAddImageToPuzzle(kObject99025, 2, kPuzzleGeneralMenu, "but_d1n.bmp", Common::Point(131, 413), true, kDrawType1, 2300);
	objectSetPuzzleAccessibilityKey(kObject99025, 0, Common::KEYCODE_RETURN);
	objectAdd(kObject99041, "", "", 1);
	objectAddPuzzleAccessibility(kObject99041, kPuzzleGeneralMenu, Common::Rect(386, 413, 506, 440), false, kCursorMenuActive, 0);
	objectAddPresentation(kObject99041);
	objectPresentationAddImageToPuzzle(kObject99041, 0, kPuzzleGeneralMenu, "but_d3h.bmp", Common::Point(386, 413), true, kDrawType1, 2400);
	objectAddPresentation(kObject99041);
	objectPresentationAddImageToPuzzle(kObject99041, 1, kPuzzleGeneralMenu, "but_d3d.bmp", Common::Point(386, 413), true, kDrawType1, 2300);
	objectAddPresentation(kObject99041);
	objectPresentationAddImageToPuzzle(kObject99041, 2, kPuzzleGeneralMenu, "but_d3n.bmp", Common::Point(386, 413), true, kDrawType1, 2300);
	objectSetPuzzleAccessibilityKey(kObject99041, 0, Common::KEYCODE_ESCAPE);
	objectAdd(kObject99042, "", "", 1);
	objectAddPuzzleAccessibility(kObject99042, kPuzzleGeneralMenu, Common::Rect(131, 413, 251, 440), false, kCursorMenuActive, 0);
	objectAddPresentation(kObject99042);
	objectPresentationAddImageToPuzzle(kObject99042, 0, kPuzzleGeneralMenu, "but_d1h.bmp", Common::Point(131, 413), true, kDrawType1, 2400);
	objectAddPresentation(kObject99042);
	objectPresentationAddImageToPuzzle(kObject99042, 1, kPuzzleGeneralMenu, "but_d1d.bmp", Common::Point(131, 413), true, kDrawType1, 2300);
	objectAddPresentation(kObject99042);
	objectPresentationAddImageToPuzzle(kObject99042, 2, kPuzzleGeneralMenu, "but_d1n.bmp", Common::Point(131, 413), true, kDrawType1, 2300);
	objectSetPuzzleAccessibilityKey(kObject99042, 0, Common::KEYCODE_RETURN);
	objectAdd(kObject99043, "", "", 1);
	objectAddPuzzleAccessibility(kObject99043, kPuzzleGeneralMenu, Common::Rect(338, 316, 405, 342), false, kCursorMenuActive, 0);
	objectAddPresentation(kObject99043);
	objectPresentationAddImageToPuzzle(kObject99043, 0, kPuzzleGeneralMenu, "Pref_off.tga", Common::Point(300, 322), true, kDrawType3, 2200);
	objectAddPresentation(kObject99043);
	objectPresentationAddImageToPuzzle(kObject99043, 1, kPuzzleGeneralMenu, "Pref_on.tga", Common::Point(300, 322), true, kDrawType3, 2200);
	objectAdd(kObject99044, "", "", 1);
	objectAddPuzzleAccessibility(kObject99044, kPuzzleGeneralMenu, Common::Rect(338, 251, 405, 277), false, kCursorMenuActive, 0);
	objectAddPresentation(kObject99044);
	objectPresentationAddImageToPuzzle(kObject99044, 0, kPuzzleGeneralMenu, "Pref_nor.tga", Common::Point(300, 257), true, kDrawType3, 2200);
	objectAddPresentation(kObject99044);
	objectPresentationAddImageToPuzzle(kObject99044, 1, kPuzzleGeneralMenu, "Pref_rev.tga", Common::Point(300, 257), true, kDrawType3, 2300);
	objectAdd(kObject99045, "", "CUR_DRAG", 4);
	objectSetActiveDrawCursor(kObject99045, Common::Point(15, 15), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	objectSetPassiveDrawCursor(kObject99045, Common::Point(15, 15), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	objectAddPuzzleAccessibility(kObject99045, kPuzzleGeneralMenu, Common::Rect(150, 160, 492, 184), false, kCursorDragDrop, 0);
	objectAddPresentation(kObject99045);
	objectPresentationAddImageToPuzzle(kObject99045, 0, kPuzzleGeneralMenu, "Volume.tga", Common::Point(0, 0), true, kDrawType3, 2200);
	objectAdd(kObject99046, "", "CUR_DRAG", 4);
	objectSetActiveDrawCursor(kObject99046, Common::Point(15, 15), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	objectSetPassiveDrawCursor(kObject99046, Common::Point(15, 15), 0, kCursorTypeImage, 0.0f, 0, kLoadFromCursor);
	objectAddPuzzleAccessibility(kObject99046, kPuzzleGeneralMenu, Common::Rect(150, 204, 492, 228), false, kCursorDragDrop, 0);
	objectAddPresentation(kObject99046);
	objectPresentationAddImageToPuzzle(kObject99046, 0, kPuzzleGeneralMenu, "Volume.tga", Common::Point(0, 0), true, kDrawType3, 2200);
	objectAdd(kObject99601, "", "", 1);
	objectAddPresentation(kObject99601);
	objectPresentationAddAnimationToPuzzle(kObject99601, 0, kPuzzleGeneralMenu, "kybcur", kImageTypeBMP, Common::Point(0, 0), kDrawType1, 2300, 6, 12.5f, 16);
	objectPresentationSetAnimationCoordinatesOnPuzzle(kObject99601, 0, Common::Point(192, 388));
	objectAddPresentation(kObject99601);
	objectPresentationAddTextToPuzzle(kObject99601, 1, kPuzzleGeneralMenu, "", Common::Point(230, 382), kFontDefault, Color(255, 255, 255), Color(-1, -1, -1));
	objectAddPresentation(kObject99601);

	_archiveType = kArchiveFile;
	_loadFrom = kLoadFrom5;
	objectPresentationAddImageToPuzzle(kObject99601, 2, kPuzzleGeneralMenu, "osc.bmp", Common::Point(240, 82), true, kDrawType1, 5000);
	_loadFrom = kLoadFromDisk;
	_archiveType = getZoneArchiveType(kZoneSY);

	objectAdd(kObject99070, "", "", 1);
	objectAddPresentation(kObject99070);
	objectPresentationAddAnimationToPuzzle(kObject99070, 0, kPuzzleGeneralMenu, "movies", kImageTypeBMP, Common::Point(137, 52), kDrawType1, 2500, 36, 15.0f, 5);
	objectPresentationAddImageToPuzzle(kObject99070, 0, kPuzzleGeneralMenu, "tit_movies.bmp", Common::Point(48, 20), true, kDrawType1, 2200);
	objectAdd(kObject99071, "", "", 1);
	objectAddPuzzleAccessibility(kObject99071, kPuzzleGeneralMenu, Common::Rect(200, 220, 430, 399), false, kCursorMenuActive, 0);

	for (uint32 i = 0; i < 32; i++) {
		objectAddPresentation(kObject99071);
		objectPresentationAddImageToPuzzle(kObject99071, i, kPuzzleGeneralMenu, Common::String::format("mov_tit%02d.bmp", i), Common::Point(130, 214), true, kDrawType1, 2200);
	}

	objectAdd(kObject99080, "", "", 1);
	objectAddPresentation(kObject99080);
	objectPresentationAddAnimationToPuzzle(kObject99080, 0, kPuzzleGeneralMenu, "music", kImageTypeBMP, Common::Point(204, 66), kDrawType1, 2500, 4, 15.0f, 5);
	objectPresentationAddImageToPuzzle(kObject99080, 0, kPuzzleGeneralMenu, "tit_music.bmp", Common::Point(48, 20), true, kDrawType1, 2200);
	objectAdd(kObject99081, "", "", 1);

	for (uint32 i = 0; i < 13; i++) {
		objectAddPresentation(kObject99081);
		objectPresentationAddImageToPuzzle(kObject99081, i, kPuzzleGeneralMenu, Common::String::format("mus_tit%02d.bmp", i + 1), Common::Point(135, 300), true, kDrawType1, 2200);
	}

	objectAdd(kObject99090, "", "", 1);
	objectAddPresentation(kObject99090);
	objectPresentationAddAnimationToPuzzle(kObject99090, 0, kPuzzleGeneralMenu, "status", kImageTypeBMP, Common::Point(181, 102), kDrawType1, 2500, 50, 15.0f, 6);
	objectPresentationAddImageToPuzzle(kObject99090, 0, kPuzzleGeneralMenu, "tit_status.bmp", Common::Point(48, 20), true, kDrawType1, 2200);
	objectAdd(kObject99091, "", "", 1);
	objectAddPuzzleAccessibility(kObject99091, kPuzzleGeneralMenu, Common::Rect(200, 120, 450, 150), false, kCursorMenuActive, 1);
	objectAddPuzzleAccessibility(kObject99091, kPuzzleGeneralMenu, Common::Rect(200, 240, 450, 270), false, kCursorMenuActive, 2);
	objectAddPuzzleAccessibility(kObject99091, kPuzzleGeneralMenu, Common::Rect(200, 360, 450, 390), false, kCursorMenuActive, 3);
	objectAddPresentation(kObject99091);
	objectPresentationAddImageToPuzzle(kObject99091, 0, kPuzzleGeneralMenu, "tit_attraction.bmp", Common::Point(48, 20), true, kDrawType1, 2200);
	objectAddPresentation(kObject99091);
	objectPresentationAddImageToPuzzle(kObject99091, 1, kPuzzleGeneralMenu, "att_01.bmp", Common::Point(200, 118), true, kDrawType1, 2200);
	objectAddPresentation(kObject99091);
	objectPresentationAddImageToPuzzle(kObject99091, 2, kPuzzleGeneralMenu, "att_02.bmp", Common::Point(200, 238), true, kDrawType1, 2200);
	objectAddPresentation(kObject99091);
	objectPresentationAddImageToPuzzle(kObject99091, 3, kPuzzleGeneralMenu, "att_03.bmp", Common::Point(200, 358), true, kDrawType1, 2200);
	objectAdd(kObject99100, "", "", 1);
	objectAddPuzzleAccessibility(kObject99100, kPuzzleGeneralMenu, Common::Rect(0, 454, 50, 480), false, kCursorMenuActive, 0);
	objectAdd(kObject99097, "", "", 1);
	objectAddPuzzleAccessibility(kObject99097, kPuzzleGeneralMenu, Common::Rect(261, 414, 381, 444), false, kCursorMenuActive, 0);
	objectAddPresentation(kObject99097);
	objectPresentationAddImageToPuzzle(kObject99097, 0, kPuzzleGeneralMenu, "but_d4h.bmp", Common::Point(261, 414), true, kDrawType1, 2400);
	objectAddPresentation(kObject99097);
	objectPresentationAddImageToPuzzle(kObject99097, 1, kPuzzleGeneralMenu, "but_d4d.bmp", Common::Point(261, 414), true, kDrawType1, 2300);
	objectAddPresentation(kObject99097);
	objectPresentationAddImageToPuzzle(kObject99097, 2, kPuzzleGeneralMenu, "but_d4n.bmp", Common::Point(261, 414), true, kDrawType1, 2300);
	objectSetPuzzleAccessibilityKey(kObject99097, 0, Common::KEYCODE_ESCAPE);
	objectAdd(kObject25, "", "", 1);
	objectAddPresentation(kObject25);
	objectPresentationAddImageToPuzzle(kObject25, 0, kPuzzleMenu, "dat01_1.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	objectAddPresentation(kObject25);
	objectPresentationAddImageToPuzzle(kObject25, 1, kPuzzleMenu, "dat02_2.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	objectAddPresentation(kObject25);
	objectPresentationAddImageToPuzzle(kObject25, 2, kPuzzleMenu, "dat03_2.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	objectAddPresentation(kObject25);
	objectPresentationAddImageToPuzzle(kObject25, 3, kPuzzleMenu, "dat04_3.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	objectAddPresentation(kObject25);
	objectPresentationAddImageToPuzzle(kObject25, 4, kPuzzleMenu, "dat05_3.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	objectAddPresentation(kObject25);
	objectPresentationAddImageToPuzzle(kObject25, 5, kPuzzleMenu, "dat06_4.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	objectAddPresentation(kObject25);
	objectPresentationAddImageToPuzzle(kObject25, 6, kPuzzleMenu, "dat07_5.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	objectAddPresentation(kObject25);
	objectPresentationAddImageToPuzzle(kObject25, 7, kPuzzleMenu, "dat08_6.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	objectAddPresentation(kObject25);
	objectPresentationAddImageToPuzzle(kObject25, 8, kPuzzleMenu, "dat09_7.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	objectAddPresentation(kObject25);
	objectPresentationAddImageToPuzzle(kObject25, 9, kPuzzleMenu, "dat10_4.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	objectAddPresentation(kObject25);
	objectPresentationAddImageToPuzzle(kObject25, 10, kPuzzleMenu, "dat11_4.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	objectAddPresentation(kObject25);
	objectPresentationAddImageToPuzzle(kObject25, 11, kPuzzleMenu, "dat12_4.bmp", Common::Point(0, 464), true, kDrawType1, 3000);
	objectAddPresentation(kObject25);
	objectPresentationAddImageToPuzzle(kObject25, 12, kPuzzleMenu, "dat13_4.bmp", Common::Point(0, 464), true, kDrawType1, 3000);

	visualListAddToPuzzle(3, kPuzzlePreferences, 129,
	                      "", "", "", "", "ar_uph.tga", "", "ar_upn.tga", "", "ar_dnh.tga", "", "ar_dnn.tga", "", "",
	                      kDrawType3, Common::Point(0, 0), Common::Point(0, 0), 230, 67, 170, 20, 20, 3,
	                      Common::Point(418, 67), 408, 57, 40, 40, Common::Point(418, 276), 408, 266,
	                      40, 40, Common::Point(0, 0), kImageTypeBMP, kDrawType1, 375, 30, 11,
	                      Color(245, 235, 50), Color(255, 255, 255), Color(-1, -1, -1), kFontDefault, kArchiveFile);

	visualListAddToPuzzle(1, kPuzzleGeneralMenu, 129,
	                      "", "", "", "", "ar_uph.tga", "", "ar_upn.tga", "", "ar_dnh.tga", "", "ar_dnn.tga", "", "",
	                      kDrawType3, Common::Point(0, 0), Common::Point(0, 0), 230, 222, 150, 20, 20, 3,
	                      Common::Point(418, 224), 408, 214, 40, 40, Common::Point(418, 351), 408, 341,
	                      40, 40, Common::Point(0, 0), kImageTypeBMP, kDrawType1, 170, 300, 7,
	                      Color(245, 235, 50), Color(255, 255, 255), Color(-1, -1, -1), kFontDefault, kArchiveFile);

	visualListSetOff(1, kPuzzleGeneralMenu);

	visualListAddToPuzzle(4, kPuzzleGeneralMenu, 65,
	                      "", "", "", "", "ar_uph.tga", "", "ar_upn.tga", "", "ar_dnh.tga", "", "ar_dnn.tga", "", "",
	                      kDrawType3, Common::Point(0, 0), Common::Point(0, 0), 230, 222, 150, 20, 20, 3,
	                      Common::Point(418, 224), 408, 214, 40, 40, Common::Point(418, 351), 408, 341,
	                      40, 40, Common::Point(240, 82), kImageTypeBMP, kDrawType1, 170, 300, 7,
	                      Color(245, 235, 50), Color(255, 255, 255), Color(-1, -1, -1), kFontDefault, kArchiveFile);

	visualListSetOff(4, kPuzzleGeneralMenu);

	soundAdd(90002, kSoundTypeAmbientEffect, "2199.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(90003, kSoundTypeAmbientEffect, "2200.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(90004, kSoundTypeAmbientEffect, "2201.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(90005, kSoundTypeAmbientEffect, "2202.wav", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(90001, kSoundTypeAmbientMusic, "2203.wac", _configuration.backgroundMusic.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	soundSetVolume(90001, 90);
	soundAdd(90708, kSoundTypeAmbientMusic, "2204.wac", _configuration.backgroundMusic.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	puzzleAddAmbientSound(kPuzzleGeneralMenu, 90001, 100, 0, 1, 1, 10);
	soundAdd(90801, kSoundTypeDialog, "1124.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90802, kSoundTypeDialog, "2205.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90803, kSoundTypeDialog, "2206.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90804, kSoundTypeDialog, "2207.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90805, kSoundTypeDialog, "2208.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90806, kSoundTypeDialog, "2209.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90301, kSoundTypeDialog, "2210.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90302, kSoundTypeDialog, "2211.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90303, kSoundTypeDialog, "2212.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90304, kSoundTypeDialog, "2213.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90305, kSoundTypeDialog, "2214.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90306, kSoundTypeDialog, "2215.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90307, kSoundTypeDialog, "2216.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90308, kSoundTypeDialog, "2217.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90309, kSoundTypeDialog, "2218.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90401, kSoundTypeDialog, "2219.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90402, kSoundTypeDialog, "2220.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90403, kSoundTypeDialog, "2221.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90404, kSoundTypeDialog, "2222.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90405, kSoundTypeDialog, "2223.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90406, kSoundTypeDialog, "2224.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90407, kSoundTypeDialog, "2225.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90408, kSoundTypeDialog, "2226.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90409, kSoundTypeDialog, "2227.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90410, kSoundTypeDialog, "2228.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90411, kSoundTypeDialog, "2229.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90412, kSoundTypeDialog, "2230.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90501, kSoundTypeDialog, "2231.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90502, kSoundTypeDialog, "2232.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90503, kSoundTypeDialog, "2233.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90504, kSoundTypeDialog, "2234.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90505, kSoundTypeDialog, "2235.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90506, kSoundTypeDialog, "2236.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90507, kSoundTypeDialog, "2237.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90601, kSoundTypeDialog, "2238.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90602, kSoundTypeDialog, "2239.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90603, kSoundTypeDialog, "2240.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90604, kSoundTypeDialog, "2241.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90605, kSoundTypeDialog, "2242.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90606, kSoundTypeDialog, "2243.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90701, kSoundTypeDialog, "2244.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90702, kSoundTypeDialog, "2245.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90703, kSoundTypeDialog, "2246.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90704, kSoundTypeDialog, "2247.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90705, kSoundTypeDialog, "2248.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90706, kSoundTypeDialog, "2249.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90707, kSoundTypeDialog, "2250.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(90800, kSoundTypeDialog, "1125.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	varDefineByte(97001, 0);
	varDefineByte(98001, _progressState);
	varDefineByte(98002, 0);
	varDefineByte(97002, 0);
	varDefineByte(97006, 1);
	varDefineByte(97007, 0);
	varDefineByte(97008, 0);
	varDefineByte(97009, 0);
	varDefineByte(97010, 0);
	varDefineByte(97011, 0);
	varDefineByte(97012, 1);

	for (uint32 i = 0; i < 100; i++)
		varDefineByte(98090 + i, 0);

	for (uint32 i = 0; i < 18; i++)
		varDefineByte(99000 + i, 0);

	varDefineByte(98004, 0);
	varDefineByte(98003, 0);
	varDefineByte(98005, 0);
	varDefineByte(98006, 0);
	varDefineByte(98007, 0);
	varDefineByte(98008, 0);
	varDefineByte(98009, 0);
	varDefineByte(98010, 0);
	varDefineByte(98011, 0);
	varDefineByte(98087, 0);
	varDefineByte(98088, 0);
	varDefineWord(97013, 0);
	varDefineDword(90025, 0);
	varDefineDword(90028, 0);
	varDefineDword(90027, 0);
	varDefineDword(90026, 0);
	varDefineByte(98012, 0);
	varDefineByte(98013, 0);
	varDefineByte(98014, 0);
	varDefineByte(98015, 0);
	varDefineByte(98016, 0);
	varDefineByte(98089, 0);
}

void ApplicationFaust::initZone2() {
	setCurrentZone(kZone2);
	rotationAdd(11001, "1006", 0, 1);
	rotationAdd(11002, "1007", 0, 2);
	rotationAdd(11003, "1008", 0, 1);
	rotationAdd(11004, "1009", 0, 0);
	rotationAdd(11005, "1010", 0, 1);
	rotationAdd(11006, "1011", 0, 1);
	rotationAddMovabilityToRotation(11001, 11005, "1012", Common::Rect(2111, 111, 2450, 345), true, kCursorMove, 0);
	rotationSetMovabilityToRotation(11001, 0, 180.0f, 13.0f, 87.0f, 0.0f, 2, 180.0f, 0.3f, 87.0f);
	rotationAddMovabilityToRotation(11002, 11005, "1013", Common::Rect(3056, -45, 3533, 369), true, kCursorMove, 0);
	rotationSetMovabilityToRotation(11002, 0, 286.0f, 0.0f, 87.0f, 0.0f, 2, 286.0f, 0.3f, 87.0f);
	rotationAddMovabilityToRotation(11002, 11003, "1014", Common::Rect(1226, -15, 1501, 289), true, kCursorMove, 0);
	rotationSetMovabilityToRotation(11002, 1, 91.0f, 0.0f, 87.0f, 0.0f, 2, 90.0f, 26.0f, 87.0f);
	rotationAddMovabilityToRotation(11002, 11004, "1015", Common::Rect(1047, -15, 1255, 214), true, kCursorMove, 0);
	rotationSetMovabilityToRotation(11002, 2, 79.0f, 0.0f, 87.0f, 0.0f, 2, 0.0f, 0.3f, 87.0f);
	rotationAddMovabilityToRotation(11003, 11002, "1016", Common::Rect(3033, -26, 3232, 355), true, kCursorMove, 0);
	rotationSetMovabilityToRotation(11003, 0, 272.0f, 0.0f, 87.0f, 0.0f, 2, 272.0f, 0.0f, 87.0f);
	rotationAddMovabilityToRotation(11003, 11004, "1017", Common::Rect(399, -52, 875, 416), true, kCursorMove, 0);
	rotationSetMovabilityToRotation(11003, 1, 12.0f, 13.0f, 87.0f, 0.0f, 2, 0.0f, 0.3f, 87.0f);
	rotationAddMovabilityToRotation(11004, 11003, "1018", Common::Rect(2828, -99, 3190, 183), true, kCursorMove, 0);
	rotationSetMovabilityToRotation(11004, 0, 248.0f, 0.0f, 87.0f, 0.0f, 2, 248.0f, 0.3f, 87.0f);
	rotationAddMovabilityToRotation(11005, 11001, "1019", Common::Rect(318, -151, 569, 205), true, kCursorMove, 0);
	rotationSetMovabilityToRotation(11005, 0, 0.0f, 0.0f, 87.0f, 0.0f, 2, 0.0f, 0.3f, 87.0f);
	rotationAddMovabilityToRotation(11005, 11002, "1020", Common::Rect(1255, -31, 1476, 304), true, kCursorMove, 0);
	rotationSetMovabilityToRotation(11005, 1, 92.0f, 0.0f, 87.0f, 0.0f, 2, 90.0f, 0.0f, 87.0f);
	rotationAddMovabilityToRotation(11005, 11006, "1021", Common::Rect(3012, -95, 3331, 256), true, kCursorMove, 0);
	rotationSetMovabilityToRotation(11005, 2, 270.0f, 0.0f, 87.0f, 0.0f, 2, 270.0f, 0.3f, 87.0f);
	rotationAddMovabilityToRotation(11006, 11005, "1022", Common::Rect(1128, -71, 1455, 231), true, kCursorMove, 0);
	rotationSetMovabilityToRotation(11006, 0, 85.0f, 0.0f, 87.0f, 0.0f, 2, 90.0f, 0.3f, 87.0f);
	objectAdd(kObject11001, "", "", 1);
	objectAddRotationAccessibility(kObject11001, 11001, Common::Rect(103, -146, 856, 336), true, kCursorMove, 0);
	objectAdd(kObject11002, "", "", 1);
	objectAddRotationAccessibility(kObject11002, 11003, Common::Rect(1058, 155, 1745, 514), false, kCursorMove, 0);
	objectAdd(kObject11003, "", "", 1);
	objectAddRotationAccessibility(kObject11003, 11006, Common::Rect(2895, -254, 3343, 317), true, kCursorMove, 0);
	objectAdd(kObject11007, "", "", 1);
	objectAddRotationAccessibility(kObject11007, 11006, Common::Rect(2921, -260, 3332, 480), false, kCursorMove, 0);
	objectAdd(kObject11004, "", "", 1);
	objectAddRotationAccessibility(kObject11004, 11002, Common::Rect(2162, -27, 2341, 244), true, kCursorAction, 0);
	objectAddRotationAccessibility(kObject11004, 11002, Common::Rect(2095, -476, 2374, -291), true, kCursorAction, 1);
	objectAddRotationAccessibility(kObject11004, 11002, Common::Rect(1286, 6, 1416, 156), false, kCursorAction, 2);
	objectAddPresentation(kObject11004);
	objectPresentationAddImageToRotation(11004, 0, 11002, 0);
	objectAddPresentation(kObject11004);
	objectPresentationAddImageToRotation(11004, 1, 11002, 1);
	objectAddPresentation(kObject11004);
	objectPresentationAddImageToRotation(11004, 2, 11001, 0);
	objectAddPresentation(kObject11004);
	objectPresentationAddImageToRotation(11004, 3, 11003, 0);
	objectAddPresentation(kObject11004);
	objectPresentationAddImageToRotation(11004, 4, 11005, 0);
	objectAddPresentation(kObject11004);
	objectPresentationAddImageToRotation(11004, 5, 11006, 0);
	objectAdd(kObject11005, "", "", 1);
	objectAddRotationAccessibility(kObject11005, 11004, Common::Rect(2139, -289, 2346, -113), true, kCursorZoom, 0);
	soundAdd(11105, kSoundTypeEffect, "1024.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.backgroundMusic.soundChunck);
	soundAdd(11106, kSoundTypeEffect, "1025.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.backgroundMusic.soundChunck);
	soundAdd(11107, kSoundTypeEffect, "1026.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.backgroundMusic.soundChunck);
	soundAdd(11108, kSoundTypeEffect, "1027.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.backgroundMusic.soundChunck);
	soundAdd(11109, kSoundTypeEffect, "1028.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.backgroundMusic.soundChunck);
	soundAdd(11110, kSoundTypeEffect, "1029.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.backgroundMusic.soundChunck);
	soundAdd(11111, kSoundTypeEffect, "1030.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.backgroundMusic.soundChunck);
	soundAdd(11112, kSoundTypeEffect, "1031.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.backgroundMusic.soundChunck);
	soundAdd(11113, kSoundTypeEffect, "1032.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.backgroundMusic.soundChunck);
	soundAdd(11114, kSoundTypeEffect, "1033.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.backgroundMusic.soundChunck);
	soundAdd(11102, kSoundTypeAmbientEffect, "1034.was", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	rotationAdd3DSound(11001, 11102, 1, 1, 15, 100, 180.0f, 10);
	rotationAdd3DSound(11002, 11102, 1, 1, 15, 100, 180.0f, 10);
	rotationAdd3DSound(11003, 11102, 1, 1, 15, 100, 180.0f, 10);
	rotationAdd3DSound(11004, 11102, 1, 1, 15, 100, 180.0f, 10);
	rotationAdd3DSound(11005, 11102, 1, 1, 15, 100, 180.0f, 10);
	rotationAdd3DSound(11006, 11102, 1, 1, 15, 100, 180.0f, 10);
	rotationSet3DSoundOff(11001, 11102);
	rotationSet3DSoundOff(11002, 11102);
	rotationSet3DSoundOff(11003, 11102);
	rotationSet3DSoundOff(11004, 11102);
	rotationSet3DSoundOff(11005, 11102);
	rotationSet3DSoundOff(11006, 11102);
	soundAdd(11103, kSoundTypeAmbientEffect, "1035.was", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	rotationAdd3DSound(11001, 11103, 1, 1, 15, 100, 180.0f, 10);
	rotationAdd3DSound(11002, 11103, 1, 1, 15, 100, 180.0f, 10);
	rotationAdd3DSound(11003, 11103, 1, 1, 15, 100, 180.0f, 10);
	rotationAdd3DSound(11004, 11103, 1, 1, 15, 100, 180.0f, 10);
	rotationAdd3DSound(11005, 11103, 1, 1, 15, 100, 180.0f, 10);
	rotationAdd3DSound(11006, 11103, 1, 1, 15, 100, 180.0f, 10);
	rotationSet3DSoundOff(11001, 11103);
	rotationSet3DSoundOff(11002, 11103);
	rotationSet3DSoundOff(11003, 11103);
	rotationSet3DSoundOff(11004, 11103);
	rotationSet3DSoundOff(11005, 11103);
	rotationSet3DSoundOff(11006, 11103);
	soundAdd(11104, kSoundTypeAmbientEffect, "1036.was", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	rotationAdd3DSound(11001, 11104, 1, 1, 15, 100, 180.0f, 10);
	rotationAdd3DSound(11002, 11104, 1, 1, 15, 100, 180.0f, 10);
	rotationAdd3DSound(11003, 11104, 1, 1, 15, 100, 180.0f, 10);
	rotationAdd3DSound(11004, 11104, 1, 1, 15, 100, 180.0f, 10);
	rotationAdd3DSound(11005, 11104, 1, 1, 15, 100, 180.0f, 10);
	rotationAdd3DSound(11006, 11104, 1, 1, 15, 100, 180.0f, 10);
	rotationSet3DSoundOff(11001, 11104);
	rotationSet3DSoundOff(11002, 11104);
	rotationSet3DSoundOff(11003, 11104);
	rotationSet3DSoundOff(11004, 11104);
	rotationSet3DSoundOff(11005, 11104);
	rotationSet3DSoundOff(11006, 11104);
	soundAdd(11001, kSoundTypeDialog, "1037.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(11101, kSoundTypeAmbientMusic, "1023.was", _configuration.backgroundMusic.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	rotationAddAmbientSound(11001, 11101, 90, 0, 1, 1, 10);
	rotationAddAmbientSound(11002, 11101, 90, 0, 1, 1, 10);
	rotationAddAmbientSound(11003, 11101, 90, 0, 1, 1, 10);
	rotationAddAmbientSound(11004, 11101, 90, 0, 1, 1, 10);
	rotationAddAmbientSound(11005, 11101, 90, 0, 1, 1, 10);
	rotationAddAmbientSound(11006, 11101, 90, 0, 1, 1, 10);
	varDefineByte(11001, 0);
	varDefineByte(11002, 0);
	varDefineByte(11003, 0);
}

void ApplicationFaust::initZone3() {
	warning("[ApplicationFaust::initZone3] Not implemented!");
}

void ApplicationFaust::initZone4() {
	warning("[ApplicationFaust::initZone4] Not implemented!");
}

void ApplicationFaust::initZone5() {
	warning("[ApplicationFaust::initZone5] Not implemented!");
}

void ApplicationFaust::initZone6() {
	warning("[ApplicationFaust::initZone6] Not implemented!");
}

void ApplicationFaust::initZone7() {
	setCurrentZone(kZone7);
	puzzleAdd(kPuzzle49000);
	puzzleAddBackgroundImage(kPuzzle49000, "a04s01n02p01.bmp", Common::Point(0, 16), true);
	puzzleAdd(kPuzzle49001);
	puzzleAddBackgroundImage(kPuzzle49001, "a04s02n01p02.bmp", Common::Point(0, 16), true);
	rotationAdd(41000, "1376", 0, 2);
	rotationAdd(41001, "1377", 0, 2);
	rotationAdd(41002, "1378", 0, 1);
	rotationAddMovabilityToRotation(41000, 41001, "", Common::Rect(3533, -150, 3600, 52), true, kCursorMove, 0);
	rotationSetMovabilityToRotation(41000, 0, 330.0f, 0.3f, 87.0f, 0.0f, 2, 0.0f, 0.3f, 87.0f);
	rotationAddMovabilityToRotation(41000, 41001, "", Common::Rect(0, -150, 138, 52), true, kCursorMove, 0);
	rotationSetMovabilityToRotation(41000, 1, 330.0f, 0.3f, 87.0f, 0.0f, 2, 0.0f, 0.3f, 87.0f);
	objectAdd(kObject49000, "", "", 1);
	objectAddRotationAccessibility(kObject49000, 41001, Common::Rect(316, 5, 611, 318), true, kCursorAction, 0);
	objectAddPresentation(kObject49000);
	objectPresentationAddAnimationToPuzzle(kObject49000, 0, kPuzzle49000, "a04s01n02a01", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 100, 12.5f, 6);
	objectAddPresentation(kObject49000);
	objectPresentationAddAnimationToPuzzle(kObject49000, 1, kPuzzle49000, "a04s02n01p01s01", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 70, 12.5f, 6);
	objectAddPresentation(kObject49000);
	objectPresentationAddAnimationToPuzzle(kObject49000, 2, kPuzzle49000, "a04s02n01p01s02", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 23, 15.0f, 6);
	objectAddPresentation(kObject49000);
	objectPresentationAddAnimationToPuzzle(kObject49000, 3, kPuzzle49000, "a04s02n01p01s03", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 17, 12.5f, 6);
	objectAddPresentation(kObject49000);
	objectPresentationAddAnimationToPuzzle(kObject49000, 4, kPuzzle49000, "a04s02n01p01s04", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 15, 12.5f, 6);
	objectAddPresentation(kObject49000);
	objectPresentationAddAnimationToPuzzle(kObject49000, 5, kPuzzle49000, "a04s02n01p01s05", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 63, 12.5f, 6);
	objectAddPresentation(kObject49000);
	objectPresentationAddAnimationToPuzzle(kObject49000, 6, kPuzzle49000, "a04s02n01p01s06", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 58, 12.5f, 6);
	objectAddPresentation(kObject49000);
	objectPresentationAddAnimationToPuzzle(kObject49000, 7, kPuzzle49000, "a04s02n01p01s07", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 70, 12.5f, 6);
	objectAddPresentation(kObject49000);
	objectPresentationAddAnimationToPuzzle(kObject49000, 8, kPuzzle49000, "a04s02n01p01s08", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 60, 12.5f, 6);
	objectAddPresentation(kObject49000);
	objectPresentationAddAnimationToPuzzle(kObject49000, 9, kPuzzle49000, "a04s02n01p01s09", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 70, 12.5f, 6);
	objectAddPresentation(kObject49000);
	objectPresentationAddAnimationToPuzzle(kObject49000, 10, kPuzzle49000, "a04s02n01p01s10", kImageTypeBMP, Common::Point(0, 16), kDrawType1, 1000, 60, 12.5f, 6);
	objectAdd(kObject49003, "", "", 1);
	objectAddRotationAccessibility(kObject49003, 41002, Common::Rect(704, 187, 847, 317), true, kCursorZoom, 0);
	objectAddPuzzleAccessibility(kObject49003, kPuzzle49001, Common::Rect(278, 280, 396, 330), true, kCursorZoom, 1);
	objectAddPuzzleAccessibility(kObject49003, kPuzzle49001, Common::Rect(0,   420, 640, 464), true, kCursorBack, 9);
	objectAddPresentation(kObject49003);
	objectPresentationAddImageToPuzzle(kObject49003, 0, kPuzzle49001, "a04s02n01p02l01.tga", Common::Point(0, 16), true, kDrawType3, 1000);
	objectPresentationAddImageToRotation(49003, 0, 41002, 0);
	objectAdd(kObject49002, "", "", 1);
	objectAddPresentation(kObject49002);
	objectPresentationAddAnimationToRotation(kObject49002, 0, 41000, 0, 40, 12.5f, 5);
	objectPresentationAddAnimationToRotation(kObject49002, 0, 41000, 1, 40, 12.5f, 5);
	objectPresentationAddAnimationToRotation(kObject49002, 0, 41001, 0, 40, 12.5f, 5);
	objectPresentationAddAnimationToRotation(kObject49002, 0, 41001, 1, 40, 12.5f, 5);
	objectPresentationShow(kObject49002);
	objectAdd(kObject19, "", "", 1);
	objectAddPuzzleAccessibility(kObject19, kPuzzle49000, Common::Rect(0, 0, 640, 480), false, kCursorMenuIdle, 1);
	objectAddPuzzleAccessibility(kObject19, kPuzzleMenu,  Common::Rect(0, 0, 640, 480), false, kCursorMenuIdle, 1);
	varDefineByte(49000, 0);
	varDefineByte(49001, 0);
	varDefineByte(49002, 0);
	varDefineByte(49003, 0);
	varDefineByte(49004, 0);
	soundAdd(41001, kSoundTypeBackgroundMusic, "1379.wac", _configuration.backgroundMusic.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundSetVolume(41001, 90);
	soundAdd(41002, kSoundTypeAmbientMusic, "1380.was", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(41003, kSoundTypeAmbientMusic, "1381.was", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(41004, kSoundTypeAmbientMusic, "1382.was", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(41005, kSoundTypeAmbientMusic, "1383.was", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(41006, kSoundTypeAmbientMusic, "1384.was", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(41007, kSoundTypeAmbientMusic, "1385.was", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(41008, kSoundTypeAmbientMusic, "1386.was", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(41009, kSoundTypeAmbientMusic, "1387.was", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(41011, kSoundTypeAmbientMusic, "1388.was", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(41012, kSoundTypeAmbientMusic, "1389.was", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(41013, kSoundTypeAmbientMusic, "1390.was", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundAdd(41000, kSoundTypeDialog, "1391.wac", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
}

void ApplicationFaust::initZone8() {
	warning("[ApplicationFaust::initZone8] Not implemented!");
}

void ApplicationFaust::initZone9() {
	warning("[ApplicationFaust::initZone9] Not implemented!");
}

void ApplicationFaust::initZone10() {
	warning("[ApplicationFaust::initZone10] Not implemented!");
}

void ApplicationFaust::initZone11() {
	setCurrentZone(kZone11);
	rotationAdd(72014, "1555", 0, 6);
	objectAdd(kObject72000, "", "", 1);
	objectAddRotationAccessibility(kObject72000, 72014, Common::Rect(174,  92,   230, 158), true, kCursorShoot, 0);
	objectAddRotationAccessibility(kObject72000, 72014, Common::Rect(567,  134,  638, 204), true, kCursorShoot, 1);
	objectAddRotationAccessibility(kObject72000, 72014, Common::Rect(1355, 129, 1423, 204), true, kCursorShoot, 2);
	objectAddRotationAccessibility(kObject72000, 72014, Common::Rect(2341, 150, 2408, 230), true, kCursorShoot, 3);
	objectAddRotationAccessibility(kObject72000, 72014, Common::Rect(2408, 68,  2459, 115), true, kCursorShoot, 4);
	objectAddRotationAccessibility(kObject72000, 72014, Common::Rect(3014, 132, 3090, 212), true, kCursorShoot, 5);
	objectAddPresentation(kObject72000);
	objectPresentationAddAnimationToRotation(kObject72000, 0, 72014, 0, 31, 12.5f, 6);
	objectPresentationSetAnimationOnRotation(kObject72000, 0, 0, kObject72000);
	objectAddPresentation(kObject72000);
	objectPresentationAddAnimationToRotation(kObject72000, 1, 72014, 1, 31, 12.5f, 6);
	objectPresentationSetAnimationOnRotation(kObject72000, 1, 0, kObject72001);
	objectAddPresentation(kObject72000);
	objectPresentationAddAnimationToRotation(kObject72000, 2, 72014, 2, 31, 12.5f, 6);
	objectPresentationSetAnimationOnRotation(kObject72000, 2, 0, kObject72002);
	objectAddPresentation(kObject72000);
	objectPresentationAddAnimationToRotation(kObject72000, 3, 72014, 3, 31, 12.5f, 6);
	objectPresentationSetAnimationOnRotation(kObject72000, 3, 0, kObject72003);
	objectAddPresentation(kObject72000);
	objectPresentationAddAnimationToRotation(kObject72000, 4, 72014, 4, 31, 12.5f, 6);
	objectPresentationSetAnimationOnRotation(kObject72000, 4, 0, kObject72004);
	objectAddPresentation(kObject72000);
	objectPresentationAddAnimationToRotation(kObject72000, 5, 72014, 5, 31, 12.5f, 6);
	objectPresentationSetAnimationOnRotation(kObject72000, 5, 0, kObject72005);
	objectAdd(kObject72001, "", "", 1);
	objectAddRotationAccessibility(kObject72001, 72014, Common::Rect(0, -600, 3600, 600), true, kCursorShoot, 0);
	soundAdd(72000, kSoundTypeEffect, "1556.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.backgroundMusic.soundChunck);
	soundAdd(72001, kSoundTypeEffect, "1557.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.backgroundMusic.soundChunck);
	soundAdd(72002, kSoundTypeEffect, "1558.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.backgroundMusic.soundChunck);
	soundAdd(72003, kSoundTypeEffect, "1559.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.backgroundMusic.soundChunck);
	soundAdd(72004, kSoundTypeEffect, "1560.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.backgroundMusic.soundChunck);
	soundAdd(72005, kSoundTypeEffect, "1561.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.backgroundMusic.soundChunck);
	soundAdd(72012, kSoundTypeEffect, "1562.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.backgroundMusic.soundChunck);
	soundAdd(72006, kSoundTypeAmbientEffect, "1563.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.ambientEffect.soundChunck);
	rotationAdd3DSound(72014, 72006, 1, 1, 25, 88, 335.0f, 10);
	rotationSet3DSoundOff(72014, 72006);
	soundAdd(72007, kSoundTypeAmbientEffect, "1563.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.ambientEffect.soundChunck);
	rotationAdd3DSound(72014, 72007, 1, 1, 25, 90, 15.0f, 10);
	rotationSet3DSoundOff(72014, 72007);
	soundAdd(72008, kSoundTypeAmbientEffect, "1563.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.ambientEffect.soundChunck);
	rotationAdd3DSound(72014, 72008, 1, 1, 25, 92, 96.0f, 10);
	rotationSet3DSoundOff(72014, 72008);
	soundAdd(72009, kSoundTypeAmbientEffect, "1563.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.ambientEffect.soundChunck);
	rotationAdd3DSound(72014, 72009, 1, 1, 25, 95, 199.0f, 10);
	rotationSet3DSoundOff(72014, 72009);
	soundAdd(72010, kSoundTypeAmbientEffect, "1563.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.ambientEffect.soundChunck);
	rotationAdd3DSound(72014, 72010, 1, 1, 25, 83, 195.0f, 10);
	rotationSet3DSoundOff(72014, 72010);
	soundAdd(72011, kSoundTypeAmbientEffect, "1563.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.ambientEffect.soundChunck);
	rotationAdd3DSound(72014, 72011, 1, 1, 25, 93, 260.0f, 10);
	rotationSet3DSoundOff(72014, 72011);
	soundAdd(72014, kSoundTypeAmbientEffect, "1564.was", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	rotationAdd3DSound(72014, 72014, 1, 1, 15, 100, 0.0f, 10);
	soundAdd(72015, kSoundTypeAmbientEffect, "1565.was", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	rotationAdd3DSound(72014, 72015, 1, 1, 15, 100, 90.0f, 10);
	soundAdd(72016, kSoundTypeAmbientEffect, "1566.was", _configuration.ambientEffect.loadFrom, 2, _configuration.ambientEffect.soundChunck);
	rotationAdd3DSound(72014, 72016, 1, 1, 15, 100, 270.0f, 10);
	soundAdd(72013, kSoundTypeBackgroundMusic, "1567.was", _configuration.backgroundMusic.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	soundSetVolume(72013, 90);
	varDefineByte(72000, 0);
	varDefineByte(72001, 0);
	varDefineByte(72002, 0);
	varDefineByte(72003, 0);
	varDefineByte(72004, 0);
	varDefineByte(72005, 0);
	varDefineByte(72006, 0);
	varDefineByte(72007, 15);
	varDefineWord(72000, 270);
}

void ApplicationFaust::initZone12() {
	warning("[ApplicationFaust::initZone12] Not implemented!");
}

void ApplicationFaust::initZone13() {
	warning("[ApplicationFaust::initZone13] Not implemented!");
}

void ApplicationFaust::initZone14() {
	warning("[ApplicationFaust::initZone14] Not implemented!");
}

void ApplicationFaust::initZone15() {
	warning("[ApplicationFaust::initZone15] Not implemented!");
}

void ApplicationFaust::initZone16() {
	warning("[ApplicationFaust::initZone16] Not implemented!");
}

void ApplicationFaust::initZone17() {
	setCurrentZone(kZone17);
	rotationAdd(141101, "2178", 0, 0);
	rotationAdd(141102, "2179", 0, 1);
	rotationAdd(141201, "2180", 0, 0);
	rotationAdd(141202, "2181", 0, 2);
	rotationAddMovabilityToRotation(141101, 141102, "", Common::Rect(3127, -68, 3396, 143), true, kCursorMove, 0);
	rotationSetMovabilityToRotation(141101, 0, 280.0f, 0.0f, 87.0f, 0.0f, 2, 270.0f, 6.0f, 87.0f);
	rotationAddMovabilityToRotation(141102, 141201, "", Common::Rect(2626, -75, 2868, 368), false, kCursorMove, 0);
	rotationSetMovabilityToRotation(141102, 0, 230.0f, 0.0f, 87.0f, 0.0f, 2, 250.0f, 0.0f, 87.0f);
	rotationAddMovabilityToRotation(141102, 141101, "", Common::Rect(1335, -101, 1687, 233), true, kCursorMove, 0);
	rotationSetMovabilityToRotation(141102, 1, 100.0f, 0.0f, 87.0f, 0.0f, 2, 100.0f, 0.0f, 87.0f);
	rotationAddMovabilityToRotation(141201, 141202, "", Common::Rect(2831, -233, 3074, 301), true, kCursorMove, 0);
	rotationSetMovabilityToRotation(141201, 0, 250.0f, 0.0f, 87.0f, 0.0f, 2, 250.0f, 0.0f, 87.0f);
	rotationAddMovabilityToRotation(141201, 141102, "", Common::Rect(1030, -143, 1877, 550), true, kCursorMove, 0);
	rotationSetMovabilityToRotation(141201, 1, 100.0f, 0.0f, 87.0f, 0.0f, 2, 100.0f, 0.0f, 87.0f);
	objectAdd(kObject142003, "", "", 1);
	objectAddRotationAccessibility(kObject142003, 141202, Common::Rect(1204, -186, 1771, 550), true, kCursorMove, 0);
	objectAdd(kObject141001, "", "", 1);
	objectAddRotationAccessibility(kObject141001, 141102, Common::Rect(3091, -34, 3418, 380), true, kCursorAction, 0);
	objectAddRotationAccessibility(kObject141001, 141102, Common::Rect(2997, 357, 3127, 438), true, kCursorTake, 1);
	objectAddPresentation(kObject141001);
	objectPresentationAddImageToRotation(141001, 0, 141102, 0);
	objectAdd(kObject142002, "", "", 1);
	objectAddRotationAccessibility(kObject142002, 141202, Common::Rect(3561, 22, 3600, 291), false, kCursorAction, 0);
	objectAddRotationAccessibility(kObject142002, 141202, Common::Rect(0, 22, 277, 291), false, kCursorAction, 0);
	objectAdd(kObjectMovieReel, "Movie reel", "a06_Reel", 9);
	objectSetActiveCursor(kObjectMovieReel, Common::Point(20, 20), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	objectSetPassiveCursor(kObjectMovieReel, Common::Point(20, 20), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	objectAddRotationAccessibility(kObjectMovieReel, 141202, Common::Rect(548, 115, 710, 173), true, kCursorTake, 0);
	objectAddRotationAccessibility(kObjectMovieReel, 141202, Common::Rect(3561, 22, 3600, 291), true, kCursorActionObject, 1);
	objectAddRotationAccessibility(kObjectMovieReel, 141202, Common::Rect(0, 22, 277, 291), true, kCursorActionObject, 1);
	objectAddPresentation(kObjectMovieReel);
	objectPresentationAddImageToRotation(142001, 0, 141202, 0);
	objectAddPresentation(kObjectMovieReel);
	objectPresentationAddImageToRotation(142001, 1, 141202, 1);
	objectPresentationShow(kObjectMovieReel, 0);
	soundAdd(142005, kSoundTypeAmbientEffect, "1326.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.ambientEffect.soundChunck);
	soundAdd(141001, kSoundTypeAmbientMusic, "1792.was", _configuration.ambientEffect.loadFrom, 2, _configuration.backgroundMusic.soundChunck);
	rotationAddAmbientSound(141101, 141001, 90, 0, 1, 1, 10);
	rotationAddAmbientSound(141102, 141001, 90, 0, 1, 1, 10);
	soundAdd(142002, kSoundTypeAmbientEffect, "1850.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.backgroundMusic.soundChunck);
	soundAdd(142003, kSoundTypeAmbientEffect, "1853.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.backgroundMusic.soundChunck);
	soundAdd(142004, kSoundTypeAmbientEffect, "1860.wav", _configuration.ambientEffect.loadFrom, 1, _configuration.backgroundMusic.soundChunck);
	soundAdd(143001, kSoundTypeDialog, "1339.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(143002, kSoundTypeDialog, "1340.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
	soundAdd(143003, kSoundTypeDialog, "1341.wav", _configuration.dialog.loadFrom, 2, _configuration.dialog.soundChunck);
}


#pragma endregion

} // End of namespace Ring
