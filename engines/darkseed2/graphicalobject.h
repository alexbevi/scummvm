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

#ifndef DARKSEED2_GRAPHICALOBJECT_H
#define DARKSEED2_GRAPHICALOBJECT_H

#include "common/rect.h"
#include "common/array.h"
#include "common/frac.h"

#include "engines/darkseed2/darkseed2.h"

namespace Common {
	class String;
}

namespace DarkSeed2 {

class Sprite;

/** Base class for a graphical object. */
class GraphicalObject {
public:
	GraphicalObject();
	virtual ~GraphicalObject();

	/** Get the object's area. */
	Common::Rect getArea() const;

	/** Move the object. */
	virtual void moveTo(int32 x, int32 y);

	/** Move the object relative to its current position. */
	virtual void move(int32 x, int32 y);

	/** Redraw the object. */
	virtual void redraw(Sprite &sprite, Common::Rect area) = 0;

protected:
	Common::Rect _area; ///< The object's area.

	/** Clear the object's area. */
	void clearArea();
};

/** A text graphic. */
class TextObject : public GraphicalObject {
public:
	TextObject(const Common::String &text, int32 x, int32 y,
			uint32 color, int32 maxWidth = 0);
	~TextObject();

	/** Redraw the object. */
	void redraw(Sprite &sprite, Common::Rect area);

	/** Recolor the text. */
	void recolor(byte color);

	/** Create a wrapped StringList out of the supplied string. */
	static int32 wrap(const Common::String &string, Common::StringList &list, int32 maxWidth);

private:
	Sprite *_sprite; ///< The text's sprite.
};

/** A simple sprite object. */
class SpriteObject : public GraphicalObject {
public:
	SpriteObject();
	~SpriteObject();

	/** Move the object to its default coordinates. */
	void moveTo();
	/** Move the object. */
	void moveTo(int32 x, int32 y);
	/** Move the object relative to its feet. */
	void moveFeetTo(int32 x, int32 y);

	/** Move the object relative to its current position. */
	void move(int32 x, int32 y);

	/** Clear the sprite. */
	void clear();

	/** Is the sprite empty? */
	bool empty() const;
	/** Are the coordinates within the sprite? */
	bool isIn(int32 x, int32 y) const;

	/** Return the sprite's x coordinate. */
	int32 getX() const;
	/** Return the sprite's y coordinate. */
	int32 getY() const;

	/** Return the sprite's feet's x coordinate. */
	int32 getFeetX() const;
	/** Return the sprite's feet's y coordinate. */
	int32 getFeetY() const;

	/** Calculate the corresponding scale value. */
	frac_t calculateScaleVal(int32 height);

	/** Get the sprite's scaling value. */
	frac_t getScale() const;
	/** Set the sprite's scaling value. */
	void setScale(frac_t scale);

	/** Return the sprite. */
	Sprite &getSprite();
	/** Return the sprite. */
	const Sprite &getSprite() const;

	/** Load the sprite from a BMP. */
	bool loadFromImage(Resources &resources, const Common::String &image);

	/** Redraw the object. */
	void redraw(Sprite &sprite, Common::Rect area);

private:
	Sprite *_sprite; ///< The sprite.
};

/** An animation consisting of several sprite objects. */
class Animation {
public:
	Animation();
	~Animation();

	/** Clear the animation. */
	void clear();

	/** Is the animation empty? */
	bool empty() const;
	/** Return the number of sprites contained in this animation. */
	int frameCount() const;
	/** Get the current frame number. */
	int currentFrame() const;

	/** Is the animation visible? */
	bool isVisible() const;
	/** Set the visibility of the animations. */
	void setVisible(bool visible);

	/** Move the animation to its default coordinates. */
	void moveTo();
	/** Move the animation. */
	void moveTo(int32 x, int32 y);
	/** Move the animation relative to its feet. */
	void moveFeetTo(int32 x, int32 y);

	/** Move the animation relative to its current position. */
	void move(int32 x, int32 y);

	/** Calculate the corresponding scale value. */
	frac_t calculateScaleVal(int frame, int32 height);

	/** Set the sprite's scaling value. */
	void setScale(frac_t scale);

	/** Set the current frame. */
	void setFrame(int frame);
	/** Advance the animation to the next frame. */
	void nextFrame();
	/** Roll back the animation to the previous frame. */
	void previousFrame();

	/** Advance the animation to the next frame. */
	void operator++(int);
	/** Roll back the animation to the previous frame. */
	void operator--(int);

	/** Return the animation's name. */
	const Common::String &getName() const;

	/** Load the animation from files. */
	bool load(Resources &resources, const Common::String &base);

	/** Flip the animation's sprites horizontally. */
	void flipHorizontally();
	/** Flip the animation's sprites vertically. */
	void flipVertically();

	/** Get the nth sprite. */
	SpriteObject &getFrame(int n);
	/** Get the nth sprite. */
	const SpriteObject &getFrame(int n) const;

	/** Get the nth sprite. */
	SpriteObject &operator[](int n);
	/** Get the nth sprite. */
	const SpriteObject &operator[](int n) const;

	/** Get the current frame. */
	SpriteObject &getCurrentFrame();
	/** Get the current frame. */
	const SpriteObject &getCurrentFrame() const;

	/** Get the current frame. */
	SpriteObject &operator*();
	/** Get the current frame. */
	const SpriteObject &operator*() const;

	/** Get the current frame. */
	SpriteObject *operator->();
	/** Get the current frame. */
	const SpriteObject *operator->() const;

private:
	/** The sprites. */
	Common::Array<SpriteObject *> _frames;
	Common::Array<SpriteObject *> _sprites;

	/** The animation's name. */
	Common::String _name;

	/** Is the animation visible? */
	bool _visible;

	/** The current frame. */
	int _curFrame;
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_GRAPHICALOBJECT_H
