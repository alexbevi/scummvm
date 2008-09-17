/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

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

class Note;

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
	bool empty() const;
	void exec(byte channel, Note *note = 0);

private:
	byte _command, _parameter;
};

class Note {
public:
	Note();
	Note(const byte *data, byte index);
	MusicCommand::Status parseNextEvent(EventInfo &info);
	uint32 delta() const;
	void reset();
	void tick(byte channel);
	byte note() const;
	void setNote(byte n);

private:
	void checkDelta() const;
	mutable const byte *_data;
	mutable uint32 _tick;
	const byte *_begin;
	byte _index;
	byte _channel;
};

class Channel {
public:
	Channel();
	Channel(const byte *def, const byte *tune, byte chanidx);
	MusicCommand::Status parseNextEvent(EventInfo &info);
	uint32 delta() const;
	void reset();
	byte index() const { return _chanidx; }
	void tick();

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
	void reset(uint32 start = 0);
	uint32 delta() const;
	void tick();

private:
	Channel _channels[8];
	uint32 _start;
};

class Tune {
public:
	Tune();
	Tune(uint16 index);
	MusicCommand::Status parseNextEvent(EventInfo &info);
	void setBeat(uint16);
	uint16 beatId() const { return _currentBeat; }
	void tick();

	friend class Note;
private:
	std::vector<Beat> _beats;

	byte _data[6666];
	int32 _currentBeat;
	uint32 _beatticks;
};

class MusicScript : public Common::NonCopyable {
public:
	MusicScript();
	MusicScript(const byte *data);
	void parseNextEvent(EventInfo &info);
	void tick();
	uint16 getTune() const { return READ_LE_UINT16(_code); }

	friend class Note;
private:
	const byte *_code;
	uint16 _offset;
};

class MusicParser : public MidiParser, public Common::Singleton<MusicParser> {
public:
	MusicParser();
	~MusicParser();

	bool loadMusic(byte *data, uint32 size = 0);
	static void timerCallback(void *data) { ((MusicParser *) data)->tick(); }
	void tick();
	virtual uint32 getTick() { return _tick; }
	void setBeat(uint16 beat) { _tune->setBeat(beat); }

	friend class Note;
	friend class MusicCommand;

private:
	void parseNextEvent(EventInfo &info) {  }
	std::auto_ptr<Tune> _tune;
	std::auto_ptr<MusicScript> _script;

	uint32 _time, _lasttick, _tick;
};

#define Music MusicParser::instance()

}

#endif
