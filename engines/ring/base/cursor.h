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

class AnimationImage;
class Image;

//////////////////////////////////////////////////////////////////////////
class CursorBase : public BaseObject {
public:
	CursorBase();
	~CursorBase();

	virtual void init(CursorId id, Common::String name, CursorType cursorType, byte a4);
	virtual void deinit() = 0;
	virtual void alloc() = 0;
	virtual void dealloc() = 0;
	virtual void draw() {}

	// Accessors
	void setName(Common::String name) { _name = name; }
	Common::String getName() { return _name; }

	void setOffset(Common::Point offset) { _offset = offset; }

	void setType(CursorType type) { _type = type; }
	CursorType getType() { return _type; }

private:
	Common::String _name;
	CursorType _type;
	Common::Point _offset;
	byte _field_18;
};

//////////////////////////////////////////////////////////////////////////
class Cursor : public CursorBase {
public:
	Cursor();
	~Cursor();

	virtual void init(CursorId id, Common::String name, CursorType cursorType, byte a4);
	virtual void deinit();
	virtual void alloc();
	virtual void dealloc();

private:
	uint32 _field_19; // HCURSOR
	bool _isDefaultCursor;

	void load();
};

//////////////////////////////////////////////////////////////////////////
class CursorImage : public CursorBase {
public:
	CursorImage();
	~CursorImage();

	void init(CursorId id, Common::String name, CursorType cursorType, byte a4, ArchiveType archiveType);
	virtual void deinit();
	virtual void alloc();
	virtual void dealloc();

private:
	Image *_image;
	ArchiveType _archiveType;
};

//////////////////////////////////////////////////////////////////////////
class CursorAnimation : public CursorBase {
public:
	CursorAnimation();
	~CursorAnimation();

	void init(CursorId id, Common::String name, CursorType cursorType, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7, ArchiveType archiveType);
	virtual void deinit();
	virtual void alloc();
	virtual void dealloc();
	virtual void draw();

private:
	AnimationImage *_image;
};

//////////////////////////////////////////////////////////////////////////
class CursorHandler {
public:
	CursorHandler();
	~CursorHandler();

	void add(CursorId id, Common::String name, CursorType cursorType, uint32 a3, ImageType imageType, ArchiveType archiveType);
	void add(CursorId id, Common::String name, CursorType cursorType, uint32 a3, uint32 a4, uint32 a5, uint32 a6, ImageType imageType, ArchiveType archiveType);
	void removeByType();

	void draw();
	void select(CursorId id);
	void setOffset(CursorId id, Common::Point offset);
	CursorType getType();

private:
	AssociativeArray<CursorBase *> _cursors;
	uint32 _index;
};

} // End of namespace Ring

#endif // RING_CURSOR_H
