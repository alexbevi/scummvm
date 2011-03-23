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

#include "ring/game/application_faust.h"

#include "ring/game/event_faust.h"

#include "ring/graphics/screen.h"

//using namespace FaustGame;

namespace Ring {

ApplicationFaust::ApplicationFaust(RingEngine *engine) : Application(engine) {
	_eventHandler = new EventHandlerFaust(this);
}

ApplicationFaust::~ApplicationFaust() {
}

#pragma region Game setup

void ApplicationFaust::initFont() {
	error("[ApplicationFaust::initFont] Not implemented");
}

void ApplicationFaust::setup() {
	error("[ApplicationFaust::setup] Not implemented");
}

#pragma endregion

#pragma region Startup

void ApplicationFaust::showStartupScreen() {
	error("[ApplicationFaust::showStartupScreen] Not implemented");
}

void ApplicationFaust::startMenu(bool savegame) {
	error("[ApplicationFaust::startMenu] Not implemented");
}

void ApplicationFaust::showMenu(ZoneId zone, uint32 a2) {
	error("[ApplicationFaust::startMenu] Not implemented");
}

void ApplicationFaust::showCredits() {
	error("[ApplicationFaust::showCredits] Not implemented");
}

void ApplicationFaust::loadPreferences() {
	error("[ApplicationFaust::loadPreferences] Not implemented");
}

#pragma endregion

#pragma region Drawing

void ApplicationFaust::draw() {
	// Update our screen
	_screenManager->updateScreen();

	// Update engine state
	error("[ApplicationFaust::draw] Engine state update not implemented!");
}

#pragma endregion

#pragma region Visual

Visual *ApplicationFaust::createVisual(Id visualId, uint32 a3, uint32 a4, uint32 left, uint32 top, uint32 offsetY, uint32 height, uint32 progressMultiplier, uint32 progressColor) {
	error("[ApplicationFaust::createVisual] Engine state update not implemented!");
}

#pragma endregion

#pragma region Zone initialization

void ApplicationFaust::initZones() {
	error("[ApplicationFaust::initZones] Not implemented");
}

void ApplicationFaust::setupZone(ZoneId zone, SetupType type) {
	error("[ApplicationFaust::setupZone] Not implemented");
}

#pragma endregion

} // End of namespace Ring
