#include "innocent/musicparser.h"

#include "common/endian.h"

#include "innocent/resources.h"
#include "innocent/util.h"

namespace Innocent {
//

DECLARE_SINGLETON(MusicParser);

bool MusicParser::loadMusic(byte *data, uint32 /*size*/) {
	_script = data;
	_scriptOffset = 2;

	loadTune();

	_ppqn = 64;
	setTempo(500000);
	setTrack(0);

	return true;
}

enum {
	kNumBeatsOffset = 0x21,
	kBeatTableOffset = 0x25
};

void MusicParser::loadTune() {
	const uint16 tune_index = READ_LE_UINT16(_script);
	debugC(1, kDebugLevelMusic, "loading tune %d", tune_index);
	Res.loadTune(tune_index, _tune);

	_num_tracks = 1;
	_tracks[0] = _tune + kBeatTableOffset;

	_num_beats = READ_LE_UINT16(_tune + kNumBeatsOffset);
	debugC(1, kDebugLevelMusic, "%d beats found", _num_beats);

	_beats = _tune + kBeatTableOffset + _num_beats * 8;
	setBeat(0);
}

void MusicParser::setBeat(uint16 beat) {
	assert(beat < _num_beats);

	_current_beat_id = beat;
	_current_beat = _tune + kBeatTableOffset + beat * 8;

	debugC(2, kDebugLevelMusic, "beat %d, offsets: %d %d %d %d %d %d %d %d", beat, _current_beat[0], _current_beat[1], _current_beat[2], _current_beat[3], _current_beat[4], _current_beat[5], _current_beat[6], _current_beat[7]);
}

void MusicParser::parseNextEvent(EventInfo &info) {
	if (_eventQueue.empty())
		fillEventQueue();

	info = _eventQueue.pop();
}

void MusicParser::fillEventQueue() {
	uint32 planTick = _position._last_event_tick;
	while (_eventQueue.empty()) {
		planTick += _ppqn;
		unless (planTick % _ppqn)
			loadActiveNotes(planTick);
		setBeat(_current_beat_id + 1);
	}
	// TODO hanging notes not supported yet
}

void MusicParser::loadActiveNotes(uint32 tick_num) {
	byte *note = _current_beat;

	uint32 delta = tick_num - getTick();
	for (byte channel = 2; channel < 10; channel++) {
		debugC(3, kDebugLevelMusic, "active note for channel %d, index %d", channel + 1, *note);

		byte *beat = _beats + *(note++) * 16 + 8;
		for (byte i = 0; i < 4; i++) {
			byte command = *(beat++);
			byte parameter = *(beat++);
			debugC(4, kDebugLevelMusic, "command 0x%02x, parameter 0x%02x", command, parameter);

			EventInfo info;
			info.start = _current_beat;
			info.delta = delta;
			info.event = channel;

			if (doCommand(command, parameter, info)) {
				_eventQueue.push(info);
				delta = 0;
			}
		}
	}
}

enum Command {
	kSetProgram = 	 0x82,
	kSetExpression = 0x89
};

enum MidiCommand {
	kMidiChannelControl = 0xb0,
	kMidiSetProgram =	  0xc0
};

enum MidiChannelControl {
	kMidiChanExpression = 0xb
};

bool MusicParser::doCommand(byte command, byte parameter, EventInfo &info) {
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

	case 0:
		return false;

	default:
		error("unhandled music command 0x%02x", command);
	}
}

} // End of namespace
