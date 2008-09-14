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
	enum Status {
		kThxBye,
		kCallMe,
		kNvm
	};

	MusicCommand();
	MusicCommand(const byte *code);
	MusicCommand::Status parseNextEvent(EventInfo &info);
	bool empty() const;

private:
	byte _command, _parameter;
};

class Note {
public:
	Note();
	Note(const byte *data);
	MusicCommand::Status parseNextEvent(EventInfo &info);
	uint32 delta() const;
	void reset();

private:
	const byte *_data;
	uint32 _tick;
	byte _note;
	const byte *_begin;
};

class Channel {
public:
	Channel();
	Channel(const byte *def, const byte *tune, byte chanidx);
	MusicCommand::Status parseNextEvent(EventInfo &info);
	uint32 delta() const;
	void reset();

private:
	Note _notes[4];
	MusicCommand _init[4];
	bool _active, _not_initialized;
	byte _initnote, _chanidx;
};

class Beat {
public:
	Beat();
	Beat(const byte *def, const byte *channels, const byte *tune);
	MusicCommand::Status parseNextEvent(EventInfo &info);
	void reset();

private:
	Channel _channels[8];
};

class Tune {
public:
	Tune();
	Tune(uint16 index);
	MusicCommand::Status parseNextEvent(EventInfo &info);
	void setBeat(uint16);

private:
	std::vector<Beat> _beats;

	byte _data[6666];
	int32 _currentBeat;
};

class MusicScript {
public:
	MusicScript();
	MusicScript(const byte *data);
	void parseNextEvent(EventInfo &info);

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

	friend class Note;
protected:
	void parseNextEvent(EventInfo &info);

private:
	MusicScript _script;
};

#define Music MusicParser::instance()

}

#endif
