#include "innocent/musicparser.h"

#include "common/endian.h"
#include "sound/mididrv.h"

#include "innocent/resources.h"
#include "innocent/util.h"

namespace Innocent {
//

DECLARE_SINGLETON(MusicParser);

bool MusicParser::loadMusic(byte *data, uint32 /*size*/) {
	_script = data;
	_scriptOffset = 2;

	loadTune();

	_driver->open();
	setTimerRate(_driver->getBaseTempo());

	_ppqn = 64;
	setTempo(500000);
	setTrack(0);

	_driver->setTimerCallback(this, &MidiParser::timerCallback);

	return true;
}

enum {
	kNumBeatsOffset = 0x21,
	kBeatTableOffset = 0x25
};

#define DATASTART(num_beats) _tune + kBeatTableOffset + (num_beats) * kChannelCount

void MusicParser::loadTune() {
	const uint16 tune_index = 1; //READ_LE_UINT16(_script);
	debugC(1, kDebugLevelMusic, "loading tune %d", tune_index);
	Res.loadTune(tune_index, _tune);

	_num_tracks = 1;
	_tracks[0] = _tune + kBeatTableOffset;

	_nbeats = READ_LE_UINT16(_tune + kNumBeatsOffset);
	_data = _tracks[0] + _nbeats * 8;

	_beat = _tracks[0];
	debugC(3, kDebugLevelMusic, "beat at offset 0x%x", _beat - _tune);
	_channel = 0;
	_nextcommand = 0;

	_beatinitialized = false;
}

void MusicParser::parseNextEvent(EventInfo &info) {
	// delta to next full tick (nearest multiple of 64)
	uint32 tickdelta = 64 - (_position._last_event_tick % 64);
	uint32 basedelta = 0;

	while (true) {
		while (_beatinitialized) {
			debugC(4, kDebugLevelMusic, "beat initialized, executing notes, tick %d", getTick());
			uint32 bestdelta = 0xffffffff;
			byte **bestnote = 0;
			uint32 *besttimes = 0;
			for (int chan = 0; chan < 8; chan++) {
				for (int note = 0; note < 4; note++) {
					unless (_notes[chan][note])
						continue;

					uint32 delta = _times[chan][note] - _position._last_event_tick;
					debugC(4, kDebugLevelMusic, "trying note for %d %d at offset 0x%x", chan, note, _notes[chan][note] - _tune);
					if (_times[chan][note] == 0) {  // instant note
						if (_notes[chan][note][0] == 0xFE) {
							delta = _times[chan][note] = _notes[chan][note][1];
							_times[chan][note] += getTick();
							_notes[chan][note] += 2;
						} else {
							info.delta = 0;
							info.event = chan + 2;
							info.basic.param1 = note;
							if (doCommand(_notes[chan][note][0], _notes[chan][note][1], info)) {
								_notes[chan][note] += 2;
								goto done;
							} else continue;
						}
					}

					if (delta < bestdelta) {
						info.event = chan + 2;
						info.basic.param1 = note;
						bestdelta = delta;
						bestnote = &_notes[chan][note];
						besttimes = &_times[chan][note];
					}
					debugC(5, kDebugLevelMusic, "best delta: %d", bestdelta);
				}
			}

			assert(bestnote);

			debugC(5, kDebugLevelMusic, "best delta: %d [%x], tickdelta: %d", bestdelta, **bestnote, tickdelta);
			if (tickdelta <= bestdelta)
				_beatinitialized = false;
			else {
				info.delta = bestdelta;
				bool ok = doCommand((*bestnote)[0], (*bestnote)[1], info);
				*bestnote += 2;
				*besttimes = 0;
				if (ok)
					goto done;
			}
		}

		unless (_beatinitialized) {
			info.delta = basedelta;
			if (nextChannelInit(info))
				break;
			else {
				_beat += 8;
				_beatinitialized = true;
				basedelta = tickdelta;
				tickdelta += 64;
			}
		}
	}

done:
	assert (_beat < _data);

//	info.delta += basedelta;
	debugC(3, kDebugLevelMusic, "event 0x%02x delta %d", info.event, info.delta);
}

bool MusicParser::nextChannelInit(EventInfo &info) {
	unless (_channel) {
		_nextcommand = 0;
		_channel = _beat;
		_beatchannel = _data + 16 * (*_channel - 1);
		for (int i = 0; i < 4; i++) {
			_notes[_channel - _beat][i] = 0;
			_times[_channel - _beat][i] = 0;
			unless (*_channel) continue;
			uint16 offset = READ_LE_UINT16(_beatchannel + 2*i);
			if (offset) {
				debugC(3, kDebugLevelMusic, "note %d %d at offset 0x%x", _channel - _beat, i, offset);
				_notes[_channel - _beat][i] = _tune + offset;
			}
		}
	}
	info.event = _channel - _beat + 2;

	debugC(3, kDebugLevelMusic, "channel at offset 0x%x", _channel - _tune);
	while (_channel < _beat + 8 && !(*_channel && nextInitCommand(info))) {
		_nextcommand = 0;
		_channel++;
		_beatchannel = _data + 16 * (*_channel - 1);
		info.event = _channel - _beat + 2;
		for (int i = 0; i < 4; i++) {
			_notes[_channel - _beat][i] = 0;
			_times[_channel - _beat][i] = 0;
			unless (*_channel) continue;
			uint16 offset = READ_LE_UINT16(_beatchannel + 2*i);
			if (offset) {
				debugC(3, kDebugLevelMusic, "note %d %d at offset 0x%x", _channel - _beat, i, offset);
				_notes[_channel - _beat][i] = _tune + offset;
			}
		}
		debugC(3, kDebugLevelMusic, "channel at offset 0x%x", _channel - _tune);
	}

	if (_channel == _beat + 8) {
		_channel = 0;
		return false;
	}

	return true;
}

bool MusicParser::nextInitCommand(EventInfo &info) {
	unless (_nextcommand)
		_nextcommand = _beatchannel + 8;

	debugC(3, kDebugLevelMusic, "command at offset 0x%x", _nextcommand - _tune);
	while (_nextcommand < _beatchannel + 16 && !doCommand(_nextcommand[0], _nextcommand[1], info)) {
		_nextcommand += 2;
		debugC(3, kDebugLevelMusic, "command at offset 0x%x", _nextcommand - _tune);
	}

	if (_nextcommand == _beatchannel + 16) {
		_nextcommand = 0;
		return false;
	}

	_nextcommand += 2;

	return true;
}

enum Command {
	kSetTerminator = 0x81,
	kSetProgram = 	 0x82,
	kSetExpression = 0x89,
	kNoteOff =		 0x8b
};

enum MidiCommand {
	kMidiNoteOff =		  0x80,
	kMidiChannelControl = 0xb0,
	kMidiSetProgram =	  0xc0
};

enum MidiChannelControl {
	kMidiChanExpression = 0xb
};

bool MusicParser::doCommand(byte command, byte parameter, EventInfo &info) {
	debugC(4, kDebugLevelMusic, "trying command %x, parameter %x", command, parameter);
	switch (command) {

	case kSetProgram:
		debugC(4, kDebugLevelMusic, "setting channel %d program to 0x%02x", info.event, parameter);
		info.event |= kMidiSetProgram;
		info.basic.param1 = parameter;
		return true;

	case kSetExpression:
		debugC(4, kDebugLevelMusic, "setting channel %d expression to 0x%02x", info.event, parameter);
		info.event |= kMidiChannelControl;
		info.basic.param1 = kMidiChanExpression;
		info.basic.param2 = parameter;
		return true;

	case kSetTerminator:
		debugC(1, kDebugLevelMusic, "set terminator for channel %d to 0x%02x STUB", info.event, parameter);
		return false;

	case kNoteOff:
		debugC(4, kDebugLevelMusic, "note on %d off", info.event);
		info.basic.param1 = _note[info.event - 2][info.basic.param1];
		_note[info.event - 2][info.basic.param1] = 0;
		info.event |= kMidiNoteOff;

	case 0:
		return false;

	default:
		if (command < 0x80) { // note
			info.event |= 0x90;
			info.basic.param1 = command - 1;
			info.basic.param2 = parameter;
			return true;
		} else error("unhandled music command 0x%02x", command);
	}
}

} // End of namespace
