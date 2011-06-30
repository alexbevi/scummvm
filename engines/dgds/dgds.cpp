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
 * $URL: https://scummvm-dgds.googlecode.com/svn/trunk/dgds.cpp $
 * $Id: dgds.cpp 28 2010-06-19 11:16:58Z alexbevi $
 *
 */

#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "common/system.h"
#include "common/savefile.h"
#include "common/EventRecorder.h"

#include "engines/util.h"

#include "dgds/dgds.h"
#include "dgds/game_willy.h"

#include "dgds/resources/moviegroup.h"
#include "dgds/resources/movie.h"

namespace Dgds {

DgdsEngine::DgdsEngine(OSystem *syst, Common::Language lang): Engine(syst), _language(lang), _resMgr(NULL) {
	DebugMan.addDebugChannel(kDebugResources, "Resources", "Debug Information for Resources");

	DebugMan.enableDebugChannel("Resources");
	_rnd = new Common::RandomSource("dgds");
}

DgdsEngine::~DgdsEngine() {
	if (_resMgr)
		delete _resMgr;

	if (_game)
		delete _game;

	DebugMan.clearAllDebugChannels();
	delete _rnd;
}

Common::Error DgdsEngine::init() {
	// Initialize backend
	initGraphics(320, 200, true);

	return Common::kNoError;
}

Common::Error DgdsEngine::run() {
	Common::String gameName = Resman.findGDS();
	if(!gameName.size())
		return Common::kNoGameDataFoundError;

	if (gameName == "WILLY") {
		debug("Starting Willy Beamish");
		_game = new WillyBeamish(this);
	} else {
		error("DGDS Title %s currently not supported", gameName.c_str());
	}

	Resource *ttm = Resman.load("TITLE.TTM");
	Movie *m = new Movie(ttm);
	m->play();

	delete ttm;
	delete m;

	/*
	bool end = false;
	Common::EventManager *em = _system->getEventManager();
	while (!end) {
		Common::Event ev;
		if (em->pollEvent(ev)) {
			if (ev.type == Common::EVENT_KEYDOWN) {
				if (ev.kbd.keycode == Common::KEYCODE_ESCAPE)
					end = true;
			}
		}
		_system->delayMillis(10);
	}
	*/

	return Common::kNoError;
}

} // End of namespace Dgds
