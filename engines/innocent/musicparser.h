#ifndef INNOCENT_MUSIC_H
#define INNOCENT_MUSIC_H

#include "common/queue.h"
#include "common/singleton.h"
#include "sound/midiparser.h"

#include "innocent/value.h"

namespace Innocent {
//

class EventQueue : public Common::List<EventInfo> {
public:
	void push(const EventInfo &info);
	EventInfo pop();
};

class MusicParser : public MidiParser, public Common::Singleton<MusicParser> {
public:
	MusicParser() : MidiParser() {}
	virtual ~MusicParser() {}

	virtual bool loadMusic(byte *data, uint32 size = 0);
	virtual bool loadMusic(CodePointer &p) {
		return loadMusic(p.code());
	}

protected:
	virtual void parseNextEvent(EventInfo &info);

private:
	void loadTune();

	bool nextChannelInit(EventInfo &info);
	bool nextInitCommand(EventInfo &info);
	bool doCommand(byte command, byte parameter, EventInfo &info);
	void callScript();

	byte *_script;
	uint16 _scriptOffset;

	byte _tune[6666];

	uint16 _nbeats, _thisbeat, _nextbeat;
	byte *_data, *_beat, *_channel, *_nextcommand, *_beatchannel;

	bool _beatinitialized;
	byte *_notes[8][4];
	byte _note[8][4];
	uint32 _times[8][4];
};

#define Music MusicParser::instance()

}

#endif
