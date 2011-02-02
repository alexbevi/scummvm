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

#include "ring/game/application.h"

#include "ring/base/art.h"
#include "ring/base/cursor.h"
#include "ring/base/font.h"
#include "ring/base/preferences.h"
#include "ring/base/sound.h"
#include "ring/base/timer.h"
#include "ring/base/var.h"

#include "ring/game/bag.h"
#include "ring/game/dialog.h"
#include "ring/game/language.h"
#include "ring/game/object.h"
#include "ring/game/puzzle.h"
#include "ring/game/saveload.h"

#include "ring/graphics/dragControl.h"
#include "ring/graphics/rotation.h"
#include "ring/graphics/video.h"

#include "ring/helpers.h"
#include "ring/ring.h"

namespace Ring {

Application::Application(RingEngine *engine) : _vm(engine),
	_video(NULL),        _artHandler(NULL),       _fontHandler(NULL),   _dialogHandler(NULL), _languageHandler(NULL),
	_field_54(1),        _archiveType(kArchiveFile), _cursorHandler(NULL), _loadFrom(kLoadFromInvalid), _field_5E(0),
	_soundHandler(NULL), _field_66(0),            _field_6A(0),         _zoneString("A0"),      _zone(kZoneInvalid),
	_field_6F(0),        _field_70(0),            _field_74(0),         _field_75(0),         _field_76(0),
	_field_77(0),        _field_78(0),            _puzzle(NULL),        _field_89(NULL),      _bag(NULL),
	_timerHandler(NULL), _var(NULL),              _dragControl(NULL),   _objectHandler(NULL), _preferenceHandler(NULL),
	_controlNotPressed(false) {
	_saveLoad = new SaveLoad();
}

Application::~Application() {
	// TODO delete global image buffer

	SAFE_DELETE(_video);
	SAFE_DELETE(_artHandler);
	SAFE_DELETE(_fontHandler);
	SAFE_DELETE(_dialogHandler);
	SAFE_DELETE(_languageHandler);
	SAFE_DELETE(_cursorHandler);
	SAFE_DELETE(_soundHandler);
	CLEAR_ARRAY(Object, _objectList);
	CLEAR_ARRAY(Puzzle, _puzzleList);
	SAFE_DELETE(_puzzle);
	CLEAR_ARRAY(Rotation, _rotationList);
	SAFE_DELETE(_field_89);
	SAFE_DELETE(_bag);
	SAFE_DELETE(_timerHandler);
	SAFE_DELETE(_var);
	SAFE_DELETE(_dragControl);
	SAFE_DELETE(_objectHandler);
	SAFE_DELETE(_preferenceHandler);

	SAFE_DELETE(_saveLoad);

	// Zero-out passed pointers
	_vm = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Initialization
//////////////////////////////////////////////////////////////////////////
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
	_video = new Video();
	_video->init(); // TODO probably not needed as we initialize graphics at engine start
	_video->sub_4028D0(0, 0);

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
	_dialogHandler->init(1, Color(200, 200, 30), Color(0, 0, 0), 461, 3);

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

	_field_89 = NULL;

	// Setup bag
	if (_configuration.artBAG)
		_archiveType = kArchiveArt;

	_bag= new Bag(this);
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
}

//////////////////////////////////////////////////////////////////////////
// Startup
//////////////////////////////////////////////////////////////////////////
void Application::showStartupScreen() {
	error("[Application::showStartupScreen] Not implemented");
}

void Application::startMenu() {
	error("[Application::startMenu] Not implemented");
}

//////////////////////////////////////////////////////////////////////////
// Event handling
//////////////////////////////////////////////////////////////////////////
void Application::onMouseLeftButtonUp(Common::Event &evt, bool isControlPressed) {
	_controlNotPressed = !isControlPressed;

	onMouseLeftButtonUp(evt);
}

void Application::onMouseLeftButtonUp(Common::Event &evt) {
	error("[Application::onMouseLeftButtonUp] Not implemented");
}

void Application::onMouseLeftButtonDown(Common::Event &evt) {
	error("[Application::onMouseLeftButtonDown] Not implemented");
}

void Application::onMouseRightButtonUp(Common::Event &evt) {
	error("[Application::onMouseRightButtonUp] Not implemented");
}

void Application::onKeyDown(Common::Event &evt) {
	error("[Application::onKeyDown] Not implemented");
}

void Application::onTimer(TimerId id) {
	if (_field_6A)
		return;

	onZoneTimer(id);

	if (_timerHandler)
		_timerHandler->incrementFiredCount(id);
}

void Application::onZoneTimer(TimerId id) {
	error("[Application::onZoneTimer] Not implemented");
}

//////////////////////////////////////////////////////////////////////////
// Language, Fonts and Cursors
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

void Application::cursorAdd(CursorId id, Common::String name, CursorType cursorType, uint32 a3, ImageType imageType, ArchiveType archiveType) {
	if (!_cursorHandler)
		error("[Application::cursorAdd] Cursor handler is not initialized properly");

	_cursorHandler->add(id, name, cursorType, a3, 0, 0, 0, imageType, archiveType);
}

void Application::cursorAdd(CursorId id, Common::String name, CursorType cursorType, uint32 a3, uint32 a4, uint32 a5, uint32 a6, ImageType imageType, ArchiveType archiveType) {
	if (!_cursorHandler)
		error("[Application::cursorAdd] Cursor handler is not initialized properly");

	_cursorHandler->add(id, name, cursorType, a3, a4, a5, a6, imageType, archiveType);
}

void Application::cursorSetOffset(CursorId id, Common::Point offset) {
	if (!_cursorHandler)
		error("[Application::cursorSetOffset] Cursor handler is not initialized properly");

	_cursorHandler->setOffset(id, offset);
}

void Application::subtitleSetColor(Color color) {
	if (!_dialogHandler)
		error("[Application::subtitleSetColor] Dialog handler is not initialized properly");

	_dialogHandler->setSubtitleColor(color);
}

void Application::subtitleSetBackgroundColor(Color color) {
	if (!_dialogHandler)
		error("[Application::subtitleSetBackgroundColor] Dialog handler is not initialized properly");

	_dialogHandler->setSubtitleBackgroundColor(color);
}

//////////////////////////////////////////////////////////////////////////
// Bag
//////////////////////////////////////////////////////////////////////////
void Application::bagAdd(ObjectId id) {
	if (id == kObjectNone)
		error("[Application::removeFromBag] Invalid ID (%d)", id);

	if (!_bag)
		error("[Application::removeFromBag] bag is not initialized properly");

	if (!_objectList.has(id))
		error("[Application::removeFromBag] ID doesn't exist (%d)", id);

	_bag->add(id);
}

void Application::bagRemove(ObjectId id) {
	if (!_bag)
		error("[Application::removeFromBag] bag is not initialized properly");

	if (!_objectList.has(id))
		error("[Application::removeFromBag] ID doesn't exist (%d)", id);

	_bag->remove(id);
}

void Application::bagRemoveAll() {
	if (!_bag)
		error("[Application::removeAllFromBag] bag is not initialized properly");

	_bag->removeAll();
}

bool Application::bagIsIn(ObjectId id) {
	if (!_bag)
		error("[Application::removeFromBag] bag is not initialized properly");

	if (!_objectList.has(id))
		error("[Application::removeFromBag] ID doesn't exist (%d)", id);

	return _bag->has(id);
}

//////////////////////////////////////////////////////////////////////////
// Puzzle
//////////////////////////////////////////////////////////////////////////
void Application::puzzleAdd(PuzzleId id) {
	if (_puzzleList.has(id))
		error("[Application::addPuzzle] ID already exists (%d)", id);

	_puzzleList.push_back(new Puzzle(id));
}

void Application::puzzleAddBackgroundImage(PuzzleId puzzleId, Common::String filename, uint32 a3, uint32 a4, uint32 a5) {
	error("[Application::puzzleAddBackgroundImage] Not implemented");
}

void Application::puzzleAddMovabilityToPuzzle(PuzzleId puzzleId, Id movabilityId, Common::String name, Common::Rect rect, uint32 a8, uint32 a9, uint32 a10) {
	error("[Application::puzzleAddMovabilityToPuzzle] Not implemented");
}

void Application::puzzleAddMovabilityToRotation(PuzzleId puzzleId, Id rotationId, Common::String name, Common::Rect rect, uint32 a8, uint32 a9, uint32 a10) {
	error("[Application::puzzleAddMovabilityToRotation] Not implemented");
}

void Application::puzzleSetMovabilityToRotation(PuzzleId puzzleId, uint32 movabilityIndex, uint32 a3, int32 a4, uint32 a5) {
	error("[Application::puzzleSetMovabilityToRotation] Not implemented");
}

void Application::puzzleAddAmbientSound(PuzzleId puzzleId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7) {
	error("[Application::puzzleAddAmbientSound] Not implemented");
}

void Application::puzzleSetAmbientSoundOff(PuzzleId puzzleId, Id soundId) {
	error("[Application::puzzleSetAmbientSoundOff] Not implemented");
}

void Application::puzzleAdd3DSound(PuzzleId puzzleId, Id soundId, uint32 a3, uint32 a4, uint32 a5, uint32 a6, float a7, uint32 a8) {
	error("[Application::puzzleAdd3DSound] Not implemented");
}

void Application::puzzleSet3DSoundOff(PuzzleId id, Id soundId) {
	error("[Application::puzzleSet3DSoundOff] Not implemented");
}

void Application::puzzleReset() {
	SAFE_DELETE(_puzzle);
	SAFE_DELETE(_field_89);

	// TODO two calls missing
	warning("[Application::resetPuzzle] Missing two function calls");
}

PuzzleId Application::getPuzzleId() {
	if (!_puzzle)
		return kPuzzleInvalid;

	return (PuzzleId)_puzzle->getId();
}

//////////////////////////////////////////////////////////////////////////
// Object
//////////////////////////////////////////////////////////////////////////
void Application::objectAdd(ObjectId id, Common::String language, Common::String name, byte a5) {
	if (_objectList.has(id))
		error("[Application::ObjectAdd] ID already exists (%d)", id);

	// Compute language and name
	Common::String processedLanguage = _objectHandler->getLanguage(id);
	if (processedLanguage.empty())
		processedLanguage = language;

	Common::String processedName = _objectHandler->getName(id);
	if (processedName.empty())
		processedName = name;

	_objectList.push_back(new Object(this, id, processedLanguage, processedName, a5));
}

void Application::objectRemove(ObjectId id) {
	_objectList.remove(id);
}

void Application::objectSetAccessibilityOnOrOff(ObjectId objectId, uint32 a2, uint32 a3) {
	error("[Application::objectSetAccessibilityOnOrOff] Not implemented");
}

void Application::objectAddPuzzleAccessibility(ObjectId objectId, PuzzleId puzzleId, Common::Rect rect, uint32 a8, uint32 a9, uint32 a10) {
	error("[Application::objectAddPuzzleAccessibility] Not implemented");
}

void Application::objectSetPuzzleAccessibilityKey(ObjectId id, uint32 accessiblityIndex, uint32 a3) {
	error("[Application::objectSetPuzzleAccessibilityKey] Not implemented");
}

void Application::objectAddRotationAccessibility(ObjectId, Id rotationId, Common::Rect rect, uint32 a8, uint32 a9, uint32 a10) {
	error("[Application::objectAddRotationAccessibility] Not implemented");
}

void Application::objectSetActiveCursor(ObjectId objectId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7, uint32 a8) {
	error("[Application::objectSetActiveCursor] Not implemented");
}

void Application::objectSetPassiveCursor(ObjectId objectId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7, uint32 a8) {
	error("[Application::objectSetPassiveCursor] Not implemented");
}

void Application::objectSetActiveDrawCursor(ObjectId objectId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7, uint32 a8) {
	error("[Application::objectSetActiveDrawCursor] Not implemented");
}

void Application::objectSetPassiveDrawCursor(ObjectId objectId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7, uint32 a8) {
	error("[Application::objectSetPassiveDrawCursor] Not implemented");
}

void Application::objectAddPresentation(ObjectId objectId) {
	if (!_objectList.has(objectId))
		error("[Application::objectAddPresentation] Id doesn't exist (%d)", objectId);

	_objectList.get(objectId)->addPresentation();
}

void Application::objectPresentationAddTextToPuzzle(ObjectId objectId, uint32 presentationIndex, PuzzleId puzzleId, Common::String text, uint32 a5, uint32 a6, FontId fontId, uint32 a8, int32 a9, int32 a10, int32 a11, int32 a12, int32 a13) {
	if (!_objectList.has(objectId))
		error("[Application::objectPresentationAddTextToPuzzle] Object Id doesn't exist (%d)", objectId);

	if (!_puzzleList.has(puzzleId))
		error("[Application::objectPresentationAddTextToPuzzle] Puzzle Id doesn't exist (%d)", puzzleId);

	_objectList.get(objectId)->addTextToPuzzle(presentationIndex, _puzzleList.get(puzzleId), text, a5, a6, fontId, a8, a9, a10, a11, a12, a13);
}

void Application::objectPresentationAddImageToPuzzle(ObjectId objectId, uint32 presentationIndex, PuzzleId puzzleId, Common::String filename, uint32 a5, uint32 a6, uint32 a7, uint32 a8, uint32 a9) {
	error("[Application::objectPresentationAddImageToPuzzle] Not implemented");
}

void Application::objectPresentationAddImageToRotation(ObjectId objectId, uint32 a2, uint32 a3, uint32 a4) {
	error("[Application::objectPresentationAddImageToRotation] Not implemented");
}

void Application::objectPresentationAddAnimationToPuzzle(ObjectId objectId, uint32 presentationIndex, PuzzleId puzzleId, Common::String filename, uint32 a5, uint32 a6, uint32 a7, uint32 a8, uint32 a9, uint32 a10, uint32 a11, uint32 a12) {
	error("[Application::objectPresentationAddAnimationToPuzzle] Not implemented");
}

void Application::objectPresentationAddAnimationToRotation(ObjectId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7) {
	error("[Application::objectPresentationAddAnimationToRotation] Not implemented");
}

void Application::objectPresentationSetAnimationOnPuzzle(ObjectId id, uint32 a2, uint32 a3, uint32 a4) {
	error("[Application::objectPresentationSetAnimationOnPuzzle] Not implemented");
}

void Application::objectPresentationSetAnimationOnRotation(ObjectId id, uint32 a2, uint32 a3, uint32 a4) {
	error("[Application::objectPresentationSetAnimationOnRotation] Not implemented");
}

void Application::objectPresentationSetAnimationCoordinatesOnPuzzle(ObjectId id, uint32 presentationIndex, Common::Point point) {
	error("[Application::objectPresentationSetAnimationCoordinatesOnPuzzle] Not implemented");
}

void Application::objectPresentationAnimationSetStartFrame(ObjectId objectId, uint32 animationIdex, uint32 startFrame) {
	error("[Application::objectPresentationAnimationSetStartFrame] Not implemented");
}

void Application::objectPresentationShow(ObjectId objectId, uint32 presentationIndex) {
	error("[Application::objectPresentationShow] Not implemented");
}

void Application::objectPresentationShow(ObjectId objectId) {
	error("[Application::objectPresentationShow] Not implemented");
}

void Application::objectPresentationPauseAnimation(ObjectId objectId, uint32 presentationIndex) {
	error("[Application::objectPresentationPauseAnimation] Not implemented");
}

void Application::objectAddBagAnimation(ObjectId objectId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6) {
	error("[Application::objectAddBagAnimation] Not implemented");
}

//////////////////////////////////////////////////////////////////////////
// Rotation
//////////////////////////////////////////////////////////////////////////
void Application::rotationAdd(Id rotationId, Common::String name, uint32 a3, uint32 a4) {
	error("[Application::rotationAdd] Not implemented");
}

void Application::rotationSetComBufferLength(Id rotationId, uint32 length) {
	error("[Application::rotationSetComBufferLength] Not implemented");
}

void Application::rotationSetMovabilityOnOrOff(Id rotationId, uint32 a2, uint32 a3) {
	error("[Application::rotationSetMovabilityOnOrOff] Not implemented");
}

void Application::rotationAddMovabilityToPuzzle(Id rotationId, PuzzleId puzzleId, Common::String name, uint32 a4, int32 a5, uint32 a6, uint32 a7, uint32 a8, uint32 a9, uint32 a10) {
	error("[Application::rotationAddMovabilityToPuzzle] Not implemented");
}

void Application::rotationSetMovabilityToPuzzle(Id rotationId, uint32 movabilityIndex, uint32 a3, int32 a4, uint32 a5, uint32 a6, uint32 a7) {
	error("[Application::rotationSetMovabilityToPuzzle] Not implemented");
}

void Application::rotationAddMovabilityToRotation(Id rotationId1, Id rotationId2, Common::String name, uint32 a4, int32 a5, uint32 a6, int32 a7, uint32 a8, uint32 a9, uint32 a10) {
	error("[Application::rotationAddMovabilityToRotation] Not implemented");
}

void Application::rotationSetMovabilityToRotation(Id rotationId, uint32 movabilityIndex, uint32 a3, int32 a4, uint32 a5,  uint32 a6, uint32 a7, int32 a8, int32 a9, uint32 a10) {
	error("[Application::rotationSetMovabilityToRotation] Not implemented");
}

void Application::rotationAddAmbientSound(Id rotationId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7) {
	error("[Application::rotationAddAmbientSound] Not implemented");
}

void Application::rotationSetAmbiantSoundOff(Id rotationId, Id soundId) {
	error("[Application::rotationSetAmbiantSoundOff] Not implemented");
}

void Application::rotationAdd3DSound(Id rotationId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, float a7, uint32 a8) {
	error("[Application::rotationAdd3DSound] Not implemented");
}

void Application::rotationSet3DSoundOff(Id rotationId, Id soundId) {
	error("[Application::rotationSet3DSoundOff] Not implemented");
}

void Application::rotationSetJugOn(Id rotationId, float amplitude, float speed) {
	error("[Application::rotationSetJugOn] Not implemented");
}

//////////////////////////////////////////////////////////////////////////
// Sound
//////////////////////////////////////////////////////////////////////////
void Application::soundAdd(Id soundId, uint32 a2, Common::String filename, LoadFrom loadFrom) {
	error("[Application::soundAdd] Not implemented");
}

void Application::soundAdd(Id soundId, uint32 a2, Common::String filename, LoadFrom loadFrom, uint32 a4, int soundChunk) {
	error("[Application::soundAdd] Not implemented");
}

void Application::soundSetVolume(Id soundId, uint32 volume) {
	error("[Application::soundSetVolume] Not implemented");
}

//////////////////////////////////////////////////////////////////////////
// Timer
//////////////////////////////////////////////////////////////////////////
void Application::timerStart(TimerId id, uint32 elapseTime) {
	error("[Application::timerStart] Not implemented");
}

void Application::timerStop(TimerId id) {
	error("[Application::timerStop] Not implemented");
}

void Application::timerStopAll() {
	error("[Application::timerStopAll] Not implemented");
}

//////////////////////////////////////////////////////////////////////////
// Var
//////////////////////////////////////////////////////////////////////////
void Application::varDefineByte(Id id, byte val) {
	error("[Application::varDefineByte] Not implemented");
}

void Application::varSetByte(Id id, byte val) {
	error("[Application::varSetByte] Not implemented");
}

byte Application::varGetByte(Id id) {
	error("[Application::varGetByte] Not implemented");
}

void Application::varDefineWord(Id id, int16 val) {
	error("[Application::varDefineWord] Not implemented");
}

void Application::varSetWord(Id id, int16 val) {
	error("[Application::varSetWord] Not implemented");
}

int16 Application::varGetWord(Id id) {
	error("[Application::varGetWord] Not implemented");
}

void Application::varDefineDword(Id id, int32 val) {
	error("[Application::varDefineDword] Not implemented");
}

void Application::varSetDword(Id id, uint32 val) {
	error("[Application::varSetDword] Not implemented");
}

uint32 Application::varGetDword(Id id) {
	error("[Application::varGetDword] Not implemented");
}

void Application::varDefineString(Id id, Common::String val) {
	error("[Application::varDefineString] Not implemented");
}

void Application::varSetString(Id id, Common::String val) {
	error("[Application::varSetString] Not implemented");
}

Common::String Application::varGetString(Id id) {
	error("[Application::varGetString] Not implemented");
}

void Application::varDefineFloat(Id id, float val) {
	error("[Application::varDefineFloat] Not implemented");
}

void Application::varSetFloat(Id id, float val) {
	error("[Application::varSetFloat] Not implemented");
}

float Application::varGetFloat(Id id) {
	error("[Application::varGetFloat] Not implemented");
}

//////////////////////////////////////////////////////////////////////////
// Visual
//////////////////////////////////////////////////////////////////////////
void Application::visualAddShowToPuzzle(Id visualId, PuzzleId puzzleId, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7, uint32 a8, uint32 a9, uint32 a10) {
	error("[Application::visualAddShowToPuzzle] Not implemented");
}

void Application::visualAddListToPuzzle(Id visualId, PuzzleId puzzleId, uint32 a3,
	                                    Common::String filename1, Common::String filename2, Common::String filename3, Common::String filename4, Common::String filename5, Common::String filename6, Common::String filename7, Common::String filename8, Common::String filename9, Common::String filename10,
	                                    Common::String filename11, Common::String filename12, Common::String filename13,
							            uint32 a16, uint32 a17, uint32 a18, uint32 a19, uint32 a20, uint32 a21, uint32 a22, uint32 a23, uint32 a24, uint32 a25,
	                                    uint32 a26, uint32 a27, uint32 a28, uint32 a29, uint32 a30, uint32 a31, uint32 a32, uint32 a33, uint32 a34, uint32 a35,
	                                    uint32 a36, uint32 a37, uint32 a38, uint32 a39, uint32 a40, uint32 a41, uint32 a42, uint32 a43, uint32 a44, uint32 a45,
							            uint32 a46, uint32 a47, uint32 a48, uint32 a49, uint32 a50, uint32 a51, int32 a52, int32 a53, int32 a54, int32 a55,
							            ArchiveType archiveType) {
	error("[Application::visualAddListToPuzzle] Not implemented");
}

} // End of namespace Ring
