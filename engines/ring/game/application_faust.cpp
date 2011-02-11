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

void ApplicationFaust::showMenu(Zone zone, uint32 a2) {
	error("[ApplicationFaust::startMenu] Not implemented");
}

void ApplicationFaust::showCredits() {
	error("[ApplicationFaust::showCredits] Not implemented");
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

#pragma region Zone full names, short string and ReadFrom

Common::String ApplicationFaust::getZoneString(Zone zone) const {
	switch (zone) {
	default:
		break;

	case kZoneSY:
		return "sy";
	}

	error("[Application::getZone] Invalid zone (%d)", zone);
}

Common::String ApplicationFaust::getZoneLongName(Zone zone) const {
	switch (zone) {
	default:
		break;

	case kZoneSY:
		return "";
	}

	error("[Application::getZoneName] Invalid zone (%d)", zone);
}

ArchiveType ApplicationFaust::getReadFrom(Zone zone) const {
	if (_archiveType == kArchiveFile)
		return kArchiveFile;

	switch (zone) {
	default:
		break;

	case kZoneSY:
		return _configuration.artSY ? kArchiveArt : kArchiveFile;
	}

	error("[ApplicationFaust::getReadFrom] Invalid zone (%d)", zone);
}

#pragma endregion

#pragma region Zone initialization

void ApplicationFaust::initZones() {
	error("[ApplicationFaust::initZones] Not implemented");
}

#pragma endregion

} // End of namespace Ring
