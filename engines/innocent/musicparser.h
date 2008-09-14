#ifndef INNOCENT_MUSIC_H
#define INNOCENT_MUSIC_H

#include <vector>

#include "common/queue.h"
#include "common/singleton.h"
#include "sound/midiparser.h"

#include "innocent/value.h"

namespace Innocent {
//

class MusicCommand {
public:
	MusicCommand();
	MusicCommand(const byte *code);

private:
	byte _command, _parameter;
};

class Note {
public:
	Note();
	Note(const byte *data);

private:
	const byte *_data;
};

class Channel {
public:
	Channel();
	Channel(const byte *def, const byte *tune);

private:
	Note _notes[4];
	MusicCommand _init[4];
};

class Beat {
public:
	Beat();
	Beat(const byte *def, const byte *channels, const byte *tune);

private:
	Channel _channels[8];
};

class Tune {
public:
	Tune();
	Tune(uint16 index);

private:
	std::vector<Beat> _beats;

	byte _data[6666];
	int32 _currentBeat;
};

class MusicScript {
public:
	MusicScript();
	MusicScript(const byte *data);

private:
	Tune _tune;
	const byte *_code;
	uint16 _offset;
};

class MusicParser : public MidiParser, public Common::Singleton<MusicParser> {
public:
	MusicParser();
	~MusicParser();

	bool loadMusic(byte *data, uint32 size = 0);

protected:
	void parseNextEvent(EventInfo &info);

private:
	MusicScript _script;
};

#define Music MusicParser::instance()

}

#endif
