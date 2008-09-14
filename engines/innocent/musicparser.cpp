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
	_script = MusicScript(data);
	return true;
}

void MusicParser::parseNextEvent(EventInfo &info) {
}

MusicScript::MusicScript() : _code(0) {}

MusicScript::MusicScript(const byte *data) :
	_code(data),
	_tune(READ_LE_UINT16(data)),
	_offset(2) {}

Tune::Tune() : _currentBeat(-1) {}

enum {
	kTuneBeatCountOffset = 0x21,
	kTuneHeaderSize =	   0x25
};

Tune::Tune(uint16 index) {
	index = 1;
	Res.loadTune(index, _data);

	uint16 nbeats = READ_LE_UINT16(_data + kTuneBeatCountOffset);
	_beats.resize(nbeats);

	const byte *beat = _data + kTuneHeaderSize;
	const byte *channels = beat + 8 * nbeats;

	for (uint i = 0; i < _beats.size(); i++) {
		_beats[i] = Beat(beat, channels, _data);
		beat += 8;
	}

	_currentBeat = 0;
}

Beat::Beat() {}

Beat::Beat(const byte *def, const byte *channels, const byte *tune) {
	for (int i = 0; i < 8; i++)
		if (def[i])
			_channels[i] = Channel(channels + 16 * (def[i] - 1), tune);
}

Channel::Channel() {}

Channel::Channel(const byte *def, const byte *tune) {
	for (int i = 0; i < 4; i++) {
		const uint16 off = READ_LE_UINT16(def);
		def += 2;
		if (off)
			_notes[i] = Note(tune + off);
	}

	for (int i = 0; i < 4; i++) {
		_init[i] = MusicCommand(def);
		def += 2;
	}
}

Note::Note() {}

Note::Note(const byte *data) :
	_data(data) {}

MusicCommand::MusicCommand() {}

MusicCommand::MusicCommand(const byte *def) :
	_command(def[0]),
	_parameter(def[1]) {}

} // End of namespace
