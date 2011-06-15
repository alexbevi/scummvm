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

#include "ring/base/puzzle.h"

#include "ring/base/accessibility.h"
#include "ring/base/application.h"
#include "ring/base/bag.h"
#include "ring/base/movability.h"
#include "ring/base/object.h"
#include "ring/base/saveload.h"
#include "ring/base/text.h"

#include "ring/graphics/animation.h"
#include "ring/graphics/hotspot.h"
#include "ring/graphics/image.h"
#include "ring/graphics/screen.h"
#include "ring/graphics/visual.h"

#include "ring/sound/soundentry.h"
#include "ring/sound/soundhandler.h"

#include "ring/ring.h"
#include "ring/helpers.h"

namespace Ring {

Puzzle::Puzzle(Application *application, PuzzleId id) : BaseObject(id), _application(application) {
	_background = NULL;
	_field_24 = 1;
	_field_28 = 0;
	_selectedId = 0;
}

Puzzle::~Puzzle() {
	SAFE_DELETE(_background);

	CLEAR_ARRAY(Movability, _movabilities);
	// Accessibilities are stored in the parent Object
	// Presentation images are stored in parent Object (or in Animation)
	// Presentation animations are stored in parent object
	// Texts are stored in parent Object
	CLEAR_ARRAY(SoundItem, _soundItems);
	CLEAR_ARRAY(Visual, _visuals);

	// Zero-out passed pointers
	_application = NULL;
}

void Puzzle::alloc() {
	if (_background && !_background->isInitialized())
		initializeImage(_background);

	for (Common::Array<ImageHandle *>::iterator it = _presentationImages.begin(); it != _presentationImages.end(); it++) {
		ImageHandle *image = (*it);

		if (!image->getObjectPresentation()
		 || !image->getObjectPresentation()->isShown()
		 || image->isInitialized()
		 || !image->isActive()
		 || image->getField6C() != 1)
			continue;

		initializeImage(image);
	}

	for (Common::Array<Visual *>::iterator it = _visuals.begin(); it != _visuals.end(); it++)
		(*it)->alloc();
}

void Puzzle::initializeImage(ImageHandle *image) const{
	Common::String path;
	switch (image->getArchiveType()) {
	default:
		error("[Puzzle::initializeImage] Invalid archive type!");
		break;

	case kArchiveFile:
		if (image->getLoadFrom() == kLoadFromCd || image->getLoadFrom() == kLoadFromDisk)
			path = Common::String::format("DATA/%s/IMAGE/%s", getApp()->getZoneFolder(image->getZone()).c_str(), image->getNameId().c_str());
		else if (image->getLoadFrom() == kLoadFrom5)
			path = Common::String::format("%s%s", image->getDirectory().c_str(), image->getNameId().c_str());
		else
			error("[Puzzle::initializeImage] Invalid load From!");
		break;

	case kArchiveArt:
		if (image->getLoadFrom() == kLoadFromCd || image->getLoadFrom() == kLoadFromDisk)
			path = Common::String::format("/IMAGE/%s", image->getNameId().c_str());
		else if (image->getLoadFrom() == kLoadFrom5)
			path = Common::String::format("%s%s", image->getDirectory().c_str(), image->getNameId().c_str());
		else
			error("[Puzzle::initializeImage] Invalid load From!");
		break;
	}

	if (!image->load(path, image->getArchiveType(), image->getZone(), image->getLoadFrom(), image->getDrawType()))
		error("[Puzzle::initializeImage] Cannot load image (%s)!", path.c_str());
}

void Puzzle::dealloc() {
	if (_background) {
		if (_background->isInitialized())
			_background->destroy();
	}

	for (Common::Array<ImageHandle *>::iterator it = _presentationImages.begin(); it != _presentationImages.end(); it++) {
		ImageHandle *image = (*it);

		if (!image->isInitialized())
			continue;

		if (image->getField6C()) {
			image->destroy();
		} else if (image->getAnimation()) {
			image->getAnimation()->dealloc();
		} else {
			error("[Puzzle::dealloc] Invalid animation in ImageHandle (%s)", image->getNameId().c_str());
		}
	}

	for (Common::Array<Visual *>::iterator it = _visuals.begin(); it != _visuals.end(); it++)
		(*it)->dealloc();
}

void Puzzle::update() {
	if (_background && _background->isActive()) {

		// Initialize background if needed
		if (!_background->isInitialized())
			initializeImage(_background);

		_application->getScreenManager()->draw(_background, _background->getCoordinates(), kDrawType1);
	}

	// Handle animations
	uint32 ticks = g_system->getMillis();
	if (!_field_28) {
		for (Common::Array<ObjectPresentation *>::iterator it = _presentationAnimations.begin(); it != _presentationAnimations.end(); it++) {
			ObjectPresentation *presentation = (*it);

			presentation->playPuzzleAnimations(ticks);
			presentation->playRotationAnimations(ticks);
		}
	}

	// Handle presentation images
	for (Common::Array<ImageHandle *>::iterator it = _presentationImages.begin(); it != _presentationImages.end(); it++) {
		ImageHandle *image = (*it);

		if (!image->getObjectPresentation())
			continue;

		// De-alloc animation
		if (!image->getObjectPresentation()->isShown()) {
			if (image->getField6C() == 2) {
				if (!image->getAnimation())
					error("[Puzzle::update] Invalid animation in image!");

				if (image->getAnimation()->getField2D() && !image->getAnimation()->getField26())
					image->getAnimation()->dealloc();
			}

			continue;
		}

		if (!image->isActive())
			continue;

		// Draw active animation frame
		if (image->getField6C() != 1) {
			if (!image->getAnimation())
				error("[Puzzle::update] Invalid animation in image!");

			image->getAnimation()->playFrame();

			continue;
		}

		if (!image->isInitialized())
			initializeImage(image);

		_application->getScreenManager()->draw(image, image->getCoordinates(), image->getDrawType());
	}

	// Draw texts
	for (Common::Array<Text *>::iterator it = _texts.begin(); it != _texts.end(); it++)
		_application->getScreenManager()->drawText(*it);

	// Draw Visuals
	for (Common::Array<Visual *>::iterator it = _visuals.begin(); it != _visuals.end(); it++)
		(*it)->draw();
}

void Puzzle::setBackgroundImage(Common::String filename, const Common::Point &point, bool isActive, LoadFrom loadFrom) {
	SAFE_DELETE(_background);

	ZoneId zone = _application->getCurrentZone();
	ArchiveType archiveType = _application->getArchiveType(zone);

	_background = new ImageHandle(filename, point, isActive, zone, loadFrom, kImageTypeBackground, archiveType);
}

void Puzzle::addPresentationText(Text *text) {
	if (!text)
		error("[Puzzle::addPresentationText] Text is not initialized properly");

	_texts.push_back(text);
}

void Puzzle::addPresentationImage(ImageHandle *image) {
	if (!image)
		error("[Puzzle::addPresentationImage] Image is not initialized properly");

	// Insert image into presentations images, and sort by priority
	_presentationImages.push_back(image);

	Common::sort(_presentationImages.begin(), _presentationImages.end(), &Puzzle::imagePriorityCompare);
}

void Puzzle::addPresentationAnimation(ObjectPresentation *objectPresentation) {
	if (!objectPresentation)
		error("[Puzzle::addPresentationAnimation] Object presentation is not initialized properly");

	_presentationAnimations.push_back(objectPresentation);
}

#pragma region Accessibility

void Puzzle::addAccessibility(Accessibility *accessibility) {
	if (!accessibility)
		error("[Puzzle::addAccessibility] Accessibility is not initialized properly");

	_accessibilities.push_back(accessibility);
}

int32 Puzzle::getAccessibilityIndex(const Common::Point &point) {
	for (uint32 i = 0; i < _accessibilities.size(); i++) {
		if (!_accessibilities[i]->getHotspot()->isEnabled())
			continue;

		if (_accessibilities[i]->getHotspot()->contains(point))
			return i;
	}

	return -1;
}

Accessibility *Puzzle::getAccessibility(const Common::Point &point) {
	for (Common::Array<Accessibility *>::iterator it = _accessibilities.begin(); it != _accessibilities.end(); it++) {
		if (!(*it)->getHotspot()->isEnabled())
			continue;

		if ((*it)->getHotspot()->contains(point))
			return (*it);
	}

	return NULL;
}

Accessibility *Puzzle::getAccessibility(Common::KeyCode key) {
	for (Common::Array<Accessibility *>::iterator it = _accessibilities.begin(); it != _accessibilities.end(); it++) {
		if (!(*it)->getHotspot()->isEnabled())
			continue;

		if ((*it)->getHotspot()->getKey() == key)
			return (*it);
	}

	return NULL;
}

#pragma endregion

#pragma region Movability

void Puzzle::addMovability(Movability *movability) {
	if (!movability)
		error("[Puzzle::addMovability] Movability is not initialized properly");

	_movabilities.push_back(movability);
}

void Puzzle::setMovabilityOnOrOff(bool enableHotspot) {
	for (Common::Array<Movability *>::iterator it = _movabilities.begin(); it != _movabilities.end(); it++) {
		if (enableHotspot)
			(*it)->enableHotspot();
		else
			(*it)->disableHotspot();
	}
}

void Puzzle::setMovabilityOnOrOff(bool enableHotspot, uint32 fromMovability, uint32 toMovability) {
	// Check from/to movability
	if (toMovability < fromMovability)
		error("[Puzzle::setMovabilityOnOrOff] From movability (%d) is greater than To movability (%d)", fromMovability, toMovability);

	if (fromMovability >= _movabilities.size())
		error("[Puzzle::setMovabilityOnOrOff] From acceleration is not in range (was:%d, max:%d)", fromMovability, _movabilities.size() - 1);

	if (toMovability >= _movabilities.size())
		error("[Puzzle::setMovabilityOnOrOff] To acceleration is not in range (was:%d, max:%d)", fromMovability, _movabilities.size() - 1);


	for (uint32 i = fromMovability; i <= toMovability; i++) {
		if (enableHotspot)
			_movabilities[i]->enableHotspot();
		else
			_movabilities[i]->disableHotspot();
	}
}

Movability *Puzzle::getMovability(const Common::Point &point) {
	for (Common::Array<Movability *>::iterator it = _movabilities.begin(); it != _movabilities.end(); it++) {
		if (!(*it)->getHotspot()->isEnabled())
			continue;

		if ((*it)->getHotspot()->contains(point))
			return (*it);
	}

	return NULL;
}

int32 Puzzle::getMovabilityIndex(const Common::Point &point) {
	for (uint32 i = 0; i < _movabilities.size(); i++) {
		if (!_movabilities[i]->getHotspot()->isEnabled())
			continue;

		if (_movabilities[i]->getHotspot()->contains(point))
			return i;
	}

	return -1;
}

#pragma endregion

void Puzzle::setMod(uint32 a2, Id selectedId) {
	_field_24 = a2;
	_selectedId = selectedId;

	switch (a2){
	default:
		break;

	case 1:
		getApp()->setFreOffCurrentRotation();
		break;

	case 2:
		getApp()->setupCurrentRotation();

		if (getApp()->getBag() && getApp()->getBag()->isInitialized())
			getApp()->getBag()->reset();

		getApp()->cursorDelete();
		break;
	}
}

#pragma region Visual

void Puzzle::addVisual(Visual *visual) {
	if (!visual)
		error("[Puzzle::addVisual] Visual is not initialized properly");

	if (hasVisual(visual->getId()))
		error("[Puzzle::addVisual] Visual Id already exists (%d)", visual->getId());

	_visuals.push_back(visual);
}

bool Puzzle::visualHandleLeftButtonUp(const Common::Point &point) {
	for (Common::Array<Visual *>::iterator it = _visuals.begin(); it != _visuals.end(); it++) {
		if ((*it)->handleLeftButtonUp(point))
			return true;
	}

	return false;
}

bool Puzzle::visualHandleUpdate(const Common::Point &point) {
	for (Common::Array<Visual *>::iterator it = _visuals.begin(); it != _visuals.end(); it++) {
		if ((*it)->handleUpdate(point))
			return true;
	}

	return false;
}

bool Puzzle::visualHandleLeftButtonDown(const Common::Point &point) {
	for (Common::Array<Visual *>::iterator it = _visuals.begin(); it != _visuals.end(); it++) {
		if ((*it)->handleLeftButtonDown(point))
			return true;
	}

	return false;
}

bool Puzzle::visualHandleKey(Common::KeyCode key) {
	for (Common::Array<Visual *>::iterator it = _visuals.begin(); it != _visuals.end(); it++) {
		if ((*it)->handleKey(key))
			return true;
	}

	return false;
}

#pragma endregion

#pragma region Sound

void Puzzle::addAmbientSound(SoundEntry *entry, uint32 volume, int32 pan, bool isOn, uint32 fadeFrames, uint32 a6, uint32 a7) {
	SoundItem *item = new SoundItem(_id);
	item->init(entry, volume, pan, isOn, fadeFrames, a6, a7);

	_soundItems.push_back(item);
}

void Puzzle::add3DSound(SoundEntry *entry, uint32 volume, bool isOn, uint32 a4, uint32 a5, uint32 fadeFrames, float a7, uint32 a8) {
	SoundItem *item = new SoundItem(_id);
	item->setField1D(a8);
	item->setAngle(a7);
	item->init(entry, volume, item->computePan(0.0), isOn, a4, a5, fadeFrames, a7, a8);

	_soundItems.push_back(item);
}

void Puzzle::setAmbientSoundOn(Id soundId) {
	SoundItem *item = getSoundItem(soundId);
	if (!item)
		error("[Puzzle::setAmbientSoundOff] Wrong sound Id (%d)", soundId);

	item->on();
}

void Puzzle::setAmbientSoundOff(Id soundId) {
	SoundItem *item = getSoundItem(soundId);
	if (!item)
		error("[Puzzle::setAmbientSoundOff] Wrong sound Id (%d)", soundId);

	item->off();
}

void Puzzle::setAmbientSoundVolume(Id soundId, int32 volume) {
	SoundItem *item = getSoundItem(soundId);
	if (!item)
		error("[Puzzle::setAmbientSoundOff] Wrong sound Id (%d)", soundId);

	item->setVolume(volume);
}

void Puzzle::updateSoundItems() {
	for (Common::Array<SoundItem *>::iterator it = _soundItems.begin(); it != _soundItems.end(); it++) {
		SoundItem *item = (*it);

		if (!item->isOn()) {
			item->turnOff();
			continue;
		}

		if (!item->getSoundEntry() || !item->getSoundEntry()->isPlaying())
			item->turnOn();
	}
}

#pragma endregion

#pragma region Accessors

Movability *Puzzle::getMovability(uint32 index) {
	if (index >= _movabilities.size())
		error("[Puzzle::getMovability] Invalid movability index (was: %d, max:%d)", index, _movabilities.size() - 1);

	return _movabilities[index];
}

#pragma endregion

#pragma region Helpers

SoundItem *Puzzle::getSoundItem(Id soundId) {
	for (Common::Array<SoundItem *>::iterator i = _soundItems.begin(); i != _soundItems.end(); i++) {
		if ((*i)->getSoundEntry()->getId() == soundId)
			return *i;
	}

	return NULL;
}

bool Puzzle::imagePriorityCompare(ImageHandle *image1, ImageHandle *image2) {
	return (image1->getPriority() > image2->getPriority());
}

#pragma endregion

#pragma region Serializable

void Puzzle::saveLoadWithSerializer(Common::Serializer &s) {
	SaveManager::syncWithFlag(s, _background);

	SaveManager::syncArray(s, &_movabilities);
	SaveManager::syncArray(s, &_soundItems);
	SaveManager::syncArray(s, &_visuals);

	s.syncAsUint32LE(_field_24);
	s.syncAsByte(_field_28);
	s.syncAsUint32LE(_selectedId);
}

#pragma endregion

} // End of namespace Ring
