#include "innocent/musicparser.h"

#include "common/endian.h"

#include "innocent/resources.h"

/*
The data is structured as follows:
first comes the header - 0x25 bytes long,
at offset 0x21 is the number of beats.

Then at 0x25 there beat definitions --
for every beat, its position in the music
stream is given (in the number of beats).

Each beat has for each of eight 3-11 channels,
four commands, two bytes each.
*/

namespace Innocent {
//

DECLARE_SINGLETON(MusicParser);

bool MusicParser::loadMusic(byte *data, uint32 /*size*/) {
	_script = data;
	_scriptOffset = 2;

	loadTune();

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
	debugC(2, kDebugLevelMusic, "%d beats found", _num_beats);

	_beats = _tune + kBeatTableOffset + _num_beats * 8;
	_current_beat = 0;
}

void MusicParser::parseNextEvent(EventInfo &info) {
	
}

} // End of namespace
