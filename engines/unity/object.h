#ifndef _OBJECT_H
#define _OBJECT_H

#include "common/array.h"
#include "common/str.h"

namespace Common {
	class SeekableReadStream;
}

namespace Unity {

class UnityEngine;
class UnityData;
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
	virtual bool check(UnityEngine *_vm) { return true; }
	virtual void execute(UnityEngine *_vm) = 0;
	virtual ~Entry() { }
};

class EntryList {
public:
	~EntryList();
	Common::Array<Entry *> entries;

	void readEntryList(Common::SeekableReadStream *objstream);
	void readEntry(int type, Common::SeekableReadStream *objstream);

	void execute(UnityEngine *_vm);
};

class ConditionBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
	bool check(UnityEngine *_vm);
	void execute(UnityEngine *_vm);
};

class AlterBlock : public Entry {
protected:
	objectID target;
	byte alter_flags, alter_reset;
	uint16 x_pos, y_pos;
	Common::String alter_name, alter_hail;

public:
	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm);
};

class ReactionBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm);
};

class CommandBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm);
};

class ScreenBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm);
};

class PathBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm);
};

class GeneralBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm);
};

class ConversationBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm);
};

class BeamBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm);
};

class TriggerBlock : public Entry {
protected:
	uint32 trigger_id;
	bool instant_disable, enable_trigger;

public:
	bool check(UnityEngine *_vm);
	void execute(UnityEngine *_vm);
	void readFrom(Common::SeekableReadStream *stream);
};

class CommunicateBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm);
};

class ChoiceBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm);

	EntryList unknown1; // 0x26
	EntryList unknown2; // 0x27
};

class Object {
public:
	objectID id;
	unsigned int x, y, z;
	unsigned int universe_x, universe_y, universe_z;
	unsigned int width, height;
	uint16 z_adjust;
	bool active;
	bool scaled; // XXX
	uint16 sprite_id;
	SpritePlayer *sprite;

	Common::String name;

	Common::Array<Description> descriptions;
	EntryList use_entries, get_entries, look_entries, timer_entries;

	void loadObject(UnityData &data, unsigned int world, unsigned int screen, unsigned int id);
	void loadSprite(UnityEngine *vm);

protected:
	void readBlock(int type, Common::SeekableReadStream *objstream);
	void readDescriptions(Common::SeekableReadStream *objstream);
	void readDescriptionBlock(Common::SeekableReadStream *objstream);
};

class ResponseBlock {
public:
	virtual ~ResponseBlock() { }
};

class WhoCanSayBlock : public ResponseBlock {
public:
	void readFrom(Common::SeekableReadStream *stream);
};

class ChangeActorBlock : public ResponseBlock {
public:
	void readFrom(Common::SeekableReadStream *stream);
};

class ResultBlock : public ResponseBlock {
public:
	EntryList entries;

	void readFrom(Common::SeekableReadStream *stream);
};

class TextBlock : public ResponseBlock {
public:
	Common::String text;
	uint32 voice_id, voice_group;
	uint16 voice_subgroup;

	void readFrom(Common::SeekableReadStream *stream);
};

class Response {
public:
	uint16 id, state;
	Common::Array<ResponseBlock *> blocks;

	objectID target;

	Common::String text;
	uint32 voice_id, voice_group;
	uint16 voice_subgroup;

	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm, Object *speaker);
};

class Conversation {
public:
	Common::Array<Response *> responses;

	void loadConversation(UnityData &data, unsigned int world, unsigned int id);
	void execute(UnityEngine *_vm, Object *speaker, unsigned int response);
};

} // Unity

#endif

