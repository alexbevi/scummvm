/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#include "innocent/graphics.h"

#include <algorithm>
#include <functional>

#include "common/events.h"
#include "common/system.h"
#include "common/util.h"
#include "graphics/cursorman.h"
#include "graphics/palette.h"

#include "innocent/animation.h"
#include "innocent/debug.h"
#include "innocent/debugger.h"
#include "innocent/exit.h"
#include "innocent/innocent.h"
#include "innocent/logic.h"
#include "innocent/resources.h"
#include "innocent/room.h"
#include "innocent/util.h"

using namespace std;

namespace Common {
	DECLARE_SINGLETON(Innocent::Graphics);
}

namespace Innocent {

//Common::Point &operator+=(Common::Point &p1, const Common::Point &p2) { return p1 = Common::Point(p1.x + p2.x, p1.y + p2.y); }

//Common::Point &operator-=(Common::Point &p1, const Common::Point &p2) { return p1 = Common::Point(p1.x - p2.x, p1.y - p2.y); }

void Graphics::setEngine(Engine *engine) {
	_engine = engine;
	_framebuffer.reset(new Surface);
	_framebuffer->create(320, 200, ::Graphics::PixelFormat::createFormatCLUT8());
	_willFadein = false;

	_speech = 0;
	_speechFramesLeft = 0;
	_fullscreen = false;
}

void Graphics::init() {
	_resources = _engine->resources();
	_system = _engine->_system;
	loadInterface();
}

void Graphics::paint() {
	debugC(3, kDebugLevelFlow | kDebugLevelGraphics, ">>>start paint procedure");

	paintBackdrop();
	paintInterface();
	paintAnimations();
	paintExits();
	paintSpeech();

	debugC(3, kDebugLevelGraphics, "painting paintables");
	foreach (Paintable *, _paintables)
		(*it)->paint(this);

	unless (_afterRepaintHooks.empty()) {
		debugC(3, kDebugLevelGraphics | kDebugLevelScript, "running hooks");
		foreach (CodePointer, _afterRepaintHooks)
			it->run();
		_afterRepaintHooks.clear();
	}

	debugC(3, kDebugLevelFlow | kDebugLevelGraphics, "<<<end paint procedure");
}

void Graphics::paintExits() {
	debugC(3, kDebugLevelFlow | kDebugLevelGraphics, "painting exits");
	foreach_const (Exit *, _engine->logic()->room()->exits())
		(*it)->paint(this);
}

void Graphics::loadInterface() {
	debugC(1, kDebugLevelGraphics, "loading interface");
	_interface = new Surface;
	_interface->create(320, 50, ::Graphics::PixelFormat::createFormatCLUT8());
	_resources->loadInterfaceImage(reinterpret_cast<byte *>(_interface->pixels), _interfacePalette);
}

void Graphics::prepareInterfacePalette() {
	debugC(1, kDebugLevelGraphics, "preparing interface palette");
	_system->getPaletteManager()->setPalette(_interfacePalette + 160 * 4, 160, 96);
}

void Graphics::paintInterface() {
	if (_fullscreen) return;
	debugC(3, kDebugLevelGraphics, "painting interface");
	_framebuffer->blit(_interface, Common::Rect(0, 152, 320, 200), 0);
}

void Graphics::setBackdrop(uint16 id) {
	byte palette[0x400];
	_backdrop.reset(_resources->loadBackdrop(id, palette));
	setPalette(palette, 0, 256);
	prepareInterfacePalette();
	paintBackdrop();
}

void Graphics::willFadein(FadeFlags f) {
	_willFadein = true;
	_fadeFlags = f;
	if (f & kPartialFade)
		clearPalette(160, 96);
	else
		clearPalette();
}

void Graphics::paintBackdrop() {
	// TODO cropping
	debugC(3, kDebugLevelGraphics, "painting backdrop");
	_framebuffer->blit(_backdrop.get());
}

void Graphics::paintSpeech() {
	if (!_speech) return;

	if (!_speechFramesLeft) {
		delete _speech;
		_speech = 0;
		CodePointer cb = _speechDoneCallback;
		_speechDoneCallback.reset();
		cb.run();
		return;
	}

	paintText(0, 0, 235, _speech);

	_speechFramesLeft--;
}

void Graphics::paintAnimations() {
	debugC(3, kDebugLevelGraphics, "painting animations");
	Common::List<Animation *> animations = _engine->logic()->animations();
	for (Common::List<Animation *>::iterator it = animations.begin(); it != animations.end(); ++it)
		(*it)->paint(this);
}

// it's modal anyway
static int _mOption = 0;
static Common::Rect _optionRects[10];
static uint16 _optionValues[10];

uint16 Graphics::ask(uint16 left, uint16 top, byte width, byte height, byte *string) {
	width += 2;
	height += 2;
	enum {
		kFrameTileHeight = 12,
		kFrameTileWidth = 16
	};

	Surface frame;
	frame.create(width * kFrameTileWidth, height * kFrameTileHeight+4, ::Graphics::PixelFormat::createFormatCLUT8());

	Sprite **frames = _resources->frames();

	Common::Point tile(0, 0);

	paint(frames[kFrameTopLeft], tile, &frame);
	tile.x += kFrameTileWidth;
	for (int x = 1; x < width - 1; x++) {
		paint(frames[kFrameTop], tile, &frame);
		tile.x += kFrameTileWidth;
	}
	paint(frames[kFrameTopRight], tile, &frame);

	tile.y += kFrameTileHeight;
	tile.x = 0;

	for (int y = 1; y < height - 1; y++) {
		paint(frames[kFrameLeft], tile, &frame);
		tile.x += kFrameTileWidth;
		for (int x = 1; x < width - 1; x++) {
			paint(frames[kFrameFill], tile, &frame);
			tile.x += kFrameTileWidth;
		}
		paint(frames[kFrameRight], tile, &frame);
		tile.y += kFrameTileHeight;
		tile.x = 0;
	}

	paint(frames[kFrameBottomLeft], tile, &frame);
	tile.x += kFrameTileWidth;
	for (int x = 1; x < width - 1; x++) {
		paint(frames[kFrameBottom], tile, &frame);
		tile.x += kFrameTileWidth;
	}
	paint(frames[kFrameBottomRight], tile, &frame);

	_mOption = 0;

	// TODO this should use the interpreter's built-in font
	// (but it does look nicer this way)
	paintText(10, 16, 254, string, &frame);

	_system->copyRectToScreen(reinterpret_cast<byte *>(frame.pixels), frame.pitch, left, top, width * kFrameTileWidth, height * kFrameTileHeight+4);

	bool show = true;
	while (show) {
		_system->updateScreen();
		_engine->debugger()->onFrame();
		Common::Event event;
		while (_engine->eventMan()->pollEvent(event)) {
			switch(event.type) {
			case Common::EVENT_LBUTTONUP:
				if (_mOption == 0)
					return 0xffff;
				else
					for (int i = 0; i < _mOption; i++) {
						Common::Point p = event.mouse;
						p.x -= left;
						p.y -= top;
						if (_optionRects[i].contains(p))
							return _optionValues[i];
					}
			default:
				break;
			}
		}
		_system->delayMillis(1000/60);
	}

	return 0xffff;
}

enum {
	kOptionColour = 254,
	kSelectedOptionColour = 227
};

void Graphics::paintSpeechBubbleColumn(Sprite *top, Sprite *fill, Common::Point &point, uint8 fill_tiles, Surface *dest) {
	paint(top, point, dest, kPaintPositionIsTop);
	point.y += 24;
	for (int i = 0; i < fill_tiles; i++) {
		paint(fill, point, dest, kPaintPositionIsTop);
		point.y += 6;
	}
}

Common::Rect Graphics::paintSpeechInBubble(Common::Point pos, byte colour, const byte *string, Surface *bubble) {
	uint16 left = pos.x, top = pos.y;
	debugC(1, kDebugLevelGraphics, "painting speech bubble \"%s\" at %d:%d", string, left, top);
	top += 4;

	bool pointsLeft = left < 160;
	bool pointsUp = top < 53;

	if (pointsLeft)
		left += 18;

	uint16 lines;
	Common::Rect textSize = textMetrics(string, &lines);

	Sprite * const *bubbles = _resources->bubbles();

	uint8 bubble_indices[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

	uint8 wadj = 0;
	{
		const uint16 height = (lines >= 3) ? (lines * 12 + 16) : 60;
		if (top < height)
			top = 0;
		else
			top -= height;
	}
	if (pointsLeft && !pointsUp) {
		bubble_indices[kBubbleBottomLeft] = kBubbleBottomLeftPoint;
		wadj = 4;
		left += 4;
	} else if (!pointsLeft && !pointsUp) {
		if (left >= 320)
			left = 320;
		const int bubbleWidth = textSize.width() + 69;
		if (bubbleWidth > left)
			left = 0;
		else
			left -= bubbleWidth;
		bubble_indices[kBubbleBottomRight] = kBubbleBottomRightPoint;
		wadj = 0;
	} else
		error("can't paint speech bubble pointing %s %s yet", pointsUp ? "up" : "down", pointsLeft ? "left" : "right");
	debugC(2, kDebugLevelGraphics, "painting speech bubble \"%s\" at (adjusted) %d:%d", string, left, top);

	uint8 vertical_tiles = 1;
	int16 height = lines * 12 - 42;
	if (height > 5)
		vertical_tiles += (height - 58) / 6;

	uint8 horizontal_tiles = (textSize.width() - 39) / 4;
	if (horizontal_tiles == 0)
		horizontal_tiles = 1;

	bubble->create(65 + wadj + 4 * horizontal_tiles, 54 + 6 * vertical_tiles, ::Graphics::PixelFormat::createFormatCLUT8());

	Common::Point position(wadj, 0);
	paintSpeechBubbleColumn(bubbles[bubble_indices[kBubbleTopLeft]], bubbles[bubble_indices[kBubbleLeft]], position, vertical_tiles, bubble);
	position.x -= wadj;
	paint(bubbles[bubble_indices[kBubbleBottomLeft]], position, bubble, kPaintPositionIsTop);

	position.x += 33 + wadj;
	for (int i = 0; i < horizontal_tiles; i++) {
		position.y = 0;
		paintSpeechBubbleColumn(bubbles[bubble_indices[kBubbleTop]], bubbles[bubble_indices[kBubbleFill]], position, vertical_tiles, bubble);
		paint(bubbles[bubble_indices[kBubbleBottom]], position, bubble, kPaintPositionIsTop);
		position.x += 4;
	}

	position.y = 0;
	paintSpeechBubbleColumn(bubbles[bubble_indices[kBubbleTopRight]], bubbles[bubble_indices[kBubbleRight]], position, vertical_tiles, bubble);
	paint(bubbles[bubble_indices[kBubbleBottomRight]], position, bubble, kPaintPositionIsTop);

	enum {
		kSpeechTwoLinesShift = 8,
		kSpeechVMargin = 8,
		kSpeechOneLineShift = 0xc,
		kSpeechLeftIndent = 16,
		kSpeechFirstLineExtraIndent = 7
	};

	int shift = 0;
	if (lines == 1)
		shift = kSpeechOneLineShift;
	if (lines == 2)
		shift = kSpeechTwoLinesShift;

	paintText(kSpeechLeftIndent, kSpeechVMargin + shift, colour, string, bubble, 0, kSpeechFirstLineExtraIndent);

	if (left + bubble->w >= 320)
		left = 320 - bubble->w;

	if (top + bubble->h >= 200)
		top = 200 - bubble->h;

	Common::Rect rect(left, top, left + bubble->w, top + bubble->h);
	return rect;
}

Common::Rect Graphics::paintText(uint16 left, uint16 top, byte colour, const byte *string, Surface *dest, uint16 *_lines, uint8 firstLineExtraIndent) {
	byte ch = 0;
	uint16 current_left = left + firstLineExtraIndent;
	uint16 current_top = top;
	uint16 max_left = left;
	byte current_colour = colour;
	uint16 lines = 1;

	int opt;
	while ((ch = *(string++))) {
		switch(ch) {
		case kStringMove:
			current_left = READ_LE_UINT16(string);
			string += 2;
			current_top = READ_LE_UINT16(string);
			string += 2;
			debugC(3, kDebugLevelGraphics, "string move to %d:%d", current_left, current_top);
			break;
		case kStringSetColour:
			current_colour = *(string++);
			break;
		case kStringDefaultColour:
			current_colour = colour;
			break;
		case kStringAdvance:
			current_left += *(string++);
			break;
		case kStringCenter:
			current_left = (320 - calculateLineWidth(string))/2;
			break;
		case '\n':
		case '\r':
			current_left = left;
			current_top += kLineHeight;
			lines++;
			break;
		case kStringMenuOption:
			opt = _mOption++;
			_optionRects[opt] = paintText(current_left, current_top, kOptionColour, string, dest);
			while (*(string++));
			_optionValues[opt] = READ_LE_UINT16(string);
			debugC(2, kDebugLevelGraphics | kDebugLevelScript, "option value %d: 0x%x", opt, _optionValues[opt]);
			string += 2;
			break;
		default:
			current_left += paintChar(current_left, current_top, current_colour, ch, dest);
			if (current_left > max_left)
				max_left = current_left;
		}
	}

	if (_lines)
		*_lines = lines;

	return Common::Rect(left, top, max_left, current_top + kLineHeight);
}

byte Graphics::clampChar(byte ch) {
	if (ch == '#')
		return '!';
	if (ch < ' ' || ch > '~')
		return '?';
	return ch;
}

uint16 Graphics::calculateLineWidth(const byte *string) const {
	byte ch;
	uint16 total = 0;
	while ((ch = *(string++))) {
		if (ch == '\n' || ch == '\r')
			break;
		total += getGlyphWidth(ch);
	}
	return total;
}

uint16 Graphics::getGlyphWidth(byte ch) const {
	if (ch == ' ')
		return 4;
	else
		return getGlyph(ch)->w-1;
}

Sprite *Graphics::getGlyph(byte ch) const {
	// TODO perhaps cache or sth
	ch = clampChar(ch);
	if (ch == ' ')
		return 0; // space has no glyph, just width 4
	return _resources->getGlyph(ch);
}

/**
 * @returns char width
 */
uint16 Graphics::paintChar(uint16 left, uint16 top, byte colour, byte ch, Surface *dest) const {
	Sprite *glyph = getGlyph(ch);
	int w;
	if (glyph) {
		glyph->recolour(colour);
		if (dest)
			paint(glyph, Common::Point(left, top+glyph->h), dest);
		w = glyph->w - 1;
		delete glyph;
	} else return 4;
	return w;
}

void Graphics::paint(const Sprite *sprite, Common::Point pos, Surface *dest, int flags) const {
	debugC(4, kDebugLevelGraphics, "painting sprite at %d:%d (+%d:%d) [%dx%d]", pos.x, pos.y, sprite->_hotPoint.x, sprite->_hotPoint.y, sprite->w, sprite->h);

	Common::Rect r(sprite->w, sprite->h);
	r.moveTo(pos);
	if (!(flags & kPaintPositionIsTop))
		r.translate(0, -sprite->h); // this is actually bottom
	r.translate(-sprite->_hotPoint.x, sprite->_hotPoint.y);

	r.clip(319, 199);
	debugC(4, kDebugLevelGraphics, "transformed rect: %d:%d %d:%d", r.left, r.top, r.right, r.bottom);

	dest->blit(sprite, r, 0, (flags & kPaintSemiTransparent) ? &_tintedPalette : 0);
}

Common::Point Graphics::cursorPosition() const {
	debugC(1, kDebugLevelGraphics, "cursor position STUB");
	return Common::Point(160, 100);
}

void Graphics::updateScreen() {
	_system->copyRectToScreen(reinterpret_cast<byte *>(_framebuffer->pixels), _framebuffer->pitch, 0, 0, 320, 200);

	if (_willFadein && (_fadeFlags & kPartialFade)) {
		debugC(3, kDebugLevelGraphics, "performing partial fade in");
		_willFadein = false;
		fadeIn(_interfacePalette + 160*4, 160, 96);
	} else if (_willFadein && !(_fadeFlags & kPartialFade)) {
		fadeIn();
		_willFadein = false;
	}

	_system->updateScreen();
}

void Graphics::showCursor() {
	Sprite *cursor = _resources->getCursor();
	assert(cursor->pitch == cursor->w);
	::Graphics::CursorManager &m = ::Graphics::CursorManager::instance();
	m.replaceCursor(reinterpret_cast<byte *>(cursor->pixels), cursor->w, cursor->h, cursor->_hotPoint.x, cursor->_hotPoint.y, 0);
	m.showMouse(true);
}

void Graphics::hideCursor() {
	::Graphics::CursorManager::instance().showMouse(false);
}

void Graphics::paintRect(const Common::Rect &r, byte colour) {
	_framebuffer->frameRect(r, colour);
}

void Graphics::push(Paintable *p) {
	debugC(3, kDebugLevelGraphics, "pushing to paintables");
	_paintables.push_back(p);
}

void Graphics::pop(Paintable *p) {
	debugC(3, kDebugLevelGraphics, "popping from paintables");
	_paintables.remove(p);
}

void Graphics::hookAfterRepaint(CodePointer &p) {
	_afterRepaintHooks.push_back(p);
}

const char Graphics::_charwidths[] = {
	#include "charwidths.data"
};

void Graphics::clearPalette(int offset, int count) {
	byte pal[0x400];
	fill(pal, pal+0x400, 0);
	_system->getPaletteManager()->setPalette(pal, offset, count);
}

void Graphics::setPalette(const byte *colours, uint start, uint num) {
	_system->getPaletteManager()->setPalette(colours, start, num);

	// calculate tinted palette
	for (int i = 0; i < 256; ++i) {
		const byte *colour = colours + i*4;
		const byte luma = (30 * colour[0] + 60 * colour[1] + 10 * colour[2])/100;

		byte curr = 174;
		byte best_diff = 255;
		byte best_color = curr;

		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 2; k++) {
				int16 diff = _interfacePalette[(curr + k) * 4] - luma;
				if (diff < 0)
					diff = -diff;
				if (diff < best_diff) {
					best_diff = diff;
					best_color = curr + k;
				}
			}
			curr += 16;
		}

		_tintedPalette[i] = best_color;
	}
}

void Graphics::goFullscreen() {
	_fullscreen = true;
}

struct Tr : public unary_function<byte, byte> {
	byte operator()(const byte &b) const { return 0xff & ((b << 1) - 63); }
};

void Graphics::fadeIn(const byte *colours, uint start, uint num) {
	byte buf[0x400];
	if (!colours) {
		_system->getPaletteManager()->grabPalette(buf, start, num);
		colours = buf;
	}

	const int bytes = num * 4;
	byte current[0x400];

	fill(current, current + bytes, 0);

	byte off = 255;
	for (int j = 0; j < 63; j++) {
		off -= 4;
		for (int i = 0; i < bytes; i++)
			current[i] = colours[i] - MIN(off, colours[i]);

		_system->getPaletteManager()->setPalette((current), start, num);
		_system->updateScreen();
		Eng.delay(1000/25);

		if (Log.canSkipCutscene() && Eng.escapePressed()) {
			_system->getPaletteManager()->setPalette(colours, start, num);
			_system->updateScreen();
			return;
		}
	}
}

bool Graphics::fadeOut(FadeFlags f) {
	int bytes = 0x400;
	int offset = 0;
	int colours = 256;
	byte current[0x400];

	if (f == kPartialFade) {
		bytes = 96 * 4;
		offset = 160;
		colours = 96;
	}

	_system->getPaletteManager()->grabPalette(current, offset, colours);

	for (int j = 0; j < 63; j++) {
		for (int i = 0; i < bytes; i++)
			current[i] -= MIN<byte>(4, current[i]);

		_system->getPaletteManager()->setPalette((current), offset, colours);
		_system->updateScreen();
		Eng.delay(1000/25);

		if (Log.canSkipCutscene() && Eng.escapePressed()) {
			Log.skipCutscene();
			return false;
		}

	}
	return true;
}

void Graphics::say(const byte *text, uint16 length, uint16 frames) {
	if (_speech)
		error("queuing speech not supported yet.");

	_speech = new byte[length];
	memcpy(_speech, text, length);
	_speechFramesLeft = frames;
	paintSpeech();
}

void Graphics::runWhenSaid(const CodePointer &cb) {
	unless (_speechDoneCallback.isEmpty())
		error("queuing events on speech complete not supported yet");

	_speechDoneCallback = cb;
}

} // End of namespace Innocent
