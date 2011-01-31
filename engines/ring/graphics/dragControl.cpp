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

#include "ring/graphics/dragControl.h"

#include "ring/graphics/hotspot.h"

#include "ring/helpers.h"

namespace Ring {

DragControl::DragControl() {
	_field_0  = 0;
	_field_4  = 0;
	_field_8  = 0;
	_field_C  = 0;
	_field_10 = 0;
	_field_14 = 0;
	_field_18 = 0;
	_field_1C = 0;
	_field_20 = 0;
	_field_21 = 0;
	_field_25 = 0;
	_field_29 = 0;
	_field_2D = 0;
	_field_31 = 0;
	_field_35 = 0;
	_field_39 = 0;
	_field_3D = 0;
	_field_41 = 0;
	_field_45 = 1;
	_hotspot = new Hotspot(Common::Rect(0, 16, 640, 461), true, 0, 0, 0);
}

DragControl::~DragControl() {
	warning("[DragControl::~DragControl] Not implemented");

	SAFE_DELETE(_hotspot);
}

} // End of namespace Ring
