#include "innocent/graphics.h"

#include <algorithm>
#include <functional>

#include "common/system.h"
#include "common/util.h"
#include "graphics/cursorman.h"

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

namespace Innocent {

DECLARE_SINGLETON(Graphics);

Common::Point &operator+=(Common::Point &p1, const Common::Point &p2) { return p1 = Common::Point(p1.x + p2.x, p1.y + p2.y); }

void Graphics::setEngine(Engine *engine) {
	_engine = engine;
	_framebuffer.reset(new Surface);
	_framebuffer->create(320, 200, 1);
	_willFadein = false;
}

void Graphics::init() {
	_resources = _engine->resources();
	_system = _engine->_system;
	loadInterface();
}

void Graphics::paint() {
	debugC(2, kDebugLevelFlow | kDebugLevelGraphics, ">>>start paint procedure");

	paintBackdrop();
	paintExits();
	paintAnimations();

	debugC(3, kDebugLevelGraphics, "painting paintables");
	foreach (Paintable *, _paintables)
		(*it)->paint(this);

	updateScreen();

	unless (_afterRepaintHooks.empty()) {
		debugC(3, kDebugLevelGraphics | kDebugLevelScript, "running hooks");
		foreach (CodePointer, _afterRepaintHooks)
			it->run();
		_afterRepaintHooks.clear();
	}

	debugC(2, kDebugLevelFlow | kDebugLevelGraphics, "<<<end paint procedure");
}

void Graphics::paintExits() {
	debugC(3, kDebugLevelFlow | kDebugLevelGraphics, "painting exits");
	foreach(Exit *, _engine->logic()->room()->exits())
		(*it)->paint(this);
}

void Graphics::loadInterface() {
	byte palette[0x400];

	_resources->loadInterfaceImage(_interface, palette);

	_engine->_system->setPalette(palette + 160 * 4, 160, 96);
}

void Graphics::paintInterface() {
//	_framebuffer->blit(_interface, Common::Point(152, 48));
}

void Graphics::setBackdrop(uint16 id) {
	byte palette[0x400];
	_backdrop.reset(_resources->loadBackdrop(id, palette));
	setPalette(palette, 0, 256);
}

void Graphics::willFadein() {
	_willFadein = true;
	clearPalette();
	updateScreen();
}

void Graphics::paintBackdrop() {
	// TODO cropping
	debugC(3, kDebugLevelGraphics, "painting backdrop");
	_framebuffer->blit(_backdrop.get());
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
	frame.create(width * kFrameTileWidth, height * kFrameTileHeight+4, 1);

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

Common::Rect Graphics::paintText(uint16 left, uint16 top, byte colour, byte *string, Surface *dest) {
	byte ch = 0;
	uint16 current_left = left;
	uint16 current_top = top;
	uint16 max_left = left;
	byte current_colour = colour;

	int opt;
	while ((ch = *(string++))) {
		switch(ch) {
		case '\n':
		case '\r':
			current_left = left;
			current_top += kLineHeight;
			break;
		case kStringDefaultColour:
			current_colour = colour;
			break;
		case kStringSetColour:
			current_colour = *(string++);
			break;
		case kStringMenuOption:
			opt = _mOption++;
			_optionRects[opt] = paintText(current_left, current_top, kOptionColour, string, dest);
			while (*(string++));
			_optionValues[opt] = READ_LE_UINT16(string);
			string += 2;
			break;
		default:
			current_left += paintChar(current_left, current_top, current_colour, ch, dest);
			if (current_left > max_left)
				max_left = current_left;
		}
	}

	return Common::Rect(left, top, max_left, current_top + kLineHeight);
}

byte Graphics::clampChar(byte ch) {
	if (ch == '#')
		return '!';
	if (ch < ' ' || ch > '~')
		return '?';
	return ch;
}

/**
 * @returns char width
 */
uint16 Graphics::paintChar(uint16 left, uint16 top, byte colour, byte ch, Surface *dest) const {
	// TODO perhaps cache or sth
	ch = clampChar(ch);
	if (ch == ' ')
		return 4; // space has no glyph, just width 4
	Sprite *glyph = _resources->getGlyph(ch);
	glyph->recolour(colour);
	paint(glyph, Common::Point(left, top+glyph->h), dest);
	int w = glyph->w - 1;
	delete glyph;
	return w;
}

void Graphics::paint(const Sprite *sprite, Common::Point pos, Surface *dest) const {
	debugC(4, kDebugLevelGraphics, "painting sprite at %d:%d (+%d:%d) [%dx%d]", pos.x, pos.y, sprite->_hotPoint.x, sprite->_hotPoint.y, sprite->w, sprite->h);
	pos += sprite->_hotPoint;

	Common::Rect r(sprite->w, sprite->h);
	r.moveTo(pos);
	// this is actually bottom
	r.translate(0, -sprite->h);

	r.clip(319, 199);
	debugC(4, kDebugLevelGraphics, "transformed rect: %d:%d %d:%d", r.left, r.top, r.right, r.bottom);

	dest->blit(sprite, r, 0);
}

Common::Point Graphics::cursorPosition() const {
	debugC(1, kDebugLevelGraphics, "cursor position STUB");
	return Common::Point(160, 100);
}

void Graphics::updateScreen() const {
	_system->copyRectToScreen(reinterpret_cast<byte *>(_framebuffer->pixels), _framebuffer->pitch, 0, 0, 320, 200);
	_system->updateScreen();
}

void Graphics::showCursor() const {
	Sprite *cursor = _resources->getCursor();
	assert(cursor->pitch == cursor->w);
	::Graphics::CursorManager &m = ::Graphics::CursorManager::instance();
	m.replaceCursor(reinterpret_cast<byte *>(cursor->pixels), cursor->w, cursor->h, cursor->_hotPoint.x, cursor->_hotPoint.y, 0);
	m.showMouse(true);
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

void Graphics::clearPalette() {
	byte pal[0x400];
	fill(pal, pal+0x400, 0);
	_system->setPalette(pal, 0, 256);
}

void Graphics::setPalette(const byte *colours, uint start, uint num) {
	if (_willFadein)
		fadeIn(colours, start, num);
	else
		_system->setPalette(colours, start, num);
	_willFadein = false;
}

struct Tr : public unary_function<byte, byte> {
	byte operator()(const byte &b) const { return 0xff & ((b << 1) - 63); }
};

void Graphics::fadeIn(const byte *colours, uint start, uint num) {
	paint();
	const int bytes = num * 4;
	byte current[0x400];

	fill(current, current + bytes, 0);

	for (int j = 0; j < 63; j++) {
		for (int i = 0; i < bytes; i++)
			current[i] += MIN<byte>(4, colours[i] - current[i]);
		_system->setPalette((current), start, num);
		_system->updateScreen();
		_system->delayMillis(1000/50);
	}
}

void Graphics::fadeOut() {
	paint();
	const int bytes = 0x400;
	byte current[0x400];

	_system->grabPalette(current, 0, 256);

	for (int j = 0; j < 63; j++) {
		for (int i = 0; i < bytes; i++)
			current[i] -= MIN<byte>(4, current[i]);
		_system->setPalette((current), 0, 256);
		_system->updateScreen();
		_system->delayMillis(1000/50);
	}
}

} // End of namespace Innocent
