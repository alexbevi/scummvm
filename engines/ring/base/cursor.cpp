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

#include "ring/graphics/animation.h"
#include "ring/graphics/image.h"

#include "ring/helpers.h"

namespace Ring {

#pragma region CursorBase

CursorBase::CursorBase() : BaseObject(kCursorInvalid) {
	_type = kCursorTypeInvalid;
	_field_18 = 0;
}

CursorBase::~CursorBase() {
}

void CursorBase::init(CursorId id, Common::String name, CursorType cursorType, byte a4) {
	_id	 = id;
	_name = name;
	_type  = cursorType;
	_field_18 = a4;
}

#pragma endregion

#pragma region Cursor

Cursor::Cursor() {
	_field_19 = 0;
	_isDefaultCursor = false;
}

Cursor::~Cursor() {
	deinit();
}

void Cursor::init(CursorId id, Common::String name, CursorType cursorType, byte a4) {
	CursorBase::init(id, name, cursorType, a4);

	if (a4 == 1)
		alloc();
}

void Cursor::deinit() {
	// TODO release cursor resources
	error("[Cursor::deinit] Not implemented");
}

void Cursor::alloc() {
	warning("[Cursor::alloc] Not implemented (win32 resource)");
}

void Cursor::dealloc() {
	error("[Cursor::dealloc] Not implemented");
}

#pragma endregion

#pragma region CursorImage

CursorImage::CursorImage() {
	_image = NULL;
}

CursorImage::~CursorImage() {
	deinit();
}

void CursorImage::init(CursorId id, Common::String name, CursorType cursorType, byte a4, ArchiveType archiveType) {
	CursorBase::init(id, name, cursorType, a4);

	_archiveType = archiveType;
	_image = new Image();

	if (a4 == 1)
		alloc();
}

void CursorImage::deinit() {
	SAFE_DELETE(_image);
}

void CursorImage::alloc() {
	if (!_image)
		return;

	if (!_image->load(getName(), _archiveType, 1, 2))
		error("[CursorImage::alloc] Cannot load image (%s)", getName().c_str());

	if (_image->getBPP() != 32) {
		dealloc();
		error("[CursorImage::alloc] Only 32bpp TGA are supported as cursors images");
	}
}

void CursorImage::dealloc() {
	if (!_image)
		return;

	if (_image->isInitialized())
		_image->destroy();
}

#pragma endregion

#pragma region CursorAnimation

CursorAnimation::CursorAnimation() {
	_image = new AnimationImage();
}

CursorAnimation::~CursorAnimation() {
	deinit();
	SAFE_DELETE(_image);
}

void CursorAnimation::init(CursorId id, Common::String name, CursorType cursorType, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7, ArchiveType archiveType) {
	CursorBase::init(id, name, cursorType, a4);

	_image->init(name, 1, 0, 0, 0, 3, a4, a5, 1, a6, a3, 0, a7, archiveType);
	_image->setTicks(g_system->getMillis());
}

void CursorAnimation::deinit() {
	_image->deinit();
}

void CursorAnimation::alloc() {
	_image->alloc();
}

void CursorAnimation::dealloc() {
	_image->dealloc();
}

void CursorAnimation::draw() {
	_image->draw();
}

#pragma endregion

#pragma region CursorHandler

CursorHandler::CursorHandler() {
	_index = 0;
}

CursorHandler::~CursorHandler() {
	CLEAR_ARRAY(CursorBase, _cursors);
}

void CursorHandler::add(CursorId id, Common::String name, CursorType cursorType, uint32 a3, uint32 a4, uint32 a5, uint32 a6, ImageType imageType, ArchiveType archiveType) {
	if (_cursors.has(id))
		error("[CursorHandler::add] ID already exists (%d)", id);

	CursorBase *cursor = NULL;
	switch (cursorType) {
	default:
		error("[CursorHandler::add] Invalid cursor type (%d)", cursorType);
		break;

	case kCursorTypeNormal:
	case kCursorType2:
		cursor = new Cursor();
		cursor->init(id, name, cursorType, a3);
		break;

	case kCursorTypeImage: {
		cursor = new CursorImage();

		// Check archive type
		if (archiveType != kArchiveFile && archiveType != kArchiveArt)
			error("[CursorHandler::add] Invalid archive type (archiveType: %d)", archiveType);

		Common::String path;
		switch (imageType) {
		default:
			error("[CursorHandler::add] Cannot load image cursor (imageType: %d)", imageType);
			break;

		case kImageCursor:
			path = Common::String::format("%sCURSOR/%s.tga", archiveType == kArchiveFile ? "" : "/", name.c_str());
			break;

		case kImageListIcon:
			path = Common::String::format("%sLSTICON/%s.tga", archiveType == kArchiveFile ? "" : "/", name.c_str());
			break;
		}


		((CursorImage *)cursor)->init(id, path, cursorType, a3, archiveType);
		}
		break;

	case kCursorTypeAnimated:
		cursor = new CursorAnimation();
		((CursorAnimation *)cursor)->init(id, name, cursorType, a3, a4, a5, a6, imageType, archiveType);
		break;
	}

	_cursors.push_back(cursor);
}

void CursorHandler::draw() {
	if (_cursors.empty())
		error("[CursorHandler::draw] No cursor present");

	_cursors[_index]->draw();
}

void CursorHandler::select(CursorId id) {
	if (!_cursors.has(id))
		error("[CursorHandler::select] ID doesn't exist (%d)", id);

	_index = _cursors.getIndex(id);
}

void CursorHandler::setOffset(CursorId id, Common::Point offset) {
	if (!_cursors.has(id))
		error("[CursorHandler::setOffset] ID doesn't exist (%d)", id);

	_cursors.get(id)->setOffset(offset);
}

CursorType CursorHandler::getType() {
	if (_cursors.empty())
		error("[CursorHandler::getType] No cursor present");

	return _cursors[_index]->getType();
}

#pragma endregion

} // End of namespace Ring
