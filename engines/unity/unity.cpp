#include "unity.h"
#include "graphics.h"
#include "sound.h"
#include "sprite.h"

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

void UnityEngine::openLocation(unsigned int location, unsigned int screen) {
	Common::String filename = Common::String::printf("sl%03d.scr", location);
	Common::SeekableReadStream *locstream = openFile(filename);

	uint16 num_entries = locstream->readUint16LE();
	Common::HashMap<uint32, uint32> offsets;
	for (unsigned int i = 0; i < num_entries; i++) {
		uint32 id = locstream->readUint32LE();
		uint32 offset = locstream->readUint32LE();
		offsets[id] = offset;
	}

	if (screen > num_entries) {
		error("no such screen %d in location %d (only %d screens)", screen, location, num_entries);
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

	filename = Common::String::printf("w%02x%02xobj.bst", location, screen);
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
		byte _location = locstream->readByte();
		assert(_location == location);

		byte unknown4 = locstream->readByte();
		assert(unknown4 == 0);

		char _name[30], _desc[260];
		locstream->read(_name, 30);
		locstream->read(_desc, 260);
		//printf("reading obj '%s' (%s)\n", _name, _desc);

		loadObject(location, screen, id);
	}

	delete locstream;
}

void UnityEngine::loadObject(unsigned int location, unsigned int screen, unsigned int id) {
	Common::String filename = Common::String::printf("o_%02x%02x%02x.bst", location, screen,id);
	Common::SeekableReadStream *objstream = openFile(filename);

	uint32 header = objstream->readUint32LE();
	assert(header == 0x01281100); // magic header value?

	byte _id = objstream->readByte();
	byte _screen = objstream->readByte();
	byte _location = objstream->readByte();

	byte unknown1 = objstream->readByte();
	assert(unknown1 == 0);
	byte unknown2 = objstream->readByte(); // XXX
	byte unknown3 = objstream->readByte(); // XXX

	// XXX: do something with these!
	int16 width = objstream->readSint16LE();
	int16 height = objstream->readSint16LE();
	int16 world_x = objstream->readSint16LE();
	int16 world_y = objstream->readSint16LE();
	int16 world_z = objstream->readSint16LE();
	int16 universe_x = objstream->readSint16LE();
	int16 universe_y = objstream->readSint16LE();
	int16 universe_z = objstream->readSint16LE();

	uint16 unknown4 = objstream->readUint16LE(); // XXX
	uint16 unknown5 = objstream->readUint16LE(); // XXX

	uint16 sprite_id = objstream->readUint16LE();

	uint16 unknown6 = objstream->readUint16LE(); // XXX
	uint16 unknown7 = objstream->readUint16LE(); // XXX

	uint8 flags = objstream->readByte();

	// XXX: no idea if 0x20 is correct for active, but it seems so
	if ((flags & 0x20) == 0x20 && sprite_id != 0xffff && sprite_id != 0xfffe) {
		// TODO: this is so terribly, terribly wrong, and we should be storing *all* objects anyway
		Common::String sprfilename = getSpriteFilename(sprite_id);
		Object *obj = new Object;
		SpritePlayer *sprite = new SpritePlayer(new Sprite(openFile(sprfilename)), obj, this);
		obj->sprite = sprite;
		obj->sprite->startAnim(0); // XXX
		obj->x = world_x;
		obj->y = world_y;
		objects.push_back(obj);
	}

	// XXX: lots more

	delete objstream;
}

struct DrawOrderComparison {
	bool operator() (const Object *a, const Object *b) {
		return a->y < b->y;
	}
};

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

				case Common::EVENT_LBUTTONUP:
					curr_screen++;
					openLocation(curr_loc, curr_screen);
					for (unsigned int i = 0; i < 4; i++) {
						objects[i]->x = current_screen.entrypoints[0][i].x;
						objects[i]->y = current_screen.entrypoints[0][i].y;
					}
					break;

				default:
					break;
			}
		}

		_gfx->drawBackgroundImage();
		//_gfx->drawBackgroundPolys(current_screen.polygonsFilename);

		Common::Array<Object *> to_draw;
		for (unsigned int i = 0; i < objects.size(); i++) {
			objects[i]->sprite->update();
			to_draw.push_back(objects[i]);
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

