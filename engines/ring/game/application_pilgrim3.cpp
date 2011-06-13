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

#include "ring/game/application_pilgrim3.h"

#include "ring/base/art.h"
#include "ring/base/bag.h"
#include "ring/base/preferences.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"
#include "ring/base/sound.h"

#include "ring/game/event_pilgrim3.h"

#include "ring/graphics/screen.h"

#include "common/textconsole.h"

//using namespace Pilgrim3Game;

namespace Ring {

ApplicationPilgrim3::ApplicationPilgrim3(RingEngine *engine) : Application(engine) {
	_eventHandler = new EventHandlerPilgrim3(this);

	_systemZone = kZone100;
}

ApplicationPilgrim3::~ApplicationPilgrim3() {
}

#pragma region Game setup

void ApplicationPilgrim3::initLanguages() {
	languageAdd(kLanguageEnglish, "ENG", "ENG", 1);
	languageAdd(kLanguageFrench,  "FRA", "FRA", 1);
	languageAdd(kLanguageGerman,  "GER", "GER", 1);
	languageAdd(kLanguageItalian, "ITA", "ITA", 1);
	languageAdd(kLanguageSpanish, "SPA", "SPA", 1);
	languageAdd(kLanguageDutch,   "HOL", "HOL", 1);
	languageAdd(kLanguageSwedish, "SWE", "SWE", 1);
}

void ApplicationPilgrim3::initFont() {
	// Original asks for size 12, but the font only contains size 8
	fontAdd(kFontDefault, "secrets.fon", "Arxel1", 8, true, false, false, false, getCurrentLanguage());
}

void ApplicationPilgrim3::setup() {
	error("[ApplicationPilgrim3::setup] Not implemented");
}

void ApplicationPilgrim3::initData() {
	_field_74 = true;
	_field_75 = true;
	_field_76 = true;
	_field_77 = false;
	_field_78 = true;
}

void ApplicationPilgrim3::initBag() {
	_bag->setOrigin(Common::Point(0, 0));
	_bag->sub_417D40(15, 26, 40, 61);
	_bag->setBackgroundOffset(Common::Point(0, 0));
	_bag->sub_417DD0(10);
	_bag->sub_417D80(0, 0, 30, 448);
	_bag->sub_417DA0(610, 0, 30, 448);
	_bag->sub_4192A0(6, 12);
	_bag->sub_4192C0(622, 12);
	_bag->sub_417DE0(506, 0);
	_bag->sub_419280(500);
	_bag->loadBackground("bagbgr.tga", "", "", "", "", "", "", "bag_h.bmp", _archiveType);
	_bag->initHotspots();
}

#pragma endregion

#pragma region Startup

void ApplicationPilgrim3::showStartupScreen() {
	error("[ApplicationPilgrim3::showStartupScreen] Not implemented");
}

void ApplicationPilgrim3::startMenu(bool savegame) {
	error("[ApplicationPilgrim3::startMenu] Not implemented");
}

void ApplicationPilgrim3::showMenu(ZoneId zone, MenuAction menuAction) {
	error("[ApplicationPilgrim3::showMenu] Not implemented");
}

void ApplicationPilgrim3::showCredits() {
	error("[ApplicationPilgrim3::showCredits] Not implemented");
}

void ApplicationPilgrim3::loadPreferences() {
	error("[ApplicationPilgrim3::loadPreferences] Not implemented");
}

#pragma endregion

#pragma region Drawing

void ApplicationPilgrim3::draw() {
	// Update our screen
	_screenManager->updateScreen();

	// Update engine state
	error("[ApplicationPilgrim3::draw] Engine state update not implemented!");
}

#pragma endregion

#pragma region Visual

Visual *ApplicationPilgrim3::createVisual(Id visualId, uint32 a3, uint32 a4, uint32 left, uint32 top, uint32 offsetY, uint32 height, uint32 progressMultiplier, uint32 progressColor) {
	error("[ApplicationPilgrim3::createVisual] Engine state update not implemented!");
}

#pragma endregion

#pragma region Zone initialization

void ApplicationPilgrim3::initZones() {
	error("[ApplicationPilgrim3::initZones] Not implemented");
}

void ApplicationPilgrim3::setupZone(ZoneId zone, SetupType type) {
	error("[ApplicationPilgrim3::setupZone] Not implemented");
}

#pragma endregion

} // End of namespace Ring
