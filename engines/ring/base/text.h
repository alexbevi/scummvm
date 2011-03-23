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

#ifndef RING_TEXT_H
#define RING_TEXT_H

#include "ring/shared.h"

namespace Ring {

class Application;
class ObjectPresentation;

class Text : public Common::Serializable {
public:
	Text();
	~Text();

	void init(Common::String text, const Common::Point &point, FontId fontId, Color foreground, Color background);
	void set(Common::String text);
	Common::Rect getBoundingBox();

	// Accessors
	void setObjectPresentation(ObjectPresentation *objectPresentation) { _objectPresentation = objectPresentation; }
	void setFontId(FontId fontId) { _fontId = fontId; }
	void setForegroundColor(Color foreground);
	void setBackgroundColor(Color background);
	void setCoordinates(Common::Point point) { _point = point; }

	Common::String getString() { return _text; }
	Common::Point getCoordinates() { return _point; }
	FontId getFontId() { return _fontId; }
	uint32 getForegroundColor() { return _foregroundColor; }
	uint32 getWidth() { return _width; }
	uint32 getHeight() { return _height; }
	bool hasTransparentBackground() { return _transparentBackground; }
	uint32 getBackgroundColor() { return _backgroundColor; }
	ObjectPresentation *getPresentation() { return _objectPresentation; }

	// Serializable
	void saveLoadWithSerializer(Common::Serializer &s);

private:
	// Data
	Common::String _text;
	FontId _fontId;
	Common::Point _point;
	uint32 _foregroundColor;
	uint32 _width;
	uint32 _height;
	bool _transparentBackground;
	uint32 _backgroundColor;
	ObjectPresentation *_objectPresentation;
};

} // End of namespace Ring

#endif // RING_TEXT_H
