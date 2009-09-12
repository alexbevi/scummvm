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
#include "common/events.h"
#include "common/file.h"

#include "base/plugins.h"
#include "base/version.h"

#include "startrek/lzss.h"
#include "startrek/startrek.h"

namespace StarTrek {

StarTrekEngine::StarTrekEngine(OSystem *syst, const StarTrekGameDescription *gamedesc) : Engine(syst), _gameDescription(gamedesc) {
}

StarTrekEngine::~StarTrekEngine() {
	delete _gfx;
	delete _sound;
}

Common::Error StarTrekEngine::run() {
	_gfx = new Graphics(this);
	_sound = new Sound(this);

	initGraphics(320, 200, false);
	
// Hexdump data
#if 0
	Common::SeekableReadStream *stream = openFile("BRIDGE.PAL");
	byte *data = (byte *)malloc(stream->size());
	stream->read(data, stream->size());
	Common::hexdump(data, stream->size());
	free(data);
	delete stream;
#endif

// Test graphics/music:

// Music Status:
// DOS Full: Adlib and MT-32 Sounds supported
// DOS Demo: Adlib and MT-32 Sounds supported
// Amiga: Sound effects supported
// Macintosh: MIDI and sound effects playable, format not handled.

// Graphics Status:
// DOS/Amiga/Macintosh/Demo Graphics: 100%
// Judgment Rites Backgrounds supported too
// EGA not supported
#if 1
	if (getGameType() == GType_ST25) {
		if (getPlatform() == Common::kPlatformMacintosh) {
			_gfx->setPalette("BRIDGES.PAL");
			_gfx->drawImage("BRIDGE0.BMP");
		} else {
			_gfx->setPalette("BRIDGE.PAL");
			//_gfx->loadEGAData("BRIDGE.EGA");
			_gfx->drawImage("BRIDGE.BMP");
		}
		
		if (getPlatform() == Common::kPlatformAmiga)
			_sound->playSoundEffect("TREK2");
		else if (getFeatures() & GF_DEMO)
			_sound->playSound("STTITLE");
		else
			_sound->playSound("TITLE");
	} else {
		_gfx->drawBackgroundImage("BRIDGE.BGD");
	}
	
	Common::Event event;
	
	while (!shouldQuit()) {
		while (_eventMan->pollEvent(event)) {
			switch (event.type) {
				case Common::EVENT_QUIT:
					_system->quit();
					break;
				default:
					break;
			}
		}
	}
#endif

	return Common::kNoError;
}

Common::SeekableReadStream *StarTrekEngine::openFile(Common::String filename) {
	// The Judgment Rites demo has its files not in the standard archive
	if (getGameType() == GType_STJR && (getFeatures() & GF_DEMO)) {
		Common::File *file = new Common::File();
		if (!file->open(filename.c_str()))
			error ("Could not find file \'%s\'", filename.c_str());
		return file;
	}

	Common::File indexFile;
	if (getPlatform() == Common::kPlatformAmiga) {
		if (!indexFile.open("data000.dir"))
			error ("Could not open data000.dir");
	} else if (getPlatform() == Common::kPlatformMacintosh) {
		if (!indexFile.open("._Star Trek Data"))
			error ("Could not open Star Trek Directory");
		// The Directory file is just the Resource Fork of the Star Trek Data file. Let's get to the actual directory part of it.
		// TODO: Use resource file info instead of just seeking to the directory
		indexFile.seek(0x3ad7);
	} else {
		if (!indexFile.open("data.dir"))
			error ("Could not open data.dir");
	}
	
	uint32 indexOffset = 0;
	bool foundData = false;
	uint16 fileCount = 1;
	uint16 uncompressedSize = 0;
	
	while (!indexFile.eos() && !indexFile.err()) {
		Common::String testfile = Common::String::emptyString;
		for (byte i = 0; i < 8; i++) {
			char c = indexFile.readByte();
			if (c)
				testfile += c;
		}
		testfile += '.';
	
		for (byte i = 0; i < 3; i++)
			testfile += indexFile.readByte();
		
		if (getFeatures() & GF_DEMO) {
			indexFile.readByte(); // Always 0?
			fileCount = indexFile.readUint16LE(); // Always 1
			indexOffset = indexFile.readUint32LE();
			uncompressedSize = indexFile.readUint16LE();
		} else {
			if (getPlatform() == Common::kPlatformAmiga)
				indexOffset = (indexFile.readByte() << 16) + (indexFile.readByte() << 8) + indexFile.readByte();	
			else 
				indexOffset = indexFile.readByte() + (indexFile.readByte() << 8) + (indexFile.readByte() << 16);
			
			if (indexOffset & (1 << 23)) {
				fileCount = (indexOffset >> 16) & 0x7F;
				indexOffset = indexOffset & 0xFFFF;
			
				// TODO: Replace necessary number with ?
			} else {
				fileCount = 1;
			}
		}
		
		if (filename.matchString(testfile)) {
			foundData = true;
			break;
		}		
	}
	
	indexFile.close();
	
	if (!foundData)
		error ("Could not find file \'%s\'", filename.c_str());
	
		
	Common::File dataFile;
	if (getPlatform() == Common::kPlatformAmiga) {
		if (!dataFile.open("data.000"))
			error ("Could not open data.000");
	} else if (getPlatform() == Common::kPlatformMacintosh) {
		if (!dataFile.open("Star Trek Data"))
			error ("Could not open Star Trek Data");
	} else {
		if (!dataFile.open("data.001"))
			error ("Could not open data.001");
	}
		
	dataFile.seek(indexOffset);
	
	if (getFeatures() & GF_DEMO) {
		assert(fileCount == 1); // Sanity check...
		Common::SeekableReadStream *stream = dataFile.readStream(uncompressedSize);
		dataFile.close();
		return stream;
	} else {
		uint16 fileIndex = 0;
	
		// TODO!
		//if (fileCount > 1)
		//	fileIndex = filename.lastChar() - '0';
		if (fileCount != 1)
			error ("Multi-part files not yet handled");
	
		for (uint16 i = 0; i < fileCount; i++) {
			uncompressedSize = (getPlatform() == Common::kPlatformAmiga) ? dataFile.readUint16BE() : dataFile.readUint16LE();
			uint16 compressedSize = (getPlatform() == Common::kPlatformAmiga) ? dataFile.readUint16BE() : dataFile.readUint16LE();
			if (i == fileIndex) {
				debug(0, "Opening file \'%s\'\n", filename.c_str());
				Common::SeekableReadStream *stream = decodeLZSS(dataFile.readStream(compressedSize), uncompressedSize);
				dataFile.close();
				return stream;
			} else {
				dataFile.seek(compressedSize, SEEK_CUR);
			}
		}
	}
	
	// We should not get to this point...
	error ("Could not find data for \'%s\'", filename.c_str());
		
	return NULL;
}

byte getStartingIndex(Common::String filename) {
	// Find last number
	int32 lastNumIndex = -1;
	for (uint32 i = 0; i < filename.size(); i++) {
		if (filename[i] >= '0' && filename[i] <= '9')
			lastNumIndex = i;
		else if (filename[i] == '.')
			break;
	}
	
	if (lastNumIndex == -1)
		return 0;
	return (filename[lastNumIndex] - '0');
}

} // End of namespace StarTrek
