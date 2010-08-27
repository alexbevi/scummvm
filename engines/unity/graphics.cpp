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
	loadFonts();
}

void Graphics::loadPalette() {
	// read the standard palette entries
	basePalette = new byte[128 * 4];
	Common::SeekableReadStream *palStream = _vm->data.openFile("STANDARD.PAL");
	for (uint16 i = 0; i < 128; i++) {
		basePalette[i * 4] = palStream->readByte();
		basePalette[i * 4 + 1] = palStream->readByte();
		basePalette[i * 4 + 2] = palStream->readByte();
		basePalette[i * 4 + 3] = 0;
	}
	delete palStream;
}

void Graphics::loadCursors() {
	Common::SeekableReadStream *stream = _vm->data.openFile("cursor.dat");
	while (stream->pos() != stream->size()) {
		Image img;
		img.width = stream->readUint16LE();
		img.height = stream->readUint16LE();
		img.data = new byte[img.width * img.height];
		stream->read(img.data, img.width * img.height);
		cursors.push_back(img);
	}
	delete stream;
	stream = _vm->data.openFile("waitcurs.dat");
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

struct Font {
	byte start, end;
	uint16 size;
	byte glyphpitch, glyphheight;
	byte *data, *widths;
};

Font fonts[10]; // TODO: store in class, free data

void Graphics::loadFonts() {
	for (unsigned int num = 0; num < 10; num++) {
		char filename[10];
		snprintf(filename, 10, "font%d.fon", num);
		Common::SeekableReadStream *fontStream = _vm->data.openFile(filename);

		byte unknown = fontStream->readByte();
		assert(unknown == 1);

		fonts[num].glyphheight = fontStream->readByte();
		fonts[num].start = fontStream->readByte();
		fonts[num].end = fontStream->readByte();
		fonts[num].size = fontStream->readUint16LE() - 1;

		unsigned int num_glyphs = fonts[num].end - fonts[num].start + 1;
		uint16 size = fonts[num].size;

		byte unknown2 = fontStream->readByte();
		assert(unknown2 == 0);

		byte unknown3 = fontStream->readByte();
		assert(unknown3 == 0 || unknown3 == 1);
		if (unknown3 == 0) {
			// TODO: not sure what's going on with these
			fonts[num].data = NULL;
			continue;
		}

		fonts[num].glyphpitch = fontStream->readByte();
		assert(size == fonts[num].glyphpitch * ((unknown3 == 0) ? 1 : fonts[num].glyphheight));

		fonts[num].data = new byte[num_glyphs * size];
		fonts[num].widths = new byte[num_glyphs];

		for (unsigned int i = 0; i < num_glyphs; i++) {
			fonts[num].widths[i] = fontStream->readByte();
			fontStream->read(fonts[num].data + (i * size), size);
		}
		// (TODO: sometimes files have exactly one more char on the end?)

		delete fontStream;
	}
}

void Graphics::drawString(unsigned int x, unsigned int y, unsigned int width, unsigned int height, Common::String text, unsigned int font) {
	assert(fonts[font].data);

	unsigned int currx = x;
	unsigned int curry = y;

	for (unsigned int i = 0; i < text.size(); i++) {
		unsigned char c = text[i];
		if (c < fonts[font].start || c > fonts[font].end) {
			printf("WARNING: can't render character %x in font %d: not between %x and %x\n",
				c, font, fonts[font].start, fonts[font].end);
			continue;
		}
		c -= fonts[font].start;

		if (currx + fonts[font].widths[c] > x + width) {
			currx = x;
			curry += fonts[font].glyphheight + 4;
		}

		// TODO: clipping
		_vm->_system->copyRectToScreen(fonts[font].data + (c * fonts[font].size),
			fonts[font].glyphpitch, currx, curry, fonts[font].widths[c],
			fonts[font].glyphheight);
		// TODO: clipping
		currx += fonts[font].widths[c];
	}
}

void Graphics::loadMRG(Common::String filename, MRGFile *mrg) {
	Common::SeekableReadStream *mrgStream = _vm->data.openFile(filename);
	uint16 num_entries = mrgStream->readUint16LE();

	Common::Array<uint32> offsets;
	for (unsigned int i = 0; i < num_entries; i++) {
		offsets.push_back(mrgStream->readUint32LE());
	}

	for (unsigned int i = 0; i < num_entries; i++) {
		bool r = mrgStream->seek(offsets[i], SEEK_SET);
		assert(r);

		uint16 width = mrgStream->readUint16LE();
		uint16 height = mrgStream->readUint16LE();
		byte *pixels = new byte[width * height];
		mrgStream->read(pixels, width * height);

		mrg->heights.push_back(height);
		mrg->widths.push_back(width);
		mrg->data.push_back(pixels);
	}

	delete mrgStream;
}

void Graphics::drawMRG(MRGFile *mrg, unsigned int entry, unsigned int x, unsigned int y) {
	assert(entry < mrg->data.size());

	// TODO: positioning/clipping?
	_vm->_system->copyRectToScreen(mrg->data[entry], mrg->widths[entry], x, y,
		mrg->widths[entry], mrg->heights[entry]);
}

void Graphics::setBackgroundImage(Common::String filename) {
	delete[] palette;
	palette = new byte[256 * 4];

	Common::SeekableReadStream *scrStream = _vm->data.openFile(filename);
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
	background.height = 480;

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
	/*surf->drawLine(x, y, x + width, y, 1);
	surf->drawLine(x, y + height, x + width, y + height, 1);
	surf->drawLine(x, y, x, y + height, 1);
	surf->drawLine(x + width, y, x + width, y + height, 1);*/

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
	unsigned int bufwidth = width, bufheight = height;

	byte *newpal = sprite->getPalette();
	if (newpal) {
		// new palette; used for things like the intro animation
		printf("new sprite-embedded palette\n");

		delete[] palette;
		palette = new byte[256 * 4];

		for (uint16 i = 0; i < 256; i++) {
			palette[i * 4] = *(newpal++) * 4;
			palette[i * 4 + 1] = *(newpal++) * 4;
			palette[i * 4 + 2] = *(newpal++) * 4;
			palette[i * 4 + 3] = 0;
		}

		_vm->_system->setPalette(palette, 0, 256);
	}

	if (scale < 256) {
		bufwidth = (width * scale) / 256;
		bufheight = (height * scale) / 256;
		// TODO: alloca probably isn't too portable
		byte *tempbuf = (byte *)alloca(bufwidth * bufheight);
		hackyImageScale(data, width, height, tempbuf, bufwidth, bufheight);
		data = tempbuf;
	}

	// XXX: what's the sane behaviour here?
	unsigned int targetx = x;
	if (sprite->getXPos() != 0) targetx = sprite->getXPos();
	unsigned int targety = y;
	if (sprite->getYPos() != 0) targety = sprite->getYPos();

	//printf("target x %d, y %d, adjustx %d, adjusty %d\n", sprite->getXPos(), sprite->getYPos(),
	//	sprite->getXAdjust(), sprite->getYAdjust());
	blit(data, targetx - ((width/2 + sprite->getXAdjust())*(int)scale)/256, targety - ((height + sprite->getYAdjust())*(int)scale)/256, bufwidth, bufheight);
	if (sprite->speaking()) {
		// XXX: this doesn't work properly, SpritePlayer side probably needs work too
		data = sprite->getSpeechData();
		unsigned int m_width = sprite->getSpeechWidth();
		unsigned int m_height = sprite->getSpeechHeight();
		bufwidth = m_width;
		bufwidth = m_height;

		if (scale < 256) {
			bufwidth = (m_width * scale) / 256;
			bufheight = (m_height * scale) / 256;
			// TODO: alloca probably isn't too portable
			byte *tempbuf = (byte *)alloca(bufwidth * bufheight);
			hackyImageScale(data, m_width, m_height, tempbuf, bufwidth, bufheight);
			data = tempbuf;
		}

		// XXX: what's the sane behaviour here?
		if (sprite->getSpeechXPos() != 0) targetx = sprite->getSpeechXPos();
		if (sprite->getSpeechYPos() != 0) targety = sprite->getSpeechYPos();

		//printf("speech target x %d, y %d, adjustx %d, adjusty %d\n",
		//	sprite->getSpeechXPos(), sprite->getSpeechYPos(),
		//	sprite->getSpeechXAdjust(), sprite->getSpeechYAdjust());
		blit(sprite->getSpeechData(),
			targetx - ((m_width/2 + sprite->getSpeechXAdjust())*(int)scale)/256,
			targety - (((int)m_height - sprite->getSpeechYAdjust())*(int)scale)/256, m_width, m_height);
	}

	// plot cross at (x, y) loc
	::Graphics::Surface *surf = _vm->_system->lockScreen();
	if (x != 0) *((byte *)surf->getBasePtr(x-1, y)) = 254;
	if (x != 640-1 ) *((byte *)surf->getBasePtr(x+1, y)) = 254;
	*((byte *)surf->getBasePtr(x, y)) = 254;
	if (y != 0) *((byte *)surf->getBasePtr(x, y-1)) = 254;
	if (y != 480-1) *((byte *)surf->getBasePtr(x, y+1)) = 254;
	_vm->_system->unlockScreen();
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
		for (unsigned int j = 0; j < poly.triangles.size(); j++) {
			Common::Array<Common::Point> points;
			points.push_back(poly.triangles[j].points[0]);
			points.push_back(poly.triangles[j].points[1]);
			points.push_back(poly.triangles[j].points[2]);
			renderPolygonEdge(points, colour);
		}
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

void Graphics::fillRect(byte colour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
	::Graphics::Surface *surf = _vm->_system->lockScreen();
	Common::Rect r(x1, y1, x2, y2);
	surf->fillRect(r, colour);
	_vm->_system->unlockScreen();
}

} // Unity

