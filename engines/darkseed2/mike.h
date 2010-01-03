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

#include "common/frac.h"

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

	bool isBusy() const;

	bool isVisible();
	void setVisible(bool visible);

	void getPosition(uint32 &x, uint32 &y) const;
	void setPosition(uint32 x, uint32 y);

	frac_t getScale() const;

	frac_t calculateScale(uint32 y) const;

	Direction getDirection() const;
	void setDirection(Direction direction);

	/** Check for status changes. */
	void updateStatus();

	void setWalkMap();
	void setWalkMap(const Sprite &walkMap);

	void setScaleFactors(const int32 *scaleFactors);

	byte getWalkData(uint32 x, uint32 y) const;

	/** Walk to a specific position. */
	void go(uint32 x, uint32 y, Direction direction);

private:
	enum State {
		kStateIdle,
		kStateWalking,
		kStateTurning
	};

	enum AnimState {
		kAnimStateStanding = 0,
		kAnimStateWalking,
		kAnimStateNone
	};

	Resources *_resources;
	Variables *_variables;
	Graphics  *_graphics;

	/** Is Mike visible? */
	bool _visible;

	/** The current position's x coordinate. */
	uint32 _x;
	/** The current position's y coordinate. */
	uint32 _y;

	/** Our target's x coordinate. */
	uint32 _targetX;
	/** Our target's y coordinate. */
	uint32 _targetY;
	/** Our target's direction. */
	Direction _targetDirection;

	Direction _turnTo;

	byte _walkMap[kWalkMapWidth * kWalkMapHeight];

	int32 _scaleFactors[3];

	frac_t _scale;
	frac_t _scaleMin;
	frac_t _scaleMax;

	/** All animations. */
	Animation _animations[kAnimStateNone][kDirNone];

	/** The current animation state. */
	AnimState _animState;
	/** The current animation direction. */
	Direction _direction;

	/** The current state. */
	State _state;

	/** The reference to the sprite in the rendering queue. */
	Graphics::SpriteRef _spriteRef;

	/** Wait until that time stamp before the next movement can be performed. */
	uint32 _waitUntil;

	/** Load animations. */
	bool loadAnimations();

	inline static uint32 screenCoordToWalkCoord(uint32 walkCoord);
	inline byte getWalk(uint32 x, uint32 y) const;

	/** Update the scaling value based on the current y coordinate. */
	void updateScale();

	/** Update all animations' positions. */
	void updateAnimPositions();

	/** Update Mike's visibility status. */
	void updateVisible();

	/** Remove the current Mike sprite from the rendering queue. */
	void removeSprite();
	/** Add the current Mike sprite to the rendering queue. */
	void addSprite();

	/** Advance turn movements. */
	void advanceTurn();
	/** Advance walk movements. */
	void advanceWalk();

	/** How much will the next step move Mike forward in the X direction? */
	int32 getStepOffsetX() const;
	/** How much will the next step move Mike forward in the Y direction? */
	int32 getStepOffsetY() const;

	/** Calculate the direction direction between two points. */
	static Direction getDirection(uint32 x1, uint32 y1, uint32 x2, uint32 y2);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_MIKE_H
