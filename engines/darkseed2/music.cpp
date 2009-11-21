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
#include "common/file.h"

#include "common/config-manager.h"

#include "sound/midiparser.h"

#include "engines/darkseed2/music.h"
#include "engines/darkseed2/options.h"
#include "engines/darkseed2/resources.h"

namespace DarkSeed2 {

Music::Music(Audio::Mixer &mixer, MidiDriver &driver) : _mixer(&mixer) {
	_midiPlayer = new MidiPlayer(&driver, 0);
}

Music::~Music() {
	stop();

	delete _midiPlayer;
}

bool Music::playMID(Common::SeekableReadStream &mid) {
	_midiPlayer->playSMF(mid, true);
	return true;
}

bool Music::playMID(const Resource &resource) {
	return playMID(resource.getStream());
}

bool Music::playMID(const Resources &resources, const Common::String &mid) {
	Common::String midFile = "sndtrack/";

	midFile += mid + ".mid";

	if (!resources.hasResource(midFile))
		return false;

	Resource *resMID = resources.getResource(midFile);

	bool result = playMID(*resMID);

	delete resMID;

	return result;
}

void Music::syncSettings(const Options &options) {
	// Getting conf values
	int volumeMusic = options.getVolumeMusic();

	// Setting values
	_mixer->setVolumeForSoundType(Audio::Mixer::kMusicSoundType, volumeMusic);

	_midiPlayer->syncVolume();
}

void Music::stop() {
	_midiPlayer->stop();
}


MidiPlayer::MidiPlayer(MidiDriver *driver, const char *pathMask) : _parser(0), _driver(driver), _pathMask(pathMask), _looping(false), _isPlaying(false), _passThrough(false), _isGM(false), _track(0) {
	memset(_channel, 0, sizeof(_channel));
	memset(_channelVolume, 255, sizeof(_channelVolume));
	_masterVolume = 0;
	this->open();
	_smfParser = MidiParser::createParser_SMF();
	_midiMusicData = NULL;

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

void MidiPlayer::onTimer(void *refCon) {
	MidiPlayer *music = (MidiPlayer *)refCon;
	Common::StackLock lock(music->_mutex);

	if (music->_parser)
		music->_parser->onTimer();
}

void MidiPlayer::playSMF(Common::SeekableReadStream &stream, bool loop) {
	stop();

	_isGM = true;


	stream.seek(0);

	int midiMusicSize = stream.size();
	delete[] _midiMusicData;
	_midiMusicData = new byte[midiMusicSize];
	stream.read(_midiMusicData, midiMusicSize);

	if (_smfParser->loadMusic(_midiMusicData, midiMusicSize)) {
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

void MidiPlayer::stop() {
	Common::StackLock lock(_mutex);

	if (!_isPlaying)
		return;

	_track = 0;
	_isPlaying = false;
	if (_parser) {
		_parser->unloadMusic();
		_parser = NULL;
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

void MidiPlayer::syncVolume() {
	int volume = ConfMan.getInt("music_volume");
	setVolume(volume);
}

} // End of namespace DarkSeed2
