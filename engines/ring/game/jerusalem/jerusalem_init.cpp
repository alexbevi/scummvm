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

#include "ring/game/jerusalem/jerusalem_init.h"

#include "ring/game/jerusalem/jerusalem_application.h"

namespace Ring {

EventInitJerusalem::EventInitJerusalem(ApplicationJerusalem *application) : _app(application) {
}

EventInitJerusalem::~EventInitJerusalem() {
	// Zero-out passed pointers
	_app = NULL;
}

void EventInitJerusalem::initZoneSystem() {
	error("[EventInitJerusalem::initZoneSystem] Not implemented");
}

void EventInitJerusalem::initZone1() {
	error("[EventInitJerusalem::initZone1] Not implemented");
}

void EventInitJerusalem::initZone2() {
	error("[EventInitJerusalem::initZone2] Not implemented");
}

void EventInitJerusalem::initZone3() {
	error("[EventInitJerusalem::initZone3] Not implemented");
}

void EventInitJerusalem::initZone4() {
	error("[EventInitJerusalem::initZone4] Not implemented");
}

void EventInitJerusalem::initZone5() {
	error("[EventInitJerusalem::initZone5] Not implemented");
}

void EventInitJerusalem::initZone6() {
	error("[EventInitJerusalem::initZone6] Not implemented");
}

void EventInitJerusalem::initZone7() {
	error("[EventInitJerusalem::initZone7] Not implemented");
}

void EventInitJerusalem::initZone8() {
	error("[EventInitJerusalem::initZone8] Not implemented");
}

void EventInitJerusalem::initZone9() {
	error("[EventInitJerusalem::initZone9] Not implemented");
}

void EventInitJerusalem::initZone10() {
	error("[EventInitJerusalem::initZone10] Not implemented");
}

} // End of namespace Ring
