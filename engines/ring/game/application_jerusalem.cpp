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

#include "ring/game/application_jerusalem.h"

#include "ring/game/event_jerusalem.h"

#include "ring/graphics/screen.h"

//using namespace JerusalemGame;

namespace Ring {

ApplicationJerusalem::ApplicationJerusalem(RingEngine *engine) : Application(engine) {
	_eventHandler = new EventHandlerJerusalem(this);
}

ApplicationJerusalem::~ApplicationJerusalem() {
}

#pragma region Game setup

void ApplicationJerusalem::initFont() {
	error("[ApplicationJerusalem::initFont] Not implemented");
}

void ApplicationJerusalem::setup() {
	error("[ApplicationJerusalem::setup] Not implemented");
}

#pragma endregion

#pragma region Startup

void ApplicationJerusalem::showStartupScreen() {
	error("[ApplicationJerusalem::showStartupScreen] Not implemented");
}

void ApplicationJerusalem::startMenu(bool savegame) {
	error("[ApplicationJerusalem::startMenu] Not implemented");
}

void ApplicationJerusalem::showMenu(ZoneId zone, uint32 a2) {
	error("[ApplicationJerusalem::showMenu] Not implemented");
}

void ApplicationJerusalem::showCredits() {
	error("[ApplicationJerusalem::showCredits] Not implemented");
}

void ApplicationJerusalem::loadPreferences() {
	error("[ApplicationJerusalem::loadPreferences] Not implemented");
}

#pragma endregion

#pragma region Drawing

void ApplicationJerusalem::draw() {
	// Update our screen
	_screenManager->updateScreen();

	// Update engine state
	error("[ApplicationJerusalem::draw] Engine state update not implemented!");
}

#pragma endregion

#pragma region Visual

Visual *ApplicationJerusalem::createVisual(Id visualId, uint32 a3, uint32 a4, uint32 left, uint32 top, uint32 offsetY, uint32 height, uint32 progressMultiplier, uint32 progressColor) {
	error("[ApplicationJerusalem::createVisual] Engine state update not implemented!");
}

#pragma endregion

#pragma region Zone initialization

void ApplicationJerusalem::initZones() {
	error("[ApplicationJerusalem::initZones] Not implemented");
}

void ApplicationJerusalem::setupZone(ZoneId zone, SetupType type) {
	error("[ApplicationJerusalem::setupZone] Not implemented");
}

#pragma endregion

} // End of namespace Ring
