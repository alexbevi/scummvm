#ifndef INNOCENT_MUSIC_H
#define INNOCENT_MUSIC_H

#include <memory>
#include <vector>

#include "common/noncopyable.h"
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
		kNvm,
		kNextBeat
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
	void checkDelta() const;
	mutable const byte *_data;
	mutable uint32 _tick;
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
	uint16 beatId() const { return _currentBeat; }

	friend class Note;
private:
	std::vector<Beat> _beats;

	byte _data[6666];
	int32 _currentBeat;
};

class MusicScript : public Common::NonCopyable {
public:
	MusicScript();
	MusicScript(const byte *data);
	void parseNextEvent(EventInfo &info);

	friend class Note;
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
	uint16 clocksPerTick() const { return _clocks_per_tick; }
	void setClocksPerTick(uint16 v) { _clocks_per_tick = v; }

	friend class Note;
protected:
	void parseNextEvent(EventInfo &info);

private:
	std::auto_ptr<MusicScript> _script;

	uint16 _clocks_per_tick;
};

#define Music MusicParser::instance()

}

#endif
