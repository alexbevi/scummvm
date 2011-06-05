/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "graphics.h"
#include "sprite_player.h"
#include "common/system.h"
#include "common/events.h"
#include "common/textconsole.h"
#include "engines/util.h" // initGraphics
#include "graphics/font.h"
#include "graphics/surface.h"
#include "graphics/palette.h"
#include "graphics/cursorman.h"
#include "fvf_decoder.h"
#include "video/qt_decoder.h"

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
	_basePalette = 0;
	_palette = 0;
	_background.data = 0;
}

Graphics::~Graphics() {
	delete[] _basePalette;
	delete[] _palette;

	for (unsigned int i = 0; i < _cursors.size(); i++)
		delete[] _cursors[i].data;
	for (unsigned int i = 0; i < _waitCursors.size(); i++)
		delete[] _waitCursors[i].data;
	delete[] _background.data;

	for (uint i = 0; i < _fonts.size(); i++)
		delete _fonts[i];
}

void Graphics::init() {
	loadPalette();
	loadCursors();
	loadFonts();
}

void Graphics::loadPalette() {
	delete[] _basePalette;
	// read the standard palette entries
	_basePalette = new byte[128 * 3];
	Common::SeekableReadStream *palStream = _vm->data.openFile("STANDARD.PAL");
	for (uint16 i = 0; i < 128; i++) {
		_basePalette[i * 3] = palStream->readByte();
		_basePalette[i * 3 + 1] = palStream->readByte();
		_basePalette[i * 3 + 2] = palStream->readByte();
	}
	delete palStream;
}

void Graphics::loadCursors() {
	for (unsigned int i = 0; i < _cursors.size(); i++)
		delete[] _cursors[i].data;
	_cursors.clear();

	Common::SeekableReadStream *stream = _vm->data.openFile("cursor.dat");
	while (stream->pos() != stream->size()) {
		Image img;
		img.width = stream->readUint16LE();
		img.height = stream->readUint16LE();
		img.data = new byte[img.width * img.height];
		stream->read(img.data, img.width * img.height);
		_cursors.push_back(img);
	}
	delete stream;

	for (unsigned int i = 0; i < _waitCursors.size(); i++)
		delete[] _waitCursors[i].data;
	_waitCursors.clear();
	stream = _vm->data.openFile("waitcurs.dat");
	// TODO: waitcursor stream has an extra byte on the end, so needs +1...
	while (stream->pos() + 1 != stream->size()) {
		Image img;
		img.width = stream->readUint16LE();
		img.height = stream->readUint16LE();
		img.data = new byte[img.width * img.height];
		stream->read(img.data, img.width * img.height);
		_waitCursors.push_back(img);
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
		assert(id < _waitCursors.size());
		img = &_waitCursors[id];
	} else {
		assert(id < _cursors.size());
		img = &_cursors[id];
	}
	// TODO: hotspot?
	CursorMan.replaceCursor(img->data, img->width, img->height, img->width/2, img->height/2, COLOUR_BLANK);
	CursorMan.showMouse(true);
}

class UnityFont : public ::Graphics::Font {
protected:
	byte _start, _end;
	uint16 _size;
	byte _glyphPitch, _glyphHeight;
	byte *_data, *_widths;

public:
	UnityFont(Common::SeekableReadStream *fontStream) {
		byte unknown = fontStream->readByte();
		assert(unknown == 1);

		_glyphHeight = fontStream->readByte();
		_start = fontStream->readByte();
		_end = fontStream->readByte();
		_size = fontStream->readUint16LE() - 1;

		byte unknown2 = fontStream->readByte();
		assert(unknown2 == 0);

		byte unknown3 = fontStream->readByte();
		assert(unknown3 == 0 || unknown3 == 1);
		if (unknown3 == 0) {
			// TODO: not sure what's going on with these
			_glyphPitch = 0;
			_data = NULL;
			_widths = NULL;
			return;
		}

		_glyphPitch = fontStream->readByte();
		assert(_size == _glyphPitch * ((unknown3 == 0) ? 1 : _glyphHeight));

		uint num_glyphs = _end - _start + 1;
		_data = new byte[num_glyphs * _size];
		_widths = new byte[num_glyphs];

		for (unsigned int i = 0; i < num_glyphs; i++) {
			_widths[i] = fontStream->readByte();
			fontStream->read(_data + (i * _size), _size);
		}
		// (TODO: sometimes files have exactly one more char on the end?)
	}
	virtual ~UnityFont() {
		delete[] _data;
		delete[] _widths;
	}

	virtual int getFontHeight() const { return _glyphHeight; }
	virtual int getMaxCharWidth() const { return _glyphPitch; }
	virtual int getCharWidth(byte chr) const {
		if (chr < _start || chr > _end)
			return 0;
		return _widths[chr - _start];
	}

	virtual void drawChar(::Graphics::Surface *dst, byte chr, int x, int y, uint32 /*color*/) const {
		assert(dst->format.bytesPerPixel == 1);

		if (chr < _start || chr > _end) {
			warning("can't render character %x: not between %x and %x",
				chr, _start, _end);
			chr = ' ';
		}

		chr -= _start;
		byte *data = _data + (chr * _size);
		for (uint line = 0; line < _glyphHeight; line++) {
			memcpy(dst->getBasePtr(x, y + line), data, _widths[chr]);
			data += _glyphPitch;
		}
	}
};

::Graphics::Font *Graphics::getFont(unsigned int id) const {
	return _fonts[id];
}

void Graphics::loadFonts() {
	for (unsigned int num = 0; num < 10; num++) {
		Common::String filename;
		filename = Common::String::format("font%d.fon", num);
		Common::SeekableReadStream *fontStream = _vm->data.openFile(filename.c_str());
		_fonts.push_back(new UnityFont(fontStream));
		delete fontStream;
	}
}

uint Graphics::getFontHeight(uint font) const {
	return _fonts[font]->getFontHeight();
}

uint Graphics::getStringWidth(const Common::String &text, uint font) const {
	return _fonts[font]->getStringWidth(text);
}

void Graphics::drawString(uint x, uint y, const Common::String &text, uint font) const {
	::Graphics::Surface *surf = _vm->_system->lockScreen();
	_fonts[font]->drawString(surf, text, x, y, 9999, 0);
	_vm->_system->unlockScreen();
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

	blit(mrg->data[entry], x, y, mrg->widths[entry], mrg->heights[entry], COLOUR_BLANK);
}

void Graphics::setBackgroundImage(Common::String filename) {
	delete[] _palette;
	_palette = new byte[256 * 3];

	Common::SeekableReadStream *scrStream = _vm->data.openFile(filename);
	for (uint16 i = 0; i < 128; i++) {
		_palette[i * 3] = scrStream->readByte();
		_palette[i * 3 + 1] = scrStream->readByte();
		_palette[i * 3 + 2] = scrStream->readByte();
	}
	memcpy(_palette + 128*3, _basePalette, 128*3);

	for (uint16 i = 0; i < 256; i++) {
		for (byte j = 0; j < 3; j++) {
			_palette[i * 3 + j] = _palette[i * 3 + j] << 2;
		}
	}

	_vm->_system->getPaletteManager()->setPalette(_palette, 0, 256);

	// some of the files seem to be 480 high, but just padded with black
	_background.width = 640;
	_background.height = 480;

	delete[] _background.data;
	_background.data = new byte[_background.width * _background.height];
	scrStream->read(_background.data, _background.width * _background.height);
	delete scrStream;
}

void Graphics::drawBackgroundImage() {
	assert(_background.data);

	_vm->_system->copyRectToScreen(_background.data, _background.width, 0, 0, _background.width, _background.height);
}

// XXX: default transparent is a hack (see header file)
void Graphics::blit(byte *data, int x, int y, unsigned int width, unsigned int height, byte transparent) {
	// TODO: work with internal buffer and dirty-rectangling?
	::Graphics::Surface *surf = _vm->_system->lockScreen();
	// XXX: this code hasn't had any thought
	int startx = 0, starty = 0;
	if (x < 0)
		startx = -x;
	if (y < 0)
		starty = -y;
	unsigned int usewidth = width, useheight = height;
	if (x + (int)width > (int)surf->w)
		usewidth = surf->w - x;
	if (y + (int)height > (int)surf->h)
		useheight = surf->h - y;
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
		debug("new sprite-embedded palette");

		delete[] _palette;
		_palette = new byte[256 * 3];

		for (uint16 i = 0; i < 256; i++) {
			_palette[i * 3] = *(newpal++) * 4;
			_palette[i * 3 + 1] = *(newpal++) * 4;
			_palette[i * 3 + 2] = *(newpal++) * 4;
		}

		_vm->_system->getPaletteManager()->setPalette(_palette, 0, 256);
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
	//targetx -= (sprite->getXAdjust()*(int)scale)/256;
	unsigned int targety = y;
	if (sprite->getYPos() != 0) targety = sprite->getYPos();
	//targety -= (sprite->getYAdjust()*(int)scale)/256;

	//printf("target x %d, y %d, adjustx %d, adjusty %d\n", sprite->getXPos(), sprite->getYPos(),
	//	sprite->getXAdjust(), sprite->getYAdjust());
	blit(data,
		(int)targetx - ((-sprite->getXAdjust() + (int)width/2)*(int)scale)/256,
		(int)targety - ((-sprite->getYAdjust() + (int)height)*(int)scale)/256,
		bufwidth,
		bufheight);

	if (sprite->speaking()) {
		// XXX: this doesn't work properly, SpritePlayer side probably needs work too
		data = sprite->getSpeechData();
		unsigned int m_width = sprite->getSpeechWidth();
		unsigned int m_height = sprite->getSpeechHeight();
		bufwidth = m_width;
		bufheight = m_height;

		if (scale < 256) {
			bufwidth = (m_width * scale) / 256;
			bufheight = (m_height * scale) / 256;
			// TODO: alloca probably isn't too portable
			byte *tempbuf = (byte *)alloca(bufwidth * bufheight);
			hackyImageScale(data, m_width, m_height, tempbuf, bufwidth, bufheight);
			data = tempbuf;
		}

		// XXX: what's the sane behaviour here?
		targetx = x;
		if (sprite->getSpeechXPos() != 0) targetx = sprite->getSpeechXPos();
		targety = y;
		if (sprite->getSpeechYPos() != 0) targety = sprite->getSpeechYPos();

		//printf("speech target x %d, y %d, adjustx %d, adjusty %d\n",
		//	sprite->getSpeechXPos(), sprite->getSpeechYPos(),
		//	sprite->getSpeechXAdjust(), sprite->getSpeechYAdjust());
		blit(data,
			(int)targetx - ((-sprite->getSpeechXAdjust() + (int)m_width/2)*(int)scale)/256,
			(int)targety - ((-sprite->getSpeechYAdjust() + (int)m_height)*(int)scale)/256,
			bufwidth,
			bufheight);
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
		if (poly.type == 4)
			colour = 254; // green
		else if (poly.type == 0)
			colour = 225; // brown
		else if (poly.type == 3)
			colour = 224; // grayish
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
	Common::SeekableReadStream *intro_movie;
	::Video::VideoDecoder *videoDecoder;
	if (SearchMan.hasFile(filename)) {
		intro_movie = _vm->data.openFile(filename);
		videoDecoder = new FVFDecoder(g_system->getMixer());
	} else {
		assert(filename.size() > 3);
		filename = Common::String(filename.c_str(), filename.size() - 3) + "mov";
		intro_movie = _vm->data.openFile(filename);
		videoDecoder = new ::Video::QuickTimeDecoder();
		// try and grab a 16bpp mode before the cinepak codec decides that it should produce 8bpp
		::Graphics::PixelFormat hackFormat = ::Graphics::PixelFormat(2, 5, 5, 5, 0, 10, 5, 0, 0);
		g_system->beginGFXTransaction();
		g_system->initSize(640, 400, &hackFormat);
		g_system->endGFXTransaction();
	}

	if (!videoDecoder->loadStream(intro_movie))
		error("failed to load movie %s", filename.c_str());

	bool skipVideo = false;

	::Graphics::PixelFormat format = videoDecoder->getPixelFormat();
	unsigned int vidwidth = videoDecoder->getWidth();
	unsigned int vidheight = videoDecoder->getHeight();
	unsigned int vidbpp = format.bytesPerPixel;

	initGraphics(vidwidth, vidheight, false, &format);
	assert(format.bytesPerPixel != 1);
	g_system->showMouse(false);

	while (!g_engine->shouldQuit() && !videoDecoder->endOfVideo() && !skipVideo) {
		if (videoDecoder->needsUpdate()) {
			const ::Graphics::Surface *frame = videoDecoder->decodeNextFrame();
			if (frame) {
				g_system->copyRectToScreen((byte *)frame->pixels, vidwidth*vidbpp,
					0, 0, vidwidth, vidheight);

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

	initGraphics(640, 480, true);
	g_system->showMouse(true);
	if (_palette) {
		_vm->_system->getPaletteManager()->setPalette(_palette, 0, 256);
	}

	delete videoDecoder;
}

} // Unity

