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

#include "ring/game/pilgrim2/pilgrim2_init.h"

#include "ring/game/pilgrim2/pilgrim2_application.h"

namespace Ring {

EventInitPilgrim2::EventInitPilgrim2(ApplicationPilgrim2 *application) : _app(application) {
}

EventInitPilgrim2::~EventInitPilgrim2() {
	// Zero-out passed pointers
	_app = NULL;
}

void EventInitPilgrim2::initZoneSystem() {
	error("[EventInitPilgrim2::initZoneSystem] Not implemented");
}

void EventInitPilgrim2::initZone1() {
	error("[EventInitPilgrim2::initZone1] Not implemented");
}

void EventInitPilgrim2::initZone2() {
	error("[EventInitPilgrim2::initZone2] Not implemented");
}

void EventInitPilgrim2::initZone3() {
	error("[EventInitPilgrim2::initZone3] Not implemented");
}

void EventInitPilgrim2::initZone4() {
	error("[EventInitPilgrim2::initZone4] Not implemented");
}

void EventInitPilgrim2::initZone5() {
	error("[EventInitPilgrim2::initZone5] Not implemented");
}

} // End of namespace Ring
