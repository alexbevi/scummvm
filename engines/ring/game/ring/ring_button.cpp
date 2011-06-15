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

#include "ring/game/ring/ring_button.h"

#include "ring/base/event.h"
#include "ring/base/preferences.h"
#include "ring/base/saveload.h"

#include "ring/game/ring/ring_application.h"
#include "ring/game/ring/ring_event.h"
#include "ring/game/ring/ring_shared.h"

#include "ring/graphics/image.h"

#include "ring/helpers.h"
#include "ring/ring.h"

#include "common/textconsole.h"

using namespace RingGame;

namespace Ring {

EventButtonRing::EventButtonRing(ApplicationRing *application, EventHandlerRing *eventHandler) : _app(application), _event(eventHandler) {
	_presentationIndexSY = 0;
	_prefsSubtitles = false;

	_prefsVolumeDialog = 0;
	_object1Visible = false;
}

EventButtonRing::~EventButtonRing() {
	// Zero-out passed pointers
	_app = NULL;
	_event = NULL;
}

#pragma region Left Button Up/Down

void EventButtonRing::onButtonDownZoneNI(ObjectId id, uint32, Id puzzleRotationId, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObject10101:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			return;
		}

		switch (puzzleRotationId) {
		default:
			break;

		case 0:
			_app->soundPlay(10103);
			_app->objectSetAccessibilityOff(kObject10101, 1, 2);
			_app->objectSetAccessibilityOff(kObject10104);
			_app->objectSetAccessibilityOff(kObject10109);
			_app->objectSetAccessibilityOff(kObject10110);
			_app->objectPresentationShow(kObject10101, _app->varGetByte(10102) ? 4 : 3);
			_app->objectPresentationHide(kObject10100);
			_app->objectPresentationHide(kObject10109);
			break;

		case 1:
			_app->rotationSetRolTo(10101, 270.4f, 10.4f, 85.7f);
			_app->puzzleSetActive(kPuzzle10100);
			_app->objectPresentationHide(kObject10102);
			_app->soundPlay(10102);
			_app->objectSetAccessibilityOff(kObject10101, 1, 2);
			_app->objectSetAccessibilityOff(kObject10102, 1, 2);
			_app->objectSetAccessibilityOff(kObject10100);
			_app->objectSetAccessibilityOff(kObject10103);

			if (_app->varGetByte(10102)) {
				if (_app->varGetByte(10113) == 9) {
					_app->objectPresentationHide(kObject10101, 0);
					_app->objectPresentationShow(kObject10101, 5);
				} else {
					_app->objectSetAccessibilityOn(kObject10110);
				}

				_app->objectPresentationShow(kObject10100, 1);
				_app->objectPresentationShow(kObject10101, 2);
			} else {
				_app->objectPresentationShow(kObject10101, 0);
				_app->objectPresentationShow(kObject10101, 1);
			}

			_app->objectPresentationShow(kObject10100, 0);
			break;
		}
		break;

	case kObject10102:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			return;
		}

		if (_app->varGetByte(10113) != 9)
			break;

		switch (puzzleRotationId) {
		default:
			break;

		case 0:
			_app->objectSetAccessibilityOff(kObject10102, 1, 2);

			if (_app->varGetByte(10103)) {
				_app->soundPlay(10105);
				_app->objectSetAccessibilityOff(kObject10105);
				_app->objectPresentationShow(kObject10102, 3);
				_app->objectPresentationHide(kObject10100);
			} else {
				_app->objectSetAccessibilityOff(kObject10105);
				_app->puzzleSetActive(kPuzzle10100);
				_app->objectPresentationHide(kObject10102, 0);
				_app->objectPresentationHide(kObject10100);
				_app->objectSetAccessibilityOn(kObject10102, 0, 0);
				_app->objectSetAccessibilityOn(kObject10102, 2, 2);
			}
			break;

		case 1:
			_app->objectPresentationHide(kObject10102);
			if (_app->varGetByte(10102) == 1) {
				_app->soundPlay(10104);
				_app->objectSetAccessibilityOff(kObject10101, 1, 2);
				_app->objectSetAccessibilityOff(kObject10102, 1, 2);
				_app->objectSetAccessibilityOff(kObject10100);
				_app->objectSetAccessibilityOff(kObject10103);
				_app->rotationSetRolTo(10101, 270.4f, 10.4f, 85.7f);
				_app->puzzleSetActive(kPuzzle10100);
				_app->objectPresentationShow(kObject10102, 0);

				if (_app->varGetByte(10103)) {
					_app->objectPresentationShow(kObject10100, 2);
					_app->objectPresentationShow(kObject10102, 2);
				} else {
					_app->objectPresentationShow(kObject10102, 1);
				}
			} else {
				_app->rotationSetRolTo(10101, 270.4f, 10.4f, 85.7f);
				_app->puzzleSetActive(kPuzzle10100);
				_app->objectPresentationShow(kObject10102, 4);

				// FIXME: Original calls it 4 times. Check why.
				for (uint32 i = 0; i < 4; i++)
					handleEvents();


				_app->objectPresentationHide(kObject10102, 4);
				_app->objectSetAccessibilityOn(kObject10101, 0, 0);
				_app->objectSetAccessibilityOn(kObject10101, 2, 2);
			}
			break;
		}
		break;

	case kObject10109:
		if (!_app->bagHasClickedObject()) {
			_app->soundPlay(10106);
			_app->objectPresentationHide(kObject10109);
			_app->objectPresentationShow(kObject10109, (uint32)puzzleRotationId);

			if (puzzleRotationId) {
				_app->varSetByte(_app->varGetByte(10113) + 10108, (byte)puzzleRotationId);
				_app->varSetByte(10113, _app->varGetByte(10113) + 1);
			}
		}
		break;

	case kObject10430:
		_app->objectSetAccessibilityOff(kObject10430, 1, 2);
		_app->objectPresentationShow(kObject10430, _app->varGetByte(10430) ? 5 : 3);
		_app->objectPresentationHide(kObject10430, 0);
		_app->objectPresentationHide(kObject10430, 1);
		break;
	}
}

void EventButtonRing::onButtonDownZoneRO(ObjectId id, uint32 target, Id, uint32, const Common::Point &) {
	if (id != kObject40202)
		return;

	if (_app->bagHasClickedObject()) {
		_app->cursorDelete();
		return;
	}

	for (uint32 i = 1; i < 16; i++)
		_app->objectPresentationHide(kObject40202, i);

	_app->objectPresentationShow(kObject40202, target + 1);

	if (target >= 7) {
		_app->soundPlay(target + 40500);

		Common::String str = _app->varGetString(40902);
		while (str.size() > 7)
			str.deleteLastChar();

		_app->varSetString(40902, Common::String::format("%s%1d", str.c_str(), target - 7));

		if (_app->varGetString(40902) == "01276534"
		 || _app->varGetString(40902) == "01476534"
		 || _app->varGetString(40902) == "01276532"
		 || _app->varGetString(40902) == "01476532") {
			_app->varSetFloat(90006, 100.0f);
			_app->soundPlay(40603);
		}
	} else {
		_app->soundPlay(target + 40500);
	}
}

void EventButtonRing::onButtonDownZoneN2(ObjectId id, uint32 target, Id, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObject70101:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			return;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->objectSetAccessibilityOff(kObject70101, 1, 2);
			_app->objectSetAccessibilityOff(kObject70101, 1, 2);
			_app->objectPresentationShow(kObject70101, 4);
			_app->objectPresentationHide(kObject70100);
			break;

		case 1:
			_app->rotationSetRolTo(70100, 270.4f, 10.4f, 85.7f);
			_app->puzzleSetActive(kPuzzle70100);
			_app->objectPresentationHide(kObject70102);
			_app->objectSetAccessibilityOff(kObject70101, 1, 2);
			_app->objectSetAccessibilityOff(kObject70102, 1, 2);
			_app->objectSetAccessibilityOff(kObject70100);
			_app->objectSetAccessibilityOff(kObject70103);
			_app->objectPresentationShow(kObject70101, 5);
			_app->objectPresentationShow(kObject70100, 1);
			_app->objectPresentationShow(kObject70101, 2);
			break;
		}
		break;

	case kObject70102:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			return;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->objectSetAccessibilityOff(kObject70102, 1, 2);
			_app->objectSetAccessibilityOff(kObject70105);
			_app->objectPresentationShow(kObject70102, 3);
			_app->objectPresentationHide(kObject70100);
			break;

		case 1:
			_app->rotationSetRolTo(70100, 270.4f, 10.4f, 85.7f);
			_app->puzzleSetActive(kPuzzle70100);
			_app->objectPresentationHide(kObject70102);
			_app->objectSetAccessibilityOff(kObject70101, 1, 2);
			_app->objectSetAccessibilityOff(kObject70102, 1, 2);
			_app->objectSetAccessibilityOff(kObject70100);
			_app->objectSetAccessibilityOff(kObject70103);
			_app->objectPresentationShow(kObject70102, 0);
			_app->objectPresentationShow(kObject70100, 2);
			_app->objectPresentationShow(kObject70102, 2);
			break;
		}
		break;

	case kObject70404:
		if (target == 2) {
			_app->objectSetAccessibilityOff(kObject70404, 1, 2);
			_app->objectPresentationShow(kObject70404, _app->varGetByte(70001) ? 5 : 3);
			_app->objectPresentationHide(kObject70404, 0);
			_app->objectPresentationHide(kObject70404, 1);

		}
		break;
	}
}

void EventButtonRing::onButtonUpZoneSY(ObjectId id, uint32 target, Id, uint32, const Common::Point &) {
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

		_event->_prefsVolume = _app->getPreferenceHandler()->getVolume();
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

		_app->objectPresentationSetImageCoordinatesOnPuzzle(kObjectPreferencesSliderVolume, 0, 0, Common::Point((int16)(5 * _event->_prefsVolume + 84), 155));
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
		_app->getPreferenceHandler()->save(_event->_prefsVolume, _prefsVolumeDialog, _presentationIndexSY != 0 ? 2 : 0, _prefsSubtitles);
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

void EventButtonRing::onButtonUpZoneNI(ObjectId id, uint32 target, Id puzzleRotationId, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObject10100:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (target == 1) {
			_app->rotationSetRolTo(10101, 270.4f, 10.4f, 85.7f);
			_app->puzzleSetActive(kPuzzle10100);
			_app->objectSetAccessibilityOn(kObject10101, 0, 0);
			_app->objectSetAccessibilityOn(kObject10102, 0, 0);
			_app->objectSetAccessibilityOn(kObject10101, 2, 2);
			_app->objectSetAccessibilityOn(kObject10102, 2, 2);
			_app->objectSetAccessibilityOn(kObject10100);
			g_system->warpMouse(505, 205);
		}
		break;

	case kObject10104:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectTile) {
				if (!_app->varGetByte(10102)) {
					_app->soundPlay(10106);
					_app->varSetByte(10102, 1);
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
					_app->objectPresentationShow(kObject10100, 1);
					_app->bagRemove(kObjectTile);
					_app->objectSetAccessibilityOff(kObject10104);
					_app->objectSetAccessibilityOn(kObject10109);
					_app->objectSetAccessibilityOn(kObject10110);
				}
			}

			_app->cursorDelete();
		}
		break;

	case kObject10105:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectBrutality) {
				if (!_app->varGetByte(10103)) {
					_app->varSetByte(10103, 1);
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 3.0f);
					_app->playMovie("1510");
					_app->objectPresentationShow(kObject10100, 2);
					_app->soundPlay(10104);
					_app->objectPresentationShow(kObject10102, 2);
					_app->objectSetAccessibilityOff(kObject10100);
					_app->objectSetAccessibilityOff(kObject10103);
					_app->objectSetAccessibilityOff(kObject10101);
					_app->objectSetAccessibilityOff(kObject10102);
					_app->objectSetAccessibilityOff(kObject10105, 1, 1);
				}
			}
			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(10103) == 1)
			_app->puzzleSetActive(kPuzzle10102);
		break;

	case kObjectGlug:
		if (_app->bagHasClickedObject()) {
			switch (target) {
			default:
				break;

			case 1:
				if (_app->bagGetClickedObject() == kObjectGlug) {
					_app->playMovie("1513");
					_app->bagRemove(kObjectGlug);
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
					_app->puzzleSetActive(kPuzzle10002);
					_app->varSetByte(10000, 1);
					_app->objectPresentationShow(kObjectGlug);
					_app->objectPresentationPauseAnimationFrame(kObjectGlug, 0, 1, 1000, 0);
					_app->soundPlay(10800, kSoundLoop);
					_app->cursorDelete();
				}
				break;

			case 2:
				if (_app->bagGetClickedObject() == kObjectMinerals) {
					if (!_app->bagHas(kObjectGlug)) {
						_app->soundStop(10800, 1024);
						_app->playMovie("1514");
						_app->rotationSetMovabilityOff(10003, 0, 0);
						_app->rotationSetMovabilityOff(10201, 0, 0);
						_app->rotationSetMovabilityOff(10401, 0, 0);
						_app->rotationSetMovabilityOn(_app->varGetDword(10000), 0, 0);

						if (!_app->soundIsPlaying(10901)) {
							_app->soundPlay(10900);
							_app->soundPlay(10901, kSoundLoop);
						}

						if (_app->varGetDword((int)10000) == 10401) {
							_app->objectPresentationHide(kObject10003, 3);
							_app->objectPresentationHide(kObject10003, 4);
							_app->objectPresentationHide(kObject10003, 5);
							_app->objectPresentationHide(kObject10003, 6);
							_app->objectPresentationHide(kObject10003, 7);
							_app->objectPresentationShow(kObject10003, 8);
							_app->objectPresentationShow(kObject10003, 9);
							_app->objectPresentationShow(kObject10003, 10);
							_app->objectPresentationShow(kObject10003, 11);
							_app->objectPresentationShow(kObject10003, 12);
						} else {
							_app->objectPresentationHide(kObject10003, 8);
							_app->objectPresentationHide(kObject10003, 9);
							_app->objectPresentationHide(kObject10003, 10);
							_app->objectPresentationHide(kObject10003, 11);
							_app->objectPresentationHide(kObject10003, 12);
							_app->objectPresentationShow(kObject10003, 3);
							_app->objectPresentationShow(kObject10003, 4);
							_app->objectPresentationShow(kObject10003, 5);
							_app->objectPresentationShow(kObject10003, 6);
							_app->objectPresentationShow(kObject10003, 7);
						}
					}
				}
				break;
			}

			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 3:
			_app->varSetDword(10000, puzzleRotationId);
			_app->varSetDword(10001, (uint32)_app->rotationGetAlp(puzzleRotationId));
			_app->puzzleSetActive(kPuzzle10000);

			waitTicks(200);

			if (_app->varGetByte(10000)) {
				_app->puzzleSetActive(kPuzzle10002);
				_app->soundPlay(10800, kSoundLoop);
				_app->soundPlay(10803);
			} else {
				_app->puzzleSetActive(kPuzzle10001);
				_app->soundPlay(10803);
			}
			break;

		case 4:
			_app->puzzleSetActive(kPuzzle10000);

			waitTicks(200);

			_app->soundPlay(10804);
			_app->rotationSetAlp(_app->varGetDword(10000), _app->varGetDword(10001));
			_app->rotationSetBet(_app->varGetDword(10000), 60.0f);
			_app->rotationSetRan(_app->varGetDword(10000), 85.7f);
			_app->rotationSetActive(_app->varGetDword(10000));
			break;
		}
		break;

	case kObject10108:
		if (_app->bagHasClickedObject())
			_app->cursorDelete();
		break;

	case kObject10110:
		if (_app->bagHasClickedObject())
			_app->cursorDelete();
		else
			_app->soundPlay(target + 10032);
		break;

	case kObject10109:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (!target) {
			_app->soundPlay(10106);
			_app->objectPresentationHide(kObject10109, 0);

			handleEvents();

			if (_app->varGetByte(10108) != 5
			 || _app->varGetByte(10109) != 2
			 || _app->varGetByte(10110) != 3
			 || _app->varGetByte(10111) != 4
			 || _app->varGetByte(10112) != 1) {
				_app->varSetByte(10108, 0);
				_app->varSetByte(10109, 0);
				_app->varSetByte(10110, 0);
				_app->varSetByte(10111, 0);
				_app->varSetByte(10112, 0);
				_app->varSetByte(10113, 0);
			} else {
				_app->objectSetAccessibilityOff(kObject10109);
				_app->objectSetAccessibilityOff(kObject10110);
				_app->puzzleAddBackgroundImage(kPuzzle10100, "NIS01N01P02.0001.bmp", Common::Point(0, 16), true);
				_app->varSetByte(10113, 9);
				_app->objectPresentationHide(kObject10101, 0);
				_app->objectPresentationShow(kObject10101, 5);
				_app->objectPresentationShow(kObject10101, 6);
				_app->objectPresentationHide(kObject10109);
			}
		}
		break;

	case kObject10200:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->objectSetAccessibilityOff(kObject10200);
		_app->soundPlay(10022);
		break;

	case kObject10300:
		if (!_app->bagHasClickedObject())
			break;

		if (_app->bagGetClickedObject() == kObjectBrutality) {
			_app->playMovie("1512");

			if (_app->varGetByte(10301) == 1) {
				_app->varSetByte(10301, 2);
				_app->puzzleSetActive(kPuzzle10390);
				_app->soundPlay(10014);
				_app->cursorDelete();
				break;
			}

			_app->puzzleSetActive(kPuzzle10390);
			_app->soundPlay(rnd(3) + 10015);
		}

		_app->cursorDelete();
		break;

	case kObjectMagicFrog:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->varSetByte(10301, 0);
		_app->puzzleSetActive(kPuzzle10392);
		_app->soundPlay(10010);
		break;

	case kObjectTile:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->puzzleSetActive(kPuzzle10392);
		_app->soundPlay(10012);
		break;

	case kObjectLogeTear:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->varSetByte(10301, 0);
		_app->puzzleSetActive(kPuzzle10392);
		_app->soundPlay(10005);
		break;

	case kObject10440:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(10106) != 1
		 || _app->varGetByte(10431) != 1) {
			if (_app->varGetByte(10106)) {
				_app->playMovie("1516");

				if (_app->varGetByte(10106) == 1)
					if (!_app->soundIsPlaying(10409))
						_app->soundPlay(10409, kSoundLoop);

				_app->rotationSetAlp(10415, 270.0f);
				_app->rotationSetBet(10415, 0.3f);
				_app->rotationSetRan(10415, 85.7f);
				_app->rotationSetActive(10415);
			} else {
				_app->playMovie("1515");
				_app->rotationSetAlp(10405, 270.0f);
				_app->rotationSetBet(10405, 0.3f);
				_app->rotationSetRan(10405, 85.7f);
				_app->rotationSetActive(10405);
			}
		} else {
			_app->soundPlay(10414);
		}
		break;

	case kObject10430:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectLogeTear) {
				if (target == 1) {
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 3.0f);
					_app->objectPresentationShow(kObject10430, 6);
					_app->objectPresentationShow(kObject10430, 1);
					_app->bagRemove(kObjectLogeTear);
					_app->varSetByte(10430, 1);
				}
			}

			_app->cursorDelete();
			_event->sub_445A10();
			break;
		}

		_app->varSetByte(10431, 0);

		switch (target) {
		default:
			_event->sub_445A10();
			break;

		case 0:
			_app->objectPresentationHide(kObject10430);
			_app->puzzleSetMovabilityOff(10411);
			_app->objectSetAccessibilityOff(kObject10430, 0, 0);

			if (_app->varGetByte(10430)) {
				_app->objectPresentationShow(kObject10430, 4);
				_app->objectPresentationShow(kObject10430, 1);
			} else {
				_app->objectPresentationShow(kObject10430, 2);
			}

			_app->objectPresentationShow(kObject10430, 0);

			_event->sub_445A10();
			break;

		case 1:
			if (_app->varGetByte(10430) == 1) {
				_app->objectPresentationHide(kObject10430, 1);
				_app->objectPresentationHide(kObject10430, 6);
				_app->bagAdd(kObjectLogeTear);
				_app->varSetFloat(90005, _app->varGetFloat(90005) - 3.0f);
				_app->varSetByte(10430, 0);
			}

			_event->sub_445A10();
			break;
		}
		break;

	case kObject10450:
		if (_app->bagHasClickedObject()) {
			if (_app->varGetByte(10106) == 1) {
				if (_app->bagGetClickedObject() == kObjectDivingHelmet2) {
					_app->playMovie("1517");
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 8.0f);
					_app->rotationSetAlp(10406, 270.0f);
					_app->rotationSetBet(10406, 0.3f);
					_app->rotationSetRan(10406, 85.7f);
					_app->rotationSetActive(10406);
					_app->objectSetAccessibilityOff(kObject10450);
					_app->cursorDelete();
					break;
				}

				_app->playMovie("1518");
				_app->exitToMenu(kMenuAction2);
				break;
			}
			break;
		}

		if (_app->varGetByte(10106) == 1) {
			_app->playMovie("1518");
			_app->exitToMenu(kMenuAction2);
		} else {
			_app->playMovie("1519");
			_app->exitToMenu(kMenuAction1);
		}
		break;

	case kObject10460:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->timerStopAll();
		_app->soundStopAll(1024);
		_app->playMovie("1520");
		_app->varSetFloat(90005, _app->varGetFloat(90005) + 3.0f);
		_event->onSwitchZone(kZoneRH, 0);
		break;

	case kObjectAntiGCells:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->playMovie("1521");
		_app->varSetByte(10501, 1);
		_app->bagAdd(kObjectAntiGCells);
		break;

	case kObjectDivingHelmet:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (!_app->varGetByte(10500)) {
			_app->objectPresentationShow(kObjectDivingHelmet);
			_app->varSetByte(10500, 1);
			_app->objectSetAccessibilityOff(kObjectDivingHelmet);
			_app->bagAdd(kObjectDivingHelmet);
			_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);

			if (_app->bagHas(kObjectMagicFrog)) {
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
				_app->bagRemove(kObjectMagicFrog);
				_app->bagRemove(kObjectDivingHelmet);
				_app->bagAdd(kObjectDivingHelmet2);
				_app->playMovie("1522");
			}
		}
		break;

	case kObject10600: {
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		int16 val1 = _app->varGetWord(10600) + 36;
		if (val1 > 47)
			val1 -= 48;

		int16 val2 = _app->varGetWord(10602) + 24;
		if (val2 > 47)
			val2 -= 48;

		uint32 index = 0;
		if (_app->varGetWord(10600) == 12)
			index = 1;
		if (!_app->varGetWord(10601))
			++index;
		if (_app->varGetWord(10602) == 24)
			++index;

		while (index < 3) {
			if (!_app->soundIsPlaying(10401))
				_app->soundPlay(10401);

			if (_app->varGetWord(10600) != 12) {
				_app->varSetWord(10600, (val1 <= 36 ? -1 : 1) + _app->varGetWord(10600));

				if (_app->varGetWord(10600) > 47)
					_app->varSetWord(10600, 0);

				if (_app->varGetWord(10600) < 0)
					_app->varSetWord(10600, 47);

				if (_app->varGetWord(10600) == 12)
					++index;

				_app->objectPresentationHideAndRemove(kObject10601);
				_app->objectPresentationShow(kObject10601, (uint16)_app->varGetWord(10600));
			}

			if (_app->varGetWord(10601)) {
				_app->varSetWord(10601, (_app->varGetWord(10601) <= 24 ? -1 : 1) + _app->varGetWord(10601));

				if (_app->varGetWord(10601) > 47)
					_app->varSetWord(10601, 0);

				if (_app->varGetWord(10601) < 0)
					_app->varSetWord(10601, 47);

				if (!_app->varGetWord(10601))
					++index;

				_app->objectPresentationHideAndRemove(kObject10602);
				_app->objectPresentationShow(kObject10602, (uint16)_app->varGetWord(10601));
			}

			if (_app->varGetWord(10602) != 24) {
				_app->varSetWord(10602, (val2 <= 24 ? -1 : 1) + _app->varGetWord(10602));

				if (_app->varGetWord(10602) > 47)
					_app->varSetWord(10602, 0);

				if (_app->varGetWord(10602) < 0)
					_app->varSetWord(10602, 47);

				if (_app->varGetWord(10602) == 24)
					++index;

				_app->objectPresentationHideAndRemove(kObject10603);
				_app->objectPresentationShow(kObject10603, (uint16)_app->varGetWord(10602));
			}

			handleEvents();
		}

		_app->rotationSetActive(10601);
		}
		break;
	}
}

void EventButtonRing::onButtonUpZoneRH(ObjectId id, uint32 target, Id, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObjectDivingHelmet2:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectDivingHelmet2) {
				_app->playMovie("1668");
				_app->soundPlay(23010, kSoundLoop);
				_app->rotationSetAlp(20701, 0.0);
				_app->rotationSetActive(20701);
			}

			_app->cursorDelete();
		} else {
			_app->playMovie("1669");
			_app->exitToMenu(kMenuAction3);
		}
		break;

	case kObject20000:
	case kObject20001:
	case kObject20002:
	case kObject20003:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (target) {
			_app->puzzleSetActive(kPuzzle20000);
			_app->objectPresentationShow(kObject21001, 0);
			_app->soundPlay(_app->varGetByte(21001) + 20031);
		} else {
			_app->puzzleSetActive(kPuzzle20000);
			_app->objectPresentationShow(kObject21001, 0);
			_app->soundPlay(_app->varGetByte(21001) + 20021);
		}
		break;

	case kObjectKeyIndifference:
	case kObjectKeyMistrust:
	case kObjectKeySelfishness:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->bagAdd(id);
		_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
		_app->objectPresentationHide(id);
		_app->objectSetAccessibilityOff(id);
		_app->rotationSetMovabilityOff(10 * (_app->varGetByte(21001) + 2001), 0, 0);
		_app->rotationSetMovabilityOn(10 * (_app->varGetByte(21001) + 2001), 1, 1);
		_app->rotationSetActive(10 * (_app->varGetByte(21001) + 2001));
		if (_app->bagHas(kObjectKeyIndifference)
		 && _app->bagHas(kObjectKeyMistrust)
		 && _app->bagHas(kObjectKeySelfishness)
		 && _app->bagHas(kObjectKeyDisgust)) {
			_app->rotationSetMovabilityOn(20101, 1, 1);
			_app->soundPlay(23009);
		}
		break;

	case kObjectKeyDisgust:
		if (!_app->bagHasClickedObject()) {
			if (!target) {
				_app->bagAdd(kObjectKeyDisgust);
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
				_app->objectPresentationHide(kObjectKeyDisgust, 0);
				_app->objectSetAccessibilityOff(kObjectKeyDisgust, 0, 1);

				if (_app->bagHas(kObjectKeyIndifference)
				 && _app->bagHas(kObjectKeyMistrust)
				 && _app->bagHas(kObjectKeySelfishness)
				 && _app->bagHas(kObjectKeyDisgust)) {
					_app->rotationSetMovabilityOn(20101, 1, 1);
					_app->soundPlay(23009);
				}
			}
			break;
		}

		_app->cursorDelete();
		_app->setField74(false);
		break;

	case kObjectRedfish:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectRedfish) {
				_app->bagRemove(kObjectRedfish);
				_app->varSetByte(20200, 1);
				_app->objectSetAccessibilityOn(kObjectDolphin, 5, 5);
				_app->objectSetAccessibilityOff(kObjectDolphin, 3, 4);
				_app->objectPresentationShow(kObjectRedfish, 0);
			}

			_app->setField74(false);
			_app->cursorDelete();
			break;
		}

		if (target == 1 && _app->varGetByte(20200) == 1) {
			if (_app->varGetByte(20202)) {
				_app->setField74(false);
				_app->playMovie("1694");
			} else {
				_app->playMovie("1693");
				_app->bagAdd(kObjectRedfish);
				_app->varSetByte(20200, 0);
				_app->objectSetAccessibilityOff(kObjectDolphin, 5, 5);
				_app->objectSetAccessibilityOn(kObjectDolphin, 3, 4);
				_app->objectPresentationHide(kObjectRedfish, 0);
			}
		}
		break;

	case kObjectDolphin:
		if (_app->bagHasClickedObject()) {
			switch (target) {
			default:
				break;

			case 0:
				if (_app->bagGetClickedObject() == kObjectMedallion) {
					_app->varSetByte(20202, 0);
					_app->bagRemove(kObjectMedallion);
					_app->objectPresentationHide(kObjectDolphin);
					_app->objectPresentationShow(kObjectDolphin, 0);
				}
				break;

			case 2:
				if (_app->bagGetClickedObject() == kObjectRedfish) {
					_app->bagRemove(kObjectRedfish);
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 3.0f);
					_app->objectSetAccessibilityOff(kObjectRedfish);
					_app->varSetByte(20202, 2);
					_app->playMovie("1688");
					_app->objectPresentationHide(kObjectDolphin);
					_app->puzzleSetActive(kPuzzle20203);
					_app->soundPlay(20201);
				}
				break;
			}

			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (_app->varGetByte(20201) == 1) {
				if (!_app->varGetByte(20202)) {
					_app->varSetByte(20202, 1);
					_app->bagAdd(kObjectMedallion);
					_app->objectPresentationHide(kObjectDolphin);
					_app->objectPresentationShow(kObjectDolphin, 1);
				}
			}
			break;

		case 1:
			if (_app->varGetByte(20201)) {
				if (_app->varGetByte(20202)) {
					_app->playMovie("1690");
					_app->objectPresentationHide(kObjectDolphin);
					_app->objectPresentationShow(kObjectDolphin, 1);
				} else {
					_app->playMovie("1689");
					_app->objectPresentationHide(kObjectDolphin);
					_app->objectPresentationShow(kObjectDolphin, 0);
				}
			} else {
				_app->varGetByte(20202);
			}

			_app->puzzleSetActive(kPuzzle20202);
			break;

		case 9:
			_app->playMovie(_app->varGetByte(20202) ? "1692" : "1691");
			_app->rotationSetActive(20201);
			break;
		}
		break;

	case kObject20204:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectKeyDisgust) {
				_app->bagRemove(kObjectKeyDisgust);
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
				_app->objectSetAccessibilityOff(kObject20204);
				_app->puzzleSetActive(kPuzzle20204);
				_app->soundPlay(20202);
			}

			_app->cursorDelete();
		}
		break;

	case kObject20301:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectMedallion) {
				if (target == 2) {
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
					_app->bagRemove(kObjectMedallion);
					_app->playMovie("1676");
					_app->objectSetAccessibilityOff(kObject20301, 1, 1);
					_app->rotationSetActive(20301);
				}
			}

			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 1:
			_app->rotationSetRolTo(20301, 42.0f, 23.0f, 85.7f);
			_app->playMovie("1679");
			_app->puzzleSetActive(kPuzzle20301);
			_app->soundPlay(20301);
			break;

		case 9:
			_app->playMovie("1680");
			_app->rotationSetActive(20301);
			break;
		}

		if (_app->varGetByte(20202) == 2) {
			if (!_app->bagHas(kObjectMedallion)) {
				_app->playMovie("1678");
				_app->rotationSetAlp(20302, 10.0);
				_app->rotationSetActive(20302);
				break;
			}
		}

		_app->playMovie("1677");
		_app->exitToMenu(kMenuAction1);
		break;

	case kObject20302:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (_app->bagHas(kObjectMedallion)) {
				_app->playMovie("1682");
				_app->rotationSetAlp(20303, 10.0);
				_app->rotationSetActive(20303);
				break;
			}

			_app->playMovie("1681");
			_app->exitToMenu(kMenuAction1);
			break;

		case 1:
			_app->rotationSetRolTo(20302, 320.0f, 26.0f, 85.7f);
			_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
			_app->objectSetAccessibilityOff(kObject20302, 1, 2);
			_app->playMovie("1683", 0.0);
			_app->puzzleSetActive(kPuzzle20302);
			_app->soundPlay(20302);
			break;
		}
		break;

	case kObject20303:
		if (_app->bagHasClickedObject()) {
			switch (target) {
			default:
				break;

			case 1:
				if (_app->bagGetClickedObject() == kObjectDivingHelmet2) {
					_app->objectPresentationShow(kObject20303, 0);
					_app->playMovie("1684");
					_app->soundPlay(23010, kSoundLoop);
					_app->puzzleSetActive(kPuzzle20303);
				}
				break;

			case 2:
				if (_app->bagGetClickedObject() == kObjectMedallion) {
					_app->varSetByte(20301, 1);
					_app->objectSetAccessibilityOn(kObject20303, 0, 0);
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
					_app->objectSetAccessibilityOff(kObject20303, 1, 2);
					_app->objectPresentationHide(kObject20303, 0);
					_app->soundPlay(20303);
				}
				break;
			}

			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (_app->varGetByte(20301) == 1) {
				_app->playMovie("1685");
				_app->rotationSetActive(20304);
			}
			break;

		case 1:
			_app->rotationSetRolTo(20303, 140.0f, 26.0f, 85.7f);
			_app->playMovie("1686");
			_app->exitToMenu(kMenuAction3);
			break;

		case 9:
			_app->soundStop(23010, 1024);
			_app->playMovie("1687");
			_app->rotationSetActive(20303);
			break;
		}
		break;

	case kObject20304:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectKeyMistrust) {
				_app->bagRemove(kObjectKeyMistrust);
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
				_app->objectSetAccessibilityOff(kObject20304);
				_app->puzzleSetActive(kPuzzle20204);
				_app->soundPlay(20304);
			}

			_app->cursorDelete();
		}
		break;

	case kObject20401:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectBrutality) {
				_app->playMovie("1675");
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 3.0f);
				_app->puzzleSetActive(kPuzzle20401);
			}

			_app->cursorDelete();
		}
		break;

	case kObjectAntiGCells2:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectMedallion) {
				_app->bagRemove(kObjectMedallion);
				_app->objectPresentationHide(kObject20402, 1);
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
				_app->soundPlay(20401);
			}

			_app->cursorDelete();
		}
		break;

	case kObject20404:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectKeySelfishness) {
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
				_app->bagRemove(kObjectKeySelfishness);
				_app->objectSetAccessibilityOff(kObject20404);
				_app->puzzleSetActive(kPuzzle20204);
				_app->soundPlay(20402);
			}

			_app->cursorDelete();
		}
		break;

	case kObject20501:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (_app->varGetByte(20500)) {
				_app->puzzleSetActive(kPuzzle20502);
			} else {
				_app->varSetByte(20500, 1);
				_app->soundStop(23011, 1024);
				_app->puzzleSetActive(kPuzzle20501);
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
				_app->soundPlay(20501);
			}
			break;

		case 1:
			if (_app->varGetByte(20500) >= 2) {
				if (_app->varGetByte(20500) == 3) {
					_app->playMovie("1671");
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
					_app->rotationSetAlp(20503, 0.0);
					_app->rotationSetActive(20503);
				}
			} else {
				_app->playMovie("1670");
				_app->exitToMenu(kMenuAction4);
			}
			break;

		case 2:
			if (_app->varGetByte(20500) == 2) {
				_app->varSetByte(20500, 3);
				_app->soundStop(23011, 1024);
				_app->playMovie("1672");
				_app->objectSetAccessibilityOn(kObject20501, 1, 1);
				_app->objectSetAccessibilityOff(kObject20501, 2, 2);
				_app->puzzleSetActive(kPuzzle20503);
				_app->soundPlay(20503);
			}
			break;

		case 3:
			_app->playMovie("1673");
			_app->varSetByte(20500, 2);
			_app->objectPresentationHide(kObject20501, 0);
			_app->objectSetAccessibilityOff(kObject20501, 0, 1);
			_app->objectSetAccessibilityOn(kObject20501, 2, 2);
			break;

		case 9:
			_app->rotationSetActive(20501);
			break;
		}
		break;

	case kObject20502:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectKeyIndifference) {
				_app->bagRemove(kObjectKeyIndifference);
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
				_app->varSetByte(20500, 4);
				_app->objectSetAccessibilityOff(kObject20502);
				_app->puzzleSetActive(kPuzzle20204);
				_app->soundPlay(20504);
			}

			_app->cursorDelete();
		}
		break;

	case kObject20700:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(20500) == 4) {
			_app->varSetFloat(90005, _app->varGetFloat(90005) + 1.0f);
			_app->soundStopAll(1024);
			_app->playMovie("1666");
			_app->bagRemove(kObjectAntiGCells2);
			_event->onSwitchZone(kZoneNI, 3);
		}
		else {
			_app->playMovie("1667");
			_app->exitToMenu(kMenuAction2);
		}
		break;
	}
}

void EventButtonRing::onButtonUpZoneFO(ObjectId id, uint32 target, Id, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObject30001:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectKey) {
				if (!target) {
					_app->objectPresentationShow(kObject30001, 0);
					_app->objectSetAccessibilityOff(kObject30001, 0, 0);
					_app->objectSetAccessibilityOn(kObject30001, 1, 3);

					if (_app->varGetByte(30040) != 1
					 || _app->varGetByte(30041) != 1
					 || _app->varGetByte(30042) != 1) {
						if (_app->varGetByte(30040) == 1) {
							_app->objectPresentationShow(kObject30001, 1);
							_app->objectSetAccessibilityOff(kObject30001, 1, 1);
						}

						if (_app->varGetByte(30041) == 1) {
							_app->objectPresentationShow(kObject30001, 2);
							_app->objectSetAccessibilityOff(kObject30001, 2, 2);
						}

						if (_app->varGetByte(30042) == 1) {
							_app->objectPresentationShow(kObject30001, 3);
							_app->objectSetAccessibilityOff(kObject30001, 3, 3);
						}
					} else {
						_app->objectPresentationShow(kObject30001, 4);
						_app->objectSetAccessibilityOn(kObject30001, 4, 4);
					}
				}
			}

			if (_app->bagGetClickedObject() != kObjectIngot
			 && _app->bagGetClickedObject() != kObjectIngot2
			 && _app->bagGetClickedObject() != kObjectIngot3) {
				_app->cursorDelete();
				break;
			}

			switch (target) {
			default:
				_app->cursorDelete();
				break;

			case 1:
				_app->varSetByte(30040, 1);
				_app->objectPresentationShow(kObject30001, 1);
				_app->objectSetAccessibilityOff(kObject30001, 1, 1);
				_app->bagRemove(_app->bagGetClickedObject());

				if (_app->varGetByte(30040) != 1
				 || _app->varGetByte(30041) != 1
				 || _app->varGetByte(30042) != 1) {
					_app->cursorDelete();
					break;
				}
				break;

			case 2:
				_app->varSetByte(30041, 1);
				_app->objectPresentationShow(kObject30001, 2);
				_app->objectSetAccessibilityOff(kObject30001, 2, 2);
				_app->bagRemove(_app->bagGetClickedObject());

				if (_app->varGetByte(30040) != 1
				 || _app->varGetByte(30041) != 1
				 || _app->varGetByte(30042) != 1) {
					_app->cursorDelete();
					break;
				}
				break;

			case 3:
				_app->varSetByte(30042, 1);
				_app->objectPresentationShow(kObject30001, 3);
				_app->objectSetAccessibilityOff(kObject30001, 3, 3);
				_app->bagRemove(_app->bagGetClickedObject());

				if (_app->varGetByte(30040) != 1
				 || _app->varGetByte(30041) != 1
				 || _app->varGetByte(30042) != 1) {
					_app->cursorDelete();
					break;
				}
				break;
			}

			_app->playMovie("1167");
			_app->objectPresentationHide(kObject30001, 1);
			_app->objectPresentationHide(kObject30001, 2);
			_app->objectPresentationHide(kObject30001, 3);
			_app->objectPresentationShow(kObject30001, 4);
			_app->objectSetAccessibilityOn(kObject30001, 4, 4);
			_app->objectSetAccessibilityOn(kObjectSleepingPotion, 1, 1);
			_app->cursorDelete();

		} else {
			if (target == 4) {
				_app->bagAdd(kObjectWolfBadge);
				_app->objectPresentationHide(kObject30001, 4);
				_app->objectSetAccessibilityOff(kObject30001, 4, 4);
				_app->varSetFloat(90007, _app->varGetFloat(90007) + 4.0f);
				_app->varSetByte(30043, 1);
			}
		}
		break;

	case kObject30002: // byte: 30009 / sound: 30110
	case kObject30003: // byte: 30010 / sound: 30111
	case kObject30004: // byte: 30011 / sound: 30112
	case kObject30005: // byte: 30012 / sound: 30113
	case kObject30006: // byte: 30013 / sound: 30114
	case kObject30007: // byte: 30014 / sound: 30115
	case kObject30008: // byte: 30015 / sound: 30116
		if (_app->bagHasClickedObject()) {
			switch (_app->bagGetClickedObject()) {
			default:
				break;

			case kObjectPatience:
				if (_app->varGetByte(id + 7)) {
					_app->playMovie("1169");
				} else {
					_app->varSetByte(id + 7, 1);
					_app->bagRemove(kObjectPatience);
					_app->playMovie("1168");
				}
				break;

			case kObjectMovementAndIntuition:
				if (_app->varGetByte(id + 7)) {
					_app->playMovie("1169");
				} else {
					_app->varSetByte(id + 7, 2);
					_app->bagRemove(kObjectMovementAndIntuition);
					_app->playMovie("1168");
				}
				break;

			case kObjectLove:
				if (_app->varGetByte(id + 7)) {
					_app->playMovie("1169");
				} else {
					_app->varSetByte(id + 7, 3);
					_app->bagRemove(kObjectLove);
					_app->playMovie("1168");
				}
				break;

			case kObjectImagination:
				if (_app->varGetByte(id + 7)) {
					_app->playMovie("1169");
				} else {
					_app->varSetByte(id + 7, 4);
					_app->bagRemove(kObjectImagination);
					_app->playMovie("1168");
				}
				break;

			case kObjectDestruction:
				if (_app->varGetByte(id + 7)) {
					_app->playMovie("1169");
				} else {
					_app->varSetByte(id + 7, 5);
					_app->bagRemove(kObjectDestruction);
					_app->playMovie("1168");
				}
				break;

			case kObjectJudgementAndDirection:
				if (_app->varGetByte(id + 7)) {
					_app->playMovie("1169");
				} else {
					_app->varSetByte(id + 7, 6);
					_app->bagRemove(kObjectJudgementAndDirection);
					_app->playMovie("1168");
				}
				break;

			case kObjectWill:
				if (_app->varGetByte(id + 7)) {
					_app->playMovie("1169");
				} else {
					_app->varSetByte(id + 7, 7);
					_app->bagRemove(kObjectWill);
					_app->playMovie("1168");
				}
				break;
			}

			if (_app->varGetByte(30009) == 1
			 && _app->varGetByte(30010) == 2
			 && _app->varGetByte(30011) == 3
			 && _app->varGetByte(30012) == 4
			 && _app->varGetByte(30013) == 5
			 && _app->varGetByte(30014) == 6
			 && _app->varGetByte(30015) == 7) {
				_app->objectSetAccessibilityOff(kObject30002);
				_app->objectSetAccessibilityOff(kObject30003);
				_app->objectSetAccessibilityOff(kObject30004);
				_app->objectSetAccessibilityOff(kObject30005);
				_app->objectSetAccessibilityOff(kObject30006);
				_app->objectSetAccessibilityOff(kObject30007);
				_app->objectSetAccessibilityOff(kObject30008);
				_app->playMovie("1170");
				_app->varSetFloat(90007, _app->varGetFloat(90007) + 5.5f);
				_app->rotationSetActive(30302);
				_app->timerStop(kTimer1);
				_app->timerStop(kTimer3);
			}

			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(id + 7)) {
			_app->bagAdd((ObjectId)(_app->varGetByte(id + 7) + 30008));
			_app->playMovie("1171");
			_app->soundPlay(id + 108);
			_app->varSetByte(id + 7, 0);
		}
		break;

	case kObject30027:
		if (_app->bagHasClickedObject())
			break;

		if (!_app->varGetByte(30016)) {
			_app->playMovie("1172");
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 3.3f);

			for (uint32 i = 0; i < 27; i++)
				_app->soundSetVolume(30300 + i, 90);

			_app->varSetByte(30017, 0);
			_app->varSetByte(30035, 1);
			_app->rotationSetMovabilityOn(30402, 2, 2);
			_app->objectPresentationShow(kObject30050, 0);
			_app->objectPresentationShow(kObject30050, 2);
			_app->rotationSetActive(30501);
			_app->rotationSetAlp(30501, 88.0);
			_app->rotationSetBet(30501, 13.0);
			_app->objectSetAccessibilityOff(kObject30002);
			_app->objectSetAccessibilityOff(kObject30003);
			_app->objectSetAccessibilityOff(kObject30004);
			_app->objectSetAccessibilityOff(kObject30005);
			_app->objectSetAccessibilityOff(kObject30006);
			_app->objectSetAccessibilityOff(kObject30007);
			_app->objectSetAccessibilityOff(kObject30008);
		}
		break;

	case kObjectBerries:
		if (_app->bagHasClickedObject()) {
			if (target == 5) {
				switch (_app->bagGetClickedObject()) {
				default:
					break;

				case kObjectSleepingBerries:
					if (!_app->varGetByte(30060)) {
						_app->bagRemove(kObjectSleepingBerries);
						_app->varSetByte(30063, 1);
						_app->objectPresentationShow(kObjectBerries, 4);
					}
					break;

				case kObjectBerries:
					if (!_app->varGetByte(30063)) {
						_app->bagRemove(kObjectBerries);
						_app->varSetByte(30060, 1);
						_app->objectPresentationShow(kObjectBerries, 4);
					}
					break;
				}
			}

			_app->cursorDelete();
			break;
		}


		switch (target) {
		default:
			break;

		case 0:
		case 4:
			if (_app->varGetByte(30017) == 1) {
				_app->bagAdd(kObjectSleepingBerries);

				if (!_app->varGetByte(30201)) {
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 3.3f);
					_app->varSetByte(30201, 1);
				}

				_app->objectPresentationHide(kObjectBerries, 3);
				_app->varSetByte(30075, 1);
				_app->objectSetAccessibilityOff(kObjectBerries, 0, 0);
				_app->bagAdd(kObjectIngot2);
				_app->varSetFloat(90007, _app->varGetFloat(90007) + 3.3f);
				_app->objectPresentationHide(kObjectBerries, 0);
				_app->objectPresentationHide(kObjectBerries, 2);
				_app->varSetByte(30076, 1);
			}
			break;

		case 1:
		case 2:
		case 3:
			_app->bagAdd(kObjectBerries);
			break;

		case 5:
			if (!_app->varGetByte(30060) && !_app->varGetByte(30063))
				_app->playMovie("1173");

			if (_app->varGetByte(30060) != 1 || _app->varGetByte(30061)) {
				if (_app->varGetByte(30060) == 1 && _app->varGetByte(30061) == 1) {
					_app->bagAdd(kObjectBerriesJuice);
					_app->objectPresentationHide(kObjectBerries, 5);
					_app->varSetByte(30060, 0);
					_app->varSetByte(30061, 0);
				}
			} else {
				_app->playMovie("1174");
				_app->varSetByte(30061, 1);
				_app->objectPresentationHide(kObjectBerries, 4);
				_app->objectPresentationShow(kObjectBerries, 5);
			}

			if (_app->varGetByte(30063) != 1 || _app->varGetByte(30064)) {
				if (_app->varGetByte(30063) == 1 && _app->varGetByte(30064) == 1) {
					_app->bagAdd(kObjectSleepingPotion2);

					if (!_app->varGetByte(30203)) {
						_app->varSetFloat(90007, _app->varGetFloat(90007) + 6.6f);
						_app->varSetByte(30203, 1);
					}

					_app->objectPresentationHide(kObjectBerries, 5);
					_app->varSetByte(30063, 0);
					_app->varSetByte(30064, 0);
				}
			} else {
				_app->playMovie("1175");
				_app->varSetByte(30064, 1);
				_app->objectPresentationHide(kObjectBerries, 4);
				_app->objectPresentationShow(kObjectBerries, 5);
			}
			break;
		}
		break;

	case kObjectSleepingPotion:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectHare && target == 0) {
				_app->bagRemove(kObjectHare);
				_app->objectSetAccessibilityOff(kObjectSleepingPotion, 0, 0);
				_app->playMovie("1176");
				_app->timerStop(kTimer2);
				_app->soundStop(30501, 1024);

				if (!_app->varGetByte(30077)) {
					_app->playMovie("1177");
					_app->bagAdd(kObjectIngot3);
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 5.5f);
					_app->varSetByte(30077, 1);
				}
			}

			_app->cursorDelete();

			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->playMovie("1178");
			break;

		case 1:
			if (_app->varGetByte(30074)) {
				_app->playMovie("1179");
				_app->rotationSetActive(30101);
				_app->rotationSetAlp(30101, 130.0);
				_app->rotationSetBet(30101, 20.0);
			} else {
				if (_app->varGetByte(30056))
					_app->puzzleSetActive(kPuzzle35100);
				else
					_app->playMovie("1178");
			}
			break;
		}
		break;

	case kObjectBow:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->soundPlay(30511);
			_app->objectPresentationShow(kObjectBow);
			_app->objectSetAccessibilityOff(kObjectBow, 0, 0);
			if (_app->varGetByte(30050) == 1)
				_app->objectPresentationHide(kObjectBow, 1);
			if (_app->varGetByte(30051) == 1)
				_app->objectPresentationHide(kObjectBow, 2);
			if (_app->varGetByte(30052) == 1)
				_app->objectPresentationHide(kObjectBow, 3);
			if (_app->varGetByte(30053) == 1)
				_app->objectPresentationHide(kObjectBow, 4);
			if (_app->varGetByte(30054) == 1)
				_app->objectPresentationHide(kObjectBow, 5);
			if (_app->varGetByte(30055) == 1)
				_app->objectPresentationHide(kObjectBow, 6);
			break;

		case 1:
		case 2:
		case 3:
		case 4:
			_app->soundPlay(30512);
			_app->objectPresentationHide(kObjectBow, target);
			_app->objectSetAccessibilityOff(kObjectBow, target, target);
			_app->varSetByte(target + 30049, 1);

			_app->bagAdd(kObjectHare);
			if (!_app->varGetByte(30204)) {
				_app->varSetFloat(90007, _app->varGetFloat(90007) + 1.1f);
				_app->varSetByte(30204, 1);
			}
			break;

		case 5:
		case 6:
			_app->soundPlay(30512);
			_app->objectPresentationHide(kObjectBow, target);
			_app->objectSetAccessibilityOff(kObjectBow, target, target);
			_app->varSetByte(target + 30049, 1);

			_app->bagAdd(kObjectBow);
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 1.1f);
			_app->objectPresentationHide(kObjectBow, 5);
			_app->objectPresentationHide(kObjectBow, 6);
			_app->objectSetAccessibilityOff(kObjectBow, 5, 6);
			_app->varSetByte(30054, 1);
			_app->varSetByte(30055, 1);
			break;
		}
		break;

	case kObject30028:
		if (_app->bagHasClickedObject()) {
			if (target == 1) {
				switch (_app->bagGetClickedObject()) {
				default:
					break;

				case kObjectLogeTear2:
					if (_app->varGetByte(30019)) {
						_app->playMovie("1181");
						_app->bagRemove(kObjectLogeTear2);
						_app->varSetByte(30021, 1);
						if (!_app->varGetByte(30020)) {
							_app->objectPresentationHide(kObject30028, 0);
							_app->objectPresentationShow(kObject30028, 1);
						}
					} else {
						_app->playMovie("1180");
					}
					break;

				case kObjectMould:
					if (_app->varGetByte(30019) == 1) {
						if (_app->varGetByte(30021) == 1) {
							_app->objectPresentationHide(kObject30028, 0);
							_app->objectPresentationShow(kObject30028, 2);
							_app->varSetByte(30020, 1);
							_app->bagRemove(kObjectMould);
						}
					}
					break;

				case kObjectMetals:
					if (_app->varGetByte(30020) == 1) {
						_app->bagRemove(kObjectMetals);
						_app->bagAdd(kObjectMeltedGold);
						_app->bagAdd(kObjectMeltedSilver);
						_app->bagAdd(kObjectMeltedCopper);
						_app->bagAdd(kObjectMeltedLead);
						_app->bagAdd(kObjectMeltedSteel);
						_app->bagAdd(kObjectMeltedTin);
						_app->bagAdd(kObjectQuicksilver);
						_app->bagAdd(kObjectLogeTear2);

						if (!_app->varGetByte(30206)) {
							_app->varSetFloat(90007, _app->varGetFloat(90007) + 4.4f);
							_app->varSetByte(30206, 1);
						}

						_app->bagAdd(kObjectMould);
						_app->playMovie("1182");
						_app->objectPresentationHide(kObject30028);
						_app->objectPresentationHide(kObject30028, 3);
						_app->varSetByte(30019, 0);
						_app->varSetByte(30020, 0);
						_app->varSetByte(30021, 0);
						_app->objectSetAccessibilityOn(kObject30028, 0, 0);
					}
					break;
				}
			}

			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (!_app->varGetByte(30020)) {
				_app->playMovie("1183");
				_app->objectPresentationShow(kObject30028, 0);
				_app->objectPresentationShow(kObject30028, 3);
				_app->varSetByte(30019, 1);
				_app->objectSetAccessibilityOff(kObject30028, 0, 0);
			}
			break;

		case 1:
			if (_app->varGetByte(30021) == 1) {
				if (!_app->varGetByte(30020)) {
					_app->playMovie("1184");
					_app->objectPresentationHide(kObject30028);
					_app->objectPresentationShow(kObject30028, 0);
					_app->varSetByte(30021, 0);
					_app->bagAdd(kObjectLogeTear2);
				}
			}

			if (_app->varGetByte(30021) == 1) {
				if (_app->varGetByte(30020) == 1) {
					_app->objectPresentationHide(kObject30028, 2);
					_app->objectPresentationShow(kObject30028, 1);
					_app->varSetByte(30020, 0);
					_app->bagAdd(kObjectMould);
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 1.1f);
				}
			}
			break;
		}
		break;

	case kObject30040:
		if (_app->bagHasClickedObject())
			break;

		switch (target) {
		default:
			break;

		case 0:
			if (_app->varGetByte(30022))
				break;

			if (_app->varGetByte(30024)) {
				_app->objectPresentationShow(kObject30040, 0);
			} else {
				_app->objectPresentationShow(kObject30040, 1);
				_app->objectSetAccessibilityOn(kObject30040, 4, 4);
			}

			_app->varSetByte(30022, 1);
			_app->objectSetAccessibilityOff(kObject30040, 0, 0);
			_app->objectSetAccessibilityOff(kObject30040, 1, 1);
			_app->objectSetAccessibilityOn(kObject30040, 2, 2);
			_app->soundPlay(30508);
			break;

		case 1:
			if (_app->varGetByte(30023))
				break;

			_app->objectPresentationShow(kObject30040, 2);
			_app->varSetByte(30023, 1);
			_app->objectSetAccessibilityOff(kObject30040, 0, 0);
			_app->objectSetAccessibilityOff(kObject30040, 1, 1);
			_app->objectSetAccessibilityOn(kObject30040, 3, 3);
			_app->objectSetAccessibilityOn(kObject30040, 5, 5);
			_app->soundPlay(30508);
			break;

		case 2:
			if (_app->varGetByte(30022) != 1)
				break;

			_app->objectPresentationHide(kObject30040, 0);
			_app->objectPresentationHide(kObject30040, 1);
			_app->varSetByte(30022, 0);
			_app->objectSetAccessibilityOff(kObject30040, 2, 2);
			_app->objectSetAccessibilityOff(kObject30040, 4, 4);
			_app->objectSetAccessibilityOn(kObject30040, 1, 1);
			_app->objectSetAccessibilityOn(kObject30040, 0, 0);
			_app->soundPlay(30508);
			break;

		case 3:
			if (_app->varGetByte(30023) != 1)
				break;

			_app->objectPresentationHide(kObject30040, 2);
			_app->varSetByte(30023, 0);
			_app->objectSetAccessibilityOff(kObject30040, 3, 3);
			_app->objectSetAccessibilityOff(kObject30040, 5, 5);
			_app->objectSetAccessibilityOn(kObject30040, 0, 0);
			_app->objectSetAccessibilityOn(kObject30040, 1, 1);
			_app->soundPlay(30508);
			break;

		case 4:
			_app->bagAdd(kObjectMould);
			_app->objectPresentationShow(kObject30040, 0);
			_app->objectPresentationHide(kObject30040, 1);
			_app->objectSetAccessibilityOff(kObject30040, 4, 4);
			_app->varSetByte(30024, 1);
			break;

		case 5:
			_app->bagAdd(kObjectMetals);

			if (!_app->varGetByte(30205)) {
				_app->varSetFloat(90007, _app->varGetFloat(90007) + 1.1f);
				_app->varSetByte(30205, 1);
			}

			_app->objectSetAccessibilityOff(kObject30040, 5, 5);
			_app->objectSetAccessibilityOff(kObject30040, 3, 3);
			_app->objectPresentationShow(kObject30040, 4);
			break;
		}
		break;

	case kObject30042:
		if (!_app->bagHasClickedObject()) {
			if (!target) {
				if (_app->varGetByte(30019)) {
					_app->objectPresentationShow(kObject30042, 0);
					_app->objectPresentationHide(kObject30042, 8);
					_app->objectPresentationHide(kObject30028, 0);
					_app->objectPresentationHide(kObject30028, 3);
					_app->varSetByte(30019, 0);
					_app->objectSetAccessibilityOn(kObject30042, 1, 7);
					_app->objectSetAccessibilityOff(kObject30042, 0, 0);
					_app->objectSetAccessibilityOn(kObject30028, 0, 0);
					_app->playMovie("1187");
				} else {
					_app->objectPresentationShow(kObject30042, 8);
					_app->objectPresentationHide(kObject30042, 0);
					_app->objectSetAccessibilityOff(kObject30042, 0, 0);
					_app->soundPlay(30508);
				}
			}
			break;
		}

		if (target >= 1 && target <= 7) {
			_app->soundPlay(30509);

			switch (_app->bagGetClickedObject()) {
			default:
				break;

			case kObjectMeltedGold:
				_app->varSetByte(30025, (byte)target);
				_app->bagRemove(kObjectMeltedGold);
				_app->objectPresentationShow(kObject30042, target);
				_app->objectSetAccessibilityOff(kObject30042, target, target);
				break;

			case  kObjectMeltedSilver:
				_app->varSetByte(30026, (byte)target);
				_app->bagRemove(kObjectMeltedSilver);
				_app->objectPresentationShow(kObject30042, target);
				_app->objectSetAccessibilityOff(kObject30042, target, target);
				break;

			case kObjectMeltedCopper:
				_app->varSetByte(30027, (byte)target);
				_app->bagRemove(kObjectMeltedCopper);
				_app->objectPresentationShow(kObject30042, target);
				_app->objectSetAccessibilityOff(kObject30042, target, target);
				break;

			case kObjectMeltedLead:
				_app->varSetByte(30028, (byte)target);
				_app->bagRemove(kObjectMeltedLead);
				_app->objectPresentationShow(kObject30042, target);
				_app->objectSetAccessibilityOff(kObject30042, target, target);
				break;

			case kObjectMeltedSteel:
				_app->varSetByte(30029, (byte)target);
				_app->bagRemove(kObjectMeltedSteel);
				_app->objectPresentationShow(kObject30042, target);
				_app->objectSetAccessibilityOff(kObject30042, target, target);
				break;

			case kObjectMeltedTin:
				_app->varSetByte(30030, (byte)target);
				_app->bagRemove(kObjectMeltedTin);
				_app->objectPresentationShow(kObject30042, target);
				_app->objectSetAccessibilityOff(kObject30042, target, target);
				break;

			case kObjectQuicksilver:
				_app->varSetByte(30031, (byte)target);
				_app->bagRemove(kObjectQuicksilver);
				_app->objectPresentationShow(kObject30042, target);
				_app->objectSetAccessibilityOff(kObject30042, target, target);
				break;
			}
		}

		if (_app->varGetByte(30025) != 1
		 || _app->varGetByte(30026) != 2
		 || _app->varGetByte(30027) != 3
		 || _app->varGetByte(30028) != 4
		 || _app->varGetByte(30029) != 5
		 || _app->varGetByte(30030) != 6
		 || _app->varGetByte(30031) != 7) {
			if (_app->varGetByte(30025)
			 && _app->varGetByte(30026)
			 && _app->varGetByte(30027)
			 && _app->varGetByte(30028)
			 && _app->varGetByte(30029)
			 && _app->varGetByte(30030)
			 && _app->varGetByte(30031)) {
				_app->objectPresentationHide(kObject30042);
				_app->playMovie("1186");
				_app->rotationSetActive(30601);
				_app->objectSetAccessibilityOn(kObject30042, 0, 0);
				_app->varSetByte(30025, 0);
				_app->varSetByte(30026, 0);
				_app->varSetByte(30027, 0);
				_app->varSetByte(30028, 0);
				_app->varSetByte(30029, 0);
				_app->varSetByte(30030, 0);
				_app->varSetByte(30031, 0);
			}
		} else {
			_app->objectPresentationHide(kObject30042);
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 9.9f);
			_app->varSetByte(30032, 1);
			_app->playMovie("1185");
			_app->bagAdd(kObjectGolem);
			_app->rotationSetActive(30601);
			_app->objectSetAccessibilityOff(kObject30042);
		}

		_app->cursorDelete();
		break;

	case kObject30044:
		if (!_app->bagHasClickedObject())
			break;

		if (_app->bagGetClickedObject() == kObjectGolem) {
			switch (target) {
			default:
				break;

			case 0:
				_app->objectPresentationShow(kObject30044);
				_app->objectSetAccessibilityOff(kObject30044, 0, 0);
				_app->objectSetAccessibilityOn(kObject30044, 1, 1);
				_app->bagRemove(kObjectGolem);
				_app->playMovie("1188");
				break;

			case 1:
				_app->objectPresentationShow(kObject30044);
				break;
			}
		}

		if (_app->bagGetClickedObject() == kObjectSleepingPotion2) {
			if (target == 1) {
				_app->objectPresentationHide(kObject30044);
				_app->objectSetAccessibilityOff(kObject30044, 1, 1);
				_app->bagAdd(kObjectGolem);
				_app->bagAdd(kObjectIngot);
				_app->playMovie("1189");
				_app->rotationSetMovabilityOff(30601, 9, 9);
				_app->objectSetAccessibilityOff(kObject30044);
				_app->varSetFloat(90007, _app->varGetFloat(90007) + 3.3f);
			}
		}

		_app->cursorDelete();
		break;

	case kObject30045:
		if (_app->bagHasClickedObject())
			break;

		switch (target) {
		default:
			break;

		case 0:
			_app->objectPresentationHide(kObject30045);
			_app->objectPresentationShow(kObject30045, 0);
			_app->soundPlay(30150);
			break;

		case 1:
			_app->objectPresentationHide(kObject30045);
			_app->objectPresentationShow(kObject30045, 1);
			_app->soundPlay(30151);
			break;

		case 2:
			_app->objectPresentationHide(kObject30045);
			_app->objectPresentationShow(kObject30045, 2);
			_app->soundPlay(30154);
			break;

		case 3:
			_app->objectPresentationHide(kObject30045);
			_app->objectPresentationShow(kObject30045, 3);
			_app->soundPlay(30152);
			break;

		case 4:
			_app->objectPresentationHide(kObject30045);
			_app->objectPresentationShow(kObject30045, 4);
			_app->soundPlay(30155);
			break;

		case 5:
			_app->objectPresentationHide(kObject30045);
			_app->objectPresentationShow(kObject30045, 5);
			_app->soundPlay(30156);
			break;

		case 6:
			_app->objectPresentationHide(kObject30045);
			_app->objectPresentationShow(kObject30045, 6);
			_app->soundPlay(30153);
			break;

		case 7:
			_app->soundPlay(30201);
			_app->varSetByte(30047, 1);
			_app->rotationSetRolTo(30602, 175.0f, -23.0f, 85.3f);
			break;
		}
		break;

	case kObjectFishingRod:
		if (_app->bagHasClickedObject()) {
			if (target == 3) {
				if (_app->bagGetClickedObject() == kObjectFishingRodWithWorms) {
					if (_app->varGetByte(30034)) {
						_app->bagAdd(kObjectKey);
						_app->varSetFloat(90007, _app->varGetFloat(90007) + 5.5f);
						_app->playMovie("1191");
					} else {
						_app->bagAdd(kObjectFish);

						if (!_app->varGetByte(30207)) {
							_app->varSetFloat(90007, _app->varGetFloat(90007) + 2.2f);
							_app->varSetByte(30207, 1);
						}

						_app->playMovie("1190");
					}

					_app->bagAdd(kObjectFishingRod);
					_app->bagRemove(kObjectFishingRodWithWorms);
					_app->varSetByte(30038, 0);
				}
				if (_app->bagGetClickedObject() == kObjectFishingRod)
					_app->playMovie("1192");
			}

			_app->cursorDelete();

			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->soundPlay(30508);

			if (_app->varGetByte(30033)) {
				_app->objectPresentationShow(kObjectFishingRod, 0);
				_app->objectPresentationHide(kObjectFishingRod, 1);
				_app->objectSetAccessibilityOff(kObjectFishingRod, 0, 1);
				_app->objectSetAccessibilityOn(kObjectFishingRod, 2, 2);
			} else {
				_app->objectPresentationHide(kObjectFishingRod, 0);
				_app->objectPresentationShow(kObjectFishingRod, 1);
				_app->objectSetAccessibilityOff(kObjectFishingRod, 0, 0);
				_app->objectSetAccessibilityOn(kObjectFishingRod, 1, 2);
			}

			_app->varSetByte(30070, 1);
			break;

		case 1:
			if (_app->varGetByte(30038)) {
				_app->objectPresentationHide(kObjectFishingRod, 1);
				_app->objectPresentationShow(kObjectFishingRod, 0);
				_app->objectSetAccessibilityOff(kObjectFishingRod, 0, 1);
				_app->bagAdd(kObjectFishingRodWithWorms);

				if (!_app->varGetByte(30208)) {
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 6.6f);
					_app->varSetByte(30208, 1);
				}

				_app->bagRemove(kObjectWorms);
				_app->varSetByte(30033, 1);
			} else {
				_app->objectPresentationHide(kObjectFishingRod, 1);
				_app->objectPresentationShow(kObjectFishingRod, 0);
				_app->objectSetAccessibilityOff(kObjectFishingRod, 0, 1);
				_app->bagAdd(kObjectFishingRod);
				_app->varSetByte(30033, 1);
			}
			break;

		case 2:
			_app->soundPlay(30511);

			if (_app->varGetByte(30070)) {
				_app->objectPresentationHide(kObjectFishingRod);
				_app->objectSetAccessibilityOn(kObjectFishingRod, 0, 0);
				_app->objectSetAccessibilityOff(kObjectFishingRod, 1, 2);
				_app->varSetByte(30070, 0);
			} else {
				if (_app->varGetByte(30033)) {
					_app->objectPresentationShow(kObjectFishingRod, 0);
					_app->varSetByte(30070, 0);
				} else {
					_app->objectPresentationShow(kObjectFishingRod, 1);
					_app->varSetByte(30070, 0);
				}
			}
			break;
		}
		break;

	case kObjectWorms:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() != kObjectGolem || _app->varGetByte(30038)) {
				_app->cursorDelete();
				break;
			}

			if (_app->varGetByte(30033)) {
				_app->bagAdd(kObjectFishingRodWithWorms);

				if (!_app->varGetByte(30208)) {
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 6.6f);
					_app->varSetByte(30208, 1);
				}

				_app->playMovie("1193");
				_app->bagRemove(kObjectFishingRod);
				_app->varSetByte(30038, 1);

				if (_app->varGetByte(30076)) {
					_app->cursorDelete();
					break;
				}
			} else {
				_app->bagAdd(kObjectWorms);

				if (!_app->varGetByte(30209)) {
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 2.2f);
					_app->varSetByte(30209, 1);
				}

				_app->playMovie("1193");
				_app->varSetByte(30038, 1);

				if (_app->varGetByte(30076)) {
					_app->cursorDelete();
					break;
				}
			}

			_app->bagAdd(kObjectIngot2);
			_app->bagAdd(kObjectSleepingBerries);
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 3.3f);
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 3.3f);
			_app->varSetByte(30076, 1);
			_app->varSetByte(30039, 1);
			_app->playMovie("1194");
			_app->objectPresentationHide(kObjectWorms, 0);
			_app->objectPresentationHide(kObjectWorms, 1);
			_app->objectPresentationHide(kObjectBerries, 0);
			_app->objectSetAccessibilityOff(kObjectWorms);
			_app->cursorDelete();
		}
		break;

	case kObject30050:
		if (!_app->bagHasClickedObject()) {
			if (!_app->varGetByte(30035)) {
				_app->objectPresentationShow(kObject30050, 1);
				_app->soundPlay(30514);
			}

			if (_app->varGetByte(30035) == 1) {
				if (_app->varGetByte(30037) == 1) {
					_app->playMovie("1195");
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 4.4f);
					_app->objectPresentationShow(kObject30051, 2);
					_app->objectPresentationHide(kObject30050);
					_app->varSetByte(30035, 0);
					_app->varSetByte(30036, 1);
					_app->varSetByte(30037, 0);
					_app->rotationSetMovabilityOff(30402, 2, 2);
					_app->rotationSetMovabilityOff(30011, 1, 1);
				}
			}
		}
		break;

	case kObject30051:
		if (_app->bagHasClickedObject()) {
			switch (target) {
			default:
				_app->cursorDelete();
				break;

			case 0:
				if (_app->bagGetClickedObject() == kObjectBurningArrow) {
					_app->bagRemove(kObjectBurningArrow);
					_app->bagAdd(kObjectBow);
					_app->varSetByte(30034, 1);
					_app->playMovie("1196", target);
					_app->varSetFloat(90007, _app->varGetFloat(90007) + 5.5f);
					_app->objectPresentationHide(kObject30051, 2);
					_app->objectSetAccessibilityOff(kObject30051, target, target);
					_app->objectSetAccessibilityOff(kObject30051, 3, 3);
				}

				if (_app->bagGetClickedObject() != kObjectBow) {
					_app->cursorDelete();
					break;
				}

				_app->playMovie("1197");
				_app->cursorDelete();
				break;

			case 1:
			case 2:
				if (_app->bagGetClickedObject() == kObjectBow) {
					_app->bagAdd(kObjectBurningArrow);

					if (!_app->varGetByte(30210)) {
						_app->varSetFloat(90007, _app->varGetFloat(90007) + 4.4f);
						_app->varSetByte(30210, 1);
					}

					_app->bagRemove(kObjectBow);
				}
				break;

			case 3:
				if (_app->bagGetClickedObject() == kObjectBurningArrow) {
					_app->playMovie("1198");
					_app->bagRemove(kObjectBurningArrow);
					_app->bagAdd(kObjectBow);
				}

				if (_app->bagGetClickedObject() != kObjectBow) {
					_app->cursorDelete();
					break;
				}

				_app->playMovie("1197");
				_app->cursorDelete();
				break;
			}
		}
		break;

	case kObject30102:
		if (_app->bagHasClickedObject()) {
			if (target == 1) {
				if (_app->bagGetClickedObject() == kObjectWolfBadge) {
					if (_app->varGetByte(30072) == 1) {
						_app->puzzleSetActive(kPuzzle35104);
						_app->soundPlay(30105);
					}
				}
			}

			_app->cursorDelete();
			break;
		}

		if (target == 1) {
			if (_app->varGetByte(30072)) {
				if (!_app->varGetByte(30078)) {
					_app->puzzleSetActive(kPuzzle35103);
					_app->soundPlay(30118);
				}
			} else {
				_app->rotationSetRolTo(30101, 130.0f, 20.0f, 85.3f);
				_app->puzzleSetActive(kPuzzle35104);
				_app->soundPlay(30102);
			}
		}
		break;

	case kObject30109:
		if (_app->bagHasClickedObject())
			_app->cursorDelete();
		else
			_app->puzzleSetActive(kPuzzle35109);
		break;

	case kObject30108:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() != kObjectSleepingPotion2) {
				_app->cursorDelete();
				break;
			}

			_app->playMovie("1199");
			_app->objectPresentationHide(kObject30110, 1);
			_app->objectPresentationShow(kObject30110, 2);
			_app->rotationSetActive(30101);
			_app->rotationSetAlp(30101, 130.0f);
			_app->rotationSetBet(30101, 20.0f);
			_app->objectSetAccessibilityOff(kObject30102, 0, 0);
			_app->varSetByte(30072, 1);
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 6.6f);
			_app->cursorDelete();
		}
		break;

	case kObjectNotung:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectWolfBadge
			 || _app->bagGetClickedObject() == kObjectWolfBrooch) {
				if (_app->bagGetClickedObject() == kObjectWolfBrooch) {
					_app->bagRemove(kObjectWolfBrooch);
					_app->varSetByte(30067, 1);
				} else {
					_app->bagRemove(kObjectWolfBadge);
					_app->varSetByte(30066, 1);
				}

				if (_app->varGetByte(30066) == 1
				 && _app->varGetByte(30067) == 1) {
					_app->objectPresentationShow(kObjectNotung, 3);
					_app->playMovie("1200");
					_app->varSetFloat(90007, 100.0f);
					_app->rotationSetAlp(30001, 180.0);
					_app->rotationSetActive(30001);
					_app->soundSetMultiplier(kSoundTypeAmbientMusic, 0);
					_app->timerStopAll();
					_app->soundPlay(30007);
					_app->objectSetAccessibilityOff(kObject30001);
					_app->objectSetAccessibilityOff(kObject30002);
					_app->objectSetAccessibilityOff(kObject30003);
					_app->objectSetAccessibilityOff(kObject30004);
					_app->objectSetAccessibilityOff(kObject30005);
					_app->objectSetAccessibilityOff(kObject30006);
					_app->objectSetAccessibilityOff(kObject30007);
					_app->objectSetAccessibilityOff(kObject30008);
					_app->objectSetAccessibilityOff(kObjectPatience);
					_app->objectSetAccessibilityOff(kObject30016);
					_app->objectSetAccessibilityOff(kObject30027);
					_app->objectSetAccessibilityOff(kObjectBerries);
					_app->objectSetAccessibilityOff(kObjectSleepingBerries);
					_app->objectSetAccessibilityOff(kObjectWolfInstinct);
					_app->objectSetAccessibilityOff(kObjectHare);
					_app->objectSetAccessibilityOff(kObjectIngot);
					_app->objectSetAccessibilityOff(kObjectIngot2);
					_app->objectSetAccessibilityOff(kObjectIngot3);
					_app->objectSetAccessibilityOff(kObjectSleepingPotion);
					_app->objectSetAccessibilityOff(kObjectBow);
					_app->objectSetAccessibilityOff(kObject30028);
					_app->objectSetAccessibilityOff(kObjectMould);
					_app->objectSetAccessibilityOff(kObjectLogeTear2);
					_app->objectSetAccessibilityOff(kObjectBerriesJuice);
					_app->objectSetAccessibilityOff(kObjectSleepingPotion2);
					_app->objectSetAccessibilityOff(kObjectMeltedGold);
					_app->objectSetAccessibilityOff(kObjectMeltedSilver);
					_app->objectSetAccessibilityOff(kObjectMeltedCopper);
					_app->objectSetAccessibilityOff(kObjectMeltedLead);
					_app->objectSetAccessibilityOff(kObjectMeltedSteel);
					_app->objectSetAccessibilityOff(kObjectMeltedTin);
					_app->objectSetAccessibilityOff(kObjectQuicksilver);
					_app->objectSetAccessibilityOff(kObject30040);
					_app->objectSetAccessibilityOff(kObjectMetals);
					_app->objectSetAccessibilityOff(kObject30042);
					_app->objectSetAccessibilityOff(kObjectGolem);
					_app->objectSetAccessibilityOff(kObject30044);
					_app->objectSetAccessibilityOff(kObject30045);
					_app->objectSetAccessibilityOff(kObjectFishingRod);
					_app->objectSetAccessibilityOff(kObjectFishingRodWithWorms);
					_app->objectSetAccessibilityOff(kObjectFish);
					_app->objectSetAccessibilityOff(kObjectWorms);
					_app->objectSetAccessibilityOff(kObject30050);
					_app->objectSetAccessibilityOff(kObject30051);
					_app->objectSetAccessibilityOff(kObject30052);
					_app->objectSetAccessibilityOff(kObjectBurningArrow);
					_app->objectSetAccessibilityOff(kObjectKey);
					_app->objectSetAccessibilityOff(kObjectWolfBadge);
					_app->objectSetAccessibilityOff(kObjectWolfBrooch);
					_app->objectSetAccessibilityOff(kObject30110);
					_app->objectSetAccessibilityOff(kObjectNotung);
					_app->objectSetAccessibilityOff(kObject30059);
					_app->rotationSetMovabilityOff(30001);
					_app->rotationSetMovabilityOff(30002);
					_app->rotationSetMovabilityOff(30003);
					_app->rotationSetMovabilityOff(30004);
					_app->rotationSetMovabilityOff(30005);
					_app->rotationSetMovabilityOff(30006);
					_app->rotationSetMovabilityOff(30008);
					_app->rotationSetMovabilityOff(30009);
					_app->rotationSetMovabilityOff(30010);
					_app->rotationSetMovabilityOff(30011);
					_app->rotationSetMovabilityOff(30012);
					_app->rotationSetMovabilityOff(30101);
					_app->rotationSetMovabilityOff(30301);
					_app->rotationSetMovabilityOff(30302);
					_app->rotationSetMovabilityOff(30303);
					_app->rotationSetMovabilityOff(30401);
					_app->rotationSetMovabilityOff(30402);
					_app->rotationSetMovabilityOff(30501);
					_app->rotationSetMovabilityOff(30601);
					_app->rotationSetMovabilityOff(30602);
					_app->rotationSetMovabilityOff(30701);
					_app->rotationSetMovabilityOff(30702);
					_app->rotationSetMovabilityOff(30703);
					_app->rotationSetMovabilityOff(30704);
					_app->rotationSetMovabilityOff(30801);
					_app->objectPresentationShow(kObject6, 0);
					_app->objectPresentationShow(kObject6, 5);
					_app->soundPlay(30120);
					_app->cursorDelete();
					break;
				}

				_app->playMovie("1201");
				_app->objectPresentationShow(kObjectNotung, 2);
			}
			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(30066) == 1
		 || _app->varGetByte(30067) == 1) {
			_app->playMovie("1202");
			_app->objectPresentationHide(kObjectNotung, 2);

			if (_app->varGetByte(30066) == 1) {
				_app->bagAdd(kObjectWolfBadge);
				_app->varSetByte(30066, 0);
			} else {
				_app->bagAdd(kObjectWolfBrooch);
				_app->varSetByte(30067, 0);
			}
		}
		break;

	case kObject30059:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->playMovie(target ? "1204" : "1203");

		if (!_app->varGetByte(30211)) {
			_app->varSetFloat(90007, _app->varGetFloat(90007) + 1.1f);
			_app->varSetByte(30211, 1);
		}
		break;

	case kObject30100:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->soundSetVolume(30506, rnd(10) + 91);
			_app->puzzleSetMovabilityOff(kPuzzle35100, 0, 0);
			_app->soundPlay(30506);
			_app->objectSetAccessibilityOff(kObject30100, 0, 0);
			break;

		case 1:
			_app->objectSetAccessibilityOff(kObject30100, 1, 1);
			_app->puzzleSetActive(kPuzzle35101);
			_app->soundPlay(30100);
			break;
		}
		break;

	case kObject30200:
		if (_app->bagHasClickedObject())
			_app->cursorDelete();
		else
			_app->soundPlay(target + 30162);
		break;
	}
}

void EventButtonRing::onButtonUpZoneRO(ObjectId id, uint32 target, Id, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObject40010:
		if (!_app->bagHasClickedObject()) {
			if (!target) {
				if (_app->varGetByte(40000)) {
					if (!_app->varGetByte(40701)
					 && !_app->varGetByte(40702)
					 && !_app->varGetByte(40703)) {
						_app->puzzleSetActive(kPuzzle40011);
						_app->objectPresentationShow(kObject40011);
						_app->objectSetAccessibilityOn(kObject40011);
					}

					if (_app->varGetByte(40701) == 1
					 && !_app->varGetByte(40702)
					 && !_app->varGetByte(40703))
						_app->puzzleSetActive(kPuzzle40013);

					if (_app->varGetByte(40701) == 1
					 && _app->varGetByte(40702) == 1
					 && !_app->varGetByte(40703))
						_app->puzzleSetActive(kPuzzle40012);

				} else {
					_app->puzzleSetActive(kPuzzle40010);
				}
			}
			break;
		}

		switch (target) {
		default:
			_app->cursorDelete();
			break;

		case 0:
			if (_app->bagGetClickedObject() != kObject40000 || _app->varGetByte(40000)) {
				_app->cursorDelete();
				break;
			}

			_app->playMovie("1780");
			_app->varSetFloat(90006, 51.8f);
			_app->puzzleSetActive(kPuzzle40011);

			handleEvents();

			if (_app->varGetByte(40701)) {
				handleEvents();

				_app->varSetByte(40000, 1);
				_app->cursorDelete();
				break;
			}

			for (uint32 i = 2000; i >= 1; i--) {
				target = (rnd(4) + 1) + 10 * (rnd(4) + 2);

				if (_app->varGetByte(target + 40501)) {
					uint32 presentationIndex = (uint8)_app->varGetByte(target + 40501) / 10 - 2;
					uint32 imageIndex = (uint8)_app->varGetByte(target + 40501) % 10 - 1;
					Common::Point coords = _app->objectPresentationGetImageCoordinatesOnPuzzle(40011, presentationIndex, imageIndex);

					if (!_app->varGetByte(target + 40491)) {
						_app->varSetByte(target + 40491, _app->varGetByte(target + 40501));
						_app->varSetByte(target + 40501, 0);
						_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x, coords.y - _app->varGetByte(40805)));
						continue;
					}

					if (!_app->varGetByte(target + 40511)) {
						_app->varSetByte(target + 40511, _app->varGetByte(target + 40501));
						_app->varSetByte(target + 40501, 0);
						_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x, coords.y + _app->varGetByte(40805)));
						continue;
					}

					if (!_app->varGetByte(target + 40500)) {
						_app->varSetByte(target + 40500, _app->varGetByte(target + 40501));
						_app->varSetByte(target + 40501, 0);
						_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x - _app->varGetByte(40805), coords.y));
						continue;
					}

					if (!_app->varGetByte(target + 40502)) {
						_app->varSetByte(target + 40502, _app->varGetByte(target + 40501));
						_app->varSetByte(target + 40501, 0);
						_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x + _app->varGetByte(40805), coords.y));
						continue;
					}
				}
			}

			handleEvents();

			_app->varSetByte(40000, 1);
			_app->cursorDelete();
			break;

		case 2:
			if (_app->bagGetClickedObject() == kObjectRing) {
				_app->objectPresentationShow(kObject40010, 0);
				_app->bagRemove(kObjectRing);
				_app->varSetByte(40801, 1);
			}

			if (_app->bagGetClickedObject() == kObjectCrown && _app->varGetByte(40801) == 1) {
				_app->objectPresentationHide(kObject40010, 0);
				_app->playMovie("1781");
				_app->varSetFloat(90006, 78.6f);
				_app->bagRemove(kObjectCrown);
				_app->varSetByte(40703, 1);
				_app->puzzleSetActive(kPuzzle40103);
				_app->soundStop(40002, 1024);
				_app->puzzleSetActive(kPuzzle40101);
				_app->soundPlay(40706);
			}

			_app->cursorDelete();
			break;
		}
		break;

	case kObject40011:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(target + 40501)) {
			uint32 presentationIndex = (uint8)_app->varGetByte(target + 40501) / 10 - 2;
			uint32 imageIndex = (uint8)_app->varGetByte(target + 40501) % 10 - 1;
			Common::Point coords = _app->objectPresentationGetImageCoordinatesOnPuzzle(40011, presentationIndex, imageIndex);

			if (_app->varGetByte(target + 40491)) {
				if (!_app->varGetByte(target + 40511)) {
					_app->varSetByte(target + 40511, _app->varGetByte(target + 40501));
					_app->varSetByte(target + 40501, 0);
					_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x, coords.y + _app->varGetByte(40805)));

					if (target == 12) {
						uint32 counter = 1;
						for (uint32 i = 0; i < (uint8)_app->varGetByte(40522); i++) {
							++counter;

							if (counter >= 5) {
								//counter = 1;

								// Original does some checks on a2, but they seem useless :S

								handleEvents();

								_app->objectPresentationHide(kObject40011);
								_app->objectSetAccessibilityOff(kObject40011);
								_app->objectSetAccessibilityOff(kObject40010, 1, 1);
								_app->objectSetAccessibilityOn(kObject40010, 2, 2);
								_app->varSetByte(40701, 1);
								_app->playMovie("1782");
								_app->varSetFloat(90006, 64.3f);
								_app->puzzleSetActive(kPuzzle40013);
								_app->objectPresentationShow(kObject40101, 0);
								_app->objectPresentationShow(kObject40102, 0);
								_app->objectPresentationShow(kObject40103, 0);
								_app->objectPresentationShow(kObject40104, 0);
								_app->objectPresentationShow(kObject40105, 0);
								_app->timerStart(kTimer0, 50);
								_app->timerStart(kTimer1, 30);
								break;
							}
						}
					}

					_app->soundSetVolume(40103, rnd(20) + 80);
					_app->soundPlay(40103);
					break;
				}

				if (!_app->varGetByte(target + 40500)) {
					_app->varSetByte(target + 40500, _app->varGetByte(target + 40501));
					_app->varSetByte(target + 40501, 0);
					_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x - _app->varGetByte(40805), coords.y));
					_app->soundSetVolume(40103, rnd(20) + 80);
					_app->soundPlay(40103);
					break;
				}

				if (_app->varGetByte(target + 40502)) {
					_app->soundSetVolume(40103, rnd(20) + 80);
					_app->soundPlay(40103);
					break;
				}

				_app->varSetByte(target + 40502, _app->varGetByte(target + 40501));
				_app->varSetByte(target + 40501, 0);
				_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x + _app->varGetByte(40805), coords.y));
				_app->soundSetVolume(40103, rnd(20) + 80);
				_app->soundPlay(40103);
			} else {
				_app->varSetByte(target + 40491, _app->varGetByte(target + 40501));
				_app->varSetByte(target + 40501, 0);
				_app->objectPresentationSetImageCoordinatesOnPuzzle(kObject40011, presentationIndex, imageIndex, Common::Point(coords.x, coords.y - _app->varGetByte(40805)));
				_app->soundSetVolume(40103, rnd(20) + 80);
				_app->soundPlay(40103);
			}
		}
		break;

	case kObject40201:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->objectSetAccessibilityOff(kObject40060);
		_app->puzzleSetMovabilityOff(kPuzzle40060, 0, 0);
		if ((_event->_presentationIndexRO / 10) == (int32)(target + 1)) {
			_app->objectPresentationShow(kObject40201, target);

			Common::String str = _app->varGetString(40901);
			while (str.size() > 6)
				str.deleteLastChar();

			_app->varSetString(40901, Common::String::format("%s%d", str.c_str(), target));
		} else {
			_app->varSetByte(target + 40200, _app->varGetByte(target + 40200) ? 0 : 1);
			_app->objectPresentationShow(kObject40201, target + 7);
			_app->soundSetVolume(40602, rnd(20) + 80);
			_app->soundPlay(40602);
		}
		break;
	}
}

void EventButtonRing::onButtonUpZoneWA(ObjectId id, uint32 target, Id, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObjectFlower:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->bagAdd(kObjectFlower);
		_app->objectPresentationHide(kObjectFlower);
		_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
		_app->objectSetAccessibilityOff(kObjectFlower);
		break;

	case kObjectDragonSword:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->bagAdd(kObjectDragonSword);
		_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
		_app->objectPresentationShow(kObjectTree, 0);
		_app->objectSetAccessibilityOff(kObjectDragonSword);
		break;

	case kObjectApple:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->bagAdd(kObjectApple);
		_app->objectPresentationHide(kObjectTree, 5);
		_app->varSetFloat(90008, _app->varGetFloat(90008) + 3.0f);
		_app->objectSetAccessibilityOff(kObjectApple);
		break;

	case kObject50600:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->soundPlay(50018);
			_app->objectPresentationUnpauseAnimation(kObject50600, 0);
			_app->objectPresentationHide(kObject50600, 6);
			_app->objectPresentationHide(kObject50600, 7);
			_app->objectPresentationShow(kObject50600, 1);
			if (_app->varGetByte(50007)) {
				_app->objectPresentationHide(kObject50600, 2);
				_app->varSetByte(50007, 0);
			} else {
				_app->objectPresentationShow(kObject50600, 2);
				_app->varSetByte(50007, 1);
			}
			break;

		case 1:
			_app->soundPlay(50018);
			_app->objectPresentationUnpauseAnimation(kObject50600, 0);
			_app->objectPresentationHide(kObject50600, 6);
			_app->objectPresentationHide(kObject50600, 7);
			_app->objectPresentationShow(kObject50600, 1);
			if (_app->varGetByte(50008)) {
				_app->objectPresentationHide(kObject50600, 3);
				_app->varSetByte(50008, 0);
			} else {
				_app->objectPresentationShow(kObject50600, 3);
				_app->varSetByte(50008, 1);
			}
			break;

		case 2:
			_app->soundPlay(50018);
			_app->objectPresentationUnpauseAnimation(kObject50600, 0);
			_app->objectPresentationHide(kObject50600, 6);
			_app->objectPresentationHide(kObject50600, 7);
			_app->objectPresentationShow(kObject50600, 1);
			if (_app->varGetByte(50009)) {
				_app->objectPresentationHide(kObject50600, 4);
				_app->varSetByte(50009, 0);
			} else {
				_app->objectPresentationShow(kObject50600, 4);
				_app->varSetByte(50009, 1);
			}
			break;

		case 3:
			_app->soundPlay(50018);
			_app->objectPresentationUnpauseAnimation(kObject50600, 0);
			_app->objectPresentationHide(kObject50600, 6);
			_app->objectPresentationHide(kObject50600, 7);
			_app->objectPresentationShow(kObject50600, 1);
			if (_app->varGetByte(50010)) {
				_app->objectPresentationHide(kObject50600, 5);
				_app->varSetByte(50010, 0);
			} else {
				_app->objectPresentationShow(kObject50600, 5);
				_app->varSetByte(50010, 1);
			}
			break;
		}
		break;

	case kObject50601:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectThread) {
				if (_app->varGetByte(50012) == 6) {
					_app->bagRemove(kObjectThread);
					_app->varSetFloat(90008, 100.0);

					_app->playMovie("1860");
					_app->playMovie("1861");

					switch (_app->getCurrentLanguage()) {
					default:
						_app->playMovieChannel("1862", _app->getLanguageChannel());
						break;

					case kLanguageItalian:
					case kLanguageSpanish:
					case kLanguageDutch:
						_app->playMovieChannel("1863", _app->getLanguageChannel());
						break;

					case kLanguageSwedish:
						_app->playMovieChannel("1864", _app->getLanguageChannel());
						break;
					}

					_app->puzzleSetActive(kPuzzle51001);
					_app->soundPlay(50021);
				}
			}

			_app->cursorDelete();
		}
		break;

	case kObject50700:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->puzzleSetActive(kPuzzle50701);
		_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
		_app->soundPlay(50009);
		break;

	case kObjectTotems:
		if (!_app->bagHasClickedObject()) {
			switch (target) {
			default:
				if (target > 9)
					_app->puzzleSetActive((PuzzleId)(target + 50491));
				break;

			case 0:
				if (_app->varGetWord(50000) % 10 == 1) {
					_app->bagAdd(kObjectFlower);
					_app->objectPresentationHide(kObjectTree, 1);
					_app->varSetWord(50000, _app->varGetWord(50000) - 1);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;

			case 1:
				if (_app->varGetWord(50000) % 100 > 1) {
					_app->bagAdd(kObjectApple);
					_app->objectPresentationHide(kObjectTree, 2);
					_app->varSetWord(50000, _app->varGetWord(50000) - 10);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;

			case 2:
				if (_app->varGetWord(50000) % 1000 > 11) {
					_app->bagAdd(kObjectDeadLeaf);
					_app->objectPresentationHide(kObjectTree, 3);
					_app->varSetWord(50000, _app->varGetWord(50000) - 100);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;

			case 3:
				if (_app->varGetWord(50000) % 10000 > 111) {
					_app->bagAdd(kObjectBark);
					_app->objectPresentationHide(kObjectTree, 4);
					_app->varSetWord(50000, _app->varGetWord(50000) - 1000);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;
			}

			break;
		}

		if (!_app->bagHasClickedObject()) {
			switch (target) {
			default:
				if (target > 9)
					_app->puzzleSetActive((PuzzleId)(target + 50491));
				break;

			case 0:
				if (_app->varGetWord(50000) % 10 == 1) {
					_app->bagAdd(kObjectFlower);
					_app->objectPresentationHide(kObjectTree, 1);
					_app->varSetWord(50000, _app->varGetWord(50000) - 1);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;

			case 1:
				if (_app->varGetWord(50000) % 100 > 1) {
					_app->bagAdd(kObjectApple);
					_app->objectPresentationHide(kObjectTree, 2);
					_app->varSetWord(50000, _app->varGetWord(50000) - 10);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;

			case 2:
				if (_app->varGetWord(50000) % 1000 > 11) {
					_app->bagAdd(kObjectDeadLeaf);
					_app->objectPresentationHide(kObjectTree, 3);
					_app->varSetWord(50000, _app->varGetWord(50000) - 100);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;

			case 3:
				if (_app->varGetWord(50000) % 10000 > 111) {
					_app->bagAdd(kObjectBark);
					_app->objectPresentationHide(kObjectTree, 4);
					_app->varSetWord(50000, _app->varGetWord(50000) - 1000);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;
			}

			break;
		}

		if (_app->bagGetClickedObject() != kObjectFlower || target)	{
			if (_app->bagGetClickedObject() != kObjectApple || target != 1) {
				if (_app->bagGetClickedObject() != kObjectDeadLeaf || target != 2) {
					if (_app->bagGetClickedObject() == kObjectBark && target == 3) {
						_app->bagRemove(_app->bagGetClickedObject());
						_app->objectPresentationShow(kObjectTree, 4);
						_app->varSetWord(50000, _app->varGetWord(50000) + 1000);
						_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
					}
				} else {
					_app->bagRemove(_app->bagGetClickedObject());
					_app->objectPresentationShow(kObjectTree, 3);
					_app->varSetWord(50000, _app->varGetWord(50000) + 100);
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
				}
			}
			else
			{
				_app->bagRemove(_app->bagGetClickedObject());
				_app->objectPresentationShow(kObjectTree, 2);
				_app->varSetWord(50000, _app->varGetWord(50000) + 10);
				_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
			}
		}
		else
		{
			_app->bagRemove(kObjectFlower);
			_app->objectPresentationShow(kObjectTree, 1);
			_app->varSetWord(50000, _app->varGetWord(50000) + 1);
			_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
		}

		if (_app->varGetWord(50000) == 1111) {
			_app->varSetWord(50000, 11111);
			_app->objectPresentationShow(kObjectTree, 6);

			handleEvents();

			_app->playMovie("1851");
			_app->rotationSetActive(50502);
			_app->bagAdd(kObjectThread);
			_app->varSetFloat(90008, _app->varGetFloat(90008) + 3.0f);
			_app->varSetByte(50012, _app->varGetByte(50012) + 1);

			if (_app->varGetByte(50012) == 2) {
				_app->objectPresentationShow(kObject50700, 0);
				_app->objectSetAccessibilityOff(kObject50700);
				_app->objectSetAccessibilityOn(kObject50700, 1, 1);
				_app->rotationSetMovabilityOff(50103, 2, 2);
				_app->rotationSetMovabilityOn(50103, 3, 3);
			}

			if (_app->varGetByte(50012) == 4) {
				_app->objectPresentationShow(kObject50700, 1);
				_app->objectSetAccessibilityOff(kObject50700);
				_app->objectSetAccessibilityOn(kObject50700, 2, 2);
			}
		}
		_app->cursorDelete();
		break;

	case kObjectGolem1:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectGolem1) {
				if (!target) {
					_app->bagRemove(kObjectGolem1);
					_app->objectPresentationShow(kObjectGolem1, 1);
					_app->objectPresentationShow(kObjectBackhead, 0);
					_app->objectPresentationShow(kObjectFronthead, 0);
					_app->objectPresentationShow(kObjectBelly, 0);
					_app->objectPresentationShow(kObjectRightArm, 0);
					_app->objectPresentationShow(kObjectLeftArm, 0);
					_app->objectPresentationShow(kObjectLegs, 0);
					_app->objectPresentationShow(kObjectHeart, 0);
					_app->objectSetAccessibilityOn(kObjectBackhead);
					_app->objectSetAccessibilityOn(kObjectFronthead);
					_app->objectSetAccessibilityOn(kObjectBelly);
					_app->objectSetAccessibilityOn(kObjectRightArm);
					_app->objectSetAccessibilityOn(kObjectLeftArm);
					_app->objectSetAccessibilityOn(kObjectLegs);
					_app->objectSetAccessibilityOn(kObjectHeart, 1, 1);
					_app->objectSetAccessibilityOff(kObjectGolem1, 0, 0);
					_app->puzzleSetMovabilityOff(kPuzzle50400);
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
				}
			}
			_app->cursorDelete();
		}
		break;

	case kObject50105:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(50011) == 1) {
			_app->objectPresentationPauseAnimationFrame(kObject50100, 8, 50, 1000, 2);
			_app->varSetByte(50012, 5);
			_app->varSetFloat(90008, _app->varGetFloat(90008) + 3.0f);
			_app->objectPresentationShow(kObject50100, 8);
			_app->objectPresentationHide(kObject50100, 7);
		}
		break;

	case kObject50100:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(50012) == 4) {
			_app->rotationSetRolTo(50108, 250.0, 15.0, 85.7f);
			_app->playMovie("1849");
			_app->puzzleSetActive(kPuzzle50100);
			_app->soundStop(51006, 1024);
		}

		if (_app->varGetByte(50012) > 4) {
			_app->playMovie("1850");
			_app->rotationSetAlp(50601, 250.0);
			_app->rotationSetActive(50601);
			_app->soundStop(51006, 1024);
		}
		break;

	case kObjectBark:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->bagAdd(kObjectBark);
		_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
		_app->objectSetAccessibilityOff(kObjectBark);
		break;

	case kObjectCounch1:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() != 50000 || _app->varGetByte(50005)) {
				if (_app->bagGetClickedObject() == 50501) {
					if (_app->varGetByte(50005) == 1) {
						_app->bagRemove(kObjectDragonSword);
						_app->rotationSetRolTo(50304, 145.0f, 9.0f, 85.7f);
						_app->varSetByte(50005, 2);
						_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
						_app->rotationSetMovabilityOff(50304, 0, 0);
						_app->rotationSetMovabilityOn(50304, 1, 1);
						_app->objectSetAccessibilityOn(kObjectCounch1, 1, 1);
						_app->objectPresentationHide(kObjectCounch1);

						handleEvents();

						_app->playMovie("1853");
						_app->rotationSetAlp(50304, 190.0);
						_app->rotationSetBet(50304, 15.0);
						_app->rotationSetActive(50304);
						_app->objectPresentationShow(kObjectCounch1, 1);

						if (!_app->varGetByte(50003))
							_app->objectPresentationShow(kObjectCounch1, 4);
					}
				}
				_app->cursorDelete();

			} else {
				_app->objectPresentationShow(kObjectCounch1, 0);
				_app->varSetByte(50005, 1);
				_app->playMovie("1852");
				_app->rotationSetAlp(50304, 190.0);
				_app->rotationSetBet(50304, 15.0);
				_app->rotationSetActive(50304);
				_app->cursorDelete();
			}
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (_app->varGetByte(50005) >= 2) {
				if (_app->varGetByte(50005) >= 2) {
					_app->objectPresentationShow(kObjectCounch1, 5);
					_app->objectSetAccessibilityOff(kObjectCounch1, 0, 0);
					_app->bagAdd(kObjectDragonSword);
				}
			}
			else {
				_app->puzzleSetActive(kPuzzle50304);
			}
			break;

		case 1:
			if (_app->varGetByte(50005) == 2) {
				_app->varSetByte(50005, 3);
				_app->objectPresentationShow(kObjectCounch1, 2);
				_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
				_app->objectSetAccessibilityOff(kObjectCounch1, 1, 1);
				_app->bagAdd(kObjectCounch1);
			}
			break;
		}
		break;

	case kObjectPhoenix:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() != 50000) {
				_app->rotationSetActive(50303);
				_app->cursorDelete();
				break;
			}

			_app->objectPresentationShow(kObjectPhoenix, 0);
			_app->objectSetAccessibilityOff(kObjectPhoenix);
			_app->objectSetAccessibilityOn(kObjectBark);
			_app->varSetByte(50003, 1);
			_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
			_app->soundPlay(51013, kSoundLoop);
			_app->rotationSetMovabilityOff(50301, 0, 1);
			_app->rotationSetMovabilityOn(50301, 2, 3);
			_app->rotationSetMovabilityOff(50302, 0, 2);
			_app->rotationSetMovabilityOn(50302, 3, 5);
			_app->rotationSetMovabilityOff(50303, 0, 0);
			_app->rotationSetMovabilityOn(50303, 1, 1);

			switch (_app->varGetByte(50005)) {
			default:
				_app->objectPresentationHide(kObjectCounch1);
				_app->objectPresentationShow(kObjectCounch1, 1);
				break;

			case 0:
			case 1:
				_app->objectPresentationShow(kObjectCounch1, 3);
				break;

			case 3:
				_app->objectPresentationHide(kObjectCounch1);
				_app->objectPresentationShow(kObjectCounch1, 1);
				_app->objectPresentationShow(kObjectCounch1, 2);
				break;
			}

			_app->objectPresentationShow(kObjectPhoenix, 0);
			_app->playMovie("1854");
			_app->varSetByte(50012, _app->varGetByte(50012) + 1);

			if (_app->varGetByte(50012) == 2) {
				_app->objectPresentationShow(kObject50700, 0);
				_app->objectSetAccessibilityOff(kObject50700);
				_app->objectSetAccessibilityOn(kObject50700, 1, 1);
				_app->rotationSetMovabilityOff(50103, 2, 2);
				_app->rotationSetMovabilityOn(50103, 3, 3);
			}

			if (_app->varGetByte(50012) == 4) {
				_app->objectPresentationShow(kObject50700, 1);
				_app->objectSetAccessibilityOff(kObject50700);
				_app->objectSetAccessibilityOn(kObject50700, 2, 2);
			}

			_app->rotationSetActive(50303);
			_app->cursorDelete();
			break;
		}

		_app->puzzleSetActive(kPuzzle50303);
		break;

	case kObjectRopes:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectDragonSword) {
				_app->playMovie("1855");
				_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
				_app->objectPresentationShow(kObjectRopes, 0);
				_app->varSetByte(50001, 1);
				_app->objectSetAccessibilityOff(kObjectRopes);
			}

			_app->cursorDelete();
		}
		break;

	case kObjectCloud:
		if (!_app->bagHasClickedObject())
			break;

		if (_app->bagGetClickedObject() == kObjectCounch1) {
			if (_app->bagHas(kObjectFeather)) {
				_app->playMovie("1856");

				if (_app->varGetByte(50001) == 1) {
					_app->objectPresentationHide(kObjectRopes, 0);
					_app->objectPresentationShow(kObjectRopes, 1);
					_app->varSetByte(50001, 2);
					_app->playMovie("1857");
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
					_app->objectSetAccessibilityOff(kObjectCloud);

					if (_app->varGetByte(50003) == 1) {
						_app->objectPresentationHide(kObjectRopes, 1);
						_app->objectPresentationShow(kObjectRopes, 2);
						_app->varSetByte(50001, 3);
						_app->playMovie("1858");
						_app->bagAdd(kObjectDeadLeaf);
						_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
						_app->varSetByte(50012, _app->varGetByte(50012) + 1);

						if (_app->varGetByte(50012) == 2) {
							_app->objectPresentationShow(kObject50700, 0);
							_app->objectSetAccessibilityOff(kObject50700);
							_app->objectSetAccessibilityOn(kObject50700, 1, 1);
							_app->rotationSetMovabilityOff(50103, 2, 2);
							_app->rotationSetMovabilityOn(50103, 3, 3);
						}

						if (_app->varGetByte(50012) == 4) {
							_app->objectPresentationShow(kObject50700, 1);
							_app->objectSetAccessibilityOff(kObject50700);
							_app->objectSetAccessibilityOn(kObject50700, 2, 2);
						}
					}
				}
			} else {
				_app->playMovie("1859", 0.0);
			}
		}

		_app->cursorDelete();
		break;
	}
}

void EventButtonRing::onButtonUpZoneAS(ObjectId id, uint32 target, Id, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObject80019:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (!_app->varGetByte(90001)) {
				_app->timerStopAll();
				_event->onSwitchZone(kZoneNI, 0);
			}
			break;

		case 1:
			if (!_app->varGetByte(90002)) {
				_app->timerStopAll();
				_event->onSwitchZone(kZoneN2, 0);
			}
			break;

		case 2:
			if (!_app->varGetByte(90001)) {
				_app->timerStopAll();
				_event->onSwitchZone(kZoneNI, 0);
			}
			break;

		case 3:
			if (!_app->varGetByte(90003)) {
				_app->timerStopAll();
				_event->onSwitchZone(kZoneFO, 0);
			}
			break;

		case 4:
			if (!_app->varGetByte(90002)) {
				_app->timerStopAll();
				_event->onSwitchZone(kZoneN2, 0);
			}
			break;

		case 5:
			if (!_app->varGetByte(90004)) {
				_app->timerStopAll();
				_event->onSwitchZone(kZoneWA, 0);
			}
			break;
		}
		break;

	case kObject80021:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (_app->varGetByte(80004) == 1) {
				if (_app->varGetByte(90001)) {
					_app->puzzleSetActive(kPuzzle80006);
					_app->playMovie("1142");
					_app->objectPresentationShow(kObject80019, 0);
				} else {
					_app->puzzleSetActive(kPuzzle80006);
					_app->playMovie("1141");
				}
			}

			if (_app->varGetByte(80004) == 11) {
				_app->puzzleSetActive(kPuzzle80007);
				_app->playMovie("1143");
			}

			if (_app->varGetByte(80004) == 21) {
				_app->puzzleSetActive(kPuzzle80009);
				_app->playMovie("1144");
			}

			if (_app->varGetByte(80004) == 31) {
				_app->puzzleSetActive(kPuzzle80008);
				_app->playMovie("1145");
			}

			if (_app->varGetByte(80004) == 41) {
				_app->puzzleSetActive(kPuzzle80010);
				_app->playMovie("1146");
			}
			break;

		case 1:
			_app->soundPlay(80208);
			_app->soundPlay(80210);

			_app->varSetByte(80004, _app->varGetByte(80005) + 10);
			if (_app->varGetByte(80004) >= 50)
				_app->varSetByte(80004, _app->varGetByte(80004) - 50);

			_app->objectPresentationUnpauseAnimation(kObject80018, 1);
			_app->objectSetAccessibilityOff(kObject80021, 0, 4);
			break;

		case 2:
			_app->soundPlay(80208);
			_app->soundPlay(80210);

			_app->varSetByte(80004, _app->varGetByte(80005) + 20);
			if (_app->varGetByte(80004) >= 50)
				_app->varSetByte(80004, _app->varGetByte(80004) - 50);

			_app->objectPresentationUnpauseAnimation(kObject80018, 1);
			_app->objectSetAccessibilityOff(kObject80021, 0, 4);
			break;

		case 3:
			_app->soundPlay(80208);
			_app->soundPlay(80210);

			_app->varSetByte(80004, _app->varGetByte(80005) + 30);
			if (_app->varGetByte(80004) >= 50)
				_app->varSetByte(80004, _app->varGetByte(80004) - 50);

			_app->objectPresentationUnpauseAnimation(kObject80018, 1);
			_app->objectSetAccessibilityOff(kObject80021, 0, 4);
			break;

		case 4:
			_app->soundPlay(80208);
			_app->soundPlay(80210);

			_app->varSetByte(80004, _app->varGetByte(80005) + 40);
			if (_app->varGetByte(80004) >= 50)
				_app->varSetByte(80004, _app->varGetByte(80004) - 50);

			_app->objectPresentationUnpauseAnimation(kObject80018, 1);
			_app->objectSetAccessibilityOff(kObject80021, 0, 4);
			break;
		}
		break;

	case kObject80007:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectDeath) {
				_app->bagRemoveAll();
				_app->soundStopAll(1024);
				_app->timerStopAll();
				_app->setupZone(kZoneAS, kSetupType6);
			}

			_app->cursorDelete();
		}
		break;

	case kObject80012:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->soundPlay(80028);
			break;

		case 1:
			_app->soundPlay(80025);
			break;
		case 2:
			_app->soundPlay(80021);
			break;
		case 3:
			_app->soundPlay(80024);
			break;
		case 4:
			_app->soundPlay(80022);
			break;
		case 5:
			_app->soundPlay(80026);
			break;
		case 6:
			_app->soundPlay(80027);
			break;
		case 7:
			_app->soundPlay(80023);
			break;
		}
		break;

	case kObject80018:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (_app->varGetByte(90001) == 1) {
				_app->puzzleSetActive(kPuzzle80002);
				_app->soundSetVolume(80201, 80);
				_app->soundPlay(80040);
			}
			break;

		case 1:
			if (_app->varGetByte(90002) == 1) {
				_app->puzzleSetActive(kPuzzle80003);
				_app->soundSetVolume(80203, 90);
				_app->soundPlay(80049);
			}
			break;
		case 2:
			if (_app->varGetByte(90003) == 1) {
				_app->puzzleSetActive(kPuzzle80004);
				_app->soundSetVolume(80204, 90);
				_app->soundPlay(80058);
			}
			break;
		case 3:
			if (_app->varGetByte(90004) == 1) {
				_app->puzzleSetActive(kPuzzle80005);
				_app->soundSetVolume(80205, 80);
				_app->soundPlay(80068);
			}
			break;
		}
		break;

	case kObject80022:
		if (_app->bagHasClickedObject())
			_app->cursorDelete();
		else
			_event->onSwitchZone(kZoneAS, 5);
		break;
	}
}

void EventButtonRing::onButtonUpZoneN2(ObjectId id, uint32 target, Id, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObjectFire:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectFire) {
				_app->varSetFloat(90006, _app->varGetFloat(90006) + 2.0f);
				_app->playMovie("1389");
				_app->rotationSetAlp(70500, 243.0f);
				_app->rotationSetRan(70500, 85.7f);
				_app->rotationSetActive(70500);
			}

			_app->cursorDelete();
		}
		break;

	case kObject70100:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (target == 1) {
			_app->rotationSetRolTo(70100, 270.4f, 10.4f, 85.7f);
			_app->puzzleSetActive(kPuzzle70100);
			_app->objectSetAccessibilityOn(kObject70101, 0, 0);
			_app->objectSetAccessibilityOn(kObject70102, 0, 0);
			_app->objectSetAccessibilityOn(kObject70101, 2, 2);
			_app->objectSetAccessibilityOn(kObject70102, 2, 2);
			_app->objectSetAccessibilityOn(kObject70100);

			g_system->warpMouse(505, 205);
		}
		break;

	case kObject70105:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->objectPresentationShow(kObject70106, (uint16)_app->varGetWord(70016));
		_app->puzzleSetActive(kPuzzle70102);

		break;

	case kObject70300:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectChrysoberyl) {
				_app->bagRemove(kObjectChrysoberyl);
				_app->playMovie("1495");
				_app->objectPresentationShow(kObjectCage);
				_app->bagAdd(kObjectCage);
				_app->varSetFloat(90006, _app->varGetFloat(90006) + 5.0f);
			}

			_app->cursorDelete();
		}
		break;

	case kObject70404:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->varSetByte(70005, 0);

		switch (target) {
		default:
			break;

		case 0:
			_app->objectPresentationHide(kObject70404);
			_app->puzzleSetMovabilityOff(kPuzzle70411);
			_app->objectSetAccessibilityOff(kObject70404, 0, 0);

			if (_app->varGetByte(70001)) {
				_app->objectPresentationShow(kObject70404, 4);
				_app->objectPresentationShow(kObject70404, 1);
			} else {
				_app->objectPresentationShow(kObject70404, 2);
			}

			_app->objectPresentationShow(kObject70404, 0);
			break;

		case 1:
			if (_app->varGetByte(70001) == 1) {
				_app->objectPresentationHide(kObject70404, 1);
				_app->bagAdd(kObjectLogeTear3);
				_app->varSetFloat(90006, _app->varGetFloat(90006) + 5.0f);
				_app->objectPresentationHide(kObject70404, 6);
				_app->varSetByte(70001, 0);
			}
			break;
		}
		break;

	case kObjectCentaur:
	case kObjectDragon:
	case kObjectPhoenix1:
	case kObjectChrysoberyl:
		if (_app->bagHasClickedObject()) {
			if (target == 1) {
				if (_app->bagGetClickedObject() == kObjectCentaur
				 || _app->bagGetClickedObject() == kObjectDragon
				 || _app->bagGetClickedObject() == kObjectPhoenix1) {
					_app->bagRemove(_app->bagGetClickedObject());
					_app->varSetByte(70015, (byte)(_app->bagGetClickedObject() - 100));
					_app->objectSetAccessibilityOff(kObjectCentaur);
					_app->objectSetAccessibilityOff(kObjectDragon);
					_app->objectSetAccessibilityOff(kObjectPhoenix1);
					_app->soundPlay(_app->varGetByte(70015) + 71010);
				}
			}

			_app->cursorDelete();

		} else {
			if (!target) {
				if (id == kObjectChrysoberyl) {
					_app->objectPresentationShow(kObjectChrysoberyl);
					_app->varSetFloat(90006, _app->varGetFloat(90006) + 5.0f);
				} else {
					_app->objectPresentationHide(id);
					_app->varSetFloat(90006, _app->varGetFloat(90006) + 3.0f);
				}

				_app->objectSetAccessibilityOff(id);
				_app->bagAdd(id);
			}
		}
		break;

	case kObject70700:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectFire) {
				if (target == 1) {
					_app->objectSetAccessibilityOff(id);
					_app->varSetFloat(90006, _app->varGetFloat(90006) + 5.0f);
					_app->objectPresentationHide(id);
					_app->soundStop(70701, 1024);
					_app->rotationSet3DSoundOff(70200, 70701);
					_app->playMovie("1494");
				}
			}

			_app->cursorDelete();

		} else {
			if (!target) {
				_app->varSetByte(70013, 31);
				_app->puzzleSetActive(kPuzzle70600);
				_app->soundPlay(70043);
			}
		}
		break;
	}
}

void EventButtonRing::onButtonUp2ZoneWA(ObjectId objectId, uint32 index, Id, uint32, const Common::Point &) {
	switch (objectId) {
	default:
		break;

	case kObjectMagnet:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() >= kObject50451
			 && _app->bagGetClickedObject() <= kObject50457) {

				_app->objectPresentationSetImageCoordinatesOnPuzzle(_app->bagGetClickedObject(), 0, Common::Point((int16)(53 * index / 10 + 162), (int16)(37 * index % 10 + 115)));
				_app->varSetDword(index + 51000, _app->bagGetClickedObject());
				_app->objectSetAccessibilityOn(_app->bagGetClickedObject(), index / 10 + 1 + 7 * index % 10, index / 10 + 1 + 7 * index % 10);
				_app->objectSetAccessibilityOff(kObjectMagnet, index / 10 + 7 * index % 10, index / 10 + 7 * index % 10);
				_app->objectPresentationShow(_app->bagGetClickedObject());

				if (_app->varGetDword(51030) == 50451
				 && _app->bagGetClickedObject() == kObject50451)
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);

				if (_app->varGetDword(51061) == 50452
				 && _app->bagGetClickedObject() == kObject50452)
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);

				if (_app->varGetDword(51001) == 50453
				 && _app->bagGetClickedObject() == kObject50453)
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);

				if (_app->varGetDword(51033) == 50454
				 && _app->bagGetClickedObject() == kObject50454)
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);

				if (_app->varGetDword(51026) == 50455
				 && _app->bagGetClickedObject() == kObject50455)
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);

				if (_app->varGetDword(51046) == 50456
				 && _app->bagGetClickedObject() == kObject50456)
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);

				if (_app->varGetDword(51042) == 50457
				 && _app->bagGetClickedObject() == kObject50457)
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);

				_app->bagRemove(_app->bagGetClickedObject());

				if (_app->varGetDword(51030) == 50451
				 && _app->varGetDword(51061) == 50452
				 && _app->varGetDword(51001) == 50453
				 && _app->varGetDword(51033) == 50454
				 && _app->varGetDword(51026) == 50455
				 && _app->varGetDword(51046) == 50456) {

					_app->objectSetAccessibilityOn(kObject50457, 0, 0);

					if (_app->varGetDword(51042) == 50457) {
						_app->playMovie("1867");
						_app->rotationSetMovabilityOff(50402, 2, 2);
						_app->bagAdd(kObjectFeather);
						_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
						_app->rotationSetAlp(50402, 180.0f);
						_app->rotationSetBet(50402, 0.3f);
						_app->rotationSetActive(50402);
						_app->varSetByte(50012, _app->varGetByte(50012) + 1);

						if (_app->varGetByte(50012) == 2) {
							_app->objectPresentationShow(kObject50700, 0);
							_app->objectSetAccessibilityOff(kObject50700);
							_app->objectSetAccessibilityOn(kObject50700, 1, 1);
							_app->rotationSetMovabilityOff(50103, 2, 2);
							_app->rotationSetMovabilityOn(50103, 3, 3);
						}

						if (_app->varGetByte(50012) == 4) {
							_app->objectPresentationShow(kObject50700, 1);
							_app->objectSetAccessibilityOff(kObject50700);
							_app->objectSetAccessibilityOn(kObject50700, 2, 2);
						}
					}
				}
			}

			_app->cursorDelete();
		}

		_app->setField74(false);
		break;

	case kObject50451:
	case kObject50452:
	case kObject50453:
	case kObject50454:
	case kObject50455:
	case kObject50456:
	case kObject50457:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() < kObject50451
			 || _app->bagGetClickedObject() > kObject50457
			 || (index && !_app->varGetDword(index + 50999))) {
				_app->setField74(false);
				_app->cursorDelete();
				break;
			}

			_app->objectPresentationSetImageOriginalCoordinatesOnPuzzle(_app->bagGetClickedObject(), 0);

			_app->objectSetAccessibilityOn(_app->bagGetClickedObject(), 0, 0);
			_app->objectPresentationShow(_app->bagGetClickedObject());
			_app->bagRemove(_app->bagGetClickedObject());
			_app->setField74(false);
			_app->cursorDelete();
			break;
		}

		switch (index) {
		default: {
			if (_app->varGetDword(51030) == 50451
				&& objectId == kObject50451)
				_app->varSetFloat(90008, _app->varGetFloat(90008) - 1.0f);

			if (_app->varGetDword(51061) == 50452
				&& objectId == kObject50452)
				_app->varSetFloat(90008, _app->varGetFloat(90008) - 1.0f);

			if (_app->varGetDword(51001) == 50453
				&& objectId == kObject50453)
				_app->varSetFloat(90008, _app->varGetFloat(90008) - 1.0f);

			if (_app->varGetDword(51033) == 50454
				&& objectId == kObject50454)
				_app->varSetFloat(90008, _app->varGetFloat(90008) - 1.0f);

			if (_app->varGetDword(51026) == 50455
				&& objectId == kObject50455)
				_app->varSetFloat(90008, _app->varGetFloat(90008) - 1.0f);

			if (_app->varGetDword(51046) == 50456
				&& objectId == kObject50456)
				_app->varSetFloat(90008, _app->varGetFloat(90008) - 1.0f);

			if (_app->varGetDword(51042) == 50457
				&& objectId == kObject50457)
				_app->varSetFloat(90008, _app->varGetFloat(90008) - 1.0f);

			_app->varSetDword(index + 50999, 0);

			uint32 idx = (index - 1) / 10 + 7 * (index - 1) % 10;
			_app->objectSetAccessibilityOff(objectId, idx + 1, idx + 1);
			_app->objectSetAccessibilityOn(kObjectMagnet, idx, idx);
			_app->objectPresentationHide(objectId);
			_app->bagAdd(objectId);
			}
			break;

		case 0:
			_app->objectPresentationHide(objectId);
			_app->bagAdd(objectId);
			_app->objectSetAccessibilityOff(objectId, index, index);
			break;
		}
		break;

	case kObjectFronthead:
	case kObjectBackhead:
	case kObjectBelly:
	case kObjectRightArm:
	case kObjectLeftArm:
	case kObjectLegs:
	case kObjectHeart:
		if (!_app->bagHasClickedObject()) {
			if (!index && !_app->bagHas(objectId)) {
				_app->objectPresentationHide(objectId, 0);
				_app->bagAdd(objectId);
				break;
			}

			_app->setField74(false);
			break;
		}

		if (_app->bagGetClickedObject().id() == objectId && index > 0) {
			_app->objectPresentationShow(objectId, 1);
			_app->objectSetAccessibilityOff(objectId, 0, 1);
			_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);
			_app->varSetDword(50000, (int32)index + _app->varGetDword(50000));

			if (_app->varGetDword(50000) == 7654321) {
				_app->playMovie("1865");
				_app->playMovie("1866");
				_app->rotationSetMovabilityOff(50401, 1, 1);
				_app->rotationSetMovabilityOn(50401, 2, 2);
				_app->rotationSetMovabilityOff(50402, 1, 1);
				_app->rotationSetMovabilityOn(50402, 2, 2);
				_app->objectPresentationShow(kObjectGolem1, 0);
				_app->rotationSetAlp(50402, 180.0f);
				_app->rotationSetBet(50402, 0.3f);
				_app->rotationSetActive(50402);
			} else {
				if (_app->varGetDword(50000) == 654321)
					_app->objectSetAccessibilityOn(kObjectHeart);
			}

			_app->bagRemove(objectId);
			_app->setField74(false);
			_app->cursorDelete();
			break;
		}

		if (_app->bagGetClickedObject() != kObjectFronthead
		 && _app->bagGetClickedObject() != kObjectBackhead
		 && _app->bagGetClickedObject() != kObjectBelly
		 && _app->bagGetClickedObject() != kObjectRightArm
		 && _app->bagGetClickedObject() != kObjectLeftArm
		 && _app->bagGetClickedObject() != kObjectLegs
		 && _app->bagGetClickedObject() != kObjectHeart) {
		 	_app->setField74(false);
			_app->cursorDelete();
			break;
		}

		_app->objectPresentationShow(_app->bagGetClickedObject(), 0);
		_app->bagRemove(_app->bagGetClickedObject());
		_app->setField74(false);
		_app->cursorDelete();
		break;

	case kObjectInk:
	case kObjectPaper:
	case kObjectStylet:
	case kObjectInkedStylet:
		if (_app->bagHasClickedObject()) {
			switch (index) {
			default:
				break;

			case 0:
			case 1:
			case 2:
				if (_app->bagGetClickedObject().id() == objectId) {
					_app->bagRemove(objectId);
					_app->objectPresentationShow(kObject50100, index);
					_app->setField74(false);
					_app->cursorDelete();
				}
				break;

			case 8:
				if (_app->bagGetClickedObject() == kObjectPaper) {
					_app->objectPresentationShow(kObject50100, 3);
					_app->bagRemove(kObjectPaper);
					_app->objectSetAccessibilityOff(kObjectPaper, 0, 0);
					_app->varSetWord(50103, 102);
					_app->setField74(false);
					_app->cursorDelete();
				}

				if (_app->bagGetClickedObject() == kObjectInkedStylet
				 && _app->varGetWord(50103) > 0) {
					_app->objectPresentationShow(kObject50100, 5);
					_app->bagRemove(kObjectInkedStylet);
					_app->varSetWord(50105, 104);
					_app->objectPresentationPauseAnimationFrame(kObject50100, 7, 30, 10000, 2);
					_app->objectPresentationShow(kObject50100, 7);
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
					_app->setField74(false);
					_app->cursorDelete();
				}
				break;

			case 9:
				if (_app->bagGetClickedObject() == kObjectInk
				 && !_app->varGetWord(50101)) {
					_app->objectPresentationShow(kObject50100, 4);
					_app->bagRemove(kObjectInk);
					_app->objectSetAccessibilityOff(kObjectInk, 0, 0);
					_app->varSetWord(50101, 101);
					_app->setField74(false);
					_app->cursorDelete();
				}

				if (_app->bagGetClickedObject() == kObjectStylet
				 && _app->varGetWord(50101) > 0) {
					_app->objectPresentationHide(kObject50100, 4);
					_app->objectPresentationShow(kObject50100, 6);
					_app->objectSetAccessibilityOff(kObjectStylet, 0, 0);
					_app->bagRemove(kObjectStylet);
					_app->varSetWord(50102, 103);
					_app->setField74(false);
					_app->cursorDelete();
				}
				break;
			}

			_app->setField74(false);
			_app->cursorDelete();
			break;
		}

		switch (index) {
		default:
			break;

		case 0:
		case 1:
		case 2:
			if (!_app->bagHas(objectId)) {
				_app->bagAdd(objectId);
				_app->objectPresentationHide(kObject50100, index);
			}
			break;

		case 9:
			if (_app->varGetWord(50102) > 0
			&& !_app->bagHas(kObjectInkedStylet)) {
				_app->objectPresentationHide(kObject50100, 6);
				_app->bagAdd(kObjectInkedStylet);
			}
			break;
		}
		_app->setField74(false);
		break;
	}
}

#pragma endregion

void EventButtonRing::waitTicks(uint32 ticks) const {
	uint32 startTicks = g_system->getMillis();
	while (g_system->getMillis() - startTicks < ticks)
		handleEvents();
}

bool EventButtonRing::copySavedTimers(uint32 slot) const {
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

bool EventButtonRing::removeSavedTimers(uint32 slot) const {
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

} // End of namespace Ring
