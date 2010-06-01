#include "graphics.h"
#include "sprite.h"
#include "common/system.h"
#include "graphics/surface.h"
#include "graphics/cursorman.h"

namespace Unity {

Graphics::Graphics(UnityEngine *_engine) : _vm(_engine) {
	basePalette = palette = 0;
	background.data = 0;
}

Graphics::~Graphics() {
	delete[] basePalette;
	delete[] palette;
	for (unsigned int i = 0; i < cursors.size(); i++)
		delete[] cursors[i].data;
	for (unsigned int i = 0; i < wait_cursors.size(); i++)
		delete[] wait_cursors[i].data;
	delete[] background.data;
}

void Graphics::init() {
	loadPalette();
	loadCursors();
}

void Graphics::loadPalette() {
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

void Graphics::loadCursors() {
	Common::SeekableReadStream *stream = _vm->openFile("cursor.dat");
	while (stream->pos() != stream->size()) {
		Image img;
		img.width = stream->readUint16LE();
		img.height = stream->readUint16LE();
		img.data = new byte[img.width * img.height];
		stream->read(img.data, img.width * img.height);
		cursors.push_back(img);
	}
	delete stream;
	stream = _vm->openFile("waitcurs.dat");
	while (stream->pos() != stream->size()) {
		Image img;
		img.width = stream->readUint16LE();
		img.height = stream->readUint16LE();
		img.data = new byte[img.width * img.height];
		stream->read(img.data, img.width * img.height);
		wait_cursors.push_back(img);
	}
	delete stream;
}

void Graphics::setCursor(unsigned int id, bool wait) {
	Image *img;
	if (wait) {
		assert(id < wait_cursors.size());
		img = &wait_cursors[id];
	} else {
		assert(id < cursors.size());
		img = &cursors[id];
	}
	// TODO: hotspot?
	CursorMan.replaceCursor(img->data, img->width, img->height, img->width/2, img->height/2, COLOUR_BLANK);
	CursorMan.showMouse(true);
}

void Graphics::drawMRG(Common::String filename, unsigned int entry) {
	Common::SeekableReadStream *mrgStream = _vm->openFile(filename);

	uint16 num_entries = mrgStream->readUint16LE();
	assert(entry < num_entries);
	bool r;
	r = mrgStream->seek(entry * 4, SEEK_CUR);
	assert(r);
	uint32 offset = mrgStream->readUint32LE();
	r = mrgStream->seek(offset, SEEK_SET);
	assert(r);

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
	background.width = 640;
	background.height = 400;

	delete[] background.data;
	background.data = new byte[background.width * background.height];
	scrStream->read(background.data, background.width * background.height);
	delete scrStream;
}

void Graphics::drawBackgroundImage() {
	assert(background.data);

	_vm->_system->copyRectToScreen(background.data, background.width, 0, 0, background.width, background.height);
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
	if (x + width > surf->w) usewidth = surf->w - x;
	if (y + height > surf->h) useheight = surf->h - y;
	for (unsigned int xpos = startx; xpos < usewidth; xpos++) {
		for (unsigned int ypos = starty; ypos < useheight; ypos++) {
			byte pixel = data[xpos + ypos*width];
			if (pixel != transparent)
				*((byte *)surf->getBasePtr(x + (int)xpos, y + (int)ypos)) = pixel;
		}
	}

	// XXX: remove this, or make it an option?
	surf->drawLine(x, y, x + width, y, 1);
	surf->drawLine(x, y + height, x + width, y + height, 1);
	surf->drawLine(x, y, x, y + height, 1);
	surf->drawLine(x + width, y, x + width, y + height, 1);

	_vm->_system->unlockScreen();
}

// TODO: replace this with something that doesn't suck :)
static void hackyImageScale(byte *src, unsigned int width, unsigned int height,
	byte *dest, unsigned int destwidth, unsigned int destheight) {
	for (unsigned int x = 0; x < destwidth; x++) {
		for (unsigned int y = 0; y < destheight; y++) {
			unsigned int srcpixel = ((y * height) / destheight) * width + (x * width) / destwidth;
			dest[y * destwidth + x] = src[srcpixel];
		}
	}
}

void Graphics::drawSprite(SpritePlayer *sprite, int x, int y, unsigned int scale) {
	assert(sprite);
	byte *data = sprite->getCurrentData();
	unsigned int width = sprite->getCurrentWidth();
	unsigned int height = sprite->getCurrentHeight();
	//int xadjust = sprite->getXAdjust(), yadjust = sprite->getYAdjust();

	if (scale < 256) {
		unsigned int newwidth = (width * scale) / 256;
		unsigned int newheight = (height * scale) / 256;
		// TODO: alloca probably isn't too portable
		byte *tempbuf = (byte *)alloca(newwidth * newheight);
		hackyImageScale(data, width, height, tempbuf, newwidth, newheight);
		width = newwidth;
		height = newheight;
		data = tempbuf;
	}

	blit(data, x - width/2 + sprite->getXAdjust(), y - height + sprite->getYAdjust(), width, height);
	if (sprite->speaking()) {
		// XXX: this doesn't work properly, SpritePlayer side probably needs work too
		// XXX: scaling
		unsigned int m_width = sprite->getSpeechWidth();
		unsigned int m_height = sprite->getSpeechHeight();
		blit(sprite->getSpeechData(),
			x - m_width/2 + sprite->getXAdjust() + sprite->getMouthXPos(),
			y - height + m_height + sprite->getYAdjust() + sprite->getMouthYPos(), m_width, m_height);
	}
}

void Graphics::drawBackgroundPolys(Common::Array<ScreenPolygon> &polys) {
	for (unsigned int i = 0; i < polys.size(); i++) {
		ScreenPolygon &poly = polys[i];
		// something == 0: not walkable by default? (i.e. script-enabled)
		// something == 3: ??
		// something == 4: seems to usually cover almost the whole screen..
		byte colour = 160 + poly.distances[0]/36;
		if (poly.type == 4) colour = 254; // green
		else if (poly.type == 0) colour = 225; // brown
		else if (poly.type == 3) colour = 224; // grayish
		renderPolygonEdge(poly.points, colour);
	}
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

