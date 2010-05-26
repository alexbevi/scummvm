#include "graphics.h"
#include "common/system.h"

namespace Unity {

Graphics::Graphics(UnityEngine *_engine) : engine(_engine) {
}

Graphics::~Graphics() {
}

void Graphics::drawBackgroundImage(const char *filename) {
	Common::SeekableReadStream *palStream = engine->openFile("STANDARD.PAL");
	byte *palette = (byte *)malloc(256 * 4);
	for (uint16 i = 0; i < 128; i++) {
		palette[128*4 + i * 4] = palStream->readByte();
		palette[128*4 + i * 4 + 1] = palStream->readByte();
		palette[128*4 + i * 4 + 2] = palStream->readByte();
		palette[128*4 + i * 4 + 3] = 0;
	}
	delete palStream;

	Common::SeekableReadStream *scrStream = engine->openFile(filename);
	for (uint16 i = 0; i < 128; i++) {
		palette[i * 4] = scrStream->readByte();
		palette[i * 4 + 1] = scrStream->readByte();
		palette[i * 4 + 2] = scrStream->readByte();
		palette[i * 4 + 3] = 0;
	}

	for (uint16 i = 0; i < 256; i++) {
		for (byte j = 0; j < 3; j++) {
			palette[i * 4 + j] = palette[i * 4 + j] << 2;
		}
	}

	engine->_system->setPalette(palette, 0, 256);
	free(palette);

	// TODO
	unsigned int width = 640, height = 400;

	byte *pixels = (byte *)malloc(width * height);
	scrStream->read(pixels, width * height);

	// TODO: make _vm?
	engine->_system->copyRectToScreen(pixels, width, 0, 0, width, height);
	engine->_system->updateScreen();

	free(pixels);
	delete scrStream;
}

}

