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

#include "innocent/musicparser.h"

#include "common/endian.h"
#include "audio/mididrv.h"

#include "innocent/resources.h"
#include "innocent/util.h"

namespace Common {
	DECLARE_SINGLETON(Innocent::MusicParser);
}

namespace Innocent {
//

MusicParser::MusicParser() : MidiParser(), _time(0), _lasttick(0), _tick(0) {
	MidiDriver::DeviceHandle dev = MidiDriver::detectDevice(MDT_MIDI | MDT_ADLIB | MDT_PREFER_GM);
	_midiDriver = MidiDriver::createMidi(dev);
	_midiDriver->open();

	setMidiDriver(_midiDriver);
	setTimerRate(_midiDriver->getBaseTempo());
	_midiDriver->setTimerCallback(this, &MusicParser::timerCallback);
}

MusicParser::~MusicParser() {
	silence();
	unloadMusic();
	_midiDriver->setTimerCallback(0, 0);
	_midiDriver->close();
	setMidiDriver(0);
	delete _midiDriver;
}

bool MusicParser::loadMusic(byte *data, uint32 size) {
	unloadMusic();
	silence();
	_script.reset(new MusicScript(data));
	_tune.reset(new Tune(_script->getTune()));

	_num_tracks = 1;
	_ppqn = 120;
//	_clocks_per_tick = 0x19;
	setTempo(500000 * 0x19);
	setTrack(0);
	return true;
}

void MusicParser::tick() {
	_time += _timer_rate;
	if (_lasttick && _time < _lasttick + _psec_per_tick)
		return;

	_lasttick = _time;

	_tune->tick();
	_tick++;
}

static byte notes[8][4];

enum {
	kMidiNoteOff = 		  0x80,
	kMidiNoteOn = 		  0x90,
	kMidiChannelControl = 0xb0,
	kMidiSetProgram = 	  0xc0
};

void MusicParser::silence() {
	debugC(2, kDebugLevelMusic, "turning off all notes");

	for (int channel = 2; channel < 10; channel++)
		for (int i = 0; i < 4; i++)
			if (notes[channel][i])
				Music._driver->send(channel | kMidiNoteOff, notes[channel][i], 0);

	memset(notes, 0, sizeof(notes));
}

MusicScript::MusicScript() : _code(0) {}

MusicScript::MusicScript(const byte *data) :
	_code(data),
	_offset(2) {}

enum {
	kJump = 0x96,
	kSetBeat = 0x9a,
	kStop =	   0x9b
};

void MusicScript::tick() {
	while (true) {
		switch (_code[_offset]) {

		case kJump: {
			uint16 target = READ_LE_UINT16(_code + _offset + 2);
			debugC(2, kDebugLevelMusic, "will jump to music script at 0x%x", target);
			_offset = target;
			break;
		}

		case kSetBeat:
			debugC(2, kDebugLevelMusic, "will set beat to %d", _code[_offset + 1]);
			Music.setBeat(_code[_offset + 1]);
			_offset += 2;
			return;

		case kStop:
			debugC(2, kDebugLevelMusic, "will stop playing");
			Music.silence();
			Music.unloadMusic();
			return;

		default:
			error("unhandled music script call %x", _code[_offset]);
		}
	}
}

Tune::Tune() : _currentBeat(-1) {}

enum {
	kTuneBeatCountOffset = 0x21,
	kTuneHeaderSize =	   0x25
};

Tune::Tune(uint16 index) {
	Res.loadTune(index, _data);

	uint16 nbeats = READ_LE_UINT16(_data + kTuneBeatCountOffset);
	_beats.resize(nbeats);

	const byte *beat = _data + kTuneHeaderSize;
	const byte *channels = beat + 8 * nbeats;

	for (uint i = 0; i < _beats.size(); i++) {
		debugC(2, kDebugLevelMusic, "found beat at offset 0x%x", beat - _data);
		_beats[i] = Beat(beat, channels, _data);
		beat += 8;
	}

	_currentBeat = 0;
	_beatticks = 0;
}

void Tune::setBeat(uint16 index) {
	_currentBeat = index;
	_beats[_currentBeat].reset();
	_beatticks = 0;
}

void Tune::tick() {
	_beats[_currentBeat].tick();
	_beatticks++;
	if (_beatticks == 64)
		setBeat(_currentBeat + 1);
	_beatticks %= 64;
}

Beat::Beat() {}

Beat::Beat(const byte *def, const byte *channels, const byte *tune) {
	for (int i = 0; i < 8; i++)
		if (def[i]) {
			uint16 off = 16 * def[i];
			debugC(2, kDebugLevelMusic, "found channel at offset 0x%x", off + channels - tune);
			_channels[i] = Channel(channels + off, tune, i + 2);
		}
}

void Beat::reset(uint32 start) {
	for (int i = 0; i < 8; i++)
		_channels[i].reset();
}

void Beat::tick() {
	for (int i = 0; i < 8; i++)
		_channels[i].tick();
}

Channel::Channel() : _active(false) {}

Channel::Channel(const byte *def, const byte *tune, byte chanidx) {
	for (int i = 0; i < 4; i++) {
		const uint16 off = READ_LE_UINT16(def);
		def += 2;
		if (off) {
			debugC(2, kDebugLevelMusic, "found note at offset 0x%x", off);
			_notes[i] = Note(tune + off, i);
		}
	}

	for (int i = 0; i < 4; i++) {
		_init[i] = MusicCommand(def);
		def += 2;
	}
	_active = true;
	_not_initialized = true;
	_initnote = 0;
	_chanidx = chanidx;
}

void Channel::reset() {
	unless (_active)
		return;

	_not_initialized = true;
	_initnote = 0;

	for (int i = 0; i < 4; i++)
		_notes[i].reset();
}

enum {
	kMidiCtrlExpression = 	0xb,
	kMidiCtrlAllNotesOff = 0x7b
};

void Channel::tick() {
	if (_not_initialized) {
		for (byte i = 0; i < 4; i++)
			_init[i].exec(_chanidx);
		_not_initialized = false;
	}

	for (byte i = 0; i < 4; i++)
		_notes[i].tick(_chanidx);
}

Note::Note() : _data(0), _begin(0) {}

Note::Note(const byte *data, byte index) :
	_data(data), _tick(0), _begin(data), _index(index) {}

void Note::setNote(byte n) {
	notes[_channel - 2][_index] = n;
}

byte Note::note() const {
	return notes[_channel - 2][_index];
}

void Note::reset() {
	unless (_data)
		return;

	_tick = Music.getTick() + 1;
	_data = _begin;
}

enum {
	kSetTempo =		 0x81,
	kSetProgram = 	 0x82,
	kCmdSetBeat =	 0x85,
	kSetExpression = 0x89,
	kCmdNoteOff =	 0x8b,
	kCmdCallScript = 0x8c,
	kHangNote = 	 0xfe
};

void Note::tick(byte channel) {
	_channel = channel;
	unless (_data && Music.getTick() == _tick)
		return;

	if (_data[0] == kHangNote) {
		_tick += _data[1];
		_data += 2;
		return;
	}

	MusicCommand cmd(_data);
	cmd.exec(channel, this);

	_data += 2;
	_tick++;
}


MusicCommand::MusicCommand() : _command(0) {}

bool MusicCommand::empty() const {
	return _command == 0;
}

MusicCommand::MusicCommand(const byte *def) :
	_command(def[0]),
	_parameter(def[1]) {}

void MusicCommand::exec(byte channel, Note *note) {
	unless (_command)
		return;

	switch (_command) {

	case kSetProgram:
		debugC(2, kDebugLevelMusic, "set program on channel %d to %d", channel, _parameter);
		Music._driver->send(channel | kMidiSetProgram, MidiDriver::_mt32ToGm[_parameter], 0);
		break;

	case kSetExpression:
		debugC(2, kDebugLevelMusic, "set expression on channel %d to %d", channel, _parameter);
		Music._driver->send(channel | kMidiChannelControl, kMidiCtrlExpression, _parameter / 2);
		break;

	case kCmdNoteOff:
		debugC(2, kDebugLevelMusic, "turn off note %d on channel %d", _parameter, channel);

		assert(note);
		Music._driver->send(channel | kMidiNoteOff, note->note(), 0);
		note->setNote(0);
		break;

	case kCmdCallScript:
		debugC(2, kDebugLevelMusic, "will call script");
		Music._script->tick();
		break;

	case kSetTempo:
		debugC(2, kDebugLevelMusic, "setting tempo to %d", _parameter);
		Music.setTempo(500000 * _parameter);
		break;

	case kCmdSetBeat:
		debugC(2, kDebugLevelMusic, "setting beat to %d", _parameter);
		Music.setBeat(_parameter);
		break;

	default:
		if (_command < 0x80) {
			assert (note);
			debugC(2, kDebugLevelMusic, "play note %d at volume %d on %d", _command, _parameter, channel);

			if (note->note()) {
				debugC(2, kDebugLevelMusic, "[first turn off note %d]", note->note());

				Music._driver->send(channel | kMidiNoteOff, note->note(), 0);
			}

			Music._driver->send(channel | kMidiNoteOn, _command, _parameter);
			note->setNote(_command);
			break;
		}

		error("unhandled music command %x", _command);
	}
}

} // End of namespace
