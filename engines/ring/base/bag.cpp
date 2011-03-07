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

#include "ring/base/bag.h"

#include "ring/base/application.h"
#include "ring/base/object.h"
#include "ring/base/text.h"

#include "ring/graphics/animation.h"
#include "ring/graphics/hotspot.h"
#include "ring/graphics/image.h"

#include "ring/ring.h"

#include "ring/helpers.h"

#include "common/file.h"

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
	_objectCount = 0;
	_field_30 = 0;
	_field_34 = 0;
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
	_field_94 = false;
	_clickedObject = kObjectInvalid;
	_field_99 = 0;
	_field_9D = 0;
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
	_hotspots.push_back(new Hotspot(Common::Rect((int16)(_field_48 + _field_1C),
	                                             (int16)(_field_4C + _field_20),
	                                             (int16)(_field_48 + _field_1C + _field_50),
	                                             (int16)(_field_4C + _field_20 + _field_54)),
	                                false,
	                                1000,
	                                1001,
	                                1001));

	_hotspots.push_back(new Hotspot(Common::Rect((int16)(_field_1C + _field_74),
	                                             (int16)(_field_20 + _field_78),
	                                             (int16)(_field_1C + _field_74 + _field_7C),
	                                             (int16)(_field_20 + _field_78 + _field_80)),
	                                false,
	                                1000,
	                                1002,
	                                1002));

	_hotspots.push_back(new Hotspot(Common::Rect(200, 0, 640, 30),
	                                true,
	                                1000,
	                                1003,
	                                1003));

	_hotspots.push_back(new Hotspot(Common::Rect(90, 0, 150, 30),
	                                _enabled,
	                                1000,
	                                1005,
	                                1005));

	for (uint32 i = 0; i < _field_24; ++i)
		_hotspots.push_back(new Hotspot(Common::Rect((int16)(_field_C + i * _field_18 + _field_1C + 1),
													 (int16)(_field_20 + _field_10),
													 (int16)(_field_18 * (i + 1) + _field_C + _field_1C - 1),
													 (int16)(_field_10 + _field_14 + 1 + _field_20)),
										false,
										1000,
										i,
										1004));
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

void Bag::sub_417E00() {
	error("[Bag::sub_417E00] Not implemented!");
}

uint32 Bag::checkHotspots(const Common::Point &point) {
	error("[Bag::checkHotspots] Not implemented!");
}

bool Bag::sub_418A70(const Common::Point &point) {
	error("[Bag::sub_418A70] Not implemented!");
}

void Bag::draw(){
	error("[Bag::draw] Not implemented!");
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

void Bag::sub_4192E0() {
	_field_94 = true;

	for (Common::Array<ImageHandle *>::iterator it = _images.begin(); it != _images.end(); it++) {
		if ((*it)->getField6C() != 2)
			continue;

		Animation *animation = (*it)->getAnimation();
		if (animation)
			animation->setTicks(g_system->getMillis());
	}
}

void Bag::sub_419350() {
	_field_94 = false;

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

void Bag::loadBackground(Common::String filename1, Common::String, Common::String filename3, Common::String, Common::String, Common::String filename6, Common::String, Common::String filename8, ArchiveType archiveType) {
	_archiveType = archiveType;

	// Load images
	loadImage(filename1, &_background, archiveType);
	loadImage(filename3, &_image3, archiveType);
	loadImage(filename6, &_image6, archiveType);
	loadImage(filename8, &_image8, archiveType);
	loadImage("erda_gun.tga", &_imageErdaGun, archiveType);
	loadImage("erda_gur.tga", &_imageErdaGur, archiveType);

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

	if (!(*image)->load(path, archiveType, kZoneSY, kLoadFromDisk))
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

		imageHandle = new ImageHandle(filename, Common::Point(0, 0), true, kZoneSY, kLoadFromDisk, kImageTypeTGA, _archiveType);
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
