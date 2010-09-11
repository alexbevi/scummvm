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

enum {
	OBJFLAG_WALK = 0x01,
	OBJFLAG_USE = 0x02,
	OBJFLAG_TALK = 0x04,
	OBJFLAG_GET = 0x08,
	OBJFLAG_LOOK = 0x10,
	OBJFLAG_ACTIVE = 0x20,
	OBJFLAG_INVENTORY = 0x40,
	OBJFLAG_STUNNED = 0x80
};

enum {
	OBJWALKTYPE_NORMAL = 0x0,
	OBJWALKTYPE_SCALED = 0x1, // scaled with walkable polygons (e.g. characters)
	OBJWALKTYPE_TS = 0x2, // transition square
	OBJWALKTYPE_AS = 0x3 // action square
};

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
	friend class EntryList; // (temporary) hack for stop_here

protected:
	objectID internal_obj;
	byte counter1, counter2, counter3, counter4;
	uint16 state_counter, response_counter;
	byte stop_here;

	void readHeaderFrom(Common::SeekableReadStream *stream, byte header_type);

public:
	virtual bool check(UnityEngine *_vm) { return true; }
	virtual void execute(UnityEngine *_vm) = 0;
	virtual ~Entry() { }
};

class EntryList {
public:
	~EntryList();
	Common::Array<Common::Array<Entry *>*> list;

	void readEntryList(Common::SeekableReadStream *objstream);
	void readEntry(int type, Common::SeekableReadStream *objstream);

	void execute(UnityEngine *_vm);
};

class ConditionBlock : public Entry {
protected:
	objectID target;
	objectID WhoCan;

	uint16 how_close_dist, how_close_x, how_close_y;
	uint16 skill_check;
	byte unknown_a, unknown_b, unknown_c;

	objectID condition[4];
	uint16 check_x[4];
	uint16 check_y[4];
	uint16 check_unknown[4];
	uint16 check_univ_x[4];
	uint16 check_univ_y[4];
	uint16 check_univ_z[4];
	byte check_screen[4];
	byte check_status[4];
	byte check_state[4];

public:
	void readFrom(Common::SeekableReadStream *stream);
	bool check(UnityEngine *_vm);
	void execute(UnityEngine *_vm);
};

class AlterBlock : public Entry {
protected:
	objectID target;
	byte alter_flags, alter_reset, alter_state;
	uint16 x_pos, y_pos;
	Common::String alter_name, alter_hail;

	uint16 alter_timer;
	uint16 alter_anim;

	byte unknown7;
	uint16 unknown8;
	byte unknown11;
	byte unknown12;
	uint16 universe_x, universe_y, universe_z;

	uint32 voice_id;
	uint32 voice_group;
	uint16 voice_subgroup;

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
protected:
	objectID target[3];
	uint16 unknown1, unknown2;
	uint32 command_id;

public:
	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm);
};

class ScreenBlock : public Entry {
protected:
	byte new_screen;
	byte unknown2;
	byte unknown3;
	uint16 unknown4;
	uint16 unknown5;
	uint16 unknown6;
	byte unknown7;
	byte unknown8;
	uint16 unknown9;
	byte unknown10;
	byte unknown11;
	byte unknown12;
	uint16 unknown13;
	byte unknown14;
	byte unknown15;
	uint16 unknown16;
	uint16 unknown17;
	byte unknown18;
	byte unknown19;
	byte unknown20;
	uint16 unknown21;

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
protected:
	uint16 movie_id;
	uint16 unknown1;
	uint16 unknown2;
	uint16 unknown3;

public:
	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm);
};

class ConversationBlock : public Entry {
protected:
	uint16 world_id, conversation_id, response_id, state_id, action_id;

public:
	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm);
};

class BeamBlock : public Entry {
protected:
	uint16 world_id;
	uint16 unknown1;
	uint16 unknown3;
	uint16 screen_id;
public:
	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm);
};

class TriggerBlock : public Entry {
protected:
	uint32 trigger_id;
	bool enable_trigger;

public:
	void execute(UnityEngine *_vm);
	void readFrom(Common::SeekableReadStream *stream);
};

class CommunicateBlock : public Entry {
protected:
	objectID target;
	uint16 conversation_id;
	uint16 unknown1, unknown2;

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
	byte flags;
	byte state;
	byte objwalktype;
	uint16 skills;
	uint16 timer;

	uint32 curr_world;
	uint16 curr_screen;

	objectID transition;
	byte cursor_flag;
	byte cursor_id;

	uint16 sprite_id;
	SpritePlayer *sprite;

	Common::String name, talk_string;

	Common::String identify();

	Common::Array<Description> descriptions;
	EntryList use_entries, get_entries, look_entries, timer_entries;

	Object(UnityEngine *p) : _vm(p) { }

	void loadObject(unsigned int world, unsigned int screen, unsigned int id);
	void loadSprite();

	void setTalkString(const Common::String &str);
	void changeTalkString(const Common::String &str);
	void runHail(const Common::String &hail);

protected:
	UnityEngine *_vm;

	void readBlock(int type, Common::SeekableReadStream *objstream);
	void readDescriptions(Common::SeekableReadStream *objstream);
	void readDescriptionBlock(Common::SeekableReadStream *objstream);
};

class ResponseBlock {
public:
	virtual void execute(UnityEngine *_vm, Object *speaker) = 0;
	virtual ~ResponseBlock() { }
};

class WhoCanSayBlock {
protected:
	objectID whocansay;

public:
	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm, objectID &speaker);
};

class ChangeActionBlock : public ResponseBlock {
public:
	int type;
	uint16 response_id, state_id;

	void readFrom(Common::SeekableReadStream *stream, int _type);
	void execute(UnityEngine *_vm, Object *speaker);
};

class ResultBlock : public ResponseBlock {
public:
	EntryList entries;

	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm, Object *speaker);
};

class TextBlock : public ResponseBlock {
public:
	Common::String text;
	uint32 voice_id, voice_group;
	uint16 voice_subgroup;

	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm, Object *speaker);
};

class Response {
public:
	uint16 id, state;
	Common::Array<ResponseBlock *> blocks;
	Common::Array<WhoCanSayBlock *> whocansayblocks;

	byte response_state;

	uint16 next_situation;
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
	unsigned int our_world, our_id;

	void loadConversation(UnityData &data, unsigned int world, unsigned int id);
	Response *getResponse(unsigned int response, unsigned int state);
	Response *getEnabledResponse(unsigned int response);
	void execute(UnityEngine *_vm, Object *speaker, unsigned int response);
	void execute(UnityEngine *_vm, Object *speaker, unsigned int response, unsigned int state);
};

} // Unity

#endif

