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

#include "ring/base/accessibility.h"
#include "ring/base/art.h"
#include "ring/base/bag.h"
#include "ring/base/cursor.h"
#include "ring/base/dialog.h"
#include "ring/base/movability.h"
#include "ring/base/preferences.h"
#include "ring/base/puzzle.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"
#include "ring/base/timer.h"

#include "ring/game/ring/ring_shared.h"
#include "ring/game/ring/ring_visual.h"
#include "ring/game/ring/ring_zonesystem.h"
#include "ring/game/ring/ring_zoneni.h"
#include "ring/game/ring/ring_zonerh.h"
#include "ring/game/ring/ring_zonefo.h"
#include "ring/game/ring/ring_zonero.h"
#include "ring/game/ring/ring_zonewa.h"
#include "ring/game/ring/ring_zoneas.h"
#include "ring/game/ring/ring_zonen2.h"

#include "ring/graphics/dragControl.h"
#include "ring/graphics/hotspot.h"
#include "ring/graphics/image.h"
#include "ring/graphics/screen.h"

#include "ring/sound/soundhandler.h"
#include "ring/sound/soundmanager.h"

#include "ring/debug.h"
#include "ring/helpers.h"
#include "ring/ring.h"

#include "common/file.h"

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
	_zoneSystem = new ZoneSystemRing(this);
	_zoneNI     = new ZoneNIRing(this);
	_zoneRH     = new ZoneRHRing(this);
	_zoneFO     = new ZoneFORing(this);
	_zoneRO     = new ZoneRORing(this);
	_zoneWA     = new ZoneWARing(this);
	_zoneAS     = new ZoneASRing(this);
	_zoneN2     = new ZoneN2Ring(this);

	_controlNotPressed = false;
	_presentationIndexNI = 0;
}

ApplicationRing::~ApplicationRing() {
	SAFE_DELETE(_zoneSystem);
	SAFE_DELETE(_zoneNI);
	SAFE_DELETE(_zoneRH);
	SAFE_DELETE(_zoneFO);
	SAFE_DELETE(_zoneRO);
	SAFE_DELETE(_zoneWA);
	SAFE_DELETE(_zoneAS);
	SAFE_DELETE(_zoneN2);
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
			// The original checks for the correct CD defined in cd.ini,
			// instead we check that the zone archive has been copied properly
			if (!Common::File::exists(getZoneFolder(zone)))
				hasData = false;
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
	_zoneSystem->onInit();

	_loadFrom = kLoadFromCd;

	drawZoneName(kZoneAS);
	_archiveType = _configuration.artAS ? kArchiveArt : kArchiveFile;
	_zoneAS->onInit();

	drawZoneName(kZoneNI);
	_archiveType = _configuration.artNI ? kArchiveArt : kArchiveFile;
	_zoneNI->onInit();

	drawZoneName(kZoneN2);
	_archiveType = _configuration.artN2 ? kArchiveArt : kArchiveFile;
	_zoneN2->onInit();

	drawZoneName(kZoneRO);
	_archiveType = _configuration.artRO ? kArchiveArt : kArchiveFile;
	_zoneRO->onInit();

	drawZoneName(kZoneRH);
	_archiveType = _configuration.artRH ? kArchiveArt : kArchiveFile;
	_zoneRH->onInit();

	drawZoneName(kZoneFO);
	_archiveType = _configuration.artFO ? kArchiveArt : kArchiveFile;
	_zoneFO->onInit();

	drawZoneName(kZoneWA);
	_archiveType = _configuration.artWA ? kArchiveArt : kArchiveFile;
	_zoneWA->onInit();

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
	_controlNotPressed = !isControlPressed;

	onMouseLeftButtonUp(evt);
}

void ApplicationRing::onMouseLeftButtonUp(const Common::Event &evt) {
	debugC(kRingDebugLogic, "onMouseLeftButtonUp");

	// Handle clicks on bag
	Bag *bag = getBag();
	if (bag->isInitialized()) {
		if (bag->checkHotspotClick(evt.mouse) == 1) {

			// Handle clicked object event
			onBagClickedObject(getBag()->getClickedObject());

			if (getField77()) {
				bag->reset();
				setFreOffCurrentRotation();

				if (getField78()) {
					initObjectCursors(bag->getClickedObject());
				} else {
					setField78(true);
					cursorDelete();
				}

				// Center cursor on screen
				g_system->warpMouse(320, 240);

			} else {
				setField77(true);
				cursorDelete();
			}
		}

		return;
	}

	// Handle clicks on drag control
	DragControl *dragControl = getDragControl();
	if (dragControl->getField20()) {
		onBag(dragControl->getObjectId(), dragControl->getTarget(), dragControl->getPuzzleRotationId(), dragControl->getField39(), dragControl, 2);

		if (getState() == kStateShowMenu)
			return;

		dragControl->reset();

		if (dragControl->getField45() == 2)
			return;
	}

	// Handle clicks on menu
	Puzzle *puzzleMenu = getPuzzle(kPuzzleMenu);
	if (puzzleMenu) {
		if (puzzleMenu->visualHandleLeftButtonUp(evt.mouse)) {
			update(evt.mouse);
			return;
		}

		Accessibility *accessibility = puzzleMenu->getAccessibility(evt.mouse);
		if (handleLeftButtonUp(accessibility, puzzleMenu->getId(), evt.mouse))
			return;

		if (puzzleMenu->getField24() == 1) {
			Movability *movability = puzzleMenu->getMovability(evt.mouse);
			if (handleLeftButtonUp(movability, (uint32)puzzleMenu->getMovabilityIndex(evt.mouse), puzzleMenu->getId()))
				return;
		}

		if (puzzleMenu->getField24() == 2)
			return;
	}

	// Handle current puzzle
	Puzzle *currentPuzzle = getCurrentPuzzle();
	if (currentPuzzle) {
		if (currentPuzzle->visualHandleLeftButtonUp(evt.mouse)) {
			update(evt.mouse);
			return;
		}

		Accessibility *accessibility = currentPuzzle->getAccessibility(evt.mouse);
		if (handleLeftButtonUp(accessibility, currentPuzzle->getId(), evt.mouse))
			return;

		Movability *movability = currentPuzzle->getMovability(evt.mouse);
		if (handleLeftButtonUp(movability, (uint32)currentPuzzle->getMovabilityIndex(evt.mouse), currentPuzzle->getId()))
			return;
	}

	// Handle current rotation
	Rotation *currentRotation = getCurrentRotation();
	if (currentRotation) {
		if (!currentRotation->getField28())
			return;

		Accessibility *accessibility = currentRotation->getAccessibility(evt.mouse);
		if (handleLeftButtonUp(accessibility, currentRotation->getId(), evt.mouse))
			return;

		Movability *movability = currentRotation->getMovability(evt.mouse);
		if (handleLeftButtonUp(movability, (uint32)currentRotation->getMovabilityIndex(evt.mouse), currentRotation->getId(), true))
			return;

		if (getField75())
			cursorDelete();

		setField75(true);
	}
}

bool ApplicationRing::handleLeftButtonUp(Accessibility *accessibility, Id id, const Common::Point &point) {
	if (!accessibility)
		return false;

	Object *object = accessibility->getObject();
	Hotspot *hotspot = accessibility->getHotspot();

	if (object->getFieldC() & 1) {
		onButtonUp(object->getId(), hotspot->getTarget(), id, 1, point);

		if (getState() == kStateShowMenu)
			return true;
	}

	if (!(object->getFieldC() & 8)) {
		update(point);
		return true;
	}

	onButtonUp2(object->getId(), (uint32)hotspot->getTarget(), id, 1, point);

	if (getState() != kStateShowMenu) {
		if (getField74()) {
			cursorDelete();
			getBag()->setClickedObject(object->getId());
			initObjectCursors(object->getId());
		}

		setField74(true);

		update(point);
	}

	return true;
}

bool ApplicationRing::handleLeftButtonUp(Movability *movability, uint32 index, Id id, bool isRotation) {
	if (!movability)
		return false;

	Hotspot *hotspot = movability->getHotspot();

	//////////////////////////////////////////////////////////////////////////
	// Before Ride
	//////////////////////////////////////////////////////////////////////////
	onBeforeRide(id, movability->getTo(), index, hotspot->getTarget(), movability->getType());

	if (getState() == kStateShowMenu)
		return true;

	bool showRide = _controlNotPressed;

	int updateType = 0;
	if (isRotation) {
		if (_controlNotPressed) {
			updateType = movability->getUpdateType();
		} else {
			updateType = 2;
			showRide = false;
		}

		switch (updateType) {
		default:
			break;

		case 0:
			showRide = getCurrentRotation()->setRolTo(movability->getAlpBefore(), movability->getBetBefore(), movability->getRanBefore());
			break;

		case 1:
			getCurrentRotation()->updateAndDraw(movability->getAlpBefore(), movability->getBetBefore(), movability->getRanBefore());
			break;
		}

	}

	reset();

	switch (movability->getType()) {
	default:
		break;

	case kMovabilityRotationToRotation:
	case kMovabilityPuzzleToRotation:
		if (isRotation)
			break;

		if (getRotation(movability->getTo())) {
			Rotation *rotation = getRotation(movability->getTo());

			float alp = movability->getAlpAfter() - 135.0f;
			if (alp < 0.0f)
				alp += 360.0f;

			rotation->setAlp(alp);

			rotation->updateAmbientSoundPan(false);

			getSoundHandler()->setSounds2(rotation->getSoundItems());
			getSoundHandler()->setField0(true);

			if (!getSoundHandler()->processSounds()) {
				getSoundHandler()->setField0(false);
				getSoundHandler()->turnOffSounds1(true);
			}
		}
		break;

	case kMovabilityRotationToPuzzle:
	case kMovabilityPuzzleToPuzzle:
		if (getPuzzle(movability->getTo().id())) {
			Puzzle *puzzle = getPuzzle(movability->getTo().id());

			getSoundHandler()->setSounds2(puzzle->getSoundItems());
			getSoundHandler()->setField0(true);

			if (!getSoundHandler()->processSounds()) {
				getSoundHandler()->setField0(false);
				getSoundHandler()->turnOffSounds1(true);
			}
		}
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	// Ride
	//////////////////////////////////////////////////////////////////////////
	if (showRide)
		playMovie(movability->getRideName());

	//////////////////////////////////////////////////////////////////////////
	// After ride
	//////////////////////////////////////////////////////////////////////////
	if (getSoundHandler()->getField0()) {
		getSoundHandler()->turnOffItems1();

		if (!getSoundHandler()->updateItems(2)) {
			getSoundHandler()->turnOffSounds1(true);
			getSoundHandler()->setField0(false);
		}
	}

	if (getSoundHandler()->getField0()) {
		getSoundHandler()->updateItems2(3);
		getSoundHandler()->updateItems3(3);
	}

	switch (movability->getType()){
	default:
		break;

	case kMovabilityRotationToRotation:
	case kMovabilityPuzzleToRotation:
		rotationSetActive(movability->getTo());
		getCurrentRotation()->updateAndDraw(movability->getAlpAfter(), movability->getBetAfter(), movability->getRanAfter());

		onAfterRide(getCurrentRotationId(), id, index, hotspot->getTarget(), movability->getType());

		if (getState() == kStateShowMenu)
			return true;
		break;

	case kMovabilityRotationToPuzzle:
	case kMovabilityPuzzleToPuzzle:
		puzzleSetActive(movability->getTo().id());

		onAfterRide(getCurrentRotationId(), id, index, hotspot->getTarget(), movability->getType());

		if (getState() == kStateShowMenu)
			return true;
		break;
	}

	if (getField75())
		cursorDelete();

	setField75(true);

	return true;
}

void ApplicationRing::onMouseLeftButtonDown(const Common::Event &evt) {
	debugC(kRingDebugLogic, "onMouseLeftButtonDown");

	if (getBag()->isInitialized())
		return;

	// Handle menu
	Puzzle *puzzleMenu = getPuzzle(kPuzzleMenu);
	if (puzzleMenu) {

		if (puzzleMenu->visualHandleLeftButtonDown(evt.mouse)) {
			update(evt.mouse);
			return;
		}

		Accessibility *accessibility = puzzleMenu->getAccessibility(evt.mouse);
		if (handleLeftButtonDown(accessibility, (uint32)puzzleMenu->getAccessibilityIndex(evt.mouse), puzzleMenu->getId(), evt.mouse))
			return;

		if (puzzleMenu->getField24() == 2)
			return;
	}

	// Handle current puzzle
	Puzzle *currentPuzzle = getCurrentPuzzle();
	if (currentPuzzle) {
		if (currentPuzzle->visualHandleLeftButtonDown(evt.mouse)) {
			update(evt.mouse);
			return;
		}

		Accessibility *accessibility = currentPuzzle->getAccessibility(evt.mouse);
		if (handleLeftButtonDown(accessibility, (uint32)currentPuzzle->getAccessibilityIndex(evt.mouse), currentPuzzle->getId(), evt.mouse))
			return;
	}

	// Handle current rotation
	Rotation *currentRotation = getCurrentRotation();
	if (currentRotation) {
		Accessibility *accessibility = currentRotation->getAccessibility(evt.mouse);
		if (handleLeftButtonDown(accessibility, (uint32)currentRotation->getAccessibilityIndex(evt.mouse), currentRotation->getId(), evt.mouse))
			return;
	}
}

bool ApplicationRing::handleLeftButtonDown(Accessibility *accessibility, uint32 index, Id id, const Common::Point &point) {
	if (!accessibility)
		return false;

	Object *object = accessibility->getObject();
	Hotspot *hotspot = accessibility->getHotspot();

	if (object->getFieldC() & 2) {
		onButtonDown(object->getId(), hotspot->getTarget(), id, 1, point);

		if (getState() == kStateShowMenu)
			return true;
	}

	if (!(object->getFieldC() & 4)) {
		update(point);
		return true;
	}

	getDragControl()->init(point, object->getId(), index, hotspot, hotspot->getTarget(), id, 1);
	onBag(object->getId(), hotspot->getTarget(), id, 1, getDragControl(), 1);

	if (getState() != kStateShowMenu) {
		if (!getField76())
			getDragControl()->reset();

		setField76(true);
		update(point);
	}

	return true;
}

void ApplicationRing::onMouseRightButtonUp(const Common::Event &) {
	debugC(kRingDebugLogic, "onMouseRightButtonUp");

	if (getApp()->getDragControl()->getField20() || getApp()->getCurrentGameZone())
		return;

	Puzzle *puzzleMenu = getApp()->getPuzzle(kPuzzleMenu);
	if (puzzleMenu && puzzleMenu->getField24() == 2)
		return;

	Bag *bag = getApp()->getBag();
	if (bag->isInitialized()) {
		bag->reset();
	} else {
		getApp()->cursorDelete();
		bag->initialize();

		if (getApp()->getCurrentRotation())
			bag->setRotationFre(getApp()->getCurrentRotation()->getFre());

		getApp()->setupCurrentRotation();
	}
}

void ApplicationRing::onKeyDown(Common::Event &evt) {
	debugC(kRingDebugLogic, "onKeyDown");

	// Stop dialogs if needed
	if (evt.kbd.keycode == Common::KEYCODE_ESCAPE)
		soundStopType(kSoundTypeDialog, 4098);

	// Check menu and simulate button click
	Puzzle *puzzleMenu = getApp()->getPuzzle(kPuzzleMenu);
	if (puzzleMenu) {
		if (puzzleMenu->visualHandleKey(evt.kbd.keycode))
			return;

		if (puzzleMenu->getField24() == 2) {
			Accessibility *accessibility = puzzleMenu->getAccessibility(evt.kbd.keycode);
			if (!accessibility)
				return;

			// Center rotation
			Common::Point point = accessibility->getHotspot()->getCenter();
			if (hasCurrentRotation())
				getCurrentRotation()->setCoordinates(&point, evt.kbd.keycode);

			// Modify event to be a mouse click on the center of the hotspot
			bool controlPressed = (bool)(evt.kbd.flags & Common::KBD_CTRL);
			evt.mouse = point;

			// Simulate click
			if (getBag()->isInitialized()) {
				onMouseLeftButtonUp(evt, controlPressed);
				return;
			}

			onMouseLeftButtonUp(evt, controlPressed);

			if (evt.mouse.y >= 16)
				return;

			// FIXME: Ring calls a method that doesn't do anything useful (get the menu puzzle if the bag has a clicked object)
			return;
		}
	}

	// Handle events on zone
	onKeyDownZone(evt.kbd);

	// Check current puzzle and simulate button click
	Puzzle *currentPuzzle = getCurrentPuzzle();
	if (currentPuzzle) {
		if (currentPuzzle->visualHandleKey(evt.kbd.keycode))
			return;

		if (currentPuzzle->getField24() == 2) {
			Accessibility *accessibility = currentPuzzle->getAccessibility(evt.kbd.keycode);
			if (!accessibility)
				return;

			bool controlPressed = (bool)(evt.kbd.flags & Common::KBD_CTRL);

			// Modify event to be a mouse click on the center of the hotspot
			Hotspot *hotspot = accessibility->getHotspot();
			evt.mouse = hotspot->getCenter();

			// Simulate click
			if (getBag()->isInitialized()) {
				onMouseLeftButtonUp(evt, controlPressed);
				return;
			}

			onMouseLeftButtonUp(evt, controlPressed);

			if (evt.mouse.y >= 16)
				return;

			// FIXME: Ring calls a method that doesn't do anything useful (get the menu puzzle if the bag has a clicked object)
			return;
		}
	}

	// Check current rotation and simulate button click
	Rotation *currentRotation = getCurrentRotation();
	if (currentRotation) {
		Accessibility *accessibility = currentRotation->getAccessibility(evt.kbd.keycode);
		if (!accessibility)
			return;

		bool controlPressed = (bool)(evt.kbd.flags & Common::KBD_CTRL);

		// Modify event to be a mouse click on the center of the hotspot
		Hotspot *hotspot = accessibility->getHotspot();
		evt.mouse = hotspot->getCenter();

		// Center rotation
		currentRotation->setCoordinates(&evt.mouse, evt.kbd.keycode);

		// Simulate click
		if (getBag()->isInitialized()) {
			onMouseLeftButtonUp(evt, controlPressed);
			return;
		}

		onMouseLeftButtonUp(evt, controlPressed);

		if (evt.mouse.y >= 16)
			return;

		// FIXME: Ring calls a method that doesn't do anything useful (get the menu puzzle if the bag has a clicked object)
	}
}

void ApplicationRing::onKeyDownZone(const Common::KeyState &keyState) {
	if (getCurrentZone() == kZoneSY)
		_zoneSystem->onKeyDown(keyState);
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
		_zoneNI->onButtonDown(id, target, puzzleRotationId, a4, point);
		break;

	case kZoneFO:
		// Function does not seem to change anything (gets the current bag object)
		//onButtonDownZoneFO(id, a2, puzzleRotationId, a4, point);
		break;

	case kZoneRO:
		_zoneRO->onButtonDown(id, target, puzzleRotationId, a4, point);
		break;

	case kZoneN2:
		_zoneN2->onButtonDown(id, target, puzzleRotationId, a4, point);
		break;
	}
}

void ApplicationRing::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
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

	case kZoneNI:
		_zoneNI->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZoneRH:
		_zoneRH->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZoneFO:
		_zoneFO->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZoneRO:
		_zoneRO->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZoneWA:
		_zoneWA->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZoneAS:
		_zoneAS->onButtonUp(id, target, puzzleRotationId, a4, point);
		break;

	case kZoneN2:
		_zoneN2->onButtonUp(id, target, puzzleRotationId, a4, point);
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
		_zoneWA->onButtonUp2(id, index, puzzleRotationId, a4, point);
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
		_zoneNI->onSetup(type);
		break;

	case kZoneRH:
		_zoneRH->onSetup(type);
		break;

	case kZoneFO:
		_zoneFO->onSetup(type);
		break;

	case kZoneRO:
		_zoneRO->onSetup(type);
		break;

	case kZoneWA:
		_zoneWA->onSetup(type);
		break;

	case kZoneAS:
		_zoneAS->onSetup(type);
		break;

	case kZoneN2:
		_zoneN2->onSetup(type);
		break;
	}
}

void ApplicationRing::onSwitchZone(ZoneId zone, uint32 type) {
	switch (zone) {
	default:
		break;

	case kZoneNI:
		_zoneNI->onSwitch(type);
		break;

	case kZoneRH:
		_zoneRH->onSwitch(type);
		break;

	case kZoneFO:
		_zoneFO->onSwitch(type);
		break;

	case kZoneRO:
		_zoneRO->onSwitch(type);
		break;

	case kZoneWA:
		_zoneWA->onSwitch(type);
		break;

	case kZoneAS:
		_zoneAS->onSwitch(type);
		break;

	case kZoneN2:
		_zoneN2->onSwitch(type);
		break;
	}
}

void ApplicationRing::onUpdateBefore(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point) {
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

	case kZoneNI:
		_zoneNI->onUpdateBefore(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
		break;

	case kZoneN2:
		_zoneN2->onUpdateBefore(movabilityFrom, movabilityTo, movabilityIndex, a4, point);
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
		_zoneNI->onTimer(timerId);
		break;

	case kZoneRH:
		_zoneRH->onTimer(timerId);
		break;

	case kZoneFO:
		_zoneFO->onTimer(timerId);
		break;

	case kZoneRO:
		_zoneRO->onTimer(timerId);
		break;

	case kZoneAS:
		_zoneAS->onTimer(timerId);
		break;

	case kZoneN2:
		_zoneN2->onTimer(timerId);
		break;
	}

	getTimerHandler()->incrementFiredCount(timerId);
}

void ApplicationRing::onBag(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type) {
	debugC(kRingDebugLogic, "onBag (object: %d)", id.id());

	if (puzzleRotationId == 1 && a4 == 1) {
		_zoneSystem->onBag(id, target, 1, 1, dragControl, type);
		return;
	}

	switch (getCurrentZone()) {
	default:
	case kZoneRH:
	case kZoneWA:
	case kZoneAS:
		break;

	case kZoneSY:
		_zoneSystem->onBag(id, target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZoneNI:
		_zoneNI->onBag(id, target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZoneFO:
		_zoneFO->onBag(id, target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZoneRO:
		_zoneRO->onBag(id, target, puzzleRotationId, a4, dragControl, type);
		break;

	case kZoneN2:
		_zoneN2->onBag(id, target, puzzleRotationId, a4, dragControl, type);
		break;
	}
}

void ApplicationRing::onUpdateBag(const Common::Point &point) {
	if (getCurrentZone() == kZoneSY)
		_zoneSystem->onUpdateBag(point);
}

void ApplicationRing::onBagClickedObject(ObjectId id) {
	switch (getCurrentZone()) {
	default:
		break;

	case kZoneFO:
		_zoneFO->onBagClickedObject(id);
		break;
	}
}

void ApplicationRing::onBagZoneSwitch() {
	switch (getApp()->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
	case kZoneAS:
		error("[EventBagRing::onBagZoneSwitch] Erda should not be active on this zone (%d)", getApp()->getCurrentZone());

	case kZoneNI:
	case kZoneRH:
		if (!getApp()->getSaveManager()->loadSaveTimer("alb", kLoadSaveWrite))
			error("[EventBagRing::onBagZoneSwitch] Failed loading timer (alb)");

		varSetDword(90013, getCurrentZone());
		varSetByte(90009, 1);

		if (hasCurrentPuzzle()) {
			varSetByte(90017, 1);
			varSetDword(90021, getCurrentPuzzleId());
		} else {
			varSetByte(90017, 0);
			varSetDword(90021, getCurrentRotationId());
			varSetByte(90025, getApp()->getBag()->getRotationFre());
		}

		onSwitchZone(kZoneAS, 13);
		break;

	case kZoneFO:
		if (!getApp()->getSaveManager()->loadSaveTimer("sie", kLoadSaveWrite))
			error("[EventBagRing::onBagZoneSwitch] Failed loading timer (sie)");

		varSetDword(90015, getCurrentZone());
		varSetByte(90011, 1);

		if (hasCurrentPuzzle()) {
			varSetByte(90019, 1);
			varSetDword(90023, getCurrentPuzzleId());
		} else {
			varSetByte(90019, 0);
			varSetDword(90023, getCurrentRotationId());
			varSetByte(90027, getApp()->getBag()->getRotationFre());
		}

		onSwitchZone(kZoneAS, 13);
		break;

	case kZoneRO:
	case kZoneN2:
		if (!getApp()->getSaveManager()->loadSaveTimer("log", kLoadSaveWrite))
			error("[EventBagRing::onBagZoneSwitch] Failed loading timer (log)");

		varSetDword(90014, getCurrentZone());
		varSetByte(90010, 1);

		if (hasCurrentPuzzle()) {
			varSetByte(90018, 1);
			varSetDword(90022, getCurrentPuzzleId());
		} else {
			varSetByte(90018, 0);
			varSetDword(90022, getCurrentRotationId());
			varSetByte(90026, getApp()->getBag()->getRotationFre());
		}

		onSwitchZone(kZoneAS, 13);
		break;

	case kZoneWA:
		if (!getApp()->getSaveManager()->loadSaveTimer("bru", kLoadSaveWrite))
			error("[EventBagRing::onBagZoneSwitch] Failed loading timer (bru)");

		varSetDword(90016, getCurrentZone());
		varSetByte(90012, 1);

		if (hasCurrentPuzzle()) {
			varSetByte(90020, 1);
			varSetDword(90024, getCurrentPuzzleId());
		} else {
			varSetByte(90020, 0);
			varSetDword(90024, getCurrentRotationId());
			varSetByte(90028, getApp()->getBag()->getRotationFre());
		}

		onSwitchZone(kZoneAS, 13);
		break;
	}
}

void ApplicationRing::onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, Id target, MovabilityType movabilityType) {
	if (movabilityFrom == 1)
		return;

	switch (getCurrentZone()) {
	default:
	case kZoneSY:
		break;

	case kZoneNI:
		_zoneNI->onBeforeRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZoneRH:
		_zoneRH->onBeforeRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZoneFO:
		_zoneFO->onBeforeRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZoneRO:
		_zoneRO->onBeforeRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZoneWA:
		_zoneWA->onBeforeRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZoneAS:
		_zoneAS->onBeforeRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZoneN2:
		_zoneN2->onBeforeRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
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
		_zoneNI->onAfterRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZoneRH:
		_zoneRH->onAfterRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZoneFO:
		_zoneFO->onAfterRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZoneRO:
		_zoneRO->onAfterRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZoneWA:
		_zoneWA->onAfterRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZoneAS:
		_zoneAS->onAfterRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
		break;

	case kZoneN2:
		_zoneN2->onAfterRide(movabilityFrom, movabilityTo, movabilityIndex, target, movabilityType);
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
		_zoneSystem->onSound(id, type, a3, process);
		break;

	case kZoneNI:
		_zoneNI->onSound(id, type, a3, process);
		break;

	case kZoneRH:
		_zoneRH->onSound(id, type, a3, process);
		break;

	case kZoneFO:
		_zoneFO->onSound(id, type, a3, process);
		break;

	case kZoneRO:
		_zoneRO->onSound(id, type, a3, process);
		break;

	case kZoneAS:
		_zoneAS->onSound(id, type, a3, process);
		break;

	case kZoneWA:
		_zoneWA->onSound(id, type, a3, process);
		break;

	case kZoneN2:
		_zoneN2->onSound(id, type, a3, process);
		break;
	}
}

void ApplicationRing::onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	switch (getCurrentZone()) {
	default:
	case kZoneSY:
		break;

	case kZoneNI:
		_zoneNI->onAnimationNextFrame(animationId, name, frame, frameCount);
		break;

	case kZoneRH:
		_zoneRH->onAnimationNextFrame(animationId, name, frame, frameCount);
		break;

	case kZoneFO:
		_zoneFO->onAnimationNextFrame(animationId, name, frame, frameCount);
		break;

	case kZoneRO:
		_zoneRO->onAnimationNextFrame(animationId, name, frame, frameCount);
		break;

	case kZoneAS:
		_zoneAS->onAnimationNextFrame(animationId, name, frame, frameCount);
		break;

	case kZoneWA:
		_zoneWA->onAnimationNextFrame(animationId, name, frame, frameCount);
		break;

	case kZoneN2:
		_zoneN2->onAnimationNextFrame(animationId, name, frame, frameCount);
		break;
	}
}

void ApplicationRing::onAnimation(uint32 type, Id animationId, const Common::String &name, uint32 frame, uint32 a5) {
	if (getCurrentZone() == kZoneWA)
		_zoneWA->onAnimation(type, animationId, name, frame, a5);
}

void ApplicationRing::onVisualList(Id id, uint32 type, const Common::Point &point) {
	if (getCurrentZone() == kZoneSY)
		_zoneSystem->onVisualList(id, type, point);
}

#pragma endregion

#pragma region Helper functions

void ApplicationRing::onSetupLoadTimers(Common::String zoneName, Id testId1, Id puzzleRotationId, Id testId2) {
	if (!getSaveManager()->hasTimer(zoneName))
		error("[EventSetupRing::onSetupLoadTimers] Cannot find savegame (%s.ars)!", zoneName.c_str());

	bagRemoveAll();

	if (varGetByte(testId1)) {
		puzzleSetActive((PuzzleId)varGetDword(puzzleRotationId));
	} else {
		rotationSetActive(varGetDword(puzzleRotationId));

		if (varGetByte(testId2))
			rotationSetFreOn(varGetDword(puzzleRotationId));
		else
			rotationSetFreOff(varGetDword(puzzleRotationId));
	}

	if (!getSaveManager()->loadSaveTimer(zoneName, kLoadSaveRead))
		error("[EventSetupRing::onSetupLoadTimers] Cannot load timers (%s)!", zoneName.c_str());

	getSoundManager()->playSounds();
}

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
