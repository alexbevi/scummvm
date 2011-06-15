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

#include "ring/base/application.h"

#include "ring/base/accessibility.h"
#include "ring/base/art.h"
#include "ring/base/bag.h"
#include "ring/base/cursor.h"
#include "ring/base/dialog.h"
#include "ring/base/event.h"
#include "ring/base/font.h"
#include "ring/base/language.h"
#include "ring/base/movability.h"
#include "ring/base/preferences.h"
#include "ring/base/puzzle.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"
#include "ring/base/timer.h"
#include "ring/base/var.h"
#include "ring/base/zone.h"

#include "ring/graphics/dragControl.h"
#include "ring/graphics/image.h"
#include "ring/graphics/hotspot.h"
#include "ring/graphics/movie.h"
#include "ring/graphics/screen.h"
#include "ring/graphics/visual.h"

#include "ring/sound/soundentry.h"
#include "ring/sound/soundhandler.h"
#include "ring/sound/soundmanager.h"

#include "ring/helpers.h"
#include "ring/ring.h"

#include "common/file.h"
#include "common/tokenizer.h"

namespace Ring {

Application::Application(RingEngine *engine) : _vm(engine),
	_screenManager(NULL),        _artHandler(NULL),          _fontHandler(NULL),   _dialogHandler(NULL),        _languageHandler(NULL),
	_isRotationCompressed(true), _archiveType(kArchiveFile), _cursorHandler(NULL), _loadFrom(kLoadFromInvalid), _field_5E(0),
	_soundHandler(NULL),         _state(kStateNone),         _field_6A(false),
	_currentGameZone(kZoneNone), _menuAction(kMenuAction0),               _field_74(false),     _field_75(false),            _field_76(false),
	_field_77(false),            _field_78(false),           _puzzle(NULL),        _rotation(NULL),             _bag(NULL),
	_timerHandler(NULL),         _var(NULL),                 _dragControl(NULL),   _objectHandler(NULL),        _preferenceHandler(NULL),
	_zoneHandler(NULL),          _eventHandler(NULL) {

	// Start managers
	_saveManager = new SaveManager(this);
	_soundManager = new SoundManager(this, _vm->_mixer);

	_currentRotation = NULL;

	_thumbnail = NULL;

	// System zone
	_systemZone = kZone1;
}

Application::~Application() {
	SAFE_DELETE(_screenManager);
	SAFE_DELETE(_artHandler);
	SAFE_DELETE(_fontHandler);
	SAFE_DELETE(_dialogHandler);
	SAFE_DELETE(_languageHandler);
	SAFE_DELETE(_cursorHandler);
	SAFE_DELETE(_soundHandler);
	CLEAR_ARRAY(Object, _objects);
	CLEAR_ARRAY(Puzzle, _puzzles);
	CLEAR_ARRAY(Rotation, _rotations);
	SAFE_DELETE(_bag);
	SAFE_DELETE(_timerHandler);
	SAFE_DELETE(_var);
	SAFE_DELETE(_dragControl);
	SAFE_DELETE(_objectHandler);
	SAFE_DELETE(_preferenceHandler);
	SAFE_DELETE(_zoneHandler);

	SAFE_DELETE(_saveManager);
	SAFE_DELETE(_soundManager);
	SAFE_DELETE(_eventHandler);

	_puzzle = NULL;
	_rotation = NULL;
	_currentRotation = NULL;

	SAFE_DELETE(_thumbnail);

	// Zero-out passed pointers
	_vm = NULL;
}

void Application::saveLoadWithSerializer(Common::Serializer &s) {
	if (!_var || !_bag || !_timerHandler)
		error("[Application::saveLoadWithSerializer] Application not initialized properly");

	for (Common::Array<Puzzle *>::iterator it = _puzzles.begin(); it != _puzzles.end(); it++)
		(*it)->saveLoadWithSerializer(s);

	for (Common::Array<Rotation *>::iterator it = _rotations.begin(); it != _rotations.end(); it++)
		(*it)->saveLoadWithSerializer(s);

	for (Common::Array<Object *>::iterator it = _objects.begin(); it != _objects.end(); it++)
		(*it)->saveLoadWithSerializer(s);

	_var->saveLoadWithSerializer(s);
	_bag->saveLoadWithSerializer(s);
	_timerHandler->saveLoadWithSerializer(s);
}

#pragma region Initialization

void Application::init() {
	// Setup zone handler
	_zoneHandler = new ZoneHandler();

	// Setup available languages
	_languageHandler = new LanguageHandler();
	initLanguages();

	_field_5E = 0;

	// Load configuration
	loadConfiguration();

	// Setup system zone (needed by ArtHandler)
	addEpisode(kZoneSY, "System", 0);
	addZone(_systemZone, "System",     "SY", _configuration.artSY ? kArchiveArt : kArchiveFile, kLoadFromDisk);

	// Setup video
	_screenManager = new ScreenManager();
	_screenManager->init();
	_screenManager->setOffset(Common::Point(0, 0));

	// Setup objects
	_objectHandler = new ObjectHandler();
	_objectHandler->addFromFile("aObj.ini", getLanguageFolder());

	// Setup art
	if (_configuration.artBAG || _configuration.artCURSOR || _configuration.artSY)
		_archiveType = kArchiveArt;

	if (_archiveType == kArchiveArt) {
		_artHandler = new ArtHandler(this);
		_artHandler->open(_systemZone, kLoadFromDisk);
	}

	_archiveType = kArchiveFile;

	// Setup fonts
	_fontHandler = new FontHandler();
	initFont();

	// Setup dialogs
	_dialogHandler = new DialogHandler();
	_dialogHandler->init(kFontDefault, Color(200, 200, 30), Color(0, 0, 0), 461, 3);

	// Setup cursors
	_cursorHandler = new CursorHandler();

	// Setup sound
	_soundHandler = new SoundHandler();

	// Setup data
	_menuAction = kMenuAction0;
	initData();
	_loadFrom = kLoadFromCd;
	_isRotationCompressed = true;
	_archiveType = kArchiveFile;
	_currentGameZone = kZoneNone;

	_rotation = NULL;

	// Setup bag
	if (_configuration.artBAG)
		_archiveType = kArchiveArt;

	_bag= new Bag();
	initBag();

	_archiveType = kArchiveFile;

	// Setup timer
	_timerHandler = new TimerHandler();

	// Setup var
	_var = new Var();

	// Setup dragControl
	_dragControl = new DragControl();

	// Setup preferences
	_preferenceHandler = new PreferenceHandler(this);
	_preferenceHandler->load();
}

void Application::initLanguages() {
	languageAdd(kLanguageEnglish, "ENG", "ENG", 1);
	languageAdd(kLanguageFrench,  "FRA", "FRA", 1);
	languageAdd(kLanguageGerman,  "GER", "GER", 1);
	languageAdd(kLanguageItalian, "ITA", "ITA", 1);
	languageAdd(kLanguageSpanish, "SPA", "SPA", 1);
	languageAdd(kLanguageDutch,   "HOL", "HOL", 1);
	languageAdd(kLanguageSwedish, "SWE", "SWE", 1);
}

void Application::initData() {
	_field_74 = true;
	_field_75 = true;
	_field_76 = true;
	_field_77 = true;
	_field_78 = true;
}

void Application::loadConfiguration() {
	if (!_languageHandler)
		error("[Application::loadConfiguration] Language handler has not been initialized!");

	// Open a stream to the configuration file
	Common::SeekableReadStream *archive = SearchMan.createReadStreamForMember("fl.ini");
	if (!archive)
		error("[Application::loadConfiguration] Error opening configuration file (fl.ini)");

	// Read the number of lines
	Common::String firstLine = archive->readLine();
	if (archive->eos() || archive->err())
		error("[Application::loadConfiguration] Error reading file (eos or read error on first line)");

	int32 numLines = 0;
	if (sscanf(firstLine.c_str(), "%d", &numLines) != 1)
		error("[Application::loadConfiguration] Error reading number of lines");

	if (numLines > 0) {
		for (int32 i = 0; i < numLines; i++) {
			Common::String line = archive->readLine();
			char token[255];
			char val[255];

			// Check for errors and end-of-file
			if (archive->eos() || archive->err())
				error("[Application::loadConfiguration] Error reading file (eos or read error)");

			// Get token and value
			if (sscanf(line.c_str(), "%s %s", (char *)&token, (char *)&val) != 2)
				error("[Application::loadConfiguration] Error parsing configuration line (%d)", i + 2);

			// Handle tokens
			if (Common::String(token) == "CDPATH:") {

				// TODO: Show a proper message box to explain that we need a full DVD copy and not an existing installation
				if (Common::String(val) != "CDROM")
					error("[Application::loadConfiguration] Disk installation detected! We need full DVD copy of the game");

				_configuration.runningFromDisk = false;
			}

			if (Common::String(token) == "LANGUAGE:")
				_languageHandler->setActiveLanguage(val);

			if (Common::String(token) == "CHECKCD:")
				_configuration.checkCD = atoi((char *)&val);

			if (Common::String(token) == "SOUNDCHUNCK_BGRMUS:")
				_configuration.backgroundMusic.soundChunck = (uint32)atoi((char *)&val);

			if (Common::String(token) == "LOADFROM_BGRMUS:")
				_configuration.backgroundMusic.loadFrom = (LoadFrom)atoi((char *)&val);

			if (Common::String(token) == "SOUNDCHUNCK_AMBMUS:")
				_configuration.ambientMusic.soundChunck = (uint32)atoi((char *)&val);

			if (Common::String(token) == "LOADFROM_AMBMUS:")
				_configuration.ambientMusic.loadFrom = (LoadFrom)atoi((char *)&val);

			if (Common::String(token) == "SOUNDCHUNCK_AMBEFE:")
				_configuration.ambientEffect.soundChunck = (uint32)atoi((char *)&val);

			if (Common::String(token) == "LOADFROM_AMBEFE:")
				_configuration.ambientEffect.loadFrom = (LoadFrom)atoi((char *)&val);

			if (Common::String(token) == "SOUNDCHUNCK_EFE:")
				_configuration.effect.soundChunck = (uint32)atoi((char *)&val);

			if (Common::String(token) == "LOADFROM_EFE:")
				_configuration.effect.loadFrom = (LoadFrom)atoi((char *)&val);

			if (Common::String(token) == "SOUNDCHUNCK_DIA:")
				_configuration.dialog.soundChunck = (uint32)atoi((char *)&val);

			if (Common::String(token) == "LOADFROM_DIA")
				_configuration.dialog.loadFrom = (LoadFrom)atoi((char *)&val);

			if (Common::String(token) == "ART_BAG:")
				_configuration.artBAG = (bool)atoi((char *)&val);

			if (Common::String(token) == "ART_CURSOR:")
				_configuration.artCURSOR = (bool)atoi((char *)&val);

			if (Common::String(token) == "ART_SY:")
				_configuration.artSY = (bool)atoi((char *)&val);

			if (Common::String(token) == "ART_AS:")
				_configuration.artAS = (bool)atoi((char *)&val);

			if (Common::String(token) == "ART_NI:")
				_configuration.artNI = (bool)atoi((char *)&val);

			if (Common::String(token) == "ART_N2:")
				_configuration.artN2 = (bool)atoi((char *)&val);

			if (Common::String(token) == "ART_RO:")
				_configuration.artRO = (bool)atoi((char *)&val);

			if (Common::String(token) == "ART_RH:")
				_configuration.artRH = (bool)atoi((char *)&val);

			if (Common::String(token) == "ART_WA:")
				_configuration.artWA = (bool)atoi((char *)&val);

			if (Common::String(token) == "ART_FO:")
				_configuration.artFO = (bool)atoi((char *)&val);

			if (Common::String(token) == "CHECKLOADSAVE:")
				_configuration.checkLoadSave = (bool)atoi((char *)&val);
		}
	}

	delete archive;
}

void Application::setupZone(ZoneId zone, SetupType type) {
	debugC(kRingDebugLogic, "Setting up zone %s", getZoneName(zone).c_str());

	bool hasData = false;

	if (zone == kZoneSY) {
		hasData = true;
	} else {
		// The original checks for the correct CD,
		// we should instead check that the zone folder
		// has been copied properly
		warning("[Application::setupZone] Zone CD check not implemented");
	}

	reset();
	soundStopAll(8);

	if (_soundHandler)
		_soundHandler->reset();

	if (zone != kZoneSY)
		_artHandler->reset();

	if (hasData) {
		setCurrentEpisode(zone);
		setZone(zone, type);
	} else {
		_saveManager->setSetupType(type);

		messageFormat("InsertCD", Common::String::format("%d", getEpisodeCd(zone)));
		messageInsertCd(zone);
	}
}

void Application::setZone(ZoneId zone, SetupType type) {

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
	if (!_eventHandler)
		error("[Application::setZone] The event handler is not initialized properly");

	_eventHandler->onSetup(zone, type);
}

void Application::exitZone() {
	soundStopAll(128);
	_soundManager->clear();

	_field_74 = true;
	_field_75 = true;
	_field_76 = true;
	_field_77 = true;
	_field_78 = true;

	timerStopAll();
	varRemoveAll();
	cursorDelete();
	bagRemoveAll();
	reset();

	CLEAR_ARRAY(Puzzle,   _puzzles);
	CLEAR_ARRAY(Rotation, _rotations);
	CLEAR_ARRAY(Object,   _objects);

	if (_dragControl)
		_dragControl->reset();

	if (_soundHandler)
		_soundHandler->reset();

	_currentGameZone = kZoneNone;

	// Destroys the savegame current image
	SAFE_DELETE(_thumbnail);
}

void Application::exitToMenu(MenuAction menuAction) {
	_state = kStateShowMenu;
	_menuAction = menuAction;
	soundStopAll(64);
}

#pragma endregion

#pragma region Event handling

void Application::onTimer(TimerId id) {
	if (!_eventHandler)
		error("[Application::onTimer] Event handler not initialized properly!");

	if (_field_6A)
		return;

	_eventHandler->onTimer(id);
}

void Application::update(const Common::Point &point) {
	if (!_bag || !_eventHandler || !_dragControl)
		error("[Application::update] Application not initialized properly");

	// Handle bag
	if (_bag->isInitialized()) {
		if (_bag->checkHotspot(point)) {
			_eventHandler->onUpdateBag(point);

			return;
		}

		if (_dragControl->getField20())
			cursorSelect(kCursorPassiveDraw);
		else if (bagHasClickedObject())
			cursorSelect(kCursorPassive);
		else
			cursorSelect(kCursorIdle);

		_eventHandler->onUpdateBag(point);
	}

	// Handle drag control
	if (_dragControl->getField20() && _dragControl->getField45() == 2) {
		if (_dragControl->getHotspot() && _dragControl->getHotspot()->contains(point))
			cursorSelect(kCursorActiveDraw);
		else
			cursorSelect(kCursorPassiveDraw);

		return;
	}

	// Menu
	if (getPuzzle(kPuzzleMenu)) {
		Puzzle *puzzleMenu = getPuzzle(kPuzzleMenu);

		if (puzzleMenu->visualHandleUpdate(point))
			return;

		// Check accessibilities
		Accessibility *accessibility = puzzleMenu->getAccessibility(point);
		if (accessibility) {
			Object *object = accessibility->getObject();
			Hotspot *hotspot = accessibility->getHotspot();
			uint32 accessibilityIndex = (uint32)puzzleMenu->getAccessibilityIndex(point);
			bool handle = true;

			if (puzzleMenu->getField24() == 2)
				if (object->getId()	!= puzzleMenu->getSelectedId())
					handle = false;

			if (handle) {
				if (_dragControl->getField20()) {
					if (_dragControl->getAccessibilityIndex() == accessibilityIndex)
						_cursorHandler->select(kCursorActiveDraw);
					else
						_cursorHandler->select(kCursorPassiveDraw);
				} else {
					if (bagHasClickedObject())
						_cursorHandler->select(kCursorActive);
					else
						_cursorHandler->select(hotspot->getCursorId());
				}

				_eventHandler->onUpdateBefore(object->getId(), hotspot->getTarget(), accessibilityIndex, 1, point);
				return;
			}
		}

		// Check movabilities
		if (puzzleMenu->getField24() == 1) {
			Movability *movability = puzzleMenu->getMovability(point);
			if (movability) {
				Hotspot *hotspot = movability->getHotspot();

				_cursorHandler->select(hotspot->getCursorId());

				_eventHandler->onUpdateAfter(puzzleMenu->getId(), movability->getTo(), (uint32)puzzleMenu->getMovabilityIndex(point), hotspot->getTarget(), movability->getType(), point);
				return;
			}
		}

		if (puzzleMenu->getField24() == 2) {
			if (_dragControl->getField20()) {
				_cursorHandler->select(kCursorPassiveDraw);
			} else {
				if (bagHasClickedObject())
					_cursorHandler->select(kCursorPassive);
				else
					_cursorHandler->select(kCursorIdle);
			}

			_eventHandler->onUpdateBag(point);
			return;
		}
	}

	// Current rotation
	if (_rotation && !_rotation->getField28()) {

		// Check accessibilities
		Accessibility *accessibility = _rotation->getAccessibility(point);
		if (accessibility) {
			Object *object = accessibility->getObject();
			Hotspot *hotspot = accessibility->getHotspot();
			uint32 accessibilityIndex = (uint32)_rotation->getAccessibilityIndex(point);

			if (_dragControl->getField20()) {
				if (_dragControl->getAccessibilityIndex() == accessibilityIndex)
					_cursorHandler->select(kCursorActiveDraw);
				else
					_cursorHandler->select(kCursorPassiveDraw);
			} else {
				if (bagHasClickedObject())
					_cursorHandler->select(kCursorActive);
				else
					_cursorHandler->select(hotspot->getCursorId());
			}

			_eventHandler->onUpdateBefore(object->getId(), hotspot->getTarget(), accessibilityIndex, 0, point);
			return;
		}

		// Check movabilities
		Movability *movability = _rotation->getMovability(point);
		if (movability) {
			Hotspot *hotspot = movability->getHotspot();

			_cursorHandler->select(hotspot->getCursorId());

			_eventHandler->onUpdateAfter(_rotation->getId(), movability->getTo(), (uint32)_rotation->getMovabilityIndex(point), hotspot->getTarget(), movability->getType(), point);
			return;
		}
	}

	// Current puzzle
	if (_puzzle) {
		if (_puzzle->visualHandleUpdate(point))
			return;

		// Check accessibilities
		Accessibility *accessibility = _puzzle->getAccessibility(point);
		if (accessibility) {
			Object *object = accessibility->getObject();
			Hotspot *hotspot = accessibility->getHotspot();
			uint32 accessibilityIndex = (uint32)_puzzle->getAccessibilityIndex(point);

			if (_dragControl->getField20()) {
				if (_dragControl->getAccessibilityIndex() == accessibilityIndex)
					_cursorHandler->select(kCursorActiveDraw);
				else
					_cursorHandler->select(kCursorPassiveDraw);
			} else {
				if (bagHasClickedObject())
					_cursorHandler->select(kCursorActive);
				else
					_cursorHandler->select(hotspot->getCursorId());
			}

			_eventHandler->onUpdateBefore(object->getId(), hotspot->getTarget(), accessibilityIndex, 0, point);
			return;
		}

		// Check movabilities
		Movability *movability = _puzzle->getMovability(point);
		if (movability) {
			Hotspot *hotspot = movability->getHotspot();

			_cursorHandler->select(hotspot->getCursorId());

			_eventHandler->onUpdateAfter(_puzzle->getId(), movability->getTo(), (uint32)_puzzle->getMovabilityIndex(point), hotspot->getTarget(), movability->getType(), point);
			return;
		}

		if (_dragControl->getField20())
			cursorSelect(kCursorPassiveDraw);
		else if (bagHasClickedObject())
			cursorSelect(kCursorPassive);
		else
			cursorSelect(kCursorIdle);

		_eventHandler->onUpdateBag(point);
	}
}

void Application::updateBag(const Common::Point &point) {
	if (!_bag || !_eventHandler || !_dragControl)
		error("[Application::updateBag] Application not initialized properly");

	if (_bag->isInitialized() || !_dragControl->getField20())
		return;

	Hotspot *hotspot = NULL;

	if (_dragControl->getField45() == 2)
		hotspot = _dragControl->getHotspot();
	else
		hotspot = _dragControl->getHotspot2();

	if (hotspot && hotspot->contains(point)) {
		_dragControl->updateCoordinates(point);
		_eventHandler->onBag(_dragControl->getObjectId(), _dragControl->getTarget(), _dragControl->getPuzzleRotationId(), _dragControl->getField39(), _dragControl, 3);

		if (_state != kStateShowMenu) {
			if (!_field_76)
				_dragControl->reset();

			_field_76 = true;
		}
	}
}

#pragma endregion

#pragma region Display and Movies

void Application::drawZoneName(ZoneId zone) {
	if (!_screenManager)
		error("[Application::drawZoneName] Screen manager not initialized properly");

	// Draw the loading zone on screen
	_screenManager->clear();

    Common::String message = Common::String::format("Loading zone %s...", getZoneName(zone).c_str());
	_screenManager->drawText(message, Common::Point(10, 435), Color(246, 234, 219));

	_screenManager->updateScreen();
	g_system->updateScreen();

	// Poll events to prevent unresponsive application
	_vm->pollEvents();
}

void Application::showImage(Common::String filename, const Common::Point &point, uint32 ticksWait, LoadFrom loadFrom, ArchiveType archiveType) {
	if (archiveType == kArchiveInvalid)
		archiveType = getArchiveType(getCurrentZone());

	Common::String path;
	if (archiveType == kArchiveFile)
		path = Common::String::format("/DATA/%s/IMAGE/%s", getCurrentZoneFolder().c_str(), filename.c_str());
	else
		path = Common::String::format("/IMAGE/%s", filename.c_str());

	Image *image = new Image();
	if (!image->load(path, archiveType, getCurrentZone(), loadFrom, kDrawType1)) {
		delete image;
		return;
	}

	_screenManager->drawImage(image,  point, image->getWidth(), image->getHeight(), 0, 0);

	waitForEscape(ticksWait);

	delete image;
}

bool Application::scrollImage(Common::String filename, uint32 ticksWait, LoadFrom loadFrom, ArchiveType archiveType) {
	if (archiveType == kArchiveInvalid)
		archiveType = getArchiveType(getCurrentZone());

	Common::String path;
	if (archiveType == kArchiveFile)
		path = Common::String::format("/DATA/%s/IMAGE/%s", getCurrentZoneFolder().c_str(), filename.c_str());
	else
		path = Common::String::format("/IMAGE/%s", filename.c_str());

	Image *image = new Image();
	if (!image->load(path, archiveType, getCurrentZone(), loadFrom, kDrawType1)) {
		warning("[Application::scrollImage] Cannot load image (%s)", path.c_str());

		delete image;
		return false;
	}

	// Scroll image
	bool skipped = false;
	uint32 offset = 0;
	if (image->getWidth() > 448) {

		while (offset < image->getWidth() - 448 && !skipped) {
			_screenManager->drawImage(image, Common::Point(0, 16), 640, 464, 0, offset);

			// Skip if ESCAPE is pressed
			if (checkEscape())
				skipped = true;
		}
	}

	if (offset == image->getWidth() - 448)
		waitForEscape(ticksWait);

	delete image;

	return skipped;
}

void Application::displayFade(Common::String filenameFrom, Common::String filenameTo, uint32 frameCount, uint32 ticksWait, LoadFrom loadFrom, ArchiveType archiveType) {
	Graphics::Surface surface;
	Image *imageTo = NULL;
	Animation *animation = NULL;
	uint16 *diff = NULL;
	uint16 *srcFrom = NULL;
	uint16 *srcTo = NULL;
	Graphics::PixelFormat format = g_system->getScreenFormat();

	if (archiveType == kArchiveInvalid)
		archiveType = getArchiveType(getCurrentZone());

	// Compute paths
	Common::String pathFrom;
	Common::String pathTo;
	if (archiveType == kArchiveFile) {
		pathFrom = Common::String::format("DATA/%s/IMAGE/%s", getCurrentZoneFolder().c_str(), filenameFrom.c_str());
		pathTo   = Common::String::format("DATA/%s/IMAGE/%s", getCurrentZoneFolder().c_str(), filenameTo.c_str());
	} else {
		pathFrom = Common::String::format("/IMAGE/%s", filenameFrom.c_str());
		pathTo   = Common::String::format("/IMAGE/%s", filenameTo.c_str());
	}

	// Load images
	Image *imageFrom = new Image();
	if (!imageFrom->load(pathFrom, archiveType, getCurrentZone(), loadFrom, kDrawType1)) {
		warning("[Application::displayFade] Cannot load imageFrom (%s)", pathFrom.c_str());
		goto cleanup;
	}

	imageTo = new Image();
	if (!imageTo->load(pathTo, archiveType, getCurrentZone(), loadFrom, kDrawType1)) {
		warning("[Application::displayFade] Cannot load imageTo (%s)", pathTo.c_str());
		goto cleanup;
	}

	// Check dimensions
	if (imageFrom->getHeight() != imageTo->getHeight()) {
		warning("[Application::displayFade] Heights are different (from: %d, to: %d)", imageFrom->getHeight(), imageTo->getHeight());
		goto cleanup;
	}

	if (imageFrom->getWidth() != imageTo->getWidth()) {
		warning("[Application::displayFade] Widths are different (from: %d, to: %d)", imageFrom->getWidth(), imageTo->getWidth());
		goto cleanup;
	}

	// Check bpp
	if (imageFrom->getBPP() != 24 || imageTo->getBPP() != 24) {
		warning("[Application::displayFade] Depths are not 24bpp (from: %d, to: %d)", imageFrom->getBPP(), imageTo->getBPP());
		goto cleanup;
	}

	// Create new surface to hold the difference frame
	format.bytesPerPixel = imageFrom->getBPP() / 8;
	surface.create((uint16)imageFrom->getWidth(), (uint16)imageFrom->getHeight(), format);

	// Put difference frame data into surface
	diff    = (uint16 *)surface.pixels;
	srcFrom = (uint16 *)imageFrom->getSurface()->pixels;
	srcTo   = (uint16 *)imageTo->getSurface()->pixels;
	for (uint32 i = 0; i < (uint32)(surface.w * surface.h * surface.format.bytesPerPixel); i++)
		diff[i] = (srcFrom[i] - srcTo[i]) / (uint16)frameCount;

	// Create animation
	animation = new Animation();
	animation->init(frameCount, 25.0f, 1, 4, 0);
	animation->setField21(2);
	animation->setTicks(g_system->getMillis());

	if (frameCount >= 1) {

		uint32 currentFrame = 1;
		while (!checkEscape()) {
			// Update imageFrom buffer
			for (uint32 i = 0; i < (uint32)(surface.w * surface.h * surface.format.bytesPerPixel); i++)
				srcFrom[i] -= diff[i];

			// Draw updated frame
			_screenManager->drawAndUpdate(imageFrom, Common::Point(0, 16));

			// Progress to next frame
			while (animation->getCurrentFrame() == currentFrame)
				handleEvents();

			uint32 nextFrame = animation->getActiveFrame() + 1;
			if (nextFrame < currentFrame)
				break;

			currentFrame = nextFrame;

			if (nextFrame > frameCount)
				break;
		}
	}

	// Draw target image
	_screenManager->drawAndUpdate(imageTo, Common::Point(0, 16));

	// Cleanup
	surface.free();

	waitForEscape(ticksWait);

cleanup:
	SAFE_DELETE(animation);
	SAFE_DELETE(imageFrom);
	SAFE_DELETE(imageTo);
}

void Application::fadeOut(uint32 frameCount, const Color &colorTo, uint32 ticksWait) {
	warning("[Application::fadeOut] Not implemented");
}

void Application::waitForEscape(uint32 ticksWait) const {
	for (uint32 i = 0; i < ticksWait; i++)
		if (checkEscape())
			break;
}

void Application::playMovie(Common::String filename, float frameDivider) {
	soundStopType(kSoundTypeEffect, 256);
	soundStopType(kSoundTypeDialog, 256);
	cursorSelect(kCursorDefault);

	// Compute full path
	if (!filename.hasSuffix(".cnm"))
		filename += ".cnm";

	Common::String path = Common::String::format("DATA/%s/PLA/", getCurrentZoneFolder().c_str());

	Movie *movie = new Movie(_screenManager);
	if (movie->init(path, filename, 1, 0)) {
		if (frameDivider != 0.0f)
			movie->setFramerate(1000.0f / frameDivider);

		movie->play(Common::Point(0, 16));
	}

	delete movie;
}

void Application::playMovieChannel(Common::String filename, uint32 channel) {
	if (channel < 1 || channel > 3)
		error("[Application::playMovieChannel] Invalid channel for movie %s (was:%d, valid=[1-3]", filename.c_str(), channel);

	soundStopType(kSoundTypeEffect, 256);
	soundStopType(kSoundTypeDialog, 256);
	cursorSelect(kCursorDefault);

	// Compute full path
	if (!filename.hasSuffix(".cnm"))
		filename += ".cnm";

	Common::String path = Common::String::format("DATA/%s/PLA/", getCurrentZoneFolder().c_str());

	Movie *movie = new Movie(_screenManager);
	if (movie->init(path, filename, 1, channel))
		movie->play(Common::Point(0, 16));

	delete movie;
}

Common::String Application::getFileExtension(ImageType imageType) {
	switch (imageType) {
	default:            return "";
	case kImageTypeBMP: return "bmp";
	case kImageTypeTGA: return "tga";
	case kImageTypeCIN: return "cin";
	case kImageTypeCNM: return "cnm";
	case kImageTypeBMA: return "bma";
	case kImageTypeTGC: return "tgc";
	}
}

#pragma endregion

#pragma region Language, Fonts and Cursors

void Application::languageAdd(LanguageId id, Common::String name, Common::String folder, uint channel) {
	if (!_languageHandler)
		error("[Application::languageAdd] Language handler is not initialized properly");

	_languageHandler->add(id, name, folder, channel);
}

LanguageId Application::getCurrentLanguage() {
	if (!_languageHandler)
		error("[Application::getLanguage] Language handler is not initialized properly");

	return _languageHandler->getLanguage();
}

Common::String Application::getLanguageFolder() {
	LanguageId id = getCurrentLanguage();

	return _languageHandler->getFolder(id);
}

uint32 Application::getLanguageChannel() {
	LanguageId id = getCurrentLanguage();

	return _languageHandler->getChannel(id);
}

bool Application::hasLanguagePack() {
	uint32 packId = 1;

	switch (getCurrentLanguage()) {
	default:
		break;

	case kLanguageItalian:
	case kLanguageSpanish:
	case kLanguageSwedish:
	case kLanguageDutch:
		packId = 2;
		break;
	}

	// Open pack.ini and check language pack id
	Common::SeekableReadStream *archive = SearchMan.createReadStreamForMember("pack.ini");
	if (!archive) {
		warning("[Application::hasLanguagePack] Error opening pack.ini file");
		return false;
	}

	Common::String pack = archive->readLine();
	if (archive->eos() || archive->err()) {
		warning("[Application::hasLanguagePack] Cannot read from pack.ini file");
		return false;
	}

	uint32 id = 0;
	if (sscanf(pack.c_str(), "%d", (int *)&id) != 1) {
		warning("[Application::hasLanguagePack] Cannot parse pack from pack.ini (%s)", pack.c_str());
		return false;
	}

	return (id == packId);
}

void Application::fontAdd(FontId id, Common::String filename, Common::String facename, uint32 height, bool smallWeight, bool underline, bool italic, bool strikeout, LanguageId langId) {
	if (!_fontHandler)
		error("[Application::fontAdd] Font handler is not initialized properly");

	_fontHandler->add(id, filename, facename, height, smallWeight, underline, italic, strikeout, langId);
}

void Application::cursorAdd(CursorId id, Common::String name, CursorType cursorType, byte imageCount, LoadFrom loadFrom, ArchiveType archiveType) {
	if (!_cursorHandler)
		error("[Application::cursorAdd] Cursor handler is not initialized properly");

	_cursorHandler->add(id, name, cursorType, imageCount, 0, 0, 0, loadFrom, archiveType);
}

void Application::cursorAdd(CursorId id, Common::String name, CursorType cursorType, byte imageCount, uint32 frameCount, float frameRate, byte a6, LoadFrom loadFrom, ArchiveType archiveType) {
	if (!_cursorHandler)
		error("[Application::cursorAdd] Cursor handler is not initialized properly");

	_cursorHandler->add(id, name, cursorType, imageCount, frameCount, frameRate, a6, loadFrom, archiveType);
}

void Application::cursorSetOffset(CursorId id, const Common::Point &offset) {
	if (!_cursorHandler)
		error("[Application::cursorSetOffset] Cursor handler is not initialized properly");

	_cursorHandler->setOffset(id, offset);
}

void Application::subtitleSetColor(const Color &color) {
	if (!_dialogHandler)
		error("[Application::subtitleSetColor] Dialog handler is not initialized properly");

	_dialogHandler->setSubtitleColor(color);
}

void Application::subtitleSetBackgroundColor(const Color &color) {
	if (!_dialogHandler)
		error("[Application::subtitleSetBackgroundColor] Dialog handler is not initialized properly");

	_dialogHandler->setSubtitleBackgroundColor(color);
}

#pragma endregion

#pragma region Bag

void Application::bagAdd(ObjectId objectId) {
	if (objectId == kObjectInvalid)
		error("[Application::bagAdd] Invalid ID (%d)", objectId.id());

	if (!_bag)
		error("[Application::bagAdd] bag is not initialized properly");

	if (!_objects.has(objectId))
		error("[Application::bagAdd] ID doesn't exist (%d)", objectId.id());

	_bag->add(objectId);
}

void Application::bagRemove(ObjectId objectId) {
	if (!_bag)
		error("[Application::bagRemove] bag is not initialized properly");

	if (!_objects.has(objectId))
		error("[Application::bagRemove] ID doesn't exist (%d)", objectId.id());

	_bag->remove(objectId);
}

void Application::bagRemoveAll() {
	if (!_bag)
		error("[Application::bagRemoveAll] bag is not initialized properly");

	_bag->removeAll();
}

bool Application::bagHas(ObjectId objectId) {
	if (!_bag)
		error("[Application::bagHas] bag is not initialized properly");

	if (!_objects.has(objectId))
		error("[Application::bagHas] ID doesn't exist (%d)", objectId.id());

	return _bag->has(objectId);
}

void Application::bagSetClickedObject(ObjectId objectId) {
	if (!_bag)
		error("[Application::bagSetClickedObject] bag is not initialized properly");

	if (!_objects.has(objectId))
		error("[Application::bagSetClickedObject] ID doesn't exist (%d)", objectId.id());

	_bag->setClickedObject(objectId);
}

bool Application::bagHasClickedObject() {
	if (!_bag)
		error("[Application::bagHasClickedObject] bag is not initialized properly");

	return _bag->getClickedObject() != kObjectInvalid;
}

ObjectId Application::bagGetClickedObject() {
	if (!_bag)
		error("[Application::bagGetClickedObject] bag is not initialized properly");

	return _bag->getClickedObject();
}

#pragma endregion

#pragma region Cursor

void Application::cursorSelect(CursorId id) {
	if (_cursorHandler)
		_cursorHandler->select(id);
}

void Application::cursorDelete() {
	if (_bag)
		_bag->setClickedObject(kObjectInvalid);

	if (_cursorHandler)
		_cursorHandler->removeByType(kCursorType2);
}

void Application::cursorRemoveByType(CursorType type) {
	if (_cursorHandler)
		_cursorHandler->removeByType(type);
}

#pragma endregion

#pragma region Message

void Application::messageShowWarning(uint32 accelerationIndex) {
	if (!puzzleSetMod(kPuzzleMenu, 2, 3))
		return;

	objectPresentationSetTextToPuzzle(kObject3, 0, 0, _messageType);
	objectPresentationSetTextCoordinatesToPuzzle(kObject3, 0, 0, Common::Point(225, 193));
	objectPresentationSetTextToPuzzle(kObject3, 0, 1, _message);
	objectPresentationSetTextCoordinatesToPuzzle(kObject3, 0, 1, Common::Point(225, 213));
	objectPresentationShow(kObject3, 0);
	objectSetAccessibilityOn(kObject3, accelerationIndex, accelerationIndex);
}

void Application::messageHideWarning(uint32 accelerationIndex) {
	objectPresentationHideAndRemove(kObject3);
	objectSetAccessibilityOff(kObject3, accelerationIndex, accelerationIndex);
	puzzleSetMod(kPuzzleMenu, 1, 0);
}

void Application::messageShowQuestion(uint32 accelerationIndex) {
	if (!puzzleSetMod(kPuzzleMenu, 2, 4))
		return;

	objectPresentationSetTextToPuzzle(kObject4, 0, 0, _messageType);
	objectPresentationSetTextCoordinatesToPuzzle(kObject4, 0, 0, Common::Point(225, 193));
	objectPresentationSetTextToPuzzle(kObject4, 0, 1, _message);
	objectPresentationSetTextCoordinatesToPuzzle(kObject4, 0, 1, Common::Point(225, 213));
	objectPresentationShow(kObject4, 0);
	objectSetAccessibilityOn(kObject4, accelerationIndex, accelerationIndex + 1);
}

void Application::messageHideQuestion(uint32 accelerationIndex) {
	objectPresentationHideAndRemove(kObject4);
	objectSetAccessibilityOff(kObject4, accelerationIndex, accelerationIndex + 1);
	puzzleSetMod(kPuzzleMenu, 1, 0);
}

bool Application::messageGet(Common::String messageId) {
	// Open a stream to the message file
	Common::SeekableReadStream *archive = SearchMan.createReadStreamForMember("ames.ini");
	if (!archive)
		error("[Application::messageGet] Error opening objects file (ames.ini)");

	Common::String language = getLanguageFolder();

	char id[255];
	while (!archive->eos() && !archive->err()) {

		Common::String line = archive->readLine();
		if (archive->eos() || archive->err())
			break;

		// Parse line for message id
		if (sscanf(line.c_str(), "%s\n", (char *)&id) != 1)
			error("[Application::messageGet] Error parsing message line!");

		// Check message id
		if (messageId != Common::String(id))
			continue;

		// Check language
		if (!line.matchString(language + "*", true))
			continue;

		// Get message type
		Common::StringTokenizer tokenizer(line, "#");
		if (tokenizer.empty())
			error("[Application::messageGet] Invalid line format (missing # separators)");

		tokenizer.nextToken();
		if (tokenizer.empty())
			error("[Application::messageGet] Invalid line format (missing message type)");

		_messageType = tokenizer.nextToken();
		if (tokenizer.empty())
			error("[Application::messageGet] Invalid line format (missing message)");

		_message = tokenizer.nextToken();

		delete archive;

		return true;
	}

	delete archive;

	return false;
}

void Application::messageFormat(Common::String messageId, Common::String argument) {
	if (messageGet(messageId))
		_message = Common::String::format(_message.c_str(), argument.c_str());
}

#pragma endregion

#pragma region Puzzle

void Application::puzzleAdd(PuzzleId puzzleId) {
	if (_puzzles.has(puzzleId))
		error("[Application::addPuzzle] ID already exists (%d)", puzzleId.id());

	_puzzles.push_back(new Puzzle(this, puzzleId));
}

Puzzle *Application::getPuzzle(PuzzleId puzzleId) {
	if (!_puzzles.has(puzzleId))
		return NULL;

	return _puzzles.get(puzzleId);
}

void Application::puzzleAddBackgroundImage(PuzzleId puzzleId, Common::String filename, const Common::Point &point, bool isActive) {
	if (!_puzzles.has(puzzleId))
		error("[Application::puzzleAddBackgroundImage] ID doesn't exist (%d)", puzzleId.id());

	_puzzles.get(puzzleId)->setBackgroundImage(filename, point, isActive, _loadFrom);
}

#pragma region Puzzle Movability

void Application::puzzleAddMovabilityToPuzzle(PuzzleId puzzleIdFrom, PuzzleId puzzleIdTo, Common::String name, const Common::Rect &rect, bool enabled, CursorId cursorId, uint32 a10) {
	if (!_puzzles.has(puzzleIdFrom))
		error("[Application::puzzleAddMovabilityToPuzzle] Wrong FROM puzzle Id (%d)", puzzleIdFrom.id());

	if (!_puzzles.has(puzzleIdTo))
		error("[Application::puzzleAddMovabilityToPuzzle] Wrong TO puzzle Id (%d)", puzzleIdTo.id());

	Movability *movability = new Movability(puzzleIdFrom, puzzleIdTo, name, kMovabilityPuzzleToPuzzle);
	movability->setHotspot(rect, enabled, cursorId, a10);

	_puzzles.get(puzzleIdFrom)->addMovability(movability);
}

void Application::puzzleAddMovabilityToRotation(PuzzleId puzzleIdFrom, Id rotationIdTo, Common::String name, const Common::Rect &rect, bool enabled, CursorId cursorId, uint32 a10) {
	if (!_rotations.has(rotationIdTo))
		error("[Application::puzzleAddMovabilityToRotation] Wrong TO rotation Id (%d)", rotationIdTo);

	if (!_puzzles.has(puzzleIdFrom))
		error("[Application::puzzleAddMovabilityToRotation] Wrong FROM puzzle Id (%d)", puzzleIdFrom.id());

	Movability *movability = new Movability(puzzleIdFrom, rotationIdTo, name, kMovabilityPuzzleToRotation);
	movability->setHotspot(rect, enabled, cursorId, a10);

	_puzzles.get(puzzleIdFrom)->addMovability(movability);
}

void Application::puzzleSetMovabilityToRotation(PuzzleId puzzleId, uint32 movabilityIndex, float a3, float a4, float a5) {
	if (!_puzzles.has(puzzleId))
		error("[Application::puzzleSetMovabilityToRotation] Wrong puzzle Id (%d)", puzzleId.id());

	Movability *movability = _puzzles.get(puzzleId)->getMovability(movabilityIndex);
	if (movability->getType() != kMovabilityPuzzleToRotation)
		error("[Application::puzzleSetMovabilityToRotation] Invalid type of movability (%d)", movability->getType());

	movability->update(0.0, 0.0, 85.0, 0.0, 2, a3, a4, a5);
}

void Application::puzzleSetMovabilityOnOrOff(PuzzleId puzzleId, bool enableHotspot, uint32 fromMovability, uint32 toMovability) {
	if (!_puzzles.has(puzzleId))
		error("[Application::puzzleSetMovabilityOnOrOff] Puzzle Id doesn't exist (%d)", puzzleId.id());

	_puzzles.get(puzzleId)->setMovabilityOnOrOff(enableHotspot, fromMovability, toMovability);
}

void Application::puzzleSetMovabilityOnOrOff(PuzzleId puzzleId, bool enableHotspot) {
	if (!_puzzles.has(puzzleId))
		error("[Application::puzzleSetMovabilityOnOrOff] Puzzle Id doesn't exist (%d)", puzzleId.id());

	_puzzles.get(puzzleId)->setMovabilityOnOrOff(enableHotspot);
}

void Application::puzzleSetMovabilityOn(const PuzzleId &puzzleId, uint32 fromMovability, uint32 toMovability) {
	puzzleSetMovabilityOnOrOff(puzzleId, true, fromMovability, toMovability);
}

void Application::puzzleSetMovabilityOff(const PuzzleId &puzzleId, uint32 fromMovability, uint32 toMovability) {
	puzzleSetMovabilityOnOrOff(puzzleId, false, fromMovability, toMovability);
}

void Application::puzzleSetMovabilityOn(const PuzzleId &puzzleId) {
	puzzleSetMovabilityOnOrOff(puzzleId, true);
}

void Application::puzzleSetMovabilityOff(const PuzzleId &puzzleId) {
	puzzleSetMovabilityOnOrOff(puzzleId, false);
}

#pragma endregion

bool Application::puzzleSetMod(PuzzleId puzzleId, uint32 a2, uint32 a3) {
	if (!_puzzles.has(puzzleId))
		error("[Application::puzzleAddAmbientSound] Wrong puzzle Id (%d)", puzzleId.id());

	Puzzle *puzzle = _puzzles.get(puzzleId);
	if (puzzle->getField24() != 2 || a2 != 2) {
		puzzle->setMod(a2, a3);

		return true;
	}

	return false;
}

void Application::puzzleAddAmbientSound(PuzzleId puzzleId, Id soundId, uint32 volume, int32 pan, uint32 fadeFrames, uint32 a6, uint32 a7) {
	if (!_puzzles.has(puzzleId))
		error("[Application::puzzleAddAmbientSound] Wrong puzzle Id (%d)", puzzleId.id());

	SoundEntry *entry = _soundManager->getEntry(soundId);
	if (!entry)
		error("[Application::puzzleAddAmbientSound] Wrong sound Id (%d)", soundId);

	if (entry->getType() != kSoundTypeAmbientMusic)
		error("[Application::puzzleAddAmbientSound] Wrong sound type, only kSoundTypeAmbientMusic(2) is allowed (%d)", entry->getType());

	_puzzles.get(puzzleId)->addAmbientSound(entry, volume, pan, true, fadeFrames, a6, a7);
}

void Application::puzzleSetAmbientSoundOn(PuzzleId puzzleId, Id soundId) {
	if (!_puzzles.has(puzzleId))
		error("[Application::puzzleSetAmbientSoundOn] Wrong puzzle Id (%d)", puzzleId.id());

	_puzzles.get(puzzleId)->setAmbientSoundOn(soundId);
}

void Application::puzzleSetAmbientSoundOff(PuzzleId puzzleId, Id soundId) {
	if (!_puzzles.has(puzzleId))
		error("[Application::puzzleSetAmbientSoundOff] Wrong puzzle Id (%d)", puzzleId.id());

	_puzzles.get(puzzleId)->setAmbientSoundOff(soundId);
}

void Application::puzzleAdd3DSound(PuzzleId puzzleId, Id soundId, uint32 a3, uint32 a4, uint32 fadeFrames, uint32 volume, float a7, uint32 a8) {
	if (!_puzzles.has(puzzleId))
		error("[Application::puzzleAdd3DSound] Wrong puzzle Id (%d)", puzzleId.id());

	SoundEntry *entry = _soundManager->getEntry(soundId);
	if (!entry)
		error("[Application::puzzleAdd3DSound] Wrong sound Id (%d)", soundId);

	if (entry->getType() != kSoundTypeAmbientEffect)
		error("[Application::puzzleAdd3DSound] Wrong sound type, only kSoundTypeAmbientEffect(3) is allowed (%d)", entry->getType());

	_puzzles.get(puzzleId)->add3DSound(entry, volume, true, a3, a4, fadeFrames, a7, a8);
}

void Application::puzzleSet3DSoundOn(const PuzzleId &puzzleId, Id soundId) {
	puzzleSetAmbientSoundOn(puzzleId, soundId);
}

void Application::puzzleSet3DSoundOff(const PuzzleId &puzzleId, Id soundId) {
	puzzleSetAmbientSoundOff(puzzleId, soundId);
}

void Application::puzzleSet3DSoundVolume(PuzzleId puzzleId, Id soundId, int32 volume) {
	if (!_puzzles.has(puzzleId))
		error("[Application::puzzleSet3DSoundVolume] Wrong puzzle Id (%d)", puzzleId.id());

	_puzzles.get(puzzleId)->setAmbientSoundVolume(soundId, volume);
}

void Application::puzzleSetActive(PuzzleId id, bool updateSoundItems, bool a3) {
	// Setup puzzle
	if (!_puzzles.has(id))
		return;

	reset();
	_puzzle = _puzzles.get(id);
	_puzzle->alloc();
	_puzzle->update();

	_state = kStateUpdatePuzzle;

	if (_soundHandler->getField0()) {
		_soundHandler->turnOnItems4();

		_soundHandler->reset();
		_soundHandler->setSounds1(_puzzle->getSoundItems());
		return;
	}

	_soundHandler->setSounds2(_puzzle->getSoundItems());
	if (_soundHandler->processSounds()) {

		_soundHandler->turnOffItems1();

		if (_soundHandler->updateItems(2)) {

			_soundHandler->updateItems2(3);
			_soundHandler->updateItems3(3);
			_soundHandler->turnOnItems4();

			_soundHandler->reset();
			_soundHandler->setSounds1(_puzzle->getSoundItems());
			return;
		}
	}

	_soundHandler->turnOffSounds1(a3);

	if (updateSoundItems)
		_puzzle->updateSoundItems();

	_soundHandler->reset();
	_soundHandler->setSounds1(_puzzle->getSoundItems());
}

void Application::reset() {
	// Those are reference to existing objects in the _puzzles and _rotations arrays
	if (_puzzle)
		_puzzle->dealloc();
	_puzzle = NULL;

	if (_rotation)
		_rotation->dealloc();
	_rotation = NULL;

	soundStop(kSoundTypeEffect, 16);
	soundStop(kSoundTypeDialog, 16);
}

PuzzleId Application::getCurrentPuzzleId() {
	if (!_puzzle)
		return kPuzzleInvalid;

	return (PuzzleId)_puzzle->getId();
}

Id Application::getCurrentRotationId() {
	if (!_rotation)
		return 0;

	return _rotation->getId();
}

void Application::setupCurrentRotation() {
	if (!hasCurrentRotation()) {
		_currentRotation = NULL;
		return;
	}

	Rotation *rotation = getCurrentRotation();
	rotation->setFreOnOff(true);

	_currentRotation = rotation;
}

void Application::setFreOffCurrentRotation() {
	if (_currentRotation)
		_currentRotation->setFreOnOff(false);
}

#pragma endregion

#pragma region Object

void Application::objectAdd(ObjectId objectId, Common::String description, Common::String name, byte a5) {
	if (!_objectHandler)
		error("[Application::objectAdd] Object handler not initialized properly");

	if (_objects.has(objectId))
		error("[Application::ObjectAdd] ID already exists (%d)", objectId.id());

	// Compute language and name
	Common::String processedDescription = _objectHandler->getDescription(objectId);
	if (processedDescription.empty())
		processedDescription = description;

	Common::String processedName = _objectHandler->getName(objectId);
	if (processedName.empty())
		processedName = name;

	_objects.push_back(new Object(this, objectId, processedDescription, processedName, a5));
}

void Application::objectAddBagAnimation(ObjectId objectId, ImageType imageType, DrawType drawType, uint32 frameCount, float framerate, byte a6) {
	if (!_objects.has(objectId))
		error("[Application::objectAddBagAnimation] Object Id doesn't exist (%d)", objectId.id());

	Object *object = _objects.get(objectId);
	AnimationImage *image = new AnimationImage();
	image->init(object->getName(), imageType, Common::Point(0, 0), 0, drawType, frameCount, framerate, 1, a6, 0, 1000, kLoadFromListIcon, (_configuration.artBAG ? kArchiveArt : kArchiveFile));
	image->setField89();

	object->setAnimationImage(image);
}

void Application::objectRemove(ObjectId id) {
	_objects.remove(id);
}

Object *Application::getObject(ObjectId objectId) {
	if (!_objects.has(objectId))
		error("[Application::objectGet] Object Id doesn't exist (%d)", objectId.id());

	return _objects.get(objectId);
}

#pragma region Object Accessibility

void Application::objectAddPuzzleAccessibility(ObjectId objectId, PuzzleId puzzleId, const Common::Rect &rect, bool enabled, CursorId cursorId, uint32 a10) {
	if (!_objects.has(objectId))
		error("[Application::objectAddPuzzleAccessibility] Object Id doesn't exist (%d)", objectId.id());

	if (!_puzzles.has(puzzleId))
		error("[Application::objectAddPuzzleAccessibility] Puzzle Id doesn't exist (%d)", puzzleId.id());

	_objects.get(objectId)->addPuzzleAccessibility(_puzzles.get(puzzleId), rect, enabled, cursorId, a10);
}

void Application::objectSetPuzzleAccessibilityKey(ObjectId objectId, uint32 accessibilityIndex, Common::KeyCode key) {
	if (!_objects.has(objectId))
		error("[Application::objectSetPuzzleAccessibilityKey] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setAccessibilityKey(accessibilityIndex, key);
}


void Application::objectSetAccessibilityOnOrOff(ObjectId objectId, bool enableHotspot, uint32 fromAcceleration, uint32 toAcceleration) {
	if (!_objects.has(objectId))
		error("[Application::objectSetAccessibilityOnOrOff] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setAccessibilityOnOrOff(enableHotspot, fromAcceleration, toAcceleration);
}

void Application::objectSetAccessibilityOnOrOff(ObjectId objectId, bool enableHotspot) {
	if (!_objects.has(objectId))
		error("[Application::objectSetAccessibilityOnOrOff] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setAccessibilityOnOrOff(enableHotspot);
}

void Application::objectSetAccessibilityOn(const ObjectId &objectId, uint32 fromAcceleration, uint32 toAcceleration) {
	objectSetAccessibilityOnOrOff(objectId, true, fromAcceleration, toAcceleration);
}

void Application::objectSetAccessibilityOff(const ObjectId &objectId, uint32 fromAcceleration, uint32 toAcceleration) {
	objectSetAccessibilityOnOrOff(objectId, false, fromAcceleration, toAcceleration);
}

void Application::objectSetAccessibilityOn(const ObjectId &objectId) {
	objectSetAccessibilityOnOrOff(objectId, true);
}

void Application::objectSetAccessibilityOff(const ObjectId &objectId) {
	objectSetAccessibilityOnOrOff(objectId, false);
}

#pragma endregion

#pragma region Object Rotation

void Application::objectAddRotationAccessibility(ObjectId objectId, Id rotationId, const Common::Rect &rect, bool enabled, CursorId cursorId, uint32 a10) {
	if (!_objects.has(objectId))
		error("[Application::objectAddRotationAccessibility] Object Id doesn't exist (%d)", objectId.id());

	if (!_rotations.has(rotationId))
		error("[Application::objectAddRotationAccessibility] Rotation Id doesn't exist (%d)", rotationId);

	_objects.get(objectId)->addRotationAccessibility(_rotations.get(rotationId), rect, enabled, cursorId, a10);
}

#pragma endregion

void Application::objectSetActiveCursor(ObjectId objectId, const Common::Point &point, uint32 frameCount, CursorType type, float framerate, byte a7, LoadFrom loadFrom) {
	if (!_objects.has(objectId))
		error("[Application::objectSetActiveCursor] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setActiveCursor(point, frameCount, type, framerate, a7, loadFrom, _configuration.artBAG ? kArchiveArt : kArchiveFile);
}

void Application::objectSetPassiveCursor(ObjectId objectId, const Common::Point &point, uint32 frameCount, CursorType type, float framerate, byte a7, LoadFrom loadFrom) {
	if (!_objects.has(objectId))
		error("[Application::objectSetPassiveCursor] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setPassiveCursor(point, frameCount, type, framerate, a7, loadFrom, _configuration.artBAG ? kArchiveArt : kArchiveFile);
}

void Application::objectSetActiveDrawCursor(ObjectId objectId, const Common::Point &point, uint32 frameCount, CursorType type, float framerate, byte a7, LoadFrom loadFrom) {
	if (!_objects.has(objectId))
		error("[Application::objectSetActiveDrawCursor] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setActiveDrawCursor(point, frameCount, type, framerate, a7, loadFrom, _configuration.artBAG ? kArchiveArt : kArchiveFile);
}

void Application::objectSetPassiveDrawCursor(ObjectId objectId, const Common::Point &point, uint32 frameCount, CursorType type, float framerate, byte a7, LoadFrom loadFrom) {
	if (!_objects.has(objectId))
		error("[Application::objectSetPassiveDrawCursor] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setPassiveDrawCursor(point, frameCount, type, framerate, a7, loadFrom, _configuration.artBAG ? kArchiveArt : kArchiveFile);
}

void Application::objectAddPresentation(ObjectId objectId) {
	if (!_objects.has(objectId))
		error("[Application::objectAddPresentation] Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->addPresentation();
}

void Application::objectPresentationAddTextToPuzzle(ObjectId objectId, uint32 presentationIndex, PuzzleId puzzleId, Common::String text, const Common::Point &point, FontId fontId, const Color &foreground, const Color &background) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationAddTextToPuzzle] Object Id doesn't exist (%d)", objectId.id());

	if (!_puzzles.has(puzzleId))
		error("[Application::objectPresentationAddTextToPuzzle] Puzzle Id doesn't exist (%d)", puzzleId.id());

	_objects.get(objectId)->addTextToPuzzle(presentationIndex, _puzzles.get(puzzleId), text, point, fontId, foreground, background);
}

void Application::objectPresentationSetTextToPuzzle(ObjectId objectId, uint32 presentationIndex, uint32 textIndex, Common::String text) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationSetTextToPuzzle] Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setTextToPuzzle(presentationIndex, textIndex, text);
}

void Application::objectPresentationSetTextCoordinatesToPuzzle(ObjectId objectId, uint32 presentationIndex, uint32 textIndex, const Common::Point &point) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationSetTextCoordinatesToPuzzle] Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setTextCoordinatesToPuzzle(presentationIndex, textIndex, point);
}

uint32 Application::objectPresentationGetTextWidth(ObjectId objectId, uint32 presentationIndex, uint32 textIndex) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationGetTextWidth] Id doesn't exist (%d)", objectId.id());

	return _objects.get(objectId)->getTextWidth(presentationIndex, textIndex);
}

void Application::objectPresentationAddImageToPuzzle(ObjectId objectId, uint32 presentationIndex, PuzzleId puzzleId, Common::String filename, const Common::Point &point, bool isActive, DrawType drawType, uint32 priority) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationAddImageToPuzzle] Object Id doesn't exist (%d)", objectId.id());

	if (!_puzzles.has(puzzleId))
		error("[Application::objectPresentationAddImageToPuzzle] Puzzle Id doesn't exist (%d)", puzzleId.id());

	_objects.get(objectId)->addImageToPuzzle(presentationIndex, _puzzles.get(puzzleId), filename, point, isActive, drawType, priority, 0, _loadFrom);
}

void Application::objectPresentationAddImageToRotation(ObjectId objectId, uint32 presentationIndex, Id rotationId, uint32 layer) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationAddImageToRotation] Object Id doesn't exist (%d)", objectId.id());

	if (!_rotations.has(rotationId))
		error("[Application::objectPresentationAddImageToRotation] Rotation Id doesn't exist (%d)", rotationId);

	Rotation *rotation = _rotations.get(rotationId);
	if (layer >= rotation->getLayerCount())
		error("[Application::objectPresentationAddImageToRotation] Rotation layer to big (was:%d, max: %d)", layer, rotation->getLayerCount() - 1);

	_objects.get(objectId)->addImageToRotation(presentationIndex, rotation, layer);
}

void Application::objectPresentationSetImageCoordinatesOnPuzzle(ObjectId objectId, uint32 presentationIndex, const Common::Point &point) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationSetImageCoordinatesOnPuzzle] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setImageCoordinatesOnPuzzle(presentationIndex, point);
}

void Application::objectPresentationSetImageCoordinatesOnPuzzle(ObjectId objectId, uint32 presentationIndex, uint32 imageIndex, const Common::Point &point) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationSetImageCoordinatesOnPuzzle] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setImageCoordinatesOnPuzzle(presentationIndex, imageIndex, point);
}

void Application::objectPresentationSetImageOriginalCoordinatesOnPuzzle(ObjectId objectId, uint32 presentationIndex) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationSetImageOriginalCoordinatesOnPuzzle] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setImageOriginalCoordinatesOnPuzzle(presentationIndex);
}

Common::Point Application::objectPresentationGetImageCoordinatesOnPuzzle(ObjectId objectId, uint32 presentationIndex, uint32 imageIndex) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationGetImageCoordinatesOnPuzzle] Object Id doesn't exist (%d)", objectId.id());

	return _objects.get(objectId)->getImageCoordinatesOnPuzzle(presentationIndex, imageIndex);
}

void Application::objectPresentationAddAnimationToPuzzle(ObjectId objectId, uint32 presentationIndex, PuzzleId puzzleId, Common::String filename, ImageType imageType, const Common::Point &point, DrawType drawType, uint32 priority, uint32 a10, float a11, byte a12) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationAddAnimationToPuzzle] Object Id doesn't exist (%d)", objectId.id());

	if (!_puzzles.has(puzzleId))
		error("[Application::objectPresentationAddAnimationToPuzzle] Puzzle Id doesn't exist (%d)", puzzleId.id());

	_objects.get(objectId)->addAnimationToPuzzle(presentationIndex, _puzzles.get(puzzleId), filename, imageType, point, 1, drawType, priority, 0, a10, a11, a12, _loadFrom);
}

void Application::objectPresentationAddAnimationToRotation(ObjectId objectId, uint32 presentationIndex, Id rotationId, uint32 layer, uint32 a5, float a6, byte a7) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationAddImageToRotation] Object Id doesn't exist (%d)", objectId.id());

	if (!_rotations.has(rotationId))
		error("[Application::objectPresentationAddImageToRotation] Rotation Id doesn't exist (%d)", rotationId);

	Rotation *rotation = _rotations.get(rotationId);
	if (layer >= rotation->getLayerCount())
		error("[Application::objectPresentationAddImageToRotation] Rotation layer to big (was:%d, max: %d)", layer, rotation->getLayerCount() - 1);

	_objects.get(objectId)->addAnimationToRotation(presentationIndex, rotation, layer, a5, a6, a7);
}

void Application::objectPresentationSetAnimationOnPuzzle(ObjectId objectId, uint32 presentationIndex, uint32 animationIndex, Id targetId) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationSetAnimationOnPuzzle] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setAnimationOnPuzzle(presentationIndex, animationIndex, targetId);
}

void Application::objectPresentationSetAnimationOnRotation(ObjectId objectId, uint32 presentationIndex, uint32 animationIndex, Id targetId) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationSetAnimationOnRotation] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setAnimationOnRotation(presentationIndex, animationIndex, targetId);
}

void Application::objectPresentationSetAnimationStartFrame(ObjectId objectId, uint32 presentationIndex, uint32 startFrame) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationAnimationSetStartFrame] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setAnimationStartFrame(presentationIndex, startFrame);
}

void Application::objectPresentationSetAnimationActiveFrame(ObjectId objectId, uint32 presentationIndex, uint32 activeFrame) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationAnimationSetActiveFrame] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setAnimationActiveFrame(presentationIndex, activeFrame);
}

void Application::objectPresentationSetAnimationCoordinatesOnPuzzle(ObjectId objectId, uint32 presentationIndex, const Common::Point &point) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationSetAnimationCoordinatesOnPuzzle] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setAnimationCoordinatesOnPuzzle(presentationIndex, point);
}

void Application::objectPresentationPauseAnimation(ObjectId objectId, uint32 presentationIndex) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationPauseAnimation] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->pauseAnimation(presentationIndex);
}

void Application::objectPresentationUnpauseAnimation(ObjectId objectId, uint32 presentationIndex) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationPauseAnimation] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->unpauseAnimation(presentationIndex);
}

void Application::objectPresentationPauseAnimationFrame(ObjectId objectId, uint32 presentationIndex, uint32 frame, uint32 a4, uint32 a5) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationPauseAnimation] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->pauseFrameAnimation(presentationIndex, frame, a4, a5);
}

void Application::objectPresentationShow(ObjectId objectId, uint32 presentationIndex) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationShow] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->show(presentationIndex);
}

void Application::objectPresentationShow(ObjectId objectId) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationShow] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->show();
}

void Application::objectPresentationHide(ObjectId objectId, uint32 presentationIndex) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationHide] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->hide(presentationIndex);
}

void Application::objectPresentationHide(ObjectId objectId) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationHide] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->hide();
}

void Application::objectPresentationHideAndRemove(ObjectId objectId, uint32 presentationIndex) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationHide] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->hideAndRemove(presentationIndex);
}

void Application::objectPresentationHideAndRemove(ObjectId objectId) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationHide] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->hideAndRemove();
}


void Application::initObjectCursors(ObjectId objectId) {
	if (!_objects.has(objectId))
		return;

	Object *object = _objects.get(objectId);

	// Passive cursor
	ObjectCursor *passiveCursor = object->getPassiveCursor();
	switch (passiveCursor->type){
	default:
		break;

	case kCursorTypeImage:
		cursorAdd(kCursorPassive, Common::String::format("%s_p", object->getName().c_str()), kCursorTypeImage, 2, passiveCursor->loadFrom, passiveCursor->archiveType);
		break;

	case kCursorTypeAnimated:
		cursorAdd(kCursorPassive, object->getName(), kCursorTypeAnimated, 2, passiveCursor->frameCount, passiveCursor->frameRate, passiveCursor->field_14, passiveCursor->loadFrom, passiveCursor->archiveType);
		break;
	}

	cursorSetOffset(kCursorPassive, passiveCursor->offset);

	// Active cursor
	ObjectCursor *activeCursor = object->getActiveCursor();
	switch (activeCursor->type){
	default:
		break;

	case kCursorTypeImage:
		cursorAdd(kCursorActive, Common::String::format("%s_a", object->getName().c_str()), kCursorTypeImage, 2, activeCursor->loadFrom, activeCursor->archiveType);
		break;

	case kCursorTypeAnimated:
		cursorAdd(kCursorActive, object->getName(), kCursorTypeAnimated, 2, activeCursor->frameCount, activeCursor->frameRate, activeCursor->field_14, activeCursor->loadFrom, activeCursor->archiveType);
		break;
	}

	cursorSetOffset(kCursorActive, passiveCursor->offset);
}

void Application::initObjectDrawCursors(ObjectId objectId) {
	if (!_objects.has(objectId))
		return;

	Object *object = _objects.get(objectId);

	// Passive draw cursor
	ObjectCursor *passiveCursor = object->getPassiveDrawCursor();
	switch (passiveCursor->type){
	default:
		break;

	case kCursorTypeImage:
		cursorAdd(kCursorPassiveDraw, Common::String::format("%s_dp", object->getName().empty() ? "dummy" : object->getName().c_str()), kCursorTypeImage, 3, passiveCursor->loadFrom, passiveCursor->archiveType);
		break;

	case kCursorTypeAnimated:
		cursorAdd(kCursorPassiveDraw, object->getName(), kCursorTypeAnimated, 3, passiveCursor->frameCount, passiveCursor->frameRate, passiveCursor->field_14, passiveCursor->loadFrom, passiveCursor->archiveType);
		break;
	}

	cursorSetOffset(kCursorPassive, passiveCursor->offset);

	// Active draw cursor
	ObjectCursor *activeCursor = object->getActiveDrawCursor();
	switch (activeCursor->type){
	default:
		break;

	case kCursorTypeImage:
		cursorAdd(kCursorActiveDraw, Common::String::format("%s_da", object->getName().empty() ? "dummy" : object->getName().c_str()), kCursorTypeImage, 2, activeCursor->loadFrom, activeCursor->archiveType);
		break;

	case kCursorTypeAnimated:
		cursorAdd(kCursorActiveDraw, object->getName(), kCursorTypeAnimated, 3, activeCursor->frameCount, activeCursor->frameRate, activeCursor->field_14, activeCursor->loadFrom, activeCursor->archiveType);
		break;
	}

	cursorSetOffset(kCursorActive, passiveCursor->offset);
}

#pragma endregion

#pragma region Rotation

Rotation *Application::getRotation(Id rotationId) {
	if (!_rotations.has(rotationId))
		return NULL;

	return _rotations.get(rotationId);
}

void Application::rotationAdd(Id rotationId, Common::String name, byte a3, uint32 nodeCount) {
	if (_rotations.has(rotationId))
		error("[Application::rotationAdd] Rotation Id already exists (%d)", rotationId);

	// Compute path
	//  Note: .aqi files are uncompressed rotation files (not present in the distributed game)
	Common::String path = Common::String::format("DATA/NODE/%s.%s", name.c_str(), _isRotationCompressed ? "aqc" : "aqi");

	// Check for single node file
	if (_field_5E && !Common::File::exists(path))
		error("[Application::rotationAdd] Node file doesn't exist (%s)", path.c_str());

	// Check for node channel files
	if (_field_5E && !_isRotationCompressed && nodeCount > 0) {
		for (uint i = 0; i < nodeCount; i++) {
			Common::String nodePath = Common::String::format("DATA/NODE/%s_%03d.aqc", name.c_str(), i);

			if (!Common::File::exists(nodePath))
				error("[Application::rotationAdd] Channel node file doesn't exist (%s)", nodePath.c_str());
		}
	}

	// Create rotation
	Rotation *rotation = new Rotation(rotationId, name, a3, _loadFrom, nodeCount, _isRotationCompressed);

	_rotations.push_back(rotation);
}

void Application::rotationSetCompressionBufferLength(Id rotationId, uint32 length) {
	if (!_rotations.has(rotationId))
		error("[Application::rotationAdd] Rotation Id doesn't exist (%d)", rotationId);

	_rotations.get(rotationId)->setCompressionBufferLength(length);
}

void Application::rotationAddMovabilityToPuzzle(Id fromRotationId, PuzzleId toPuzzleId, Common::String name, const Common::Rect &rect, bool enabled, CursorId cursorId, uint32 a10) {
	// Check ids
	if (!_rotations.has(fromRotationId))
		error("[Application::rotationAddMovabilityToRotation] Wrong From rotation Id (%d)", fromRotationId);

	if (!_puzzles.has(toPuzzleId))
		error("[Application::rotationAddMovabilityToRotation] Wrong To puzzle Id (%d)", toPuzzleId.id());

	// Create movability
	Movability *movability = new Movability(fromRotationId, toPuzzleId, name, kMovabilityRotationToPuzzle);
	movability->setHotspot(rect, enabled, cursorId, a10);

	_rotations.get(fromRotationId)->addMovability(movability);
}

void Application::rotationSetMovabilityToPuzzle(Id rotationId, uint32 movabilityIndex, float a3, float a4, float a5, float a6, byte a7) {
	if (!_rotations.has(rotationId))
		error("[Application::rotationSetMovabilityToPuzzle] Wrong rotation Id (%d)", rotationId);

	Movability *movability = _rotations.get(rotationId)->getMovability(movabilityIndex);
	if (movability->getType() != kMovabilityRotationToPuzzle)
		error("[Application::rotationSetMovabilityToPuzzle] Invalid type of movability (%d)", movability->getType());

	movability->update(a3, a4, a5, a6, a7, 0.0, 0.0, 85.0);
}

void Application::rotationAddMovabilityToRotation(Id fromRotationId, Id toRotationId, Common::String name, const Common::Rect &rect, bool enabled, CursorId cursorId, uint32 a10) {
	// Check ids
	if (!_rotations.has(fromRotationId))
		error("[Application::rotationAddMovabilityToRotation] Wrong From rotation Id (%d)", fromRotationId);

	if (!_rotations.has(toRotationId))
		error("[Application::rotationAddMovabilityToRotation] Wrong To rotation Id (%d)", toRotationId);

	// Create movability
	Movability *movability = new Movability(fromRotationId, toRotationId, name, kMovabilityRotationToRotation);
	movability->setHotspot(rect, enabled, cursorId, a10);

	_rotations.get(fromRotationId)->addMovability(movability);
}

void Application::rotationSetMovabilityToRotation(Id rotationId, uint32 movabilityIndex, float a3, float a4, float a5, float a6, byte a7, float a8, float a9, float a10) {
	if (!_rotations.has(rotationId))
		error("[Application::rotationSetMovabilityToRotation] Wrong rotation Id (%d)", rotationId);

	Movability *movability = _rotations.get(rotationId)->getMovability(movabilityIndex);
	if (movability->getType() != kMovabilityRotationToRotation)
		error("[Application::rotationSetMovabilityToRotation] Invalid type of movability (%d)", movability->getType());

	movability->update(a3, a4, a5, a6, a7, a8, a9, a10);
}

void Application::rotationSetMovabilityOnOrOff(Id rotationId, bool enableHotspot) {
	if (!_rotations.has(rotationId))
		error("[Application::rotationSetMovabilityOnOrOff] Rotation Id doesn't exist (%d)", rotationId);

	_rotations.get(rotationId)->setMovabilityOnOrOff(enableHotspot);
}

void Application::rotationSetMovabilityOnOrOff(Id rotationId, bool enableHotspot, uint32 fromMovability, uint32 toMovability) {
	if (!_rotations.has(rotationId))
		error("[Application::rotationSetMovabilityOnOrOff] Rotation Id doesn't exist (%d)", rotationId);

	_rotations.get(rotationId)->setMovabilityOnOrOff(enableHotspot, fromMovability, toMovability);
}

void Application::rotationSetMovabilityOn(Id rotationId) {
	rotationSetMovabilityOnOrOff(rotationId, true);
}

void Application::rotationSetMovabilityOff(Id rotationId) {
	rotationSetMovabilityOnOrOff(rotationId, false);
}

void Application::rotationSetMovabilityOn(Id rotationId, uint32 fromMovability, uint32 toMovability) {
	rotationSetMovabilityOnOrOff(rotationId, true, fromMovability, toMovability);
}

void Application::rotationSetMovabilityOff(Id rotationId, uint32 fromMovability, uint32 toMovability) {
	rotationSetMovabilityOnOrOff(rotationId, false, fromMovability, toMovability);
}

void Application::rotationSetMovabilityRideName(Id rotationId, uint32 movabilityIndex, Common::String name) {
	if (!_rotations.has(rotationId))
		error("[Application::rotationSetAlp] Rotation Id doesn't exist (%d)", rotationId);

	_rotations.get(rotationId)->setMovabilityRideName(movabilityIndex, name);
}

void Application::rotationSetAlp(Id rotationId, float alp) {
	if (!_rotations.has(rotationId))
		error("[Application::rotationSetAlp] Rotation Id doesn't exist (%d)", rotationId);

	alp -= 135.0f;
	if (alp < 0.0f)
		alp += 360.0f;

	_rotations.get(rotationId)->setAlp(alp);
}

void Application::rotationSetBet(Id rotationId, float bet) {
	if (!_rotations.has(rotationId))
		error("[Application::rotationSetBet] Rotation Id doesn't exist (%d)", rotationId);

	_rotations.get(rotationId)->setBet(bet);
}

void Application::rotationSetRan(Id rotationId, float ran) {
	if (!_rotations.has(rotationId))
		error("[Application::rotationSetRan] Rotation Id doesn't exist (%d)", rotationId);

	_rotations.get(rotationId)->setRan(ran);
}

float Application::rotationGetAlp(Id rotationId) {
	if (!_rotations.has(rotationId))
		error("[Application::rotationGetAlp] Rotation Id doesn't exist (%d)", rotationId);

	float alp = _rotations.get(rotationId)->getAlp() + 135.0f;
	if (alp > 360.0f)
		alp -= 360.0f;

	return alp;
}

float Application::rotationGetBet(Id rotationId) {
	if (!_rotations.has(rotationId))
		error("[Application::rotationGetBet] Rotation Id doesn't exist (%d)", rotationId);

	return _rotations.get(rotationId)->getBet();
}

float Application::rotationGetRan(Id rotationId) {
	if (!_rotations.has(rotationId))
		error("[Application::rotationGetRan] Rotation Id doesn't exist (%d)", rotationId);

	return _rotations.get(rotationId)->getRan();
}

void Application::rotationSetRolTo(Id rotationId, float a2, float a3, float a4) {
	if (!_rotations.has(rotationId))
		error("[Application::rotationSetRolTo] Rotation Id doesn't exist (%d)", rotationId);

	_rotations.get(rotationId)->setRolTo(a2, a3, a4);
}

void Application::rotationSetJugOn(Id rotationId, float amplitude, float speed) {
	if (amplitude < 10.0f || amplitude > 50.0f)
		error("[Application::rotationSetJugOn] Wrong amplitude for rotation (was: %f, valid: [10.0, 50.0])", amplitude);

	if (speed < 0.1f || speed > 10.0f)
		error("[Application::rotationSetJugOn] Wrong speed for rotation (was: %f, valid: [10.0, 50.0])", speed);

	if (!_rotations.has(rotationId))
		error("[Application::rotationSetJugOn] Wrong rotation Id (%d)", rotationId);

	_rotations.get(rotationId)->setAmplitudeAndSpeed(amplitude, speed);
}

void Application::rotationAddAmbientSound(Id rotationId, Id soundId, uint32 volume, int32 pan, uint32 fadeFrames, uint32 a6, uint32 a7) {
	if (!_rotations.has(rotationId))
		error("[Application::rotationAddAmbientSound] Wrong rotation Id (%d)", rotationId);

	SoundEntry *entry = _soundManager->getEntry(soundId);
	if (!entry)
		error("[Application::rotationAddAmbientSound] Wrong sound Id (%d)", soundId);

	if (entry->getType() != kSoundTypeAmbientMusic)
		error("[Application::rotationAddAmbientSound] Wrong sound type, only kSoundTypeAmbientMusic(2) is allowed (%d)", entry->getType());

	_rotations.get(rotationId)->addAmbientSound(entry, volume, pan, true, fadeFrames, a6, a7);
}

void Application::rotationSetAmbientSoundOn(Id rotationId, Id soundId) {
	if (!_rotations.has(rotationId))
		error("[Application::rotationSetAmbientSoundOn] Wrong rotation Id (%d)", rotationId);

	_rotations.get(rotationId)->setAmbientSoundOn(soundId);
}

void Application::rotationSetAmbientSoundOff(Id rotationId, Id soundId) {
	if (!_rotations.has(rotationId))
		error("[Application::rotationSetAmbientSoundOff] Wrong rotation Id (%d)", rotationId);

	_rotations.get(rotationId)->setAmbientSoundOff(soundId);
}

void Application::rotationAdd3DSound(Id rotationId, uint32 soundId, uint32 a3, uint32 a4, uint32 fadeFrames, uint32 volume, float a7, uint32 a8) {
	if (!_rotations.has(rotationId))
		error("[Application::rotationAdd3DSound] Wrong rotation Id (%d)", rotationId);

	SoundEntry *entry = _soundManager->getEntry(soundId);
	if (!entry)
		error("[Application::rotationAdd3DSound] Wrong sound Id (%d)", soundId);

	if (entry->getType() != kSoundTypeAmbientEffect)
		error("[Application::rotationAdd3DSound] Wrong sound type, only kSoundTypeAmbientEffect(3) is allowed (%d)", entry->getType());

	_rotations.get(rotationId)->add3DSound(entry, volume, true, a3, a4, fadeFrames, a7, a8);
}

void Application::rotationSet3DSoundOn(Id rotationId, Id soundId) {
	rotationSetAmbientSoundOn(rotationId, soundId);
}

void Application::rotationSet3DSoundOff(Id rotationId, Id soundId) {
	rotationSetAmbientSoundOff(rotationId, soundId);
}

void Application::rotationSetFreOn(Id rotationId) {
	if (!_rotations.has(rotationId))
		error("[Application::rotationSetFreOn] Rotation Id doesn't exist (%d)", rotationId);

	_rotations.get(rotationId)->setFreOnOff(true);
}

void Application::rotationSetFreOff(Id rotationId) {
	if (!_rotations.has(rotationId))
		error("[Application::rotationSetFreOff] Rotation Id doesn't exist (%d)", rotationId);

	_rotations.get(rotationId)->setFreOnOff(false);
}

void Application::rotationSetActive(Id id, bool updateSoundItems, bool a3) {
	// Setup puzzle
	if (!_rotations.has(id))
		return;

	reset();
	_rotation = _rotations.get(id);
	_rotation->alloc();

	g_system->warpMouse(320, 240);

	_state = kStateUpdateRotation;

	if (_soundHandler->getField0()) {
		_soundHandler->turnOnItems4();

		_soundHandler->reset();
		_soundHandler->setSounds1(_rotation->getSoundItems());
		return;
	}

	_soundHandler->setSounds2(_rotation->getSoundItems());

	if (_soundHandler->processSounds()) {

		_soundHandler->turnOffItems1();

		if (_soundHandler->updateItems(2)) {

			_soundHandler->updateItems2(3);
			_soundHandler->updateItems3(3);
			_soundHandler->turnOnItems4();

			_soundHandler->reset();
			_soundHandler->setSounds1(_rotation->getSoundItems());
			return;
		}
	}

	_soundHandler->turnOffSounds1(a3);

	_rotation->updateSoundItems(updateSoundItems);

	_soundHandler->reset();
	_soundHandler->setSounds1(_rotation->getSoundItems());
}

#pragma endregion

#pragma region Sound

void Application::soundAdd(Id soundId, SoundType type, Common::String filename, LoadFrom loadFrom) {
	soundAdd(soundId, type, filename, loadFrom, 2, 3);
}

void Application::soundAdd(Id soundId, SoundType type, Common::String filename, LoadFrom loadFrom, uint32 a4, uint32 soundChunk) {
	// Check filename
	if (filename.size() <= 4)
		error("[Application::soundAdd] Wrong filename (%s)", filename.c_str());

	// Check format
	SoundFormat format = SoundEntry::getFormat(filename);
	if (format == kSoundFormatInvalid)
		error("[Application::soundAdd] Wrong file format (%s)", filename.c_str());

	// Compute path
	Common::String path;
	if (type == kSoundTypeDialog)
		path = Common::String::format("DATA/%s/SOUND/%s/%s", getCurrentZoneFolder().c_str(), getLanguageFolder().c_str(), filename.c_str());
	else
		path = Common::String::format("DATA/%s/SOUND/%s", getCurrentZoneFolder().c_str(), filename.c_str());

	if (!Common::File::exists(path))
		error("[Application::soundAdd] File doesn't exist (%s)", path.c_str());

	_soundManager->addEntry(soundId, type, filename, loadFrom, format, a4 != 1, soundChunk);
}

void Application::soundPlay(Id soundId, SoundLoopType loop) {
	_soundManager->play(soundId, loop != kSoundOnce);
}

void Application::soundStop(Id soundId, uint32 a2) {
	_soundManager->stop(soundId, a2);
}

void Application::soundSetVolume(Id soundId, uint32 volume) {
	_soundManager->setVolume(soundId, volume);
}

void Application::soundStopType(SoundType soundType, uint32 a2) {
	_soundManager->stopType(soundType, a2);
}

void Application::soundSetMultiplier(SoundType soundType, int32 multiplier) {
	_soundManager->setMultiplier(soundType, multiplier);
}

void Application::soundSetMultiplierIfNotType(SoundType soundType, int32 multiplier) {
	_soundManager->setMultiplierIfNotType(soundType, multiplier);
}

void Application::soundStopAll(uint32 a1) {
	_soundManager->stopAll(a1);
}

void Application::soundSetPan(Id soundId, int32 pan) {
	_soundManager->setPan(soundId, pan);
}

bool Application::soundIsPlaying(Id soundId) {
	return _soundManager->isPlaying(soundId);
}

bool Application::soundIsPlayingType(SoundType soundType) {
	return _soundManager->isPlayingType(soundType);
}

#pragma endregion

#pragma region Timer

void Application::timerStart(TimerId id, uint32 elapseTime) {
	_timerHandler->start(id, elapseTime);
}

void Application::timerStop(TimerId id) {
	_timerHandler->stop(id);
}

void Application::timerStopAll() {
	_timerHandler->stopAll();
}

bool Application::hasTimer(TimerId id) {
	return _timerHandler->has(id);
}

#pragma endregion

#pragma region Var

#define IMPLEMENT_VAR_FUNCTIONS(name, type) \
	void Application::varDefine##name(Id id, type val) { \
		if (!_var) \
			error("[Application::varDefine" #name "] Var not initialized properly"); \
		_var->define##name(id, val); \
	} \
	type Application::varGet##name(Id id) { \
		if (!_var) \
			error("[Application::varGet" #name "] Var not initialized properly"); \
		return _var->get##name(id); \
	} \
	void Application::varSet##name(Id id, type val) { \
		if (!_var) \
			error("[Application::varSet" #name "] Var not initialized properly"); \
		_var->set##name(id, val); \
	}

IMPLEMENT_VAR_FUNCTIONS(Byte,   int8)
IMPLEMENT_VAR_FUNCTIONS(Word,   int16)
IMPLEMENT_VAR_FUNCTIONS(Dword,  int32)
IMPLEMENT_VAR_FUNCTIONS(String, Common::String)
IMPLEMENT_VAR_FUNCTIONS(Float,  float)

#undef IMPLEMENT_VAR_FUNCTIONS

void Application::varRemoveAll() {
	if (!_var)
		error("[Application::varRemoveAll] Var not initialized properly");

	_var->removeAll();
}

#pragma endregion

#pragma region Visual

void Application::visualAddShowToPuzzle(Id visualId, PuzzleId puzzleId, uint32 a3, uint32 a4, uint32 left, uint32 top, uint32 offsetY, uint32 height, uint32 progressMultiplier, uint32 progressColor) {
	if (!_puzzles.has(puzzleId))
		error("[Application::visualAddListToPuzzle] Puzzle Id doesn't exist (%d)", puzzleId.id());

	Puzzle *puzzle = _puzzles.get(puzzleId);
	if (puzzle->hasVisual(visualId))
		error("[Application::visualAddListToPuzzle] Visual already exists on puzzle (%d)", visualId);

	Visual *visual = createVisual(visualId, a3, a4, left, top, offsetY, height, progressMultiplier, progressColor);
	puzzle->addVisual(visual);
}

void Application::visualListAddToPuzzle(Id visualId, PuzzleId puzzleId, uint32 a3,
	                                    Common::String imagePath, Common::String iconPath, Common::String filename3, Common::String filename4, Common::String filename5, Common::String filename6, Common::String filename7, Common::String filename8, Common::String filename9, Common::String filename10,
	                                    Common::String filename11, Common::String filename12, Common::String filename13,
							            DrawType drawType, const Common::Point &origin, const Common::Point &backgroundOffset, uint32 a22, uint32 a23, uint32 a24, uint32 a25, uint32 a26,
	                                    uint32 a27, const Common::Point &upOffset, uint32 a30, uint32 a31, uint32 a32, uint32 a33, const Common::Point &downOffset, uint32 a36,
	                                    uint32 a37, uint32 a38, uint32 a39, const Common::Point &imageCoords, ImageType imageType, DrawType imageDrawType, uint32 a44, uint32 a45, uint32 a46,
							            const Color &foreground, const Color &foregroundSelected, const Color &background, FontId fontId,
							            ArchiveType archiveType) {


	if (!_puzzles.has(puzzleId))
		error("[Application::visualAddListToPuzzle] Puzzle Id doesn't exist (%d)", puzzleId.id());

	Puzzle *puzzle = _puzzles.get(puzzleId);
	if (puzzle->hasVisual(visualId))
		error("[Application::visualAddListToPuzzle] Visual already exists on puzzle (%d)", visualId);

	VisualObjectList *list = new VisualObjectList(visualId);
	list->init(a3, imagePath, iconPath, filename3, filename4, filename5, filename6, filename7, filename8, filename9, filename10, filename11, filename12, filename13, drawType, archiveType);
	list->setField8(1);
	list->setVisible(true);
	list->setOrigin(origin);
	list->setBackgroundOffset(backgroundOffset);
	list->sub_46DD30(a22, a23, a24, a25, a26, a27);
	list->setUpOffset(upOffset);
	list->sub_46DDA0(a30, a31, a32, a33);
	list->setDownOffset(downOffset);
	list->sub_46DDD0(a36, a37, a38, a39);
	list->setImageInfo(imageCoords, imageType, imageDrawType);
	list->sub_46DE30(a44, a45);
	list->sub_46E330(a46);
	list->initHotspots();
	list->setTextForegroundColor(foreground, foregroundSelected);
	list->setTextBackgroundColor(background);
	list->setFontId(fontId);

	puzzle->addVisual(list);
}

void Application::visualListAdd(Id visualId, PuzzleId puzzleId, const ObjectId &objectId) {
	if (!_puzzles.has(puzzleId))
		error("[Application::visualListAdd] Puzzle Id doesn't exist (%d)", puzzleId.id());

	Puzzle *puzzle = _puzzles.get(puzzleId);
	if (!puzzle->hasVisual(visualId))
		error("[Application::visualListAdd] Visual (%d) is not on puzzle (%d)", visualId, puzzleId.id());

	((VisualObjectList *)puzzle->getVisual(visualId))->add(objectId);
}

void Application::visualListRemove(Id visualId, PuzzleId puzzleId, const ObjectId &objectId, bool removeObject) {
	if (!_puzzles.has(puzzleId))
		error("[Application::visualListRemove] Puzzle Id doesn't exist (%d)", puzzleId.id());

	Puzzle *puzzle = _puzzles.get(puzzleId);
	if (!puzzle->hasVisual(visualId))
		error("[Application::visualListRemove] Visual (%d) is not on puzzle (%d)", visualId, puzzleId.id());

	((VisualObjectList *)puzzle->getVisual(visualId))->remove(objectId, removeObject);
}

void Application::visualListRemove(Id visualId, PuzzleId puzzleId, bool removeObject) {
	if (!_puzzles.has(puzzleId))
		error("[Application::visualListRemove] Puzzle Id doesn't exist (%d)", puzzleId.id());

	Puzzle *puzzle = _puzzles.get(puzzleId);
	if (!puzzle->hasVisual(visualId))
		error("[Application::visualListRemove] Visual (%d) is not on puzzle (%d)", visualId, puzzleId.id());

	((VisualObjectList *)puzzle->getVisual(visualId))->removeAll(removeObject);
}

bool Application::visualListIsIn(Id visualId, PuzzleId puzzleId, const Common::String &description) {
	if (!_puzzles.has(puzzleId))
		error("[Application::visualListResetObjectClicked] Puzzle Id doesn't exist (%d)", puzzleId.id());

	Puzzle *puzzle = _puzzles.get(puzzleId);
	if (!puzzle->hasVisual(visualId))
		error("[Application::visualListResetObjectClicked] Visual (%d) is not on puzzle (%d)", visualId, puzzleId.id());

	return ((VisualObjectList *)puzzle->getVisual(visualId))->isIn(description);
}

void Application::visualListSetOn(Id visualId, PuzzleId puzzleId) {
	if (!_puzzles.has(puzzleId))
		error("[Application::visualListResetObjectClicked] Puzzle Id doesn't exist (%d)", puzzleId.id());

	Puzzle *puzzle = _puzzles.get(puzzleId);
	if (!puzzle->hasVisual(visualId))
		error("[Application::visualListResetObjectClicked] Visual (%d) is not on puzzle (%d)", visualId, puzzleId.id());

	((VisualObjectList *)puzzle->getVisual(visualId))->setVisible(true);
}

void Application::visualListSetOff(Id visualId, PuzzleId puzzleId) {
	if (!_puzzles.has(puzzleId))
		error("[Application::visualListResetObjectClicked] Puzzle Id doesn't exist (%d)", puzzleId.id());

	Puzzle *puzzle = _puzzles.get(puzzleId);
	if (!puzzle->hasVisual(visualId))
		error("[Application::visualListResetObjectClicked] Visual (%d) is not on puzzle (%d)", visualId, puzzleId.id());

	((VisualObjectList *)puzzle->getVisual(visualId))->setVisible(false);
}

void Application::visualListSetIconDirectory(Id visualId, PuzzleId puzzleId, const Common::String &directory) {
	if (!_puzzles.has(puzzleId))
		error("[Application::visualListResetObjectClicked] Puzzle Id doesn't exist (%d)", puzzleId.id());

	Puzzle *puzzle = _puzzles.get(puzzleId);
	if (!puzzle->hasVisual(visualId))
		error("[Application::visualListResetObjectClicked] Visual (%d) is not on puzzle (%d)", visualId, puzzleId.id());

	((VisualObjectList *)puzzle->getVisual(visualId))->setIconDirectory(directory);
}

uint32 Application::visualListGetItemCount(Id visualId, PuzzleId puzzleId) {
	if (!_puzzles.has(puzzleId))
		error("[Application::visualListGetItemCount] Puzzle Id doesn't exist (%d)", puzzleId.id());

	Puzzle *puzzle = _puzzles.get(puzzleId);
	if (!puzzle->hasVisual(visualId))
		error("[Application::visualListGetItemCount] Visual (%d) is not on puzzle (%d)", visualId, puzzleId.id());

	return ((VisualObjectList *)puzzle->getVisual(visualId))->getItemCount();
}

int32 Application::visualListGetImageIndexClicked(Id visualId, PuzzleId puzzleId) {
	if (!_puzzles.has(puzzleId))
		error("[Application::visualListGetImageIndexClicked] Puzzle Id doesn't exist (%d)", puzzleId.id());

	Puzzle *puzzle = _puzzles.get(puzzleId);
	if (!puzzle->hasVisual(visualId))
		error("[Application::visualListGetImageIndexClicked] Visual (%d) is not on puzzle (%d)", visualId, puzzleId.id());

	return ((VisualObjectList *)puzzle->getVisual(visualId))->getImageIndexClicked();
}

ObjectId Application::visualListGetObjectIdClicked(Id visualId, PuzzleId puzzleId) {
	if (!_puzzles.has(puzzleId))
		error("[Application::visualListGetObjectIdClicked] Puzzle Id doesn't exist (%d)", puzzleId.id());

	Puzzle *puzzle = _puzzles.get(puzzleId);
	if (!puzzle->hasVisual(visualId))
		error("[Application::visualListGetObjectIdClicked] Visual (%d) is not on puzzle (%d)", visualId, puzzleId.id());

	return ((VisualObjectList *)puzzle->getVisual(visualId))->getObjectIdClicked();
}

int32 Application::visualListGetObjectIndexClicked(Id visualId, PuzzleId puzzleId) {
	if (!_puzzles.has(puzzleId))
		error("[Application::visualListGetObjectIndexClicked] Puzzle Id doesn't exist (%d)", puzzleId.id());

	Puzzle *puzzle = _puzzles.get(puzzleId);
	if (!puzzle->hasVisual(visualId))
		error("[Application::visualListGetObjectIndexClicked] Visual (%d) is not on puzzle (%d)", visualId, puzzleId.id());

	return ((VisualObjectList *)puzzle->getVisual(visualId))->getObjectIndexClicked();
}

void Application::visualListResetObjectClicked(Id visualId, PuzzleId puzzleId) {
	if (!_puzzles.has(puzzleId))
		error("[Application::visualListResetObjectClicked] Puzzle Id doesn't exist (%d)", puzzleId.id());

	Puzzle *puzzle = _puzzles.get(puzzleId);
	if (!puzzle->hasVisual(visualId))
		error("[Application::visualListResetObjectClicked] Visual (%d) is not on puzzle (%d)", visualId, puzzleId.id());

	((VisualObjectList *)puzzle->getVisual(visualId))->resetObjectClicked();
}

#pragma endregion

#pragma region DragControl

void Application::dragControlSetField45(uint32 a1) {
	if (!_dragControl)
		error("[Application::dragControlSetField45] DragControl not initialized properly!");

	_dragControl->setField45(a1);
}

void Application::dragControlSetHotspot(const Common::Rect &rect) {
	if (!_dragControl)
		error("[Application::dragControlSetHotspot] DragControl not initialized properly!");

	_dragControl->setHotspot(rect);
}

void Application::dragControlSetCoords1(const Common::Point &point) {
	if (!_dragControl)
		error("[Application::dragControlSetCoords1] DragControl not initialized properly!");

	_dragControl->SetCoords1(point);
}

bool Application::dragControlYLower() {
	if (!_dragControl)
		error("[Application::dragControlYLower] DragControl not initialized properly!");

	return _dragControl->getCurrentCoords().y < _dragControl->getOldCoords().y;
}

bool Application::dragControlYEqual() {
	if (!_dragControl)
		error("[Application::dragControlYEqual] DragControl not initialized properly!");

	return _dragControl->getCurrentCoords().y == _dragControl->getOldCoords().y;
}

bool Application::dragControlYHigher() {
	if (!_dragControl)
		error("[Application::dragControlYHigher] DragControl not initialized properly!");

	return _dragControl->getCurrentCoords().y > _dragControl->getOldCoords().y;
}

bool Application::dragControlXLower() {
	if (!_dragControl)
		error("[Application::dragControlXLower] DragControl not initialized properly!");

	return _dragControl->getCurrentCoords().x < _dragControl->getOldCoords().x;
}

bool Application::dragControlXEqual() {
	if (!_dragControl)
		error("[Application::dragControlXEqual] DragControl not initialized properly!");

	return _dragControl->getCurrentCoords().x == _dragControl->getOldCoords().x;
}

bool Application::dragControlXHigher() {
	if (!_dragControl)
		error("[Application::dragControlXHigher] DragControl not initialized properly!");

	return _dragControl->getCurrentCoords().x > _dragControl->getOldCoords().x;
}

bool Application::dragControlYLower0() {
	if (!_dragControl)
		error("[Application::dragControlYLower0] DragControl not initialized properly!");

	return _dragControl->getCurrentCoords().y < _dragControl->getCoords0().y;
}

bool Application::dragControlYHigher0() {
	if (!_dragControl)
		error("[Application::dragControlYHigher0] DragControl not initialized properly!");

	return _dragControl->getCurrentCoords().y > _dragControl->getCoords0().y;
}

bool Application::dragControlXLower0() {
	if (!_dragControl)
		error("[Application::dragControlXLower0] DragControl not initialized properly!");

	return _dragControl->getCurrentCoords().x < _dragControl->getCoords0().x;
}

bool Application::dragControlXHigher0() {
	if (!_dragControl)
		error("[Application::dragControlXHigher0] DragControl not initialized properly!");

	return _dragControl->getCurrentCoords().x > _dragControl->getCoords0().x;
}

bool Application::dragControlYLower1() {
	if (!_dragControl)
		error("[Application::dragControlYLower1] DragControl not initialized properly!");

	return _dragControl->getCurrentCoords().y < _dragControl->getCoords1().y;
}

bool Application::dragControlYHigher1() {
	if (!_dragControl)
		error("[Application::dragControlYHigher1] DragControl not initialized properly!");

	return _dragControl->getCurrentCoords().y > _dragControl->getCoords1().y;
}

bool Application::dragControlXLower1() {
	if (!_dragControl)
		error("[Application::dragControlXLower1] DragControl not initialized properly!");

	return _dragControl->getCurrentCoords().x < _dragControl->getCoords1().x;
}

bool Application::dragControlXHigher1() {
	if (!_dragControl)
		error("[Application::dragControlXHigher1] DragControl not initialized properly!");

	return _dragControl->getCurrentCoords().x > _dragControl->getCoords1().x;
}


uint32 Application::dragControlGetOffsetX() const {
	if (!_dragControl)
		error("[Application::dragControlGetOffsetX] DragControl not initialized properly!");

	return _dragControl->getOffsetX();
}

uint32 Application::dragControlGetOffsetY() const {
	if (!_dragControl)
		error("[Application::dragControlGetOffsetY] DragControl not initialized properly!");

	return _dragControl->getOffsetY();
}

uint32 Application::dragControlGetDistance() const {
	if (!_dragControl)
		error("[Application::dragControlGetDistance] DragControl not initialized properly!");

	return _dragControl->getDistance();
}

uint32 Application::dragControlGetOffsetX0() {
	if (!_dragControl)
		error("[Application::dragControlGetOffsetX0] DragControl not initialized properly!");

	return (uint32)abs(_dragControl->getCurrentCoords().x - _dragControl->getCoords0().x);
}

uint32 Application::dragControlGetOffsetY0() {
	if (!_dragControl)
		error("[Application::dragControlGetOffsetY0] DragControl not initialized properly!");

	return (uint32)abs(_dragControl->getCurrentCoords().y - _dragControl->getCoords0().y);
}

uint32 Application::dragControlGetOffsetX1() {
	if (!_dragControl)
		error("[Application::dragControlGetOffsetX1] DragControl not initialized properly!");

	return (uint32)abs(_dragControl->getCurrentCoords().x - _dragControl->getCoords1().x);
}

uint32 Application::dragControlGetOffsetY1() {
	if (!_dragControl)
		error("[Application::dragControlGetOffsetY1] DragControl not initialized properly!");

	return (uint32)abs(_dragControl->getCurrentCoords().y - _dragControl->getCoords1().y);
}

#pragma endregion

#pragma region Zone & Episode

void Application::addEpisode(ZoneId id, Common::String name, uint32 cd) {
	if (!_zoneHandler)
		error("[Application::addEpisode] Zone handler not initialized properly");

	_zoneHandler->addEpisode(id, name, cd);
}

void Application::setCurrentEpisode(ZoneId id) {
	if (!_zoneHandler)
		error("[Application::setCurrentEpisode] Zone handler not initialized properly");

	_zoneHandler->setCurrentEpisode(id);
}

ZoneId Application::getCurrentEpisode() {
	if (!_zoneHandler)
		error("[Application::getCurrentEpisode] Zone handler not initialized properly");

	return _zoneHandler->getCurrentEpisode();
}

uint32 Application::getEpisodeCd(ZoneId id) {
	if (!_zoneHandler)
		error("[Application::getCurrentEpisode] Zone handler not initialized properly");

	if (!_configuration.checkCD)
		return 1;

	return _zoneHandler->getEpisode(id)->getCd();
}

void Application::addZone(ZoneId id, Common::String name, Common::String folder, ArchiveType archiveType, LoadFrom loadFrom) {
	if (!_zoneHandler)
		error("[Application::addZone] Zone handler not initialized properly");

	_zoneHandler->addZone(id, name, folder, archiveType, loadFrom);
}

void Application::setSpace(ZoneId id) {
	if (id != _systemZone) {
		_artHandler->reset();

		if (getArchiveType(id) == kArchiveArt)
			_artHandler->open(id, getZoneLoadFrom(id));
	}

	setCurrentZone(id);
}

void Application::setCurrentZone(ZoneId id) {
	if (!_zoneHandler)
		error("[Application::setCurrentZone] Zone handler not initialized properly");

	_zoneHandler->setCurrentZone(id);
}

ZoneId Application::getCurrentZone() {
	if (!_zoneHandler)
		error("[Application::getCurrentZone] Zone handler not initialized properly");

	return _zoneHandler->getCurrentZone();
}

Common::String Application::getCurrentZoneFolder() {
	if (!_zoneHandler)
		error("[Application::getCurrentZoneFolder] Zone handler not initialized properly");

	return _zoneHandler->getZone(_zoneHandler->getCurrentZone())->getFolder();
}

Common::String Application::getZoneName(ZoneId zone) const {
	if (!_zoneHandler)
		error("[Application::getZoneName] Zone handler not initialized properly");

	if (!_zoneHandler->has(zone))
		return "";

	return _zoneHandler->getZone(zone)->getName();
}

Common::String Application::getZoneFolder(ZoneId zone) const {
	if (!_zoneHandler)
		error("[Application::getZoneFolder] Zone handler not initialized properly");

	return _zoneHandler->getZone(zone)->getFolder();
}

ArchiveType Application::getZoneArchiveType(ZoneId zone) const {
	if (!_zoneHandler)
		error("[Application::getZoneArchiveType] Zone handler not initialized properly");

	return _zoneHandler->getZone(zone)->getArchiveType();
}

LoadFrom Application::getZoneLoadFrom(ZoneId zone) const {
	if (!_zoneHandler)
		error("[Application::getZoneLoadFrom] Zone handler not initialized properly");

	return _zoneHandler->getZone(zone)->getLoadFrom();
}

ArchiveType Application::getArchiveType(ZoneId zone) const {
	if (!_zoneHandler)
		error("[Application::getZoneArchiveType] Zone handler not initialized properly");

	if (_archiveType != kArchiveFile)
		return _zoneHandler->getZone(zone)->getArchiveType();

	return _archiveType;
}

#pragma endregion

#pragma region Login

void Application::startLogin() {
	// Load the list of users
	Common::Array<SaveManager::SaveEntry *> *users = getSaveManager()->loadUsers();

	// Add list to login screen
	for (uint32 i = 0; i < users->size(); i++) {
		ObjectId objectId = i + 20000000;
		objectAdd(objectId, (*users)[i]->description, (*users)[i]->filename, 1);
		visualListAdd(3, kPuzzlePreferences, objectId);
	}

	// Setup game
	timerStopAll();
	setupZone(_systemZone, kSetupTypeNone);
	setSpace(_systemZone);
	puzzleSetActive(kPuzzlePreferences);
	_currentGameZone = _systemZone;
}

#pragma endregion

} // End of namespace Ring
