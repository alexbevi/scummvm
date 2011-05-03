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

#include "common/file.h"

#include "common/config-manager.h"

#include "common/serializer.h"

#include "audio/midiparser.h"

#include "darkseed2/music.h"
#include "darkseed2/options.h"
#include "darkseed2/resources.h"
#include "darkseed2/saveload.h"

namespace DarkSeed2 {

Music::Music(Audio::Mixer &mixer, MidiDriver &driver) : _mixer(&mixer) {
	_midiPlayer = new MidiPlayer(&driver, 0);

	_midiMode = kMidiModeGM;
	_mute = false;
}

Music::~Music() {
	stop();

	delete _midiPlayer;
}

void Music::setMidiMode(MidiMode midiMode) {
	_midiMode = midiMode;
}

bool Music::playMID(Common::SeekableReadStream &mid) {
	_midiPlayer->loadSMF(mid);

	if (!_mute)
		_midiPlayer->play(true);

	return true;
}

bool Music::playMID(Resources &resources, const Common::String &mid) {
	if (mid.empty()) {
		// No file specified, stop playback

		stop();
		return true;
	}

	if (mid == _name)
		// If the current music is already playing, don't restart it
		return true;

	debugC(-1, kDebugMusic, "Playing MIDI \"%s\"", mid.c_str());

	Common::String midFile = "sndtrack/";

	midFile += mid;

	if      (_midiMode == kMidiModeGM)
		midFile += "gm";
	else if (_midiMode == kMidiModeFM)
		midFile += "fm";

	midFile = Resources::addExtension(midFile, "MID");

	if (!resources.hasResource(midFile))
		return false;

	Common::SeekableReadStream *resMID = resources.getResource(midFile);

	bool result = playMID(*resMID);

	delete resMID;

	_name = mid;

	return result;
}

void Music::syncSettings(const Options &options) {
	// Getting conf values
	int volumeMusic = options.getVolumeMusic();

	_mute = (volumeMusic == 0);

	// Setting values
	_mixer->setVolumeForSoundType(Audio::Mixer::kMusicSoundType, volumeMusic);

	_midiPlayer->syncVolume();

	if (_mute)
		_midiPlayer->stop(false);
	else
		_midiPlayer->play(true);
}

void Music::stop() {
	debugC(-1, kDebugMusic, "Stopping music");

	_name.clear();
	_midiPlayer->stop();
}

bool Music::saveLoad(Common::Serializer &serializer, Resources &resources) {
	byte midiMode = (byte) _midiMode;

	SaveLoad::sync(serializer, midiMode);
	SaveLoad::sync(serializer, _name);

	_midiMode = (MidiMode) midiMode;

	return true;
}

bool Music::loading(Resources &resources) {
	Common::String name = _name;

	stop();
	playMID(resources, name);

	return true;
}


MidiPlayer::MidiPlayer(MidiDriver *driver, const char *pathMask) : _parser(0), _driver(driver), _pathMask(pathMask), _looping(false), _isPlaying(false), _passThrough(false), _isGM(true), _track(0) {
	memset(_channel, 0, sizeof(_channel));
	memset(_channelVolume, 255, sizeof(_channelVolume));
	_masterVolume = 0;
	this->open();
	_smfParser = MidiParser::createParser_SMF();
	_midiMusicData = NULL;
	_nativeMT32 = false;

	// TODO: Load cmf.ins with the instrument table.  It seems that an
	// interface for such an operation is supported for Adlib.  Maybe for
	// this card, setting instruments is necessary.
}

MidiPlayer::~MidiPlayer() {
	_driver->setTimerCallback(NULL, NULL);
	stop();
	this->close();
	_smfParser->setMidiDriver(NULL);
	delete _smfParser;
	delete[] _midiMusicData;
}

bool MidiPlayer::isPlaying() {
	return _isPlaying;
}

void MidiPlayer::setPlaying(bool playing) {
	_isPlaying = playing;
}

void MidiPlayer::setChannelVolume(int channel) {
	int newVolume = _channelVolume[channel] * _masterVolume / 255;
	_channel[channel]->volume(newVolume);
}

void MidiPlayer::setVolume(int volume) {
	Common::StackLock lock(_mutex);

	volume = CLIP(volume, 0, 255);
	if (_masterVolume == volume)
		return;
	_masterVolume = volume;

	for (int i = 0; i < 16; ++i) {
		if (_channel[i]) {
			setChannelVolume(i);
		}
	}
}

int MidiPlayer::getVolume() {
	return _masterVolume;
}

void MidiPlayer::setNativeMT32(bool b) {
	_nativeMT32 = b;
}

bool MidiPlayer::hasNativeMT32() {
	return _nativeMT32;
}

int MidiPlayer::open() {
	// Don't ever call open without first setting the output driver!
	if (!_driver)
		return 255;

	int ret = _driver->open();
	if (ret)
		return ret;

	_driver->setTimerCallback(this, &onTimer);
	return 0;
}

void MidiPlayer::close() {
	stop();
	if (_driver)
		_driver->close();
	_driver = 0;
}

void MidiPlayer::send(uint32 b) {
	if (_passThrough) {
		_driver->send(b);
		return;
	}

	byte channel = (byte)(b & 0x0F);
	if ((b & 0xFFF0) == 0x07B0) {
		// Adjust volume changes by master volume
		byte volume = (byte)((b >> 16) & 0x7F);
		_channelVolume[channel] = volume;
		volume = volume * _masterVolume / 255;
		b = (b & 0xFF00FFFF) | (volume << 16);
	} else if ((b & 0xF0) == 0xC0 && !_isGM && !_nativeMT32) {
		b = (b & 0xFFFF00FF) | MidiDriver::_mt32ToGm[(b >> 8) & 0xFF] << 8;
	}
	else if ((b & 0xFFF0) == 0x007BB0) {
		//Only respond to All Notes Off if this channel
		//has currently been allocated
		if (_channel[b & 0x0F])
			return;
	}

	if (!_channel[channel]) {
		_channel[channel] = (channel == 9) ? _driver->getPercussionChannel() : _driver->allocateChannel();
		// If a new channel is allocated during the playback, make sure
		// its volume is correctly initialized.
		setChannelVolume(channel);
	}

	if (_channel[channel])
		_channel[channel]->send(b);
}

void MidiPlayer::metaEvent(byte type, byte *data, uint16 length) {

	switch (type) {
	case 0x2F:	// End of Track
		if (_looping)
			_parser->jumpToTick(0);
		else
			stop();
		break;
	default:
		//warning("Unhandled meta event: %02x", type);
		break;
	}
}

void MidiPlayer::setTimerCallback(void *timerParam, void (*timerProc)(void *)) {
	if (_driver)
		_driver->setTimerCallback(timerParam, timerProc);
}

uint32 MidiPlayer::getBaseTempo() {
	return _driver ? _driver->getBaseTempo() : 0;
}

MidiChannel *MidiPlayer::allocateChannel() {
	return _driver ? _driver->allocateChannel() : 0;
}

MidiChannel *MidiPlayer::getPercussionChannel() {
	return _driver ? _driver->getPercussionChannel() : 0;
}

void MidiPlayer::onTimer(void *refCon) {
	MidiPlayer *music = (MidiPlayer *)refCon;
	Common::StackLock lock(music->_mutex);

	if (music->_parser)
		music->_parser->onTimer();
}

void MidiPlayer::loadSMF(Common::SeekableReadStream &stream) {
	stop();

	stream.seek(0);

	_midiMusicSize = stream.size();
	_midiMusicData = new byte[_midiMusicSize];
	stream.read(_midiMusicData, _midiMusicSize);
}

void MidiPlayer::play(bool loop) {
	if (_isPlaying)
		return;

	if (!_midiMusicData)
		return;

	if (_smfParser->loadMusic(_midiMusicData, _midiMusicSize)) {
		MidiParser *parser = _smfParser;
		parser->setTrack(0);
		parser->setMidiDriver(this);
		parser->setTimerRate(getBaseTempo());
		parser->property(MidiParser::mpCenterPitchWheelOnUnload, 1);

		_parser = parser;

		syncVolume();

		_looping = loop;
		_isPlaying = true;
	}
}

void MidiPlayer::stop(bool unload) {
	Common::StackLock lock(_mutex);

	if (!_isPlaying) {
		if (unload) {
			delete[] _midiMusicData;
			_midiMusicData = 0;
			_midiMusicSize = 0;
		}

		return;
	}

	_track = 0;
	_isPlaying = false;
	if (_parser) {
		_parser->unloadMusic();
		_parser = NULL;
	}

	if (unload) {
		delete[] _midiMusicData;
		_midiMusicData = 0;
		_midiMusicSize = 0;
	}
}

void MidiPlayer::pause() {
	setVolume(-1);
	_isPlaying = false;
}

void MidiPlayer::resume() {
	syncVolume();
	_isPlaying = true;
}

void MidiPlayer::setLoop(bool loop) {
	_looping = loop;
}

void MidiPlayer::setPassThrough(bool b) {
	_passThrough = b;
}

void MidiPlayer::setGM(bool isGM) {
	_isGM = isGM;
}

void MidiPlayer::syncVolume() {
	int volume = ConfMan.getInt("music_volume");
	setVolume(volume);
}

} // End of namespace DarkSeed2
