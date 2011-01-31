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

#ifndef RING_DRAGCONTROL_H
#define RING_DRAGCONTROL_H

#include "ring/shared.h"

namespace Ring {

class Hotspot;

class DragControl {
public:
	DragControl();
	~DragControl();

private:
	uint32 _field_0;
	uint32 _field_4;
	uint32 _field_8;
	uint32 _field_C;
	uint32 _field_10;
	uint32 _field_14;
	uint32 _field_18;
	uint32 _field_1C;
	byte _field_20;
	uint32 _field_21;
	uint32 _field_25;
	uint32 _field_29;
	uint32 _field_2D;
	uint32 _field_31;
	uint32 _field_35;
	uint32 _field_39;
	uint32 _field_3D;
	uint32 _field_41;
	uint32 _field_45;
	Hotspot *_hotspot;
};

} // End of namespace Ring

#endif // RING_DRAGCONTROL_H
