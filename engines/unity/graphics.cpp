#include "graphics.h"
#include "sprite.h"
#include "common/system.h"
#include "graphics/surface.h"

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

// XXX: default transparent is a hack (see header file)
void Graphics::blit(byte *data, int x, int y, unsigned int width, unsigned int height, byte transparent) {
	// TODO: work with internal buffer and dirty-rectangling?
	::Graphics::Surface *surf = _vm->_system->lockScreen();
	// XXX: this code hasn't had any thought
	int startx = 0, starty = 0;
	if (x < 0) startx = -x;
	if (y < 0) starty = -y;
	unsigned int usewidth = width, useheight = height;
	if (x - startx + width > surf->w) usewidth = surf->w - (x - startx);
	if (y - starty + height > surf->h) useheight = surf->h - (y - starty);
	for (unsigned int xpos = startx; xpos < usewidth; xpos++) {
		for (unsigned int ypos = starty; ypos < useheight; ypos++) {
			byte pixel = data[xpos + ypos*width];
			if (pixel != transparent)
				*((byte *)surf->getBasePtr(x + (int)xpos, y + (int)ypos)) = pixel;
		}
	}

	_vm->_system->unlockScreen();
}

void Graphics::drawSprite(SpritePlayer *sprite, int x, int y) {
	assert(sprite);
	unsigned int width = sprite->getCurrentWidth();
	unsigned int height = sprite->getCurrentHeight();
	blit(sprite->getCurrentData(), x - width/2 + sprite->getXPos(), y - height + sprite->getYPos(), width, height);
	if (sprite->speaking()) {
		// XXX: this doesn't work properly, SpritePlayer side probably needs work too
		unsigned int m_width = sprite->getSpeechWidth();
		unsigned int m_height = sprite->getSpeechHeight();
		blit(sprite->getSpeechData(),
			x - m_width/2 + sprite->getXPos() + sprite->getMouthXPos(),
			y - height + m_height + sprite->getYPos() + sprite->getMouthYPos(), m_width, m_height);
	}
}

void Graphics::drawBackgroundPolys(Common::String filename) {
	Common::SeekableReadStream *mrgStream = _vm->openFile(filename);

	uint16 num_entries = mrgStream->readUint16LE();
	Common::Array<uint32> offsets;
	for (unsigned int i = 0; i < num_entries; i++) {
		uint32 id = mrgStream->readUint32LE();
		uint32 offset = mrgStream->readUint32LE();
		offsets.push_back(offset);
	}

	for (unsigned int i = 0; i < num_entries; i++) {
		Common::Array<Common::Point> points;
		Common::Array<uint16> distances;
		assert(mrgStream->seek(offsets[i]));
		byte something = mrgStream->readByte();
		assert(something == 0 || something == 1 || something == 3 || something == 4);
		uint16 something2 = mrgStream->readUint16LE();
		assert(something2 == 0);
		byte count = mrgStream->readByte();
		for (unsigned int j = 0; j < count; j++) {
			uint16 x = mrgStream->readUint16LE();
			uint16 y = mrgStream->readUint16LE();
			// 0-256, higher is nearer (larger characters);
			// (maybe 0 means not shown at all?)
			uint16 distance = mrgStream->readUint16LE();
			assert(distance <= 0x100);
			distances.push_back(distance);
			points.push_back(Common::Point(x, y));
		}
		// something == 0: not walkable by default? (i.e. script-enabled)
		// something == 3: ??
		// something == 4: seems to usually cover almost the whole screen..
		byte colour = 160 + distances[0]/36;
		if (something == 4) colour = 254; // green
		else if (something == 0) colour = 225; // brown
		else if (something == 3) colour = 224; // grayish
		renderPolygonEdge(points, colour);
	}
	delete mrgStream;
}

void Graphics::renderPolygonEdge(Common::Array<Common::Point> &points, byte colour) {
	::Graphics::Surface *surf = _vm->_system->lockScreen();
	for (unsigned int i = 0; i < points.size(); i++) {
		if (i + 1 < points.size())
			surf->drawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, colour);
		else
			surf->drawLine(points[i].x, points[i].y, points[0].x, points[0].y, colour);
	}
	_vm->_system->unlockScreen();
}

} // Unity

