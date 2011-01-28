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

#ifndef RING_BAG_H
#define RING_BAG_H

#include "ring/shared.h"

namespace Ring {

class Bag {
public:
	Bag();
	~Bag();

	void init();
	void sub_417D20(int32 a1, int32 a2);
	void sub_417D40(int32 a1, int32 a2, int32 a3, int32 a4);
	void sub_417D60(int32 a1, int32 a2);
	void sub_417DD0(int32 a1);
	void sub_417D80(int32 a1, int32 a2, int32 a3, int32 a4);
	void sub_417DA0(int32 a1, int32 a2, int32 a3, int32 a4);
	void sub_4192A0(int32 a1, int32 a2);
	void sub_4192C0(int32 a1, int32 a2);
	void sub_417DE0(int32 a1, int32 a2);
	void sub_419280(int32 a1);
	void loadBackground(Common::String str1, Common::String str2, Common::String str3, Common::String str4, Common::String str5, Common::String str6, Common::String str7, Common::String str8, int a9);
	void sub_4178C0();

private:

};

} // End of namespace Ring

#endif // RING_BAG_H
