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

#include "gargoyle/frotz_sound.h"

// Minimum size of a sound file (the header itself
#define MIN_HEADER_SIZE 10

namespace Frotz {

/**
 * Check to see if the game supports sound
 */
FrotzSound *FrotzSound::init(Gargoyle::BlorbFile *blorbFile, const Common::FSNode &soundFolder) {
	if (!soundFolder.isDirectory() && !blorbFile)
		return NULL;

	return new FrotzSound(blorbFile, soundFolder);
}

FrotzSound::FrotzSound(Gargoyle::BlorbFile *bf, const Common::FSNode &soundFolder):
		_blorbFile(bf), _soundFolder(soundFolder) {
	_currentSample = -1;
	_sampleData = NULL;

	// Get the sound file prefix by finding the name of the first .SND file in the folder
	_gamePrefix = NULL;
	Common::FSList fsList;
	if (_soundFolder.getChildren(fsList, Common::FSNode::kListFilesOnly, false)) {
		for (Common::FSList::const_iterator file = fsList.begin(); file != fsList.end(); ++file) {
			Common::String filename = file->getName();
			filename.toLowercase();

			const char *fname = filename.c_str();
			if (strstr(fname, ".snd")) {
				// Get the filename up to the first digit (which will be the sound number
				const char *p = fname;
				while (*p && ((*p < '0') || (*p > '9'))) ++p;

				_gamePrefix = new Common::String(fname, p - fname);
				break;
			}
		}
	}
}

FrotzSound::~FrotzSound() {
	if (_gamePrefix) delete _gamePrefix;
	if (_sampleData) delete _sampleData;
}

void FrotzSound::loadSample(int soundNumber) {
    char sampleStr[5];
	Common::File fp;
	uint16 filelen;

    if (_currentSample == soundNumber)
		return;

	_has_played_sound = true;

    resetSample();

	if (!_soundFolder.isDirectory())
		error("Sound not supported");

    // Build sample file name
	Common::String fname = *_gamePrefix;
	sampleStr[0] = '0' + soundNumber / 10;
	sampleStr[1] = '0' + soundNumber % 10;
	sampleStr[2] = '\0';
	fname += sampleStr;
	fname += ".snd";

    // Open sample file
	Common::FSNode sndFile = _soundFolder.getChild(fname);
	if (!fp.open(sndFile))
		error("Could not locate sound number %d", soundNumber);

	// Find out how long the file is for error checking later
	filelen = fp.size();

	// If it's shorter than this, it has to be corrupt (minimum size of sound header)
	if (filelen < MIN_HEADER_SIZE) {
		fp.close();
		error("Sound file %s is too short to contain a sound", fname.c_str());
	}

	// Header fields are in BE format
	_sheader.prefix = fp.readUint16BE();

	if (_sheader.prefix > filelen)
		error("Sound file %s appears to be short", fname.c_str());

	_sheader.repeats   = fp.readByte();
	_sheader.base_note = fp.readByte();
	_sheader.frequency = fp.readUint16BE();

	// Skip the unused two bytes
	fp.readUint16BE();

	_sheader.length = fp.readUint16BE();

	if (fp.eos())
		error("EOF reached reading header of sound file %s", fname.c_str());

    // Allocate memory for sample data
	_sampleData = (byte *)malloc(_sheader.length);
	if (!_sampleData)
		error("Not enough memory for sound");

	// Read sample data
	if (fp.read(_sampleData, _sheader.length) != _sheader.length)
		error("Error reading sound data from file %s", fname.c_str());

	// Store sample number
	_currentSample = soundNumber;

    // Close sample file
	fp.close();
}

void FrotzSound::play(int soundNumber, int repeat, int volume, unsigned short eos) {
	loadSample(soundNumber);
	warning("TODO: Play sound %d", soundNumber);
}

void FrotzSound::resetSample() {
	if (_sampleData) {
		free(_sampleData);
		_sampleData = NULL;
	}
}

} // end of namespace Frotz
