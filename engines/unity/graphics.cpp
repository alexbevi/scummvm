#include "graphics.h"
#include "sprite.h"
#include "common/system.h"

namespace Unity {

Graphics::Graphics(UnityEngine *_engine) : _vm(_engine) {
	basePalette = palette = 0;
	backgroundPixels = 0;
}

Graphics::~Graphics() {
	delete[] basePalette;
	delete[] palette;
	delete[] backgroundPixels;
}

void Graphics::init() {
	// read the standard palette entries
	basePalette = new byte[128 * 4];
	Common::SeekableReadStream *palStream = _vm->openFile("STANDARD.PAL");
	for (uint16 i = 0; i < 128; i++) {
		basePalette[i * 4] = palStream->readByte();
		basePalette[i * 4 + 1] = palStream->readByte();
		basePalette[i * 4 + 2] = palStream->readByte();
		basePalette[i * 4 + 3] = 0;
	}
	delete palStream;
}

void Graphics::drawMRG(Common::String filename, unsigned int entry) {
	Common::SeekableReadStream *mrgStream = _vm->openFile(filename);

	uint16 num_entries = mrgStream->readUint16LE();
	assert(entry < num_entries);
	assert(mrgStream->seek(entry * 4, SEEK_CUR));
	uint32 offset = mrgStream->readUint32LE();
	assert(mrgStream->seek(offset, SEEK_SET));

	uint16 width = mrgStream->readUint16LE();
	uint16 height = mrgStream->readUint16LE();

	byte *pixels = new byte[width * height];
	mrgStream->read(pixels, width * height);

	// TODO: positioning
	_vm->_system->copyRectToScreen(pixels, width, 0, 400, width, height);

	delete[] pixels;
	delete mrgStream;
}

void Graphics::setBackgroundImage(Common::String filename) {
	delete[] palette;
	palette = new byte[256 * 4];

	Common::SeekableReadStream *scrStream = _vm->openFile(filename);
	for (uint16 i = 0; i < 128; i++) {
		palette[i * 4] = scrStream->readByte();
		palette[i * 4 + 1] = scrStream->readByte();
		palette[i * 4 + 2] = scrStream->readByte();
		palette[i * 4 + 3] = 0;
	}
	memcpy(palette + 128*4, basePalette, 128*4);

	for (uint16 i = 0; i < 256; i++) {
		for (byte j = 0; j < 3; j++) {
			palette[i * 4 + j] = palette[i * 4 + j] << 2;
		}
	}

	_vm->_system->setPalette(palette, 0, 256);

	// some of the files seem to be 480 high, but just padded with black
	backgroundWidth = 640;
	backgroundHeight = 400;

	delete[] backgroundPixels;
	backgroundPixels = new byte[backgroundWidth * backgroundHeight];
	scrStream->read(backgroundPixels, backgroundWidth * backgroundHeight);
	delete scrStream;
}

void Graphics::drawBackgroundImage() {
	assert(backgroundPixels);

	_vm->_system->copyRectToScreen(backgroundPixels, backgroundWidth, 0, 0, backgroundWidth, backgroundHeight);
}

void Graphics::drawSprite(SpritePlayer *sprite, unsigned int x, unsigned int y) {
	assert(sprite);
	// XXX: this doesn't work properly, either
	unsigned int width = sprite->getCurrentWidth();
	unsigned int height = sprite->getCurrentHeight();
	_vm->_system->copyRectToScreen(sprite->getCurrentData(), width,
		x + sprite->getXPos(), y + sprite->getYPos(), width, height);
	if (sprite->speaking()) {
		// XXX: this doesn't work properly, SpritePlayer side probably needs work too
		unsigned int m_width = sprite->getSpeechWidth();
		unsigned int m_height = sprite->getSpeechHeight();
		_vm->_system->copyRectToScreen(sprite->getSpeechData(), m_width,
			x + width/2 - m_width/2 + sprite->getMouthXPos(),
			y + height - m_height + sprite->getMouthYPos(), m_width, m_height);
	}
}

}

