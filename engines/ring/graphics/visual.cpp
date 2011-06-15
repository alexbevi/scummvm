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
 */

#include "ring/graphics/visual.h"

#include "ring/base/application.h"
#include "ring/base/event.h"
#include "ring/base/object.h"
#include "ring/base/text.h"

#include "ring/graphics/animation.h"
#include "ring/graphics/hotspot.h"
#include "ring/graphics/image.h"
#include "ring/graphics/screen.h"

#include "ring/ring.h"
#include "ring/helpers.h"

#include "common/file.h"
#include "common/tokenizer.h"

namespace Ring {

#pragma region Visual

Visual::Visual(Id id) : BaseObject(id) {
	_field_8 = 0;
	_visible = false;
}

Visual::~Visual() {}

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
	_field_59 = 0;
	_field_5D = 0;
	_field_61 = 0;
	_field_65 = 0;
	_field_69 = 0;
	_field_6D = 0;
	_objectIndexClicked = -1;
	_imageIndexClicked = -1;
	_foregroundColor = Color(200, 200, 30);
	_field_81 = 0;
	_field_85 = 0;
	_foregroundColorSelected = Color(255, 255, 255);
	_field_89 = 0;
	_field_8D = 0;
	_field_91 = 0;
	_field_95 = 0;
	_field_99 = 0;
	_field_9D = 0;
	_imageType = kImageTypeBMP;
	_drawType = kDrawTypeInvalid;
	_field_B1 = 0;
	_field_B5 = 0;
	_field_B9 = 0;
	_field_BD = 0;
	_objectIdClicked = 0;
	_objectIndex = 0;
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

void VisualObjectList::loadImage(ImageHandle *image) const {
	if (!image || image->getNameId().empty())
		return;

	// Compute file path
	Common::String filename = Common::String::format("%s%s", image->getDirectory().c_str(), image->getNameId().c_str());

	image->load(filename, image->getArchiveType(), image->getZone(), image->getLoadFrom(), image->getDrawType());
}

Hotspot *VisualObjectList::getHotspot(const Common::Point &point) {
	for (Common::Array<Hotspot *>::iterator it = _hotspots.begin(); it != _hotspots.end(); it++)
		if ((*it)->contains(point))
			return (*it);

	return NULL;
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

void VisualObjectList::init(uint32 a1, Common::String imagePath, Common::String iconPath, Common::String filename3, Common::String filename4, Common::String filename5, Common::String filename6, Common::String filename7, Common::String filename8, Common::String filename9, Common::String filename10, Common::String filename11, Common::String filename12, Common::String filename13, DrawType drawType, ArchiveType archiveType) {
	_archiveType = archiveType;

	if (iconPath != "")
		_iconPath = iconPath;
	else
		_iconPath = Common::String::format("DATA/%s/VISUAL/", getApp()->getCurrentZoneFolder().c_str());

	// Compute image path
	Common::String path;
	if (imagePath != "") {
		path = imagePath;
	} else {
		if (_archiveType == kArchiveFile)
			path = Common::String::format("DATA/%s/VISUAL/", getApp()->getCurrentZoneFolder().c_str());
		else
			path = "/VISUAL/";
	}

	_field_B9 = a1;

	// Create images
	_backgroundImage = new ImageHandle(filename3, Common::Point(0, 0), true, drawType, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_backgroundImage->setDirectory(path);

	_upGun = new ImageHandle(filename4, Common::Point(0, 0), true, drawType, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_upGun->setDirectory(path);

	_upGur = new ImageHandle(filename5, Common::Point(0, 0), true, drawType, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_upGur->setDirectory(path);

	_upGus = new ImageHandle(filename6, Common::Point(0, 0), true, drawType, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_upGus->setDirectory(path);

	_upGua = new ImageHandle(filename7, Common::Point(0, 0), true, drawType, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_upGua->setDirectory(path);

	_downGun = new ImageHandle(filename8, Common::Point(0, 0), true, drawType, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_downGun->setDirectory(path);

	_downGur = new ImageHandle(filename9, Common::Point(0, 0), true, drawType, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_downGur->setDirectory(path);

	_downGus = new ImageHandle(filename10, Common::Point(0, 0), true, drawType, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_downGus->setDirectory(path);

	_downGua = new ImageHandle(filename11, Common::Point(0, 0), true, drawType, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_downGua->setDirectory(path);

	_cliImageP = new ImageHandle(filename12, Common::Point(0, 0), true, drawType, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_cliImageP->setDirectory(path);

	_cliImageA = new ImageHandle(filename13, Common::Point(0, 0), true, drawType, 1000, 0, getApp()->getCurrentZone(), kLoadFrom5, kImageTypeBackground, _archiveType);
	_cliImageA->setDirectory(path);

	_text1 = new Text();
	_text1->init("", Common::Point(0, 0), _fontId, _foregroundColor, _backgroundColor);

	_text2 = new Text();
	_text2->init("", Common::Point(0, 0), _fontId, _foregroundColorSelected, _backgroundColor);
}

void VisualObjectList::sub_46DD30(uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6) {
	_field_59 = a1;
	_field_5D = a2;
	_field_61 = a3;
	_field_65 = a4;
	_field_69 = a5;
	_field_6D = a6;
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

void VisualObjectList::setImageInfo(const Common::Point &point, ImageType imageType, DrawType drawType) {
	_imageCoords = point;
	_imageType = imageType;
	_drawType = drawType;
}

void VisualObjectList::sub_46DE30(uint32 a1, uint32 a2) {
	_field_B1 = a1;
	_field_B5 = a2;
}

void VisualObjectList::initHotspots() {
	_hotspots.push_back(new Hotspot(Common::Rect(_origin.x + (int16)(_field_81),
	                                             _origin.y + (int16)(_field_85),
	                                             _origin.x + (int16)(_field_81 + _field_89),
	                                             _origin.y + (int16)(_field_85 + _field_8D)),
	                                false,
	                                0,
	                                kCursorPassive,
	                                0));

	_hotspots.push_back(new Hotspot(Common::Rect(_origin.x + (int16)(_field_91),
	                                             _origin.y + (int16)(_field_95),
												 _origin.x + (int16)(_field_91 + _field_99),
												 _origin.y + (int16)(_field_95 + _field_9D)),
	                                false,
	                                0,
	                                kCursorActive,
	                                0));

	if (_field_B9 & 1) {
		for (uint32 i = 0; i < _field_BD; i++) {
			uint32 y = i * _field_69 + _field_69 / 2;
			_hotspots.push_back(new Hotspot(Common::Rect(_origin.x + (int16)(_field_59),
			                                             _origin.y + (int16)(_field_5D + y - _field_65 / 2),
														 _origin.x + (int16)(_field_59 + _field_61),
														 _origin.y + (int16)(_field_5D + _field_65 / 2 + y)),
			                                false,
			                                0,
			                                kCursorPassiveDraw,
			                                i));
		}
	}

	if (_field_B9 & 2) {
		for (uint32 i = 0; i < _field_BD; i++) {
			uint32 y = i * _field_69 + _field_69 / 2;
			_hotspots.push_back(new Hotspot(Common::Rect(_origin.x + (int16)(_field_59 + _field_61 / 2 + y),
			                                             _origin.y + (int16)(_field_5D),
														 _origin.x + (int16)(_field_59 + _field_61 / 2 + y),
														 _origin.y + (int16)(_field_5D + _field_65)),
			                                false,
			                                0,
			                                kCursorPassiveDraw,
			                                i));
		}
	}
}

void VisualObjectList::sub_46E330(uint32 a1) {
	_field_BD = a1;
}

void VisualObjectList::setTextForegroundColor(const Color &foreground, const Color &foregroundSelected) {
	if (_text1)
		_text1->setForegroundColor(foreground);

	if (_text2)
		_text2->setForegroundColor(foregroundSelected);

	if (_text1 || _text2) {
		_foregroundColor = foreground;
		_foregroundColorSelected = foregroundSelected;
	}
}

void VisualObjectList::setTextBackgroundColor(const Color &background) {
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

void VisualObjectList::setIconDirectory(const Common::String &directory) {
	if (directory.empty()) {
		_iconPath = Common::String::format("DATA/%s/VISUAL/", getApp()->getCurrentZoneFolder().c_str());
	} else {
		_iconPath = directory;
	}
}

void VisualObjectList::draw() {
	if (!_visible)
		return;

	ScreenManager *screen = getApp()->getScreenManager();

	if (_backgroundImage && !_backgroundImage->getNameId().empty())
		screen->draw(_backgroundImage, _origin + _backgroundOffset, _backgroundImage->getDrawType());

	if (_objectIndex == 0) {
		if (_upGun && !_upGun->getNameId().empty())
			screen->draw(_upGun, _origin + _upOffset, _upGun->getDrawType());

		if (_hotspots.size() > 0)
			_hotspots[0]->disable();
	} else {
		if (_upGua && !_upGua->getNameId().empty())
			screen->draw(_upGua, _origin + _upOffset, _upGua->getDrawType());

		if (_hotspots.size() > 0)
			_hotspots[0]->enable();
	}

	if ((_objectIndex + _field_BD) >= _itemCount) {
		if (_downGun && !_downGun->getNameId().empty())
			screen->draw(_downGun, _origin + _downOffset, _downGun->getDrawType());

		if (_hotspots.size() > 1)
			_hotspots[1]->disable();
	} else {
		if (_downGua && !_downGua->getNameId().empty())
			screen->draw(_downGua, _origin + _downOffset, _downGua->getDrawType());

		if (_hotspots.size() > 1)
			_hotspots[1]->enable();
	}

	// Show objects
	int16 offset = _origin.x + (int16)_field_59;
	uint32 count = _objectIndex + _field_BD;
	if (count >= _itemCount)
		count = _itemCount;

	// Check text
	if (!_text1 || !_text2)
		error("[VisualObjectList::draw] Text objects not initialized properly");

	if (!_cliImageP || !_cliImageA)
		error("[VisualObjectList::draw] Cli images not initialized properly");

	if (_objectIndex < count) {
		for (uint32 i = _objectIndex; i < count; i++) {
			Object *object = _objects[i];

			// Draw description
			Common::String description = object->getDescription();
			if (!description.empty()) {
				// Set object text
				Common::StringTokenizer line(description, "#");
				if (!line.empty()) {
					_text1->set(line.nextToken());
					_text2->set(line.nextToken());
				} else {
					_text1->set(description);
					_text2->set("");
				}

				// Set text coordinates
				Common::Point imageCoords;
				if (_field_B9 & 1) {
					int16 text1Y = _origin.y + (int16)(i * _field_69 + _field_5D + (_field_69 - _text1->getHeight()) / 2);
					_text1->setCoordinates(Common::Point(offset, text1Y));
					_text2->setCoordinates(Common::Point(offset, text1Y + (int16)(_text1->getHeight() + _field_6D)));

					imageCoords = Common::Point(_origin.x + (int16)_field_B1, _origin.y + (int16)(i * _field_69 + _field_B5 - (_field_69 - _cliImageP->getHeight()) / 2));
				}

				if (_field_B9 & 2) {
					int16 textX = _origin.x + (int16)(i * _field_69 + _field_59 + (_field_69 - _text1->getWidth()) / 2);
					_text1->setCoordinates(Common::Point(textX, _origin.y + (int16)_field_5D));
					_text2->setCoordinates(Common::Point(textX, _origin.y + (int16)(_field_5D + _text1->getHeight() + _field_6D)));

					imageCoords = Common::Point(_origin.x + (int16)(i * _field_69 + _field_B1 + (_field_69 - _text1->getWidth()) / 2), _origin.y + (int16)_field_B5);
				}

				if (_objectIdClicked == object->getId()) {
					_text1->setForegroundColor(_foregroundColorSelected);
					_text2->setForegroundColor(_foregroundColorSelected);

					if (!_cliImageA->getNameId().empty())
						screen->draw(_cliImageA, imageCoords, _cliImageP->getDrawType());
				} else {
					_text1->setForegroundColor(_foregroundColor);
					_text2->setForegroundColor(_foregroundColor);

					if (!_cliImageP->getNameId().empty())
						screen->draw(_cliImageP, imageCoords, _cliImageP->getDrawType());
				}

				// Draw text
				screen->drawText(_text1);
				screen->drawText(_text2);
			}

			// Draw images
			if (_field_B9 & 64) {

				if (_imageIndexClicked != -1)
					_images[_imageIndexClicked]->destroy();

				if (_objectIndexClicked != -1) {
					ImageHandle *imageClicked = _images[_objectIndexClicked];

					if (imageClicked->getField6C() == 1) {
						bool loaded = true;
						if (!imageClicked->isInitialized()) {
							Common::String path = Common::String::format("%s%s.%s", _iconPath.c_str(), _objects[_objectIndexClicked]->getName().c_str(), Application::getFileExtension(_imageType).c_str());
							loaded = imageClicked->load(path, kArchiveFile, kZoneNone, kLoadFromInvalid, imageClicked->getDrawType());
						}

						if (loaded)
							screen->draw(imageClicked, _imageCoords, _drawType);
					} else {
						AnimationImage *animation = imageClicked->getAnimation();
						animation->allocActive();
						animation->computeCurrentFrame(g_system->getMillis());
						animation->playFrame(_imageCoords);
					}
				}
			}
		}
	}

	// Update hotspots
	for (uint32 i = 0; i < _field_BD; i++) {
		if (i >= (count - _objectIndex))
			_hotspots[i + 2]->disable();
		else
			_hotspots[i + 2]->enable();
	}

}

uint32 VisualObjectList::handleLeftButtonUp(const Common::Point &point) {
	if (!_visible)
		return false;

	Hotspot *hotspot = getHotspot(point);
	if (!hotspot)
		return false;

	switch (hotspot->getCursorId()) {
	default:
		break;

	case kCursorPassive:
		if (_objectIndex > 0)
			--_objectIndex;

		getApp()->getEventHandler()->onVisualList(_id, 1, point);
		return 1;

	case kCursorActive:
		if (_objectIndex < _itemCount)
			++_objectIndex;

		getApp()->getEventHandler()->onVisualList(_id, 2, point);
		return 2;

	case kCursorPassiveDraw:
		_imageIndexClicked  = _objectIndexClicked;
		_objectIndexClicked = (int32)_objectIndex + hotspot->getTarget();
		_objectIdClicked    = _objects[_objectIndexClicked]->getId();

		getApp()->getEventHandler()->onVisualList(_id, 3, point);
		return 3;
	}

	return 0;
}

uint32 VisualObjectList::handleUpdate(const Common::Point &point) {
	if (!_visible)
		return false;

	Hotspot *hotspot = getHotspot(point);
	if (!hotspot)
		return false;

	switch (hotspot->getCursorId()) {
	default:
		break;

	case kCursorPassive:
		if (hotspot->isEnabled()) {
			getApp()->cursorSelect(kCursorMenuActive);

			if (!_upGur->getNameId().empty())
				getApp()->getScreenManager()->draw(_upGur, _origin + _upOffset, _upGur->getDrawType());
		}

		getApp()->getEventHandler()->onVisualList(_id, 1, point);
		return 1;

	case kCursorActive:
		if (hotspot->isEnabled()) {
			getApp()->cursorSelect(kCursorMenuActive);

			if (!_upGur->getNameId().empty())
				getApp()->getScreenManager()->draw(_downGur, _origin + _downOffset, _downGur->getDrawType());
		}

		getApp()->getEventHandler()->onVisualList(_id, 2, point);
		return 2;

	case kCursorPassiveDraw:
		getApp()->cursorSelect(kCursorMenuActive);

		getApp()->getEventHandler()->onVisualList(_id, 3, point);
		return 3;
	}

	return 0;
}

#pragma endregion

#pragma region Management

void VisualObjectList::add(ObjectId objectId) {
	if (_objects.has(objectId)) {
		_objectIdClicked = 0;
		_objectIndexClicked = -1;
		_imageIndexClicked = -1;

		return;
	}

	Object *object = getApp()->getObject(objectId);
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
		_objectIndex = 0;
	_objectIdClicked = 0;
	_objectIndexClicked = -1;
	_imageIndexClicked = -1;
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
		_objectIndex = 0;
	_objectIdClicked = 0;
	_objectIndexClicked = -1;
	_imageIndexClicked = -1;
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

	_objectIndex = 0;
	_itemCount = _objects.size();
	_objectIdClicked = 0;
	_objectIndexClicked = -1;
	_imageIndexClicked = -1;
}

bool VisualObjectList::isIn(const Common::String &description) {
	for (Common::Array<Object *>::iterator it = _objects.begin(); it != _objects.end(); it++) {
		if ((*it)->getDescription() == description)
			return true;
	}

	return false;
}

void VisualObjectList::resetObjectClicked() {
	_objectIdClicked = kObjectNone;
	_objectIndexClicked = -1;
	_imageIndexClicked = -1;
}

#pragma endregion

} // End of namespace Ring
