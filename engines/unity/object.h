#ifndef _OBJECT_H
#define _OBJECT_H

#include "common/array.h"
#include "common/str.h"

namespace Common {
	class SeekableReadStream;
}

namespace Unity {

class UnityEngine;
class SpritePlayer;

struct objectID {
	byte id;
	byte screen;
	byte world;
	byte unused;
	objectID(): id(255), screen(255), world(255), unused(0) { }
	objectID(byte i, byte s, byte w) : id(i), screen(s), world(w), unused(0) { }
};

objectID readObjectID(Common::SeekableReadStream *stream);

struct Description {
	Common::String text;
	uint32 entry_id;
	uint32 voice_group, voice_subgroup, voice_id;
};

int readBlockHeader(Common::SeekableReadStream *objstream);

class Entry {
public:
	virtual ~Entry() { }
};

class EntryList {
public:
	~EntryList();
	Common::Array<Entry *> entries;

	void readEntryList(Common::SeekableReadStream *objstream);
	void readEntry(int type, Common::SeekableReadStream *objstream);
};

class ConditionBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
};

class AlterBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
};

class ReactionBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
};

class CommandBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
};

class ScreenBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
};

class PathBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
};

class GeneralBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
};

class ConversationBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
};

class BeamBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
};

class TriggerBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
};

class CommunicateBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
};

class ChoiceBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);

	EntryList unknown1; // 0x26
	EntryList unknown2; // 0x27
};

class Object {
public:
	objectID id;
	unsigned int x, y;
	unsigned int width, height;
	uint16 z_adjust;
	bool active;
	bool scaled; // XXX
	SpritePlayer *sprite;

	Common::Array<Description> descriptions;
	EntryList use_entries, get_entries, look_entries, timer_entries;

	void loadObject(UnityEngine *_vm, unsigned int world, unsigned int screen, unsigned int id);

protected:
	void readBlock(int type, Common::SeekableReadStream *objstream);
	void readDescriptions(Common::SeekableReadStream *objstream);
	void readDescriptionBlock(Common::SeekableReadStream *objstream);
};

} // Unity

#endif

