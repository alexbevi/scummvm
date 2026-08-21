/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef ASYLUM_RESOURCES_POLYGONS_H
#define ASYLUM_RESOURCES_POLYGONS_H

#include "common/array.h"
#include "common/rect.h"
#include "common/stream.h"

#define MAX_POLYGONS 200

namespace Asylum {

class Polygon {
public:
	Common::Array<Common::Point> points;
	Common::Rect  boundingRect;

	Polygon() {};
	Polygon(Common::Point point1, Common::Point point2, Common::Point point3, Common::Point point4) {
		points.push_back(point1);
		points.push_back(point2);
		points.push_back(point3);
		points.push_back(point4);

		boundingRect.left = boundingRect.right = point1.x;
		boundingRect.top = boundingRect.bottom = point1.y;
		for (uint32 i = 1; i < points.size(); i++) {
			if (points[i].x < boundingRect.left)
				boundingRect.left = points[i].x;
			if (points[i].x > boundingRect.right)
				boundingRect.right = points[i].x;
			if (points[i].y < boundingRect.top)
				boundingRect.top = points[i].y;
			if (points[i].y > boundingRect.bottom)
				boundingRect.bottom = points[i].y;
		}
	}

	bool contains(const Common::Point &point);
	uint32 count() { return points.size(); }
};

class Polygons {
public:
	Polygons(Common::SeekableReadStream *stream);
	virtual ~Polygons();

	Polygon get(uint32 index);
	uint32 size() { return _entries.size(); }

private:
	int32 _size;
	int32 _numEntries;

	Common::Array<Polygon> _entries;

	void load(Common::SeekableReadStream *stream);
}; // end of class Polygons

} // end of namespace Asylum

#endif // ASYLUM_RESOURCES_POLYGONS_H
