#ifndef INNOCENT_GRAPHICS_H
#define INNOCENT_GRAPHICS_H

#include <memory>

#include "common/rect.h"
#include "config.h"

class OSystem;

namespace Innocent {

class Engine;
class Resources;
class Surface;
class Sprite;

Common::Point &operator+=(Common::Point &p1, const Common::Point &p2);

class Graphics {
public:
	Graphics(Engine *engine);

	/**
	 * Load interface image and palette; sets the palette.
	 */
	void loadInterface();

	void init();
	void paint();

	/**
	 * Paint the interface to proper portion of the screen.
	 */
	void paintInterface();
	void paintAnimations();
	void paintExits();

	void setBackdrop(uint16 id);
	void paintBackdrop();

	void paintText(uint16 left, uint16 top, byte colour, byte *string);

	Common::Point cursorPosition() const;
	void paint(const Sprite *sprite, Common::Point pos) const;

	void updateScreen() const;

	private:
	enum {
		kLineHeight = 12
	};
	static byte clampChar(byte ch);
	/**
	 * paint a character on screen
	 * @returns char width
	 */
	uint16 paintChar(uint16 left, uint16 top, byte colour, byte character) const;
	byte _interface[0x3c00];
	Engine *_engine;
	Resources *_resources;
	OSystem *_system;
	std::auto_ptr<Surface> _backdrop, _framebuffer;

	static const char _charwidths[];
};

} // End of namespace Innocent

#endif // INNOCENT_GRAPHICS_H
