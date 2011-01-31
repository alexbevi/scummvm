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

#include "ring/game/bag.h"

#include "ring/game/object.h"

#include "ring/graphics/hotspot.h"
#include "ring/graphics/image.h"

#include "ring/helpers.h"

namespace Ring {

Bag::Bag() {
	_field_C = 0;
	_field_10 = 0;
	_field_14 = 0;
	_field_18 = 0;
	_field_1C = 0;
	_field_20 = 0;
	_field_24 = 0;
	_field_28 = 0;
	_field_2C = 0;
	_field_30 = 0;
	_field_34 = 0;
	_image1 = NULL;
	_image2 = NULL;
	_image3 = NULL;
	_image4 = NULL;
	_field_48 = 0;
	_field_4C = 0;
	_field_50 = 0;
	_field_54 = 0;
	_field_58 = 0;
	_field_5C = 0;
	_field_60 = 0;
	_field_64 = 0;
	_field_68 = NULL;
	_field_6C = NULL;
	_field_70 = NULL;
	_field_74 = 0;
	_field_78 = 0;
	_field_7C = 0;
	_field_80 = 0;
	_field_84 = NULL;
	_field_88 = 0;
	_field_8C = 0;
	_field_90 = 6;
	_field_94 = 0;
	_field_95 = 0;
	_field_99 = 0;
	_field_9D = 0;
	_ticks = 0;
	_field_A5 = NULL;
	_field_A9 = 1;
	_field_AD = 245;
	_field_B1 = 235;
	_field_B5 = 50;
	_field_B9 = 0;
	_field_BD = 0;
	_field_C1 = 0;
	_field_C5 = 90;
	_archiveType = kTypeInvalid;
	_field_CA = NULL;
	_field_CE = NULL;
	_field_D2 = 0;
}

Bag::~Bag() {
	CLEAR_ARRAY(Object, _objects);
	CLEAR_ARRAY(Hotspot, _field_4);
	CLEAR_ARRAY(uint32, _field_8);

	SAFE_DELETE(_image1);
	SAFE_DELETE(_image2);
	SAFE_DELETE(_image3);
	SAFE_DELETE(_image4);

	SAFE_DELETE(_field_68);
	SAFE_DELETE(_field_6C);
	SAFE_DELETE(_field_70);
	SAFE_DELETE(_field_84);
	SAFE_DELETE(_field_A5);
	SAFE_DELETE(_field_CA);
	SAFE_DELETE(_field_CE);
}

//////////////////////////////////////////////////////////////////////////
// Initialization
//////////////////////////////////////////////////////////////////////////
void Bag::sub_4178C0() {
	error("[Bag::sub_4178C0] Not implemented");
}

void Bag::sub_417D20(uint32 a1, uint32 a2) {
	_field_1C = a1;
	_field_20 = a2;
}

void Bag::sub_417D40(uint32 a1, uint32 a2, uint32 a3, uint32 a4) {
	_field_C = a1;
	_field_10 = a2;
	_field_14 = a3;
	_field_18 = a4;
}

void Bag::sub_417D60(uint32 a1, uint32 a2) {
	_field_30 = a1;
	_field_34 = a2;
}

void Bag::sub_417D80(uint32 a1, uint32 a2, uint32 a3, uint32 a4) {
	_field_48 = a1;
	_field_4C = a2;
	_field_50 = a3;
	_field_54 = a4;
}

void Bag::sub_417DA0(uint32 a1, uint32 a2, uint32 a3, uint32 a4) {
	_field_74 = a1;
	_field_78 = a2;
	_field_7C = a3;
	_field_80 = a4;
}

void Bag::sub_417DD0(uint32 a1) {
	_field_24 = a1;
}

void Bag::sub_417DE0(uint32 a1, uint32 a2) {
	_field_88 = a1;
	_field_8C = a2;
}

void Bag::sub_419280(uint32 a1) {
	_field_9D = a1;
	_ticks = g_system->getMillis();
}

void Bag::sub_4192A0(uint32 a1, uint32 a2) {
	_field_58 = a1;
	_field_5C = a2;
}

void Bag::sub_4192C0(uint32 a1, uint32 a2) {
	_field_60 = a1;
	_field_64 = a2;
}

void Bag::loadBackground(Common::String filename1, Common::String filename2, Common::String filename3, Common::String filename4, Common::String filename5, Common::String filename6, Common::String filename7, Common::String filename8, ArchiveType archiveType) {
	_archiveType = archiveType;

	error("[Bag::loadBackground] Not implemented");
}

void Bag::LoadImage(Common::String filename, Image *image, ArchiveType archiveType) {
	error("[Bag::LoadImage] Not implemented");
}

} // End of namespace Ring
