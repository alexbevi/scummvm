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

#include "engines/engine.h"

#include "common/archive.h"
#include "common/config-manager.h"

namespace Ring {

PreferenceHandler::PreferenceHandler(Application *application) : _app(application) {
	_volume = 0;
	_volumeDialog = 0;
	_reverseStereo = 0;
	_showSubtitles = false;
}

PreferenceHandler::~PreferenceHandler() {
	// Zero-out passed pointers
	_app = NULL;
}

void PreferenceHandler::load() {
	loadDefaults();

	// Register default values
	ConfMan.registerDefault("music_volume", _volume);
	ConfMan.registerDefault("speech_volume", _volumeDialog);
	ConfMan.registerDefault("subtitles", _showSubtitles);
	ConfMan.registerDefault("reverse_stereo", _reverseStereo);

	// Get values from ScummVM preferences
	_volume = ConfMan.getInt("music_volume");
	_volumeDialog = ConfMan.getInt("speech_volume");
	_showSubtitles = ConfMan.getBool("subtitles");
	_reverseStereo = ConfMan.getInt("reverse_stereo");
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
	int32 volume;
	int32 volumeDialog;
	int32 reverseStereo;
	int32 showSubtitles;
	if (sscanf(line.c_str(), "%d %d %d %d", &volume, &volumeDialog, &reverseStereo, &showSubtitles) != 4)
		error("[PreferenceHandler::load] Invalid configuration format");

	set(volume, volumeDialog, reverseStereo, showSubtitles);
	setup();
}

void PreferenceHandler::save(int32 volume, int32 volumeDialog, int32 reverseStereo, bool showSubtitles) {
	set(volume, volumeDialog, reverseStereo, showSubtitles);
	setup();

	ConfMan.setInt("music_volume", _volume);
	ConfMan.setInt("speech_volume", _volumeDialog);
	ConfMan.setBool("subtitles", _showSubtitles);
	ConfMan.setBool("reverse_stereo", _reverseStereo);
}

void PreferenceHandler::set(int32 volume, int32 volumeDialog, int32 reverseStereo, bool showSubtitles) {
	_volume = volume;
	_volumeDialog = volumeDialog;
	_reverseStereo = reverseStereo;
	_showSubtitles = showSubtitles;
}

void PreferenceHandler::setup() {
	_app->soundSetMultiplierIfNotType(kSoundTypeDialog, _volume);
	_app->soundSetMultiplier(kSoundTypeDialog, _volumeDialog);

	if (_app->getSoundHandler())
		_app->getSoundHandler()->setReverseStereo(_reverseStereo);

	if (_app->getDialogHandler())
		_app->getDialogHandler()->showSubtitles(_showSubtitles);

	g_engine->syncSoundSettings();
}

} // End of namespace Ring
