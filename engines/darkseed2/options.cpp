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

#include "common/config-manager.h"

#include "engines/darkseed2/options.h"

namespace DarkSeed2 {

Options::Options() {
	syncSettings();
}

Options::~Options() {
}

void Options::syncSettings() {
	_volumeSFX    = ConfMan.getInt("sfx_volume");
	_volumeSpeech = ConfMan.getInt("speech_volume");
	_volumeMusic  = ConfMan.getInt("music_volume");

	bool muteSFX    = ConfMan.getBool("speech_mute");
	bool muteSpeech = ConfMan.getBool("sfx_mute");
	bool muteMusic  = ConfMan.getBool("music_mute");
	bool mute       = ConfMan.getBool("mute");

	if (muteSFX    || mute)
		_volumeSFX    = 0;
	if (muteSpeech || mute)
		_volumeSpeech = 0;
	if (muteMusic  || mute)
		_volumeMusic  = 0;
}

int Options::getVolumeSFX() const {
	return _volumeSFX;
}

int Options::getVolumeSpeech() const {
	return _volumeSpeech;
}

int Options::getVolumeMusic() const {
	return _volumeMusic;
}

} // End of namespace DarkSeed2
