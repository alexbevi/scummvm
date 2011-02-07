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

#include "ring/graphics/visual.h"

#include "ring/base/text.h"

#include "ring/game/application.h"
#include "ring/game/object.h"

#include "ring/graphics/hotspot.h"
#include "ring/graphics/image.h"

#include "ring/ring.h"
#include "ring/helpers.h"

namespace Ring {

#pragma region VisualElement

VisualElement::VisualElement(Id id) : Visual(id) {
	_field_8  = 0;
	_field_C  = 0;
	_field_D  = 0;
	_field_11 = 0;
	_field_15 = 0;
	_field_19 = 0;
	_field_1D = 0;
	_field_21 = 0;
	_field_25 = 0;
	_field_29 = 0;
	_field_2D = 0;
	_field_2E = 0;
	_field_32 = 0;
	_field_36 = 0;
	_field_3A = 0;
}

VisualElement::~VisualElement() {
}

void VisualElement::init(uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7, uint32 a8) {
	_field_D  = a1;
	_field_11 = a2;
	_field_15 = a3;
	_field_19 = a4;
	_field_1D = a5;
	_field_21 = a6;
	_field_25 = a7;
	_field_29 = a8;
}

#pragma endregion

#pragma region VisualObjectList

VisualObjectList::VisualObjectList(Id id) : Visual(id) {
	_backgroundImage = NULL;
	_upGun = NULL;
	_upGur = NULL;
	_upGus = NULL;
	_upGua = NULL;
	_downGun = NULL;
	_downGur = NULL;
	_downGus = NULL;
	_downGua = NULL;
	_cliImageP = NULL;
	_cliImageA = NULL;
	_field_49 = 0;
	_field_4D = 0;
	_field_51 = 0;
	_field_55 = 0;
	_field_59 = 0;
	_field_5D = 0;
	_field_61 = 0;
	_field_69 = 0;
	_field_6D = 0;
	_field_CD = -1;
	_field_D1 = -1;
	_field_71 = 0;
	_field_75 = 0;
	_field_79 = 0;
	_field_E2 = 200;
	_field_E6 = 200;
	_field_7D = 0;
	_field_81 = 0;
	_field_85 = 0;
	_field_EE = 255;
	_field_F2 = 255;
	_field_F6 = 255;
	_field_89 = 0;
	_field_8D = 0;
	_field_91 = 0;
	_field_95 = 0;
	_field_99 = 0;
	_field_9D = 0;
	_field_A1 = 0;
	_field_A5 = 0;
	_field_A9 = 0;
	_field_AD = 0;
	_field_B1 = 0;
	_field_B5 = 0;
	_field_B9 = 0;
	_field_BD = 0;
	_field_C9 = 0;
	_field_C1 = 0;
	_itemCount = 0;
	_text1 = NULL;
	_text2 = NULL;
	_fontId = kFontDefault;
	_field_EA = 30;
	_field_FA = 0;
	_field_FE = 0;
	_field_102 = 0;
	_archiveType = kArchiveFile;
	_field_106 = 0;
}

VisualObjectList::~VisualObjectList() {
	CLEAR_ARRAY(Object, _objects);
	CLEAR_ARRAY(uint32, _field_15);
	CLEAR_ARRAY(Hotspot, _hotspots);

	SAFE_DELETE(_backgroundImage);
	SAFE_DELETE(_upGun);
	SAFE_DELETE(_upGur);
	SAFE_DELETE(_upGus);
	SAFE_DELETE(_upGua);
	SAFE_DELETE(_downGun);
	SAFE_DELETE(_downGur);
	SAFE_DELETE(_downGus);
	SAFE_DELETE(_downGua);
	SAFE_DELETE(_cliImageP);
	SAFE_DELETE(_cliImageA);
}

void VisualObjectList::init(uint32 a1, Common::String imagePath, Common::String iconPath, Common::String filename3, Common::String filename4, Common::String filename5, Common::String filename6, Common::String filename7, Common::String filename8, Common::String filename9, Common::String filename10, Common::String filename11, Common::String filename12, Common::String filename13, uint32 a15, ArchiveType archiveType) {
	_archiveType = archiveType;

	if (iconPath != "")
		_iconPath = iconPath;
	else
		_iconPath = Common::String::format("/DATA/%s/VISUAL/", getApp()->getZoneString(getApp()->getCurrentZone()).c_str());

	// Compute image path
	Common::String path;
	if (imagePath != "") {
		path = imagePath;
	} else {
		if (_archiveType == kArchiveFile)
			path = Common::String::format("/DATA/%s/VISUAL/", getApp()->getZoneString(getApp()->getCurrentZone()).c_str());
		else
			path = "/VISUAL/";
	}

	// Create images
	_backgroundImage = new ImageHandle(filename3, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, 4, _archiveType);
	_backgroundImage->setPath(path);

	_upGun = new ImageHandle(filename4, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, 4, _archiveType);
	_upGun->setPath(path);

	_upGur = new ImageHandle(filename5, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, 4, _archiveType);
	_upGur->setPath(path);

	_upGus = new ImageHandle(filename6, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, 4, _archiveType);
	_upGus->setPath(path);

	_upGua = new ImageHandle(filename7, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, 4, _archiveType);
	_upGua->setPath(path);

	_downGun = new ImageHandle(filename8, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, 4, _archiveType);
	_downGun->setPath(path);

	_downGur = new ImageHandle(filename9, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, 4, _archiveType);
	_downGur->setPath(path);

	_downGus = new ImageHandle(filename10, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, 4, _archiveType);
	_downGus->setPath(path);

	_downGua = new ImageHandle(filename11, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, 4, _archiveType);
	_downGua->setPath(path);

	_cliImageP = new ImageHandle(filename12, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, 4, _archiveType);
	_cliImageP->setPath(path);

	_cliImageA = new ImageHandle(filename13, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, 4, _archiveType);
	_cliImageA->setPath(path);

	_text1 = new Text();
	_text1->init("", 0, 0, _fontId, _field_E2, _field_E6, _field_EA, _field_FA, _field_FE, _field_102);

	_text2 = new Text();
	_text2->init("", 0, 0, _fontId, _field_E2, _field_E6, _field_EA, _field_FA, _field_FE, _field_102);
}

void VisualObjectList::sub_46DCF0(uint32 a1, uint32 a2) {
	_field_49 = a1;
	_field_4D = a2;
}

void VisualObjectList::sub_46DD11(uint32 a1, uint32 a2) {
	_field_51 = a1;
	_field_55 = a2;
}

void VisualObjectList::sub_46DD30(uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6) {
	_field_59 = a1;
	_field_5D = a2;
	_field_61 = a3;
	_field_65 = a4;
	_field_69 = a5;
	_field_6D = a6;
}

void VisualObjectList::sub_46DD60(uint32 a1, uint32 a2) {
	_field_71 = a1;
	_field_75 = a2;
}

void VisualObjectList::sub_46DD80(uint32 a1, uint32 a2) {
	_field_79 = a1;
	_field_7D = a2;
}


void VisualObjectList::sub_46DDA0(uint32 a1, uint32 a2, uint32 a3, uint32 a4) {
	_field_81 = a1;
	_field_85 = a2;
	_field_89 = a3;
	_field_8D = a4;
}


void VisualObjectList::sub_46DDD0(uint32 a1, uint32 a2, uint32 a3, uint32 a4) {
	_field_91 = a1;
	_field_95 = a2;
	_field_99 = a3;
	_field_9D = a4;
}

void VisualObjectList::sub_46DE00(uint32 a1, uint32 a2, uint32 a3, uint32 a4) {
	_field_A1 = a1;
	_field_A5 = a2;
	_field_A9 = a3;
	_field_AD = a4;
}

void VisualObjectList::sub_46DE30(uint32 a1, uint32 a2) {
	_field_B1 = a1;
	_field_B5 = a2;
}

void VisualObjectList::initHotspots() {
	_hotspots.push_back(new Hotspot(Common::Rect(_field_81 + _field_49,
	                                             _field_85 + _field_4D,
	                                             _field_81 + _field_49 + _field_89,
	                                             _field_85 + _field_4D + _field_8D),
	                                false,
	                                0,
	                                1,
	                                0));

	_hotspots.push_back(new Hotspot(Common::Rect(_field_49 + _field_91,
	                                             _field_4D + _field_95,
	                                             _field_49 + _field_91 + _field_99,
	                                             _field_4D + _field_95 + _field_9D),
	                                false,
	                                0,
	                                2,
	                                0));

	if (_field_B9 & 1) {
		for (uint32 i = 0; i < _field_BD; i++) {
			uint32 y = i * _field_69 + _field_69 / 2;
			_hotspots.push_back(new Hotspot(Common::Rect(_field_49 + _field_59,
			                                             _field_4D + _field_5D - _field_65 / 2 + y,
			                                             _field_49 + _field_59 + _field_61,
			                                             _field_4D + _field_5D + _field_65 / 2 + y),
			                                false,
			                                0,
			                                3,
			                                i));
		}
	}

	if (_field_B9 & 2) {
		for (uint32 i = 0; i < _field_BD; i++) {
			uint32 y = i * _field_69 + _field_69 / 2;
			_hotspots.push_back(new Hotspot(Common::Rect(_field_49 + _field_59 + _field_61 / 2 + y,
			                                             _field_4D + _field_5D,
			                                             _field_49 + _field_59 + _field_61 / 2 + y,
			                                             _field_4D + _field_5D + _field_65),
			                                false,
			                                0,
			                                3,
			                                i));
		}
	}
}

void VisualObjectList::sub_46E330(uint32 a1) {
	_field_BD = a1;
}

void VisualObjectList::sub_46E340(uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6) {
	if (_text1)
		_text1->setField10(a1, a2, a3);

	if (_text2)
		_text2->setField10(a1, a2, a3);

	if (_text1 || _text2) {
		_field_E2 = a1;
		_field_E6 = a2;
		_field_EA = a3;
		_field_EE = a4;
		_field_F2 = a5;
		_field_F6 = a6;
	}
}

void VisualObjectList::sub_46E3F0(int32 a1, int32 a2, int32 a3) {
	if (_text1)
		_text1->setFields1C1D(a1, a2, a3);

	if (_text2)
		_text2->setFields1C1D(a1, a2, a3);

	if (_text1 || _text2) {
		_field_FA = a1;
		_field_FE = a2;
		_field_102 = a3;
	}
}

void VisualObjectList::setFontId(FontId fontId) {
	if (_text1)
		_text1->setFontId(fontId);

	if (_text2)
		_text2->setFontId(fontId);

	if (_text1 || _text2)
		_fontId = fontId;
}

#pragma endregion

#pragma region Management

void VisualObjectList::add(ObjectId objectId) {
	error("[Application::add] Not implemented");
}

void VisualObjectList::remove(ObjectId objectId, bool removeObject) {
	error("[Application::remove] Not implemented");
}

void VisualObjectList::removeAll(bool removeObject) {
	error("[Application::removeAll] Not implemented");
}


#pragma endregion

} // End of namespace Ring
