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
}

void MusicParser::parseNextEvent(EventInfo &info) {
	info.delta = 0;
	while (_beat < _tracks[0] + _nbeats * 8 && !nextChannelInit(info)) {
		_beat += 8;
		info.delta += 64;
	}

	assert (_beat < _data);

	debugC(3, kDebugLevelMusic, "event 0x%02x delta %d", info.event, info.delta);
}

bool MusicParser::nextChannelInit(EventInfo &info) {
	unless (_channel) {
		_nextcommand = 0;
		_channel = _beat;
		_beatchannel = _data + 16 * (*_channel - 1);
	}
	info.event = _channel - _beat + 2;

	debugC(3, kDebugLevelMusic, "channel at offset 0x%x", _channel - _tune);
	while (_channel < _beat + 8 && !(*_channel && nextInitCommand(info))) {
		_nextcommand = 0;
		_channel++;
		_beatchannel = _data + 16 * (*_channel - 1);
		info.event = _channel - _beat + 2;
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
