#include "innocent/graphics.h"

#include "common/system.h"
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

namespace Innocent {

DECLARE_SINGLETON(Graphics);

Common::Point &operator+=(Common::Point &p1, const Common::Point &p2) { return p1 = Common::Point(p1.x + p2.x, p1.y + p2.y); }

void Graphics::setEngine(Engine *engine) {
	_engine = engine;
	_framebuffer.reset(new Surface);
	_framebuffer->create(320, 200, 1);
}

void Graphics::init() {
	_resources = _engine->resources();
	_system = _engine->_system;
	loadInterface();
}

void Graphics::paint() {
	debugC(2, kDebugLevelFlow | kDebugLevelGraphics, ">>>start paint procedure");

	paintExits();
	paintAnimations();

	debugC(3, kDebugLevelGraphics, "painting paintables");
	foreach (Paintable *, _paintables)
		(*it)->paint(this);

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
	_system->setPalette(palette, 0, 256);
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

int8 Graphics::ask(uint16 left, uint16 top, byte width, byte height, byte *string) {
	width += 2;
	height += 2;
	enum {
		kFrameTileHeight = 12,
		kFrameTileWidth = 16
	};

	Surface frame;
	frame.create(width * kFrameTileWidth, height * kFrameTileHeight, 1);

	Sprite **frames = _resources->frames();

	Common::Rect tile(kFrameTileWidth, kFrameTileHeight);

	frame.blit(frames[kFrameTopLeft], tile);
	tile.translate(kFrameTileWidth, 0);
	for (int x = 1; x < width - 1; x++) {
		frame.blit(frames[kFrameTop], tile);
		tile.translate(kFrameTileWidth, 0);
	}
	frame.blit(frames[kFrameTopRight], tile);

	tile.translate(0, kFrameTileHeight);
	tile.moveTo(0, tile.top);

	for (int y = 1; y < height - 1; y++) {
		frame.blit(frames[kFrameLeft], tile);
		tile.translate(kFrameTileWidth, 0);
		for (int x = 1; x < width - 1; x++) {
			frame.blit(frames[kFrameFill], tile);
			tile.translate(kFrameTileWidth, 0);
		}
		frame.blit(frames[kFrameTopRight], tile);
		tile.translate(0, kFrameTileHeight);
		tile.moveTo(0, tile.top);
	}

	frame.blit(frames[kFrameBottomLeft], tile);
	tile.translate(kFrameTileWidth, 0);
	for (int x = 1; x < width - 1; x++) {
		frame.blit(frames[kFrameBottom], tile);
		tile.translate(kFrameTileWidth, 0);
	}
	frame.blit(frames[kFrameBottomRight], tile);

	paintText(10, 16, 254, string, &frame);

	_system->copyRectToScreen(reinterpret_cast<byte *>(frame.pixels), frame.pitch, top, left, width * kFrameTileWidth, height * kFrameTileHeight);
	_system->updateScreen();

	bool show = true;
	while (show) {
		_engine->debugger()->onFrame();
		_system->delayMillis(1000/60);
		Common::Event event;
		while (_engine->eventMan()->pollEvent(event)) {
			switch(event.type) {
			case Common::EVENT_LBUTTONUP:
				show = false;

			default:
				break;
			}
		}
	}

	return -1;
}

void Graphics::paintText(uint16 left, uint16 top, byte colour, byte *string, Surface *dest) {
	byte ch = 0;
	uint16 current_left = left;
	byte current_colour = colour;

	while ((ch = *(string++))) {
		switch(ch) {
		case '\n':
		case '\r':
			current_left = left;
			top += kLineHeight;
			break;
		case kStringDefaultColour:
			current_colour = colour;
			break;
		case kStringSetColour:
			current_colour = *(string++);
			break;
		default:
			if (current_left > 310) {
				current_left = left;
				top += kLineHeight;
			}
			current_left += paintChar(current_left, top, current_colour, ch, dest);
		}
	}
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
	debugC(3, kDebugLevelGraphics, "painting sprite at %d:%d (+%d:%d) [%dx%d]", pos.x, pos.y, sprite->_hotPoint.x, sprite->_hotPoint.y, sprite->w, sprite->h);
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

const char Graphics::_charwidths[] = {
	#include "charwidths.data"
};


} // End of namespace Innocent
