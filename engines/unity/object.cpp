#include "object.h"
#include "unity.h"
#include "sprite.h"
#include "trigger.h"
#include "sound.h"

#include "common/stream.h"

namespace Unity {

#define VERIFY_LENGTH(x) do { uint16 block_length = objstream->readUint16LE(); assert(block_length == x); } while (0)

objectID readObjectID(Common::SeekableReadStream *stream) {
	objectID r;
	stream->read(&r, 4);
	assert(r.unused == 0 || r.unused == 0xff);
	return r;
}

enum {
	BLOCK_OBJ_HEADER = 0x0,
	BLOCK_DESCRIPTION = 0x1,
	BLOCK_USE_ENTRIES = 0x2,
	BLOCK_GET_ENTRIES = 0x3,
	BLOCK_LOOK_ENTRIES = 0x4,
	BLOCK_TIMER_ENTRIES = 0x5, // XXX: maybe :)

	BLOCK_CONDITION = 0x6,
	BLOCK_ALTER = 0x7,
	BLOCK_REACTION = 0x8,
	BLOCK_COMMAND = 0x9,
	BLOCK_SCREEN = 0x10,
	BLOCK_PATH = 0x11,
	// 0x12 unused?
	BLOCK_GENERAL = 0x13,
	BLOCK_CONVERSATION = 0x14,
	BLOCK_BEAM = 0x15,
	BLOCK_TRIGGER = 0x16,
	BLOCK_COMMUNICATE = 0x17,
	BLOCK_CHOICE = 0x18,
	// 0x19, 0x20, 0x21, 0x22 unused?

	BLOCK_END_ENTRY = 0x23,
	BLOCK_BEGIN_ENTRY = 0x24,

	BLOCK_END_BLOCK = 0x25,

	// TODO: 0x26, 0x27 for CHOICE

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
	OBJFLAG_WALK = 0x01,
	OBJFLAG_USE = 0x02,
	OBJFLAG_TALK = 0x04,
	OBJFLAG_GET = 0x08,
	OBJFLAG_LOOK = 0x10,
	OBJFLAG_ACTIVE = 0x20,
	OBJFLAG_INVENTORY = 0x40
};

enum {
	OBJWALKTYPE_NORMAL = 0x0,
	OBJWALKTYPE_SCALED = 0x1, // scaled with walkable polygons (e.g. characters)
	OBJWALKTYPE_TS = 0x2, // transition square
	OBJWALKTYPE_AS = 0x3 // action square
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
	return Common::String::printf("%s (%02x%02x%02x)", name.c_str(), id.world, id.screen, id.id);
}

void Object::loadObject(unsigned int for_world, unsigned int for_screen, unsigned int for_id) {
	Common::String filename = Common::String::printf("o_%02x%02x%02x.bst", for_world, for_screen, for_id);
	Common::SeekableReadStream *objstream = _vm->data.openFile(filename);

	int type = readBlockHeader(objstream);
	if (type != BLOCK_OBJ_HEADER)
		error("bad block type %x encountered at start of object", type);
	VERIFY_LENGTH(0x128);

	id = readObjectID(objstream);
	assert(id.id == for_id);
	assert(id.screen == for_screen);
	assert(id.world == for_world);

	byte unknown2 = objstream->readByte(); // XXX
	byte unknown3 = objstream->readByte(); // XXX

	width = objstream->readSint16LE();
	height = objstream->readSint16LE();

	int16 world_x = objstream->readSint16LE();
	int16 world_y = objstream->readSint16LE();
	int16 world_z = objstream->readSint16LE();
	x = world_x;
	y = world_y;
	z = world_z;

	universe_x = objstream->readSint16LE();
	universe_y = objstream->readSint16LE();
	universe_z = objstream->readSint16LE();

	// TODO: this doesn't work properly (see DrawOrderComparison)
	z_adjust = objstream->readUint16LE();

	uint16 unknown5 = objstream->readUint16LE(); // XXX

	sprite_id = objstream->readUint16LE();
	sprite = NULL;

	uint16 unknown6 = objstream->readUint16LE(); // XXX
	uint16 unknown7 = objstream->readUint16LE(); // XXX

	// XXX: save!!
	uint8 objflags = objstream->readByte();
	active = (objflags & OBJFLAG_ACTIVE) != 0;

	state = objstream->readByte();

	uint8 unknown8 = objstream->readByte(); // XXX: block count
	uint8 unknown9 = objstream->readByte(); // XXX: same
	uint8 unknown10 = objstream->readByte(); // XXX: same

	uint8 unknown11 = objstream->readByte();
	assert(unknown11 == 0);

	byte objwalktype = objstream->readByte();
	assert(objwalktype <= OBJWALKTYPE_AS);
	scaled = (objwalktype == OBJWALKTYPE_SCALED); // XXX
	byte description_count = objstream->readByte();

	char _name[20];
	objstream->read(_name, 20);
	name = _name;

	for (unsigned int i = 0; i < 6; i++) {
		uint16 unknowna = objstream->readUint16LE(); // XXX
		byte unknownb = objstream->readByte();
		byte unknownc = objstream->readByte();
	}

	uint16 unknown14 = objstream->readUint16LE(); // XXX
	uint16 unknown15 = objstream->readUint16LE(); // XXX

	char _str[101];
	objstream->read(_str, 100);
	_str[100] = 0;
	hail_string = _str;

	uint16 zero16;
	zero16 = objstream->readUint16LE();
	assert(zero16 == 0x0);
	zero16 = objstream->readUint16LE();
	assert(zero16 == 0x0);
	zero16 = objstream->readUint16LE();
	assert(zero16 == 0x0);

	uint16 unknown21 = objstream->readUint16LE(); // XXX
	uint32 unknown22 = objstream->readUint32LE(); // XXX
	uint32 unknown23 = objstream->readUint32LE(); // XXX
	uint16 unknown24 = objstream->readUint16LE(); // XXX
	uint16 unknown25 = objstream->readUint16LE(); // XXX
	uint16 unknown26 = objstream->readUint16LE(); // XXX

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

	// not sure, but see usage in TriggerBlock
	unknown_flag = stream->readByte();
	assert(unknown_flag == 0 || unknown_flag == 1 || unknown_flag == 0xff);

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

	target[0] = readObjectID(objstream);
	assert(target[0].world == 0xff || valid_world_id(target[0].world));
	target[1] = readObjectID(objstream);
	assert(target[1].world == 0xff || valid_world_id(target[1].world));

	/*if (target[0].world != 0xff)
		printf("targ 0: %02x%02x%02x\n", target[0].world, target[0].screen, target[0].id);
	if (target[1].world != 0xff)
		printf("targ 1: %02x%02x%02x\n", target[1].world, target[1].screen, target[1].id);*/

	for (unsigned int i = 0; i < 5; i++) {
		condition[i] = readObjectID(objstream);
		assert(condition[i].world == 0xff || valid_world_id(condition[i].world));

		/*printf("cond %d: ", i);
		if (condition[i].world != 0xff)
			printf("%02x%02x%02x ", condition[i].world, condition[i].screen, condition[i].id);*/

		unknown1[i] = objstream->readUint16LE();
		unknown2[i] = objstream->readUint16LE();
		unknown3[i] = objstream->readUint16LE();
		unknown4[i] = objstream->readUint16LE();
		unknown5[i] = objstream->readUint16LE();
		unknown6[i] = objstream->readUint16LE();
		unknown7[i] = objstream->readByte();
		unknown8[i] = objstream->readByte();
		state_check[i] = objstream->readByte();

		/*if (unknown1 != 0xffff) printf("unknown1: %04x ", unknown1);
		if (unknown2 != 0xffff) printf("unknown2: %04x ", unknown2);
		if (unknown3 != 0xffff) printf("unknown3: %04x ", unknown3);
		if (unknown4 != 0xffff) printf("unknown4: %04x ", unknown4);

		if (unknown5 != 0xffff) printf("unknown5: %04x ", unknown5);
		if (unknown6 != 0xffff) printf("unknown6: %04x ", unknown6);

		if (unknown7 != 0xff) printf("unknown7: %02x ", unknown7);
		if (unknown8 != 0xff) printf("unknown8: %02x ", unknown8);
		if (state_check[i] != 0xff) printf("state check: %x ", state_check[i]);

		printf("\n");*/

		switch (i) {
			case 0:
			case 1:
				assert(unknown4[i] == 0xffff);
				assert(unknown5[i] == 0xffff);
				assert(unknown6[i] == 0xffff);
				break;
			case 2:
			case 3:
				assert(unknown1[i] == 0xffff);
				assert(unknown2[i] == 0xffff);
				assert(unknown3[i] == 0xffff);
				assert(unknown4[i] == 0xffff);
				assert(unknown5[i] == 0xffff);
				assert(unknown6[i] == 0xffff);
				break;
			case 4:
				// can only check 4 condition objects, so
				// this is perhaps something else entirely
				assert(condition[i].world == 0xff);
				assert(unknown1[i] == 0xffff);
				assert(unknown4[i] == 0xffff);
				/*
				byte unknown6 = objstream->readByte();
				assert(unknown36 == 0);
				byte unknown7 = objstream->readByte();
				uint16 unknown8 = objstream->readUint16LE();
				byte unknown9 = objstream->readByte();
				*/
				break;
		}
	}

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

	unknown3 = objstream->readUint16LE();

	uint16 unknown16 = objstream->readUint16LE();
	assert(unknown16 == 0xffff);

	unknown4 = objstream->readByte();
	unknown5 = objstream->readByte();
	alter_state = objstream->readByte(); // set state..
	unknown7 = objstream->readByte();

	x_pos = objstream->readUint16LE();
	y_pos = objstream->readUint16LE();

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

	unknown11 = objstream->readByte();
	unknown12 = objstream->readByte();
	// checks
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

	objstream->seek(0x78, SEEK_CUR); // XXX
}

void CommandBlock::readFrom(Common::SeekableReadStream *objstream) {
	readHeaderFrom(objstream, 0x45);

	target[0] = readObjectID(objstream);
	target[1] = readObjectID(objstream);
	target[2] = readObjectID(objstream);

	// both usually 0xffff (but not an object?)
	unknown1 = objstream->readUint16LE();
	unknown2 = objstream->readUint16LE();

	command_id = objstream->readUint32LE();

	/*
	 * 2/3 are about items? GET? DROP? INV?
	 * 4 is WALK?
	 * 5 is TALK?
	 * 6 is USE?
	 */
	assert(command_id >= 2 && command_id <= 6);

	for (unsigned int i = 0; i < 97; i++) {
		byte unknown = objstream->readByte();
		assert(unknown == 0);
	}
}

void ScreenBlock::readFrom(Common::SeekableReadStream *objstream) {
	readHeaderFrom(objstream, 0x46);

	objstream->seek(0x81, SEEK_CUR); // XXX
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

	screen_id = objstream->readUint16LE();
	assert(screen_id == 0xffff || valid_world_id(screen_id));
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

	objstream->seek(0x90, SEEK_CUR); // XXX
}

void TriggerBlock::readFrom(Common::SeekableReadStream *objstream) {
	readHeaderFrom(objstream, 0x4b);

	// TODO: is this really some kind of 'priority'?
	assert(unknown_flag == 0 || unknown_flag == 1);
	instant_disable = (unknown_flag == 1);

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
	unknown1 = objstream->readUint16LE();
	unknown2 = objstream->readUint16LE();
	uint16 unknown4 = objstream->readUint16LE();
	assert(unknown4 == 0);

	for (unsigned int i = 0; i < 24; i++) {
		uint32 unknown32 = objstream->readUint32LE();
		assert(unknown32 == 0);
	}
	byte unknown = objstream->readByte();
	assert(unknown == 0);
}

void ChoiceBlock::readFrom(Common::SeekableReadStream *objstream) {
	readHeaderFrom(objstream, 0x4d);

	objstream->seek(0xfc, SEEK_CUR); // XXX
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

		case BLOCK_BEAM:
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

				if (type == 0x26) {
					type = readBlockHeader(objstream);
					// XXX: horrible
					block->unknown1.list.push_back(new Common::Array<Entry *>());
					block->unknown1.readEntry(type, objstream);
				} else if (type == 0x27) {
					type = readBlockHeader(objstream);
					// XXX: horrible
					block->unknown2.list.push_back(new Common::Array<Entry *>());
					block->unknown2.readEntry(type, objstream);
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

void Object::setHail(const Common::String &str) {
	if (!str.size()) {
		hail_string.clear();
		return;
	}

	bool immediate = (str[0] == '1');
	if (immediate) {
		// run the conversation immediately, don't change anything
		runHail(str);
	} else {
		printf("hail of %s changed from '%s' to '%s'\n", identify().c_str(), hail_string.c_str(), str.c_str());
		hail_string = str;
	}
}

void Object::runHail(const Common::String &hail) {
	printf("%s running hail '%s'\n", identify().c_str(), hail.c_str());

	if (hail.size() < 2) {
		error("failed to parse hail '%s'", hail.c_str());
	}

	bool immediate = (hail[0] == '1');
	if ((immediate && hail[1] != '@') || (!immediate && hail[0] != '@')) {
		// TODO: handle plain strings?
		error("failed to parse hail (no @?) '%s'", hail.c_str());
	}

	int conversation, response;
	if (sscanf(hail.begin() + (immediate ? 2 : 1), "%d,%d",
		&conversation, &response) != 2) {
		// TODO: handle further parameters (@%d,%d,%d)
		error("failed to parse hail '%s'", hail.c_str());
	}

	// TODO: de-hardcode 0x5f, somehow
	_vm->current_conversation = _vm->data.getConversation(0x5f, conversation);
	_vm->current_conversation->execute(_vm, this, response);
}

EntryList::~EntryList() {
	for (unsigned int i = 0; i < list.size(); i++) {
		for (unsigned int j = 0; j < list[i]->size(); j++) {
			delete (*list[i])[j];
		}
		delete list[i];
	}
}

void EntryList::execute(UnityEngine *_vm) {
	printf("\n");
	for (unsigned int i = 0; i < list.size(); i++) {
		printf("EntryList::execute: block %d of %d (size %d)\n", i + 1, list.size(), list[i]->size());
		bool run = true;
		for (unsigned int j = 0; j < list[i]->size(); j++) {
			if (!(*list[i])[j]->check(_vm)) {
				run = false;
				break;
			}
		}
		if (!run) continue;
		for (unsigned int j = 0; j < list[i]->size(); j++) {
			(*list[i])[j]->execute(_vm);
		}
	}
	printf("EntryList::execute: ran %d blocks\n\n", list.size());
}

bool ConditionBlock::check(UnityEngine *_vm) {
	warning("unimplemented: ConditionBlock::check: %02x%02x%02x, %02x%02x%02x",
		target[0].world, target[0].screen, target[0].id,
		target[1].world, target[1].screen, target[1].id);

	// TODO: this isn't complete
	for (unsigned int i = 0; i < 4; i++) {
		if (condition[i].world == 0xff) continue;

		Object *obj = _vm->data.getObject(condition[i]);
		printf("checking state of %s", obj->identify().c_str());
		if (unknown1[i] != 0xffff) {
			printf(" (is unknown1 %x?)", unknown1[i]);
		}
		if (unknown2[i] != 0xffff) {
			printf(" (is unknown2 %x?)", unknown2[i]);
		}
		if (unknown3[i] != 0xffff) {
			printf(" (is unknown3 %x?)", unknown3[i]);
		}
		if (unknown4[i] != 0xffff) {
			printf(" (is unknown4 %x?)", unknown4[i]);
		}
		if (unknown5[i] != 0xffff) {
			printf(" (is unknown5 %x?)", unknown5[i]);
		}
		if (unknown6[i] != 0xffff) {
			printf(" (is unknown6 %x?)", unknown6[i]);
		}
		if (unknown7[i] != 0xff) {
			printf(" (is unknown7 %x?)", unknown7[i]);
		}
		if (unknown8[i] != 0xffff) {
			printf(" (is unknown8 %x?)", unknown8[i]);
		}
		if (state_check[i] != 0xff) {
			printf(" (is state %x?)", state_check[i]);
			if (obj->state != state_check[i]) {
				printf(" -- nope!\n");
				return false;
			}
			// XXX: definitely don't commit this ;p
			/*if (condition[i].id == 7 && state_check[i] == 1) { }
			else if (condition[i].id == 8 && state_check[i] == 0) { }
			else if (condition[i].id == 0xc && state_check[i] == 0) { }
			else if (condition[i].id == 0x13 && state_check[i] == 0) { }
			else if (condition[i].id == 0x28 && state_check[i] == 1) { }
			else if (condition[i].id == 0x31 && state_check[i] == 0) { }
			else { printf(" -- nope!\n"); return false; }*/
		}
		printf("\n");
	}

	return true;
}

void ConditionBlock::execute(UnityEngine *_vm) {
	// nothing to do?
}

void AlterBlock::execute(UnityEngine *_vm) {
	Object *obj = _vm->data.getObject(target);

	bool did_something = false; // just for debugging

	if (alter_flags || alter_reset) {
		did_something = true;
		// TODO: change flags?
		warning("unimplemented: AlterBlock::execute (%s): alter_flags %x, %x", obj->identify().c_str(), alter_flags, alter_reset);
	}

	if (x_pos != 0xffff || y_pos != 0xffff) {
		did_something = true;
		// TODO: move target?
		warning("unimplemented: AlterBlock::execute (%s): position (%x, %x)", obj->identify().c_str(), x_pos, y_pos);
	}

	if (alter_name.size()) {
		did_something = true;
		printf("altering name of %s to %s\n", obj->identify().c_str(), alter_name.c_str());
		obj->name = alter_name;
	}

	if (alter_hail.size()) {
		did_something = true;
		obj->setHail(alter_hail);
	}

	if (voice_group != 0xffffffff || voice_subgroup != 0xffff || voice_id != 0xffffffff) {
		did_something = true;
		// TODO: alter voice for the description?
		warning("unimplemented: AlterBlock::execute (%s): voice %x/%x/%x", obj->identify().c_str(), voice_group, voice_subgroup, voice_id);
	}

	if (alter_state != 0xff) {
		did_something = true;
		obj->state = alter_state;
		warning("unimplemented: AlterBlock::execute (%s): state %x", obj->identify().c_str(), alter_state);
	}

	if (unknown3 != 0xffff) {
		did_something = true;
		warning("unimplemented: AlterBlock::execute (%s): unknown3 %x", obj->identify().c_str(), unknown3);
	}

	if (unknown4 != 0xff) {
		did_something = true;
		warning("unimplemented: AlterBlock::execute (%s): unknown4 %x", obj->identify().c_str(), unknown4);
	}

	if (unknown5 != 0xff) {
		did_something = true;
		warning("unimplemented: AlterBlock::execute (%s): unknown5 %x", obj->identify().c_str(), unknown5);
	}

	if (unknown7 != 0) {
		did_something = true;
		warning("unimplemented: AlterBlock::execute (%s): unknown7 %x", obj->identify().c_str(), unknown7);
	}

	if (unknown8 != 0xffff) {
		did_something = true;
		warning("unimplemented: AlterBlock::execute (%s): unknown8 %x", obj->identify().c_str(), unknown8);
	}

	if (universe_x != 0xffff || universe_y != 0xffff || universe_z != 0xffff) {
		did_something = true;
		// TODO: for the Enterprise, at least, go into astrogation and start warp to this universe location
		obj->universe_x = universe_x;
		obj->universe_y = universe_y;
		obj->universe_z = universe_z;
		warning("unimplemented: AlterBlock::execute (%s): to universe loc %x, %x, %x", obj->identify().c_str(), universe_x, universe_y, universe_z);
	}

	if (unknown11 != 0xff) {
		did_something = true;
		warning("unimplemented: AlterBlock::execute (%s): unknown11 %x", obj->identify().c_str(), unknown11);
	}

	if (unknown12 != 0xff) {
		did_something = true;
		warning("unimplemented: AlterBlock::execute (%s): unknown12 %x", obj->identify().c_str(), unknown12);
	}

	if (!did_something) {
		warning("empty AlterBlock::execute (%s)?", obj->identify().c_str());
	}
}

void ReactionBlock::execute(UnityEngine *_vm) {
	error("unimplemented: ReactionBlock::execute");
}

void CommandBlock::execute(UnityEngine *_vm) {
	printf("CommandBlock: %02x%02x%02x/%02x%02x%02x/%02x%02x%02x, %02x, %02x, command %d\n",
		target[0].world, target[0].screen, target[0].id,
		target[1].world, target[1].screen, target[1].id,
		target[2].world, target[2].screen, target[2].id,
		unknown1, unknown2, command_id);

	switch (command_id) {
		case 5: {
			// target[0] target (e.g. Pentara), target[1] source (e.g. Picard)
			Object *targ = _vm->data.getObject(target[0]);
			Object *src = _vm->data.getObject(target[1]);
			// TODO..
			warning("unimplemented: CommandBlock::execute: TALK (on %s)", targ->identify().c_str());
			targ->runHail(targ->hail_string);
			}
			break;

		case 6: {
			Object *targ = _vm->data.getObject(target[0]);
			Object *src = _vm->data.getObject(target[1]);
			// TODO..
			warning("unimplemented: CommandBlock::execute: USE (on %s)", targ->identify().c_str());
			targ->use_entries.execute(_vm);
			}
			break;

		default:
			warning("unimplemented: CommandBlock::execute (type %x)", command_id);
	}
}

void ScreenBlock::execute(UnityEngine *_vm) {
	error("unimplemented: ScreenBlock::execute");
}

void PathBlock::execute(UnityEngine *_vm) {
	error("unimplemented: PathBlock::execute");
}

void GeneralBlock::execute(UnityEngine *_vm) {
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

		// TODO
		printf("GeneralBlock: play movie %d (%s: '%s')\n", movie_id,
			_vm->data.movie_filenames[movie_id].c_str(),
			_vm->data.movie_descriptions[movie_id].c_str());
	}
}

void ConversationBlock::execute(UnityEngine *_vm) {
	warning("unimplemented: ConversationBlock::execute: screen %02x, @%d,%d,%d: action %d",
		screen_id, conversation_id, response_id, state_id, action_id);

	Conversation *conv = _vm->data.getConversation(screen_id, conversation_id);
	Response *resp = conv->getResponse(response_id, state_id);
	resp->response_state = action_id;
}

void BeamBlock::execute(UnityEngine *_vm) {
	error("unimplemented: BeamBlock::execute");
}

bool TriggerBlock::check(UnityEngine *_vm) {
	// This is here because I don't understand how the original engine
	// does the "disable everything a trigger did" trick (see, for example,
	// the Initial Log trigger which sabotages itself at startup).
	if (instant_disable) {
		execute(_vm);
		return false;
	}
	return true;
}

void TriggerBlock::execute(UnityEngine *_vm) {
	Trigger *trigger = _vm->data.getTrigger(trigger_id);

	printf("triggerBlock: trying to set trigger %x to %d\n", trigger_id, enable_trigger);
	trigger->enabled = enable_trigger;
}

void CommunicateBlock::execute(UnityEngine *_vm) {
	warning("unimplemented: CommunicateBlock::execute");

	printf("communicate: at %02x%02x%02x, %04x, %04x, %04x\n", target.world, target.screen, target.id, conversation_id, unknown1, unknown2);

	// TODO: does unknown2 indicate viewscreen vs bridge or what?
	if (unknown1 == 0xffff) return; // XXX: TODO

	Object *targ;
	// TODO: not Picard!! what are we meant to do here?
	if (target.id == 0xff)
		targ = _vm->data.getObject(objectID(0, 0, 0));
	else
		targ = _vm->data.getObject(target);

	// TODO: de-hardcode 0x5f, somehow
	_vm->current_conversation = _vm->data.getConversation(0x5f, conversation_id);

	// original engine simply ignores this when there are no enabled situations, it seems
	// (TODO: check what happens when there is no such situation at all)
	if (_vm->current_conversation->getEnabledResponse(unknown1)) {
		// this overrides any existing conversations.. possibly that is a good thing
		_vm->next_situation = unknown1;
	}
}

void ChoiceBlock::execute(UnityEngine *_vm) {
	error("unimplemented: ChoiceBlock::execute");
}

void WhoCanSayBlock::readFrom(Common::SeekableReadStream *stream) {
	uint16 unknown = stream->readUint16LE();
	assert(unknown == 8);

	// XXX
	objectID whocansay = readObjectID(stream);
	byte unknown1 = stream->readByte();
	byte unknown2 = stream->readByte();
	byte unknown3 = stream->readByte();
	byte unknown4 = stream->readByte();
}

void WhoCanSayBlock::execute(UnityEngine *_vm, Object *speaker) {
	// TODO
	//warning("unimplemented: WhoCanSayBlock::execute");
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
		printf("text is weird: %02x, %02x, %02x, %02x\n", buf[0], buf[1], buf[2], buf[3]);
}

void TextBlock::execute(UnityEngine *_vm, Object *speaker) {
	if (text.size()) {
		_vm->dialog_text = text;

		_vm->setSpeaker(speaker->id);
		printf("%s says '%s'\n", speaker->identify().c_str(), text.c_str());

		uint32 entry_id = speaker->id.id;
		if (speaker->id.world != 0x0)
			entry_id = 0xff; // XXX: hack (for Pentara, Daenub, etc)

		Common::String file;
		file = Common::String::printf("%02x%02x%02x%02x.vac",
			voice_group, entry_id, voice_subgroup, voice_id);
		if (!SearchMan.hasFile(file)) {
			// TODO: wtf?
			file = Common::String::printf("%02x%02x%02x%02x.vac",
				voice_group, voice_subgroup, entry_id, voice_id);
		}
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

void ChangeActionBlock::execute(UnityEngine *_vm, Object *speaker) {
	warning("unimplemented: ChangeActionBlock::execute: %s on %d,%d",
		change_actor_names[type - BLOCK_CONV_CHANGEACT_DISABLE],
		response_id, state_id);

	Response *resp = _vm->current_conversation->getResponse(response_id, state_id);

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

void ResultBlock::execute(UnityEngine *_vm, Object *speaker) {
	warning("unimplemented: ResultBlock::execute");
	entries.execute(_vm);
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
	assert(unknown2 == 0 || (unknown2 >= 7 && unknown2 <= 11) || unknown2 == 0xffff);

	unknown1 = stream->readUint16LE();
	//printf("(%04x), ", unknown1);

	next_situation = stream->readUint16LE();
	//printf("(next %04x), ", next_situation);

	for (unsigned int i = 0; i < 5; i++) {
		uint16 unknown1 = stream->readUint16LE();
		uint16 unknown2 = stream->readUint16LE();
		//printf("(%04x, %04x), ", unknown1, unknown2);
		assert(unknown2 == 0 || (unknown2 >= 7 && unknown2 <= 11) || unknown2 == 0xffff);
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
					blocks.push_back(block);

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
					blocks.push_back(block);

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

	Common::String filename = Common::String::printf("w%03xc%03d.bst", world, id);
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

void Response::execute(UnityEngine *_vm, Object *speaker) {
	if (text.size()) {
		_vm->dialog_text = text;

		// TODO: this is not good
		_vm->setSpeaker(objectID(0, 0, 0));
		printf("%s says '%s'\n", "Picard", text.c_str());

		// TODO: 0xcc some marks invalid entries, but should we check something else?
		// (the text is invalid too, but i display it, to make it clear we failed..)

		if (voice_group != 0xcc) {
			uint32 entry_id = 0; // TODO: work out correct entry for actor
			Common::String file;
			file = Common::String::printf("%02x%02x%02x%02x.vac",
				voice_group, entry_id, voice_subgroup, voice_id);
			if (!SearchMan.hasFile(file)) {
				// TODO: wtf?
				file = Common::String::printf("%02x%02x%02x%02x.vac",
					voice_group, voice_subgroup, entry_id, voice_id);
			}

			_vm->_snd->playSpeech(file);
		}

		_vm->runDialog();
	}

	Object *targetobj = speaker;
	if (target.id != 0xff) {
		targetobj = _vm->data.getObject(target);
	}

	printf("***begin execute***\n");
	for (unsigned int i = 0; i < blocks.size(); i++) {
		blocks[i]->execute(_vm, targetobj);
	}
	printf("***end execute***\n");

	_vm->dialog_choice_responses.clear();
	_vm->dialog_choice_states.clear();
	if (next_situation != 0xffff) {
		Common::Array<Response *> &responses = _vm->current_conversation->responses;
		for (unsigned int i = 0; i < responses.size(); i++) {
			if (responses[i]->id == next_situation) {
				if (responses[i]->response_state == RESPONSE_ENABLED) {
					_vm->dialog_choice_responses.push_back(responses[i]->id);
					_vm->dialog_choice_states.push_back(responses[i]->state);
				}
			}
		}

		if (!_vm->dialog_choice_responses.size()) {
			error("didn't find a next situation");
		}

		if (_vm->dialog_choice_responses.size() > 1) {
			printf("continuing with conversation, using choices\n");
			_vm->runDialogChoice();
		} else {
			printf("continuing with conversation, using single choice\n");
			_vm->next_situation = _vm->dialog_choice_responses[0];
			_vm->next_state = _vm->dialog_choice_states[0];
		}
	} else {
		printf("end of conversation\n");
	}
}

Response *Conversation::getEnabledResponse(unsigned int response) {
	for (unsigned int i = 0; i < responses.size(); i++) {
		if (responses[i]->id == response) {
			if (responses[i]->response_state == RESPONSE_ENABLED) {
				return responses[i];
			}
		}
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

void Conversation::execute(UnityEngine *_vm, Object *speaker, unsigned int response) {
	printf("running situation (%02x) @%d,%d\n", our_world, our_id, response);

	Response *resp = getEnabledResponse(response);
	if (!resp) error("couldn't find active response %d", response);
	resp->execute(_vm, speaker);
}

void Conversation::execute(UnityEngine *_vm, Object *speaker, unsigned int response, unsigned int state) {
	printf("running situation (%02x) @%d,%d,%d\n", our_world, our_id, response, state);

	Response *resp = getResponse(response, state);
	if (!resp) error("couldn't find response %d/%d", response, state);
	resp->execute(_vm, speaker);
}

} // Unity

