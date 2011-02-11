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

#include "ring/base/preferences.h"

#include "ring/base/application.h"
#include "ring/base/dialog.h"
#include "ring/base/sound.h"

#include "common/archive.h"

namespace Ring {

PreferenceHandler::PreferenceHandler(Application *application) : _app(application) {
	_pref1 = 0;
	_pref2 = 0;
	_reverseStereo = 0;
	_pref4 = 0;
}

PreferenceHandler::~PreferenceHandler() {
	// Zero-out passed pointers
	_app = NULL;
}

void PreferenceHandler::load() {
	loadDefaults();

	warning("[PreferenceHandler::load] Loading from ScummVM preferences not implemented");
}

void PreferenceHandler::loadDefaults() {
	// Open a stream to the configuration file
	Common::SeekableReadStream *archive = SearchMan.createReadStreamForMember("aPre.ini");
	if (!archive)
		error("[PreferenceHandler::load] Error opening configuration file (aPre.ini)");

	Common::String line = archive->readLine();
	if (archive->err()) {
		delete archive;
		error("[PreferenceHandler::load] Error reading from file");
	}

	delete archive;

	// Read preferences
	int32 pref1;
	int32 pref2;
	int32 reverseStereo;
	int32 pref4;
	if (sscanf(line.c_str(), "%d %d %d %d", &pref1, &pref2, &reverseStereo, &pref4) != 4)
		error("[PreferenceHandler::load] Invalid configuration format");

	set(pref1, pref2, reverseStereo, pref4);
	setup();
}

void PreferenceHandler::save() {
	error("[PreferenceHandler::save] No implemented");
}

void PreferenceHandler::set(int32 pref1, int32 pref2, int32 reverseStereo, int32 pref4) {
	_pref1 = pref1;
	_pref2 = pref2;
	_reverseStereo = reverseStereo;
	_pref4 = pref4;
}

void PreferenceHandler::setup() {
	warning("[PreferenceHandler::setup] Sound volume setup not implemented");

	if (_app->getSoundHandler())
		_app->getSoundHandler()->setReverseStereo(_reverseStereo);

	if (_app->getDialogHandler())
		_app->getDialogHandler()->setField28(_pref4 ? 1 : 0);
}

} // End of namespace Ring
