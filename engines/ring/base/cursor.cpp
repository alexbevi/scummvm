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

#include "ring/base/cursor.h"

#include "ring/helpers.h"

namespace Ring {

//////////////////////////////////////////////////////////////////////////
// CursorBase
//////////////////////////////////////////////////////////////////////////
CursorBase::CursorBase() : BaseObject(kCursorInvalid) {
	_field_4  = 0;
	_field_C  = 0;
	_field_10 = 0;
	_field_14 = 0;
	_field_18 = 0;
}

CursorBase::~CursorBase() {
}

void CursorBase::init(uint32 a1, Common::String name, uint32 a3, byte a4) {
	_field_4  = a1;
	_name = name;
	_field_C  = a3;
	_field_10 = 0;
	_field_14 = 0;
	_field_18 = a4;
}

//////////////////////////////////////////////////////////////////////////
// Cursor
//////////////////////////////////////////////////////////////////////////
Cursor::Cursor() {
	_field_19 = 0;
	_isDefaultCursor = false;
}

Cursor::~Cursor() {
	deinit();
}

void Cursor::init(uint32 a1, Common::String name, uint32 a3, byte a4) {
	CursorBase::init(a1, name, a3, a4);

	if (a4 == 1)
		load();
}

void Cursor::deinit() {
	// TODO release cursor resources
	warning("[Cursor::deinit] Not implemented");
}

void Cursor::load() {
	warning("[Cursor::load] Not implemented");
}

//////////////////////////////////////////////////////////////////////////
// CursorHandler
//////////////////////////////////////////////////////////////////////////
CursorHandler::CursorHandler() {
	_index = 0;
}

CursorHandler::~CursorHandler() {
	CLEAR_ARRAY(CursorBase, _cursors);
}

void CursorHandler::add(CursorId id, Common::String name, CursorType cursorType, uint32 a3, uint32 a4, ArchiveType archiveType) {
	add(id, name, cursorType, a3, 0, 0, 0, a4, archiveType);
}

void CursorHandler::add(CursorId id, Common::String name, CursorType cursorType, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7, ArchiveType archiveType) {
	error("[CursorHandler::add] Not implemented");
}

void CursorHandler::setOffset(CursorId id, Common::Point offset) {
	error("[CursorHandler::setOffset] Not implemented");
}

void CursorHandler::select(CursorId id) {
	error("[CursorHandler::select] Not implemented");
}

} // End of namespace Ring
