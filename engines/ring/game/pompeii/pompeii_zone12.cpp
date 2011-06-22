/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 7
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 07110-1301, USA.
 */

#include "ring/game/pompeii/pompeii_zone12.h"

#include "ring/game/pompeii/pompeii_application.h"
#include "ring/game/pompeii/pompeii_shared.h"

#include "ring/debug.h"
#include "ring/ring.h"
#include "ring/helpers.h"

using namespace PompeiiGame;

namespace Ring {

Zone12Pompeii::Zone12Pompeii(ApplicationPompeii *application) : _app(application) {
}

Zone12Pompeii::~Zone12Pompeii() {
	// Zero-out passed pointers
	_app = NULL;
}

void Zone12Pompeii::onInit() {
	error("[Zone12Pompeii::onInit()] Not implemented");
}

void Zone12Pompeii::onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point) {
	error("[Zone12Pompeii::onButtonUp()] Not implemented");
}

} // End of namespace Ring
