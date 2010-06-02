#include "object.h"
#include "unity.h"
#include "sprite.h"

#include "common/stream.h"

namespace Unity {

#define VERIFY_LENGTH(x) do { uint16 block_length = objstream->readUint16LE(); assert(block_length == x); } while (0)

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
	assert(type == 0x00);
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
		case 0x00:
			error("header block type encountered after header");

		case 0x01:
			readDescriptionBlock(objstream);
			break;

		case 0x40:
			// should only occur immediately after description block, handled there
			error("voice entry block encountered outside description");

		default:
			// XXX: too lazy to implement the rest right now, ensure EOS
			objstream->seek(objstream->size());
			break;
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
	assert(type == 0x40);
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

