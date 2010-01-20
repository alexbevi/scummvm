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
 
#include "startrek/sound.h"

#include "common/file.h"

#include "sound/mods/protracker.h"
#include "sound/raw.h"

namespace StarTrek {

// Main Sound Functions

Sound::Sound(StarTrekEngine *vm) : _vm(vm) {
	if (_vm->getPlatform() == Common::kPlatformPC || _vm->getPlatform() == Common::kPlatformMacintosh) {
		// The main PC versions use XMIDI. ST25 Demo and Macintosh versions use SMF.
		if ((_vm->getGameType() == GType_ST25 && _vm->getFeatures() & GF_DEMO) || _vm->getPlatform() == Common::kPlatformMacintosh)
			_midiParser = MidiParser::createParser_SMF();
		else
			_midiParser = MidiParser::createParser_XMIDI();
			
		_midiDriverType = MidiDriver::detectMusicDriver(MDT_PCSPK|MDT_ADLIB|MDT_MIDI);
		_midiDriver = MidiDriver::createMidi(_midiDriverType);
		_midiDriver->open();
		_midiParser->setMidiDriver(_midiDriver);
		_midiParser->setTimerRate(_midiDriver->getBaseTempo());
	}

	_soundHandle = new Audio::SoundHandle();
}

Sound::~Sound() {
	delete _midiParser;
	delete _midiDriver;
	delete _soundHandle;
}

void Sound::playSound(const char *baseSoundName) {
	if (_vm->getPlatform() == Common::kPlatformAmiga)
		playAmigaSound(baseSoundName);
	else if (_vm->getPlatform() == Common::kPlatformMacintosh)
		playMacSMFSound(baseSoundName);
	else if (_vm->getFeatures() & GF_DEMO)
		playSMFSound(baseSoundName);
	else
		playXMIDISound(baseSoundName);
}

void Sound::playSoundEffect(const char *baseSoundName) {
	if (_vm->getPlatform() == Common::kPlatformAmiga)
		playAmigaSoundEffect(baseSoundName);
	else if (_vm->getPlatform() == Common::kPlatformMacintosh)
		playMacSoundEffect(baseSoundName);
	else
		error ("PC Sound Effects Not Supported");
}

// PC Functions

void Sound::playSMFSound(const char *baseSoundName) {
	Common::String soundName = baseSoundName;
	
	soundName += '.';
	
	switch (_midiDriverType) {
		case MD_MT32:
			soundName += "ROL";
			break;
		case MD_PCSPK:
			return; // Not supported...
		default:
			soundName += "ADL";
			break;
	}
	
	debug(0, "Playing sound \'%s\'\n", soundName.c_str());
	Common::SeekableReadStream *soundStream = _vm->openFile(soundName.c_str());
	
	byte *soundData = (byte *)malloc(soundStream->size());
	soundStream->read(soundData, soundStream->size());
	_midiParser->loadMusic(soundData, soundStream->size());
	delete soundStream;
	
	_midiDriver->setTimerCallback(_midiParser, MidiParser::timerCallback);
}

void Sound::playXMIDISound(const char *baseSoundName) {
	Common::String soundName = baseSoundName;
	
	soundName += '.';
	
	switch (_midiDriverType) {
		case MD_MT32:
			soundName += "MT";
			break;
		case MD_PCSPK:
			soundName += "PC";
			break;
		default:
			soundName += "AD";
			break;
	}
	
	debug(0, "Playing sound \'%s\'\n", soundName.c_str());
	Common::SeekableReadStream *soundStream = _vm->openFile(soundName.c_str());
	
	byte *soundData = (byte *)malloc(soundStream->size());
	soundStream->read(soundData, soundStream->size());
	_midiParser->loadMusic(soundData, soundStream->size());
	delete soundStream;
	
	_midiDriver->setTimerCallback(_midiParser, MidiParser::timerCallback);
}

// Amiga Functions

void Sound::playAmigaSound(const char *baseSoundName) {
	// Nope, this is wrong... see http://protracker.de/files/amiga/formats/theplayer_41_format.txt
#if 0
	Common::String soundName = baseSoundName;
	soundName += ".SNG";
	if (_vm->_mixer->isSoundHandleActive(*_soundHandle))
		_vm->_mixer->stopHandle(*_soundHandle);
	_vm->_mixer->playInputStream(Audio::Mixer::kMusicSoundType, _soundHandle, Audio::makeProtrackerStream(_vm->openFile(soundName.c_str())));
#endif
}

void Sound::playAmigaSoundEffect(const char *baseSoundName) {
	Common::String soundName = baseSoundName;
	soundName += ".SFX";
	if (_vm->_mixer->isSoundHandleActive(*_soundHandle))
		_vm->_mixer->stopHandle(*_soundHandle);
	Common::SeekableReadStream *stream = _vm->openFile(soundName.c_str());
	byte *data = (byte *)malloc(stream->size());
	stream->read(data, stream->size());
	Audio::AudioStream *audStream = (Audio::AudioStream *)Audio::makeRawMemoryStream(data, stream->size(), DisposeAfterUse::YES, 11025, 0);
	_vm->_mixer->playInputStream(Audio::Mixer::kSFXSoundType, _soundHandle, audStream);
	delete stream;
}

// Macintosh Functions

void Sound::playMacSMFSound(const char *baseSoundName) {
	Common::SeekableReadStream *soundStream = getMacintoshFile(baseSoundName);
	byte *soundData = (byte *)malloc(soundStream->size());
	soundStream->read(soundData, soundStream->size());
	_midiParser->loadMusic(soundData, soundStream->size());
	delete soundStream;
	
	_midiDriver->setTimerCallback(_midiParser, MidiParser::timerCallback);
}

void Sound::playMacSoundEffect(const char *baseSoundName) {
	Common::SeekableReadStream *stream = getMacintoshFile(baseSoundName);
	if (_vm->_mixer->isSoundHandleActive(*_soundHandle))
		_vm->_mixer->stopHandle(*_soundHandle);
	byte *data = (byte *)malloc(stream->size());
	stream->read(data, stream->size());
	Audio::AudioStream *audStream = (Audio::AudioStream *)Audio::makeRawMemoryStream(data, stream->size(), DisposeAfterUse::YES, 11025, 0);
	_vm->_mixer->playInputStream(Audio::Mixer::kSFXSoundType, _soundHandle, audStream);
	delete stream;
}

#define MAC_AUD_FILE_COUNT 139

static const int16 macFileList[MAC_AUD_FILE_COUNT] = {
	-1, -1, -1, -1, -1, -1, -1, 1, -1, 2, 3, 4, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, 3, 4, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

Common::SeekableReadStream *Sound::getMacintoshFile(const char *baseSoundName) {
	// FIXME: This needs to be cleaned up and switched over to the actual resource fork format.

	Common::File file;
	if (!file.open("Star Trek Audio"))
		error ("Could not open Star Trek Audio");

	// Check the file table
	file.seek(0x179AD4);
	int32 index = -1;
	
	for (uint16 i = 0; i < MAC_AUD_FILE_COUNT; i++) {
		Common::String filename;
		byte stringLength = file.readByte();
		for (byte j = 0; j < stringLength; j++)
			filename += file.readByte();
		filename += '\0';
		if (!scumm_stricmp(baseSoundName, filename.c_str())) {
			index = i;
			break;
		}
	}
	
	if (index < 0)
		error ("Could not find \'%s\' in \'Star Trek Audio\'", baseSoundName);
		
	if (macFileList[index] < 0)
		error ("File mapping for \'%s\' has not yet been found", baseSoundName);
		
	Common::SeekableReadStream *stream = NULL;
		
	file.seek(0xD74C);
	for (uint16 i = 0; i < macFileList[index] + 1; i++) {
		uint32 fileSize = file.readUint32BE();
		if (i == macFileList[index]) {
			stream = file.readStream(fileSize);
			break;
		} else
			file.seek(fileSize, SEEK_CUR);
	}
	
	file.close();
	
	return stream;
}

}
