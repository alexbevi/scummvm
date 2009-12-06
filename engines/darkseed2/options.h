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

#ifndef DARKSEED2_OPTIONS_H
#define DARKSEED2_OPTIONS_H

#include "engines/darkseed2/darkseed2.h"

namespace DarkSeed2 {

/** Global game options. */
class Options {
public:
	Options();
	~Options();

	/** Sync with ScummVM's settings. */
	void syncSettings();

	int getVolumeSFX() const;    ///< Get the SFX volume.
	int getVolumeSpeech() const; ///< Get the speech volume.
	int getVolumeMusic() const;  ///< Get the music volume.

	int  getSubtitleSpeed() const;    ///< Get the subtitle speed.
	bool getSubtitlesEnabled() const; ///< Are the subtitles enabled?

private:
	int _volumeSFX;    ///< SFX volume.
	int _volumeSpeech; ///< Speech volume.
	int _volumeMusic;  ///< Music volume.

	int  _subtitleSpeed;    ///< Subtitle speed.
	bool _subtitlesEnabled; ///< Subtitles enabled?
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_OPTIONS_H
