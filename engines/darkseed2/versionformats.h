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

#ifndef DARKSEED2_VERSIONFORMATS_H
#define DARKSEED2_VERSIONFORMATS_H

#include "common/util.h"

namespace DarkSeed2 {

enum GameVersion {
	kGameVersionWindows = 0, ///< The Windows PC version.
	kGameVersionSaturn  = 1  ///< The Sega Saturn version.
};

/** An image type. */
enum ImageType {
	kImageTypeBMP = 0, ///< BMP images.
	kImageTypeRGB = 1, ///< RGB images.
	kImageTypeBDP = 2, ///< BDP images.
	kImageType256 = 3  ///< 256 images.
};

enum WalkMapType {
	kWalkMapTypeBMP = 0, ///< Walk map in BMP image.
	kWalkMapTypeMAP = 1  ///< Raw walk map data in a MAP file.
};

enum SoundType {
	kSoundTypeWAV = 0, ///< WAV sounds.
	kSoundTypeAIF = 1  ///< AIF sounds.
};

class VersionFormats {
public:
	VersionFormats();

	void setGameVersion(GameVersion gameVersion);
	GameVersion getGameVersion() const;

	void setLanguage(Common::Language language);
	Common::Language getLanguage() const;

	/** Get the type of images the game uses. */
	ImageType getImageType() const;

	/** Get the type of images the game uses for rooms. */
	ImageType getRoomImageType() const;

	/** Get the type of images the game uses for boxes. */
	ImageType getBoxImageType() const;

	/** Get the type of file the game uses for walk maps. */
	WalkMapType getWalkMapType() const;

	/** Get the type of file the game uses for walk maps. */
	SoundType getSoundType() const;

	/** Get the extension used for images by the game. */
	const char *getImageExtension(ImageType imageType) const;
	/** Get the extension used for walk maps by the game. */
	const char *getWalkMapExtension(WalkMapType walkMapType) const;
	/** Get the extension used for sounds by the game. */
	const char *getSoundExtension(SoundType soundType) const;

private:
	static const char *kImageExtensions[];
	static const char *kWalkMapExtensions[];
	static const char *kSoundExtensions[];

	GameVersion _gameVersion; ///< The game version.

	Common::Language _language; ///< The game's language.

	ImageType _imageType;     ///< The type of images the game uses for images.
	ImageType _roomImageType; ///< The type of images the game uses for room images.
	ImageType _boxImageType;  ///< The type of images the game uses for box images.

	WalkMapType _walkMapType; ///< The type of file the game uses for walk maps.

	SoundType _soundType;     ///< The type of sounds the game uses.
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_VERSIONFORMATS_H
