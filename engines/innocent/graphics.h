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

#ifndef INNOCENT_GRAPHICS_H
#define INNOCENT_GRAPHICS_H

// Used for auto_ptr (Not needed?)
#include <memory>

#include "common/rect.h"
#include "common/list.h"
#include "common/singleton.h"

#include "innocent/types.h"
#include "innocent/value.h"

class OSystem;

namespace Innocent {

class Engine;
class Resources;
class Surface;
class Sprite;

//Common::Point &operator+=(Common::Point &p1, const Common::Point &p2);
//Common::Point &operator-=(Common::Point &p1, const Common::Point &p2);

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

	enum FadeFlags {
		kFullFade = 0,
		kPartialFade = 1
	};
	void willFadein(FadeFlags f = kFullFade);
	bool fadeOut(FadeFlags f = kFullFade);

	void say(const byte *text, uint16, uint16 frames = 50);
	void runWhenSaid(const CodePointer &p);

	uint16 ask(uint16 left, uint16 top, byte width, byte height, byte *string);
	Common::Rect paintText(uint16 left, uint16 top, byte colour, const byte *string) {
		return paintText(left, top, colour, string, _framebuffer.get());
	}

	Common::Rect textMetrics(const byte *string, uint16 *lines = 0, uint16 left = 0, uint16 top = 0) {
		return paintText(left, top, 235, string, 0, lines);
	}
	Common::Rect paintText(uint16 left, uint16 top, byte colour, const byte *string, Surface *s, uint16 *lines = 0, uint8 firstLineExtraIndent = 0);

	void paintSpeechBubbleColumn(Sprite *top, Sprite *fill, Common::Point &point, uint8 fill_tiles, Surface *dest);
	Common::Rect paintSpeechInBubble(Common::Point pos, byte colour, const byte *string, Surface *dest);

	void paintRect(const Common::Rect &r, byte colour = 235);

	enum PaintFlags {
		kPaintNormal = 0,
		kPaintPositionIsTop = 1,
		kPaintSemiTransparent = 2
	};
	void paint(const Sprite *sprite, Common::Point pos, int flags = kPaintNormal) const {
		paint(sprite, pos, _framebuffer.get(), flags);
	}
	void paint(const Sprite *sprite, uint16 left, uint16 top, Surface *dest, int flags = kPaintNormal) const {
		paint(sprite, Common::Point(left, top), dest, flags);
	}
	void paint(const Sprite *sprite, Common::Point pos, Surface *s, int flags = kPaintNormal) const;

	Common::Point cursorPosition() const;
	void showCursor();
	void hideCursor();

	void updateScreen();
	void setPalette(const byte *colours, uint start, uint num);

	/** Go fullscreen. This will hide the interface. */
	void goFullscreen();

private:
	enum {
		kLineHeight = 12
	};
	static byte clampChar(byte ch);
	uint16 calculateLineWidth(const byte *string) const;
	uint16 getGlyphWidth(byte ch) const;
	Sprite *getGlyph(byte ch) const;

	/**
	 * paint a character on screen
	 * @returns char width
	 */
	uint16 paintChar(uint16 left, uint16 top, byte colour, byte character, Surface *s) const;
	Surface *_interface;
	Engine *_engine;
	Resources *_resources;
	OSystem *_system;
	std::auto_ptr<Surface> _backdrop, _framebuffer;

	static const char _charwidths[];

private:
	void clearPalette(int start = 0, int count = 256);
	void fadeIn(const byte *colours = 0, uint start = 0, uint num = 256);

	Common::List<Paintable *> _paintables;
	Common::List<CodePointer> _afterRepaintHooks;

	bool _willFadein;
	FadeFlags _fadeFlags;
	byte _interfacePalette[0x400];
	byte _tintedPalette[256];

	byte *_speech;
	uint16 _speechFramesLeft;
	CodePointer _speechDoneCallback;
	bool _fullscreen;
};

#define Graf Graphics::instance()

} // End of namespace Innocent

#endif // INNOCENT_GRAPHICS_H
