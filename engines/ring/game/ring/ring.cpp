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

#include "ring/game/ring/ring.h"

#include "ring/base/art.h"
#include "ring/base/bag.h"
#include "ring/base/cursor.h"
#include "ring/base/dialog.h"
#include "ring/base/preferences.h"
#include "ring/base/puzzle.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"

#include "ring/game/ring/event.h"
#include "ring/game/ring/shared.h"
#include "ring/game/ring/visual.h"

#include "ring/graphics/image.h"
#include "ring/graphics/screen.h"

#include "ring/sound/soundhandler.h"
#include "ring/sound/soundmanager.h"

#include "ring/ring.h"

#include "common/archive.h"
#include "common/textconsole.h"

using namespace RingGame;

namespace Ring {

#pragma region Screen lists

// List of intro screens
static const struct {
	Common::String filenameFrom;
	Common::String filenameTo;
	uint32 a3;
	uint32 ticksWait;
	LoadFrom loadFrom;
	ArchiveType archiveType;
} introScreens[11] = {
	{"beg0.bmp", "beg1.bmp", 20, 3000, kLoadFromDisk, kArchiveFile},
	{"beg1.bmp", "beg0.bmp", 20,    0, kLoadFromDisk, kArchiveFile},
	{"beg0.bmp", "beg2.bmp", 20, 3000, kLoadFromDisk, kArchiveFile},
	{"beg2.bmp", "beg0.bmp", 20,    0, kLoadFromDisk, kArchiveFile},
	{"beg0.bmp", "beg3.bmp", 20, 3000, kLoadFromDisk, kArchiveFile},
	{"beg3.bmp", "beg0.bmp", 20,    0, kLoadFromDisk, kArchiveFile},
	{"beg0.bmp", "beg4.bmp", 20, 3000, kLoadFromDisk, kArchiveFile},
	{"beg4.bmp", "beg0.bmp", 20,    0, kLoadFromDisk, kArchiveFile},
	{"beg0.bmp", "beg5.bmp", 20, 3000, kLoadFromDisk, kArchiveFile},
	{"beg5.bmp", "beg0.bmp", 20,    0, kLoadFromDisk, kArchiveFile},
	{"beg0.bmp", "beg6.bmp", 20, 6000, kLoadFromDisk, kArchiveFile}
};

// List of credits screens
static const struct {
	Common::String filename;
	uint32 ticksWait;
	LoadFrom loadFrom;
	ArchiveType archiveType;
} creditsScreens[11] = {
	{"cre_01.bmp",    0, kLoadFromDisk, kArchiveFile},
	{"cre_02.bmp",    0, kLoadFromDisk, kArchiveFile},
	{"cre_03.bmp",    0, kLoadFromDisk, kArchiveFile},
	{"cre_04.bmp",    0, kLoadFromDisk, kArchiveFile},
	{"cre_05.bmp",    0, kLoadFromDisk, kArchiveFile},
	{"cre_06.bmp",    0, kLoadFromDisk, kArchiveFile},
	{"cre_07.bmp",    0, kLoadFromDisk, kArchiveFile},
	{"cre_08.bmp",    0, kLoadFromDisk, kArchiveFile},
	{"cre_09.bmp",    0, kLoadFromDisk, kArchiveFile},
	{"cre_10.bmp",    0, kLoadFromDisk, kArchiveFile},
	{"cre_11.bmp", 5000, kLoadFromDisk, kArchiveFile}
};

#pragma endregion

ApplicationRing::ApplicationRing(RingEngine *engine) : Application(engine) {
	_eventHandler = new EventHandlerRing(this);
}

ApplicationRing::~ApplicationRing() {
	// the event handler is deleted by the base class
}

#pragma region Game setup

void ApplicationRing::initLanguages() {
	languageAdd(kLanguageEnglish, "ENG", "ENG", 1);
	languageAdd(kLanguageFrench,  "FRA", "FRA", 2);
	languageAdd(kLanguageGerman,  "GER", "GER", 3);
	languageAdd(kLanguageItalian, "ITA", "ITA", 1);
	languageAdd(kLanguageSpanish, "SPA", "SPA", 2);
	languageAdd(kLanguageSwedish, "SWE", "SWE", 1);
	languageAdd(kLanguageDutch,   "HOL", "HOL", 3);
	languageAdd(kLanguageHebrew,  "HEB", "HEB", 1);
	languageAdd(kLanguageGreek,   "GRE", "GRE", 1);
	languageAdd(kLanguageSlovak,  "SLO", "SLO", 1);
}

void ApplicationRing::initFont() {
	switch (getCurrentLanguage()) {
	default:
		fontAdd(kFontDefault, "arxrin.fon", "ARX Pilgrim L", 12, true, false, false, false, getCurrentLanguage());
		break;

	case kLanguageSlovak:
		fontAdd(kFontDefault, "arxrin.slo", "ARX Pilgrim L", 12, true, false, false, false, getCurrentLanguage());
		break;

	case kLanguageHebrew:
		fontAdd(kFontDefault, "arxrin.heb", "ArxelHebrew", 12, true, false, false, false, getCurrentLanguage());
		break;

	case kLanguageGreek:
		fontAdd(kFontDefault, "arxrin.gre", "Arial", 16, true, false, false, false, getCurrentLanguage());
		break;
	}
}

void ApplicationRing::setup() {
	// Add the list of zones
	addZone(kZoneNI, "Alberich",   "NI", _configuration.artNI ? kArchiveArt : kArchiveFile, kLoadFromDisk);
	addZone(kZoneRH, "Alberich",   "RH", _configuration.artRH ? kArchiveArt : kArchiveFile, kLoadFromDisk);
	addZone(kZoneFO, "Siegmund",   "FO", _configuration.artFO ? kArchiveArt : kArchiveFile, kLoadFromDisk);
	addZone(kZoneRO, "Loge",       "RO", _configuration.artRO ? kArchiveArt : kArchiveFile, kLoadFromDisk);
	addZone(kZoneWA, "Brnnnhilde", "WA", _configuration.artWA ? kArchiveArt : kArchiveFile, kLoadFromDisk);
	addZone(kZoneAS, "Dril",       "AS", _configuration.artAS ? kArchiveArt : kArchiveFile, kLoadFromDisk);
	addZone(kZoneN2, "Loge",       "N2", _configuration.artN2 ? kArchiveArt : kArchiveFile, kLoadFromDisk);

	// Setup system zone
	setupZone(kZoneSY, kSetupTypeNone);

	// Setup cursors
	ArchiveType archiveType = (_configuration.artCURSOR ? kArchiveArt : kArchiveFile);

	cursorAdd(kCursorDefault,       "",                kCursorTypeNormal,   1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorBusy,          "cur_busy",        kCursorTypeImage,    1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorHandSelection, "ni_handsel",      kCursorTypeImage,    1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorIdle,          "cur_idle",        kCursorTypeAnimated, 1, 15, 12.5f, 4, kLoadFromCursor, archiveType);
	cursorAdd(kCursorMove,          "cur_muv",         kCursorTypeAnimated, 1, 20, 12.5f, 4, kLoadFromCursor, archiveType);
	cursorAdd(kCursorHotspot,       "cur_hotspot",     kCursorTypeAnimated, 1, 19, 12.5f, 4, kLoadFromCursor, archiveType);
	cursorAdd(kCursorBack,          "cur_back",        kCursorTypeImage,    1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorMenuIdle,      "cur_menuidle",    kCursorTypeNormal,   1, kLoadFromCursor, archiveType);
	cursorAdd(kCursorMenuActive,    "cur_menuactive",  kCursorTypeImage,    1, kLoadFromCursor, archiveType);

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

void ApplicationRing::initBag() {
	_bag->setOrigin(Common::Point(0, 0));
	_bag->sub_417D40(18, 42, 44, 100);
	_bag->setBackgroundOffset(Common::Point(0, 0));
	_bag->sub_417DD0(6);
	_bag->sub_417D80(0, 24, 30, 448);
	_bag->sub_417DA0(610, 24, 30, 448);
	_bag->sub_4192A0(7, 48);
	_bag->sub_4192C0(627, 48);
	_bag->sub_417DE0(335, 8);
	_bag->sub_419280(500);
	_bag->loadBackground("bagbgr.tga", "", "bagarr.tga", "", "", "bagarr.tga", "", "menu_gur.tga", _archiveType, true);
	_bag->initHotspots();
}

#pragma endregion

#pragma region Startup

void ApplicationRing::showStartupScreen() {
	setZoneAndEnableBag(kZoneSY);
	playMovie("logo");

	for (uint i = 0; i < ARRAYSIZE(introScreens); i++) {

		displayFade(introScreens[i].filenameFrom, introScreens[i].filenameTo, introScreens[i].a3, introScreens[i].ticksWait, introScreens[i].loadFrom, introScreens[i].archiveType);

		// Skip intro screens if ESCAPE is pressed
		if (checkEscape())
			return;
	}
}

void ApplicationRing::startMenu(bool savegame) {
	if (_currentGameZone)
		return;

	if (savegame) {
		cursorSelect(kCursorBusy);
		_vm->setFlag(false);
		_vm->update();
		_bag->reset();

		if (!_saveManager->loadSave(0, kLoadSaveWrite))
			error("[ApplicationRing::startMenu] Cannot save game in slot 0");

		// Save a copy of the screen surface for savegame
		SAFE_DELETE(_thumbnail);

		_thumbnail = new Image();
		_thumbnail->create(24, 2, 640, 480);

		// Save a copy of the screen to our image
		_screenManager->copySurface(_thumbnail, 0, 0);
	}

	_currentGameZone = getCurrentZone();

	soundStopAll(4);
	setZoneAndEnableBag(kZoneSY);
	puzzleSetActive(kPuzzleGeneralMenu, true, true);
	puzzleSetMod(kPuzzleMenu, 1, 0);

	for (uint32 i = 1; i < 8; i++) {
		objectSetAccessibilityOff((ObjectId)i);
		objectPresentationHideAndRemove((ObjectId)i);
	}

	Bag *bag = getBag();
	if (bag && bag->isInitialized())
		bag->reset();

	cursorDelete();

	if (savegame) {
		objectSetAccessibilityOn(kObjectMenuContinue);
		objectSetAccessibilityOn(kObjectMenuSave);
	} else {
		objectSetAccessibilityOff(kObjectMenuContinue);
		objectSetAccessibilityOff(kObjectMenuSave);
	}
}

void ApplicationRing::showMenu(ZoneId zone, MenuAction menuAction) {
	ArchiveType archiveType = _configuration.artSY ? kArchiveArt : kArchiveFile;

	switch (zone) {
	default:
		break;

	case kZoneNI:
	case kZoneRH:
		setZoneAndEnableBag(kZoneSY);

		switch (menuAction){
		default:
			break;

		case kMenuAction1:
		case kMenuAction2:
		case kMenuAction3:
		case kMenuAction4:
			showImage("End.bmp", Common::Point(0, 16), 4000, kLoadFromDisk, archiveType);
			break;
		}

		startMenu(false);
		break;

	case kZoneN2:
		setZoneAndEnableBag(kZoneSY);

		switch (menuAction){
		default:
			break;

		case kMenuAction1:
		case kMenuAction2:
			showImage("End.bmp", Common::Point(0, 16), 4000, kLoadFromDisk, archiveType);
			break;
		}

		startMenu(false);
		break;

	case kZoneAS:
		if (menuAction == kMenuAction0) {
			setZoneAndEnableBag(kZoneSY);
			puzzleSetActive(kPuzzleMenu);
			objectPresentationShow(kObject7, 0);
			soundPlay(90001);
		}
		break;
	}
}

void ApplicationRing::showCredits() {
	soundStopAll(1024);
	setZoneAndEnableBag(kZoneWA);
	soundPlay(51002, kSoundLoop);

	setZoneAndEnableBag(kZoneSY);

	// Scroll credits
	for (uint i = 0; i < ARRAYSIZE(creditsScreens); i++) {
		if (scrollImage(creditsScreens[i].filename, creditsScreens[i].ticksWait, creditsScreens[i].loadFrom, creditsScreens[i].archiveType))
			break;
	}

	soundStop(51002, 1024);
}

void ApplicationRing::loadPreferences() {
	if (!_preferenceHandler)
		error("[] Preference handler not initialized properly");

	_preferenceHandler->load();

	setupZone(kZoneAS, kSetupType999);
}

#pragma endregion

#pragma region Drawing

void ApplicationRing::draw() {
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
		exitZone();
		initZones();
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

#pragma region Message

void ApplicationRing::messageInsertCd(ZoneId zone) {
	setZoneAndEnableBag(kZoneSY);
	objectPresentationSetTextToPuzzle(kObject90912, 1, 0, _messageType);
	objectPresentationSetTextCoordinatesToPuzzle(kObject90912, 1, 0, Common::Point(225, 193));
	objectPresentationSetTextToPuzzle(kObject90912, 1, 1, _message);
	objectPresentationSetTextCoordinatesToPuzzle(kObject90912, 1, 1, Common::Point(225, 213));
	objectPresentationShow(kObject90912, 0);
	objectSetAccessibilityOff(kObject90912);
	objectSetAccessibilityOn(kObject90912, zone, zone);
	puzzleSetActive(kPuzzleInsertCd);
}

#pragma endregion

#pragma region Zone setup

void ApplicationRing::setupZone(ZoneId zone, SetupType type) {
	debugC(kRingDebugLogic, "Setting up zone %s", getZoneName(zone).c_str());

	// Check saved data for zone and/or puzzle id
	bool hasData = isDataPresent(type);
	if (!hasData) {
		if (zone == kZoneSY) {
			hasData = true;
		} else {
			// The original checks for the correct CD,
			// we should instead check that the zone folder
			// has been copied properly
			warning("[ApplicationRing::setupZone] Zone CD check not implemented");
		}
	}

	reset();
	soundStopAll(8);

	if (_soundHandler)
		_soundHandler->reset();

	if (zone != kZoneSY)
		_artHandler->reset();

	if (hasData) {
		setZoneAndEnableBag(zone);
		setZone(zone, type);
	} else {
		_saveManager->setSetupType(type);
		messageFormat("InsertCD", Common::String::format("%d", getCdForZone(getCurrentZone())));
		messageInsertCd(zone);
	}
}

bool ApplicationRing::isDataPresent(SetupType type) {
	if (type != kSetupTypeLoading || getCurrentZone() != kZoneAS)
		return false;

	if (_saveManager->getData()->hasCurrentRotation) {
		if (_saveManager->getData()->rotationId == 80101)
			return true;
		else
			return false;
	}

	PuzzleId id = _saveManager->getData()->puzzleId;
	if (id == (Id)kPuzzle80002
	 || id == (Id)kPuzzle80003
	 || id == (Id)kPuzzle80004
	 || id == (Id)kPuzzle80005
	 || id == (Id)kPuzzle80006
	 || id == (Id)kPuzzle80007
	 || id == (Id)kPuzzle80008
	 || id == (Id)kPuzzle80009
	 || id == (Id)kPuzzle80010)
		return true;

	return false;
}

uint32 ApplicationRing::getCdForZone(ZoneId zone) const {
	if (!_configuration.checkCD)
		return 1;

	switch (zone) {
	default:
		break;

	case kZoneSY:
		return 0;

	case kZoneAS:
		return 1;

	case kZoneNI:
		return 2;

	case kZoneRH:
		return 3;

	case kZoneFO:
		return 5;

	case kZoneRO:
	case kZoneN2:
		return 4;

	case kZoneWA:
		return 6;
	}

	error("[ApplicationRing::getCdForZone] Invalid zone (%d)", zone);
}

void ApplicationRing::setZoneAndEnableBag(ZoneId zone) {
	setCurrentZone(zone);

	// Enable or disable bag
	if (zone == kZoneSY || zone == kZoneAS)
		getBag()->disable();
	else
		getBag()->enable();
}

void ApplicationRing::setZone(ZoneId zone, SetupType type) {
	debugC(kRingDebugLogic, "Setting zone %s", getZoneFolder(zone).c_str());

	bool hasData = isDataPresent(type);
	if (zone != kZoneSY && !hasData) {
		if (getArchiveType(zone) == kArchiveArt) {
			if (!getArtHandler())
				error("[ApplicationRing::setZone] Art handler is not initialized properly");

			getArtHandler()->open(zone, kLoadFromCd);
		}
	}

	// Finish loading savegame
	if (type == kSetupTypeLoading) {
		SaveManager::SavegameData *data = getSaveManager()->getData();

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

		return;
	}

	// Setup zone
	_eventHandler->onSetup(zone, type);
}

#pragma endregion

#pragma region Visual

Visual *ApplicationRing::createVisual(Id visualId, uint32 a3, uint32 a4, uint32 left, uint32 top, uint32 offsetY, uint32 height, uint32 progressMultiplier, uint32 progressColor) {
	VisualElementRing *visual = new VisualElementRing(visualId);

	visual->init(a3, a4, left, top, offsetY, height, progressMultiplier, progressColor);
	visual->setField8(2);
	visual->setVisible(true);

	return visual;
}

#pragma endregion

#pragma region Zone initialization

void ApplicationRing::initZones() {
	debugC(kRingDebugLogic, "Init zone data");

	_loadFrom = kLoadFromDisk;

	drawZoneName(kZoneSY);
	_archiveType = _configuration.artSY ? kArchiveArt : kArchiveFile;
	_eventHandler->onInitZone(kZoneSY);

	_loadFrom = kLoadFromCd;

	drawZoneName(kZoneAS);
	_archiveType = _configuration.artAS ? kArchiveArt : kArchiveFile;
	_eventHandler->onInitZone(kZoneAS);

	drawZoneName(kZoneNI);
	_archiveType = _configuration.artNI ? kArchiveArt : kArchiveFile;
	_eventHandler->onInitZone(kZoneNI);

	drawZoneName(kZoneN2);
	_archiveType = _configuration.artN2 ? kArchiveArt : kArchiveFile;
	_eventHandler->onInitZone(kZoneN2);

	drawZoneName(kZoneRO);
	_archiveType = _configuration.artRO ? kArchiveArt : kArchiveFile;
	_eventHandler->onInitZone(kZoneRO);

	drawZoneName(kZoneRH);
	_archiveType = _configuration.artRH ? kArchiveArt : kArchiveFile;
	_eventHandler->onInitZone(kZoneRH);

	drawZoneName(kZoneFO);
	_archiveType = _configuration.artFO ? kArchiveArt : kArchiveFile;
	_eventHandler->onInitZone(kZoneFO);

	drawZoneName(kZoneWA);
	_archiveType = _configuration.artWA ? kArchiveArt : kArchiveFile;
	_eventHandler->onInitZone(kZoneWA);

	if (_configuration.artSY || _configuration.artAS || _configuration.artNI || _configuration.artN2
		|| _configuration.artRO || _configuration.artRH || _configuration.artFO || _configuration.artWA)
		_archiveType = kArchiveArt;
	else
		_archiveType = kArchiveFile;

	setState(kStateNone);

	// Clear screen
	_screenManager->clear();
	g_system->updateScreen();
}

#pragma endregion

} // End of namespace Ring
