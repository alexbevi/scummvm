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

#include "ring/game/ring/ring_application.h"

#include "ring/base/art.h"
#include "ring/base/bag.h"
#include "ring/base/cursor.h"
#include "ring/base/dialog.h"
#include "ring/base/preferences.h"
#include "ring/base/puzzle.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"
#include "ring/base/timer.h"

#include "ring/game/ring/ring_animation.h"
#include "ring/game/ring/ring_bag.h"
#include "ring/game/ring/ring_button.h"
#include "ring/game/ring/ring_init.h"
#include "ring/game/ring/ring_input.h"
#include "ring/game/ring/ring_ride.h"
#include "ring/game/ring/ring_setup.h"
#include "ring/game/ring/ring_shared.h"
#include "ring/game/ring/ring_sound.h"
#include "ring/game/ring/ring_timer.h"
#include "ring/game/ring/ring_visual.h"
#include "ring/game/ring/ring_zone.h"

#include "ring/graphics/image.h"
#include "ring/graphics/screen.h"

#include "ring/sound/soundhandler.h"
#include "ring/sound/soundmanager.h"

#include "ring/debug.h"
#include "ring/helpers.h"
#include "ring/ring.h"

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
	// Event handlers
	_eventAnimation = new EventAnimationRing(this);
	_eventBag       = new EventBagRing(this);
	_eventButton    = new EventButtonRing(this);
	_eventInit      = new EventInitRing(this);
	_eventInput     = new EventInputRing(this);
	_eventRide      = new EventRideRing(this);
	_eventSetup     = new EventSetupRing(this);
	_eventSound     = new EventSoundRing(this);
	_eventTimer     = new EventTimerRing(this);
	_eventVisual    = new EventVisualRing(this);
	_eventZone      = new EventZoneRing(this);

	// Shared data
	_prefsVolume = 0;
	_presentationIndexRO = 0;
}

ApplicationRing::~ApplicationRing() {
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
	onSetup(zone, type);
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
	onInitZone(kZoneSY);

	_loadFrom = kLoadFromCd;

	drawZoneName(kZoneAS);
	_archiveType = _configuration.artAS ? kArchiveArt : kArchiveFile;
	onInitZone(kZoneAS);

	drawZoneName(kZoneNI);
	_archiveType = _configuration.artNI ? kArchiveArt : kArchiveFile;
	onInitZone(kZoneNI);

	drawZoneName(kZoneN2);
	_archiveType = _configuration.artN2 ? kArchiveArt : kArchiveFile;
	onInitZone(kZoneN2);

	drawZoneName(kZoneRO);
	_archiveType = _configuration.artRO ? kArchiveArt : kArchiveFile;
	onInitZone(kZoneRO);

	drawZoneName(kZoneRH);
	_archiveType = _configuration.artRH ? kArchiveArt : kArchiveFile;
	onInitZone(kZoneRH);

	drawZoneName(kZoneFO);
	_archiveType = _configuration.artFO ? kArchiveArt : kArchiveFile;
	onInitZone(kZoneFO);

	drawZoneName(kZoneWA);
	_archiveType = _configuration.artWA ? kArchiveArt : kArchiveFile;
	onInitZone(kZoneWA);

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

#pragma region Event handling

void ApplicationRing::onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed) {
	_eventInput->onMouseLeftButtonUp(evt, isControlPressed);
}

void ApplicationRing::onMouseLeftButtonDown(const Common::Event &evt) {
	_eventInput->onMouseLeftButtonDown(evt);
}

void ApplicationRing::onMouseRightButtonUp(const Common::Event &evt) {
	_eventInput->onMouseRightButtonUp(evt);
}

void ApplicationRing::onKeyDown(Common::Event &evt) {
	_eventInput->onKeyDown(evt);
}

void ApplicationRing::onButtonDown(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	debugC(kRingDebugLogic, "onButtonDown (ObjectId: %d, coords: (%d, %d))", id.id(), point.x, point.y);

	if (puzzleRotationId == 1 && a4 == 1)
		return;

	switch (getCurrentZone()) {
	default:
	case kZoneSY:
	case kZoneRH:
	case kZoneWA:
	case kZoneAS:
		break;

	case kZoneNI:
		_eventButton->onButtonDownZoneNI(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneFO:
		// Function does not seem to change anything (gets the current bag object)
		//onButtonDownZoneFO(id, a2, puzzleRotationId, a4, point);
		break;

	case kZoneRO:
		_eventButton->onButtonDownZoneRO(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneN2:
		_eventButton->onButtonDownZoneN2(id, (uint32)target, puzzleRotationId, a4, point);
		break;
	}
}

void ApplicationRing::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
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

	case kZoneNI:
		_eventButton->onButtonUpZoneNI(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneRH:
		_eventButton->onButtonUpZoneRH(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneFO:
		_eventButton->onButtonUpZoneFO(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneRO:
		_eventButton->onButtonUpZoneRO(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneWA:
		_eventButton->onButtonUpZoneWA(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneAS:
		_eventButton->onButtonUpZoneAS(id, (uint32)target, puzzleRotationId, a4, point);
		break;

	case kZoneN2:
		_eventButton->onButtonUpZoneN2(id, (uint32)target, puzzleRotationId, a4, point);
		break;
	}
}

void ApplicationRing::onButtonUp2(ObjectId id, uint32 index, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	debugC(kRingDebugLogic, "onButtonUp2 (ObjectId: %d, coords: (%d, %d))", id.id(), point.x, point.y);

	if (puzzleRotationId == 1 && a4 == 1) {
		return;
	}

	switch (getCurrentZone()) {
	default:
	case kZoneSY:
	case kZoneNI:
	case kZoneRH:
	case kZoneRO:
	case kZoneAS:
	case kZoneN2:
		break;

	case kZoneFO:
		// Function does not seem to change anything (gets the current bag object)
		// onButtonDownZoneFO(id, a2, puzzleRotationId, a4, point);
		break;

	case kZoneWA:
		_eventButton->onButtonUp2ZoneWA(id, index, puzzleRotationId, a4, point);
		break;
	}
}

void ApplicationRing::onSetup(ZoneId zone, SetupType type) {
	debugC(kRingDebugLogic, "onSetup (zone: %s, type: %d)", getZoneFolder(zone).c_str(), type);

	switch (zone) {
	default:
	case kZoneSY:
		break;

	case kZoneNI:
		_eventSetup->onSetupZoneNI(type);
		break;

	case kZoneRH:
		_eventSetup->onSetupZoneRH(type);
		break;

	case kZoneFO:
		_eventSetup->onSetupZoneFO(type);
		break;

	case kZoneRO:
		_eventSetup->onSetupZoneRO(type);
		break;

	case kZoneWA:
		_eventSetup->onSetupZoneWA(type);
		break;

	case kZoneAS:
		_eventSetup->onSetupZoneAS(type);
		break;

	case kZoneN2:
		_eventSetup->onSetupZoneN2(type);
		break;
	}
}

void ApplicationRing::onInitZone(ZoneId zone) {
	switch (zone) {
	default:
		break;

	case kZoneSY:
		_eventInit->initZoneSY();
		break;

	case kZoneNI:
		_eventInit->initZoneNI();
		break;

	case kZoneRH:
		_eventInit->initZoneRH();
		break;

	case kZoneFO:
		_eventInit->initZoneFO();
		break;

	case kZoneRO:
		_eventInit->initZoneRO();
		break;

	case kZoneWA:
		_eventInit->initZoneWA();
		break;

	case kZoneAS:
		_eventInit->initZoneAS();
		break;

	case kZoneN2:
		_eventInit->initZoneN2();
		break;
	}
}

void ApplicationRing::onSwitchZone(ZoneId zone, uint32 type) {
	switch (zone) {
	default:
		break;

	case kZoneNI:
		_eventZone->onSwitchZoneNI(type);
		break;

	case kZoneRH:
		_eventZone->onSwitchZoneRH(type);
		break;

	case kZoneFO:
		_eventZone->onSwitchZoneFO(type);
		break;

	case kZoneRO:
		_eventZone->onSwitchZoneRO(type);
		break;

	case kZoneWA:
		_eventZone->onSwitchZoneWA(type);
		break;

	case kZoneAS:
		_eventZone->onSwitchZoneAS(type);
		break;

	case kZoneN2:
		_eventZone->onSwitchZoneN2(type);
		break;
	}
}

void ApplicationRing::onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
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

	case kZoneNI:
		_eventZone->onUpdateBeforeZoneNI(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZoneN2:
		_eventZone->onUpdateBeforeZoneN2(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;
	}
}

void ApplicationRing::onTimer(TimerId timerId) {
	debugC(kRingDebugLogic, "onTimer (id: %d)", timerId);

	switch (getCurrentZone()) {
	default:
	case kZoneSY:
	case kZoneWA:
		break;

	case kZoneNI:
		_eventTimer->onTimerZoneNI(timerId);
		break;

	case kZoneRH:
		_eventTimer->onTimerZoneRH(timerId);
		break;

	case kZoneFO:
		_eventTimer->onTimerZoneFO(timerId);
		break;

	case kZoneRO:
		_eventTimer->onTimerZoneRO(timerId);
		break;

	case kZoneAS:
		_eventTimer->onTimerZoneAS(timerId);
		break;

	case kZoneN2:
		_eventTimer->onTimerZoneN2(timerId);
		break;
	}

	getTimerHandler()->incrementFiredCount(timerId);
}

void ApplicationRing::onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	debugC(kRingDebugLogic, "onBag (object: %d)", id.id());

	if (puzzleRotationId == 1 && a4 == 1) {
		_eventBag->onBagZoneSY(id, (uint32)target, 1, 1, dragControl, type);
		return;
	}

	switch (getCurrentZone()) {
	default:
	case kZoneRH:
	case kZoneWA:
	case kZoneAS:
		break;

	case kZoneSY:
		_eventBag->onBagZoneSY(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZoneNI:
		_eventBag->onBagZoneNI(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZoneFO:
		_eventBag->onBagZoneFO(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZoneRO:
		_eventBag->onBagZoneRO(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZoneN2:
		_eventBag->onBagZoneN2(id, (uint32)target, puzzleRotationId, a4, dragControl, type);
		break;
	}
}

void ApplicationRing::onUpdateBag(const Common::Point &point) {
	if (getCurrentZone() == kZoneSY)
		_eventBag->onUpdateBagZoneSY(point);
}

void ApplicationRing::onBagClickedObject(ObjectId id) {
	switch (getCurrentZone()) {
	default:
		break;

	case kZoneFO:
		_eventBag->onBagClickedObjectZoneFO(id);
		break;
	}
}

void ApplicationRing::onBagZoneSwitch() {
	_eventBag->onBagZoneSwitch();
}

void ApplicationRing::onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {
	if (movabilityFrom == 1)
		return;

	switch (getCurrentZone()) {
	default:
	case kZoneSY:
		break;

	case kZoneNI:
		_eventRide->onBeforeRideZoneNI(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneRH:
		_eventRide->onBeforeRideZoneRH(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneFO:
		_eventRide->onBeforeRideZoneFO(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneRO:
		_eventRide->onBeforeRideZoneRO(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneWA:
		_eventRide->onBeforeRideZoneWA(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneAS:
		_eventRide->onBeforeRideZoneAS(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneN2:
		_eventRide->onBeforeRideZoneN2(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;
	}
}

void ApplicationRing::onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {
	if (movabilityTo == 1)
		return;

	switch (getCurrentZone()) {
	default:
	case kZoneSY:
		break;

	case kZoneNI:
		_eventRide->onAfterRideZoneNI(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneRH:
		_eventRide->onAfterRideZoneRH(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneFO:
		_eventRide->onAfterRideZoneFO(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneRO:
		_eventRide->onAfterRideZoneRO(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneWA:
		_eventRide->onAfterRideZoneWA(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneAS:
		_eventRide->onAfterRideZoneAS(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;

	case kZoneN2:
		_eventRide->onAfterRideZoneN2(movabilityFrom, movabilityTo, movabilityIndex, (uint32)target, movabilityType);
		break;
	}
}

void ApplicationRing::onSound(Id id, SoundType type, uint32 a3) {
	debugC(kRingDebugLogic, "onSound (id: %d, type: %d)", id, type);

	bool process = (a3 & 0x1000) != 0;
	a3 &= 239;

	switch (getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		_eventSound->onSoundZoneSY(id, type, a3, process);
		break;

	case kZoneNI:
		_eventSound->onSoundZoneNI(id, type, a3, process);
		break;

	case kZoneRH:
		_eventSound->onSoundZoneRH(id, type, a3, process);
		break;

	case kZoneFO:
		_eventSound->onSoundZoneFO(id, type, a3, process);
		break;

	case kZoneRO:
		_eventSound->onSoundZoneRO(id, type, a3, process);
		break;

	case kZoneAS:
		_eventSound->onSoundZoneAS(id, type, a3, process);
		break;

	case kZoneWA:
		_eventSound->onSoundZoneWA(id, type, a3, process);
		break;

	case kZoneN2:
		_eventSound->onSoundZoneN2(id, type, a3, process);
		break;
	}
}

void ApplicationRing::onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	switch (getCurrentZone()) {
	default:
	case kZoneSY:
		break;

	case kZoneNI:
		_eventAnimation->onAnimationNextFrameZoneNI(animationId, name, frame, frameCount);
		break;

	case kZoneRH:
		_eventAnimation->onAnimationNextFrameZoneRH(animationId, name, frame, frameCount);
		break;

	case kZoneFO:
		_eventAnimation->onAnimationNextFrameZoneFO(animationId, name, frame, frameCount);
		break;

	case kZoneRO:
		_eventAnimation->onAnimationNextFrameZoneRO(animationId, name, frame, frameCount);
		break;

	case kZoneAS:
		_eventAnimation->onAnimationNextFrameZoneAS(animationId, name, frame, frameCount);
		break;

	case kZoneWA:
		_eventAnimation->onAnimationNextFrameZoneWA(animationId, name, frame, frameCount);
		break;

	case kZoneN2:
		_eventAnimation->onAnimationNextFrameZoneN2(animationId, name, frame, frameCount);
		break;
	}
}

void ApplicationRing::onAnimation(uint32 type, Id animationId, const Common::String &name, uint32 frame, uint32 a5) {
	if (getCurrentZone() == kZoneWA)
		_eventAnimation->onAnimationZoneWA(type, animationId, name, frame, a5);
}

void ApplicationRing::onVisualList(Id id, uint32 type, const Common::Point &point) {
	if (getCurrentZone() == kZoneSY)
		_eventVisual->onVisualListZoneSY(id, type, point);
}

#pragma endregion

#pragma region Helper functions

void ApplicationRing::sub_433EE0() {
	if (varGetByte(70012) == 1
		&& varGetByte(70001) == 0
		&& bagHas(kObjectCage)
		&& bagHas(kObjectCentaur)
		&& bagHas(kObjectDragon)
		&& bagHas(kObjectPhoenix1)) {
			varSetByte(70013, 11);
			puzzleSetActive(kPuzzle70305);
			soundPlay(70017);
	}
}

void ApplicationRing::sub_445A10() {
	int32 val1 = varGetByte(10106) + varGetByte(10431);
	int32 val2 = varGetByte(10420) + varGetByte(10421);

	if (!varGetByte(10107)) {

		if (val1 != 2)
			return;

		soundPlay(10412, kSoundLoop);
		objectPresentationShow(kObject10432, 0);
		soundSetVolume(10412, 80);

		if (val2 == 2) {
			timerStop(kTimer0);
			varSetByte(10107, 0);
			varSetByte(10432, 0);

			timerStart(kTimer1, 1000);
			objectPresentationHide(kObject10431);
			objectPresentationShow(kObject10431, 6);
		} else {
			varSetByte(10432, 0);
			timerStop(kTimer1);
			timerStart(kTimer0, 1000);
			varSetByte(10107, 1);
		}

		return;
	}

	if (val1 == 2) {
		if (val2 == 2) {
			soundPlay(10412, kSoundLoop);
			objectPresentationShow(kObject10432, 0);
			soundSetVolume(10412, 80);
			timerStop(kTimer0);
			varSetByte(10107, 0);
			varSetByte(10432, 0);
			timerStop(kTimer1);
			timerStart(kTimer1, 1000);
			objectPresentationHide(kObject10431);
			objectPresentationShow(kObject10431, 6);
		}
	} else {
		soundStop(10412, 1024);
		objectPresentationHide(kObject10432);
		varSetByte(10107, 0);
		timerStop(kTimer0);
		timerStop(kTimer1);
		objectPresentationHide(kObject10431);
	}
}

#pragma endregion

} // End of namespace Ring
