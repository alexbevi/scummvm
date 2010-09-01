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
	BLOCK_CONV_CHANGEACT_UNKNOWN1 = 0x30,
	BLOCK_CONV_CHANGEACT_SET = 0x31,
	BLOCK_CONV_CHANGEACT_CHOICE = 0x32,
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

	uint8 objflags = objstream->readByte();
	active = (objflags & OBJFLAG_ACTIVE) != 0;

	byte state = objstream->readByte();

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

void ConditionBlock::readFrom(Common::SeekableReadStream *objstream) {
	objstream->seek(0xd8, SEEK_CUR); // XXX
}

void AlterBlock::readFrom(Common::SeekableReadStream *objstream) {
	readHeaderFrom(objstream, 0x43);

	target = readObjectID(objstream);

	alter_flags = objstream->readByte();
	alter_reset = objstream->readByte();

	uint16 unknown3 = objstream->readUint16LE();

	uint16 unknown16 = objstream->readUint16LE();
	assert(unknown16 == 0xffff);

	uint16 unknown4 = objstream->readUint16LE();
	byte unknown5 = objstream->readByte();
	byte unknown6 = objstream->readByte();

	x_pos = objstream->readUint16LE();
	y_pos = objstream->readUint16LE();

	uint16 unknown7 = objstream->readUint16LE();

	unknown16 = objstream->readUint16LE();
	// always 0xffff inside objects..
	(void)unknown16;
	//assert(unknown16 == 0xffff);

	uint32 unknown32 = objstream->readUint32LE();
	// TODO: probably individual bytes or two uint16s?
	(void)unknown32;
	// always 0xffffffff inside objects..
	//assert(unknown32 == 0xffffffff);

	char text[101];
	objstream->read(text, 20);
	text[20] = 0;
	alter_name = text;
	objstream->read(text, 100);
	text[100] = 0;
	alter_hail = text;

	objstream->seek(0x64, SEEK_CUR); // XXX
}

void ReactionBlock::readFrom(Common::SeekableReadStream *objstream) {
	readHeaderFrom(objstream, 0x44);

	objstream->seek(0x78, SEEK_CUR); // XXX
}

void CommandBlock::readFrom(Common::SeekableReadStream *objstream) {
	readHeaderFrom(objstream, 0x45);

	objstream->seek(0x75, SEEK_CUR); // XXX
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

	uint16 unknown7 = objstream->readUint16LE(); // XXX
	uint16 unknown8 = objstream->readUint16LE(); // XXX
	uint16 unknown9 = objstream->readUint16LE(); // XXX
	printf("general block: %04x, %04x, %04x\n", unknown7, unknown8, unknown9);

	for (unsigned int i = 0; i < 0x64; i++) {
		byte zero = objstream->readByte();
		assert(zero == 0);
	}
}

bool valid_screen_id(uint16 screen_id) {
	return screen_id == 0x5f || screen_id == 0x10 || (screen_id >= 2 && screen_id <= 7);
}

void ConversationBlock::readFrom(Common::SeekableReadStream *objstream) {
	readHeaderFrom(objstream, 0x49);

	// the details of the conversation to modify?
	screen_id = objstream->readUint16LE();
	assert(screen_id == 0xffff || valid_screen_id(screen_id));
	conversation_id = objstream->readUint16LE();
	response_id = objstream->readUint16LE();
	state_id = objstream->readUint16LE();

	// this probably indicates what to change?
	action_id = objstream->readUint16LE();
	assert(action_id == 2 || action_id == 3 || action_id == 4); // XXX: what are these?

	// padding
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

	objstream->seek(0x6d, SEEK_CUR); // XXX
}

void ChoiceBlock::readFrom(Common::SeekableReadStream *objstream) {
	readHeaderFrom(objstream, 0x4d);

	objstream->seek(0xfc, SEEK_CUR); // XXX
}

void EntryList::readEntry(int type, Common::SeekableReadStream *objstream) {
	uint16 header;

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
					block->unknown1.readEntry(type, objstream);
				} else if (type == 0x27) {
					type = readBlockHeader(objstream);
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
		printf("hail of %s changed from '%s' to '%s'\n", name.c_str(), hail_string.c_str(), str.c_str());
		hail_string = str;
	}
}

void Object::runHail(const Common::String &hail) {
	printf("%s running hail '%s'\n", name.c_str(), hail.c_str());

	if (hail.size() < 2) {
		error("failed to parse hail '%s'", hail.c_str());
	}

	bool immediate = (hail[0] == '1');
	if ((immediate && hail[1] != '@') || (!immediate && hail[0] != '@')) {
		// TODO: handle plain strings?
		error("failed to parse hail (no @?) '%s'", hail.c_str());
	}

	int conversation, response, state = 0;
	if (sscanf(hail.begin() + (immediate ? 2 : 1), "%d,%d",
		&conversation, &response) != 2) {
		// TODO: handle further parameters (@%d,%d,%d)
		error("failed to parse hail '%s'", hail.c_str());
	}

	// TODO: keep these in data
	Conversation conv;
	// TODO: de-hardcode 0x5f, somehow
	conv.loadConversation(_vm->data, 0x5f, conversation);
	_vm->current_conversation = conv;
	_vm->current_conversation.execute(_vm, this, response, state);
}

EntryList::~EntryList() {
	for (unsigned int i = 0; i < entries.size(); i++) {
		delete entries[i];
	}
}

void EntryList::execute(UnityEngine *_vm) {
	for (unsigned int i = 0; i < entries.size(); i++) {
		if (!entries[i]->check(_vm)) return;
	}
	for (unsigned int i = 0; i < entries.size(); i++) {
		entries[i]->execute(_vm);
	}
}

bool ConditionBlock::check(UnityEngine *_vm) {
	warning("unimplemented: ConditionBlock::check");
	return true;
}

void ConditionBlock::execute(UnityEngine *_vm) {
	// nothing to do
}

void AlterBlock::execute(UnityEngine *_vm) {
	Object *obj = _vm->data.getObject(target);

	bool did_something = false; // just for debugging

	if (alter_flags || alter_reset) {
		did_something = true;
		warning("unimplemented: AlterBlock::execute (%s): alter_flags", obj->name.c_str());
	}

	if (x_pos != 0xffff) {
		did_something = true;
		warning("unimplemented: AlterBlock::execute (%s): position", obj->name.c_str());
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

	if (!did_something) {
		// TODO: all the other unknowns we don't read yet
		warning("unimplemented: AlterBlock::execute (%s)", obj->name.c_str());
	}
}

void ReactionBlock::execute(UnityEngine *_vm) {
	warning("unimplemented: ReactionBlock::execute");
}

void CommandBlock::execute(UnityEngine *_vm) {
	warning("unimplemented: CommandBlock::execute");
}

void ScreenBlock::execute(UnityEngine *_vm) {
	warning("unimplemented: ScreenBlock::execute");
}

void PathBlock::execute(UnityEngine *_vm) {
	warning("unimplemented: PathBlock::execute");
}

void GeneralBlock::execute(UnityEngine *_vm) {
	warning("unimplemented: GeneralBlock::execute");

	if (movie_id != 0xffff) {
		assert(_vm->data.movie_filenames.contains(movie_id));

		warning("unimplemented: play movie %d (%s: '%s')", movie_id,
			_vm->data.movie_filenames[movie_id].c_str(),
			_vm->data.movie_descriptions[movie_id].c_str());
	}
}

void ConversationBlock::execute(UnityEngine *_vm) {
	warning("unimplemented: ConversationBlock::execute");
}

void BeamBlock::execute(UnityEngine *_vm) {
	warning("unimplemented: BeamBlock::execute");
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
}

void ChoiceBlock::execute(UnityEngine *_vm) {
	warning("unimplemented: ChoiceBlock::execute");
}

void WhoCanSayBlock::readFrom(Common::SeekableReadStream *stream) {
	stream->seek(0xa, SEEK_CUR); // XXX
}

void WhoCanSayBlock::execute(UnityEngine *_vm, Object *speaker) {
	warning("unimplemented: WhoCanSayBlock::execute");
}

void TextBlock::readFrom(Common::SeekableReadStream *stream) {
	uint16 unknown = stream->readUint16LE();
	assert(unknown == 0x10d);

	char buf[256];
	stream->read(buf, 255);
	buf[255] = 0;
	text = buf;

	stream->read(buf, 4);
	assert(buf[0] == 0 && buf[1] == 0 && buf[2] == 0 && buf[3] == 0);

	voice_id = stream->readUint32LE();
	voice_group = stream->readUint32LE();
	voice_subgroup = stream->readUint16LE();
	uint32 entry_id = 0; // TODO: work out correct entry for actor

	if (text.size()) {
		printf("text '%s'", text.c_str());
		if (voice_id != 0xffffffff) {
			printf(" (%02x%02x%02x%02x.vac)", voice_group, entry_id, voice_subgroup, voice_id);
		}
		printf("\n");
	}
}

void TextBlock::execute(UnityEngine *_vm, Object *speaker) {
	warning("unimplemented: TextBlock::execute");

	if (text.size()) {
		_vm->dialog_text = text;

		_vm->setSpeaker(speaker->id);

		uint32 entry_id = speaker->id.id;
		Common::String file;
		file = Common::String::printf("%02x%02x%02x%02x.vac",
			voice_group, entry_id, voice_subgroup, voice_id);
		_vm->_snd->playSpeech(file);

		_vm->runDialog();
	}
}

const char *change_actor_names[4] = { "unknown1", "set response", "add choice", "unknown2" };

void ChangeActorBlock::readFrom(Common::SeekableReadStream *stream, int _type) {
	uint16 unknown = stream->readUint16LE();
	assert(unknown == 8);

	assert(_type >= BLOCK_CONV_CHANGEACT_UNKNOWN1 &&
		_type <= BLOCK_CONV_CHANGEACT_UNKNOWN2);
	type = _type;

	response_id = stream->readUint16LE();
	state_id = stream->readUint16LE();

	byte unknown4 = stream->readByte();
	byte unknown5 = stream->readByte();
	uint16 unknown6 = stream->readUint16LE();
	assert(unknown6 == 0 || (unknown6 >= 7 && unknown6 <= 12));

	printf("%s: response %d, state %d; unknowns: %x, %x, %x\n",
		change_actor_names[type - BLOCK_CONV_CHANGEACT_UNKNOWN1],
		response_id, state_id, unknown4, unknown5, unknown6);
}

void ChangeActorBlock::execute(UnityEngine *_vm, Object *speaker) {
	warning("unimplemented: ChangeActorBlock::execute");

	if (type == BLOCK_CONV_CHANGEACT_CHOICE) {
		// TODO: terrible hack
		_vm->dialog_choice_responses.push_back(response_id);
		_vm->dialog_choice_states.push_back(state_id);
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

	unsigned char buffer[0x1d + 0xa];
	stream->read(buffer, 0x1d); // XXX

	target = readObjectID(stream);

	stream->read(buffer + 0x1d, 0xa); // XXX

	voice_id = stream->readUint32LE();
	voice_group = stream->readUint32LE();
	voice_subgroup = stream->readUint16LE();

	printf("response %d, %d", id, state);
	if (text.size()) {
		printf(": text '%s'", text.c_str());
		if (voice_id != 0xffffffff) {
			printf(" (%02x??%02x%02x.vac)", voice_group, voice_subgroup, voice_id);
		}
	}
	printf("\n");

	for (unsigned int i = 0; i < sizeof(buffer); i++) printf("%02x ", (unsigned int)buffer[i]);
	printf("\n");

	int type;
	while ((type = readBlockHeader(stream)) != -1) {
		switch (type) {
			case BLOCK_END_BLOCK:
				printf("\n");
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

			case BLOCK_CONV_CHANGEACT_UNKNOWN1:
			case BLOCK_CONV_CHANGEACT_CHOICE:
			case BLOCK_CONV_CHANGEACT_UNKNOWN2:
			case BLOCK_CONV_CHANGEACT_SET:
				while (true) {
					ChangeActorBlock *block = new ChangeActorBlock();
					block->readFrom(stream, type);
					blocks.push_back(block);

					int oldtype = type; // XXX
					type = readBlockHeader(stream);
					if (type == BLOCK_END_BLOCK)
						break;
					if (type != oldtype)
						error("bad block type %x encountered while parsing changeactor", type);
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

		// TODO: 0xcc some marks invalid entries, but should we check something else?
		// (the text is invalid too, but i display it, to make it clear we failed..)

		if (voice_group != 0xcc) {
			uint32 entry_id = 0; // TODO: work out correct entry for actor
			Common::String file;
			file = Common::String::printf("%02x%02x%02x%02x.vac",
				voice_group, entry_id, voice_subgroup, voice_id);
			_vm->_snd->playSpeech(file);
		}

		_vm->runDialog();
	}

	_vm->dialog_choice_responses.clear();
	_vm->dialog_choice_states.clear();

	Object *targetobj = speaker;
	if (target.id != 0xff) {
		targetobj = _vm->data.getObject(target);
	}

	for (unsigned int i = 0; i < blocks.size(); i++) {
		blocks[i]->execute(_vm, targetobj);
	}

	if (_vm->dialog_choice_responses.size()) {
		if (_vm->dialog_choice_responses.size() > 1) {
			_vm->runDialogChoice();
		} else {
			_vm->current_conversation.execute(_vm, speaker, _vm->dialog_choice_responses[0], _vm->dialog_choice_states[0]);
		}
	}
}

Response *Conversation::getResponse(unsigned int response, unsigned int state) {
	for (unsigned int i = 0; i < responses.size(); i++) {
		if (responses[i]->id == response) {
			if (responses[i]->state == state) {
				return responses[i];
			}
		}
	}

	error("couldn't find response %d\n", response);
}

void Conversation::execute(UnityEngine *_vm, Object *speaker, unsigned int response, unsigned int state) {
	getResponse(response, state)->execute(_vm, speaker);
}

} // Unity

