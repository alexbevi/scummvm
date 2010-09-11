#include "graphics.h"
#include "sprite.h"
#include "common/system.h"
#include "graphics/surface.h"
#include "graphics/cursorman.h"
#include "fvf_decoder.h"

namespace Unity {

/**
 * A black and white SCI-style arrow cursor (11x16).
 * 3 = Transparent.
 * 0 = Black (#000000 in 24-bit RGB).
 * 1 = White (#FFFFFF in 24-bit RGB).
 */
static const byte sciMouseCursor[] = {
	0,0,3,3,3,3,3,3,3,3,3,
	0,1,0,3,3,3,3,3,3,3,3,
	0,1,1,0,3,3,3,3,3,3,3,
	0,1,1,1,0,3,3,3,3,3,3,
	0,1,1,1,1,0,3,3,3,3,3,
	0,1,1,1,1,1,0,3,3,3,3,
	0,1,1,1,1,1,1,0,3,3,3,
	0,1,1,1,1,1,1,1,0,3,3,
	0,1,1,1,1,1,1,1,1,0,3,
	0,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,0,3,3,3,3,
	0,1,0,3,0,1,1,0,3,3,3,
	0,0,3,3,0,1,1,0,3,3,3,
	3,3,3,3,3,0,1,1,0,3,3,
	3,3,3,3,3,0,1,1,0,3,3,
	3,3,3,3,3,3,0,1,1,0,3
};

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
	if (id == 0xffffffff) {
		// XXX: this mouse cursor is borrowed from SCI
		CursorMan.replaceCursor(sciMouseCursor, 11, 16, 1, 1, 3);
		CursorMan.showMouse(true);
		return;
	}

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

void Graphics::calculateStringMaxBoundary(unsigned int &width, unsigned int &height,
	Common::String str, unsigned int font) {
	Common::Array<unsigned int> strwidths, starts;
	calculateStringBoundary(320, strwidths, starts, height, str, font);
	for (unsigned int i = 0; i < strwidths.size(); i++) {
		if (strwidths[i] > width)
			width = strwidths[i];
	}
}

#define FONT_VERT_SPACING 8

void Graphics::calculateStringBoundary(unsigned int maxwidth, Common::Array<unsigned int> &widths,
	Common::Array<unsigned int> &starts, unsigned int &height,
	const Common::String text, unsigned int font) {
	Font &f = fonts[font];
	assert(f.data);

	starts.push_back(0);

	unsigned int currx = 0, curry = 0;
	unsigned int last_good_char = 0, last_good_x = 0;

	for (unsigned int i = 0; i < text.size(); i++) {
		if (text[i] == '\n') {
			// original engine probably doesn't handle newlines, but this is convenient
			widths.push_back(currx);
			starts.push_back(i + 1);

			last_good_char = i + 1;
			currx = last_good_x = 0;
			curry += f.glyphheight + FONT_VERT_SPACING;
			continue;
		}

		unsigned char c = text[i];
		if (c < f.start || c > f.end) {
			printf("WARNING: can't render character %x in font %d: not between %x and %x\n",
				c, font, f.start, f.end);
			continue;
		}
		c -= f.start;

		if (currx + f.widths[c] > maxwidth) {
			if (text[i] == ' ') {
				// we can skip this space..

				// end of text?
				if (i + 1 == text.size())
					continue;

				widths.push_back(currx);
				starts.push_back(i + 1);
			} else {
				// backtrack to last line split
				assert(last_good_char > 0);
				assert(last_good_x != 0);

				widths.push_back(last_good_x);
				starts.push_back(last_good_char);

				i = last_good_char - 1;
			}

			// reset
			last_good_char = i + 1;
			currx = last_good_x = 0;
			curry += f.glyphheight + FONT_VERT_SPACING;

			continue;
		}

		if (text[i] == ' ') {
			last_good_char = i + 1; // TODO: cope with multiple spaces?
			last_good_x = currx;
		}

		currx += f.widths[c];
	}

	height = curry;
	if (last_good_x != currx) {
		height += f.glyphheight;
	}

	widths.push_back(currx);
}

void Graphics::drawString(unsigned int x, unsigned int y, unsigned int width, unsigned int maxheight,
	Common::String text, unsigned int font) {
	Font &f = fonts[font];
	assert(f.data);

	unsigned int height; // unused
	Common::Array<unsigned int> widths, starts;
	calculateStringBoundary(width, widths, starts, height, text, font);

	unsigned int currx = x;
	unsigned int curry = y;

	unsigned int j = 0;
	for (unsigned int i = 0; i < text.size(); i++) {
		if (j + 1 < starts.size() && i == starts[j + 1]) {
			// next line
			j++;
			i = starts[j];

			currx = x;
			curry += f.glyphheight + FONT_VERT_SPACING;
		}
		if (curry + f.glyphheight - y > maxheight) return;

		assert(j + 1 == starts.size() || i < starts[j + 1]);

		if (text[i] == '\n') continue;

		unsigned char c = text[i];
		if (c < f.start || c > f.end) {
			printf("WARNING: can't render character %x in font %d: not between %x and %x\n",
				c, font, f.start, f.end);
			continue;
		}
		c -= f.start;

		// maybe we don't need to render anything..
		if (text[i] == ' ') {
			// end of text?
			if (i + 1 == text.size())
				continue;
			// next char is on a new line?
			if (j + 1 < starts.size() && i + 1 == starts[j + 1])
				continue;
		}

		assert (currx + f.widths[c] <= x + widths[j]);
		assert (currx + f.widths[c] <= x + width);

		// TODO: clipping
		_vm->_system->copyRectToScreen(f.data + (c * f.size),
			f.glyphpitch, currx, curry, f.widths[c],
			f.glyphheight);
		// TODO: clipping
		currx += f.widths[c];
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
	targetx -= (sprite->getXAdjust()*(int)scale)/256;
	unsigned int targety = y;
	if (sprite->getYPos() != 0) targety = sprite->getYPos();
	targety -= (sprite->getYAdjust()*(int)scale)/256;

	//printf("target x %d, y %d, adjustx %d, adjusty %d\n", sprite->getXPos(), sprite->getYPos(),
	//	sprite->getXAdjust(), sprite->getYAdjust());
	blit(data, targetx - ((width/2)*(int)scale)/256, targety - ((height)*(int)scale)/256, bufwidth, bufheight);

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
	/*::Graphics::Surface *surf = _vm->_system->lockScreen();
	if (targetx != 0) *((byte *)surf->getBasePtr(targetx-1, targety)) = 254;
	if (targetx != 640-1 ) *((byte *)surf->getBasePtr(targetx+1, targety)) = 254;
	*((byte *)surf->getBasePtr(targetx, targety)) = 254;
	if (targety != 0) *((byte *)surf->getBasePtr(targetx, targety-1)) = 254;
	if (targety != 480-1) *((byte *)surf->getBasePtr(targetx, targety+1)) = 254;
	_vm->_system->unlockScreen();*/
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

void Graphics::playMovie(Common::String filename) {
	Common::SeekableReadStream *intro_movie = _vm->data.openFile(filename);

	::Graphics::VideoDecoder *videoDecoder = new FVFDecoder(g_system->getMixer());
	if (!videoDecoder->load(intro_movie)) error("failed to load movie %s", filename.c_str());

	bool skipVideo = false;

	::Graphics::PixelFormat format = videoDecoder->getPixelFormat();
	unsigned int vidwidth = videoDecoder->getWidth();
	unsigned int vidheight = videoDecoder->getHeight();
	unsigned int vidbpp = format.bytesPerPixel;

	g_system->beginGFXTransaction();
	g_system->initSize(vidwidth*2, vidheight*2, &format);
	g_system->endGFXTransaction();

	byte *vidpixels = (byte *)alloca(vidwidth*2 * vidheight*2 * vidbpp);

	g_system->showMouse(false);

	while (!g_engine->shouldQuit() && !videoDecoder->endOfVideo() && !skipVideo) {
		if (videoDecoder->needsUpdate()) {
			::Graphics::Surface *frame = videoDecoder->decodeNextFrame();
			if (frame) {
				// TODO: this is some very slow 2x scaling
				unsigned int x = 0, y = 0;

				byte *in = (byte *)frame->pixels, *out = vidpixels;
				for (unsigned int i = 0; i < vidheight; i++) {
					for (unsigned int j = 0; j < vidwidth; j++) {
						// first pixel, on two lines
						memcpy(out, in, vidbpp);
						memcpy(out + vidwidth*2*vidbpp, in, vidbpp);
						out += vidbpp;
						// second pixel, on two lines
						memcpy(out, in, vidbpp);
						memcpy(out + vidwidth*2*vidbpp, in, vidbpp);
						out += vidbpp;

						in += vidbpp;
					}
					// every other line already handled
					out += vidwidth * 2 * vidbpp;
				}

				g_system->copyRectToScreen(vidpixels, vidwidth*2*vidbpp,
					x, y, vidwidth*2, vidheight*2);

				g_system->updateScreen();
			}
		}

		Common::Event event;
		while (g_system->getEventManager()->pollEvent(event)) {
			if (event.type == Common::EVENT_KEYDOWN && event.kbd.keycode == Common::KEYCODE_ESCAPE) {
				skipVideo = true;
			}
		}

		if (!videoDecoder->needsUpdate())
			g_system->delayMillis(10);
	}

	g_system->beginGFXTransaction();
	g_system->initSize(640, 480);
	g_system->endGFXTransaction();
	g_system->showMouse(true);
	if (palette) {
		_vm->_system->setPalette(palette, 0, 256);
	}

	delete videoDecoder;
}

} // Unity

