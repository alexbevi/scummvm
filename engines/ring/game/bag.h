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

class Hotspot;
class Object;

class Bag {
public:
	Bag();
	~Bag();

	// Initialization
	void sub_4178C0();

	void sub_417D20(int32 a1, int32 a2);
	void sub_417D40(int32 a1, int32 a2, int32 a3, int32 a4);
	void sub_417D60(int32 a1, int32 a2);
	void sub_417D80(int32 a1, int32 a2, int32 a3, int32 a4);
	void sub_417DA0(int32 a1, int32 a2, int32 a3, int32 a4);
	void sub_417DD0(int32 a1);
	void sub_417DE0(int32 a1, int32 a2);

	void sub_419280(int32 a1);
	void sub_4192A0(int32 a1, int32 a2);
	void sub_4192C0(int32 a1, int32 a2);

	void loadBackground(Common::String str1, Common::String str2, Common::String str3, Common::String str4, Common::String str5, Common::String str6, Common::String str7, Common::String str8, int a9);

private:

	// Bag data
	Common::Array<Object *> _objects;
	Common::Array<Hotspot*> _field_4;
	Common::Array<uint32*> _field_8;
	uint32 _field_C;
	uint32 _field_10;
	uint32 _field_14;
	uint32 _field_18;
	uint32 _field_1C;
	uint32 _field_20;
	uint32 _field_24;
	uint32 _field_28;
	uint32 _field_2C;
	uint32 _field_30;
	uint32 _field_34;
	uint32 *_field_38;
	uint32 *_field_3C;
	uint32 *_field_40;
	uint32 *_field_44;
	uint32 _field_48;
	uint32 _field_4C;
	uint32 _field_50;
	uint32 _field_54;
	uint32 _field_58;
	uint32 _field_5C;
	uint32 _field_60;
	uint32 _field_64;
	uint32 *_field_68;
	uint32 *_field_6C;
	uint32 *_field_70;
	uint32 _field_74;
	uint32 _field_78;
	uint32 _field_7C;
	uint32 _field_80;
	uint32 *_field_84;
	uint32 _field_88;
	uint32 _field_8C;
	uint32 _field_90;
	byte _field_94;
	uint32 _field_95;
	uint32 _field_99;
	uint32 _field_9D;
	uint32 _ticks;
	uint32 *_field_A5;
	uint32 _field_A9;
	uint32 _field_AD;
	uint32 _field_B1;
	uint32 _field_B5;
	uint32 _field_B9;
	uint32 _field_BD;
	uint32 _field_C1;
	uint32 _field_C5;
	byte _field_C9;
	uint32 *_field_CA;
	uint32 *_field_CE;
	byte _field_D2;
};

} // End of namespace Ring

#endif // RING_BAG_H
