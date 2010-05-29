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

	//_snd->playSpeech("02140000.vac");

	unsigned int curr_loc = 4;
	unsigned int curr_screen = 1;
	openLocation(curr_loc, curr_screen);

	_gfx->drawMRG("awayteam.mrg", 0);

	Common::Array<SpritePlayer *> sprites;
	for (unsigned int i = 0; i < 4; i++) {
		Common::String filename = getSpriteFilename(i);
		sprites.push_back(new SpritePlayer(new Sprite(openFile(filename))));
	}
	unsigned int anim = 26;
	for (unsigned int i = 0; i < sprites.size(); i++) sprites[i]->startAnim(anim);

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
					anim %= sprites[0]->numAnims();
					for (unsigned int i = 0; i < sprites.size(); i++) sprites[i]->startAnim(anim);
					break;

				case Common::EVENT_LBUTTONUP:
					curr_screen++;
					openLocation(curr_loc, curr_screen);
					break;

				default:
					break;
			}
		}

		_gfx->drawBackgroundImage();
		_gfx->drawBackgroundPolys(current_screen.polygonsFilename);

		for (unsigned int i = 0; i < sprites.size(); i++) {
			sprites[i]->update();
			_gfx->drawSprite(sprites[i], current_screen.entrypoints[0][i].x, current_screen.entrypoints[0][i].y);
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

