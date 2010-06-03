#include "object.h"
#include "unity.h"
#include "sprite.h"

#include "common/stream.h"

namespace Unity {

#define VERIFY_LENGTH(x) do { uint16 block_length = objstream->readUint16LE(); assert(block_length == x); } while (0)

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
	// TODO: 0x29
	// 0x30, 0x31 unused?
	// TODO: 0x32
	// TODO: 0x33
	BLOCK_CONV_ENTRY = 0x34,
	// TODO: 0x35

	// TODO: 0x36
	// TODO: 0x37
	// TODO: 0x38
	BLOCK_PHASER_RECORD = 0x39,

	BLOCK_SPEECH_INFO = 0x40
};

enum {
	// XXX
	OBJFLAG_ACTIVE = 0x20
};

enum {
	OBJWALKTYPE_NORMAL = 0x0,
	OBJWALKTYPE_SCALED = 0x1, // scaled with walkable polygons (e.g. characters)
	OBJWALKTYPE_TS = 0x2, // transition square
	OBJWALKTYPE_AS = 0x3
};

void Object::loadObject(UnityEngine *_vm, unsigned int for_world, unsigned int for_screen, unsigned int for_id) {
	Common::String filename = Common::String::printf("o_%02x%02x%02x.bst", for_world, for_screen, for_id);
	Common::SeekableReadStream *objstream = _vm->openFile(filename);

	int type = readBlockHeader(objstream);
	if (type != BLOCK_OBJ_HEADER)
		error("bad block type %x encountered at start of object", type);
	VERIFY_LENGTH(0x128);

	id = objstream->readByte();
	assert(id == for_id);
	screen = objstream->readByte();
	assert(screen == for_screen);
	world = objstream->readByte();
	assert(world == for_world);

	byte zerobyte = objstream->readByte();
	assert(zerobyte == 0);

	byte unknown2 = objstream->readByte(); // XXX
	byte unknown3 = objstream->readByte(); // XXX

	width = objstream->readSint16LE();
	height = objstream->readSint16LE();

	int16 world_x = objstream->readSint16LE();
	int16 world_y = objstream->readSint16LE();
	x = world_x;
	y = world_y;

	// TODO: do something with these!
	int16 world_z = objstream->readSint16LE();
	int16 universe_x = objstream->readSint16LE();
	int16 universe_y = objstream->readSint16LE();
	int16 universe_z = objstream->readSint16LE();

	// TODO: this doesn't work properly (see DrawOrderComparison)
	z_adjust = objstream->readUint16LE();

	uint16 unknown5 = objstream->readUint16LE(); // XXX

	uint16 sprite_id = objstream->readUint16LE();
	if (sprite_id != 0xffff && sprite_id != 0xfffe) {
		Common::String sprfilename = _vm->getSpriteFilename(sprite_id);
		sprite = new SpritePlayer(new Sprite(_vm->openFile(sprfilename)), this, _vm);
		sprite->startAnim(0); // XXX
	} else {
		sprite = 0;
	}

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

	for (unsigned int i = 0; i < 6; i++) {
		uint16 unknowna = objstream->readUint16LE(); // XXX
		byte unknownb = objstream->readByte();
		byte unknownc = objstream->readByte();
	}

	uint16 unknown14 = objstream->readUint16LE(); // XXX
	uint16 unknown15 = objstream->readUint16LE(); // XXX

	char _str[100];
	objstream->read(_str, 100);

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
		readBlock(blockType, objstream);
	}

	assert(descriptions.size() == description_count);

	delete objstream;
}

int Object::readBlockHeader(Common::SeekableReadStream *objstream) {
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
			readEntryList(objstream, use_entries);
			break;

		case BLOCK_GET_ENTRIES:
			readEntryList(objstream, get_entries);
			break;

		case BLOCK_LOOK_ENTRIES:
			readEntryList(objstream, look_entries);
			break;

		case BLOCK_TIMER_ENTRIES:
			readEntryList(objstream, timer_entries);
			break;

		default:
			// either an invalid block type, or it shouldn't be here
			error("bad block type %x encountered while parsing object", type);
	}
}

void Object::readEntryList(Common::SeekableReadStream *objstream, EntryList &entries) {
	byte num_entries = objstream->readByte();

	for (unsigned int i = 0; i < num_entries; i++) {
		int type = readBlockHeader(objstream);
		if (type != BLOCK_BEGIN_ENTRY)
			error("bad block type %x encountered while parsing entry", type);
		VERIFY_LENGTH(0xae);

		uint16 header = objstream->readUint16LE();
		assert(header == 9);

		// XXX: seeking over important data!
		objstream->seek(0xac, SEEK_CUR);

		while (true) {
			type = readBlockHeader(objstream);

			if (type == BLOCK_END_ENTRY)
				break;

			readEntry(type, objstream, entries);
		}
	}
}

void Object::readEntry(int type, Common::SeekableReadStream *objstream, EntryList &entries) {
	uint16 header;

	switch (type) {
		case BLOCK_CONDITION:
			VERIFY_LENGTH(0xda);
			header = objstream->readUint16LE();
			assert(header == 9);
			objstream->seek(0xd8, SEEK_CUR); // XXX
			break;

		case BLOCK_ALTER:
			while (true) {
				VERIFY_LENGTH(0x105);
				header = objstream->readUint16LE();
				assert(header == 9);
				objstream->seek(0x103, SEEK_CUR); // XXX

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
				objstream->seek(0x85, SEEK_CUR); // XXX

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
				objstream->seek(0x82, SEEK_CUR); // XXX

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
			objstream->seek(0x8e, SEEK_CUR); // XXX
			break;

		case BLOCK_PATH:
			VERIFY_LENGTH(0x17b);
			header = objstream->readUint16LE();
			assert(header == 9);
			objstream->seek(0x179, SEEK_CUR); // XXX
			break;

		case BLOCK_GENERAL:
			VERIFY_LENGTH(0x7b);
			header = objstream->readUint16LE();
			assert(header == 9);
			objstream->seek(0x79, SEEK_CUR); // XXX
			break;

		case BLOCK_CONVERSATION:
			while (true) {
				VERIFY_LENGTH(0x7c);
				header = objstream->readUint16LE();
				assert(header == 9);
				objstream->seek(0x7a, SEEK_CUR); // XXX

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
			objstream->seek(0x9d, SEEK_CUR); // XXX
			break;

		case BLOCK_TRIGGER:
			while (true) {
				VERIFY_LENGTH(0x78);
				header = objstream->readUint16LE();
				assert(header == 9);
				objstream->seek(0x76, SEEK_CUR); // XXX

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
			objstream->seek(0x7a, SEEK_CUR); // XXX
			break;

		case BLOCK_CHOICE:
			VERIFY_LENGTH(0x10b);
			header = objstream->readUint16LE();
			assert(header == 9);
			objstream->seek(0x109, SEEK_CUR); // XXX

			while (true) {
				type = readBlockHeader(objstream);
				if (type == BLOCK_END_BLOCK)
					break;
				// XXX: decode these properly
				EntryList temp;
				if (type == 0x26) {
					type = readBlockHeader(objstream);
					readEntry(type, objstream, temp);
				} else if (type == 0x27) {
					type = readBlockHeader(objstream);
					readEntry(type, objstream, temp);
				} else
					error("bad block type %x encountered while parsing choices", type);
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

} // Unity

