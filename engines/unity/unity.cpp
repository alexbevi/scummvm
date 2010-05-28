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
	Common::Array<uint32> offsets;
	offsets.reserve(num_entries);
	for (unsigned int i = 0; i < num_entries; i++) {
		uint32 id = locstream->readUint32LE();
		assert(id == i + 1); // XXX
		uint32 offset = locstream->readUint32LE();
		offsets.push_back(offset);
	}

	if (screen > num_entries) {
		error("no such screen %d in location %d (only %d screens)", screen, location, num_entries);
	}

	locstream->seek(offsets[screen - 1]);

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

	polygonsFilename = polygons;

	byte num_entrances = locstream->readByte();
	// TODO: don't ignore all but the first entrance
	// XXX: read this properly
	uint16 unknown = locstream->readUint16BE(); // 0001, 0101, 0201, ...
	for (unsigned int i = 0; i < 4; i++) {
		entrypoints[i].x = locstream->readUint16LE();
		entrypoints[i].y = locstream->readUint16LE();
	}

	delete locstream;
}

Common::Error UnityEngine::run() {
	init();

	initGraphics(640, 480, true);
	_gfx->init();
	_snd->init();

	// XXX: this mouse cursor is borrowed from SCI
	CursorMan.replaceCursor(sciMouseCursor, 11, 16, 1, 1, 0);
	CursorMan.showMouse(true);
	_system->updateScreen();

	//_snd->playSpeech("02140000.vac");

	openLocation(4, 1);

	_gfx->drawMRG("awayteam.mrg", 0);

	Common::Array<SpritePlayer *> sprites;
	sprites.push_back(new SpritePlayer(new Sprite(openFile("picard.spr"))));
	sprites.push_back(new SpritePlayer(new Sprite(openFile("data.spr"))));
	sprites.push_back(new SpritePlayer(new Sprite(openFile("laforge.spr"))));
	sprites.push_back(new SpritePlayer(new Sprite(openFile("troi.spr"))));
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

				default:
					break;
			}
		}

		_gfx->drawBackgroundImage();
		_gfx->drawBackgroundPolys(polygonsFilename);

		for (unsigned int i = 0; i < sprites.size(); i++) {
			sprites[i]->update();
			_gfx->drawSprite(sprites[i], entrypoints[i].x, entrypoints[i].y);
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

} // Unity

