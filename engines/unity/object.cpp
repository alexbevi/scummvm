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

#include "object.h"
#include "unity.h"
#include "sprite.h"
#include "trigger.h"
#include "sound.h"
#include "graphics.h"

#include "common/stream.h"
#include "common/textconsole.h"

namespace Unity {

#define VERIFY_LENGTH(x) do { uint16 block_length = objstream->readUint16LE(); assert(block_length == x); } while (0)

objectID readObjectID(Common::SeekableReadStream *stream) {
	objectID r;
	stream->read(&r, 4);
	assert(r.unused == 0 || r.unused == 0xff);
	return r;
}

objectID readObjectIDBE(Common::SeekableReadStream *stream) {
	objectID r;
	stream->read(&r.unused, 1);
	stream->read(&r.world, 1);
	stream->read(&r.screen, 1);
	stream->read(&r.id, 1);
	assert(r.unused == 0 || r.unused == 0xff);
	return r;
}

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

// TODO: these are 'sanity checks' for verifying global state
bool g_debug_loading_object = false;
objectID g_debug_object_id;
unsigned int g_debug_conv_response, g_debug_conv_state;

Common::String Object::identify() {
	return Common::String::format("%s (%02x%02x%02x)", name.c_str(), id.world, id.screen, id.id);
}

void Object::loadObject(unsigned int for_world, unsigned int for_screen, unsigned int for_id) {
	Common::String filename = Common::String::format("o_%02x%02x%02x.bst", for_world, for_screen, for_id);
	Common::SeekableReadStream *objstream = _vm->data.openFile(filename);

	int type = readBlockHeader(objstream);
	if (type != BLOCK_OBJ_HEADER)
		error("bad block type %x encountered at start of object", type);
	VERIFY_LENGTH(0x128);

	id = readObjectID(objstream);
	assert(id.id == for_id);
	assert(id.screen == for_screen);
	assert(id.world == for_world);

	curr_screen = objstream->readByte();

	byte unknown3 = objstream->readByte(); // XXX: unused?

	width = objstream->readSint16LE();
	height = objstream->readSint16LE();

	x = objstream->readSint16LE();
	y = objstream->readSint16LE();
	z = objstream->readSint16LE();
	universe_x = objstream->readSint16LE();
	universe_y = objstream->readSint16LE();
	universe_z = objstream->readSint16LE();

	// TODO: this doesn't work properly (see DrawOrderComparison)
	y_adjust = objstream->readSint16LE();

	anim_index = objstream->readUint16LE();

	sprite_id = objstream->readUint16LE();
	sprite = NULL;

	region_id = objstream->readUint32LE();

	flags = objstream->readByte();
	state = objstream->readByte();

	uint8 use_count = objstream->readByte();
	uint8 get_count = objstream->readByte();
	uint8 look_count = objstream->readByte();

	uint8 unknown11 = objstream->readByte(); // XXX
	assert(unknown11 == 0);

	objwalktype = objstream->readByte();
	assert(objwalktype <= OBJWALKTYPE_AS);

	uint8 description_count = objstream->readByte();

	char _name[20];
	objstream->read(_name, 20);
	name = _name;

	// pointers for DESCRIPTION, USE, LOOK, WALK and TIME,
	// which we probably don't care about?
	for (unsigned int i = 0; i < 5; i++) {
		uint16 unknowna = objstream->readUint16LE();
		byte unknownb = objstream->readByte();
		byte unknownc = objstream->readByte();
		if (i == 0) {
			// but we do want to snaffle this, lurking in the middle
			transition = readObjectID(objstream);
		}
	}

	skills = objstream->readUint16LE();
	timer = objstream->readUint16LE();

	char _str[101];
	objstream->read(_str, 100);
	_str[100] = 0;
	setTalkString(_str);

	uint16 unknown19 = objstream->readUint16LE(); // XXX
	assert(unknown19 == 0x0);
	uint16 unknown20 = objstream->readUint16LE(); // XXX
	assert(unknown20 == 0x0);

	uint16 zero16 = objstream->readUint16LE();
	assert(zero16 == 0x0);

	uint16 unknown21 = objstream->readUint16LE(); // XXX: unused?

	voice_id = objstream->readUint32LE();
	voice_group = objstream->readUint32LE();
	voice_subgroup = objstream->readUint16LE();

	cursor_id = objstream->readByte();
	cursor_flag = objstream->readByte();

	byte unknown26 = objstream->readByte(); // XXX
	byte unknown27 = objstream->readByte(); // XXX

	zero16 = objstream->readUint16LE();
	assert(zero16 == 0x0);

	for (unsigned int i = 0; i < 21; i++) {
		uint32 padding = objstream->readUint32LE();
		assert(padding == 0x0);
	}

	int blockType;
	while ((blockType = readBlockHeader(objstream)) != -1) {
		g_debug_object_id = id;
		g_debug_loading_object = true;
		readBlock(blockType, objstream);
		g_debug_loading_object = false;
	}

	assert(descriptions.size() == description_count);
	assert(use_entries.list.size() == use_count);
	assert(get_entries.list.size() == get_count);
	assert(look_entries.list.size() == look_count);
	assert(timer_entries.list.size() <= 1); // timers can only have one result

	delete objstream;
}

void Object::loadSprite() {
	if (sprite) return;

	if (sprite_id == 0xffff || sprite_id == 0xfffe) return;

	Common::String sprfilename = _vm->data.getSpriteFilename(sprite_id);
	sprite = new SpritePlayer(new Sprite(_vm->data.openFile(sprfilename)), this, _vm);
	sprite->startAnim(0); // XXX
}

int readBlockHeader(Common::SeekableReadStream *objstream) {
	byte type = objstream->readByte();
	if (objstream->eos()) return -1;

	byte header = objstream->readByte();
	assert(header == 0x11);

	return type;
}

void Object::readBlock(int type, Common::SeekableReadStream *objstream) {
	switch (type) {
		case BLOCK_DESCRIPTION:
			// technically these are always first, but we'll be forgiving
			readDescriptions(objstream);
			break;

		case BLOCK_USE_ENTRIES:
			use_entries.readEntryList(objstream);
			break;

		case BLOCK_GET_ENTRIES:
			get_entries.readEntryList(objstream);
			break;

		case BLOCK_LOOK_ENTRIES:
			look_entries.readEntryList(objstream);
			break;

		case BLOCK_TIMER_ENTRIES:
			timer_entries.readEntryList(objstream);
			break;

		default:
			// either an invalid block type, or it shouldn't be here
			error("bad block type %x encountered while parsing object", type);
	}
}

void EntryList::readEntryList(Common::SeekableReadStream *objstream) {
	byte num_entries = objstream->readByte();

	for (unsigned int i = 0; i < num_entries; i++) {
		int type = readBlockHeader(objstream);
		if (type != BLOCK_BEGIN_ENTRY)
			error("bad block type %x encountered while parsing entry", type);
		VERIFY_LENGTH(0xae);

		uint16 header = objstream->readUint16LE();
		assert(header == 9);

		// XXX: seeking over important data!
		// this seems to be offsets etc, unimportant?
		objstream->seek(0xac, SEEK_CUR);

		// XXX: horrible
		list.push_back(new Common::Array<Entry *>());

		while (true) {
			type = readBlockHeader(objstream);

			if (type == BLOCK_END_ENTRY)
				break;

			readEntry(type, objstream);
		}
	}
}

void Entry::readHeaderFrom(Common::SeekableReadStream *stream, byte header_type) {
	// in objects: this is the object id
	// in states: 'id' is the state id, the rest are zero?
	internal_obj.id = stream->readByte();
	internal_obj.screen = stream->readByte();
	internal_obj.world = stream->readByte();
	if (g_debug_loading_object) {
		assert(internal_obj.id == g_debug_object_id.id);
		assert(internal_obj.screen == g_debug_object_id.screen);
		assert(internal_obj.world == g_debug_object_id.world);
	} else {
		assert(internal_obj.id == g_debug_conv_state);
		assert(internal_obj.screen == 0);
		assert(internal_obj.world == 0);
	}

	// counters for offset within parent blocks (inner/outer)
	// TODO: add g_debug checks for these
	counter1 = stream->readByte();
	counter2 = stream->readByte();
	counter3 = stream->readByte();
	counter4 = stream->readByte();

	// misc header stuff
	byte h_type = stream->readByte();
	assert(h_type == header_type);

	stop_here = stream->readByte();
	assert(stop_here == 0 || stop_here == 1 || stop_here == 0xff);

	// the state/response of this conversation block?, or 0xffff in an object
	response_counter = stream->readUint16LE();
	state_counter = stream->readUint16LE();
	if (g_debug_loading_object) {
		assert(response_counter == 0xffff);
		assert(state_counter == 0xffff);
	} else {
		assert(response_counter == g_debug_conv_response);
		assert(state_counter == g_debug_conv_state);
	}
}

bool valid_world_id(uint16 world_id) {
	return world_id == 0x5f || world_id == 0x10 || world_id == 0 || (world_id >= 2 && world_id <= 7);
}

void ConditionBlock::readFrom(Common::SeekableReadStream *objstream) {
	readHeaderFrom(objstream, 0xff);

	// target
	target = readObjectID(objstream);
	assert(target.world == 0xff || valid_world_id(target.world));

	// WhoCan?
	WhoCan = readObjectID(objstream);
	assert(WhoCan.world == 0xff || valid_world_id(WhoCan.world));

	for (unsigned int i = 0; i < 4; i++) {
		condition[i] = readObjectID(objstream);
		assert(condition[i].world == 0xff || valid_world_id(condition[i].world));

		check_x[i] = objstream->readUint16LE();
		check_y[i] = objstream->readUint16LE();
		check_unknown[i] = objstream->readUint16LE(); // unused check_z?
		check_univ_x[i] = objstream->readUint16LE();
		check_univ_y[i] = objstream->readUint16LE();
		check_univ_z[i] = objstream->readUint16LE();
		check_screen[i] = objstream->readByte();
		check_status[i] = objstream->readByte();
		check_state[i] = objstream->readByte();

		switch (i) {
			case 0:
			case 1:
				assert(check_univ_x[i] == 0xffff);
				assert(check_univ_y[i] == 0xffff);
				assert(check_univ_z[i] == 0xffff);
				break;
			case 2:
			case 3:
				assert(check_x[i] == 0xffff);
				assert(check_y[i] == 0xffff);
				assert(check_unknown[i] == 0xffff);
				assert(check_univ_x[i] == 0xffff);
				assert(check_univ_y[i] == 0xffff);
				assert(check_univ_z[i] == 0xffff);
				break;
		}
	}

	uint16 unknown1 = objstream->readUint16LE();
	assert(unknown1 == 0xffff);
	uint16 unknown2 = objstream->readUint16LE();
	assert(unknown2 == 0xffff);
	uint16 unknown3 = objstream->readUint16LE();
	assert(unknown3 == 0xffff);

	how_close_x = objstream->readUint16LE();
	how_close_y = objstream->readUint16LE();

	uint16 unknown4 = objstream->readUint16LE();
	assert(unknown4 == 0xffff);

	how_close_dist = objstream->readUint16LE();

	// TODO: zero skill check becomes -1 at load
	skill_check = objstream->readUint16LE();

	counter_value = objstream->readUint16LE();
	counter_when = objstream->readByte();

	for (unsigned int i = 0; i < 25; i++) {
		uint32 zero = objstream->readUint32LE();
		assert(zero == 0);
	}
}

void AlterBlock::readFrom(Common::SeekableReadStream *objstream) {
	readHeaderFrom(objstream, 0x43);

	target = readObjectID(objstream);

	alter_flags = objstream->readByte();
	alter_reset = objstream->readByte();

	alter_timer = objstream->readUint16LE(); // set timer..

	uint16 unknown16 = objstream->readUint16LE();
	assert(unknown16 == 0xffff);

	alter_anim = objstream->readUint16LE();
	alter_state = objstream->readByte();

	play_description = objstream->readByte();

	x_pos = objstream->readUint16LE();
	y_pos = objstream->readUint16LE();
	// z_pos?
	unknown8 = objstream->readUint16LE();

	// these are always 0xffff inside objects..
	universe_x = objstream->readUint16LE();
	universe_y = objstream->readUint16LE();
	universe_z = objstream->readUint16LE();

	char text[101];
	objstream->read(text, 20);
	text[20] = 0;
	alter_name = text;
	objstream->read(text, 100);
	text[100] = 0;
	alter_hail = text;

	uint32 unknown32 = objstream->readUint32LE();
	assert(unknown32 == 0xffffffff);

	voice_id = objstream->readUint32LE();
	voice_group = objstream->readUint32LE();
	voice_subgroup = objstream->readUint16LE();
	// the voice_subgroup is not always changed..
	assert(voice_group != 0xffffffff || voice_subgroup == 0xffff);
	// .. neither is the voice_id
	assert(voice_group != 0xffffffff || voice_id == 0xffffffff);

	// talk begin/end stuff??
	unknown11 = objstream->readByte();
	unknown12 = objstream->readByte();
	// random checks
	assert(unknown11 == 0xff || unknown11 == 0 || unknown11 == 1 || unknown11 == 2 || unknown11 == 3);
	assert(unknown12 == 0xff || unknown12 == 0 || unknown12 == 1);

	// zeros
	for (unsigned int i = 0; i < 21; i++) {
		unknown32 = objstream->readUint32LE();
		assert(unknown32 == 0);
	}
}

void ReactionBlock::readFrom(Common::SeekableReadStream *objstream) {
	readHeaderFrom(objstream, 0x44);

	target = readObjectID(objstream);
	dest_world = objstream->readUint16LE();
	dest_screen = objstream->readUint16LE();
	dest_entrance = objstream->readUint16LE();
	target_type = objstream->readByte();
	action_type = objstream->readByte();
	damage_amount = objstream->readByte();
	beam_type = objstream->readByte();
	dest_x = objstream->readUint16LE();
	dest_y = objstream->readUint16LE();
	dest_unknown = objstream->readUint16LE();

	assert(target_type >= 1 && target_type <= 7);
	if (target_type != 6) {
		assert(target.id == 0xff);
	} else {
		assert(target.id != 0xff);
	}
	if (damage_amount == 0) {
		assert(action_type <= 3);
		// otherwise it can be 0, 1 or ff, but afaik just junk
	}

	for (unsigned int i = 0; i < 100; i++) {
		byte unknown = objstream->readByte();
		assert(unknown == 0);
	}
}

void CommandBlock::readFrom(Common::SeekableReadStream *objstream) {
	readHeaderFrom(objstream, 0x45);

	target[0] = readObjectID(objstream);
	target[1] = readObjectID(objstream);
	target[2] = readObjectID(objstream);

	// both usually 0xffff
	target_x = objstream->readUint16LE();
	target_y = objstream->readUint16LE();

	command_id = objstream->readUint32LE();

	assert(command_id >= 2 && command_id <= 6);

	for (unsigned int i = 0; i < 97; i++) {
		byte unknown = objstream->readByte();
		assert(unknown == 0);
	}
}

void ScreenBlock::readFrom(Common::SeekableReadStream *objstream) {
	readHeaderFrom(objstream, 0x46);

	new_screen = objstream->readByte();
	new_entrance = objstream->readByte();
	advice_screen = objstream->readByte();
	new_advice_id = objstream->readUint16LE();
	new_world = objstream->readUint16LE();

	unknown6 = objstream->readUint16LE();
	unknown7 = objstream->readUint32LE();
	unknown8 = objstream->readByte();
	unknown9 = objstream->readUint32LE();
	unknown10 = objstream->readUint32LE();
	unknown11 = objstream->readUint16LE();
	assert(unknown11 == 0xffff);
	unknown12 = objstream->readUint16LE();
	assert(unknown12 == 0xffff);

	unknown13 = objstream->readUint16LE();
	unknown14 = objstream->readByte();
	unknown15 = objstream->readByte();
	unknown16 = objstream->readByte();

	new_advice_timer = objstream->readUint16LE();

	for (unsigned int i = 0; i < 24; i++) {
		uint32 zero = objstream->readUint32LE();
		assert(zero == 0);
	}
}

void PathBlock::readFrom(Common::SeekableReadStream *objstream) {
	readHeaderFrom(objstream, 0x47);

	objstream->seek(0x16c, SEEK_CUR); // XXX
}

void GeneralBlock::readFrom(Common::SeekableReadStream *objstream) {
	readHeaderFrom(objstream, 0x48);

	movie_id = objstream->readUint16LE();

	unknown1 = objstream->readUint16LE(); // XXX
	unknown2 = objstream->readUint16LE(); // XXX
	unknown3 = objstream->readUint16LE(); // XXX

	for (unsigned int i = 0; i < 0x64; i++) {
		byte zero = objstream->readByte();
		assert(zero == 0);
	}
}

void ConversationBlock::readFrom(Common::SeekableReadStream *objstream) {
	readHeaderFrom(objstream, 0x49);

	world_id = objstream->readUint16LE();
	assert(world_id == 0xffff || valid_world_id(world_id));
	conversation_id = objstream->readUint16LE();
	response_id = objstream->readUint16LE();
	state_id = objstream->readUint16LE();

	action_id = objstream->readUint16LE();
	assert(action_id == RESPONSE_ENABLED || action_id == RESPONSE_DISABLED ||
		action_id == RESPONSE_UNKNOWN1);

	for (unsigned int i = 0; i < 0x63; i++) {
		byte zero = objstream->readByte();
		assert(zero == 0);
	}
}

void BeamBlock::readFrom(Common::SeekableReadStream *objstream) {
	readHeaderFrom(objstream, 0x4a);

	// world id, or 00 to kill?
	world_id = objstream->readUint16LE();

	// unused?
	unknown1 = objstream->readUint16LE();
	assert(unknown1 == 0xffff || unknown1 == 1 || unknown1 == 4);

	uint16 unknown2 = objstream->readUint16LE();
	assert(unknown2 == 0xffff);
	unknown2 = objstream->readUint16LE();
	assert(unknown2 == 0xffff);
	unknown2 = objstream->readUint16LE();
	assert(unknown2 == 0xffff);

	// unused?
	unknown3 = objstream->readUint16LE();
	assert(unknown3 == 0 || unknown3 == 0xffff);

	unknown2 = objstream->readUint16LE();
	assert(unknown2 == 0xffff);
	unknown2 = objstream->readUint16LE();
	assert(unknown2 == 0xffff);
	unknown2 = objstream->readUint16LE();
	assert(unknown2 == 0xffff);

	screen_id = objstream->readUint16LE();
	assert(screen_id <= 4 || screen_id == 0xffff);

	// from here this is all unused junk, i think
	uint16 unknown5 = objstream->readUint16LE();
	uint16 unknown6 = objstream->readUint16LE();
	assert(unknown5 == 0xffff || unknown5 == 0 || unknown5 == 0x60);
	assert(unknown6 == 0xffff || unknown6 == 0x20 || unknown6 == 0x1b);
	for (unsigned int i = 0; i < 3; i++) {
		unknown2 = objstream->readUint16LE();
		assert(unknown2 == 0x0);
		uint32 unknown32 = objstream->readUint32LE();
		assert(unknown32 == 0xffffffff);
	}
	for (unsigned int i = 0; i < 51; i++) {
		unknown2 = objstream->readUint16LE();
		assert(unknown2 == 0x0);
	}
}

void TriggerBlock::readFrom(Common::SeekableReadStream *objstream) {
	readHeaderFrom(objstream, 0x4b);

	trigger_id = objstream->readUint32LE();

	byte flag = objstream->readByte();
	enable_trigger = (flag == 1);

	for (unsigned int i = 0; i < 25; i++) {
		uint32 unknown32 = objstream->readUint32LE();
		assert(unknown32 == 0);
	}
}

void CommunicateBlock::readFrom(Common::SeekableReadStream *objstream) {
	readHeaderFrom(objstream, 0x4c);

	target = readObjectID(objstream);

	if (target.id == 0xff) {
		assert(target.screen == 0xff);
		assert(target.world == 0xff);
		assert(target.unused == 0xff);
	}

	conversation_id = objstream->readUint16LE();
	situation_id = objstream->readUint16LE();
	hail_type = objstream->readByte();
	assert(hail_type <= 0x10);

	for (unsigned int i = 0; i < 25; i++) {
		uint32 unknown32 = objstream->readUint32LE();
		assert(unknown32 == 0);
	}
}

void ChoiceBlock::readFrom(Common::SeekableReadStream *objstream) {
	readHeaderFrom(objstream, 0x4d);

	// TODO: x/y?
	_unknown1 = objstream->readUint16LE();
	_unknown2 = objstream->readUint16LE();

	char buffer[101];
	buffer[100] = 0;
	objstream->read(buffer, 100);
	_questionstring = buffer;

	buffer[16] = 0;
	objstream->read(buffer, 16);
	_choicestring[0] = buffer;
	objstream->read(buffer, 16);
	_choicestring[1] = buffer;

	_object = readObjectID(objstream);

	// offsets and counts for the choices, it seems; we don't need this
	uint32 offset1 = objstream->readUint32LE();
	uint32 offset2 = objstream->readUint32LE();
	uint16 count1 = objstream->readUint16LE();
	uint16 count2 = objstream->readUint16LE();
	(void)offset1; (void)offset2; (void)count1; (void)count2;

	for (unsigned int i = 0; i < 25; i++) {
		uint32 unknown32 = objstream->readUint32LE();
		assert(unknown32 == 0);
	}
}

void EntryList::readEntry(int type, Common::SeekableReadStream *objstream) {
	uint16 header;

	// XXX: horrible
	Common::Array<Entry *> &entries = *list[list.size() -1 ];

	switch (type) {
		case BLOCK_CONDITION:
			VERIFY_LENGTH(0xda);
			header = objstream->readUint16LE();
			assert(header == 9);

			{
			ConditionBlock *block = new ConditionBlock();
			block->readFrom(objstream);
			entries.push_back(block);
			}
			break;

		case BLOCK_ALTER:
			while (true) {
				VERIFY_LENGTH(0x105);
				header = objstream->readUint16LE();
				assert(header == 9);

				AlterBlock *block = new AlterBlock();
				block->readFrom(objstream);
				entries.push_back(block);

				type = readBlockHeader(objstream);
				if (type == BLOCK_END_BLOCK)
					break;
				if (type != BLOCK_ALTER)
					error("bad block type %x encountered while parsing alters", type);
			}
			break;

		case BLOCK_REACTION:
			while (true) {
				VERIFY_LENGTH(0x87);
				header = objstream->readUint16LE();
				assert(header == 9);

				ReactionBlock *block = new ReactionBlock();
				block->readFrom(objstream);
				entries.push_back(block);

				type = readBlockHeader(objstream);
				if (type == BLOCK_END_BLOCK)
					break;
				if (type != BLOCK_REACTION)
					error("bad block type %x encountered while parsing reactions", type);
			}
			break;

		case BLOCK_COMMAND:
			while (true) {
				VERIFY_LENGTH(0x84);
				header = objstream->readUint16LE();
				assert(header == 9);

				CommandBlock *block = new CommandBlock();
				block->readFrom(objstream);
				entries.push_back(block);

				type = readBlockHeader(objstream);
				if (type == BLOCK_END_BLOCK)
					break;
				if (type != BLOCK_COMMAND)
					error("bad block type %x encountered while parsing commands", type);
			}
			break;

		case BLOCK_SCREEN:
			VERIFY_LENGTH(0x90);
			header = objstream->readUint16LE();
			assert(header == 9);
			{
			ScreenBlock *block = new ScreenBlock();
			block->readFrom(objstream);
			entries.push_back(block);
			}
			break;

		case BLOCK_PATH:
			VERIFY_LENGTH(0x17b);
			header = objstream->readUint16LE();
			assert(header == 9);
			{
			PathBlock *block = new PathBlock();
			block->readFrom(objstream);
			entries.push_back(block);
			}
			break;

		case BLOCK_GENERAL:
			VERIFY_LENGTH(0x7b);
			header = objstream->readUint16LE();
			assert(header == 9);
			{
			GeneralBlock *block = new GeneralBlock();
			block->readFrom(objstream);
			entries.push_back(block);
			}
			break;

		case BLOCK_CONVERSATION:
			while (true) {
				VERIFY_LENGTH(0x7c);
				header = objstream->readUint16LE();
				assert(header == 9);

				ConversationBlock *block = new ConversationBlock();
				block->readFrom(objstream);
				entries.push_back(block);

				type = readBlockHeader(objstream);
				if (type == BLOCK_END_BLOCK)
					break;
				if (type != BLOCK_CONVERSATION)
					error("bad block type %x encountered while parsing conversation", type);
			}
			break;

		case BLOCK_BEAMDOWN:
			VERIFY_LENGTH(0x9f);
			header = objstream->readUint16LE();
			assert(header == 9);
			{
			BeamBlock *block = new BeamBlock();
			block->readFrom(objstream);
			entries.push_back(block);
			}
			break;

		case BLOCK_TRIGGER:
			while (true) {
				VERIFY_LENGTH(0x78);
				header = objstream->readUint16LE();
				assert(header == 9);

				TriggerBlock *block = new TriggerBlock();
				block->readFrom(objstream);
				entries.push_back(block);

				type = readBlockHeader(objstream);
				if (type == BLOCK_END_BLOCK)
					break;
				if (type != BLOCK_TRIGGER)
					error("bad block type %x encountered while parsing triggers", type);
			}
			break;

		case BLOCK_COMMUNICATE:
			VERIFY_LENGTH(0x7c);
			header = objstream->readUint16LE();
			assert(header == 9);
			{
			CommunicateBlock *block = new CommunicateBlock();
			block->readFrom(objstream);
			entries.push_back(block);
			}
			break;

		case BLOCK_CHOICE:
			VERIFY_LENGTH(0x10b);
			header = objstream->readUint16LE();
			assert(header == 9);
			{
			ChoiceBlock *block = new ChoiceBlock();
			block->readFrom(objstream);
			entries.push_back(block);

			while (true) {
				type = readBlockHeader(objstream);
				if (type == BLOCK_END_BLOCK)
					break;

				if (type == BLOCK_CHOICE1) {
					type = readBlockHeader(objstream);
					// XXX: horrible
					block->_choice[0].list.push_back(new Common::Array<Entry *>());
					block->_choice[0].readEntry(type, objstream);
				} else if (type == BLOCK_CHOICE2) {
					type = readBlockHeader(objstream);
					// XXX: horrible
					block->_choice[1].list.push_back(new Common::Array<Entry *>());
					block->_choice[1].readEntry(type, objstream);
				} else
					error("bad block type %x encountered while parsing choices", type);
			}
			}
			break;

		default:
			error("bad block type %x encountered while parsing entries", type);
	}
}

void Object::readDescriptions(Common::SeekableReadStream *objstream) {
	readDescriptionBlock(objstream);

	while (true) {
		int type = readBlockHeader(objstream);
		switch (type) {
			case BLOCK_DESCRIPTION:
				readDescriptionBlock(objstream);
				break;

			case BLOCK_END_BLOCK:
				return;

			default:
				error("bad block type %x encountered while parsing object", type);
		}
	}
}

void Object::readDescriptionBlock(Common::SeekableReadStream *objstream) {
	VERIFY_LENGTH(0xa5);

	byte entry_for = objstream->readByte();

	for (unsigned int i = 0; i < 7; i++) {
		uint16 unknowna = objstream->readUint16LE();
		assert(unknowna == 0xffff);
	}

	char text[150];
	objstream->read(text, 149);
	text[150] = 0;

	// XXX: is this just corrupt entries and there should be a null byte here?
	byte unknown2 = objstream->readByte();

	int type = readBlockHeader(objstream);
	if (type != BLOCK_SPEECH_INFO)
		error("bad block type %x encountered while parsing description", type);
	VERIFY_LENGTH(0x0c);

	Description desc;
	desc.text = text;
	desc.entry_id = entry_for;

	desc.voice_id = objstream->readUint32LE();
	desc.voice_group = objstream->readUint32LE();
	desc.voice_subgroup = objstream->readUint32LE();

	descriptions.push_back(desc);
}

void Object::changeTalkString(const Common::String &str) {
	bool immediate = (str.size() && str[0] == '1');
	if (immediate) {
		// run the conversation immediately, don't change anything
		// TODO: this should be *queued* to be run once we're done (or be wiped by fail)
		// TODO: voice stuff
		runHail(str);
	} else {
		debug(1, "talk string of %s changing from '%s' to '%s'", identify().c_str(),
			talk_string.c_str(), str.c_str());
		setTalkString(str);
	}
}

void Object::setTalkString(const Common::String &str) {
	if (!str.size()) {
		talk_string.clear();
		return;
	}

	if (str[0] == '1') error("unexpected immediate talk string '%s'", str.c_str());

	// TODO: 'abc     ---hello---' should be stripped to 'abchello'
	// (skip two sets of dashes, then spaces)

	talk_string = str;
}

void Object::runHail(const Common::String &hail) {
	debug(1, "%s running hail '%s'", identify().c_str(), hail.c_str());

	// TODO: check OBJFLAG_ACTIVE?
	// TODO: use source of change if an away team member

	if (hail.size() < 2) {
		error("failed to parse hail '%s'", hail.c_str());
	}

	bool immediate = (hail[0] == '1');
	if ((immediate && hail[1] != '@') || (!immediate && hail[0] != '@')) {
		_vm->_dialog_text = hail.c_str() + (immediate ? 1 : 0);

		// TODO: this is VERY not good
		_vm->setSpeaker(id);
		debug(1, "%s says '%s'", identify().c_str(), hail.c_str());

		_vm->runDialog();

		// text, not a proper hail
		return;
	}

	int world, conversation, situation;
	if (sscanf(hail.begin() + (immediate ? 2 : 1), "%d,%d,%d",
		&world, &conversation, &situation) != 3) {
		// try two-parameter form (with default world)
		world = _vm->data.current_screen.world;
		if (sscanf(hail.begin() + (immediate ? 2 : 1), "%d,%d",
			&conversation, &situation) != 2) {
			error("failed to parse hail '%s'", hail.c_str());
		}
	}

	// TODO: not always Picard! :(
	objectID speaker = objectID(0, 0, 0);
	if (_vm->_current_away_team_member) speaker = _vm->_current_away_team_member->id;

	Conversation *conv = _vm->data.getConversation(world, conversation);
	conv->execute(_vm, _vm->data.getObject(speaker), situation);
}

EntryList::~EntryList() {
	for (unsigned int i = 0; i < list.size(); i++) {
		for (unsigned int j = 0; j < list[i]->size(); j++) {
			delete (*list[i])[j];
		}
		delete list[i];
	}
}

ResultType EntryList::execute(UnityEngine *_vm, Action *context) {
	debug(1, "");
	ResultType r = 0;
	for (unsigned int i = 0; i < list.size(); i++) {
		debug(1, "EntryList::execute: block %d of %d (size %d)", i + 1, list.size(), list[i]->size());
		bool run = true;
		r &= (RESULT_WALKING | RESULT_MATCHOTHER | RESULT_DIDSOMETHING);
		for (unsigned int j = 0; j < list[i]->size(); j++) {
			r |= (*list[i])[j]->check(_vm, context);
			if (r & ~(RESULT_MATCHOTHER | RESULT_DIDSOMETHING)) {
				run = false;
				break;
			}
		}
		if (!run) continue;
		for (unsigned int j = 0; j < list[i]->size(); j++) {
			r |= (*list[i])[j]->execute(_vm, context);
			r |= RESULT_DIDSOMETHING;
			if ((*list[i])[j]->stop_here) {
				r |= RESULT_STOPPED;
				debug(1, "EntryList::execute: stopped at entry %d of %d", j + 1, list[i]->size());
				break;
			}
		}
		if (r & RESULT_STOPPED) break;
	}
	debug(1, "EntryList::execute: done with %d blocks", list.size());
	debug(1, "");
	return r;
}

ResultType ConditionBlock::check(UnityEngine *_vm, Action *context) {
	debug(1, "ConditionBlock::check: %02x%02x%02x, %02x%02x%02x",
		target.world, target.screen, target.id,
		WhoCan.world, WhoCan.screen, WhoCan.id);

	ResultType r = 0;

	if (target.world == 0xff && target.screen == 0xff && target.id == 0xfe) {
		// fail if other was passed in
		printf("(checking if other was set) ");
		if (context->other.id != 0xff) {
			printf("-- it was!\n");
			return RESULT_FAILOTHER;
		}
	} else if (target.world != 0xff && target.screen != 0xff && target.id != 0xff) {
		// fail if source != other
		printf("(checking if target %02x%02x%02x matches other %02x%02x%02x) ",
			target.world, target.screen, target.id,
			context->other.world, context->other.screen, context->other.id);
		if (target != context->other) {
			printf("-- it doesn't!\n");
			return RESULT_FAILOTHER;
		}
		r |= RESULT_MATCHOTHER;
	}

	if (how_close_dist != 0xffff) {
		// TODO: fail if `who' object doesn't exist?
		uint16 x = how_close_x;
		uint16 y = how_close_y;
		if (x == 0xffff) {
			// TODO: if source has OBJFLAG_INVENTORY, take `who' position
			// otherwise, take source position
		}
		// TODO: if squared distance between `who' and x/y > how_close_dist*how_close_dist {
		//	run walk action with `who', return RESULT_WALKING|RESULT_DIDSOMETHING
		// }
		// TODO: (wth? checking `who' vs `who'?)
		warning("unimplemented: ConditionCheck: ignoring HowClose");
	}

	if ((WhoCan.id != 0xff) &&
		!((WhoCan.world == 0) && (WhoCan.screen) == 0 && (WhoCan.id == 0x10))) {
		printf("(checking if WhoCan %02x%02x%02x matches who %02x%02x%02x) ",
			WhoCan.world, WhoCan.screen, WhoCan.id,
			context->who.world, context->who.screen, context->who.id);
		if (WhoCan != context->who) {
			printf(" -- nope\n");
			return r | RESULT_AWAYTEAM;
		}
	}

	// TODO: skill level of who or RESULT_FAILSKILL

	if (counter_when != 0xff) {
		// counter_flag = 1;

		if (counter_when == 1) {
			// do when
			if (counter_value) {
				counter_value--;
				printf("counter: not yet\n");
				return r | RESULT_COUNTER_DOWHEN;
			}
		} else if (counter_when == 0) {
			// do until
			if (counter_value == 0xffff) {
				printf("counter: not any more\n");
				return r | RESULT_COUNTER_DOUNTIL;
			}
		}
		counter_value--;
	}

	bool did_something = false;
	for (unsigned int i = 0; i < 4; i++) {
		// TODO: some special handling of check_x/check_y for invalid obj?
		if (condition[i].world == 0xff) continue;

		Object *obj;
		if (condition[i].world == 0 && condition[i].screen == 0 && condition[i].id == 0x10) {
			printf("(got away team member) ");
			// TODO: what to do if not on away team?
			obj = _vm->_current_away_team_member;
		} else {
			obj = _vm->data.getObject(condition[i]);
		}
		printf("checking state of %s", obj->identify().c_str());

		if (check_state[i] != 0xff) {
			printf(" (is state %x?)", check_state[i]);
			if (obj->state != check_state[i]) {
				printf(" -- nope!\n");
				return r | RESULT_FAILSTATE;
			}
		}

		if (!(obj->flags & OBJFLAG_ACTIVE)) {
			if (check_screen[i] == 0xfe) {
				// check for inactive
				return r;
			}

			if (check_state[i] == 0xff) {
				printf(" (object inactive!)\n");
				return r | RESULT_INACTIVE;
			}
		}

		// TODO: check whether stunned or not?

		if (check_status[i] != 0xff) {
			did_something = true;

			if (check_status[i]) {
				if (obj->id.world == 0 && obj->id.screen == 0 && obj->id.id < 0x10) {
					printf(" (in away team?)");
					if (Common::find(_vm->_away_team_members.begin(),
						_vm->_away_team_members.end(),
						obj) == _vm->_away_team_members.end()) {
						printf(" -- nope!\n");
						return r | RESULT_AWAYTEAM;
					}
				} else {
					printf(" (in inventory?)");
					if (!(obj->flags & OBJFLAG_INVENTORY)) {
						printf(" -- nope!\n");
						return r | RESULT_INVENTORY;
					}
				}
			} else {
				// note that there is no inverse away team check
				printf(" (not in inventory?)");
				if (obj->flags & OBJFLAG_INVENTORY) {
					printf(" -- it is!\n");
					return r | RESULT_INVENTORY;
				}
			}
		}

		if (check_x[i] != 0xffff) {
			did_something = true;
			printf(" (is x/y %x/%x?)", check_x[i], check_y[i]);
			if (obj->x != check_x[i] || obj->y != check_y[i]) {
				printf(" -- nope!\n");
				return r | RESULT_FAILPOS;
			}
		}

		// (check_unknown unused?)

		if (check_univ_x[i] != 0xffff) {
			did_something = true;
			printf(" (is universe x/y/z %x/%x/%x?)", check_univ_x[i], check_univ_y[i], check_univ_z[i]);
			if (obj->universe_x != check_univ_x[i] || obj->universe_y != check_univ_y[i] ||
				obj->universe_z != check_univ_z[i]) {
				printf(" -- nope!\n");
				return r | RESULT_FAILPOS;
			}
		}

		if (check_screen[i] != 0xff) {
			did_something = true;
			printf(" (is screen %x?)", check_screen[i]);

			if (obj->curr_screen != check_screen[i]) {
				printf(" -- nope!\n");
				return r | RESULT_FAILSCREEN;
			}
		}

		printf("\n");
	}

	// TODO: stupid hardcoded tricorder sound

	return r;
}

ResultType ConditionBlock::execute(UnityEngine *_vm, Action *context) {
	return 0; // nothing to do
}

ResultType AlterBlock::execute(UnityEngine *_vm, Action *context) {
	debug(1, "Alter: on %02x%02x%02x", target.world, target.screen, target.id);

	Object *obj;
	if (target.world == 0 && target.screen == 0 && target.id == 0x10) {
		// TODO: use 'who' from context
		obj = _vm->data.getObject(objectID(0, 0, 0));
	} else {
		obj = _vm->data.getObject(target);
	}

	bool did_something = false; // just for debugging

	if (alter_flags || alter_reset) {
		did_something = true;

		// magical flags!

		if (alter_reset & 0x01) {
			// activate
			debug(1, "AlterBlock::execute (%s): activating", obj->identify().c_str());
			obj->flags |= OBJFLAG_ACTIVE;
		}
		if (alter_reset & ~0x01) {
			warning("unimplemented: AlterBlock::execute (%s): alter_reset %x", obj->identify().c_str(), alter_reset);
		}

		if (alter_flags & 0x1) {
			// TODO: talk? - run immediately (action 5), who is worldobj+16(??)
			warning("unimplemented: AlterBlock::execute (%s): alter_flag 1", obj->identify().c_str());
		}
		if (alter_flags & 0x02) {
			error("invalid AlterBlock flag 0x02");
		}
		if (alter_flags & 0x4) {
			// drop
			debug(1, "AlterBlock::execute (%s): dropping", obj->identify().c_str());
			obj->flags &= ~OBJFLAG_INVENTORY;

			if (Common::find(_vm->_inventory_items.begin(),
				_vm->_inventory_items.end(), obj) != _vm->_inventory_items.end()) {
				_vm->removeFromInventory(obj);
			}
		}
		if (alter_flags & 0x8) {
			// get
			debug(1, "AlterBlock::execute (%s): getting", obj->identify().c_str());
			if (!(obj->flags & OBJFLAG_INVENTORY)) {
				obj->flags |= (OBJFLAG_ACTIVE | OBJFLAG_INVENTORY);
			}

			if (Common::find(_vm->_inventory_items.begin(),
				_vm->_inventory_items.end(), obj) == _vm->_inventory_items.end()) {
				_vm->addToInventory(obj);
			}
		}
		if (alter_flags & 0x10) {
			// unstun
			debug(1, "AlterBlock::execute (%s): unstunning", obj->identify().c_str());
			// TODO: special behaviour on away team?
			obj->flags &= ~OBJFLAG_STUNNED;
		}
		if (alter_flags & 0x20) {
			// stun
			debug(1, "AlterBlock::execute (%s): stunning", obj->identify().c_str());
			// TODO: special behaviour on away team?
			obj->flags |= OBJFLAG_STUNNED;
		}
		if (alter_flags & 0x40) {
			// TODO: ?? (anim change?)
			warning("unimplemented: AlterBlock::execute (%s): alter_flag 0x40", obj->identify().c_str());
		}
		if (alter_flags & 0x80) {
			// deactivate
			debug(1, "AlterBlock::execute (%s): deactivating", obj->identify().c_str());
			obj->flags &= ~(OBJFLAG_ACTIVE | OBJFLAG_INVENTORY);
		}
	}

	if (x_pos != 0xffff && y_pos != 0xffff) {
		did_something = true;
		debug(1, "AlterBlock::execute (%s): position (%x, %x)", obj->identify().c_str(), x_pos, y_pos);

		obj->x = x_pos;
		obj->y = y_pos;
		// TODO: z_pos too?
	}

	if (alter_name.size()) {
		did_something = true;
		debug(1, "altering name of %s to %s", obj->identify().c_str(), alter_name.c_str());

		obj->name = alter_name;
	}

	if (alter_hail.size()) {
		did_something = true;

		obj->changeTalkString(alter_hail);
	}

	if (voice_group != 0xffffffff || voice_subgroup != 0xffff || voice_id != 0xffffffff) {
		did_something = true;
		warning("unimplemented: AlterBlock::execute (%s): voice %x/%x/%x", obj->identify().c_str(), voice_group, voice_subgroup, voice_id);

		// TODO: alter voice for the description? set all, if voice_id is not 0xffffffff
	}

	if (alter_state != 0xff) {
		did_something = true;
		debug(1, "AlterBlock::execute (%s): state %x", obj->identify().c_str(), alter_state);

		obj->state = alter_state;
	}

	if (alter_timer != 0xffff) {
		did_something = true;
		debug(1, "AlterBlock::execute (%s): timer %x", obj->identify().c_str(), alter_timer);

		obj->timer = alter_timer;
	}

	if (alter_anim != 0xffff) {
		did_something = true;
		debug(1, "AlterBlock::execute (%s): anim %04x", obj->identify().c_str(), alter_anim);

		uint16 anim_id = alter_anim;

		if (anim_id >= 31000) {
			// TODO (some magic with (alter_anim - 31000))
			// (this sets a different animation type - index?)
			// this doesn't run ANY of the code below
			anim_id -= 31000;
			error("very weird animation id %04x (%d)", alter_anim, anim_id);
		} else if (anim_id >= 29000) {
			if (alter_anim < 30000) {
				anim_id = 30000 - alter_anim;
				// TODO: some other magic?
				// (try going into the third area of Allanor: 020412 tries this on the drone)
				// TODO: this probably just force-stops the animation
				warning("weird animation id %04x (%d)", alter_anim, anim_id);
			} else {
				// TODO: ?!?
				anim_id -= 30000;
			}
		}

		if (obj->sprite) {
			if (obj->sprite->numAnims() <= anim_id) {
				// use the Chodak transporter bar gauge, for example
				warning("animation %d exceeds %d?!", anim_id, obj->sprite->numAnims());
			} else {
				obj->sprite->startAnim(anim_id);
			}
		} else if (obj->id == objectID(0, 1, 0x5f)) {
			// sprite change on the Enterprise
			_vm->_viewscreen_sprite_id = alter_anim;
		} else {
			warning("no sprite?!");
		}
	}

	if (play_description != 0) {
		did_something = true;
		// TODO: is the value of play_description meaningful?
		_vm->playDescriptionFor(obj);
	}

	if (unknown8 != 0xffff) {
		did_something = true;
		// TODO: set y_adjust
		warning("unimplemented: AlterBlock::execute (%s): unknown8 %x", obj->identify().c_str(), unknown8);
	}

	if (universe_x != 0xffff || universe_y != 0xffff || universe_z != 0xffff) {
		did_something = true;
		debug(1, "AlterBlock::execute (%s): warp to universe loc %x, %x, %x", obj->identify().c_str(), universe_x, universe_y, universe_z);

		if (obj->id.world == 0x5f && obj->id.screen == 1 && obj->id.id == 0) {
			// TODO: go into astrogation and start warp to this universe location
			warning("unimplemented: Enterprise warp");
		}
		obj->universe_x = universe_x;
		obj->universe_y = universe_y;
		obj->universe_z = universe_z;
	}

	if (unknown11 != 0xff) {
		// TODO: cursor id
		did_something = true;
		warning("unimplemented: AlterBlock::execute (%s): unknown11 %x", obj->identify().c_str(), unknown11);
	}

	if (unknown12 != 0xff) {
		// TODO: cursor flag
		did_something = true;
		warning("unimplemented: AlterBlock::execute (%s): unknown12 %x", obj->identify().c_str(), unknown12);
	}

	if (!did_something) {
		warning("empty AlterBlock::execute (%s)?", obj->identify().c_str());
	}

	return 0;
}

ResultType ReactionBlock::execute(UnityEngine *_vm, Action *context) {
	warning("unimplemented: ReactionBlock::execute");

	Common::Array<Object *> objects;

	switch (target_type - 1) {
	case 0: // who from context
		// TODO
		break;

	case 2: // all away team members (plus stunned members on current screen?)
		// TODO
		break;

	case 5: // provided object
		// TODO: force onto current screen?
		objects.push_back(_vm->data.getObject(target));
		break;

	case 6: // random away team member
		// TODO
		break;

	default: error("bad reaction target type %d", target_type);
	}

	if (damage_amount == 0) {
		switch (action_type) {
		case 0: // stun all
			// TODO: set health to 1 below minimum
			break;

		case 1: // kill all
			// TODO: set health to 0
			break;

			// TODO: for below, beam_type is 0 for normal (teffect/beamout/beamin),
			// 1 for shodak (chodbeam/acid/acid)

		case 2: // beam in all to dest_x/dest_y
			// TODO:
			break;

		case 3: if (dest_world == 0) {
				// TODO: beam out to ship
			} else {
				// TODO: ensure that dest_world is 0xffff or current world, since
				// you can't beam between worlds

				if (dest_entrance == 0xffff) error("no destination entrance in reaction");

				// TODO: if dest_screen is current screen, beam in to dest_entrance
				// TODO: else, beam out to dest_entrance
			}
			break;
		default: error("bad reaction action type %d", action_type);
		}
	} else if (damage_amount == 0x7f) {
		// heal all objects to 1 above minimum health
		// TODO
	} else {
		// do damage_amount of damage to all objects
		// TODO
		// TODO: if beam_type is set, do phaser effects too
	}

	return 0;
}

ResultType CommandBlock::execute(UnityEngine *_vm, Action *context) {
	debug(1, "CommandBlock: %02x%02x%02x/%02x%02x%02x/%02x%02x%02x, (%d, %d), command %d",
		target[0].world, target[0].screen, target[0].id,
		target[1].world, target[1].screen, target[1].id,
		target[2].world, target[2].screen, target[2].id,
		target_x, target_y, command_id);

	ActionType action_id;
	switch (command_id) {
		case 2: action_id = ACTION_LOOK; break;
		case 3: action_id = ACTION_GET; break;
		case 4: action_id = ACTION_WALK; break;
		case 5: action_id = ACTION_TALK; break;
		case 6: action_id = ACTION_USE; break;
		default: error("unknown command type %x", command_id);
	}

	Object *targ = NULL;
	if (target[0].world != 0xff)
		targ = _vm->data.getObject(target[0]);

	_vm->performAction(action_id, targ, target[1], target[2], target_x, target_y);

	return 0;
}

ResultType ScreenBlock::execute(UnityEngine *_vm, Action *context) {
	if (new_screen != 0xff) {
		debug(1, "ScreenBlock::execute: new screen: %02x, entrance %02x", new_screen, new_entrance);

		byte entrance = new_entrance;
		// TODO: if new_entrance not -1 and matches 0x40, weird stuff with away team?
		// (forcing into source region, etc)
		if (entrance != 0xff) entrance &= ~0x40;
		// TODO: 0x80 can be set too.. see exit from first Unity Device screen
		if (entrance != 0xff) entrance &= ~0x80;

		_vm->startAwayTeam(_vm->data.current_screen.world, new_screen, entrance);
	} else
		warning("unimplemented: ScreenBlock::execute");

	if (advice_screen == 0xff) {
		if (new_advice_id != 0xffff) {
			// TODO: live change of new_advice_id and new_advice_timer for current screen right now
		}
	} else {
		// TODO: set new_advice_id for target screen
		if (new_advice_timer != 0xffff) {
			// TODO: set new_advice_timer for target screen
		}
	}

	if (new_world != 0xffff) {
		// TODO: this is just a guess
		debug(1, "ScreenBlock::execute: new_world (back to bridge?): %04x", new_world);
		_vm->startBridge();
	}

	// TODO: screen to bump?
	if (unknown6 != 0xffff) printf ("6: %04x ", unknown6);

	// unused??
	if (unknown7 != 0xffffffff) printf ("7: %08x ", unknown7);
	if (unknown8 != 0xff) printf ("8: %02x ", unknown8);
	if (unknown9 != 0xffffffff) printf ("9: %08x ", unknown9);
	if (unknown10 != 0xffffffff) printf ("10: %08x ", unknown10);
	if (unknown11 != 0xffff) printf ("11: %04x ", unknown11);
	if (unknown12 != 0xffff) printf ("12: %04x ", unknown12);

	// TODO: screen id + changes for screen?
	if (unknown13 != 0xffff) printf ("13: %04x ", unknown13);
	if (unknown14 != 0xff) printf ("14: %02x ", unknown14);
	if (unknown15 != 0xff) printf ("15: %02x ", unknown15);
	if (unknown16 != 0xff) printf ("16: %02x ", unknown16);

	printf("\n");

	return 0;
}

ResultType PathBlock::execute(UnityEngine *_vm, Action *context) {
	warning("unimplemented: PathBlock::execute");

	return 0;
}

ResultType GeneralBlock::execute(UnityEngine *_vm, Action *context) {
	if (unknown1 != 0xffff) {
		warning("unimplemented: GeneralBlock::execute: unknown1 %04x", unknown1);
	}
	if (unknown2 != 0) {
		warning("unimplemented: GeneralBlock::execute: unknown2 %04x", unknown2);
	}
	if (unknown3 != 0xffff) {
		warning("unimplemented: GeneralBlock::execute: unknown3 %04x", unknown3);
	}
	if (movie_id != 0xffff) {
		assert(_vm->data.movie_filenames.contains(movie_id));

		debug(1, "GeneralBlock: play movie %d (%s: '%s')", movie_id,
			_vm->data.movie_filenames[movie_id].c_str(),
			_vm->data.movie_descriptions[movie_id].c_str());

		_vm->_gfx->playMovie(_vm->data.movie_filenames[movie_id]);
	}

	return 0;
}

ResultType ConversationBlock::execute(UnityEngine *_vm, Action *context) {
	debug(1, "ConversationBlock::execute: @0x%02x,%d,%d,%d: action %d",
		world_id, conversation_id, response_id, state_id, action_id);

	uint16 world = world_id;
	if (world == 0xffff) world = _vm->data.current_screen.world;

	Conversation *conv = _vm->data.getConversation(world, conversation_id);
	Response *resp = conv->getResponse(response_id, state_id);
	resp->response_state = action_id;

	return 0;
}

ResultType BeamBlock::execute(UnityEngine *_vm, Action *context) {
	debug(1, "BeamBlock::execute with %04x, %04x", world_id, screen_id);

	_vm->_beam_world = world_id;
	_vm->_beam_screen = screen_id;

	return 0;
}

ResultType TriggerBlock::execute(UnityEngine *_vm, Action *context) {
	Trigger *trigger = _vm->data.getTrigger(trigger_id);

	debug(1, "triggerBlock: trying to set trigger %x to %d", trigger_id, enable_trigger);
	trigger->enabled = enable_trigger;

	return 0;
}

ResultType CommunicateBlock::execute(UnityEngine *_vm, Action *context) {
	debug(1, "CommunicateBlock::execute: at %02x%02x%02x, %04x, %04x, %02x", target.world, target.screen, target.id, conversation_id, situation_id, hail_type);

	Object *targ;
	// TODO: not Picard!! what are we meant to do here?
	if (target.id == 0xff) {
		targ = _vm->data.getObject(objectID(0, 0, 0));
	} else {
		targ = _vm->data.getObject(target);
	}

	if (hail_type != 0xff && hail_type != 0x7 && hail_type != 0x8) {
		if (target.id != 0xff) {
			// must be a non-immediate hail
			if (!targ->talk_string.size() || targ->talk_string[0] != '@')
				error("'%s' is not a valid hail string (during Communicate)",
					targ->talk_string.c_str());

			// TODO: use hail_type!!
			targ->runHail(targ->talk_string);
			return 0;
		}
	}

	switch (hail_type) {
	case 0:
		// 0: we are being hailed (on screen)
	case 1:
		// 1: subspace frequency (open channels)
	case 2:
		// 2: hailed by planet (on screen)
	case 3:
		// 3: beacon (open channels)
		warning("unhandled optional hail (%02x)", hail_type);
		break;
	case 4:
		// 4: delayed conversation? (forced 0x5f)
	case 5:
		// 5: delayed conversation? (forced 0x5f)
	case 6:
		// 6: immediate conversation
	case 7:
		// 7: delayed conversation, oddness with 0xFE and 0x00/0x5F checks
		//    (elsewhere: special case conversation 99?!?)
		// on-viewscreen?
	case 8:
		// 8: delayed conversation? (forced 0x5f)
		_vm->_next_conversation = _vm->data.getConversation(_vm->data.current_screen.world,
			conversation_id);

		// original engine simply ignores this when there are no enabled situations, it seems
		// (TODO: check what happens when there is no such situation at all)
		// TODO: which speaker?? not Picard :(
		if (_vm->_next_conversation->getEnabledResponse(_vm, situation_id, objectID(0, 0, 0))) {
			// this overrides any existing conversations.. possibly that is a good thing
			_vm->_next_situation = situation_id;
		} else _vm->_next_situation = 0xffffffff;
		break;

	case 9:
		// 9: reset conversation to none
	case 0xa:
		// a: reset conversation to none
	case 0xb:
		// b: magicB + some storing of obj screen/id + wiping of that obj + reset conversation?
		// (back to bridge?)
	case 0xc:
		// c: magicB + reset conversation?
		// (some bridge change?)
	case 0xd:
		// d: reset conversation (but not obj) + magic?
		// (some bridge change? more complex than the others)
	case 0xe:
		// e: do nothing?
	case 0xf:
		// f: magicF + reset conversation?
		// (some *different* bridge change?)
	case 0x10:
		// 0x10: magicB + reset conversation + magic?
		warning("unhandled special hail (%02x)", hail_type);
	}

	return 0;
}

ResultType ChoiceBlock::execute(UnityEngine *_vm, Action *context) {
	warning("unimplemented: ChoiceBlock::execute");

	return 0;
}

void WhoCanSayBlock::readFrom(Common::SeekableReadStream *stream) {
	uint16 unknown = stream->readUint16LE();
	assert(unknown == 8);

	whocansay = readObjectID(stream);
	// XXX
	byte unknown1 = stream->readByte();
	byte unknown2 = stream->readByte();
	byte unknown3 = stream->readByte();
	byte unknown4 = stream->readByte();
}

void TextBlock::readFrom(Common::SeekableReadStream *stream) {
	uint16 unknown = stream->readUint16LE();
	assert(unknown == 0x10d);

	char buf[256];
	stream->read(buf, 255);
	buf[255] = 0;
	text = buf;

	// make sure it's all zeros?
	unsigned int i = 255;
	while (buf[i] == 0) i--;
	//assert(strlen(buf) == i + 1); XXX: work out what's going on here

	stream->read(buf, 4);

	voice_id = stream->readUint32LE();
	voice_group = stream->readUint32LE();
	voice_subgroup = stream->readUint16LE();

	/*if (text.size()) {
		printf("text '%s'", text.c_str());
		if (voice_id != 0xffffffff) {
			printf(" (%02x??%02x%02x.vac)", voice_group, voice_subgroup, voice_id);
		}
		printf("\n");
	} else {
		// these fields look like they could mean something else?
		printf("no text: %x, %x, %x\n", voice_group, voice_subgroup, voice_id);
	}*/

	// XXX: work out what's going on here
	if (!(buf[0] == 0 && buf[1] == 0 && buf[2] == 0 && buf[3] == 0))
		warning("text is weird: %02x, %02x, %02x, %02x", buf[0], buf[1], buf[2], buf[3]);
}

void TextBlock::execute(UnityEngine *_vm, Object *speaker) {
	if (text.size()) {
		_vm->_dialog_text = text;

		_vm->setSpeaker(speaker->id);
		debug(1, "%s says '%s'", speaker->identify().c_str(), text.c_str());

		Common::String file = _vm->voiceFileFor(voice_group, voice_subgroup,
			speaker->id, voice_id);
		_vm->_snd->playSpeech(file);

		_vm->runDialog();
	}
}

const char *change_actor_names[4] = { "disable", "set response", "enable", "unknown2" };

void ChangeActionBlock::readFrom(Common::SeekableReadStream *stream, int _type) {
	uint16 unknown = stream->readUint16LE();
	assert(unknown == 8);

	assert(_type >= BLOCK_CONV_CHANGEACT_DISABLE &&
		_type <= BLOCK_CONV_CHANGEACT_UNKNOWN2);
	type = _type;

	response_id = stream->readUint16LE();
	state_id = stream->readUint16LE();

	byte unknown4 = stream->readByte();
	byte unknown5 = stream->readByte();
	uint16 unknown6 = stream->readUint16LE();
	assert(unknown6 == 0 || (unknown6 >= 7 && unknown6 <= 12));

	/*printf("%s: response %d, state %d; unknowns: %x, %x, %x\n",
		change_actor_names[type - BLOCK_CONV_CHANGEACT_DISABLE],
		response_id, state_id, unknown4, unknown5, unknown6);*/
}

void ChangeActionBlock::execute(UnityEngine *_vm, Object *speaker, Conversation *src) {
	debug(1, "ChangeActionBlock::execute: %s on %d,%d",
		change_actor_names[type - BLOCK_CONV_CHANGEACT_DISABLE],
		response_id, state_id);

	Response *resp = src->getResponse(response_id, state_id);
	if (!resp) error("ChangeAction couldn't find state %d of situation %d", state_id, response_id);

	// TODO: terrible hack
	if (type == BLOCK_CONV_CHANGEACT_ENABLE) {
		resp->response_state = RESPONSE_ENABLED;
	} else {
		resp->response_state = RESPONSE_DISABLED;
	}
}

void ResultBlock::readFrom(Common::SeekableReadStream *stream) {
	entries.readEntryList(stream);
}

void ResultBlock::execute(UnityEngine *_vm, Object *speaker, Conversation *src) {
	debug(1, "ResultBlock::execute");
	(void)src;

	// TODO: what are we meant to provide for context here?
	Action context;
	context.action_type = ACTION_TALK;
	context.target = speaker;
	context.who = speaker->id;
	context.other = objectID();
	context.x = 0xffffffff;
	context.y = 0xffffffff;
	entries.execute(_vm, &context);
}

void Response::readFrom(Common::SeekableReadStream *stream) {
	id = stream->readUint16LE();
	state = stream->readUint16LE();

	g_debug_conv_state = state;
	g_debug_conv_response = id;

	char buf[256];
	stream->read(buf, 255);
	buf[255] = 0;
	text = buf;

	response_state = stream->readByte();
	assert(response_state == RESPONSE_ENABLED || response_state == RESPONSE_DISABLED ||
		response_state == RESPONSE_UNKNOWN1);

	uint16 unknown1 = stream->readUint16LE();
	uint16 unknown2 = stream->readUint16LE();
	//printf("(%04x, %04x), ", unknown1, unknown2);
	// the 0xfffb is somewhere after a USE on 070504, see 7/4
	assert(unknown2 == 0 || (unknown2 >= 6 && unknown2 <= 15) || unknown2 == 0xffff || unknown2 == 0xfffb);

	unknown1 = stream->readUint16LE();
	//printf("(%04x), ", unknown1);

	next_situation = stream->readUint16LE();
	//printf("(next %04x), ", next_situation);

	for (unsigned int i = 0; i < 5; i++) {
		uint16 unknown1 = stream->readUint16LE();
		uint16 unknown2 = stream->readUint16LE();
		//printf("(%04x, %04x), ", unknown1, unknown2);
		assert(unknown2 == 0 || (unknown2 >= 6 && unknown2 <= 15) || unknown2 == 0xffff);
	}

	target = readObjectID(stream);

	uint16 unknown3 = stream->readUint16LE();
	uint16 unknown4 = stream->readUint16LE();
	uint16 unknown5 = stream->readUint16LE();
	uint16 unknown6 = stream->readUint16LE();
	uint16 unknown7 = stream->readUint16LE();

	voice_id = stream->readUint32LE();
	voice_group = stream->readUint32LE();
	voice_subgroup = stream->readUint16LE();

	/*
	printf("%x: ", response_state);
	printf("%04x -- (%04x, %04x, %04x, %04x)\n", unknown3, unknown4, unknown5, unknown6, unknown7);
	printf("response %d, %d", id, state);
	if (text.size()) {
		printf(": text '%s'", text.c_str());
		if (voice_id != 0xffffffff) {
			printf(" (%02x??%02x%02x.vac)", voice_group, voice_subgroup, voice_id);
		}
	}
	printf("\n");
	*/

	int type;
	while ((type = readBlockHeader(stream)) != -1) {
		switch (type) {
			case BLOCK_END_BLOCK:
				//printf("\n");
				return;

			case BLOCK_CONV_WHOCANSAY:
				while (true) {
					WhoCanSayBlock *block = new WhoCanSayBlock();
					block->readFrom(stream);
					whocansayblocks.push_back(block);

					type = readBlockHeader(stream);
					if (type == BLOCK_END_BLOCK)
						break;
					if (type != BLOCK_CONV_WHOCANSAY)
						error("bad block type %x encountered while parsing whocansay", type);
				}
				break;

			case BLOCK_CONV_TEXT:
				while (true) {
					TextBlock *block = new TextBlock();
					block->readFrom(stream);
					textblocks.push_back(block);

					type = readBlockHeader(stream);
					if (type == BLOCK_END_BLOCK)
						break;
					if (type != BLOCK_CONV_TEXT)
						error("bad block type %x encountered while parsing text", type);
				}
				break;

			case BLOCK_CONV_CHANGEACT_DISABLE:
			case BLOCK_CONV_CHANGEACT_ENABLE:
			case BLOCK_CONV_CHANGEACT_UNKNOWN2:
			case BLOCK_CONV_CHANGEACT_SET:
				while (true) {
					ChangeActionBlock *block = new ChangeActionBlock();
					block->readFrom(stream, type);
					blocks.push_back(block);

					int oldtype = type; // XXX
					type = readBlockHeader(stream);
					if (type == BLOCK_END_BLOCK)
						break;
					if (type != oldtype)
						error("bad block type %x encountered while parsing changeaction", type);
				}
				break;

			case BLOCK_CONV_RESULT:
				{
				ResultBlock *block = new ResultBlock();
				block->readFrom(stream);
				blocks.push_back(block);
				break;
				}

			default:
				error("bad block type %x encountered while parsing response", type);
		}
	}

	error("didn't find a BLOCK_END_BLOCK for a BLOCK_CONV_RESPONSE");
}

void Conversation::loadConversation(UnityData &data, unsigned int world, unsigned int id) {
	assert(!responses.size());
	our_world = world;
	our_id = id;

	Common::String filename = Common::String::format("w%03xc%03d.bst", world, id);
	Common::SeekableReadStream *stream = data.openFile(filename);

	int blockType;
	while ((blockType = readBlockHeader(stream)) != -1) {
		assert(blockType == BLOCK_CONV_RESPONSE);
		Response *r = new Response();
		r->readFrom(stream);
		responses.push_back(r);
	}

	delete stream;
}

bool WhoCanSayBlock::match(UnityEngine *_vm, objectID speaker) {
	if (speaker.world != whocansay.world) return false;
	if (speaker.screen != whocansay.screen) return false;

	if (whocansay.world == 0 && whocansay.screen == 0 && whocansay.id == 0x10) {
		// any away team member
		// TODO: do we really need to check this?
		if (Common::find(_vm->_away_team_members.begin(),
			_vm->_away_team_members.end(),
			_vm->data.getObject(speaker)) == _vm->_away_team_members.end()) {
			return false;
		}
		return true;
	}

	if (speaker.id != whocansay.id) return false;

	return true;
}

bool Response::validFor(UnityEngine *_vm, objectID speaker) {
	for (unsigned int i = 0; i < whocansayblocks.size(); i++) {
		if (whocansayblocks[i]->match(_vm, speaker)) return true;
	}

	return false;
}

void Response::execute(UnityEngine *_vm, Object *speaker, Conversation *src) {
	if (text.size()) {
		// TODO: response escape strings
		// (search backwards between two '@'s, format: '0' + %c, %1x, %02x)

		_vm->_dialog_text = text;

		// TODO: this is VERy not good
		_vm->setSpeaker(speaker->id);
		debug(1, "%s says '%s'", "Picard", text.c_str());

		// TODO: 0xcc some marks invalid entries, but should we check something else?
		// (the text is invalid too, but i display it, to make it clear we failed..)

		if (voice_group != 0xcc) {
			Common::String file = _vm->voiceFileFor(voice_group, voice_subgroup,
				speaker->id, voice_id);

			_vm->_snd->playSpeech(file);
		}

		_vm->runDialog();
	}

	Object *targetobj = speaker;
	if (target.id != 0xff) {
		if (target.world == 0 && target.screen == 0 && (target.id >= 0x20 && target.id <= 0x28)) {
			// TODO: super hack
			warning("weird target: this is meant to be actors on the Enterprise?");
			target.id -= 0x20;
		}
		targetobj = _vm->data.getObject(target);
	}

	assert(textblocks.size() == 1); // TODO: make this not an array?
	textblocks[0]->execute(_vm, targetobj);

	debug(1, "***begin execute***");
	for (unsigned int i = 0; i < blocks.size(); i++) {
		blocks[i]->execute(_vm, targetobj, src);
	}
	debug(1, "***end execute***");
}

Response *Conversation::getEnabledResponse(UnityEngine *_vm, unsigned int response, objectID speaker) {
	for (unsigned int i = 0; i < responses.size(); i++) {
		if (responses[i]->id != response) continue;
		if (responses[i]->response_state != RESPONSE_ENABLED) continue;
		if (speaker.id != 0xff && !responses[i]->validFor(_vm, speaker)) continue;
		return responses[i];
	}

	return NULL;
}

Response *Conversation::getResponse(unsigned int response, unsigned int state) {
	for (unsigned int i = 0; i < responses.size(); i++) {
		if (responses[i]->id == response) {
			if (responses[i]->state == state) {
				return responses[i];
			}
		}
	}

	return NULL;
}

void Conversation::execute(UnityEngine *_vm, Object *speaker, unsigned int situation) {
	debug(1, "running situation (%02x) @%d,%d", situation, our_world, our_id);

	uint16 state = 0xffff;
	while (situation != 0xffff) {
		Response *resp;
		if (state == 0xffff) {
			resp = getEnabledResponse(_vm, situation, speaker->id);
		} else {
			resp = getResponse(situation, state);
		}
		if (!resp) error("couldn't find active response for situation %d", situation);

		resp->execute(_vm, speaker, this);

		_vm->_dialog_choice_states.clear();
		if (resp->next_situation != 0xffff) {
			situation = resp->next_situation;
			_vm->_dialog_choice_situation = situation; // XXX: hack

			for (unsigned int i = 0; i < responses.size(); i++) {
				if (responses[i]->id == situation) {
					if (responses[i]->response_state == RESPONSE_ENABLED) {
						if (responses[i]->validFor(_vm, speaker->id)) {
							_vm->_dialog_choice_states.push_back(responses[i]->state);
						}
					}
				}
			}

			if (!_vm->_dialog_choice_states.size()) {
				// see first conversation in space station
				warning("didn't find a next situation");
				return;
			}

			if (_vm->_dialog_choice_states.size() > 1) {
				debug(1, "continuing with conversation, using choices");
				state = _vm->runDialogChoice(this);
			} else {
				debug(1, "continuing with conversation, using single choice");
				state = _vm->_dialog_choice_states[0];
			}
		} else {
			debug(1, "end of conversation");
			return;
		}
	}
}

/*void Conversation::execute(UnityEngine *_vm, Object *speaker, unsigned int response, unsigned int state) {
	debug("1, running situation (%02x) @%d,%d,%d", our_world, our_id, response, state);

	Response *resp = getResponse(response, state);
	if (!resp) error("couldn't find response %d/%d", response, state);
	resp->execute(_vm, speaker);
}*/

} // Unity

