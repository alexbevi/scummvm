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

#include "ring/game/puzzle.h"

#include "ring/base/text.h"
#include "ring/base/sound.h"

#include "ring/game/application.h"

#include "ring/graphics/accessibility.h"
#include "ring/graphics/image.h"
#include "ring/graphics/movability.h"
#include "ring/graphics/presentation.h"

#include "ring/helpers.h"

namespace Ring {

Puzzle::Puzzle(Application *application, PuzzleId id) : BaseObject(id), _application(application) {
	_background = NULL;
	_field_24 = 1;
	_field_28 = 0;
	_field_29 = 0;
}

Puzzle::~Puzzle() {
	SAFE_DELETE(_background);

	CLEAR_ARRAY(Movability, _movabilities);
	CLEAR_ARRAY(Accessibility, _accessibilities);
	CLEAR_ARRAY(ImageHandle, _presentationImages);
	CLEAR_ARRAY(ObjectPresentation, _presentationAnimations);
	CLEAR_ARRAY(Text, _texts);
	CLEAR_ARRAY(SoundItem, _soundItems);
	CLEAR_ARRAY(Visual, _visuals);

	// Zero-out passed pointers
	_application = NULL;
}


void Puzzle::setBackgroundImage(Common::String filename, uint32 a3, uint32 a4, bool isActive, LoadFrom loadFrom) {
	SAFE_DELETE(_background);

	Zone zone = _application->getCurrentZone();
	ArchiveType archiveType = _application->getReadFrom(zone);

	_background = new ImageHandle(filename, a3, a4, isActive, zone, loadFrom, 4, archiveType);
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

void Puzzle::addAccessibility(Accessibility *accessibility) {
	if (!accessibility)
		error("[Puzzle::addAccessibility] Accessibility is not initialized properly");

	_accessibilities.push_back(accessibility);
}

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
		error("[Object::setMovabilityOnOrOff] From movability (%d) is greater than To movability (%d)", fromMovability, toMovability);

	if (fromMovability < 0 || fromMovability >= _movabilities.size())
		error("[Object::setMovabilityOnOrOff] From acceleration is not in range (was:%d, max:%d)", fromMovability, _movabilities.size() - 1);

	if (toMovability < 0 || toMovability >= _movabilities.size())
		error("[Object::setMovabilityOnOrOff] To acceleration is not in range (was:%d, max:%d)", fromMovability, _movabilities.size() - 1);


	for (uint32 i = fromMovability; i <= toMovability; i++) {
		if (enableHotspot)
			_movabilities[i]->enableHotspot();
		else
			_movabilities[i]->disableHotspot();
	}
}

#pragma endregion

void Puzzle::addAmbientSound(SoundEntry *entry, uint32 volume, uint32 a3, bool isOn, uint32 fadeFrames, uint32 a6, uint32 a7) {
	SoundItem *item = new SoundItem(_id);
	item->init(entry, volume, a3, isOn, fadeFrames, a6, a7);

	_soundItems.push_back(item);
}

void Puzzle::add3DSound(SoundEntry *entry, uint32 volume, bool isOn, uint32 a4, uint32 a5, uint32 fadeFrames, float a7, uint32 a8) {
	SoundItem *item = new SoundItem(_id);
	item->setField1D(a8);
	item->setAngle(a7);
	item->init(entry, volume, item->computeFieldC(0.0), isOn, a4, a5, fadeFrames, a7, a8);

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

void Puzzle::setAmbientSoundVolume(Id soundId, uint32 volume) {
	SoundItem *item = getSoundItem(soundId);
	if (!item)
		error("[Puzzle::setAmbientSoundOff] Wrong sound Id (%d)", soundId);

	item->setVolume(volume);
}


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

} // End of namespace Ring
