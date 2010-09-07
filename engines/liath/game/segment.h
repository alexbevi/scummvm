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

#ifndef LIATH_SEGMENT_H
#define LIATH_SEGMENT_H

#include "liath/shared.h"

#include "common/array.h"

namespace Liath {

class LiathEngine;

class SegmentManager {
public:
	SegmentManager(LiathEngine *engine);
	~SegmentManager();

	Common::String load(SegmentType type, uint32 index);
	void unload(Segment segment);
	void unload(SegmentType type) { unload(*getSegmentByType(type)); }

	// Accessors
	uint32 *getData(SegmentType type, uint32 offset);

	Segment get(SegmentType type) { return *getSegmentByType(type); }
	void set(SegmentType type, Segment segment) { *getSegmentByType(type) = segment; }

private:
	LiathEngine* _engine;

	// Segments
	Segment _gameSegment;
	Segment _actSegment;
	Segment _heroSegment;
	Segment _exprSegment;

	Segment *getSegmentByType(SegmentType type);
};

} // End of namespace Liath

#endif // LIATH_SEGMENT_H
