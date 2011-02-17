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

#include "ring/base/application.h"

#include "ring/base/art.h"
#include "ring/base/bag.h"
#include "ring/base/cursor.h"
#include "ring/base/dialog.h"
#include "ring/base/event.h"
#include "ring/base/font.h"
#include "ring/base/language.h"
#include "ring/base/movability.h"
#include "ring/base/object.h"
#include "ring/base/preferences.h"
#include "ring/base/puzzle.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"
#include "ring/base/sound.h"
#include "ring/base/timer.h"
#include "ring/base/var.h"

#include "ring/graphics/animation.h"
#include "ring/graphics/dragControl.h"
#include "ring/graphics/screen.h"
#include "ring/graphics/visual.h"

#include "ring/helpers.h"
#include "ring/ring.h"

#include "common/file.h"
#include "common/tokenizer.h"

namespace Ring {

Application::Application(RingEngine *engine) : _vm(engine),
	_screenManager(NULL),  _artHandler(NULL),          _fontHandler(NULL),   _dialogHandler(NULL), _languageHandler(NULL),
	_field_54(1),          _archiveType(kArchiveFile), _cursorHandler(NULL), _loadFrom(kLoadFromInvalid), _field_5E(0),
	_soundHandler(NULL),   _state(kStateNone),         _field_6A(0),         _zoneString("A0"),      _zone(kZoneInvalid),
	_field_6F(0),          _field_70(0),               _field_74(0),         _field_75(0),         _field_76(0),
	_field_77(0),          _field_78(0),               _puzzle(NULL),        _rotation(NULL),      _bag(NULL),
	_timerHandler(NULL),   _var(NULL),                 _dragControl(NULL),   _objectHandler(NULL), _preferenceHandler(NULL),
	_eventHandler(NULL) {

	// Start managers
	_saveManager = new SaveManager(this);
	_soundManager = new SoundManager(this, _vm->_mixer);

	_currentRotation = NULL;
}

Application::~Application() {
	// TODO delete global image buffer

	SAFE_DELETE(_screenManager);
	SAFE_DELETE(_artHandler);
	SAFE_DELETE(_fontHandler);
	SAFE_DELETE(_dialogHandler);
	SAFE_DELETE(_languageHandler);
	SAFE_DELETE(_cursorHandler);
	SAFE_DELETE(_soundHandler);
	CLEAR_ARRAY(Object, _objects);
	CLEAR_ARRAY(Puzzle, _puzzles);
	SAFE_DELETE(_puzzle);
	CLEAR_ARRAY(Rotation, _rotations);
	SAFE_DELETE(_rotation);
	SAFE_DELETE(_bag);
	SAFE_DELETE(_timerHandler);
	SAFE_DELETE(_var);
	SAFE_DELETE(_dragControl);
	SAFE_DELETE(_objectHandler);
	SAFE_DELETE(_preferenceHandler);

	SAFE_DELETE(_saveManager);
	SAFE_DELETE(_soundManager);
	SAFE_DELETE(_eventHandler);

	_currentRotation = NULL;

	// Zero-out passed pointers
	_vm = NULL;
}

#pragma region Initialization

void Application::init() {
	// Setup available languages
	_languageHandler = new LanguageHandler();
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

	_field_5E = 0;

	// Load configuration
	loadConfiguration();

	// Setup video
	_screenManager = new ScreenManager();
	_screenManager->init();
	_screenManager->sub_4028D0(0, 0);

	// Setup objects
	_objectHandler = new ObjectHandler();
	_objectHandler->addFromFile("aObj.ini", languageGetFolder());

	// Setup art
	if (_configuration.artBAG || _configuration.artCURSOR || _configuration.artSY)
		_archiveType = kArchiveArt;

	if (_archiveType == kArchiveArt) {
		_artHandler = new ArtHandler(this);
		_artHandler->open(kZoneSY, kLoadFromDisk);
	}

	_archiveType = kArchiveFile;

	// Setup fonts
	_fontHandler = new FontHandler();
	switch (languageGetCurrent()) {
	default:
		fontAdd(kFontDefault, "arxrin.fon", "ARX Pilgrim L", 12, true, false, false, false, languageGetCurrent());
		break;

	case kLanguageHebrew:
		fontAdd(kFontDefault, "arxrin.fon", "ArxelHebrew", 12, true, false, false, false, languageGetCurrent());
		break;

	case kLanguageGreek:
		// FIXME replace by proper GUI font
		fontAdd(kFontDefault, "arxrin.fon", "Arial", 16, true, false, false, false, languageGetCurrent());
		break;
	}

	// Setup dialogs
	_dialogHandler = new DialogHandler();
	_dialogHandler->init(kFontDefault, Color(200, 200, 30), Color(0, 0, 0), 461, 3);

	// Setup cursors
	_cursorHandler = new CursorHandler();

	// Setup sound
	_soundHandler = new SoundHandler();

	// Setup data
	_field_70 = 0;
	_field_74 = 1;
	_field_75 = 1;
	_field_76 = 1;
	_field_77 = 1;
	_field_78 = 1;
	_loadFrom = kLoadFromCd;
	_field_54 = 1;
	_archiveType = kArchiveFile;
	_field_6F = 0;

	_rotation = NULL;

	// Setup bag
	if (_configuration.artBAG)
		_archiveType = kArchiveArt;

	_bag= new Bag();
	_bag->sub_417D20(0, 0);
	_bag->sub_417D40(18, 42, 44, 100);
	_bag->sub_417D60(0, 0);
	_bag->sub_417DD0(6);
	_bag->sub_417D80(0, 24, 30, 448);
	_bag->sub_417DA0(610, 24, 30, 448);
	_bag->sub_4192A0(7, 48);
	_bag->sub_4192C0(627, 48);
	_bag->sub_417DE0(335, 8);
	_bag->sub_419280(500);
	_bag->loadBackground("bagbgr.tga", "", "bagarr.tga", "", "", "bagarr.tga", "", "menu_gur.tga", _archiveType);
	_bag->initHotspots();

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
				_configuration.backgroundMusic.soundChunck = atoi((char *)&val);

			if (Common::String(token) == "LOADFROM_BGRMUS:")
				_configuration.backgroundMusic.loadFrom = (LoadFrom)atoi((char *)&val);

			if (Common::String(token) == "SOUNDCHUNCK_AMBMUS:")
				_configuration.ambientMusic.soundChunck = atoi((char *)&val);

			if (Common::String(token) == "LOADFROM_AMBMUS:")
				_configuration.ambientMusic.loadFrom = (LoadFrom)atoi((char *)&val);

			if (Common::String(token) == "SOUNDCHUNCK_AMBEFE:")
				_configuration.ambientEffect.soundChunck = atoi((char *)&val);

			if (Common::String(token) == "LOADFROM_AMBEFE:")
				_configuration.ambientEffect.loadFrom = (LoadFrom)atoi((char *)&val);

			if (Common::String(token) == "SOUNDCHUNCK_EFE:")
				_configuration.effect.soundChunck = atoi((char *)&val);

			if (Common::String(token) == "LOADFROM_EFE:")
				_configuration.effect.loadFrom = (LoadFrom)atoi((char *)&val);

			if (Common::String(token) == "SOUNDCHUNCK_DIA:")
				_configuration.dialog.soundChunck = atoi((char *)&val);

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

void Application::exitZone() {
	error("[Application::exitZone] Not implemented");
}

void Application::exitToMenu(uint32 a1) {
	_state = kStateShowMenu;
	_field_70 = a1;
	soundStopAll(64);
}

#pragma endregion

#pragma region Event handling

void Application::onMouseLeftButtonUp(Common::Event &evt, bool isControlPressed) {
	if (!_eventHandler)
		error("[Application::onMouseLeftButtonUp] Event handler not initialized properly!");

	_eventHandler->onMouseLeftButtonUp(evt, isControlPressed);
}

void Application::onMouseLeftButtonDown(Common::Event &evt) {
	if (!_eventHandler)
		error("[Application::onMouseLeftButtonDown] Event handler not initialized properly!");

	_eventHandler->onMouseLeftButtonDown(evt);
}

void Application::onMouseRightButtonUp(Common::Event &evt) {
	if (!_eventHandler)
		error("[Application::onMouseRightButtonUp] Event handler not initialized properly!");

	_eventHandler->onMouseRightButtonUp(evt);
}

void Application::onKeyDown(Common::Event &evt) {
	if (!_eventHandler)
		error("[Application::onKeyDown] Event handler not initialized properly!");

	_eventHandler->onKeyDown(evt);
}

void Application::onTimer(TimerId id) {
	if (!_eventHandler)
		error("[Application::onTimer] Event handler not initialized properly!");

	if (_field_6A)
		return;

	_eventHandler->onTimer(id);

	if (_timerHandler)
		_timerHandler->incrementFiredCount(id);
}

void Application::onSound(Id id, SoundType type, uint32 a3) {
	if (!_eventHandler)
		error("[Application::onSound] Event handler not initialized properly!");

	_eventHandler->onSound(id, type, a3);
}

void Application::update(const Common::Point &point) {
	error("[Application::update] Not implemented");
}

#pragma endregion

#pragma region Display and Movies

int Application::scrollImage(Common::String filename,  uint32 ticksWait, LoadFrom loadFrom, ArchiveType archiveType) {
	error("[Application::scrollImage] Not implemented");
}

void Application::displayFade(Common::String filenameFrom, Common::String filenameTo, uint32 a3, uint32 ticksWait, LoadFrom loadFrom, ArchiveType archiveType) {
	warning("[Application::displayFade] Not implemented");
}

void Application::playMovie(Common::String filename, float a2) {
	warning("[Application::playMovie] Not implemented (%s)", filename.c_str());
}

void Application::playMovieChannel(Common::String filename, uint32 channel) {
	warning("[Application::playplayMovieChannelMovie] Not implemented (%s)", filename.c_str());
}

#pragma endregion

#pragma region Language, Fonts and Cursors

void Application::languageAdd(LanguageId id, Common::String name, Common::String folder, uint channel) {
	if (!_languageHandler)
		error("[Application::languageAdd] Language handler is not initialized properly");

	_languageHandler->add(id, name, folder, channel);
}

LanguageId Application::languageGetCurrent() {
	if (!_languageHandler)
		error("[Application::getLanguage] Language handler is not initialized properly");

	return _languageHandler->getLanguage();
}

Common::String Application::languageGetFolder() {
	LanguageId id = languageGetCurrent();

	return _languageHandler->getFolder(id);
}

uint32 Application::languageGetChannel() {
	LanguageId id = languageGetCurrent();

	return _languageHandler->getChannel(id);
}

void Application::fontAdd(FontId id, Common::String filename, Common::String facename, uint32 height, bool smallWeight, bool underline, bool italic, bool strikeout, LanguageId langId) {
	if (!_fontHandler)
		error("[Application::fontAdd] Font handler is not initialized properly");

	_fontHandler->add(id, filename, facename, height, smallWeight, underline, italic, strikeout, langId);
}

void Application::cursorAdd(CursorId id, Common::String name, CursorType cursorType, byte frameCount, LoadFrom loadFrom, ArchiveType archiveType) {
	if (!_cursorHandler)
		error("[Application::cursorAdd] Cursor handler is not initialized properly");

	_cursorHandler->add(id, name, cursorType, frameCount, 0, 0, 0, loadFrom, archiveType);
}

void Application::cursorAdd(CursorId id, Common::String name, CursorType cursorType, byte frameCount, uint32 a4, float a5, byte a6, LoadFrom loadFrom, ArchiveType archiveType) {
	if (!_cursorHandler)
		error("[Application::cursorAdd] Cursor handler is not initialized properly");

	_cursorHandler->add(id, name, cursorType, frameCount, a4, a5, a6, loadFrom, archiveType);
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
		error("[Application::removeFromBag] Invalid ID (%d)", objectId.id());

	if (!_bag)
		error("[Application::removeFromBag] bag is not initialized properly");

	if (!_objects.has(objectId))
		error("[Application::removeFromBag] ID doesn't exist (%d)", objectId.id());

	_bag->add(objectId);
}

void Application::bagRemove(ObjectId objectId) {
	if (!_bag)
		error("[Application::removeFromBag] bag is not initialized properly");

	if (!_objects.has(objectId))
		error("[Application::removeFromBag] ID doesn't exist (%d)", objectId.id());

	_bag->remove(objectId);
}

void Application::bagRemoveAll() {
	if (!_bag)
		error("[Application::removeAllFromBag] bag is not initialized properly");

	_bag->removeAll();
}

bool Application::bagHas(ObjectId objectId) {
	if (!_bag)
		error("[Application::removeFromBag] bag is not initialized properly");

	if (!_objects.has(objectId))
		error("[Application::removeFromBag] ID doesn't exist (%d)", objectId.id());

	return _bag->has(objectId);
}

bool Application::bagHasClickedObject() {
	if (!_bag)
		error("[Application::bagHasClickedObject] bag is not initialized properly");

	return _bag->getClickedObject() != kObjectInvalid;
}

ObjectId Application::bagGetClickedObject() {
	if (!_bag)
		error("[Application::bagHasClickedObject] bag is not initialized properly");

	return _bag->getClickedObject();
}

void Application::bagOpen(const Common::Point &point) {
	error("[Application::bagOpen] Not implemented!");
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

#pragma endregion

#pragma region Message

bool Application::messageGet(Common::String messageId) {
	// Open a stream to the message file
	Common::SeekableReadStream *archive = SearchMan.createReadStreamForMember("ames.ini");
	if (!archive)
		error("[Application::messageGet] Error opening objects file (ames.ini)");

	Common::String language = languageGetFolder();

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

void Application::puzzleAddBackgroundImage(PuzzleId puzzleId, Common::String filename, const Common::Point &point, bool isActive) {
	if (!_puzzles.has(puzzleId))
		error("[Application::puzzleAddBackgroundImage] ID doesn't exist (%d)", puzzleId.id());

	_puzzles.get(puzzleId)->setBackgroundImage(filename, point, isActive, _loadFrom);
}

#pragma region Puzzle Movability

void Application::puzzleAddMovabilityToPuzzle(PuzzleId puzzleIdFrom, PuzzleId puzzleIdTo, Common::String name, const Common::Rect &rect, bool enabled, uint32 a9, uint32 a10) {
	if (!_puzzles.has(puzzleIdFrom))
		error("[Application::puzzleAddMovabilityToPuzzle] Wrong FROM puzzle Id (%d)", puzzleIdFrom.id());

	if (!_puzzles.has(puzzleIdTo))
		error("[Application::puzzleAddMovabilityToPuzzle] Wrong TO puzzle Id (%d)", puzzleIdTo.id());

	Movability *movability = new Movability(puzzleIdFrom, puzzleIdTo, name, kMovabilityPuzzleToPuzzle);
	movability->setHotspot(rect, enabled, a9, a10);

	_puzzles.get(puzzleIdFrom)->addMovability(movability);
}

void Application::puzzleAddMovabilityToRotation(PuzzleId puzzleIdFrom, Id rotationIdTo, Common::String name, const Common::Rect &rect, bool enabled, uint32 a9, uint32 a10) {
	if (!_rotations.has(rotationIdTo))
		error("[Application::puzzleAddMovabilityToRotation] Wrong TO rotation Id (%d)", rotationIdTo);

	if (!_puzzles.has(puzzleIdFrom))
		error("[Application::puzzleAddMovabilityToRotation] Wrong FROM puzzle Id (%d)", puzzleIdFrom.id());

	Movability *movability = new Movability(puzzleIdFrom, rotationIdTo, name, kMovabilityPuzzleToRotation);
	movability->setHotspot(rect, enabled, a9, a10);

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

	puzzleReset();
	_puzzle = _puzzles.get(id);
	_puzzle->alloc();
	_puzzle->update(_screenManager);

	_state = kStateUpdatePuzzle;

	if (_soundHandler->getField0()) {
		_soundHandler->sub_41B520();

		_soundHandler->reset();
		_soundHandler->setCount1(_puzzle->getSoundItemsCount());
		return;
	}

	_soundHandler->setCount2(_puzzle->getSoundItemsCount());
	if (_soundHandler->sub_41AA00()) {

		_soundHandler->turnOffItems1();

		if (_soundHandler->sub_41AEE0(2)) {

			_soundHandler->sub_41B180(3);
			_soundHandler->sub_41B350(3);
			_soundHandler->sub_41B520();

			_soundHandler->reset();
			_soundHandler->setCount1(_puzzle->getSoundItemsCount());
			return;
		}
	}

	_soundHandler->turnOffItems2(a3);

	if (updateSoundItems)
		_puzzle->updateSoundItems();

	_soundHandler->reset();
	_soundHandler->setCount1(_puzzle->getSoundItemsCount());
}

void Application::puzzleReset() {
	SAFE_DELETE(_puzzle);
	SAFE_DELETE(_rotation);

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

void Application::objectAdd(ObjectId objectId, Common::String language, Common::String name, byte a5) {
	if (!_objectHandler)
		error("[Application::objectAdd] Object handler not initialized properly");

	if (_objects.has(objectId))
		error("[Application::ObjectAdd] ID already exists (%d)", objectId.id());

	// Compute language and name
	Common::String processedLanguage = _objectHandler->getLanguage(objectId);
	if (processedLanguage.empty())
		processedLanguage = language;

	Common::String processedName = _objectHandler->getName(objectId);
	if (processedName.empty())
		processedName = name;

	_objects.push_back(new Object(this, objectId, processedLanguage, processedName, a5));
}

void Application::objectAddBagAnimation(ObjectId objectId, uint32 a2, uint32 a3, uint32 frameCount, float a5, uint32 a6) {
	if (!_objects.has(objectId))
		error("[Application::objectAddBagAnimation] Object Id doesn't exist (%d)", objectId.id());

	Object *object = _objects.get(objectId);
	AnimationImage *image = new AnimationImage();
	image->init(object->getName(), a2, Common::Point(0, 0), 0, a3, frameCount, a5, 1, a6, 0, 1000, kLoadFromListIcon, (_configuration.artBAG ? kArchiveArt : kArchiveFile));
	image->setField89();

	object->setAnimationImage(image);
}

void Application::objectRemove(ObjectId id) {
	_objects.remove(id);
}

#pragma region Object Accessibility

void Application::objectAddPuzzleAccessibility(ObjectId objectId, PuzzleId puzzleId, const Common::Rect &rect, bool enabled, uint32 a9, uint32 a10) {
	if (!_objects.has(objectId))
		error("[Application::objectAddPuzzleAccessibility] Object Id doesn't exist (%d)", objectId.id());

	if (!_puzzles.has(puzzleId))
		error("[Application::objectAddPuzzleAccessibility] Puzzle Id doesn't exist (%d)", puzzleId.id());

	_objects.get(objectId)->addPuzzleAccessibility(_puzzles.get(puzzleId), rect, enabled, a9, a10);
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

void Application::objectAddRotationAccessibility(ObjectId objectId, Id rotationId, const Common::Rect &rect, bool enabled, uint32 a9, uint32 a10) {
	if (!_objects.has(objectId))
		error("[Application::objectAddRotationAccessibility] Object Id doesn't exist (%d)", objectId.id());

	if (!_rotations.has(rotationId))
		error("[Application::objectAddRotationAccessibility] Rotation Id doesn't exist (%d)", rotationId);

	_objects.get(objectId)->addRotationAccessibility(_rotations.get(rotationId), rect, enabled, a9, a10);
}

#pragma endregion

void Application::objectSetActiveCursor(ObjectId objectId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8) {
	if (!_objects.has(objectId))
		error("[Application::objectSetActiveCursor] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setActiveCursor(a2, a3, a4, a5, a6, a7, a8, _configuration.artBAG ? kArchiveArt : kArchiveFile);
}

void Application::objectSetPassiveCursor(ObjectId objectId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8) {
	if (!_objects.has(objectId))
		error("[Application::objectSetPassiveCursor] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setPassiveCursor(a2, a3, a4, a5, a6, a7, a8, _configuration.artBAG ? kArchiveArt : kArchiveFile);
}

void Application::objectSetActiveDrawCursor(ObjectId objectId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8) {
	if (!_objects.has(objectId))
		error("[Application::objectSetActiveDrawCursor] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setActiveDrawCursor(a2, a3, a4, a5, a6, a7, a8, _configuration.artBAG ? kArchiveArt : kArchiveFile);
}

void Application::objectSetPassiveDrawCursor(ObjectId objectId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8) {
	if (!_objects.has(objectId))
		error("[Application::objectSetPassiveDrawCursor] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setPassiveDrawCursor(a2, a3, a4, a5, a6, a7, a8, _configuration.artBAG ? kArchiveArt : kArchiveFile);
}

void Application::objectAddPresentation(ObjectId objectId) {
	if (!_objects.has(objectId))
		error("[Application::objectAddPresentation] Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->addPresentation();
}

void Application::objectPresentationAddTextToPuzzle(ObjectId objectId, uint32 presentationIndex, PuzzleId puzzleId, Common::String text, const Common::Point &point, FontId fontId, Color foreground, Color background) {
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

void Application::objectPresentationAddImageToPuzzle(ObjectId objectId, uint32 presentationIndex, PuzzleId puzzleId, Common::String filename, const Common::Point &point, bool isActive, uint32 a8, uint32 priority) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationAddImageToPuzzle] Object Id doesn't exist (%d)", objectId.id());

	if (!_puzzles.has(puzzleId))
		error("[Application::objectPresentationAddImageToPuzzle] Puzzle Id doesn't exist (%d)", puzzleId.id());

	_objects.get(objectId)->addImageToPuzzle(presentationIndex, _puzzles.get(puzzleId), filename, point, isActive, a8, priority, 0, _loadFrom);
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

void Application::objectPresentationAddAnimationToPuzzle(ObjectId objectId, uint32 presentationIndex, PuzzleId puzzleId, Common::String filename, uint32 a5, const Common::Point &point, uint32 a8, uint32 a9, uint32 a10, float a11, uint32 a12) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationAddAnimationToPuzzle] Object Id doesn't exist (%d)", objectId.id());

	if (!_puzzles.has(puzzleId))
		error("[Application::objectPresentationAddAnimationToPuzzle] Puzzle Id doesn't exist (%d)", puzzleId.id());

	_objects.get(objectId)->addAnimationToPuzzle(presentationIndex, _puzzles.get(puzzleId), filename, a5, point, 1, a8, a9, 0, a10, a11, a12, _loadFrom);
}

void Application::objectPresentationAddAnimationToRotation(ObjectId objectId, uint32 presentationIndex, Id rotationId, uint32 layer, uint32 a5, float a6, uint32 a7) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationAddImageToRotation] Object Id doesn't exist (%d)", objectId.id());

	if (!_rotations.has(rotationId))
		error("[Application::objectPresentationAddImageToRotation] Rotation Id doesn't exist (%d)", rotationId);

	Rotation *rotation = _rotations.get(rotationId);
	if (layer >= rotation->getLayerCount())
		error("[Application::objectPresentationAddImageToRotation] Rotation layer to big (was:%d, max: %d)", layer, rotation->getLayerCount() - 1);

	_objects.get(objectId)->addAnimationToRotation(presentationIndex, rotation, layer, a5, a6, a7);
}

void Application::objectPresentationSetAnimationOnPuzzle(ObjectId objectId, uint32 presentationIndex, uint32 animationIndex, const ObjectId &targetId) {
	if (!_objects.has(objectId))
		error("[Application::objectPresentationSetAnimationOnPuzzle] Object Id doesn't exist (%d)", objectId.id());

	_objects.get(objectId)->setAnimationOnPuzzle(presentationIndex, animationIndex, targetId);
}

void Application::objectPresentationSetAnimationOnRotation(ObjectId objectId, uint32 presentationIndex, uint32 animationIndex, const ObjectId &targetId) {
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

#pragma endregion

#pragma region Rotation

void Application::rotationAdd(Id rotationId, Common::String name, byte a3, uint32 nodeCount) {
	if (_rotations.has(rotationId))
		error("[Application::rotationAdd] Rotation Id already exists (%d)", rotationId);

	// Compute path
	//  Note: .aqi files do not seem to exists in the data
	Common::String path = Common::String::format("DATA/NODE/%s.%s", name.c_str(), _field_54 ? "aqc" : "aqi");

	// Check for single node file
	if (_field_5E && !Common::File::exists(path))
		error("[Application::rotationAdd] Node file doesn't exist (%s)", path.c_str());

	// Check for node channel files
	if (_field_5E && !_field_54 && nodeCount > 0) {
		for (uint i = 0; i < nodeCount; i++) {
			Common::String nodePath = Common::String::format("DATA/NODE/%s_%03d.aqc", name.c_str(), i);

			if (!Common::File::exists(nodePath))
				error("[Application::rotationAdd] Channel node file doesn't exist (%s)", nodePath.c_str());
		}
	}

	// Create rotation
	Rotation *rotation = new Rotation(rotationId, name, a3, _loadFrom, nodeCount, _field_54);

	_rotations.push_back(rotation);
}

void Application::rotationSetComBufferLength(Id rotationId, uint32 length) {
	if (!_rotations.has(rotationId))
		error("[Application::rotationAdd] Rotation Id doesn't exist (%d)", rotationId);

	_rotations.get(rotationId)->setComBufferLength(length);
}

void Application::rotationAddMovabilityToPuzzle(Id fromRotationId, PuzzleId toPuzzleId, Common::String name, const Common::Rect &rect, bool enabled, uint32 a9, uint32 a10) {
	// Check ids
	if (!_rotations.has(fromRotationId))
		error("[Application::rotationAddMovabilityToRotation] Wrong From rotation Id (%d)", fromRotationId);

	if (!_puzzles.has(toPuzzleId))
		error("[Application::rotationAddMovabilityToRotation] Wrong To puzzle Id (%d)", toPuzzleId.id());

	// Create movability
	Movability *movability = new Movability(fromRotationId, toPuzzleId, name, kMovabilityRotationToPuzzle);
	movability->setHotspot(rect, enabled, a9, a10);

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

void Application::rotationAddMovabilityToRotation(Id fromRotationId, Id toRotationId, Common::String name, const Common::Rect &rect, bool enabled, uint32 a9, uint32 a10) {
	// Check ids
	if (!_rotations.has(fromRotationId))
		error("[Application::rotationAddMovabilityToRotation] Wrong From rotation Id (%d)", fromRotationId);

	if (!_rotations.has(toRotationId))
		error("[Application::rotationAddMovabilityToRotation] Wrong To rotation Id (%d)", toRotationId);

	// Create movability
	Movability *movability = new Movability(fromRotationId, toRotationId, name, kMovabilityRotationToRotation);
	movability->setHotspot(rect, enabled, a9, a10);

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

	puzzleReset();
	_rotation = _rotations.get(id);
	_rotation->load();

	g_system->warpMouse(320, 240);

	_state = kStateUpdateRotation;

	if (_soundHandler->getField0()) {
		_soundHandler->sub_41B520();

		_soundHandler->reset();
		_soundHandler->setCount1(_rotation->getSoundItemsCount());
		return;
	}

	_soundHandler->setCount2(_rotation->getSoundItemsCount());

	if (_soundHandler->sub_41AA00()) {

		_soundHandler->turnOffItems1();

		if (_soundHandler->sub_41AEE0(2)) {

			_soundHandler->sub_41B180(3);
			_soundHandler->sub_41B350(3);
			_soundHandler->sub_41B520();

			_soundHandler->reset();
			_soundHandler->setCount1(_rotation->getSoundItemsCount());
			return;
		}
	}

	_soundHandler->turnOffItems2(a3);

	_rotation->updateSoundItems();

	_soundHandler->reset();
	_soundHandler->setCount1(_rotation->getSoundItemsCount());
}

#pragma endregion

#pragma region Sound

void Application::soundAdd(Id soundId, SoundType type, Common::String filename, LoadFrom loadFrom) {
	soundAdd(soundId, type, filename, loadFrom, 2, 3);
}

void Application::soundAdd(Id soundId, SoundType type, Common::String filename, LoadFrom loadFrom, uint32 a4, int soundChunk) {
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
		path = Common::String::format("DATA/%s/SOUND/%s/%s", _zoneString.c_str(), languageGetFolder().c_str(), filename.c_str());
	else
		path = Common::String::format("DATA/%s/SOUND/%s", _zoneString.c_str(), filename.c_str());

	if (!Common::File::exists(path))
		error("[Application::soundAdd] File doesn't exist (%s)", path.c_str());

	_soundManager->addEntry(soundId, type, filename, loadFrom, format, a4 != 1, soundChunk);
}

void Application::soundPlay(Id soundId, int a2) {
	_soundManager->play(soundId, a2 != 1);
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

void Application::soundSetMultiplier(SoundType soundType, uint32 multiplier) {
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

bool Application::timerHas(TimerId id) {
	return _timerHandler->has(id);
}

#pragma endregion

#pragma region Var

#define IMPLEMENT_VAR_FUNCTIONS(name, type) \
	void Application::varDefine##name(Id id, type val) { \
		if (!_var) \
			error("[Application::varDefine##name] Var not initialized properly"); \
		_var->define##name(id, val); \
	} \
	type Application::varGet##name(Id id) { \
		if (!_var) \
			error("[Application::varGet##name] Var not initialized properly"); \
		return _var->get##name(id); \
	} \
	void Application::varSet##name(Id id, type val) { \
		if (!_var) \
			error("[Application::varSet##name] Var not initialized properly"); \
		_var->set##name(id, val); \
	}

IMPLEMENT_VAR_FUNCTIONS(Byte,   byte);
IMPLEMENT_VAR_FUNCTIONS(Word,   int16);
IMPLEMENT_VAR_FUNCTIONS(Dword,  int32);
IMPLEMENT_VAR_FUNCTIONS(String, Common::String);
IMPLEMENT_VAR_FUNCTIONS(Float,  float);

#undef IMPLEMENT_VAR_FUNCTIONS

#pragma endregion

#pragma region Visual

void Application::visualAddShowToPuzzle(Id visualId, PuzzleId puzzleId, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7, uint32 a8, uint32 a9, uint32 a10) {
	if (!_puzzles.has(puzzleId))
		error("[Application::visualAddListToPuzzle] Puzzle Id doesn't exist (%d)", puzzleId.id());

	Puzzle *puzzle = _puzzles.get(puzzleId);
	if (puzzle->hasVisual(visualId))
		error("[Application::visualAddListToPuzzle] Visual already exists on puzzle (%d)", visualId);

	VisualElement *visual = new VisualElement(visualId);
	visual->init(a3, a4, a5, a6, a7, a8, a9, a10);
	visual->setField8(2);
	visual->setFieldC(1);

	puzzle->addVisual(visual);
}

void Application::visualListAddToPuzzle(Id visualId, PuzzleId puzzleId, uint32 a3,
	                                    Common::String imagePath, Common::String iconPath, Common::String filename3, Common::String filename4, Common::String filename5, Common::String filename6, Common::String filename7, Common::String filename8, Common::String filename9, Common::String filename10,
	                                    Common::String filename11, Common::String filename12, Common::String filename13,
							            uint32 a17, uint32 a18, uint32 a19, uint32 a20, uint32 a21, uint32 a22, uint32 a23, uint32 a24, uint32 a25, uint32 a26,
	                                    uint32 a27, uint32 a28, uint32 a29, uint32 a30, uint32 a31, uint32 a32, uint32 a33, uint32 a34, uint32 a35, uint32 a36,
	                                    uint32 a37, uint32 a38, uint32 a39, uint32 a40, uint32 a41, uint32 a42, uint32 a43, uint32 a44, uint32 a45, uint32 a46,
							           Color foreground1, Color foreground2, Color background, FontId fontId,
							            ArchiveType archiveType) {


	if (!_puzzles.has(puzzleId))
		error("[Application::visualAddListToPuzzle] Puzzle Id doesn't exist (%d)", puzzleId.id());

	Puzzle *puzzle = _puzzles.get(puzzleId);
	if (puzzle->hasVisual(visualId))
		error("[Application::visualAddListToPuzzle] Visual already exists on puzzle (%d)", visualId);

	VisualObjectList *list = new VisualObjectList(visualId);
	list->init(a3, imagePath, iconPath, filename3, filename4, filename5, filename6, filename7, filename8, filename9, filename10, filename11, filename12, filename13, a17, archiveType);
	list->setField8(1);
	list->setFieldC(1);
	list->sub_46DCF0(a18, a19);
	list->sub_46DD11(a20, a21);
	list->sub_46DD30(a22, a23, a24, a25, a26, a27);
	list->sub_46DD60(a28, a29);
	list->sub_46DDA0(a30, a31, a32, a33);
	list->sub_46DD80(a34, a35);
	list->sub_46DDD0(a36, a37, a38, a39);
	list->sub_46DE00(a40, a41, a42, a43);
	list->sub_46DE30(a44, a45);
	list->sub_46E330(a46);
	list->initHotspots();
	list->setTextForegroundColor(foreground1, foreground2);
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
		error("[Application::visualListAdd] Puzzle Id doesn't exist (%d)", puzzleId.id());

	Puzzle *puzzle = _puzzles.get(puzzleId);
	if (!puzzle->hasVisual(visualId))
		error("[Application::visualListAdd] Visual (%d) is not on puzzle (%d)", visualId, puzzleId.id());

	((VisualObjectList *)puzzle->getVisual(visualId))->remove(objectId, removeObject);
}

void Application::visualListRemove(Id visualId, PuzzleId puzzleId, bool removeObject) {
	if (!_puzzles.has(puzzleId))
		error("[Application::visualListAdd] Puzzle Id doesn't exist (%d)", puzzleId.id());

	Puzzle *puzzle = _puzzles.get(puzzleId);
	if (!puzzle->hasVisual(visualId))
		error("[Application::visualListAdd] Visual (%d) is not on puzzle (%d)", visualId, puzzleId.id());

	((VisualObjectList *)puzzle->getVisual(visualId))->removeAll(removeObject);
}

uint32 Application::visualListGetItemCount(Id visualId, PuzzleId puzzleId) {
	if (!_puzzles.has(puzzleId))
		error("[Application::visualListAdd] Puzzle Id doesn't exist (%d)", puzzleId.id());

	Puzzle *puzzle = _puzzles.get(puzzleId);
	if (!puzzle->hasVisual(visualId))
		error("[Application::visualListAdd] Visual (%d) is not on puzzle (%d)", visualId, puzzleId.id());

	return ((VisualObjectList *)puzzle->getVisual(visualId))->getItemCount();
}

#pragma endregion

} // End of namespace Ring
