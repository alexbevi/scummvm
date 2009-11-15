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

#include "common/stream.h"

#include "common/config-manager.h"

#include "engines/darkseed2/music.h"
#include "engines/darkseed2/resources.h"

namespace DarkSeed2 {

Music::Music(Audio::Mixer &mixer) : _mixer(&mixer) {
	syncSettings();
}

Music::~Music() {
	stop();
}

bool Music::playMID(Common::SeekableReadStream &wav) {
	if (_mute)
		return true;

	return true;
}

bool Music::playMID(const Resource &resource) {
	return playMID(resource.getStream());
}

void Music::syncSettings() {
	// Getting conf values
	int volumeMusic = ConfMan.getInt("music_volume");
	bool muteMusic  = ConfMan.getBool("music_mute");
	bool mute       = ConfMan.getBool("mute");

	// Looking for muted music
	_mute = false;
	if (muteMusic || mute)
		volumeMusic = 0;
	if (volumeMusic == 0)
		_mute = true;

	// Setting values
	_mixer->setVolumeForSoundType(Audio::Mixer::kMusicSoundType, volumeMusic);
}

void Music::stop() {
}

} // End of namespace DarkSeed2
