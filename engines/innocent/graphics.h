#ifndef INNOCENT_GRAPHICS_H
#define INNOCENT_GRAPHICS_H

#include <memory>

#include "common/rect.h"
#include "common/singleton.h"
#include "config.h"

#include "innocent/types.h"
#include "innocent/value.h"

class OSystem;

namespace Innocent {

class Engine;
class Resources;
class Surface;
class Sprite;

Common::Point &operator+=(Common::Point &p1, const Common::Point &p2);

class Graphics : public Common::Singleton<Graphics> {
public:
	Graphics() {}
	
	void setEngine(Engine *engine);

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
	void paintSpeech();
	void prepareInterfacePalette();

	void push(Paintable *p);
	void pop(Paintable *p);
	void hookAfterRepaint(CodePointer &p);

	void setBackdrop(uint16 id);
	void paintBackdrop();

	void willFadein();
	void fadeOut();

	void say(const byte *text, uint16 frames = 50);
	void runWhenSaid(const CodePointer &p);

	uint16 ask(uint16 left, uint16 top, byte width, byte height, byte *string);
	Common::Rect paintText(uint16 left, uint16 top, byte colour, byte *string) {
		return paintText(left, top, colour, string, _framebuffer.get());
	}
	Common::Rect paintText(uint16 left, uint16 top, byte colour, byte *string, Surface *s);
	void paintRect(const Common::Rect &r, byte colour = 235);

	void paint(const Sprite *sprite, Common::Point pos) const {
		paint(sprite, pos, _framebuffer.get());
	}
	void paint(const Sprite *sprite, uint16 left, uint16 top, Surface *dest) const {
		paint(sprite, Common::Point(left, top), dest);
	}
	void paint(const Sprite *sprite, Common::Point pos, Surface *s) const;

	Common::Point cursorPosition() const;
	void showCursor() const;

	void updateScreen() const;

	private:
	enum {
		kLineHeight = 12
	};
	static byte clampChar(byte ch);
	uint16 calculateLineWidth(byte *string) const;
	uint16 getGlyphWidth(byte ch) const;
	Sprite *getGlyph(byte ch) const;

	/**
	 * paint a character on screen
	 * @returns char width
	 */
	uint16 paintChar(uint16 left, uint16 top, byte colour, byte character, Surface *s) const;
	byte _interface[0x3c00];
	Engine *_engine;
	Resources *_resources;
	OSystem *_system;
	std::auto_ptr<Surface> _backdrop, _framebuffer;

	static const char _charwidths[];

private:
	void clearPalette();
	void setPalette(const byte *colours, uint start, uint num);
	void fadeIn(const byte *colours, uint start, uint num);

	Common::List<Paintable *> _paintables;
	Common::List<CodePointer> _afterRepaintHooks;

	bool _willFadein;
	byte _interfacePalette[0x400];

	byte *_speech;
	uint16 _speechFramesLeft;
	CodePointer _speechDoneCallback;
};

#define Graf Graphics::instance()

} // End of namespace Innocent

#endif // INNOCENT_GRAPHICS_H
