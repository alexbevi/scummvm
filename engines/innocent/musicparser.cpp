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

	_current_beat = _tune + kBeatTableOffset + beat * 8;

	debugC(2, kDebugLevelMusic, "beat %d, offsets: %d %d %d %d %d %d %d %d", beat, _current_beat[0], _current_beat[1], _current_beat[2], _current_beat[3], _current_beat[4], _current_beat[5], _current_beat[6], _current_beat[7]);
}

void MusicParser::parseNextEvent(EventInfo &info) {
	if (_eventQueue.empty())
		fillEventQueue();

	info = _eventQueue.pop();
}

void MusicParser::fillEventQueue() {
	unless (getTick() % _ppqn)
		loadActiveNotes();

	// TODO hanging notes not supported yet
}

void MusicParser::loadActiveNotes() {
	byte *note = _current_beat;
	for (int channel = 2; channel < 10; channel++) {
		debugC(3, kDebugLevelMusic, "active note for channel %d, index %d", channel + 1, *note);
		byte *beat = _beats + *(note++) * 16 + 8;
		for (int i = 0; i < 4; i++) {
			byte command = *(beat++);
			byte parameter = *(beat++);
			debugC(4, kDebugLevelMusic, "command %d, parameter %d", command, parameter);
			switch (command) {
			default:
				error("unhandled music command %d", command);
			}
		}
	}
}

} // End of namespace
