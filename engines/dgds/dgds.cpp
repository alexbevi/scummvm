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

namespace Dgds {

DgdsEngine::DgdsEngine(OSystem *syst, Common::Language lang): Engine(syst), _language(lang), _resMgr(NULL) {
	DebugMan.addDebugChannel(kDebugResources, "Resources", "Debug Information for Resources");

	g_eventRec.registerRandomSource(_rnd, "dgds");
	DebugMan.enableDebugChannel("Resources");
}

DgdsEngine::~DgdsEngine() {
	if (_resMgr)
		delete _resMgr;

	DebugMan.clearAllDebugChannels();
}

Common::Error DgdsEngine::init() {
	// Initialize backend
	initGraphics(320, 200, true);

	return Common::kNoError;
}

Common::Error DgdsEngine::run() {
	// Start the resource manager
	if (Common::File::exists("volume.rmf"))
		_resMgr = new ResourceManager("volume.rmf");
	if (!_resMgr && Common::File::exists("volume.vga"))
		_resMgr = new ResourceManager("volume.vga");
	if (!_resMgr && Common::File::exists("resource.map"))
		_resMgr = new ResourceManager("resource.map");
	if (!_resMgr)
		_resMgr = new ResourceManager("");

	// XXX dump resources and tagged subresources
	// to the specified folders
	//_resMgr->dumpResources("dump_res/", false);
	//_resMgr->dumpResources("dump_subres/", true);
	//return Common::kNoError;

	Common::String gameName = findGDS();
	if(!gameName.size())
		return Common::kNoGameDataFoundError;

	_game.load(_resMgr, gameName);

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

	return Common::kNoError;
}

Common::String DgdsEngine::findGDS(void) {
	Common::String name;

	// Search a GDS file inside the volumes
	ResourceFiles::const_iterator _resIter = _resMgr->_resourceFiles.begin();
	while (!name.size() && _resIter != _resMgr->_resourceFiles.end()) {
		if (_resIter->_key.hasSuffix(".GDS"))
			name = _resIter->_key;

		++_resIter;
	}

	// Get just the name without the extension
	if (name.size()) {
		name = Common::String(name.c_str(), name.size() - 4);
		name.toUppercase();
	}

	return name;
}

} // End of namespace Dgds
