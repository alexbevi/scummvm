#include "innocent/musicparser.h"

#include "common/endian.h"
#include "sound/mididrv.h"

#include "innocent/resources.h"
#include "innocent/util.h"

namespace Innocent {
//

DECLARE_SINGLETON(MusicParser);

void EventQueue::push(const EventInfo &info) {
	EventQueue::iterator it = begin();
	while (it != end() && it->delta < info.delta)
		it++;

	debugC(4, kDebugLevelMusic, "inserted event 0x%02x at %d", info.event, info.delta);
	insert(it, info);
}

EventInfo EventQueue::pop() {
	EventInfo info = *begin();
	pop_front();
	return info;
}

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
	info.delta -= _position._last_event_tick;
	debugC(4, kDebugLevelMusic, "got event 0x%02x, delta %d from the queue, pushing", info.event, info.delta);
}

void MusicParser::fillEventQueue() {
	while (_eventQueue.empty()) {
		debugC(4, kDebugLevelMusic, "current tick: %d", getTick());
		loadActiveNotes();
		setBeat(_current_beat_id + 1);
	}
	// TODO hanging notes not supported yet
}

void MusicParser::loadActiveNotes() {
	byte *note = _current_beat;

	uint32 delta = _current_beat_id * _ppqn;
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

			if (doCommand(command, parameter, info))
				_eventQueue.push(info);
		}
	}
}

enum Command {
	kSetTerminator = 0x81,
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

	case kSetTerminator:
		debugC(1, kDebugLevelMusic, "set terminator for channel %d to 0x%02x STUB", info.event, parameter);
		return false;

	case 0:
	case 0x97:
		return false;

	default:
		error("unhandled music command 0x%02x", command);
	}
}

} // End of namespace
