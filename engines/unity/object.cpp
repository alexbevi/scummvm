#include "object.h"
#include "unity.h"
#include "sprite.h"

#include "common/stream.h"

namespace Unity {

void Object::loadObject(UnityEngine *_vm, unsigned int for_world, unsigned int for_screen, unsigned int for_id) {
	Common::String filename = Common::String::printf("o_%02x%02x%02x.bst", for_world, for_screen, for_id);
	Common::SeekableReadStream *objstream = _vm->openFile(filename);

	uint32 header = objstream->readUint32LE();
	assert(header == 0x01281100); // magic header value?

	id = objstream->readByte();
	assert(id == for_id);
	screen = objstream->readByte();
	assert(screen == for_screen);
	world = objstream->readByte();
	assert(world == for_world);

	byte unknown1 = objstream->readByte();
	assert(unknown1 == 0);
	byte unknown2 = objstream->readByte(); // XXX
	byte unknown3 = objstream->readByte(); // XXX

	width = objstream->readSint16LE();
	height = objstream->readSint16LE();

	int16 world_x = objstream->readSint16LE();
	int16 world_y = objstream->readSint16LE();
	x = world_x;
	y = world_y;

	// XXX: do something with these!
	int16 world_z = objstream->readSint16LE();
	int16 universe_x = objstream->readSint16LE();
	int16 universe_y = objstream->readSint16LE();
	int16 universe_z = objstream->readSint16LE();

	uint16 unknown4 = objstream->readUint16LE(); // XXX
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

	uint8 flags = objstream->readByte();
	// XXX: no idea if 0x20 is correct for active, but it seems so
	active = (flags & 0x20) == 0x20;

	byte unknown9 = objstream->readByte();
	uint16 unknown10 = objstream->readUint16LE(); // XXX
	uint16 unknown11 = objstream->readUint16LE(); // XXX

	byte unknown12 = objstream->readByte();
	byte unknown13 = objstream->readByte();

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

	uint16 unknown18 = objstream->readUint16LE(); // XXX
	assert(unknown18 == 0x0);
	uint16 unknown19 = objstream->readUint16LE(); // XXX
	assert(unknown19 == 0x0);
	uint16 unknown20 = objstream->readUint16LE(); // XXX
	assert(unknown20 == 0x0);

	uint16 unknown21 = objstream->readUint16LE(); // XXX
	uint32 unknown22 = objstream->readUint32LE(); // XXX
	uint32 unknown23 = objstream->readUint32LE(); // XXX
	uint16 unknown24 = objstream->readUint16LE(); // XXX
	uint16 unknown25 = objstream->readUint16LE(); // XXX
	uint16 unknown26 = objstream->readUint16LE(); // XXX
	uint16 unknown27 = objstream->readUint16LE(); // XXX
	assert(unknown27 == 0x0);

	for (unsigned int i = 0; i < 21; i++) {
		uint32 unknowna = objstream->readUint32LE(); // XXX
		assert(unknowna == 0x0);
	}

	byte blockType = objstream->readByte();
	while (!objstream->eos()) {
		readBlock(blockType, objstream);
		blockType = objstream->readByte();
	}

	delete objstream;
}

void Object::readBlock(byte type, Common::SeekableReadStream *objstream) {
	byte header = objstream->readByte();
	assert(header == 0x11);

	if (type != 0x1) {
		// XXX: too lazy to implement the rest right now, ensure EOS
		objstream->seek(objstream->size());
		return;
	}

	uint16 block_length = objstream->readUint16LE();
	assert(block_length == 0xa5);

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

	// XXX: this is totally a seperate block, but we'll cheat for now
	type = objstream->readByte();
	assert(type == 0x40);
	header = objstream->readByte();
	assert(header == 0x11);
	block_length = objstream->readUint16LE();
	assert(block_length == 0x0c);

	Description desc;
	desc.text = text;
	desc.entry_id = entry_for;

	desc.voice_id = objstream->readUint32LE();
	desc.voice_group = objstream->readUint32LE();
	desc.voice_subgroup = objstream->readUint32LE();

	descriptions.push_back(desc);
}

} // Unity

