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

/*
 * Implementing the IDA* path finding algorithm,
 * as described in the respective Wikipedia article.
 */

#ifndef DARKSEED2_PATHFINDER_H
#define DARKSEED2_PATHFINDER_H

#include "common/list.h"

#include "engines/darkseed2/darkseed2.h"

namespace DarkSeed2 {

/** A position within a walk map. */
struct Position {
	int32 x; ///< X coordinate.
	int32 y; ///< Y coordinate.

	Position(int32 pX = 0, int32 pY = 0);

	/** Is the position within a given range? */
	bool isIn(int32 minX, int32 minY, int32 maxX, int32 maxY) const;

	/** Compare two postions. */
	bool operator==(const Position &right) const;
};

/** A path finding class implementing the IDA* algorithm. */
class Pathfinder {
public:
	Pathfinder(int32 width, int32 height);
	~Pathfinder();

	/** Clear the pathfinder's walk map. */
	void clear();

	/** Set the walk map. */
	void setWalkMap(const byte *map);

	/** Find a path between two positions. */
	Common::List<Position> findPath(int32 x1, int32 y1, int32 x2, int32 y2);

private:
	struct Walkable {
		Position position; ///< The position of the walkable tile.

		byte value; ///< The value of the tile, its type/properties.

		uint32 lastCost; ///< The last path cost to that tile.

		Common::List<Walkable *> neighbours; ///< The neighbouring tiles.

		Walkable(int32 x, int32 y, byte v);

		/** Return a comparable distance value. */
		int32 getDistanceValue(int32 x, int32 y) const;
		/** Return a comparable distance value. */
		int32 getDistanceValue(const Walkable right) const;
		/** Estimate the real distance. */
		int32 estimateDistance(const Walkable right) const;

		bool operator==(const Walkable &right) const;
	};

	int32 _width;  ///< The walk map's width.
	int32 _height; ///< The walk map's height.

	/** The complete walk map. */
	Walkable **_tiles;

	// Temporaries for a path search
	Walkable *_goalNode;       ///< Our current goal.
	uint32 _nodesVisited;      ///< Number of nodes visited during the search.
	uint32 _nodesVisitedLimit; ///< A limit on the visited nodes.
	bool   _abortSearch;       ///< Should we abort the current search?

	/** Build the neighbour list for each tile. */
	void findNeighbours();

	/** Reset temporary information. */
	void reset();

	/** Find the nearest walkable tile to a given position. */
	Walkable *findNearest(int32 x, int32 y);

	/** Find a path between two nodes using the IDA* search algorithm. */
	Common::List<const Walkable *> findPathIDAStar(Walkable &start, Walkable &end);
	/** Recursively called depth-first search method. */
	bool DFS(uint32 cost, Walkable &node, uint32 &costLimit, Common::List<const Walkable *> &path);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_PATHFINDER_H
