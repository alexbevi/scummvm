#include "innocent/musicparser.h"

#include "common/endian.h"
#include "sound/mididrv.h"

#include "innocent/resources.h"
#include "innocent/util.h"

namespace Innocent {
//

DECLARE_SINGLETON(MusicParser);

MusicParser::MusicParser() : MidiParser() {}

MusicParser::~MusicParser() {}

bool MusicParser::loadMusic(byte *data, uint32 /*size*/) {
	_script.reset(new MusicScript(data));

	_driver->open();
	setTimerRate(_driver->getBaseTempo());
	_driver->setTimerCallback(this, &MidiParser::timerCallback);

	_num_tracks = 1;
//	_clocks_per_tick = 0x19;
	setTempo(500000 * 0x19);
	setTrack(0);
	return true;
}

void MusicParser::parseNextEvent(EventInfo &info) {
	info.length = 0;
	_script->parseNextEvent(info);
}

MusicScript::MusicScript() : _code(0) {}

MusicScript::MusicScript(const byte *data) :
	_code(data),
	_tune(READ_LE_UINT16(data)),
	_offset(2) {}

enum {
	kSetBeat = 0x9a,
	kStop =	   0x9b
};

void MusicScript::parseNextEvent(EventInfo &info) {
	MusicCommand::Status ret = _tune.parseNextEvent(info);

	while (ret != MusicCommand::kThxBye) {
		while (ret == MusicCommand::kCallMe) {
			switch (_code[_offset]) {

			case kSetBeat:
				debugC(2, kDebugLevelMusic, "will set beat to %d", _code[_offset + 1]);
				_tune.setBeat(_code[_offset + 1], Music.getTick());
				_offset += 2;
				ret = MusicCommand::kThxBye;
				break;

			case kStop:
				debugC(2, kDebugLevelMusic, "will stop playing");
				Music.unloadMusic();
				return;

			default:
				error("unhandled music script call %x", _code[_offset]);
			}
		}

		while (ret == MusicCommand::kNextBeat) {
			debugC(3, kDebugLevelMusic, "===== next beat at tick %d", info.delta);
			_tune.setBeat(_tune.beatId() + 1, info.delta);
			break;
		}
		ret = _tune.parseNextEvent(info);
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
}

void Tune::setBeat(uint16 index, uint32 start) {
	_currentBeat = index;
	_beats[_currentBeat].reset(start);
}

MusicCommand::Status Tune::parseNextEvent(EventInfo &info) {
	return _beats[_currentBeat].parseNextEvent(info);
}

Beat::Beat() : _start(0) {}

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
	_start = start;
}

MusicCommand::Status Beat::parseNextEvent(EventInfo &info) {
	Channel *best = 0;
	uint32 bestdelta = 0xffffffff;
	for (int i = 0; i < 8; i++) {
		uint32 delta = _channels[i].delta();
		if (delta < bestdelta) {
			bestdelta = delta;
			best = &_channels[i];
		}
		debugC(5, kDebugLevelMusic, "channel %d delta: %d", i+2, delta);
	}

	if (!best) {
		info.delta = _start + 64;
		return MusicCommand::kNextBeat;
	}
	debugC(5, kDebugLevelMusic, "best delta: %d, channel %d", bestdelta, best->index());

	if (bestdelta + Music.getTick() > _start + 64) {
		info.delta = _start + 64;
		return MusicCommand::kNextBeat;
	}

	return best->parseNextEvent(info);
}

Channel::Channel() : _active(false) {}

Channel::Channel(const byte *def, const byte *tune, byte chanidx) {
	for (int i = 0; i < 4; i++) {
		const uint16 off = READ_LE_UINT16(def);
		def += 2;
		if (off) {
			debugC(2, kDebugLevelMusic, "found note at offset 0x%x", off);
			_notes[i] = Note(tune + off);
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

uint32 Channel::delta() const {
	unless (_active)
		return 0xffffffff;

	if (_not_initialized)
		return 0;

	uint32 bestdelta = 0xffffffff;

	for (int i = 0; i < 4; i++) {
		uint32 d = _notes[i].delta();
		if (d < bestdelta) {
			debugC(4, kDebugLevelMusic, "best note at %d is %d", _chanidx, i);
			bestdelta = d;
		}
	}

	return bestdelta;
}

MusicCommand::Status Channel::parseNextEvent(EventInfo &info) {
	MusicCommand::Status ret;
	info.event = _chanidx;
	info.delta = 0;
	if (_not_initialized) {
		while (_initnote < 4) {
			unless (_init[_initnote].empty()) {
				ret = _init[_initnote++].parseNextEvent(info);
				break;
			} else
				_initnote++;
		}

		// let's see if we're finished with initialization
		int i = _initnote;
		while (i < 4)
			unless (_init[i++].empty())
				return ret;

		_not_initialized = false;
	} else {
		uint32 bestdelta = 0xffffffff;
		Note *best = 0;

		for (int i = 0; i < 4; i++) {
			uint32 d = _notes[i].delta();
			if (d < bestdelta) {
				bestdelta = d;
				best = &_notes[i];
			}
		}

		assert(best);
		info.event = _chanidx;
		ret = best->parseNextEvent(info);
	}
	return ret;
}


Note::Note() : _data(0), _begin(0) {}

Note::Note(const byte *data) :
	_data(data), _tick(0), _note(0), _begin(data) {}

void Note::reset() {
	unless (_data)
		return;

	_tick = 0;
	_note = 0;
	_data = _begin;
}

uint32 Note::delta() const {
	unless (_data)
		return 0xffffffff;

	checkDelta();

	debugC(5, kDebugLevelMusic, "calculating delta, tick = %d, last tick = %d", _tick, Music._position._last_event_tick);
	assert (_tick >= Music._position._last_event_tick);
	return (_tick - Music._position._last_event_tick);
}

enum {
	kSetTempo =		 0x81,
	kSetProgram = 	 0x82,
	kSetExpression = 0x89,
	kCmdNoteOff =	 0x8b,
	kCmdCallScript = 0x8c,
	kHangNote = 	 0xfe
};

enum {
	kMidiNoteOff = 		  0x80,
	kMidiNoteOn = 		  0x90,
	kMidiChannelControl = 0xb0,
	kMidiSetProgram = 	  0xc0
};

MusicCommand::Status Note::parseNextEvent(EventInfo &info) {
	assert (_data);

	checkDelta();

	MusicCommand cmd(_data);

	debugC(4, kDebugLevelMusic, "playing note code at 0x%x", _data - Music._script->_tune._data);

	info.delta = delta();
	info.basic.param1 = _note;
	MusicCommand::Status ret = cmd.parseNextEvent(info);
	if ((info.event & 0xf0) == kMidiNoteOn) {
		if (_note) {
			debugC(2, kDebugLevelMusic, "my note still playing, stopping it first");
			info.event = kMidiNoteOff | (info.event & 0xf);
			info.basic.param1 = _note;
			_note = 0;
			return MusicCommand::kThxBye;
		}
		_note = info.basic.param1;
	}

	_data += 2;
	if (info.delta == 0)
		_tick ++;
	checkDelta();
	return ret;
}

void Note::checkDelta() const {
	unless (_tick)
		_tick = Music._position._last_event_tick;
	if (_data[0] == kHangNote) {
		byte d = _data[1];
		_data += 2;
		_tick += d;
	}
}

MusicCommand::MusicCommand() : _command(0) {}

bool MusicCommand::empty() const {
	return _command == 0;
}

MusicCommand::MusicCommand(const byte *def) :
	_command(def[0]),
	_parameter(def[1]) {}

enum {
	kMidiCtrlExpression = 0xb
};

MusicCommand::Status MusicCommand::parseNextEvent(EventInfo &info) {
	switch (_command) {

	case kSetProgram:
		debugC(2, kDebugLevelMusic, "will set program on channel %d to %d in %d ticks", info.event, _parameter, info.delta);
		info.event |= kMidiSetProgram;
		info.basic.param1 = _parameter;
		break;

	case kSetExpression:
		debugC(2, kDebugLevelMusic, "will set expression on channel %d to %d in %d ticks", info.event, _parameter, info.delta);
		info.event |= kMidiChannelControl;
		info.basic.param1 = kMidiCtrlExpression;
		info.basic.param2 = _parameter;
		break;

	case kCmdNoteOff:
		debugC(2, kDebugLevelMusic, "will turn off note %d on channel %d in %d ticks", info.basic.param1, info.event, info.delta);
		info.event |= kMidiNoteOff;
		break;

	case kCmdCallScript:
		debugC(2, kDebugLevelMusic, "will call script");
		return kCallMe;

	case kSetTempo:
		debugC(2, kDebugLevelMusic, "setting tempo to %d", _parameter);
		Music.setTempo(500000 * _parameter);
		return kNvm;

	default:
		if (_command < 0x80) {
			debugC(2, kDebugLevelMusic, "will play note %d at volume %d on %d in %d ticks", _command, _parameter, info.event, info.delta);
			info.event |= kMidiNoteOn;
			info.basic.param1 = _command;
			info.basic.param2 = _parameter;
			break;
		}

		error("unhandled music command %x", _command);
	}

	return kThxBye;
}

} // End of namespace
