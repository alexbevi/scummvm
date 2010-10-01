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

#include "liath/game/segment.h"

#include "liath/helpers.h"
#include "liath/liath.h"
#include "liath/resource.h"

namespace Liath {

SegmentManager::SegmentManager(LiathEngine *engine) : _engine(engine) ,
	_gameSegment(NULL), _actSegment(NULL), _heroSegment(NULL), _exprSegment(NULL) {}

SegmentManager::~SegmentManager() {
	// Zero-out passed pointers
	_engine = NULL;

	unload(_gameSegment);
}

//////////////////////////////////////////////////////////////////////////
// Data
//////////////////////////////////////////////////////////////////////////
Common::String SegmentManager::load(SegmentType type, uint32 index) {
	Common::String name;
	switch (type) {
	default:
		error("SegmentManager::load: Invalid segment type!");

	case kSegmentGame:
		name = "game";
		break;

	case kSegmentAction:
		name = "act";
		break;

	case kSegmentHero:
		name = "hero";
		break;
	}

	Common::String filename = Common::String::printf("%s%04d.dat", name.c_str(), index);
	if (!getResource()->hasFile(filename))
		error("SegmentManager::load: File not found (%s)!", filename.c_str());

	// Load the file stream
	Common::SeekableReadStream *stream = getResource()->createReadStreamForMember(filename);
	if (!stream)
		error("SegmentManager::load: Invalid stream!");

	// Read signature/name
	char segmentName[9];
	stream->read(&segmentName, 9);

	// Read the number of segment data streams
	byte count = stream->readByte();

	// If less than 2 data streams, abort
	if (count < 2) {
		delete stream;
		error("SegmentManager::load: Invalid stream - less than 2 data streams (was: %d, min: 2)!", count);
	}

	Segment segment = new SegmentDef();

	// Push empty data segment
	segment->push_back(new SegmentData());

	// Read the data streams
	for (uint i = 1; i < count; i++) {
		// Read the data size
		size_t size = stream->readUint32LE();

		// Allocate memory
		void *data = calloc(size, 1);
		memset(data, 0, size);

		stream->read(data, size);

		// Store into the segment
		segment->push_back(new SegmentData());
		segment->back()->data = data;
		segment->back()->size = size;
	}

	*getSegmentByType(type) = segment;

	delete stream;

	return Common::String(segmentName);
}

//////////////////////////////////////////////////////////////////////////
// Helper functions
//////////////////////////////////////////////////////////////////////////

uint32 *SegmentManager::getData(SegmentType type, uint32 offset) {
	SegmentDef seg = **getSegmentByType(type);
	SegmentData *data = seg[1];

	if (data == NULL || data->data == NULL || data->size == 0)
		error("SegmentManager::getData: Invalid segment data (type: %d, offset: %d)", type, offset);

	if (offset >= data->size)
		error("SegmentManager::getData: Invalid offset (was: %d, max: %d)", offset, (uint32)data->size);

	return (uint32 *)((byte *)data->data + offset);
}

Segment *SegmentManager::getSegmentByType(SegmentType type) {
	switch (type) {
	default:
		error("SegmentManager::getSegmentByType: Invalid segment type!");

	case kSegmentGame:
		return &_gameSegment;

	case kSegmentAction:
		return &_actSegment;

	case kSegmentHero:
		return &_heroSegment;

	case kSegmentExpression:
		return &_exprSegment;
	}
}

/**
 * Unloads segment data
 *
 * @param segment The segment to unload
 */
void SegmentManager::unload(Segment segment) {
	SegmentDef seg = *segment;

	for (SegmentDef::iterator it = seg.begin(); it != seg.end(); it++)
		SAFE_DELETE(*it);

	seg.clear();
}

} // End of namespace Liath
