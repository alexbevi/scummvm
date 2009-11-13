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

#include "common/endian.h"
#include "common/md5.h"

#include "base/plugins.h"

#include "common/config-manager.h"

#include "sound/mixer.h"
#include "sound/mididrv.h"

#include "darkseed2/darkseed2.h"
#include "darkseed2/resources.h"

namespace DarkSeed2 {

DarkSeed2Engine::DarkSeed2Engine(OSystem *syst) : Engine(syst) {
	Common::addDebugChannel(kDebugResources, "Resources", "Resource handling debug level");

	// Setup mixer
	_mixer->setVolumeForSoundType(Audio::Mixer::kSFXSoundType, ConfMan.getInt("sfx_volume"));
	_mixer->setVolumeForSoundType(Audio::Mixer::kMusicSoundType, ConfMan.getInt("music_volume"));

	_resources = 0;
}

DarkSeed2Engine::~DarkSeed2Engine() {
	delete _resources;
}

Common::Error DarkSeed2Engine::run() {
	_resources = new Resources();

	if (!_resources->index("gfile.hdr")) {
		warning("Couldn't index resources");
		return Common::kUnknownError;
	}

	warning("-> %d", _resources->hasResource("Foobar"));
	warning("-> %d", _resources->hasResource("002BTN01.BMP"));
	warning("-> %d", _resources->hasResource("DJG001.WAV"));
	return Common::kNoError;
}

void DarkSeed2Engine::pauseEngineIntern(bool pause) {
	_mixer->pauseAll(pause);
}

void DarkSeed2Engine::syncSoundSettings() {
	Engine::syncSoundSettings();
}

} // End of namespace DarkSeed2
