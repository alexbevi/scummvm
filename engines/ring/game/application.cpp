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

#include "ring/base/cursor.h"
#include "ring/base/font.h"
#include "ring/base/preferences.h"
#include "ring/base/sound.h"
#include "ring/base/timer.h"
#include "ring/base/var.h"

#include "ring/game/art.h"
#include "ring/game/bag.h"
#include "ring/game/dialog.h"
#include "ring/game/language.h"
#include "ring/game/object.h"
#include "ring/game/puzzle.h"

#include "ring/graphics/dragControl.h"
#include "ring/graphics/rotation.h"
#include "ring/graphics/video.h"

#include "ring/helpers.h"
#include "ring/ring.h"

namespace Ring {

Application::Application(RingEngine *engine) : _vm(engine),
	_video(NULL), _artHandler(NULL), _fontHandler(NULL), _dialogHandler(NULL), _languageHandler(NULL),
	_field_54(1), _zoneName("A0"), _cursorHandler(NULL), _soundHandler(NULL), _field_5D(0),
	_field_5E(0), _field_58(101), _field_66(0), _field_6A(0), _zone(kZoneInvalid),
	_field_6F(0), _field_70(0), _puzzle(NULL), _field_89(NULL), _bag(NULL),
	_timerHandler(NULL), _var(NULL), _dragControl(NULL), _objectHandler(NULL), _preferenceHandler(NULL),
	_field_A5(0) {
}

Application::~Application() {
	// TODO delete global image buffer

	SAFE_DELETE(_dragControl);
	SAFE_DELETE(_var);
	SAFE_DELETE(_timerHandler);
	SAFE_DELETE(_field_89);
	CLEAR_ARRAY(Rotation, _rotationList);
	SAFE_DELETE(_puzzle);
	CLEAR_ARRAY(PuzzleInfo, _puzzleList);
	CLEAR_ARRAY(ObjectInfo, _objectList);
	SAFE_DELETE(_cursorHandler);
	SAFE_DELETE(_soundHandler);
	SAFE_DELETE(_fontHandler);
	SAFE_DELETE(_languageHandler);
	SAFE_DELETE(_artHandler);
	SAFE_DELETE(_objectHandler);
	SAFE_DELETE(_preferenceHandler);
	SAFE_DELETE(_video);

	// Zero-out passed pointers
	_vm = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Initialization
//////////////////////////////////////////////////////////////////////////
void Application::init() {

	// Setup available languages
	_languageHandler = new LanguageHandler();
	_languageHandler->add(kLanguageEnglish, "ENG", "ENG", 1);
	_languageHandler->add(kLanguageFrench,  "FRA", "FRA", 2);
	_languageHandler->add(kLanguageGerman,  "GER", "GER", 3);
	_languageHandler->add(kLanguageItalian, "ITA", "ITA", 1);
	_languageHandler->add(kLanguageSpanish, "SPA", "SPA", 2);
	_languageHandler->add(kLanguageSwedish, "SWE", "SWE", 1);
	_languageHandler->add(kLanguageDutch,   "HOL", "HOL", 3);
	_languageHandler->add(kLanguageHebrew,  "HEB", "HEB", 1);
	_languageHandler->add(9, "GRE", "GRE", 1);
	_languageHandler->add(10, "SLO", "SLO", 1);

	loadConfiguration();

	// Setup video
	_video = new Video();
	_video->init(); // TODO probably not needed as we initialize graphics at engine start
	_video->sub_4028D0(0, 0);

	// Setup objects
	_objectHandler = new ObjectHandler();
	_objectHandler->init();
	_objectHandler->addObjectsFromFile("aObj.ini");

	// Setup art
	if (_configuration.artBAG || _configuration.artCURSOR || _configuration.artSY)
		_field_58 = 102;

	if (_field_58 == 102) {
		_artHandler = new ArtHandler();
		_artHandler->init();
		_artHandler->open(kZoneSY, kLoadFromDisk);
	}

	_field_58 = 101;

	// Setup fonts
	_fontHandler = new FontHandler();
	_fontHandler->addResource("arxrin.fon");

	switch (getLanguage()) {
	default:
		_fontHandler->add(1, "ARX Pilgrim L", 12, 1, 2, 2, 2);
		break;

	case kLanguageHebrew:
		_fontHandler->add(1, "ArxelHebrew", 12, 1, 2, 2, 2);
		break;

	case kLanguageGreek:
		// FIXME replace by proper GUI font
		_fontHandler->add(1, "Arial", 16, 1, 2, 2, 2);
		break;
	}

	// Setup dialogs
	_dialogHandler = new DialogHandler();
	_dialogHandler->init(1, 200, 200, 30, 0, 0, 0, 461, 3);

	// Setup cursors
	_cursorHandler = new CursorHandler();

	// Setup sound
	_soundHandler = new SoundHandler();
	_soundHandler->init();

	// Setup data
	_field_70 = 0;
	_field_74 = 1;
	_field_75 = 1;
	_field_76 = 1;
	_field_77 = 1;
	_field_78 = 1;
	_field_5D = 1;
	_field_54 = 1;
	_field_58 = 101;
	_field_6F = 0;

	_field_89 = NULL;

	// Setup bag
	if (_configuration.artBAG)
		_field_58 = 102;

	_bag= new Bag();
	_bag->init();
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
	_bag->loadBackground("bagbgr.tga", "", "bagarr.tga", "", "", "bagarr.tga", "", "menu_gur.tga", _field_58);
	_bag->sub_4178C0();

	_field_58 = 101;

	// Setup timer
	_timerHandler = new TimerHandler();

	// Setup var
	_var = new Var();

	// Setup dragControl
	_dragControl = new DragControl();
	_dragControl->init();

	// Setup preferences
	_preferenceHandler = new PreferenceHandler();
	_preferenceHandler->load();
}

void Application::loadConfiguration() {
	error("[Application::loadConfiguration] Not implemented");
}

void Application::setupCursors() {
	error("[Application::setupCursors] Not implemented");
}

void Application::setupZones() {
	error("[Application::setupZones] Not implemented");
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
// Zone
//////////////////////////////////////////////////////////////////////////
Common::String Application::getZone(Zone zone) {
	switch (zone) {
	default:
		break;

	case kZoneSY:
		return "sy";

	case kZoneNI:
		return "ni";

	case kZoneRH:
		return "rh";

	case kZoneFO:
		return "fo";

	case kZoneRO:
		return "ro";

	case kZoneWA:
		return "wa";

	case kZoneAS:
		return "as";

	case kZoneN2:
		return "n2";
	}

	error("[Application::getZone] Invalid zone (%d)", zone);
}

Common::String Application::getZoneName(Zone zone) {
	switch (zone) {
	default:
		break;

	case kZoneSY:
		return "";

	case kZoneNI:
	case kZoneRH:
		return "Alberich";

	case kZoneFO:
		return "Siegmund";

	case kZoneWA:
		return "Brnnnhilde";

	case kZoneAS:
		return "Dril";

	case kZoneRO:
	case kZoneN2:
		return "Loge";
	}

	error("[Application::getZoneName] Invalid zone (%d)", zone);
}

uint32 Application::getReadFrom(Zone zone) {
	if (_field_58 == 101)
		return 101;

	switch (zone) {
	default:
		break;

	case kZoneSY:
		return _configuration.artSY ? 102 : 101;

	case kZoneNI:
		return _configuration.artNI ? 102 : 101;

	case kZoneRH:
		return _configuration.artRH ? 102 : 101;

	case kZoneFO:
		return _configuration.artFO ? 102 : 101;

	case kZoneRO:
		return _configuration.artRO ? 102 : 101;

	case kZoneWA:
		return _configuration.artWA ? 102 : 101;

	case kZoneAS:
		return _configuration.artAS ? 102 : 101;

	case kZoneN2:
		return _configuration.artN2 ? 102 : 101;
	}

	error("[Application::getReadFrom] Invalid zone (%d)", zone);
}

//////////////////////////////////////////////////////////////////////////
// Accessors
//////////////////////////////////////////////////////////////////////////
LanguageId Application::getLanguage() {
	if (!_languageHandler)
		error("[Application::getLanguage] languageHandler not initialized properly");

	return _languageHandler->getLanguage();
}

} // End of namespace Ring
