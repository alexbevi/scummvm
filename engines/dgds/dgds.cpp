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

#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "common/events.h"
#include "common/system.h"
#include "common/file.h"
#include "common/EventRecorder.h"

#include "engines/advancedDetector.h"
#include "engines/util.h"

#include "dgds/dgds.h"

namespace Dgds {

DgdsEngine::DgdsEngine(OSystem *syst, Common::Language lang):
	Engine(syst),
	_language(lang) {
	g_eventRec.registerRandomSource(_rnd, "dgds");
}

DgdsEngine::~DgdsEngine() {
}

Common::Error DgdsEngine::init() {
	// Initialize backend
	initGraphics(640, 480, true);

	return Common::kNoError;
}

Common::Error DgdsEngine::run() {
	Common::Error err;
	err = init();

	if (err != Common::kNoError)
		return err;

	return Common::kNoError;
}

} // End of namespace Dgds
