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

#include "ring/game/application_pompeii.h"

#include "ring/game/event_pompeii.h"

#include "ring/graphics/screen.h"

//using namespace PompeiiGame;

namespace Ring {

ApplicationPompeii::ApplicationPompeii(RingEngine *engine) : Application(engine) {
	_eventHandler = new EventHandlerPompeii(this);
}

ApplicationPompeii::~ApplicationPompeii() {
}

#pragma region Game setup

void ApplicationPompeii::initFont() {
	// Original asks for size 12, but the font only contains size 8
	fontAdd(kFontDefault, "pompei.fon", "Arxel1", 8, true, false, false, false, getCurrentLanguage());
}

void ApplicationPompeii::setup() {
	error("[ApplicationPompeii::setup] Not implemented");
}

void ApplicationPompeii::initBag() {
	error("[ApplicationPompeii::initBag] Not implemented");
}

#pragma endregion

#pragma region Startup

void ApplicationPompeii::showStartupScreen() {
	error("[ApplicationPompeii::showStartupScreen] Not implemented");
}

void ApplicationPompeii::startMenu(bool savegame) {
	error("[ApplicationPompeii::startMenu] Not implemented");
}

void ApplicationPompeii::showMenu(ZoneId zone, uint32 a2) {
	error("[ApplicationPompeii::showMenu] Not implemented");
}

void ApplicationPompeii::showCredits() {
	error("[ApplicationPompeii::showCredits] Not implemented");
}

void ApplicationPompeii::loadPreferences() {
	error("[ApplicationPompeii::loadPreferences] Not implemented");
}

#pragma endregion

#pragma region Drawing

void ApplicationPompeii::draw() {
	// Update our screen
	_screenManager->updateScreen();

	// Update engine state
	error("[ApplicationPompeii::draw] Engine state update not implemented!");
}

#pragma endregion

#pragma region Visual

Visual *ApplicationPompeii::createVisual(Id visualId, uint32 a3, uint32 a4, uint32 left, uint32 top, uint32 offsetY, uint32 height, uint32 progressMultiplier, uint32 progressColor) {
	error("[ApplicationPompeii::createVisual] Engine state update not implemented!");
}

#pragma endregion

#pragma region Zone initialization

void ApplicationPompeii::initZones() {
	error("[ApplicationPompeii::initZones] Not implemented");
}

void ApplicationPompeii::setupZone(ZoneId zone, SetupType type) {
	error("[ApplicationPompeii::setupZone] Not implemented");
}


#pragma endregion

} // End of namespace Ring
