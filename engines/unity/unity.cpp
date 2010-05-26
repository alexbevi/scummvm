#include "unity.h"
#include "graphics.h"

#include "common/fs.h"
#include "common/config-manager.h"
#include "engines/util.h"
#include "common/events.h"
#include "common/system.h"
#include "common/unzip.h"
#include "common/file.h"
#include "common/archive.h"

namespace Unity {

UnityEngine::UnityEngine(OSystem *syst) : Engine(syst) {
}

UnityEngine::~UnityEngine() {
	delete _gfx;
	delete data;
}

Common::Error UnityEngine::init() {
	_gfx = new Graphics(this);

	data = Common::makeZipArchive("STTNG.ZIP");
	if (!data) {
		error("couldn't open data file");
	}
	SearchMan.add("sttngzip", data);

	return Common::kNoError;
}

Common::Error UnityEngine::run() {
	init();

	initGraphics(640, 480, true);
	_gfx->init();

	_gfx->setBackgroundImage("sb003003.scr");
	_gfx->drawBackgroundImage();

	Common::Event event;
	while (!shouldQuit()) {
		while (_eventMan->pollEvent(event)) {
			switch (event.type) {
				case Common::EVENT_QUIT:
					_system->quit();
					break;

				default:
					break;
			}
		}
	}

	return Common::kNoError;
}

Common::SeekableReadStream *UnityEngine::openFile(Common::String filename) {
	Common::SeekableReadStream *stream = SearchMan.createReadStreamForMember(filename);
	if (!stream) error("couldn't open '%s'", filename.c_str());
	return stream;
}

} // Unity

