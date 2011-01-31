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

#ifndef RING_CURSOR_H
#define RING_CURSOR_H

#include "ring/shared.h"

namespace Ring {

class CursorBase : public BaseObject {
public:
	CursorBase();
	~CursorBase();

	virtual void init(uint32 a1, Common::String name, uint32 a3, byte a4);
	virtual void deinit() = 0;

private:
	uint32 _field_4;
	Common::String _name;
	uint32 _field_C;
	uint32 _field_10;
	uint32 _field_14;
	byte _field_18;
};

class Cursor : public CursorBase {
public:
	Cursor();
	~Cursor();

	virtual void init(uint32 a1, Common::String name, uint32 a3, byte a4);
	virtual void deinit();

private:
	uint32 _field_19; // HCURSOR
	bool _isDefaultCursor;

	void load();
};

class CursorHandler {
public:
	CursorHandler();
	~CursorHandler();

	void add(CursorId id, Common::String name, CursorType cursorType, uint32 a3, uint32 a4, ArchiveType archiveType);
	void add(CursorId id, Common::String name, CursorType cursorType, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7, ArchiveType archiveType);
	void setOffset(CursorId id, Common::Point offset);
	void select(CursorId id);

private:
	AssociativeArray<CursorBase *> _cursors;
	uint32 _index;
};

} // End of namespace Ring

#endif // RING_CURSOR_H
