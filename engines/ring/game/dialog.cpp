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

#include "ring/game/dialog.h"

#include "ring/helpers.h"

namespace Ring {

//////////////////////////////////////////////////////////////////////////
// Dialog
//////////////////////////////////////////////////////////////////////////
Dialog::Dialog() {
}

Dialog::~Dialog() {
}

//////////////////////////////////////////////////////////////////////////
// DialogHandler
//////////////////////////////////////////////////////////////////////////
DialogHandler::DialogHandler() {
	_field_0 = 0;
	_field_4 = 0;
	_field_8 = 0;
	_field_C = 0;
	_field_10 = 0;
	_field_14 = 0;
	_field_18 = 0;
	_field_1C = 0;
	_field_20 = 0;
	_field_28 = 1;
}

DialogHandler::~DialogHandler() {
	CLEAR_ARRAY(Dialog, _dialogs);
}

void DialogHandler::init(int32 a1, int32 a2, int32 a3, int32 a4, int32 a5, int32 a6, int32 a7, int32 a8, int32 a9) {
	_field_0 = a1;
	_field_4 = a2;
	_field_8 = a3;
	_field_C = a4;
	_field_10 = a5;
	_field_14 = a6;
	_field_18 = a7;
	_field_1C = a8;
	_field_20 = a9;
}

} // End of namespace Ring
