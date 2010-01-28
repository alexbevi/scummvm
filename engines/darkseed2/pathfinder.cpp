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

#include "engines/darkseed2/pathfinder.h"
#include "engines/darkseed2/sprite.h"

namespace DarkSeed2 {

Position::Position(int32 pX, int32 pY) {
	x = pX;
	y = pY;
}

bool Position::isIn(int32 minX, int32 minY, int32 maxX, int32 maxY) const {
	return (x >= minX) && (y >= minY) && (x <= maxX) && (y <= maxY);
}

bool Position::operator==(const Position &right) const {
	return (x == right.x) && (y == right.y);
}


Pathfinder::Walkable::Walkable(int32 x, int32 y, byte v) {
	position.x = x;
	position.y = y;
	value      = v;
	lastCost   = 0xFFFFFFFF;
}

#define SQR(a) ((a) * (a))
int32 Pathfinder::Walkable::getDistanceValue(int32 x, int32 y) const {
	return SQR(position.x - x) + SQR(position.y - y);
}

int32 Pathfinder::Walkable::getDistanceValue(const Walkable right) const {
	return getDistanceValue(right.position.x, right.position.y);
}

int32 Pathfinder::Walkable::estimateDistance(const Walkable right) const {
	// We just estimate that we can always walk directly diagonal, then directly straight,
	// thus walking a length of tiles equal to the greatest coordinate.
	return MAX<int32>(ABS(position.x - right.position.x), ABS(position.y - right.position.y)) - 1;
}

bool Pathfinder::Walkable::operator==(const Walkable &right) const {
	return (value == right.value) && (position == right.position);
}


Pathfinder::Pathfinder(int32 width, int32 height) {
	// Sanity checks
	assert((width >= 0) && (height >= 0));

	_width  = width;
	_height = height;

	_tiles = new Walkable*[_width * _height];
	for (int32 i = 0; i < (_width * _height); i++)
		_tiles[i] = 0;

	_nodesVisited      = 0;
	// A limit that seems high enough for all Dark Seed II walk maps :P
	_nodesVisitedLimit = 3 * _width * _height;
	_abortSearch       = false;

	_topY =  0;
	_resY = 10;
}

Pathfinder::~Pathfinder() {
	clear();

	delete[] _tiles;
}

void Pathfinder::clear() {
	for (int32 i = 0; i < (_width * _height); i++) {
		delete _tiles[i];
		_tiles[i] = 0;
	}

	_topY =  0;
	_resY = 10;
}

void Pathfinder::setWalkMap(const Sprite &map, int32 topY, int32 resY) {
	if (!map.exists())
		return;

	clear();
	delete[] _tiles;

	_width  = map.getWidth();
	_height = map.getHeight();

	_topY = topY;
	_resY = resY;

	_tiles = new Walkable*[_width * _height];
	for (int32 i = 0; i < (_width * _height); i++)
		_tiles[i] = 0;

	const byte *mapData = map.getData();
	for (int32 y = 0; y < _height; y++)
		for (int32 x = 0; x < _width; x++)
			if (mapData[y * _width + x])
				_tiles[y * _width + x] = new Walkable(x, y, mapData[y * _width + x]);

	findNeighbours();
}

int32 Pathfinder::getXResolution() const {
	return 10;
}

int32 Pathfinder::getYResolution() const {
	return _resY;
}

void Pathfinder::findNeighbours() {
	// Find existing neighbours for each tile
	for (int32 y = 0; y < _height; y++) {
		for (int32 x = 0; x < _width; x++) {
			Walkable *tile = _tiles[y * _width + x];
			if (!tile)
				continue;

			// Positions of the neighbouring tiles. The straight neighbours first, so that
			// the pathfinding will favour straight lines over diagonals.
			Position neighbours[8] = {
				Position(x - 1, y    ), Position(x + 1, y    ),
				Position(x    , y - 1), Position(x    , y + 1),
				Position(x - 1, y - 1), Position(x + 1, y - 1),
				Position(x - 1, y + 1), Position(x + 1, y + 1)
			};

			// Go through the neighbouring tiles, look if they exists and add them to the neighbours list
			for (int i = 0; i < ARRAYSIZE(neighbours); i++) {
				if (neighbours[i].isIn(0, 0, _width - 1, _height - 1)) {
					Walkable *neighbour = _tiles[neighbours[i].y * _width + neighbours[i].x];
					if (neighbour)
						tile->neighbours.push_back(neighbour);
				}
			}

		}
	}
}

void Pathfinder::reset() {
	for (int32 i = 0; i < (_width * _height); i++)
		if (_tiles[i])
			_tiles[i]->lastCost = 0xFFFFFFFF;

	_nodesVisited = 0;
	_abortSearch  = false;
}

Pathfinder::Walkable *Pathfinder::findNearest(int32 x, int32 y) {
	int32 position = y * _width + x;

	// If a walkable tile in this position exists, return this
	if ((x >= 0) && (y >= 0) && (x < _width) && (y < _height))
		if (_tiles[position])
			return _tiles[position];

	// If not, go over the whole map, calculating the distance and return the one with the smallest one.
	Walkable *nearest = 0;
	int32 distance = 0x7FFFFFFF;
	for (int32 i = 0; i < (_width * _height); i++) {
		if (!_tiles[i])
			continue;

		int32 iDistance = _tiles[i]->getDistanceValue(x, y);
		if (iDistance < distance) {
			nearest = _tiles[i];
			distance = iDistance;
		}
	}

	return nearest;
}

Common::List<Position> Pathfinder::findPath(int32 x1, int32 y1, int32 x2, int32 y2) {
	Common::List<Position> pathPos;

	Walkable *start = 0;
	Walkable *end   = 0;

	int32 fromX = x1;
	int32 fromY = y1;
	int32 toX   = x2;
	int32 toY   = y2;

	convertToMapCoordinates(x1, y1);
	convertToMapCoordinates(x2, y2);

	// If the coordinates of either node are valid, look at the walk map
	if ((x1 >= 0) && (y1 >= 0) && (x1 < _width) && (y1 < _height))
		start = _tiles[y1 * _width + x1];
	if ((x2 >= 0) && (y2 >= 0) && (x2 < _width) && (y2 < _height))
		end   = _tiles[y2 * _width + x2];

	// If one of the nodest doesn't exist, try to find the nearest existent one
	if (!start)
		start = findNearest(x1, y1);
	if (!end)
		end   = findNearest(x2, y2);

	// If they still don't exist, no path is possible
	if (!start || !end)
		return pathPos;

	// Find the path
	Common::List<const Walkable *> path = findPathIDAStar(*start, *end);

	// Create a position list
	for (Common::List<const Walkable *>::iterator it = path.begin(); it != path.end(); ++it)
		pathPos.push_front(convertFromMapCoordinates((*it)->position));
	pathPos.push_front(pathPos.back());
	pathPos.pop_back();

	pathPos.push_front(Position(fromX, fromY));
	pathPos.push_back (Position(toX  , toY  ));

	simplifyPath(pathPos);

	return pathPos;
}

Common::List<const Pathfinder::Walkable *> Pathfinder::findPathIDAStar(Walkable &start, Walkable &end) {
	// Set the goal
	_goalNode = &end;

	// Estimate the lower cost limit
	uint32 costLimit = start.estimateDistance(end);

	Common::List<const Walkable *> path;

	bool finished = false;
	while (!finished) {
		// Clear cached information
		reset();

		// Reset the path
		path.clear();
		path.push_back(&start);

		if (DFS(0, start, costLimit, path))
			// Found path
			finished = true;

		if (costLimit == 0xFFFFFFFF) {
			// No path possible
			path.clear();
			finished = true;
		}

	}

	return path;
}

bool Pathfinder::DFS(uint32 cost, Walkable &node, uint32 &costLimit, Common::List<const Walkable *> &path) {
	// Did we reach our node visiting limit?
	if (++_nodesVisited > _nodesVisitedLimit) {
		// If yes, abort the search
		_abortSearch = true;
		return false;
	}

	// Estimate the current cost
	uint32 minCost = cost + node.estimateDistance(*_goalNode);

	if (minCost > costLimit) {
		// Reached the cost limit, push it further
		costLimit = minCost;
		return false;
	}

	if (node == *_goalNode)
		// Reached our goal
		return true;

	uint32 nextCostLimit = 0xFFFFFFFF;
	// Iterator over all neighbours
	for (Common::List<Walkable *>::iterator it = node.neighbours.begin(); it != node.neighbours.end(); ++it) {
		Walkable &neighbour = **it;
		uint32 newCost      = cost + 1;
		uint32 newCostLimit = costLimit;

		// If we already arrived at this node and the costs were lower, ignore the node
		if (newCost >= neighbour.lastCost)
			continue;

		// Assign the cached node cost
		neighbour.lastCost = newCost;

		// Try to continue that path
		if (DFS(newCost, neighbour, newCostLimit, path)) {
			// Yup, found correct path

			path.push_back(&neighbour);
			costLimit = newCostLimit;
			return true;
		}

		if (_abortSearch) {
			costLimit = 0xFFFFFFFF;
			return false;
		}

		// Update our cost limit
		nextCostLimit = MIN(nextCostLimit, newCostLimit);
	}

	costLimit = nextCostLimit;
	return false;
}

bool Pathfinder::isSameTile(const Common::List<Position>::iterator &a,
		const Common::List<Position>::iterator &b) const {

	int inX = ABS(a->x - b->x);
	int inY = ABS(a->y - b->y);

	return (inX < 10) && (inY < _resY);
}

void Pathfinder::simplifyPath(Common::List<Position> &path) const {
	Common::List<Position>::iterator first, second, third;

	// Look if the start nodes are on the same tile and remove the inner one then
	first = path.begin();
	second = first;
	second++;

	while ((first != path.end()) && (second != path.end()) && isSameTile(first, second))
		second = path.erase(second);

	// Look if the end nodes are on the same tile and remove the inner one then
	first = path.end();
	first--;
	second = first;
	second--;

	while ((first  != path.end()) && (first  != path.begin()) &&
	       (second != path.end()) && (second != path.begin()) &&
	       isSameTile(first, second)) {
		second = path.erase(second);
		second--;
	}

	// Remove not needed nodes
	first = path.begin();

	second = first;
	second++;

	third = second;
	third++;

	while ((first != path.end()) && (second != path.end()) && (third != path.end())) {
		if (!isStraightLine(first, second, third)) {
			first++;
			second++;
			third++;
		} else
			removeMiddleman(path, first, second, third);
	}

}

bool Pathfinder::isStraightLine(const Common::List<Position>::iterator &a,
		const Common::List<Position>::iterator &b, const Common::List<Position>::iterator &c) {

	// Straight horizontal
	if ((a->x == b->x) && (a->x == c->x))
		return true;

	// Straight vertical
	if ((a->y == b->y) && (a->y == c->y))
		return true;

	int32 dx1 = b->x - a->x;
	int32 dx2 = c->x - b->x;
	int32 dy1 = b->y - a->y;
	int32 dy2 = c->y - b->y;

	// Diagonal
	if ((ABS(dx1) == ABS(dy1)) && (ABS(dx2) == ABS(dy2)))
		return true;

	return false;
}

void Pathfinder::removeMiddleman(Common::List<Position> &list,
		Common::List<Position>::iterator &a, Common::List<Position>::iterator &b,
		Common::List<Position>::iterator &c) {

	// Remove the node
	list.erase(b);

	// Set the iterator to the next three positions
	b = c;
	c++;
}

void Pathfinder::convertToMapCoordinates(int32 &x, int32 &y) const {
	x /= 10;
	y = (y - _topY) / _resY;
}

Position Pathfinder::convertFromMapCoordinates(const Position position) const {
	return Position(position.x * 10, (position.y * _resY) + _topY);
}

} // End of namespace DarkSeed2
