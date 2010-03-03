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
#include "engines/darkseed2/pathfinder.h"
#include "engines/darkseed2/saveable.h"

namespace Common {
	class String;
}

namespace DarkSeed2 {

class Resources;
class Variables;

class Sprite;

class Mike : public Saveable {
public:
	/** A direction. */
	enum Direction {
		kDirN    = 0, ///< North.
		kDirNE   = 1, ///< North-East.
		kDirE    = 2, ///< East.
		kDirSE   = 3, ///< South-East.
		kDirS    = 4, ///< South.
		kDirSW   = 5, ///< South-West.
		kDirW    = 6, ///< West.
		kDirNW   = 7, ///< North-West.
		kDirNone = 8  ///< No direction.
	};

	Mike(Resources &resources, Variables &variables, Graphics &graphics);
	~Mike();

	/** Initialize Mike. */
	bool init(bool needPalette);

	/** Is Mike busy (not idly standing around)? */
	bool isBusy() const;

	/** Is Mike visible? */
	bool isVisible();
	/** Set Mike visibility. */
	void setVisible(bool visible);

	/** Set Mike's position. */
	void getPosition(int32 &x, int32 &y) const;
	/** Get Mike's position. */
	void setPosition(int32 x, int32 y);

	/** Get Mike's current scaling value. */
	frac_t getScale() const;

	/** Calculate the scaling value of an object in the given y coordinate with the current scaling factors. */
	frac_t calculateScale(int32 y) const;

	/** Get Mike's current direction. */
	Direction getDirection() const;
	/** Set Mike's current direction. */
	void setDirection(Direction direction);

	/** Check for status changes. */
	void updateStatus();

	/** Reset the walk map. */
	void setWalkMap();
	/** Set the walk map. */
	void setWalkMap(const Sprite &walkMap, int32 arg1, int32 arg2);

	/** Set the scaling factors. */
	void setScaleFactors(const int32 *scaleFactors);

	/** Get the walk map data in that coordinate. */
	byte getWalkData(int32 x, int32 y) const;

	/** Walk to a specific position. */
	void go(int32 x, int32 y, Direction direction);

protected:
	bool saveLoad(Common::Serializer &serializer, Resources &resources);
	bool loading(Resources &resources);

private:
	/** Mike's state. */
	enum State {
		kStateIdle    = 0, ///< Idling standing by.
		kStateWalking = 1, ///< Walking somewhere.
		kStateTurning = 2  ///< Turning around.
	};

	/** The animation state Mike's currently in. */
	enum AnimState {
		kAnimStateStanding = 0, ///< Standing.
		kAnimStateWalking  = 1, ///< Walking.
		kAnimStateNone     = 2  ///< No/Invalid state.
	};

	Resources *_resources;
	Variables *_variables;
	Graphics  *_graphics;

	Pathfinder *_pathfinder;

	/** Is Mike visible? */
	bool _visible;

	// Current position
	int32    _x;          ///< Mike's current x position.
	int32    _y;          ///< Mike's current y position.
	Direction _direction; ///< Mike's current direction.

	// Targeting
	int32    _targetX;          ///< Our target's x coordinate
	int32    _targetY;          ///< Our target's y coordinate.
	Direction _targetDirection; ///< Our target's direction.
	Common::List<Position> _wayPoints;
	Common::List<Position>::const_iterator _currentWayPoint;
	uint32 _currentWayPointNumber;

	/** The direction to turn to. */
	Direction _turnTo;

	int32  _scaleFactors[3]; ///< The scaling factors.
	frac_t _scale;           ///< The current scaling value.
	frac_t _scaleMin;        ///< Minimal scaling.
	frac_t _scaleMax;        ///< Maximal scaling.

	/** All animations. */
	Animation _animations[kAnimStateNone][kDirNone];

	State     _state;     ///< The current state. */
	AnimState _animState; ///< The current animation state. */

	/** The reference to the sprite in the rendering queue. */
	Graphics::SpriteRef _spriteRef;

	/** Wait until that time stamp before the next movement can be performed. */
	uint32 _waitUntil;

	/** Load animations. */
	bool loadAnimations(bool needPalette);

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
	static Direction getDirection(int32 x1, int32 y1, int32 x2, int32 y2);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_MIKE_H
