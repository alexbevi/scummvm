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
	error("[ApplicationPilgrim2::initFont] Not implemented");
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

void ApplicationPilgrim2::showMenu(Zone zone, uint32 a2) {
	error("[ApplicationPilgrim2::showMenu] Not implemented");
}

void ApplicationPilgrim2::showCredits() {
	error("[ApplicationPilgrim2::showCredits] Not implemented");
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

#pragma region Zone full names, short string and ReadFrom

Common::String ApplicationPilgrim2::getZoneString(Zone zone) const {
	switch (zone) {
	default:
		break;

	case kZoneSY:
		return "sy";
	}

	error("[Application::getZone] Invalid zone (%d)", zone);
}

Common::String ApplicationPilgrim2::getZoneLongName(Zone zone) const {
	switch (zone) {
	default:
		break;

	case kZoneSY:
		return "";
	}

	error("[Application::getZoneName] Invalid zone (%d)", zone);
}

ArchiveType ApplicationPilgrim2::getReadFrom(Zone zone) const {
	if (_archiveType == kArchiveFile)
		return kArchiveFile;

	switch (zone) {
	default:
		break;

	case kZoneSY:
		return _configuration.artSY ? kArchiveArt : kArchiveFile;
	}

	error("[ApplicationPilgrim2::getReadFrom] Invalid zone (%d)", zone);
}

#pragma endregion

#pragma region Zone initialization

void ApplicationPilgrim2::initZones() {
	error("[ApplicationPilgrim2::initZones] Not implemented");
}

#pragma endregion

} // End of namespace Ring
