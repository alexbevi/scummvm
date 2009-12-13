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

#ifndef DARKSEED2_MIKE_H
#define DARKSEED2_MIKE_H

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/graphics.h"
#include "engines/darkseed2/graphicalobject.h"

namespace Common {
	class String;
}

namespace DarkSeed2 {

class Resources;
class Variables;

class Sprite;

class Mike {
public:
	static const uint32 kWalkMapResolution = 10;
	static const uint32 kWalkMapWidth      = Graphics::kScreenWidth  / kWalkMapResolution;
	static const uint32 kWalkMapHeight     = Graphics::kScreenHeight / kWalkMapResolution;

	enum Direction {
		kDirN = 0,
		kDirNE,
		kDirE,
		kDirSE,
		kDirS,
		kDirSW,
		kDirW,
		kDirNW,
		kDirNone
	};

	Mike(Resources &resources, Variables &variables, Graphics &graphics);
	~Mike();

	Common::Rect getArea() const;

	bool init();

	bool isVisible();
	void setVisible(bool visible);

	void getPosition(uint32 &x, uint32 &y) const;
	void setPosition(uint32 x, uint32 y);

	Direction getDirection() const;
	void setDirection(Direction direction);

	/** Check for status changes. */
	void updateStatus();

	void redraw(Sprite &sprite, Common::Rect area);

	void setWalkMap();
	void setWalkMap(const Sprite &walkMap);

	byte getWalkData(uint32 x, uint32 y) const;

private:
	enum State {
		kStateStanding = 0,
		kStateWalking,
		kStateNone
	};

	Resources *_resources;
	Variables *_variables;
	Graphics  *_graphics;

	bool _visible;

	uint32 _x;
	uint32 _y;

	byte _walkMap[kWalkMapWidth * kWalkMapHeight];

	Animation _animations[kStateNone][kDirNone];

	State _state;
	Direction _direction;

	bool loadAnimations();

	inline static uint32 screenCoordToWalkCoord(uint32 walkCoord);
	inline byte getWalk(uint32 x, uint32 y) const;

	void updateVisible();
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_MIKE_H
