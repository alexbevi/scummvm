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

#include "gargoyle/gargoyle.h"
#include "gargoyle/graphics.h"
#include "common/file.h"
#include "common/endian.h"
#include "common/system.h"
#include "common/util.h"
#include "common/ptr.h"
#include "graphics/fontman.h"

namespace Gargoyle {

const char *FONT_STD = "STD";
const char *FONT_FIXED = "FIXED";
const char *FONT_FIXED2 = "FIXED2";

RGBList::RGBList(int numEntries, uint32 *srcData, bool freeData) {
	_size = numEntries;
	assert(numEntries <= 256);

	if (srcData == NULL) {
		_data = new uint32[numEntries];
		_freeData = true;
	} else {
		_data = srcData;
		_freeData = freeData;
	}

	_palIndexes = new byte[numEntries];
	Common::set_to(&_palIndexes[0], &_palIndexes[numEntries], 0);
}

RGBList::~RGBList() {
	if (_freeData)
		delete[] _data;
	delete[] _palIndexes;
}

//--------------------------------------------------------------------------

TextSettings::TextSettings(): textPosition(0, 0) {
	_style = 0;
	_stdFont = true;
	setFont(FONT_STD);
	fore = FOREGROUND_COLOUR;
	back = BACKGROUND_COLOUR;
	foreDefault = true;
	backDefault = true;
	backTransparent = false;
	cursorOn = false;
	_minBoundingBox.x = 0;
	_minBoundingBox.y = 0;
}

void TextSettings::setMinCharBounds(int xs, int ys) {
	_minBoundingBox.x = xs;
	_minBoundingBox.y = ys;
}

const Graphics::Font *TextSettings::getFont(const char *fontName) const {
	const Graphics::Font *f = NULL;

	// Try and get the specified font by name
	f = FontMan.getFontByName(fontName);	

	if (!f) {
		// Handle special font identifiers
		if (!strcmp(fontName, FONT_FIXED))
			f = FontMan.getFontByUsage(Graphics::FontManager::kGUIFont);
		else if (!strcmp(fontName, FONT_FIXED2))
			f = FontMan.getFontByUsage(Graphics::FontManager::kConsoleFont);
		else
			f = FontMan.getFontByUsage(Graphics::FontManager::kBigGUIFont);
	}

	return f;
}

void TextSettings::setFont(const char *fontName) {
	// Try and get the specified font by name

	_font = FontMan.getFontByName(fontName);	
	_fixedFont = false;

	if (!_font) {
		// Handle special font identifiers
		if (((_style & FIXED_WIDTH_STYLE) == 0) && !strcmp(fontName, FONT_STD))
			_font = FontMan.getFontByUsage(Graphics::FontManager::kBigGUIFont);
		else if (!strcmp(fontName, FONT_FIXED2))
			_font = FontMan.getFontByUsage(Graphics::FontManager::kConsoleFont);
		else
			_font = FontMan.getFontByUsage(Graphics::FontManager::kGUIFont);
	}
}

void TextSettings::setStyle(int newStyle) {
	if (_stdFont && ((newStyle & FIXED_WIDTH_STYLE) != (_style & FIXED_WIDTH_STYLE)))
		// Alternate to the other GUI font
		_font = FontMan.getFontByUsage((newStyle & FIXED_WIDTH_STYLE) ?
			Graphics::FontManager::kGUIFont : Graphics::FontManager::kBigGUIFont);		
		
	_style = newStyle;

}

/*
 * Gives a foreground colour depending on the current style
 */
uint8 TextSettings::foreground() const {
	// TODO: Currently, I'm relying on the Frotz palette to represent bold text as colour 3 (red),
	// and emphasis (aka. underline) using colour 4 (green). This should ideally be replaced with
	// a text display that is actually bold or underlined
	if (_style & REVERSE_STYLE)
		// Reverse mode, so return background as foreground
		return back;	
	else if (_style & BOLDFACE_STYLE)
		return 3;
	else if (_style & EMPHASIS_STYLE)
		return 4;
	else
		return fore;
}

uint8 TextSettings::background() const {
	return ((_style & REVERSE_STYLE) != 0) ? fore : back;
}

int TextSettings::getStringWidth(const Common::String &s) {
	return (!_fixedFont && ((_style & FIXED_WIDTH_STYLE) != 0)) ? getCharWidth() * s.size() :
		_font->getStringWidth(s.c_str());
}

void TextSettings::writeString(Graphics::Surface *surface, const Common::String &s, const Common::Point &r, 
							   int strWidth) {
	// Write out the string
	
	// FIXME: The ScummVM fixed-width GUI font is 12 pixels high, but the variable-width font is 14 pixels.
	// So if the fixed width font is active, for now increment y by 1 pixel, ie. (14 - 12) / 1
 	Common::Point pt(r.x, (_stdFont && (_style & FIXED_WIDTH_STYLE)) ? r.y + 1 : r.y);

	// Handle any extra spacing if the current font is too small for the given bounds
	if (_minBoundingBox.y > getFontHeight())
		pt.y = r.y + (_minBoundingBox.y - getFontHeight()) / 2;
	if ((_minBoundingBox.x > 0) && (_fixedFont || ((_style & FIXED_WIDTH_STYLE) != 0))) {
		// It's a fixed width font, but with a custom width. So draw each character separately
		const char *p = s.c_str();
		for (; *p; ++p, pt.x += getCharWidth())
			_font->drawChar(surface, *p, pt.x + 1, pt.y, foreground());

	} else {
		_font->drawString(surface, s, pt.x, pt.y, strWidth, foreground());
	}
}

//--------------------------------------------------------------------------

ScreenSurface::ScreenSurface() {
	_freeData = false;
}

ScreenSurface::~ScreenSurface() {
	if (_freeData)
		free();
}

void ScreenSurface::init(const Common::Rect &bounds, int pitchAmount, byte *data) {
	_top = bounds.top;
	_left = bounds.left;
	
	w = bounds.width();
	h = bounds.height();
	bytesPerPixel = 1;
	pitch = pitchAmount;

	_freeData = !data;
	if (!data) {
		assert((_top == 0) && (_left == 0));
		data = (byte *)calloc(pitchAmount * h, 1);
	}

	pixels = data + (_top * pitchAmount) + _left;
}

/**
 * Creates the union of two rectangles.
 */
static bool unionRectangle(Common::Rect &pDest, const Common::Rect &pSrc1, const Common::Rect &pSrc2) {
	pDest.left   = MIN(pSrc1.left, pSrc2.left);
	pDest.top    = MIN(pSrc1.top, pSrc2.top);
	pDest.right  = MAX(pSrc1.right, pSrc2.right);
	pDest.bottom = MAX(pSrc1.bottom, pSrc2.bottom);

	return !pDest.isEmpty();
}

/**
 * Update the screen from the screen surface buffer
 */
void ScreenSurface::update() { 
	RectList::iterator rOuter, rInner;

	// Merge any overlapping rectangles
	for (rOuter = _dirtyRects.begin(); rOuter != _dirtyRects.end(); ++rOuter) {
		rInner = rOuter;
		while (++rInner != _dirtyRects.end()) {

			if ((*rOuter).intersects(*rInner)) {
				// these two rectangles overlap, so translate it to a bigger rectangle
				// that contains both of them
				unionRectangle(*rOuter, *rOuter, *rInner);

				// remove the inner rect from the list
				_dirtyRects.erase(rInner);

				// move back to beginning of list
				rInner = rOuter;
			}
		}
	}

	// Copy any modified areas
	RectList::iterator dr;
	for (dr = _dirtyRects.begin(); dr != _dirtyRects.end(); ++dr) {
		Common::Rect &r = *dr;
		g_system->copyRectToScreen(getBasePtr(r.left, r.top), pitch, r.left, r.top, r.width(), r.height());
	}

	_dirtyRects.clear();

	// Allow the screen to update
	g_system->updateScreen();
}

void ScreenSurface::copyFrom(Graphics::Surface *src, const Common::Rect &srcBounds,
							 int destX, int destY, int transparentColour) {
	

	// Validation of the rectangle and position
	if ((destX >= width()) || (destY >= height()))
		return;

	Common::Rect copyRect = srcBounds;
	if (destX < 0) {
		copyRect.left += -destX;
		destX = 0;
	} else if (destX + copyRect.width() > width()) {
		copyRect.right -= destX + copyRect.width() - width();
	}
	if (destY < 0) {
		copyRect.top += -destY;
		destY = 0;
	} else if (destY + copyRect.height() > height()) {
		copyRect.bottom -= destY + copyRect.height() - height();
	}

	if (!copyRect.isValidRect())
		return;

	// Copy the specified area

	const byte *srcPtr = (const byte *)src->getBasePtr(copyRect.left, copyRect.top);
	byte *destPtr = getBasePtr(destX, destY);

	for (int rowCtr = 0; rowCtr < copyRect.height(); ++rowCtr) {
		if (transparentColour == -1)
			// No transparency, so copy line over
			Common::copy(srcPtr, srcPtr + copyRect.width(), destPtr);
		else {
			// Copy each byte one at a time checking for the transparency colour
			for (int xCtr = 0; xCtr < copyRect.width(); ++xCtr)
				if (srcPtr[xCtr] != transparentColour) destPtr[xCtr] = srcPtr[xCtr];
		}

		srcPtr += src->pitch;
		destPtr += pitch;
	}

	addDirtyRect(Common::Rect(destX, destY, destX + srcBounds.width(), destY + srcBounds.height()));
}

void ScreenSurface::fillRect(const Common::Rect &r, int colour) {
	Graphics::Surface::fillRect(r, colour);
	addDirtyRect(r);
}

void ScreenSurface::copyFrom(Graphics::Surface *src, int destX, int destY, int transparentColour) {
	copyFrom(src, Common::Rect(0, 0, src->w, src->h), destX, destY, transparentColour);
}

void ScreenSurface::setTextPosition(const Common::Point &pt) {
	TextSettings &ts = getTextSettings();

	if (ts.cursorOn)
		drawCursor(false);
	ts.textPosition = pt;
	if (ts.cursorOn)
		drawCursor(true);
}

void ScreenSurface::addChar(char c) {
	getTextSettings().pendingText += c;
}

void ScreenSurface::flushText(void) {
	if (getTextSettings().pendingText.size() > 0) {
		// Write the output string to the surface
		writeString(getTextSettings().pendingText);

		// Store the output string
		getTextSettings().lastOutput = getTextSettings().pendingText;
		getTextSettings().pendingText.clear();
	}
}

/**
 * Writes a string to the current position on the surface and updates the current position
 */
void ScreenSurface::writeString(const Common::String &s) {
	int strWidth = _textSettings.getStringWidth(s);
	writeString(s, getTextSettings().textPosition, strWidth);
}

void ScreenSurface::writeString(const Common::String &s, const Common::Point &r) {
	int strWidth = _textSettings.getStringWidth(s);
	writeString(s, r, strWidth);
}

void ScreenSurface::writeString(const Common::String &s, const Common::Point &r, int strWidth) {
	debugC(MESSAGE_BASIC, kGargoyleDebugScripts, "Message: %s", s.c_str());

	// Erase the background in the area the string will be written to
	Common::Rect textArea = Common::Rect(r.x, r.y, r.x + strWidth, r.y + _textSettings.getFontHeight());
	fillRect(textArea,  _textSettings.background());

	_textSettings.writeString(this, s, r, strWidth);

	if (_textSettings.textPosition == r)
		_textSettings.textPosition.x += _textSettings.getStringWidth(s);
}

void ScreenSurface::drawCursor(bool flag) {
 	const Common::Point &pt = getTextPosition();
	Common::Rect r = Common::Rect(pt.x, pt.y, pt.x + _textSettings.getCharWidth('0'),
		pt.y + _textSettings.getFontHeight());

	fillRect(r, flag ? _textSettings.foreground() : _textSettings.background());
}

void ScreenSurface::drawInput(char *buffer, uint pos, uint widthVal, bool cursor) {
	Common::Point &pt = _textSettings.textPosition;

	int fore = _textSettings.fore;
	int back = _textSettings.back;
	int style = _textSettings.getStyle();

	_textSettings.setStyle(style | FIXED_WIDTH_STYLE);

	if (!cursor) {
		// Cursor isn't turned on, so draw the string immediately
		writeString(buffer, pt, widthVal);
	} else {
		// Handle any part of the string before the cursor
		if (pos > 0) {
			// Draw any part of the line before the cursor
			Common::String s(buffer, pos);
			writeString(s, pt, widthVal);
		} else {
			fillRect(Common::Rect(pt.x, pt.y, pt.x + widthVal, pt.y + _textSettings.getFontHeight()),
				_textSettings.back);
		}

		// Handle drawing the cursor
		if (pos == strlen(buffer)) {
			// After end of text
			Common::Rect r = Common::Rect(pt.x, pt.y, pt.x + _textSettings.getCharWidth('X'), 
				pt.y + _textSettings.getFontHeight());
			fillRect(r,  _textSettings.fore);
		} else {
			// Within the text string
			Common::String s(*(buffer + pos));
			_textSettings.fore = back;
			_textSettings.back = fore;
			writeString(s, _textSettings.textPosition);

			_textSettings.fore = fore;
			_textSettings.back = back;
		}
		
		// Any remaining text
		if ((pos + 1) < strlen(buffer)) {
			Common::String s(buffer + pos + 1);
			writeString(s, _textSettings.textPosition);
		}
	}

	_textSettings.setStyle(style);
}


/**
 * Writes a specified character to the surface
 */
void ScreenSurface::writeChar(char c) {
	const Common::Point &pt = getTextPosition();

	// Erase the background of the cell to the current text background
	Common::Rect textArea = Common::Rect(pt.x, pt.y, pt.x + _textSettings.getCharWidth(c),
		pt.y + _textSettings.getFontHeight());
	fillRect(textArea, getTextSettings().back);

	// Write the character out
	getTextSettings().getFont()->drawChar(this, c, pt.x, pt.y, getTextSettings().fore);

	getTextSettings().textPosition.x += _textSettings.getFont()->getCharWidth(c);
}

/**
 * Scrolls the surface up by a specified amount. If none is provided, then the 
 * current font's height is used
 */
void ScreenSurface::scroll(int size) {
	if (size == -1)
		size = _textSettings.getFontHeight();

	scrollArea(Common::Rect(0, 0, width(), height()), size);
}

void ScreenSurface::nextLine() {
	int yPos = getTextPosition().y + _textSettings.getFontHeight();

	if (yPos <= height())
		setTextPosition(Common::Point(0, yPos));
	else
		scroll();
}

void ScreenSurface::scrollArea(const Common::Rect &r, int yAmount) {
	// Scroll the area
	if (yAmount < r.height()) {
		byte *pSrc = getBasePtr(r.left, r.top + yAmount);
		byte *pDest = getBasePtr(r.left, r.top);
		
		for (int y = 0; y < (r.height() - yAmount); ++y, pSrc += pitch, pDest += pitch)
			memcpy(pDest, pSrc, r.width());
	}

	// Fill the bottom area of the scroll region
	fillRect(Common::Rect(r.left, r.bottom - yAmount, r.right, r.bottom), 
		getTextSettings().back);

	addDirtyRect(r);
}

/**
 * Adds a rectangle to the dirty rects list
 */
void ScreenSurface::addDirtyRect(const Common::Rect &r) {
	_dirtyRects.push_back(r);
}

//--------------------------------------------------------------------------

} // End of namespace Gargoyle
