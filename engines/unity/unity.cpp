#include "unity.h"
#include "graphics.h"
#include "sound.h"
#include "sprite.h"
#include "object.h"

#include "common/fs.h"
#include "common/config-manager.h"
#include "engines/util.h"
#include "common/events.h"
#include "common/system.h"
#include "common/unzip.h"
#include "common/file.h"
#include "common/archive.h"
#include "graphics/cursorman.h"
#include "common/EventRecorder.h"

namespace Unity {

/**
 * A black and white SCI-style arrow cursor (11x16).
 * 0 = Transparent.
 * 1 = Black (#000000 in 24-bit RGB).
 * 2 = White (#FFFFFF in 24-bit RGB).
 */
static const byte sciMouseCursor[] = {
	1,1,0,0,0,0,0,0,0,0,0,
	1,2,1,0,0,0,0,0,0,0,0,
	1,2,2,1,0,0,0,0,0,0,0,
	1,2,2,2,1,0,0,0,0,0,0,
	1,2,2,2,2,1,0,0,0,0,0,
	1,2,2,2,2,2,1,0,0,0,0,
	1,2,2,2,2,2,2,1,0,0,0,
	1,2,2,2,2,2,2,2,1,0,0,
	1,2,2,2,2,2,2,2,2,1,0,
	1,2,2,2,2,2,2,2,2,2,1,
	1,2,2,2,2,2,1,0,0,0,0,
	1,2,1,0,1,2,2,1,0,0,0,
	1,1,0,0,1,2,2,1,0,0,0,
	0,0,0,0,0,1,2,2,1,0,0,
	0,0,0,0,0,1,2,2,1,0,0,
	0,0,0,0,0,0,1,2,2,1,0
};

UnityEngine::UnityEngine(OSystem *syst) : Engine(syst) {
}

UnityEngine::~UnityEngine() {
	delete _snd;
	delete _gfx;
	delete data;
}

Common::Error UnityEngine::init() {
	g_eventRec.registerRandomSource(_rnd, "unity");

	_gfx = new Graphics(this);
	_snd = new Sound(this);

	data = Common::makeZipArchive("STTNG.ZIP");
	if (!data) {
		error("couldn't open data file");
	}
	/*Common::ArchiveMemberList list;
	data->listMembers(list);
	for (Common::ArchiveMemberList::const_iterator file = list.begin(); file != list.end(); ++file) {
		Common::String filename = (*file)->getName();
		filename.toLowercase();
		if (filename.hasSuffix(".spr") || filename.hasSuffix(".spt")) {
			Common::SeekableReadStream *ourstr = (*file)->createReadStream();
			printf("trying '%s'\n", filename.c_str());
			Sprite spr(ourstr);
		}
	}*/
	SearchMan.add("sttngzip", data);

	return Common::kNoError;
}

void UnityEngine::openLocation(unsigned int world, unsigned int screen) {
	Common::String filename = Common::String::printf("sl%03d.scr", world);
	Common::SeekableReadStream *locstream = openFile(filename);

	uint16 num_entries = locstream->readUint16LE();
	Common::HashMap<uint32, uint32> offsets;
	for (unsigned int i = 0; i < num_entries; i++) {
		uint32 id = locstream->readUint32LE();
		uint32 offset = locstream->readUint32LE();
		offsets[id] = offset;
	}

	if (screen > num_entries) {
		error("no such screen %d in world %d (only %d screens)", screen, world, num_entries);
	}

	locstream->seek(offsets[screen]);

	// XXX: null-terminated in file?
	byte length = locstream->readByte();
	char background[length + 1];
	locstream->read(background, length);
	background[length] = 0;

	_gfx->setBackgroundImage(background);

	// XXX: null-terminated in file?
	length = locstream->readByte();
	char polygons[length + 1];
	locstream->read(polygons, length);
	polygons[length] = 0;

	current_screen.polygonsFilename = polygons;

	current_screen.entrypoints.clear();
	byte num_entrances = locstream->readByte();
	for (unsigned int e = 0; e < num_entrances; e++) {
		Common::Array<Common::Point> entrypoints;
		// XXX: read this properly
		uint16 unknown = locstream->readUint16BE(); // 0001, 0101, 0201, ...
		entrypoints.resize(4);
		for (unsigned int i = 0; i < 4; i++) {
			entrypoints[i].x = locstream->readUint16LE();
			entrypoints[i].y = locstream->readUint16LE();
		}
		current_screen.entrypoints.push_back(entrypoints);
	}

	delete locstream;

	// TODO: what do we do with all the objects?
	// for now, we just delete all but the away team
	for (unsigned int i = 4; i < objects.size(); i++) {
		delete objects[i]->sprite;
		delete objects[i];
	}
	objects.resize(4);

	filename = Common::String::printf("w%02x%02xobj.bst", world, screen);
	locstream = openFile(filename);

	// TODO: is there data we need here in w_XXstrt.bst?
	// (seems to be only trigger activations)
	while (true) {
		uint16 counter = locstream->readUint16LE();
		if (locstream->eos()) break;

		byte id = locstream->readByte();
		assert(id == counter);
		byte _screen = locstream->readByte();
		assert(_screen == screen);
		byte _world = locstream->readByte();
		assert(_world == world);

		byte unknown4 = locstream->readByte();
		assert(unknown4 == 0);

		char _name[30], _desc[260];
		locstream->read(_name, 30);
		locstream->read(_desc, 260);
		//printf("reading obj '%s' (%s)\n", _name, _desc);

		loadObject(world, screen, id);
	}

	delete locstream;
}

void UnityEngine::loadObject(unsigned int world, unsigned int screen, unsigned int id) {
	Common::String filename = Common::String::printf("o_%02x%02x%02x.bst", world, screen,id);
	Common::SeekableReadStream *objstream = openFile(filename);

	uint32 header = objstream->readUint32LE();
	assert(header == 0x01281100); // magic header value?

	Object *obj = new Object;

	obj->id = objstream->readByte();
	obj->screen = objstream->readByte();
	obj->world = objstream->readByte();

	byte unknown1 = objstream->readByte();
	assert(unknown1 == 0);
	byte unknown2 = objstream->readByte(); // XXX
	byte unknown3 = objstream->readByte(); // XXX

	obj->width = objstream->readSint16LE();
	obj->height = objstream->readSint16LE();

	int16 world_x = objstream->readSint16LE();
	int16 world_y = objstream->readSint16LE();
	obj->x = world_x;
	obj->y = world_y;

	// XXX: do something with these!
	int16 world_z = objstream->readSint16LE();
	int16 universe_x = objstream->readSint16LE();
	int16 universe_y = objstream->readSint16LE();
	int16 universe_z = objstream->readSint16LE();

	uint16 unknown4 = objstream->readUint16LE(); // XXX
	uint16 unknown5 = objstream->readUint16LE(); // XXX

	uint16 sprite_id = objstream->readUint16LE();
	if (sprite_id != 0xffff && sprite_id != 0xfffe) {
		Common::String sprfilename = getSpriteFilename(sprite_id);
		SpritePlayer *sprite = new SpritePlayer(new Sprite(openFile(sprfilename)), obj, this);
		obj->sprite = sprite;
		obj->sprite->startAnim(0); // XXX
	} else {
		obj->sprite = 0;
	}

	uint16 unknown6 = objstream->readUint16LE(); // XXX
	uint16 unknown7 = objstream->readUint16LE(); // XXX

	uint8 flags = objstream->readByte();
	// XXX: no idea if 0x20 is correct for active, but it seems so
	obj->active = (flags & 0x20) == 0x20;

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
		readBlock(blockType, obj, objstream);
		blockType = objstream->readByte();
	}

	objects.push_back(obj);

	delete objstream;
}

void UnityEngine::readBlock(byte type, Object *obj, Common::SeekableReadStream *objstream) {
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

	obj->descriptions.push_back(desc);
}

struct DrawOrderComparison {
	bool operator() (const Object *a, const Object *b) {
		return a->y < b->y;
	}
};

Object *UnityEngine::objectAt(unsigned int x, unsigned int y) {
	for (unsigned int i = 0; i < objects.size(); i++) {
		if (!objects[i]->active) continue;
		if (objects[i]->width == ~0) continue;

		// TODO: should we be doing this like this, or keeping track of original coords, or what?
		if (objects[i]->x - objects[i]->width/2 > x) continue;
		if (objects[i]->x + objects[i]->width/2 < x) continue;
		if (objects[i]->y - objects[i]->height > y) continue;
		if (objects[i]->y < y) continue;
		return objects[i];
	}

	return 0;
}

Common::Error UnityEngine::run() {
	init();

	initGraphics(640, 480, true);
	_gfx->init();
	_snd->init();

	loadSpriteFilenames();

	// XXX: this mouse cursor is borrowed from SCI
	CursorMan.replaceCursor(sciMouseCursor, 11, 16, 1, 1, 0);
	CursorMan.showMouse(true);

	// and we stomp over it anyway, but this only good for some situations :)
	_gfx->setCursor(0, false);

	//_snd->playSpeech("02140000.vac");

	// beam in an away team
	for (unsigned int i = 0; i < 4; i++) {
		loadObject(0, 0, i);
	}
	unsigned int anim = 26;
	for (unsigned int i = 0; i < 4; i++) objects[i]->sprite->startAnim(anim);

	unsigned int curr_loc = 4;
	unsigned int curr_screen = 1;
	openLocation(curr_loc, curr_screen);
	for (unsigned int i = 0; i < 4; i++) {
		objects[i]->x = current_screen.entrypoints[0][i].x;
		objects[i]->y = current_screen.entrypoints[0][i].y;
	}

	// draw UI
	_gfx->drawMRG("awayteam.mrg", 0);

	Common::Event event;
	while (!shouldQuit()) {
		while (_eventMan->pollEvent(event)) {
			switch (event.type) {
				case Common::EVENT_QUIT:
					_system->quit();
					break;

				case Common::EVENT_KEYUP:
					printf("trying anim %d\n", anim);
					anim++;
					anim %= objects[0]->sprite->numAnims();
					for (unsigned int i = 0; i < 4; i++)
						objects[i]->sprite->startAnim(anim);
					break;

				case Common::EVENT_RBUTTONUP:
					curr_screen++;
					openLocation(curr_loc, curr_screen);
					for (unsigned int i = 0; i < 4; i++) {
						objects[i]->x = current_screen.entrypoints[0][i].x;
						objects[i]->y = current_screen.entrypoints[0][i].y;
					}
					break;

				case Common::EVENT_MOUSEMOVE:
					if (objectAt(event.mouse.x, event.mouse.y)) {
						_gfx->setCursor(1, false);
					} else {
						_gfx->setCursor(0, false);
					}
					break;

				case Common::EVENT_LBUTTONUP: {
					Object *obj = objectAt(event.mouse.x, event.mouse.y);
					if (obj && obj->descriptions.size()) {
						// TODO: this is very wrong, of course :)
						Description &desc = obj->descriptions[0];
						Common::String file;
						file = Common::String::printf("%02x%02x%02x%02x.vac",
							desc.voice_group, desc.entry_id, desc.voice_subgroup,
							desc.voice_id);
						if (!SearchMan.hasFile(file)) {
							// TODO: this is broken
							// for these in-area files, it's sometimes %02x%c%1x%02x%02x.vac ?!
							// where %c is 'l' or 't' - i haven't worked out why yet
							file = Common::String::printf("%02x%c%1x%02x%02x.vac",
								desc.voice_group, 'l', desc.voice_subgroup,
								desc.entry_id, desc.voice_id);
							if (!SearchMan.hasFile(file))
								file = Common::String::printf("%02x%c%1x%02x%02x.vac",
								desc.voice_group, 't', desc.voice_subgroup,
								desc.entry_id, desc.voice_id);
						}
						_snd->playSpeech(file);
					}
					} break;

				default:
					break;
			}
		}

		_gfx->drawBackgroundImage();
		//_gfx->drawBackgroundPolys(current_screen.polygonsFilename);

		Common::Array<Object *> to_draw;
		for (unsigned int i = 0; i < objects.size(); i++) {
			if (objects[i]->sprite && objects[i]->active) {
				objects[i]->sprite->update();

				// TODO
				if (!objects[i]->sprite->valid()) { warning("invalid sprite?"); continue; }

				to_draw.push_back(objects[i]);
			}
		}

		Common::sort(to_draw.begin(), to_draw.end(), DrawOrderComparison());
		for (unsigned int i = 0; i < to_draw.size(); i++) {
			_gfx->drawSprite(to_draw[i]->sprite, to_draw[i]->x, to_draw[i]->y);
		}

		_system->updateScreen();
	}

	return Common::kNoError;
}

Common::SeekableReadStream *UnityEngine::openFile(Common::String filename) {
	Common::SeekableReadStream *stream = SearchMan.createReadStreamForMember(filename);
	if (!stream) error("couldn't open '%s'", filename.c_str());
	return stream;
}

void UnityEngine::loadSpriteFilenames() {
	Common::SeekableReadStream *stream = openFile("sprite.lst");

	uint32 num_sprites = stream->readUint32LE();
	sprite_filenames.reserve(num_sprites);

	for (unsigned int i = 0; i < num_sprites; i++) {
		char buf[9]; // DOS filenames, so 9 should really be enough
		for (unsigned int j = 0; j < 9; j++) {
			char c = stream->readByte();
			buf[j] = c;
			if (c == 0) break;
			assert (j != 8);
		}
		sprite_filenames.push_back(buf);
	}

	delete stream;
}

Common::String UnityEngine::getSpriteFilename(unsigned int id) {
	assert(id != 0xffff && id != 0xfffe);
	assert(id < sprite_filenames.size());
	return sprite_filenames[id] + ".spr";
}

} // Unity

