#include "innocent/graphics.h"

#include "common/system.h"

#include "innocent/innocent.h"
#include "innocent/resources.h"

namespace Innocent {

Common::Point operator+=(Common::Point &p1, const Common::Point &p2) { return p1 = Common::Point(p1.x + p2.x, p1.y + p2.y); }

Graphics::Graphics(Engine *engine)
	 : _engine(engine) {
}

void Graphics::init() {
	 _resources = _engine->resources();
	 _system = _engine->_system;
	 loadInterface();
}

void Graphics::loadInterface() {
	byte palette[0x400];

	_resources->loadInterfaceImage(_interface, palette);
	debug(kAck, "loaded interface image");

	_engine->_system->setPalette(palette + 160 * 4, 160, 96);
	debug(kAck, "set interface palette");
}

void Graphics::paintInterface() {
	debug(kAck, "painting interface");
	_engine->_system->copyRectToScreen(_interface, 320, 0, 152, 320, 48);
	debug(kAck, "painted interface");
}

void Graphics::setBackdrop(uint16 id) {
	debug(2, "setting backdrop image id to 0x%04x", id);

	byte palette[0x400];
	_backdrop.reset(_resources->loadBackdrop(id, palette));
	debug(3, "backdrop loaded");
	_system->setPalette(palette, 0, 256);
}

void Graphics::paintBackdrop() {
	// TODO cropping
	_system->copyRectToScreen(reinterpret_cast<byte *>(_backdrop->pixels), 320, 0, 0, 320, 200);
}

void Graphics::paintText(uint16 left, uint16 top, byte colour, byte *string) {
	byte ch = 0;
	uint16 current_left = left;
	byte current_colour = colour;

	debug(1, "painting string %s", string);
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
	debug(2, "painting glyph '%c' at position %d:%d", ch, left, top);
	ch = clampChar(ch);
	if (ch == ' ')
		return 4; // space has no glyph, just width 4
	Sprite *glyph = _resources->getGlyph(ch);
	glyph->recolour(colour);
	paint(glyph, Common::Point(left, top));
	uint16 w = glyph->w;
	delete glyph;
	return w;
}

void Graphics::paint(Sprite *sprite, Common::Point pos) const {
	pos += sprite->_hotPoint;
	debug(4, "copying rect to screen, %d %d %d %d %d", sprite->pitch, pos.x, pos.y, sprite->w, sprite->h);
	_system->copyRectToScreen(reinterpret_cast<byte *>(sprite->pixels), sprite->pitch,
							   pos.x, pos.y, sprite->w, sprite->h);
}


} // End of namespace Innocent
