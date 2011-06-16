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

#include "ring/game/pilgrim3/pilgrim3_init.h"

#include "ring/game/pilgrim3/pilgrim3_application.h"

namespace Ring {

EventInitPilgrim3::EventInitPilgrim3(ApplicationPilgrim3 *application) : _app(application) {
}

EventInitPilgrim3::~EventInitPilgrim3() {
	// Zero-out passed pointers
	_app = NULL;
}

void EventInitPilgrim3::initZoneSystem() {
	error("[EventInitPilgrim3::initZoneSystem] Not implemented");
}

void EventInitPilgrim3::initZone6() {
	error("[EventInitPilgrim3::initZone6] Not implemented");
}

void EventInitPilgrim3::initZone7() {
	error("[EventInitPilgrim3::initZone7] Not implemented");
}

void EventInitPilgrim3::initZone8() {
	error("[EventInitPilgrim3::initZone8] Not implemented");
}

void EventInitPilgrim3::initZone9() {
	error("[EventInitPilgrim3::initZone9] Not implemented");
}

void EventInitPilgrim3::initZone10() {
	error("[EventInitPilgrim3::initZone10] Not implemented");
}

} // End of namespace Ring
