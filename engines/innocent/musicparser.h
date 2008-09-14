#ifndef INNOCENT_MUSIC_H
#define INNOCENT_MUSIC_H

#include "common/queue.h"
#include "common/singleton.h"
#include "sound/midiparser.h"

#include "innocent/value.h"

namespace Innocent {
//

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
	void setBeat(uint16 beat);

	void loadActiveNotes();
	bool doCommand(byte command, byte parameter, EventInfo &info);
	void fillEventQueue();

	byte *_script;
	uint16 _scriptOffset;

	byte _tune[6666];
	uint16 _num_beats;
	byte *_beats;
	byte *_current_beat;
	uint16 _current_beat_id;
	Common::Queue<EventInfo> _eventQueue;
};

#define Music MusicParser::instance()

}

#endif
