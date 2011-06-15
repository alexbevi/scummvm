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

#include "ring/game/faust/faust_setup.h"

#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_shared.h"

using namespace FaustGame;

namespace Ring {

EventSetupFaust::EventSetupFaust(ApplicationFaust *application) : _app(application) {
}

EventSetupFaust::~EventSetupFaust() {
	// Zero-out passed pointers
	_app = NULL;
}

void EventSetupFaust::onSetupZone2(SetupType type) {
	error("[EventSetupFaust::onSetupZone2] Not implemented");
}

void EventSetupFaust::onSetupZone3(SetupType type) {
	error("[EventSetupFaust::onSetupZone3] Not implemented");
}

void EventSetupFaust::onSetupZone4(SetupType type) {
	error("[EventSetupFaust::onSetupZone4] Not implemented");
}

void EventSetupFaust::onSetupZone5(SetupType type) {
	error("[EventSetupFaust::onSetupZone5] Not implemented");
}

void EventSetupFaust::onSetupZone6(SetupType type) {
	error("[EventSetupFaust::onSetupZone6] Not implemented");
}

void EventSetupFaust::onSetupZone7(SetupType type) {
	error("[EventSetupFaust::onSetupZone7] Not implemented");
}

void EventSetupFaust::onSetupZone8(SetupType type) {
	error("[EventSetupFaust::onSetupZone8] Not implemented");
}

} // End of namespace Ring
