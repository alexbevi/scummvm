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

#include "ring/base/bag.h"

#include "ring/base/application.h"
#include "ring/base/event.h"
#include "ring/base/object.h"
#include "ring/base/saveload.h"
#include "ring/base/text.h"

#include "ring/graphics/animation.h"
#include "ring/graphics/hotspot.h"
#include "ring/graphics/image.h"
#include "ring/graphics/screen.h"

#include "ring/ring.h"

#include "ring/helpers.h"

#include "common/file.h"

namespace Ring {

Bag::Bag() {
	_field_C = 0;
	_field_10 = 0;
	_field_14 = 0;
	_field_18 = 0;
	_field_24 = 0;
	_field_28 = 0;
	_objectCount = 0;
	_background = NULL;
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
	_image5 = NULL;
	_image6 = NULL;
	_image7 = NULL;
	_field_74 = 0;
	_field_78 = 0;
	_field_7C = 0;
	_field_80 = 0;
	_image8 = NULL;
	_field_88 = 0;
	_field_8C = 0;
	_field_90 = 6;
	_isInitialized = false;
	_clickedObject = kObjectInvalid;
	_field_99 = 0;
	_tickInterval = 0;
	_ticks = 0;
	_text = NULL;
	_fontId = kFontDefault;
	_foregroundColor = Color(245, 235, 50);
	_backgroundColor = Color(0, 0, 0);
	_field_C5 = 90;
	_archiveType = kArchiveInvalid;
	_imageErdaGun = NULL;
	_imageErdaGur = NULL;
	_enabled = false;

	_fre = false;

	// Flags
	_drawImage8 = false;
	_drawImageErdaGur = false;
}

Bag::~Bag() {
	CLEAR_ARRAY(Object, _objects);
	CLEAR_ARRAY(ImageHandle, _images);
	CLEAR_ARRAY(Hotspot, _hotspots);

	SAFE_DELETE(_background);
	SAFE_DELETE(_image2);
	SAFE_DELETE(_image3);
	SAFE_DELETE(_image4);
	SAFE_DELETE(_image5);
	SAFE_DELETE(_image6);
	SAFE_DELETE(_image7);
	SAFE_DELETE(_image8);
	SAFE_DELETE(_text);
	SAFE_DELETE(_imageErdaGun);
	SAFE_DELETE(_imageErdaGur);
}

#pragma region Initialization

void Bag::initHotspots() {
	_hotspots.push_back(new Hotspot(Common::Rect(_origin.x + (int16)_field_48,
	                                             _origin.y + (int16)_field_4C,
	                                             _origin.x + (int16)(_field_48 + _field_50),
	                                             _origin.y + (int16)(_field_4C + _field_54)),
	                                false,
	                                1000,
	                                kCursor1001,
	                                1001));

	_hotspots.push_back(new Hotspot(Common::Rect(_origin.x + (int16)_field_74,
	                                             _origin.y + (int16)_field_78,
	                                             _origin.x + (int16)(_field_74 + _field_7C),
	                                             _origin.y + (int16)(_field_78 + _field_80)),
	                                false,
	                                1000,
	                                kCursor1002,
	                                1002));

	_hotspots.push_back(new Hotspot(Common::Rect(200, 0, 640, 30),
	                                true,
	                                1000,
	                                kCursor1003,
	                                1003));

	_hotspots.push_back(new Hotspot(Common::Rect(90, 0, 150, 30),
	                                _enabled,
	                                1000,
	                                kCursor1005,
	                                1005));

	for (uint32 i = 0; i < _field_24; ++i)
		_hotspots.push_back(new Hotspot(Common::Rect(_origin.x + (int16)(_field_C + i * _field_18 + 1),
													 _origin.y + (int16)(_field_10),
													 _origin.x + (int16)(_field_18 * (i + 1) + _field_C - 1),
													 _origin.y + (int16)(_field_10 + _field_14 + 1)),
										false,
										1000,
										(CursorId)i,
										1004));
}

void Bag::sub_417D40(uint32 a1, uint32 a2, uint32 a3, uint32 a4) {
	_field_C = a1;
	_field_10 = a2;
	_field_14 = a3;
	_field_18 = a4;
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

void Bag::sub_417E00() {
	for (Common::Array<ImageHandle *>::iterator it = _images.begin(); it != _images.end(); it++) {
		ImageHandle *image = (*it);

		if (image->getField6C() == 2) {
			if (image->getAnimation()) {
				image->getAnimation()->sub_416710();
				image->getAnimation()->dealloc();
			}
		} else {
			image->destroy();
		}
	}
}

uint32 Bag::checkHotspotClick(const Common::Point &point) {
	Hotspot *hotspot = getHotspot(point);
	if (!hotspot)
		return 0;

	switch (hotspot->getCursorId()) {
	default: {
		uint32 index = _field_28 + hotspot->getCursorId();

		if (index < _objects.size())
			_clickedObject = _objects[index]->getId();
		else
			_clickedObject = kObjectInvalid;
		}
		return 1;

	case kCursor1001:
		if (_field_28 > 0) {
			--_field_28;
			_ticks = g_system->getMillis();
		}
		return 2;

	case kCursor1002:
		if (_field_28 < _objectCount) {
			++_field_28;
			_ticks = g_system->getMillis();
		}
		return 2;

	case kCursor1003:
		getApp()->startMenu(true);
		return 3;

	case kCursor1005:
		getApp()->onBagZoneSwitch();
		return 4;
	}
}

uint32 Bag::checkHotspot(const Common::Point &point) {
	Hotspot *hotspot = getHotspot(point);
	if (!hotspot)
		return 0;

	switch (hotspot->getTarget()) {
	default:
		break;

	case 1001:
		if (_field_28 && (g_system->getMillis() - _ticks) > _tickInterval) {
			--_field_28;
			_ticks = g_system->getMillis();
		}
		return 1;

	case 1002:
		if (_field_28 && (g_system->getMillis() - _ticks) > _tickInterval) {
			++_field_28;
			_ticks = g_system->getMillis();
		}
		return 2;

	case 1003:
		_drawImage8 = true;
		break;

	case 1004: {
		uint32 index = _field_28 + hotspot->getCursorId();

		// Show object description
		if (index < _objects.size()) {
			Object *object = _objects[index];

			if (!object->getDescription().empty()) {
				_text->set(object->getDescription());

				// Compute coordinates
				ImageHandle *image = _images[index];

				int16 x = image->getCoordinates().x + _text->getCoordinates().x;
				if (x < 0)
					x = 0;

				if (x > 640)
					x = 640 - _text->getCoordinates().x;

				_text->setCoordinates(Common::Point(x, image->getCoordinates().y));

				// Draw description
				getApp()->getScreenManager()->drawText(_text);
			}
		}
		}
		return 3;

	case 1005:
		if (_enabled)
			_drawImageErdaGur = true;
		break;
	}

	return 0;
}

Hotspot *Bag::getHotspot(const Common::Point &point) {
	for (Common::Array<Hotspot *>::iterator it = _hotspots.begin(); it != _hotspots.end(); it++) {
		if ((*it)->contains(point))
			return (*it);
	}

	return NULL;
}

void Bag::draw(){
	if (!_background || !_image3 || !_image6 || !_image8)
		return;

	if (!_background->isInitialized()
	 || !_image3->isInitialized()
	 || !_image6->isInitialized()
	 || !_image8->isInitialized())
		return;

	uint32 ticks = g_system->getMillis();
	ScreenManager *screen = getApp()->getScreenManager();

	// Background
	screen->draw(_background, Common::Point(_origin.x + _backgroundOffset.x, _origin.y + _backgroundOffset.y), kDrawType3);

	// Bag elements
	if (_field_28 == 0) {
		if (_hotspots.size() > 0)
			_hotspots[0]->disable();
	} else {
		screen->draw(_image3, Common::Point(_origin.x + (int16)_field_58, _origin.y + (int16)_field_5C), kDrawType3);

		if (_hotspots.size() > 0)
			_hotspots[0]->enable();
	}

	if ((_field_28 + _field_24) >= _objectCount) {
		if (_hotspots.size() > 1)
			_hotspots[1]->disable();
	} else {
		screen->draw(_image6, Common::Point(_origin.x + (int16)_field_60, _origin.y + (int16)_field_64), kDrawType3);

		if (_hotspots.size() > 1)
			_hotspots[1]->enable();
	}

	if (_drawImage8) {
		screen->draw(_image8, Common::Point(_origin.x + (int16)_field_88, _origin.y + (int16)_field_8C), kDrawType3);

		_drawImage8 = false;
	}

	if (_enabled) {
		if (_drawImageErdaGur) {
			if (_imageErdaGur)
				screen->draw(_imageErdaGur, Common::Point(_origin.x + 103, _origin.y), kDrawType3);

			_drawImageErdaGur = false;
		} else {
			if (_imageErdaGun)
				screen->draw(_imageErdaGun, Common::Point(_origin.x + 103, _origin.y), kDrawType3);
		}
	}

	// Show objects
	int16 offset = (int16)_field_C + _origin.x;
	uint32 count = _field_28 + _field_24;
	if (count >= _objectCount)
		count = _objectCount;

	for (uint32 i = _field_28; i < count; i++) {
		ImageHandle *image = _images[i];

		if (image->getField6C() == 1) {
			bool loaded = true;
			if (!image->isInitialized()) {
				// Compute image path
				Common::String filename;
				switch (image->getArchiveType()) {
				default:
					error("[Bag::draw] Invalid archive type %d for image", image->getArchiveType());
					break;

				case kArchiveFile:
					filename = Common::String::format("DATA/LSTICON/%s.tga", _objects[i]->getName().c_str());
					break;

				case kArchiveArt:
					filename = Common::String::format("/LSTICON/%s.tga", _objects[i]->getName().c_str());
					break;
				}

				loaded = image->load(filename, image->getArchiveType(), image->getZone(), image->getLoadFrom(), image->getDrawType());
			}

			if (loaded)
				screen->draw(image, Common::Point(offset + (int16)(_field_18 + image->getWidth()) / 2, _origin.y + (int16)_field_10), kDrawType3);

		} else {
			AnimationImage *animation = image->getAnimation();
			animation->allocActive();

			// Get active frame to draw
			animation->updateCurrentImage();
			animation->computeCurrentFrame(ticks);
			animation->playFrame(Common::Point(offset + (int16)(_field_18 + animation->getCurrentImage()->getWidth()) / 2, _origin.y + (int16)_field_10));
		}

		// Update image coordinates
		image->setCoordinates(Common::Point(offset + (int16)_field_18 / 2, (int16)_field_C5));

		offset += (int16)_field_18;
	}

	for (uint32 i = 0; i < _field_24; i++) {
		if (i >= (count - _field_28))
			_hotspots[i + 4]->disable();
		else
			_hotspots[i + 4]->enable();
	}
}

void Bag::sub_419280(uint32 a1) {
	_tickInterval = a1;
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

void Bag::initialize() {
	_isInitialized = true;

	for (Common::Array<ImageHandle *>::iterator it = _images.begin(); it != _images.end(); it++) {
		if ((*it)->getField6C() != 2)
			continue;

		Animation *animation = (*it)->getAnimation();
		if (animation)
			animation->setTicks(g_system->getMillis());
	}
}

void Bag::reset() {
	_isInitialized = false;

	for (uint i = 0; i < _images.size();) {
		if (_images[i]->getField6C() == 2) {

			AnimationImage *animation = _images[i]->getAnimation();
			if (animation) {
				animation->sub_416710();
				animation->dealloc();
			}

			++i;
		} else {
			SAFE_DELETE(_images[i]);
			_images.remove_at(i);
		}
	}
}

void Bag::loadBackground(Common::String background, Common::String, Common::String filename3, Common::String, Common::String, Common::String filename6, Common::String, Common::String filename8, ArchiveType archiveType, bool loadAdditionalImages) {
	_archiveType = archiveType;

	// Load images
	loadImage(background, &_background, archiveType);

	if (!filename3.empty())
		loadImage(filename3, &_image3, archiveType);

	if (!filename6.empty())
		loadImage(filename6, &_image6, archiveType);

	if (!filename8.empty())
		loadImage(filename8, &_image8, archiveType);

	if (loadAdditionalImages) {
		loadImage("erda_gun.tga", &_imageErdaGun, archiveType);
		loadImage("erda_gur.tga", &_imageErdaGur, archiveType);
	}

	// Setup text
	SAFE_DELETE(_text);
	_text = new Text();
	_text->init("", Common::Point(0, 0), _fontId, _foregroundColor, _backgroundColor);
}

void Bag::loadImage(Common::String filename, Image **image, ArchiveType archiveType) const {
	Common::String path;

	if (archiveType == kArchiveFile)
		path = Common::String::format("LIST/%s", filename.c_str());
	else
		path = Common::String::format("/LIST/%s", filename.c_str());

	SAFE_DELETE(*image);
	*image = new Image();

	if (!(*image)->load(path, archiveType, getZoneSY(), kLoadFromDisk, kDrawType1))
		error("[Bag::LoadImage] Cannot load image: %s", path.c_str());
}

#pragma endregion

#pragma region Enable/Disable

void Bag::enable() {
	if (_hotspots.empty() || _hotspots.size() <= 3)
		return;

	_hotspots[3]->enable();
	_enabled = true;
}

void Bag::disable() {
	if (_hotspots.empty() || _hotspots.size() <= 3)
		return;

	_hotspots[3]->disable();
	_enabled = false;
}

#pragma endregion

#pragma region Management

void Bag::add(ObjectId objectId) {
	if (_objects.has(objectId))
		return;

	Object *object = getApp()->getObject(objectId);
	_objects.push_back(object);

	// Setup animation image
	ImageHandle *imageHandle = NULL;
	AnimationImage *animationImage = object->getAnimationImage();
	if (animationImage) {
		animationImage->updateCurrentImage();
		imageHandle = animationImage->getCurrentImage();
	} else {
		Common::String filename;
		if (_archiveType == kArchiveFile) {
			filename = Common::String::format("LSTICON/%s.tga", object->getName().c_str());

			if (!Common::File::exists(filename)) {
				if (object->getName().empty()) {
					filename = "dummy.tga";
				} else {
					filename = Common::String::format("%s.tga", object->getName().c_str());
				}
			}
		} else {
			filename = Common::String::format("/LSTICON/%s.tga", object->getName().c_str());
		}

		imageHandle = new ImageHandle(filename, Common::Point(0, 0), true, getZoneSY(), kLoadFromDisk, kImageTypeTGA, _archiveType);
	}

	if (!imageHandle)
		error("[Bag::add] Invalid image handle!");

	_images.push_back(imageHandle);

	_objectCount = _objects.size();
	if (_objectCount > _field_24)
		_field_28 = 0;
}

void Bag::remove(ObjectId objectId) {
	if (!_objects.has(objectId))
		return;

	uint32 index = _objects.getIndex(objectId);

	if (_images[index]->getField6C() == 1) {
		// Stores references to existing objects, so we should not delete them
		_objects.remove_at(index);

		SAFE_DELETE(_images[index]);
		_images.remove_at(index);
	} else {
		AnimationImage *image = _objects[index]->getAnimationImage();
		if (image)
			image->dealloc();

		// Stores references to existing objects, so we should not delete them!
		_objects.remove_at(index);

		// Stores a reference to the object current image, so we should not delete it!
		_images.remove_at(index);
	}

	_objectCount = _objects.size();
	if (_objectCount <= _field_24)
		_field_28 = 0;
}

void Bag::removeAll() {
	for (uint32 i = 0; i < _objects.size(); i++)
		remove(_objects[i]->getId());
}

bool Bag::has(ObjectId objectId) {
	return _objects.has(objectId);
}

#pragma endregion

#pragma region Serializable

void Bag::saveLoadWithSerializer(Common::Serializer &s) {
	uint32 count = _objects.size();
	s.syncAsUint32LE(count);

	// Store each object id
	if (s.isSaving()) {
		for (Common::Array<Object *>::iterator it = _objects.begin(); it != _objects.end(); it++) {
			Id id = (*it)->getId();
			s.syncAsUint32LE(id);
		}
	}

	// Add a new object into the bag for each stored object
	if (s.isLoading()) {
		_objects.clear();

		for (uint32 i = 0; i < count; i++) {
			Id id = 0;
			s.syncAsUint32LE(id);

			add(id);
		}
	}

	s.syncAsUint32LE(_clickedObject);
}

#pragma endregion

} // End of namespace Ring
