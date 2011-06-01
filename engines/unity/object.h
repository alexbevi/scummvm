/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef UNITY_OBJECT_H
#define UNITY_OBJECT_H

#include "common/array.h"
#include "common/str.h"

namespace Common {
	class SeekableReadStream;
}

namespace Unity {

class UnityEngine;
class UnityData;
class SpritePlayer;

class Conversation;

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

enum {
	RESULT_FAILOTHER = 0x2, // failed to match other object
	RESULT_WALKING = 0x4, // had to start walking
	RESULT_FAILSKILL = 0x8, // skill level too low
	RESULT_NOTARGET = 0x10, // couldn't find target
	RESULT_OTHERPOS = 0x20, // other object wasn't in position (unused?)
	RESULT_FAILPOS = 0x40, // something wasn't in position
	RESULT_AWAYTEAM = 0x80, // not in away team
	RESULT_INVENTORY = 0x100, // something was/wasn't in inventory
	RESULT_INACTIVE = 0x200, // something was inactive
	RESULT_FAILSCREEN = 0x400, // something wasn't on right screen
	RESULT_STOPPED = 0x800, // stop here
	RESULT_FAILSTATE = 0x1000, // something didn't have right state
	RESULT_MATCHOTHER = 0x2000, // successfully matched other object
	RESULT_EMPTYTALK = 0x4000, // talk string was empty
	RESULT_DIDSOMETHING = 0x8000, // a result was actually executed
	RESULT_COUNTER_DOWHEN = 0x10000, // wasn't time for DO WHEN yet
	RESULT_COUNTER_DOUNTIL = 0x20000, // all done with DO UNTIL
	RESULT_STUNNED = 0x40000, // something was stunned
	RESULT_FAILPHASER = 0x80000 // couldn't phaser object
};

enum {
	BLOCK_OBJ_HEADER = 0x0,
	BLOCK_DESCRIPTION = 0x1,
	BLOCK_USE_ENTRIES = 0x2,
	BLOCK_GET_ENTRIES = 0x3,
	BLOCK_LOOK_ENTRIES = 0x4,
	BLOCK_TIMER_ENTRIES = 0x5,

	BLOCK_CONDITION = 0x6,
	BLOCK_ALTER = 0x7,
	BLOCK_REACTION = 0x8,
	BLOCK_COMMAND = 0x9,
	BLOCK_SCREEN = 0x10,
	BLOCK_PATH = 0x11,
	// 0x12 unused?
	BLOCK_GENERAL = 0x13,
	BLOCK_CONVERSATION = 0x14,
	BLOCK_BEAMDOWN = 0x15,
	BLOCK_TRIGGER = 0x16,
	BLOCK_COMMUNICATE = 0x17,
	BLOCK_CHOICE = 0x18,
	// 0x19, 0x20, 0x21, 0x22 unused?
	// (planet, computer, game, encounter?)

	BLOCK_END_ENTRY = 0x23,
	BLOCK_BEGIN_ENTRY = 0x24,

	BLOCK_END_BLOCK = 0x25,

	BLOCK_CHOICE1 = 0x26,
	BLOCK_CHOICE2 = 0x27,

	BLOCK_CONV_RESPONSE = 0x28,
	BLOCK_CONV_WHOCANSAY = 0x29,
	BLOCK_CONV_CHANGEACT_DISABLE = 0x30,
	BLOCK_CONV_CHANGEACT_SET = 0x31,
	BLOCK_CONV_CHANGEACT_ENABLE = 0x32,
	BLOCK_CONV_CHANGEACT_UNKNOWN2 = 0x33,
	BLOCK_CONV_TEXT = 0x34,
	BLOCK_CONV_RESULT = 0x35,

	BLOCK_PHASER_STUN = 0x36,
	BLOCK_PHASER_GTP = 0x37,
	BLOCK_PHASER_KILL = 0x38,
	BLOCK_PHASER_RECORD = 0x39,

	BLOCK_SPEECH_INFO = 0x40
};

enum {
	RESPONSE_ENABLED = 0x2,
	RESPONSE_DISABLED = 0x3,
	RESPONSE_UNKNOWN1 = 0x4
};

typedef unsigned int ResultType;

struct objectID {
	byte id;
	byte screen;
	byte world;
	byte unused;
	objectID(): id(255), screen(255), world(255), unused(0) { }
	objectID(byte i, byte s, byte w) : id(i), screen(s), world(w), unused(0) { }
	bool operator == (const objectID &o) const {
		return id == o.id && screen == o.screen && world == o.world;
	}
	bool operator != (const objectID &o) const {
		return id != o.id || screen != o.screen || world != o.world;
	}
};

objectID readObjectID(Common::SeekableReadStream *stream);
objectID readObjectIDBE(Common::SeekableReadStream *stream);

enum ActionType {
	ACTION_USE = 0,
	ACTION_GET = 1,
	ACTION_LOOK = 2,
	ACTION_TIMER = 3,
	ACTION_WALK = 4,
	ACTION_TALK = 5
};

struct Action {
	ActionType action_type;
	class Object *target;
	objectID who;
	objectID other;
	unsigned int x, y;
};

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
	virtual ResultType check(UnityEngine *_vm, Action *context) { return 0; }
	virtual ResultType execute(UnityEngine *_vm, Action *context) = 0;
	virtual ~Entry() { }
};

class EntryList {
public:
	~EntryList();
	Common::Array<Common::Array<Entry *>*> list;

	void readEntryList(Common::SeekableReadStream *objstream);
	void readEntry(int type, Common::SeekableReadStream *objstream);

	ResultType execute(UnityEngine *_vm, Action *context);
};

class ConditionBlock : public Entry {
protected:
	objectID target;
	objectID WhoCan;

	uint16 how_close_dist, how_close_x, how_close_y;
	uint16 skill_check;

	uint16 counter_value;
	byte counter_when;

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
	ResultType check(UnityEngine *_vm, Action *context);
	ResultType execute(UnityEngine *_vm, Action *context);
};

class AlterBlock : public Entry {
protected:
	objectID target;
	byte alter_flags, alter_reset, alter_state;
	uint16 x_pos, y_pos;
	Common::String alter_name, alter_hail;

	uint16 alter_timer;
	uint16 alter_anim;

	byte play_description;

	uint16 unknown8;
	byte unknown11;
	byte unknown12;
	uint16 universe_x, universe_y, universe_z;

	uint32 voice_id;
	uint32 voice_group;
	uint16 voice_subgroup;

public:
	void readFrom(Common::SeekableReadStream *stream);
	ResultType execute(UnityEngine *_vm, Action *context);
};

class ReactionBlock : public Entry {
protected:
	objectID target;
	uint16 dest_world, dest_screen, dest_entrance;
	byte target_type;
	byte action_type;
	byte damage_amount;
	byte beam_type;
	uint16 dest_x, dest_y, dest_unknown;

public:
	void readFrom(Common::SeekableReadStream *stream);
	ResultType execute(UnityEngine *_vm, Action *context);
};

class CommandBlock : public Entry {
protected:
	objectID target[3];
	uint16 target_x, target_y;
	uint32 command_id;

public:
	void readFrom(Common::SeekableReadStream *stream);
	ResultType execute(UnityEngine *_vm, Action *context);
};

class ScreenBlock : public Entry {
protected:
	uint16 new_world;
	byte new_screen;
	byte new_entrance;

	byte advice_screen;
	uint16 new_advice_id;
	uint16 new_advice_timer;

	uint16 unknown6;
	uint32 unknown7;
	byte unknown8;
	uint32 unknown9;
	uint32 unknown10;
	uint16 unknown11;
	uint16 unknown12;
	uint16 unknown13;
	byte unknown14;
	byte unknown15;
	byte unknown16;

public:
	void readFrom(Common::SeekableReadStream *stream);
	ResultType execute(UnityEngine *_vm, Action *context);
};

class PathBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
	ResultType execute(UnityEngine *_vm, Action *context);
};

class GeneralBlock : public Entry {
protected:
	uint16 movie_id;
	uint16 unknown1;
	uint16 unknown2;
	uint16 unknown3;

public:
	void readFrom(Common::SeekableReadStream *stream);
	ResultType execute(UnityEngine *_vm, Action *context);
};

class ConversationBlock : public Entry {
protected:
	uint16 world_id, conversation_id, response_id, state_id, action_id;

public:
	void readFrom(Common::SeekableReadStream *stream);
	ResultType execute(UnityEngine *_vm, Action *context);
};

class BeamBlock : public Entry {
protected:
	uint16 world_id;
	uint16 unknown1;
	uint16 unknown3;
	uint16 screen_id;
public:
	void readFrom(Common::SeekableReadStream *stream);
	ResultType execute(UnityEngine *_vm, Action *context);
};

class TriggerBlock : public Entry {
protected:
	uint32 trigger_id;
	bool enable_trigger;

public:
	void readFrom(Common::SeekableReadStream *stream);
	ResultType execute(UnityEngine *_vm, Action *context);
};

class CommunicateBlock : public Entry {
protected:
	objectID target;
	uint16 conversation_id;
	uint16 situation_id;
	byte hail_type;

public:
	void readFrom(Common::SeekableReadStream *stream);
	ResultType execute(UnityEngine *_vm, Action *context);
};

class ChoiceBlock : public Entry {
public:
	void readFrom(Common::SeekableReadStream *stream);
	ResultType execute(UnityEngine *_vm, Action *context);

	uint16 _unknown1, _unknown2;
	objectID _object;
	Common::String _questionstring;
	Common::String _choicestring[2];
	EntryList _choice[2];
};

class Object {
public:
	Object(UnityEngine *p);
	~Object();

	objectID id;
	byte curr_screen;

	unsigned int x, y, z;
	unsigned int universe_x, universe_y, universe_z;
	unsigned int width, height;
	int16 y_adjust;
	uint32 region_id;

	byte flags;
	byte state;

	uint16 skills;
	uint16 timer;

	byte objwalktype;
	objectID transition;

	byte cursor_flag;
	byte cursor_id;

	uint16 anim_index;
	uint16 sprite_id;
	SpritePlayer *sprite;

	Common::String name;

	Common::String talk_string;
	uint32 voice_id;
	uint32 voice_group;
	uint16 voice_subgroup;

	Common::String identify();

	Common::Array<Description> descriptions;
	EntryList use_entries, get_entries, look_entries, timer_entries;

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
	virtual void execute(UnityEngine *_vm, Object *speaker, Conversation *src) = 0;
	virtual ~ResponseBlock() { }
};

class WhoCanSayBlock {
protected:
	objectID whocansay;

public:
	bool match(UnityEngine *_vm, objectID speaker);
	void readFrom(Common::SeekableReadStream *stream);
};

class ChangeActionBlock : public ResponseBlock {
public:
	int type;
	uint16 response_id, state_id;

	void readFrom(Common::SeekableReadStream *stream, int _type);
	void execute(UnityEngine *_vm, Object *speaker, Conversation *src);
};

class ResultBlock : public ResponseBlock {
public:
	EntryList entries;

	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm, Object *speaker, Conversation *src);
};

class TextBlock {
public:
	Common::String text;
	uint32 voice_id, voice_group;
	uint16 voice_subgroup;

	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm, Object *speaker);
};

class Response {
public:
	Response();
	~Response();

	uint16 id, state;
	Common::Array<ResponseBlock *> blocks;
	Common::Array<TextBlock *> textblocks;
	Common::Array<WhoCanSayBlock *> whocansayblocks;

	byte response_state;

	uint16 next_situation;
	objectID target;

	Common::String text;
	uint32 voice_id, voice_group;
	uint16 voice_subgroup;

	bool validFor(UnityEngine *_vm, objectID speaker);
	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm, Object *speaker, Conversation *src);
};

class Conversation {
public:
	Conversation();
	~Conversation();

	Common::Array<Response *> responses;
	unsigned int our_world, our_id;

	void loadConversation(UnityData &data, unsigned int world, unsigned int id);
	Response *getResponse(unsigned int response, unsigned int state);
	Response *getEnabledResponse(UnityEngine *_vm, unsigned int response, objectID speaker);
	void execute(UnityEngine *_vm, Object *speaker, unsigned int situation);
	//void execute(UnityEngine *_vm, Object *speaker, unsigned int response, unsigned int state);
};

} // Unity

#endif

