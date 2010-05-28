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

	_gfx->setBackgroundImage("sb004001.scr");
	_gfx->drawBackgroundImage();
	_gfx->drawMRG("awayteam.mrg", 0);

	Common::SeekableReadStream *ourstr = openFile("picard.spr");
	Sprite sprite(ourstr);
	SpritePlayer spr(&sprite);
	printf("picard has %d animations\n", spr.numAnims());
	unsigned int i = 0;
	spr.startAnim(i);

	Common::Event event;
	while (!shouldQuit()) {
		while (_eventMan->pollEvent(event)) {
			switch (event.type) {
				case Common::EVENT_QUIT:
					_system->quit();
					break;

				case Common::EVENT_KEYUP:
					printf("trying anim %d\n", i);
					i++;
					i %= spr.numAnims();
					spr.startAnim(i);
					break;

				default:
					break;
			}
		}
		_gfx->drawBackgroundImage();
		spr.update();
		_gfx->drawSprite(&spr, 150, 150);
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

