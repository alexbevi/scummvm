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
void Application::init() {
	error("[Application:init] Not implemented");
}

void Application::setupCursors() {
	error("[Application:setupCursors] Not implemented");
}

void Application::setupZones() {
	error("[Application:setupZones] Not implemented");
}

//////////////////////////////////////////////////////////////////////////
// Startup
void Application::showStartupScreen() {
	error("[Application:showStartupScreen] Not implemented");
}

void Application::startMenu() {
	error("[Application:startMenu] Not implemented");
}

} // End of namespace Ring
