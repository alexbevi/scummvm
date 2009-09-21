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

#ifndef GARGOYLE_GRAPHICS_H
#define GARGOYLE_GRAPHICS_H

#include "common/rect.h"
#include "common/str.h"
#include "common/stream.h"
#include "common/system.h"
#include "graphics/font.h"
#include "graphics/surface.h"

namespace Gargoyle {

enum TextStyle {
	REVERSE_STYLE = 1,
	BOLDFACE_STYLE = 2,
	EMPHASIS_STYLE = 4,
	FIXED_WIDTH_STYLE = 8
};

extern const char *FONT_STD;
extern const char *FONT_FIXED;
extern const char *FONT_FIXED2;

class Surface;

typedef Common::List<Common::Rect> RectList;

// RGBList
// Implements a list of RGB entries 

class RGBList {
private:
	int _size;
	uint32 *_data;
	byte *_palIndexes;
	bool _freeData;
public:
	RGBList(int numEntries = 256, uint32 *srcData = NULL, bool freeData = true);
	~RGBList();

	uint32 *data() { return _data; }
	byte *palIndexes() { return _palIndexes; }
	int size() { return _size; }
};

// TextSettings
// Text related settings used by the Surface class

class TextSettings {
private:
	const Graphics::Font *_font;
	int _style;
	bool _stdFont, _fixedFont;
	Common::Point _minBoundingBox;
public:
	TextSettings();

	bool cursorOn;
	Common::Point textPosition;
	Common::String pendingText, lastOutput;

	const Graphics::Font *getFont() const { return _font; }
	const Graphics::Font *getFont(const char *fontName) const;
	void setFont(const char *fontName);
	int getStyle() const { return _style; }
	void setStyle(int newStyle);
	void setMinCharBounds(int xs, int ys);

	/* Note: For standard font we use the ScummVM GUI 6x12 font for fixed width, and Big 6x14
	 * for variable. So when in standard font mode, always report the height as 14 pixels so
	 * the two font sizes seem equivalent
	 */
	int getStringWidth(const Common::String &s);
	int getCharWidth(char c) const { return _font->getCharWidth(c); }
	int getCharWidth() const { return MAX(_font->getMaxCharWidth(), (int)_minBoundingBox.x); }
	int getFontHeight() const { return MAX(_stdFont ? 14 : _font->getFontHeight(), (int)_minBoundingBox.y); }
	void writeString(Graphics::Surface *surface, const Common::String &s, const Common::Point &r, int strWidth);

	uint8 fore;
	uint8 back;
	bool foreDefault;
	bool backDefault;
	bool backTransparent;

	uint8 background() const;
	uint8 foreground() const;
};

// ScreenSurface
// Interface wrapper for the ScummVM screen

class ScreenSurface: public Graphics::Surface {
private:
	bool _freeData;
protected:
	int _top, _left;
	uint8 _colour;
	TextSettings _textSettings;
	RectList _dirtyRects;
public:
	ScreenSurface();
	virtual ~ScreenSurface();

	void init(const Common::Rect &bounds, int pitchAmount, byte *data = NULL);
	inline byte *getBasePtr(int x, int y) {
		return (byte *)Graphics::Surface::getBasePtr(x, y);
	}

	void update();
	void setColour(byte value) { _colour = value; }
	byte getColour() { return _colour; }

	int width() { return w; }
	int height() { return h; }
	void empty() { fillRect(Common::Rect(width(), height()), 0); }
	void fillRect(const Common::Rect &r, int colour = -1);
	void copyFrom(Graphics::Surface *src, const Common::Rect &srcBounds, int destX, int destY,
		int transparentColour = -1);
	void copyFrom(Graphics::Surface *src, int destX, int destY, int transparentColour = -1);
	void scrollArea(const Common::Rect &r, int yAmount);

	TextSettings &getTextSettings() { return _textSettings; }
	void setTextSettings(const TextSettings &v) { _textSettings = v; }
	const Common::Point &getTextPosition() { return getTextSettings().textPosition; }
	void setTextPosition(const Common::Point &pt);

//	FontType getFontType() { return getTextSettings().getFontType(); }
	void setFont(const char *fontName) { _textSettings.setFont(fontName); }
	void addChar(char c);
	void flushText();
	void flushDisplay() { flushText(); update(); }
	void writeString(const Common::String &s);
	void writeString(const Common::String &s, const Common::Point &r);
	void writeString(const Common::String &s, const Common::Point &r, int strWidth);
	void writeChar(char c);
	void applyTextSettings() {}
	void drawCursor(bool flag);
	void drawInput(char *buffer, uint pos, uint widthVal, bool cursor);
	void nextLine();
	void scroll(int size = -1);
	void addDirtyRect(const Common::Rect &r);
};

class SubSurface {
private:
	ScreenSurface *_parent;
	Common::Rect _bounds;
	bool _separateText;
	uint8 _colour;
	TextSettings _textSettings;

	const Common::Rect getBounds(const Common::Rect &r) {
		return Common::Rect(_bounds.left + r.left, _bounds.top + r.top,
			MAX(_parent->width(), (int)_bounds.right), 
			MAX(_parent->height(), (int)_bounds.bottom));
	}
public:
	SubSurface(ScreenSurface *ParentSurface, const Common::Rect &r, bool SeparateText = false);
	SubSurface(ScreenSurface *ParentSurface, bool SeparateText = false);
	virtual ~SubSurface() {}

	void frameRect(const Common::Rect &r, int colour = -1);
	void fillRect(const Common::Rect &r, int colour = -1);
	void vLine(int x, int y1, int y2, int colour = -1);
	void hLine(int x1, int x2, int y, int colour = -1);

	// Surface methods
	Common::Rect &bounds() { return _bounds; }
	int width() { return _bounds.width(); }
	int height() { return _bounds.height(); }
	int pitch() { return _parent->width(); }
	void setSize(int sizeX, int sizeY) { 
		_bounds.setWidth(sizeX);
		_bounds.setHeight(sizeY);
	}
	void empty() { fillRect(Common::Rect(width(), height()), 0); }
	void copyFrom(Graphics::Surface *src, const Common::Rect &srcBounds, int destX, int destY,
		int transparentColour = -1);
	void scrollArea(const Common::Rect &r, int yAmount);

	TextSettings &getTextSettings();
	void setTextSettings(const TextSettings &v);
	const Common::Point &getTextPosition();
	void setTextPosition(const Common::Point &pt);
};

#define FOREGROUND_COLOUR 1
#define BACKGROUND_COLOUR 0

} // End of namespace Gargoyle

#endif
