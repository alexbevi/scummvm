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

#include "asylum/resources/polygons.h"

namespace Asylum {

static int32 comparePointOrientation(const Common::Point &lineStart, const Common::Point &lineEnd, const Common::Point &point) {
	int32 crossProduct = (lineEnd.x - lineStart.x) * (point.y - lineStart.y)
	                   - (lineEnd.y - lineStart.y) * (point.x - lineStart.x);

	return crossProduct <= 0 ? -1 : 1;
}

static bool segmentsIntersect(const Common::Point &line1Start, const Common::Point &line1End,
		const Common::Point &line2Start, const Common::Point &line2End) {
	return comparePointOrientation(line1Start, line1End, line2Start)
	           * comparePointOrientation(line1Start, line1End, line2End) < 1
	    && comparePointOrientation(line2Start, line2End, line1Start)
	           * comparePointOrientation(line2Start, line2End, line1End) < 1;
}

//////////////////////////////////////////////////////////////////////////
// Contains
//////////////////////////////////////////////////////////////////////////
bool Polygon::contains(const Common::Point &point) {
	// Sanitarium's IsPointInPolygon (sntrm.exe 0x0042FFC0) uses the
	// Win32 PtInRect convention, so the right and bottom edges are excluded.
	if (points.empty() || !boundingRect.contains(point))
		return false;

	Common::Point rayEnd(boundingRect.right + 5000, point.y);
	uint32 intersections = 0;

	for (uint32 i = 0; i < count(); i++) {
		if (segmentsIntersect(point, rayEnd, points[i], points[(i + 1) % count()]))
			intersections++;
	}

	return (intersections & 1) != 0;
}

//////////////////////////////////////////////////////////////////////////
// Polygons
//////////////////////////////////////////////////////////////////////////
Polygons::Polygons(Common::SeekableReadStream *stream) : _size(0), _numEntries(0) {
	load(stream);
}

Polygons::~Polygons() {
	_entries.clear();
}

Polygon Polygons::get(uint32 index) {
	if (index >= _entries.size())
		error("[Polygons::getEntry] Invalid polygon index (was: %d, max: %d)", index, _entries.size() - 1);

	return _entries[index];
}

void Polygons::load(Common::SeekableReadStream *stream) {
	_size       = stream->readSint32LE();
	_numEntries = stream->readSint32LE();

	for (int32 g = 0; g < _numEntries; g++) {
		Polygon poly;

		uint32 numPoints = stream->readUint32LE();

		for (uint32 i = 0; i < numPoints; i++) {
			Common::Point point;
			point.x = (int16)(stream->readSint32LE() & 0xFFFF);
			point.y = (int16)(stream->readSint32LE() & 0xFFFF);

			poly.points.push_back(point);
		}

		stream->skip((MAX_POLYGONS - numPoints) * 8);

		poly.boundingRect.left   = (int16)(stream->readSint32LE() & 0xFFFF);
		poly.boundingRect.top    = (int16)(stream->readSint32LE() & 0xFFFF);
		poly.boundingRect.right  = (int16)(stream->readSint32LE() & 0xFFFF);
		poly.boundingRect.bottom = (int16)(stream->readSint32LE() & 0xFFFF);

		_entries.push_back(poly);
	}
}

} // end of namespace Asylum
