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

void ApplicationPompeii::setup() {
	error("[ApplicationPompeii::setup] Not implemented");
}

#pragma endregion

#pragma region Startup

void ApplicationPompeii::showStartupScreen() {
	error("[ApplicationPompeii::showStartupScreen] Not implemented");
}

void ApplicationPompeii::startMenu(bool savegame) {
	error("[ApplicationPompeii::startMenu] Not implemented");
}

void ApplicationPompeii::showCredits() {
	error("[ApplicationPompeii::showCredits] Not implemented");
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

#pragma region Zone full names, short string and ReadFrom

Common::String ApplicationPompeii::getZoneString(Zone zone) const {
	switch (zone) {
	default:
		break;

	case kZoneSY:
		return "sy";
	}

	error("[Application::getZone] Invalid zone (%d)", zone);
}

Common::String ApplicationPompeii::getZoneLongName(Zone zone) const {
	switch (zone) {
	default:
		break;

	case kZoneSY:
		return "";
	}

	error("[Application::getZoneName] Invalid zone (%d)", zone);
}

ArchiveType ApplicationPompeii::getReadFrom(Zone zone) const {
	if (_archiveType == kArchiveFile)
		return kArchiveFile;

	switch (zone) {
	default:
		break;

	case kZoneSY:
		return _configuration.artSY ? kArchiveArt : kArchiveFile;
	}

	error("[ApplicationPompeii::getReadFrom] Invalid zone (%d)", zone);
}

#pragma endregion

#pragma region Zone initialization

void ApplicationPompeii::initZones() {
	error("[ApplicationPompeii::initZones] Not implemented");
}

#pragma endregion

} // End of namespace Ring
