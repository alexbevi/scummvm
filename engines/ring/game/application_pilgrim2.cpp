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

#include "ring/game/application_pilgrim2.h"

#include "ring/game/event_pilgrim2.h"

#include "ring/graphics/screen.h"

//using namespace Pilgrim2Game;

namespace Ring {

ApplicationPilgrim2::ApplicationPilgrim2(RingEngine *engine) : Application(engine) {
	_eventHandler = new EventHandlerPilgrim2(this);
}

ApplicationPilgrim2::~ApplicationPilgrim2() {
}

#pragma region Game setup

void ApplicationPilgrim2::initFont() {
	// Original asks for size 12, but the font only contains size 8
	fontAdd(kFontDefault, "legend.fon", "Arxel1", 8, true, false, false, false, getCurrentLanguage());
}

void ApplicationPilgrim2::setup() {
	error("[ApplicationPilgrim2::setup] Not implemented");
}

#pragma endregion

#pragma region Startup

void ApplicationPilgrim2::showStartupScreen() {
	error("[ApplicationPilgrim2::showStartupScreen] Not implemented");
}

void ApplicationPilgrim2::startMenu(bool savegame) {
	error("[ApplicationPilgrim2::startMenu] Not implemented");
}

void ApplicationPilgrim2::showMenu(ZoneId zone, uint32 a2) {
	error("[ApplicationPilgrim2::showMenu] Not implemented");
}

void ApplicationPilgrim2::showCredits() {
	error("[ApplicationPilgrim2::showCredits] Not implemented");
}

void ApplicationPilgrim2::loadPreferences() {
	error("[ApplicationPilgrim2::loadPreferences] Not implemented");
}

#pragma endregion

#pragma region Drawing

void ApplicationPilgrim2::draw() {
	// Update our screen
	_screenManager->updateScreen();

	// Update engine state
	error("[ApplicationPilgrim2::draw] Engine state update not implemented!");
}

#pragma endregion

#pragma region Visual

Visual *ApplicationPilgrim2::createVisual(Id visualId, uint32 a3, uint32 a4, uint32 left, uint32 top, uint32 offsetY, uint32 height, uint32 progressMultiplier, uint32 progressColor) {
	error("[ApplicationPilgrim2::createVisual] Engine state update not implemented!");
}

#pragma endregion

#pragma region Zone initialization

void ApplicationPilgrim2::initZones() {
	error("[ApplicationPilgrim2::initZones] Not implemented");
}

void ApplicationPilgrim2::setupZone(ZoneId zone, SetupType type) {
	error("[ApplicationPilgrim2::setupZone] Not implemented");
}

#pragma endregion

} // End of namespace Ring
