#include "innocent/graphics.h"

#include "common/system.h"

#include "innocent/animation.h"
#include "innocent/debug.h"
#include "innocent/exit.h"
#include "innocent/innocent.h"
#include "innocent/logic.h"
#include "innocent/resources.h"
#include "innocent/room.h"
#include "innocent/util.h"

namespace Innocent {

Common::Point &operator+=(Common::Point &p1, const Common::Point &p2) { return p1 = Common::Point(p1.x + p2.x, p1.y + p2.y); }

Graphics::Graphics(Engine *engine)
	 : _engine(engine), _framebuffer(new Surface) {
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

void Graphics::paintText(uint16 left, uint16 top, byte colour, byte *string) {
	byte ch = 0;
	uint16 current_left = left;
	byte current_colour = colour;

	while ((ch = *(string++))) {
		switch(ch) {
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
			current_left += paintChar(current_left, top, current_colour, ch);
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
uint16 Graphics::paintChar(uint16 left, uint16 top, byte colour, byte ch) const {
	// TODO perhaps cache or sth
	ch = clampChar(ch);
	if (ch == ' ')
		return 4; // space has no glyph, just width 4
	Sprite *glyph = _resources->getGlyph(ch);
	glyph->recolour(colour);
	paint(glyph, Common::Point(left, top+glyph->h));
	int w = glyph->w - 1;
	delete glyph;
	return w;
}

void Graphics::paint(const Sprite *sprite, Common::Point pos) const {
	debugC(3, kDebugLevelGraphics, "painting sprite at %d:%d (+%d:%d) [%dx%d]", pos.x, pos.y, sprite->_hotPoint.x, sprite->_hotPoint.y, sprite->w, sprite->h);
	pos += sprite->_hotPoint;

	Common::Rect r(sprite->w, sprite->h);
	r.moveTo(pos);
	// this is actually bottom
	r.translate(0, -sprite->h);

	r.clip(319, 199);
	debugC(4, kDebugLevelGraphics, "transformed rect: %d:%d %d:%d", r.left, r.top, r.right, r.bottom);

	_framebuffer->blit(sprite, r, 0);
}

Common::Point Graphics::cursorPosition() const {
	debugC(1, kDebugLevelGraphics, "cursor position STUB");
	return Common::Point(160, 100);
}

void Graphics::updateScreen() const {
	_system->copyRectToScreen(reinterpret_cast<byte *>(_framebuffer->pixels), _framebuffer->pitch, 0, 0, 320, 200);
	_system->updateScreen();
}

const char Graphics::_charwidths[] = {
	#include "charwidths.data"
};

} // End of namespace Innocent
