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

#include "ring/base/application.h"
#include "ring/base/object.h"
#include "ring/base/text.h"

#include "ring/graphics/animation.h"
#include "ring/graphics/hotspot.h"
#include "ring/graphics/image.h"
#include "ring/graphics/screen.h"

#include "ring/ring.h"
#include "ring/helpers.h"

#include "common/file.h"

namespace Ring {

#pragma region VisualElement

VisualElement::VisualElement(Id id) : Visual(id) {
	_field_8  = 0;
	_field_C  = 0;
	_field_D  = 0;
	_field_11 = 0;
	_left = 0;
	_top = 0;
	_offsetY = 0;
	_height = 0;
	_progressMultiplier = 0;
	_progressColor = 0;
	_initialized = false;
	_progress1 = 0.0f;
	_progress2 = 0.0f;
	_progress3 = 0.0f;
	_progress4 = 0.0f;
}

VisualElement::~VisualElement() {
}

// FIXME This should be moved to application_ring or abstracted
#include "ring/shared_ring.h"
void VisualElement::alloc() {
	if (_initialized)
		return;

	setupProgress(90005, 0, &_progress1);
	setupProgress(90006, 1, &_progress2);
	setupProgress(90007, 2, &_progress3);
	setupProgress(90008, 3, &_progress4);

	_initialized = true;
}

void VisualElement::setupProgress(Id progressId, uint32 textIndex, float *width) {
	float progress = getApp()->varGetFloat(progressId);
	if (progress > 100.0f)
		progress = 100.0f;

	if (progress < 0.0f)
		progress = 0.0f;

	getApp()->objectPresentationSetTextToPuzzle(RingGame::kObjectStatusProgress, 0, textIndex, Common::String::format("%3.1f", progress));

	*width = ceil(_progressMultiplier * progress * 0.01);
}

void VisualElement::dealloc() {
	_initialized = true;
}

void VisualElement::init(uint32 a1, uint32 a2, uint32 left, uint32 top, uint32 offsetY, uint32 height, uint32 progressMultiplier, uint32 progressColor) {
	_field_D  = a1;
	_field_11 = a2;
	_left = left;
	_top = top;
	_offsetY = offsetY;
	_height = height;
	_progressMultiplier = progressMultiplier;
	_progressColor = progressColor;
}

void VisualElement::draw() {
	if (!_field_C)
		return;

	getApp()->getScreenManager()->drawRectangle(Common::Rect(_left, _top,                    _left + _progress1, _top + _height),                    _progressColor);
	getApp()->getScreenManager()->drawRectangle(Common::Rect(_left, _top + _offsetY + 1,     _left + _progress2, _top + _offsetY + _height + 1),     _progressColor);
	getApp()->getScreenManager()->drawRectangle(Common::Rect(_left, _top + 2 * _offsetY + 1, _left + _progress3, _top + 2 * _offsetY + _height + 1), _progressColor);
	getApp()->getScreenManager()->drawRectangle(Common::Rect(_left, _top + 3 * _offsetY - 1, _left + _progress4, _top + 3 * _offsetY + _height - 1), _progressColor);
}

#pragma endregion

#pragma region VisualObjectList

VisualObjectList::VisualObjectList(Id id) : Visual(id) {
	_field_8 = 0;
	_field_C = 0;
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
	_field_65 = 0;
	_field_69 = 0;
	_field_6D = 0;
	_field_CD = -1;
	_field_D1 = -1;
	_field_71 = 0;
	_field_75 = 0;
	_field_79 = 0;
	_foregroundColor1 = Color(200, 200, 30);
	_field_7D = 0;
	_field_81 = 0;
	_field_85 = 0;
	_foregroundColor2 = Color(255, 255, 255);
	_field_89 = 0;
	_field_8D = 0;
	_field_91 = 0;
	_field_95 = 0;
	_field_99 = 0;
	_field_9D = 0;
	_field_A1 = 0;
	_field_A5 = 0;
	_imageType = kImageTypeBMP;
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
	_backgroundColor = Color(0, 0, 0);
	_archiveType = kArchiveFile;
	_allocated = false;
}

VisualObjectList::~VisualObjectList() {
	CLEAR_ARRAY(Object, _objects);
	CLEAR_ARRAY(ImageHandle, _images);
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
	SAFE_DELETE(_text1);
	SAFE_DELETE(_text2);
}

void VisualObjectList::alloc() {
	if (_allocated)
		return;

	loadImage(_backgroundImage);
	loadImage(_upGun);
	loadImage(_upGur);
	loadImage(_upGus);
	loadImage(_upGua);
	loadImage(_downGun);
	loadImage(_downGur);
	loadImage(_downGus);
	loadImage(_downGua);
	loadImage(_cliImageP);
	loadImage(_cliImageA);

	_allocated = true;
}

void VisualObjectList::loadImage(ImageHandle *image) {
	if (image->getNameId().empty())
		return;

	// Compute file path
	Common::String filename = Common::String::format("%s%s", image->getDirectory().c_str(), image->getNameId().c_str());

	image->load(filename, image->getArchiveType(), image->getZone(), image->getLoadFrom());
}

void VisualObjectList::dealloc() {
	_allocated = false;

	if (_backgroundImage)
		_backgroundImage->destroy();

	if (_upGun)
		_upGun->destroy();

	if (_upGur)
		_upGur->destroy();

	if (_upGus)
		_upGus->destroy();

	if (_upGua)
		_upGua->destroy();

	if (_downGun)
		_downGun->destroy();

	if (_downGur)
		_downGur->destroy();

	if (_downGus)
		_downGus->destroy();

	if (_downGua)
		_downGua->destroy();

	if (_cliImageP)
		_cliImageP->destroy();

	if (_cliImageA)
		_cliImageA->destroy();

	for (Common::Array<ImageHandle *>::iterator it = _images.begin(); it != _images.end(); it++)
		(*it)->destroy();
}

void VisualObjectList::init(uint32 a1, Common::String imagePath, Common::String iconPath, Common::String filename3, Common::String filename4, Common::String filename5, Common::String filename6, Common::String filename7, Common::String filename8, Common::String filename9, Common::String filename10, Common::String filename11, Common::String filename12, Common::String filename13, byte a15, ArchiveType archiveType) {
	_archiveType = archiveType;

	if (iconPath != "")
		_iconPath = iconPath;
	else
		_iconPath = Common::String::format("DATA/%s/VISUAL/", getApp()->getCurrentZoneString().c_str());

	// Compute image path
	Common::String path;
	if (imagePath != "") {
		path = imagePath;
	} else {
		if (_archiveType == kArchiveFile)
			path = Common::String::format("DATA/%s/VISUAL/", getApp()->getCurrentZoneString().c_str());
		else
			path = "/VISUAL/";
	}

	// Create images
	_backgroundImage = new ImageHandle(filename3, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_backgroundImage->setDirectory(path);

	_upGun = new ImageHandle(filename4, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_upGun->setDirectory(path);

	_upGur = new ImageHandle(filename5, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_upGur->setDirectory(path);

	_upGus = new ImageHandle(filename6, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_upGus->setDirectory(path);

	_upGua = new ImageHandle(filename7, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_upGua->setDirectory(path);

	_downGun = new ImageHandle(filename8, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_downGun->setDirectory(path);

	_downGur = new ImageHandle(filename9, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_downGur->setDirectory(path);

	_downGus = new ImageHandle(filename10, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_downGus->setDirectory(path);

	_downGua = new ImageHandle(filename11, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_downGua->setDirectory(path);

	_cliImageP = new ImageHandle(filename12, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_cliImageP->setDirectory(path);

	_cliImageA = new ImageHandle(filename13, Common::Point(0, 0), true, a15, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_cliImageA->setDirectory(path);

	_text1 = new Text();
	_text1->init("", Common::Point(0, 0), _fontId, _foregroundColor1, _backgroundColor);

	_text2 = new Text();
	_text2->init("", Common::Point(0, 0), _fontId, _foregroundColor2, _backgroundColor);
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

void VisualObjectList::sub_46DE00(uint32 a1, uint32 a2, ImageType imageType, uint32 a4) {
	_field_A1 = a1;
	_field_A5 = a2;
	_imageType = imageType;
	_field_AD = a4;
}

void VisualObjectList::sub_46DE30(uint32 a1, uint32 a2) {
	_field_B1 = a1;
	_field_B5 = a2;
}

void VisualObjectList::initHotspots() {
	_hotspots.push_back(new Hotspot(Common::Rect((int16)(_field_81 + _field_49),
	                                             (int16)(_field_85 + _field_4D),
	                                             (int16)(_field_81 + _field_49 + _field_89),
	                                             (int16)(_field_85 + _field_4D + _field_8D)),
	                                false,
	                                0,
	                                1,
	                                0));

	_hotspots.push_back(new Hotspot(Common::Rect((int16)(_field_49 + _field_91),
	                                             (int16)(_field_4D + _field_95),
	                                             (int16)(_field_49 + _field_91 + _field_99),
	                                             (int16)(_field_4D + _field_95 + _field_9D)),
	                                false,
	                                0,
	                                2,
	                                0));

	if (_field_B9 & 1) {
		for (uint32 i = 0; i < _field_BD; i++) {
			uint32 y = i * _field_69 + _field_69 / 2;
			_hotspots.push_back(new Hotspot(Common::Rect((int16)(_field_49 + _field_59),
			                                             (int16)(_field_4D + _field_5D + y - _field_65 / 2),
			                                             (int16)(_field_49 + _field_59 + _field_61),
			                                             (int16)(_field_4D + _field_5D + _field_65 / 2 + y)),
			                                false,
			                                0,
			                                3,
			                                i));
		}
	}

	if (_field_B9 & 2) {
		for (uint32 i = 0; i < _field_BD; i++) {
			uint32 y = i * _field_69 + _field_69 / 2;
			_hotspots.push_back(new Hotspot(Common::Rect((int16)(_field_49 + _field_59 + _field_61 / 2 + y),
			                                             (int16)(_field_4D + _field_5D),
			                                             (int16)(_field_49 + _field_59 + _field_61 / 2 + y),
			                                             (int16)(_field_4D + _field_5D + _field_65)),
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

void VisualObjectList::setTextForegroundColor(Color foreground1, Color foreground2) {
	if (_text1)
		_text1->setForegroundColor(foreground1);

	if (_text2)
		_text2->setForegroundColor(foreground2);

	if (_text1 || _text2) {
		_foregroundColor1 = foreground1;
		_foregroundColor2 = foreground2;
	}
}

void VisualObjectList::setTextBackgroundColor(Color background) {
	if (_text1)
		_text1->setBackgroundColor(background);

	if (_text2)
		_text2->setBackgroundColor(background);

	if (_text1 || _text2) {
		_backgroundColor = background;
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
	if (_objects.has(objectId)) {
		_field_C9 = 0;
		_field_CD = -1;
		_field_D1 = -1;

		return;
	}

	Object *object = getApp()->objectGet(objectId);
	ImageHandle *image = NULL;

	if (object->getAnimationImage()) {
		object->getAnimationImage()->updateCurrentImage();
		image = object->getAnimationImage()->getCurrentImage();
	} else {
		// Compute path
		Common::String path = Common::String::format("%s%s.%s", _iconPath.c_str(), object->getName().c_str(), Application::getFileExtension(_imageType).c_str());
		if (!Common::File::exists(path))
			path = "dummyLS.bmp";

		image = new ImageHandle(path, Common::Point(0, 0), true, kZoneNone, kLoadFromDisk, _imageType, kArchiveFile);
	}

	if (!image)
		error("[VisualObjectList::add] Could not get an image for this object");

	_objects.push_back(object);
	_images.push_back(image);

	_itemCount = _objects.size();
	if (_itemCount > _field_BD)
		_field_C1 = 0;
	_field_C9 = 0;
	_field_CD = -1;
	_field_D1 = -1;
}

void VisualObjectList::remove(ObjectId objectId, bool removeObject) {
	if (!_objects.has(objectId))
		return;

	if (removeObject)
		getApp()->objectRemove(objectId);

	// Remove object image
	uint32 index = _objects.getIndex(objectId);

	_images.remove_at(index);
	_objects.remove_at(index);

	_itemCount = _objects.size();
	if (_itemCount <= _field_BD)
		_field_C1 = 0;
	_field_C9 = 0;
	_field_CD = -1;
	_field_D1 = -1;
}

void VisualObjectList::removeAll(bool removeObject) {
	// Remove all objects if asked
	if (removeObject) {
		for (Common::Array<Object *>::iterator it = _objects.begin(); it != _objects.end(); it++)
			getApp()->objectRemove((*it)->getId());
	}

	// Clear the list of objects
	_objects.clear();

	// Remove all images
	CLEAR_ARRAY(ImageHandle, _images);

	_field_C1 = 0;
	_itemCount = _objects.size();
	_field_C9 = 0;
	_field_CD = -1;
	_field_D1 = -1;
}


#pragma endregion

} // End of namespace Ring
