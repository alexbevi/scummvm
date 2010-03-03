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

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/versionformats.h"

namespace DarkSeed2 {

const char *VersionFormats::kImageExtensions[]   = {"BMP", "RGB", "BDP", "256"};
const char *VersionFormats::kWalkMapExtensions[] = {"BMP", "MAP"};
const char *VersionFormats::kSoundExtensions[]   = {"WAV", "AIF"};

VersionFormats::VersionFormats() {
	_gameVersion = kGameVersionWindows;

	_language = Common::UNK_LANG;

	_imageType     = kImageTypeBMP;
	_roomImageType = kImageTypeBMP;
	_boxImageType  = kImageTypeBMP;

	_walkMapType = kWalkMapTypeBMP;

	_soundType = kSoundTypeWAV;
}

void VersionFormats::setGameVersion(GameVersion gameVersion) {
	_gameVersion = gameVersion;

	switch (_gameVersion) {
	case kGameVersionWindows:
		_imageType     = kImageTypeBMP;
		_roomImageType = kImageTypeBMP;
		_boxImageType  = kImageTypeBMP;
		_walkMapType   = kWalkMapTypeBMP;
		_soundType     = kSoundTypeWAV;
		break;

	case kGameVersionSaturn:
		_imageType     = kImageTypeRGB;
		_roomImageType = kImageTypeBDP;
		_boxImageType  = kImageType256;
		_walkMapType   = kWalkMapTypeMAP;
		_soundType     = kSoundTypeAIF;
		break;

	default:
		warning("VersionFormats::setGameVersion(): Unknown game version");
		assert(false);
	}
}

GameVersion VersionFormats::getGameVersion() const {
	return _gameVersion;
}

void VersionFormats::setLanguage(Common::Language language) {
	_language = language;
}

Common::Language VersionFormats::getLanguage() const {
	return _language;
}

ImageType VersionFormats::getImageType() const {
	return _imageType;
}

ImageType VersionFormats::getRoomImageType() const {
	return _roomImageType;
}

ImageType VersionFormats::getBoxImageType() const {
	return _boxImageType;
}

WalkMapType VersionFormats::getWalkMapType() const {
	return _walkMapType;
}

SoundType VersionFormats::getSoundType() const {
	return _soundType;
}

const char *VersionFormats::getImageExtension(ImageType imageType) const {
	assert((imageType >= 0) && (imageType < ARRAYSIZE(kImageExtensions)));

	return kImageExtensions[imageType];
}

const char *VersionFormats::getWalkMapExtension(WalkMapType walkMapType) const {
	assert((walkMapType >= 0) && (walkMapType < ARRAYSIZE(kWalkMapExtensions)));

	return kWalkMapExtensions[walkMapType];
}

const char *VersionFormats::getSoundExtension(SoundType soundType) const {
	assert((soundType >= 0) && (soundType < ARRAYSIZE(kSoundExtensions)));

	return kSoundExtensions[soundType];
}

} // End of namespace DarkSeed2
