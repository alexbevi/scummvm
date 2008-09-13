#ifndef INNOCENT_MUSIC_H
#define INNOCENT_MUSIC_H

#include "sound/midiparser.h"

#include "innocent/value.h"

namespace Innocent {
//

class MusicParser : public MidiParser {
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
	byte *_script;
	uint16 _scriptOffset;

	byte _tune[6000];
	uint16 _num_beats;
	byte *_beats;
	uint16 _current_beat;
};

}

#endif
