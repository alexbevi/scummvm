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

#include "engines/darkseed2/neresources.h"

namespace DarkSeed2 {

NECursor::NECursor() {
	width    = 0;
	height   = 0;
	hotspotX = 0;
	hotspotY = 0;
	dataSize = 0;
	data     = 0;
}

NECursor::~NECursor() {
	delete[] data;
}


NEResources::NEResources() {
}

NEResources::~NEResources() {
}

void NEResources::clear() {
	_fileName.clear();
	_exe.close();

	_resources.clear();
	_cursors.clear();
}

const Common::Array<NECursorGroup> &NEResources::getCursors() const {
	return _cursors;
}

bool NEResources::loadFromEXE(const Common::String &fileName) {
	clear();

	_fileName = fileName;

	if (!tryOpen())
		return false;

	uint32 offsetResourceTable = getResourceTableOffset();
	if (offsetResourceTable == 0xFFFFFFFF)
		return false;
	if (offsetResourceTable == 0)
		return true;

	if (!readResourceTable(offsetResourceTable))
		return false;

	if (!readCursors())
		return false;

	close();

	return true;
}

bool NEResources::tryOpen() {
	if (_fileName.empty())
		return false;

	if (_exe.isOpen())
		return true;

	return _exe.open(_fileName);
}

void NEResources::close() {
	_exe.close();
}

uint32 NEResources::getResourceTableOffset() {
	if (!tryOpen())
		return 0xFFFFFFFF;

	if (!_exe.seek(0))
		return 0xFFFFFFFF;

	//                          'MZ'
	if (_exe.readUint16BE() != 0x4D5A)
		return 0xFFFFFFFF;

	if (!_exe.seek(60))
		return 0xFFFFFFFF;

	uint32 offsetSegmentEXE = _exe.readUint16LE();
	if (!_exe.seek(offsetSegmentEXE))
		return 0xFFFFFFFF;

	//                          'NE'
	if (_exe.readUint16BE() != 0x4E45)
		return 0xFFFFFFFF;

	if (!_exe.seek(offsetSegmentEXE + 36))
		return 0xFFFFFFFF;

	uint32 offsetResourceTable = _exe.readUint16LE();
	if (offsetResourceTable == 0)
		// No resource table
		return 0;

	// Offset relative to the segment _exe header
	offsetResourceTable += offsetSegmentEXE;
	if (!_exe.seek(offsetResourceTable))
		return 0xFFFFFFFF;

	return offsetResourceTable;
}

bool NEResources::readResourceTable(uint32 offset) {
	if (!tryOpen())
		return false;

	if (!_exe.seek(offset))
		return false;

	uint32 align = 1 << _exe.readUint16LE();

	uint16 typeID = _exe.readUint16LE();
	while (typeID != 0) {
		uint16 resCount = _exe.readUint16LE();

		_exe.skip(4); // reserved

		for (int i = 0; i < resCount; i++) {
			_resources.push_back(Resource());

			Resource &res = _resources.back();

			res.offset = _exe.readUint16LE() * align;
			res.size   = _exe.readUint16LE() * align;
			res.flags  = _exe.readUint16LE();
			res.id     = _exe.readUint16LE();
			res.handle = _exe.readUint16LE();
			res.usage  = _exe.readUint16LE();

			res.type = typeID;

			if ((res.id & 0x8000) == 0) {
				// String name

				res.idType = kIDTypeString;
				res.name   = getResourceString(_exe, offset + res.id);
				res.id     = 0;

			} else {
				res.idType = kIDTypeNumerical;
				res.id    &= 0x7FFF;
			}

		}

		typeID = _exe.readUint16LE();
	}

	return true;
}

Common::String NEResources::getResourceString(Common::SeekableReadStream &exe, uint32 offset) {
	uint32 curPos = exe.pos();

	if (!exe.seek(offset)) {
		exe.seek(curPos);
		return "";
	}

	uint8 length = exe.readByte();

	Common::String string;
	for (uint16 i = 0; i < length; i++)
		string += (char) exe.readByte();

	exe.seek(curPos);
	return string;
}

const NEResources::Resource *NEResources::findResource(uint16 type, uint16 id) const {
	for (Common::List<Resource>::const_iterator it = _resources.begin(); it != _resources.end(); ++it)
		if ((it->type == type) && (it->idType == kIDTypeNumerical) && (it->id == id))
			return &*it;

	return 0;
}

bool NEResources::readCursors() {
	uint32 cursorCount;

	for (Common::List<Resource>::const_iterator it = _resources.begin(); it != _resources.end(); ++it)
		if ((it->type == 0x800C) && (it->idType == kIDTypeString))
			cursorCount++;

	_cursors.resize(cursorCount);

	Common::Array<NECursorGroup>::iterator cursorGroup = _cursors.begin();
	for (Common::List<Resource>::const_iterator it = _resources.begin(); it != _resources.end(); ++it) {
		if ((it->type == 0x800C) && (it->idType == kIDTypeString)) {
			if (!readCursorGroup(*cursorGroup, *it))
				return false;

			++cursorGroup;
		}
	}

	return true;
}

bool NEResources::readCursorGroup(NECursorGroup &group, const Resource &resource) {
	if (!tryOpen())
		return false;

	if (resource.size <= 6)
		return false;

	if (!_exe.seek(resource.offset))
		return false;

	byte *data = new byte[resource.size];

	if (!_exe.read(data, resource.size)) {
		delete[] data;
		return false;
	}

	uint32 cursorCount = READ_LE_UINT16(data + 4);
	if (resource.size < (6 + cursorCount * 16)) {
		delete[] data;
		return false;
	}

	group.cursors.resize(cursorCount);

	uint32 offset = 6;
	for (uint32 i = 0; i < cursorCount; i++) {
		NECursor &cursor = group.cursors[i];

		// Plane count
		if (READ_LE_UINT16(data + offset + 4) != 1) {
			delete[] data;
			return false;
		}

		// Bit count
		if (READ_LE_UINT16(data + offset + 6) != 1) {
			delete[] data;
			return false;
		}

		uint32 id = READ_LE_UINT32(data + offset + 12);
		const Resource *cursorResource = findResource(0x8001, id);
		if (!cursorResource) {
			delete[] data;
			return false;
		}

		cursor.width    = READ_LE_UINT16(data + offset +  0);
		cursor.height   = READ_LE_UINT16(data + offset +  2);
		cursor.dataSize = READ_LE_UINT32(data + offset +  8);

		if (!readCursor(cursor, *cursorResource)) {
			delete[] data;
			return false;
		}

		offset += 16;
	}

	group.name = resource.name;

	return true;
}

bool NEResources::readCursor(NECursor &cursor, const Resource &resource) {
	if (!tryOpen())
		return false;

	if (cursor.dataSize <= 4)
		return false;
	if (resource.size < cursor.dataSize)
		return false;

	if (!_exe.seek(resource.offset))
		return false;

	cursor.hotspotX = _exe.readUint16LE();
	cursor.hotspotY = _exe.readUint16LE();

	cursor.dataSize -= 4;

	cursor.data = new byte[cursor.dataSize];
	if (!_exe.read(cursor.data, cursor.dataSize))
		return false;

	return true;
}

} // End of namespace DarkSeed2
