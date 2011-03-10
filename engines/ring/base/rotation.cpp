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

#include "ring/base/rotation.h"

#include "ring/base/application.h"
#include "ring/base/accessibility.h"
#include "ring/base/application.h"
#include "ring/base/movability.h"
#include "ring/base/object.h"
#include "ring/base/saveload.h"
#include "ring/base/sound.h"
#include "ring/base/text.h"

#include "ring/graphics/animation.h"
#include "ring/graphics/hotspot.h"
#include "ring/graphics/image.h"
#include "ring/graphics/screen.h"

#include "ring/ring.h"

namespace Ring {

#pragma region ImageHeaderEntry

ImageHeaderEntry::ImageHeaderEntry() {
	_field_0 = 0;
	_field_4 = 0;
	_field_8 = 0;
	_field_1C = 0;
	_field_20 = 0;
	_field_24 = 0;
	_field_28 = 0;
	_buffer = NULL;
	_field_38 = 0;
	_field_3C = 0;
}

ImageHeaderEntry::~ImageHeaderEntry() {
	SAFE_DELETE(_buffer);
}

#pragma endregion

#pragma region ImageHeader

ImageHeader::ImageHeader() {
	_field_0 = 0;
	_field_4 = 0;
	_field_4C = -1;
}

ImageHeader::~ImageHeader() {
	CLEAR_ARRAY(ImageHeaderEntry, _headers);
}

#pragma endregion

#pragma region AquatorImageHeader

AquatorImageHeader::AquatorImageHeader() {
	_field_0 = 0;
	_field_4 = 0;
	_field_8 = 0;
	_channel = 0;
}

AquatorImageHeader::~AquatorImageHeader() {
}

void AquatorImageHeader::setChannel(uint32 channel) {
	if (_channel != channel) {
 		_channel = channel;
 		_field_4 = 1;
 	}
}

#pragma endregion

#pragma region AquatorStream

AquatorStream::AquatorStream(uint32 count, Common::String path) {
	_entry = new ImageHeaderEntry();

	_path = path;

	for (uint32 i = 0; i < count; i++)
		_headers.push_back(new AquatorImageHeader());
}

AquatorStream::~AquatorStream() {
	CLEAR_ARRAY(AquatorImageHeader, _headers);
}

void AquatorStream::alloc(bool isCompressed, byte a2, int a3, byte a4, int a5, byte a6, uint32 a7, uint32 size) {
	error("[AquatorStream::alloc] Not implemented");
}

void AquatorStream::dealloc() {
	CLEAR_ARRAY(AquatorImageHeader, _headers);

	delete _entry;
	_entry = new ImageHeaderEntry();
}

void AquatorStream::setChannel(uint32 index, uint32 channel) {
	_headers[index]->setChannel(channel);
}

uint32 AquatorStream::getChannel(uint32 index) {
	return _headers[index]->getChannel();
}

#pragma endregion

#pragma region Rotation

Rotation::Rotation(Id id, Common::String name, byte a3, LoadFrom, uint32 nodeCount, bool isCompressedStream) : BaseObject(id) {
	_isCompressedStream = isCompressedStream;
	_comBufferLength = 0;

	// Compute path (Original checks loadFrom)
	_path = Common::String::format("DATA/%s/NODE/%s", getApp()->getCurrentZoneString().c_str(), name.c_str());

	// Create animation for each node
	for (uint32 i = 0; i < nodeCount; i++)
		_animations.push_back(new Animation());

	_field_28 = a3;

	// Init stream
	_stream = new AquatorStream(nodeCount, _path);

	_field_31  = 1.0f;
	_field_35  = 0.3f;
	_amplitude = 30.0f;
	_field_3D  = 1.0f;
	_speed     = 1.0f;
	_field_45  = 0;
	_field_49  = 0;
	_field_4D  = 0;
	_field_51  = 0;
	_field_55  = 0;
	_field_59  = 0;
	_field_5D  = 0;
	_field_61  = 0;
	_field_65  = 0;
	_field_66  = 0;
	_fre       = false;
	_alp       = 225.0f;
	_bet       = 0;
	_ran       = 85.0f;

	_imageHandle = NULL;
}

Rotation::~Rotation() {
	CLEAR_ARRAY(Movability,         _movabilities);
	// Accessibilities are stored in the parent Object
	// Presentations are stored in the parent Object
	// Animations are stored in the parent Object
	CLEAR_ARRAY(Text,               _texts);
	CLEAR_ARRAY(SoundItem,          _soundItems);

	SAFE_DELETE(_imageHandle);
	SAFE_DELETE(_stream);
}

void Rotation::alloc() {
	error("[Rotation::alloc] Not implemented");
}

void Rotation::dealloc() {
	if (_stream->isInitialized())
		_stream->dealloc();
}

void Rotation::update() {
	error("[Rotation::update] Not implemented");
}

void Rotation::updateAndDraw(float alp, float bet, float ran) {
	error("[Rotation::updateAndDraw] Not implemented");
}

void Rotation::setCoordinates(const Common::Point &point){
	error("[Rotation::setCoordinates] Not implemented");
}

void Rotation::loadImage() {
	if (!_imageHandle || _imageHandle->isInitialized())
		return;

	if (!_imageHandle->isActive())
		return;

	// Compute filename
	Common::String filename;
	switch (_imageHandle->getArchiveType()) {
	default:
		error("[Rotation::drawImage] Invalid archive type (%d)", _imageHandle->getArchiveType());

	case kArchiveFile:
		// Original checks if we are loading from cd or from disk
		filename = Common::String::format("DATA/%s/IMAGE/%s", getApp()->getZoneString(_imageHandle->getZone()).c_str(), _imageHandle->getNameId().c_str());
		break;

	case kArchiveArt:
		filename = Common::String::format("/DATA/%s", _imageHandle->getNameId().c_str());
		break;
	}

	_imageHandle->load(filename, _imageHandle->getArchiveType(), _imageHandle->getZone(), _imageHandle->getLoadFrom());
}

void Rotation::destroyImage() {
	if (_imageHandle && _imageHandle->isInitialized())
		_imageHandle->destroy();
}

void Rotation::drawImage(ScreenManager *screen) {
	if (_imageHandle)
		screen->draw(_imageHandle, _imageHandle->getCoordinates(), kDrawType1);
}

void Rotation::draw(ScreenManager *screen) {
	error("[Rotation::draw] Not implemented");
}

void Rotation::drawText() {
	error("[Rotation::drawText] Not implemented");
}


void Rotation::addAccessibility(Accessibility *accessibility) {
	if (accessibility == NULL)
		error("[Rotation::addAccessibility] Accessibility is NULL!");

	_accessibilities.push_back(accessibility);
}

int32 Rotation::getAccessibilityIndex(const Common::Point &point) {
	for (uint32 i = 0; i < _accessibilities.size(); i++) {
		if (!_accessibilities[i]->getHotspot()->isEnabled())
			continue;

		if (_accessibilities[i]->getHotspot()->contains(point))
			return i;
	}

	return -1;
}

Accessibility *Rotation::getAccessibility(const Common::Point &point) {
	for (Common::Array<Accessibility *>::iterator it = _accessibilities.begin(); it != _accessibilities.end(); it++) {
		if (!(*it)->getHotspot()->isEnabled())
			continue;

		if ((*it)->getHotspot()->contains(point))
			return (*it);
	}

	return NULL;
}

Accessibility *Rotation::getAccessibility(Common::KeyCode key) {
	for (Common::Array<Accessibility *>::iterator it = _accessibilities.begin(); it != _accessibilities.end(); it++) {
		if (!(*it)->getHotspot()->isEnabled())
			continue;

		if ((*it)->getHotspot()->getKey() == key)
			return (*it);
	}

	return NULL;
}

void Rotation::addMovability(Movability *movability) {
	if (movability == NULL)
		error("[Rotation::addMovability] Movability is NULL!");

	_movabilities.push_back(movability);
}

void Rotation::setMovabilityOnOrOff(bool enableHotspot) {
	for (Common::Array<Movability *>::iterator it = _movabilities.begin(); it != _movabilities.end(); it++) {
		if (enableHotspot)
			(*it)->enableHotspot();
		else
			(*it)->disableHotspot();
	}
}

void Rotation::setMovabilityOnOrOff(bool enableHotspot, uint32 fromMovability, uint32 toMovability) {
	// Check from/to movability
	if (toMovability < fromMovability)
		error("[Rotation::setMovabilityOnOrOff] From movability (%d) is greater than To movability (%d)", fromMovability, toMovability);

	if (fromMovability >= _movabilities.size())
		error("[Rotation::setMovabilityOnOrOff] From acceleration is not in range (was:%d, max:%d)", fromMovability, _movabilities.size() - 1);

	if (toMovability >= _movabilities.size())
		error("[Rotation::setMovabilityOnOrOff] To acceleration is not in range (was:%d, max:%d)", fromMovability, _movabilities.size() - 1);


	for (uint32 i = fromMovability; i <= toMovability; i++) {
		if (enableHotspot)
			_movabilities[i]->enableHotspot();
		else
			_movabilities[i]->disableHotspot();
	}
}

Movability *Rotation::getMovability(uint32 index) {
	if (index >= _movabilities.size())
		error("[Rotation::getMovability] Invalid movability index (was: %d, max:%d)", index, _movabilities.size() - 1);

	return _movabilities[index];
}

int32 Rotation::getMovabilityIndex(const Common::Point &point) {
	for (uint32 i = 0; i < _movabilities.size(); i++) {
		if (!_movabilities[i]->getHotspot()->isEnabled())
			continue;

		if (_movabilities[i]->getHotspot()->contains(point))
			return i;
	}

	return -1;
}

Movability *Rotation::getMovability(const Common::Point &point) {
	for (Common::Array<Movability *>::iterator it = _movabilities.begin(); it != _movabilities.end(); it++) {
		if (!(*it)->getHotspot()->isEnabled())
			continue;

		if ((*it)->getHotspot()->contains(point))
			return (*it);
	}

	return NULL;
}

void Rotation::setMovabilityRideName(uint32 movabilityIndex, Common::String name) {
	if (movabilityIndex >= _movabilities.size())
		error("[Rotation::setMovabilityRideName] Invalid movability index (was:%d, max:%d)", movabilityIndex, _movabilities.size() - 1);

	_movabilities[movabilityIndex]->setRideName(name);
}

bool Rotation::setRolTo(float a2, float a3, float a4) {
	error("[Rotation::SetRolTo] Not implemented");
}

Animation *Rotation::addPresentationAnimation(ObjectPresentation *presentation, uint32 layer, uint32 frameCount, float frameRate, byte a5) {
	if (!presentation)
		error("[Rotation::addPresentationAnimation] Presentation is NULL!");

	if (layer >= _animations.size())
		error("[Rotation::addPresentationAnimation] Invalid layer index (was: %d, max:%d)", layer, _animations.size() - 1);

	Animation *animation = _animations[layer];
	animation->init(frameCount, frameRate, 1, a5, 0);

	_presentations.push_back(presentation);

	return animation;
}

void Rotation::setChannel(uint32 index, uint32 channel) {
	if (!_stream)
		error("[Rotation::getLayerCount] Stream not initialized!");

	_stream->setChannel(index, channel);
}

uint32 Rotation::getChannel(uint32 index) {
	if (!_stream)
		error("[Rotation::getLayerCount] Stream not initialized!");

	return _stream->getChannel(index);
}

uint32 Rotation::getLayerCount() {
	if (!_stream)
		error("[Rotation::getLayerCount] Stream not initialized!");

	return _stream->getCount();
}

void Rotation::addAmbientSound(SoundEntry *entry, uint32 volume, int32 pan, bool isOn, uint32 fadeFrames, uint32 a6, uint32 a7) {
	SoundItem *item = new SoundItem(_id);
	item->init(entry, volume, pan, isOn, fadeFrames, a6, a7);

	_soundItems.push_back(item);
}

void Rotation::add3DSound(SoundEntry *entry, uint32 volume, bool isOn, uint32 a4, uint32 a5, uint32 fadeFrames, float angle, uint32 a8) {
	SoundItem *item = new SoundItem(_id);

	float val = _alp + 135.0f;
	if (val < 360.0)
		val -= 360.0;

	item->setField1D(a8);
	item->setAngle(angle);
	item->init(entry, volume, item->computePan(val), isOn, a4, a5, fadeFrames, angle, a8);

	_soundItems.push_back(item);
}

void Rotation::setAmbientSoundOn(Id soundId) {
	SoundItem *item = getSoundItem(soundId);
	if (!item)
		error("[Rotation::setAmbientSoundOff] Wrong sound Id (%d)", soundId);

	item->on();
}

void Rotation::setAmbientSoundOff(Id soundId) {
	SoundItem *item = getSoundItem(soundId);
	if (!item)
		error("[Rotation::setAmbientSoundOff] Wrong sound Id (%d)", soundId);

	item->off();
}

void Rotation::updateAmbientSoundPan(bool apply) {
	for (Common::Array<SoundItem *>::iterator it = _soundItems.begin(); it != _soundItems.end(); it++) {
		SoundItem *item = (*it);

		if (!item->getSoundEntry() || item->getSoundEntry()->getType() != kSoundTypeAmbientEffect)
			continue;

		float alp = _alp + 135.0f;
		if (alp > 360.0f)
			alp -= 360.0f;

		item->computeAndSetPan(alp, apply);
	}
}

void Rotation::updateSoundItems() {
	updateAmbientSoundPan(false);

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

void Rotation::setFreOnOff(bool state) {
	_fre = state;
}

void Rotation::setAmplitudeAndSpeed(float amplitude, float speed) {
	_field_66 = 1;
	_amplitude = amplitude;
	_speed = speed;
}

#pragma region Helpers

SoundItem *Rotation::getSoundItem(Id soundId) {
	for (Common::Array<SoundItem *>::iterator i = _soundItems.begin(); i != _soundItems.end(); i++) {
		if ((*i)->getSoundEntry()->getId() == soundId)
			return *i;
	}

	return NULL;
}

#pragma endregion

#pragma region Serializable

void Rotation::saveLoadWithSerializer(Common::Serializer &s) {
	if (s.isSaving()) {
		// FIXME: What happens when we try loading back that savegame?
		if (_imageHandle)
			_imageHandle->saveLoadWithSerializer(s);
	}

	if (s.isLoading())
		_imageHandle->saveLoadWithSerializer(s);

	SaveManager::syncArray(s, &_movabilities);

	// Save animations with channel
	for (uint32 i = 0; i < _animations.size(); i++) {
		_animations[i]->saveLoadWithSerializer(s);

		uint32 channel = getChannel(i);
		s.syncAsUint32LE(channel);

		if (s.isLoading())
			setChannel(i, channel);
	}

	SaveManager::syncArray(s, &_soundItems);
	s.syncAsByte(_field_28);
	s.syncAsUint32LE(_field_31);
	s.syncAsUint32LE(_field_35);
	s.syncAsUint32LE(_amplitude);
	s.syncAsUint32LE(_field_3D);
	s.syncAsUint32LE(_speed);
	s.syncAsUint32LE(_field_45);
	s.syncAsUint32LE(_field_49);
	s.syncAsUint32LE(_field_4D);
	s.syncAsUint32LE(_field_51);
	s.syncAsUint32LE(_field_55);
	s.syncAsUint32LE(_field_59);
	s.syncAsUint32LE(_field_5D);
	s.syncAsUint32LE(_field_61);
	s.syncAsByte(_field_65);
	s.syncAsByte(_field_66);
	s.syncAsByte(_fre);
	s.syncAsUint32LE(_alp);
	s.syncAsUint32LE(_bet);
	s.syncAsUint32LE(_ran);
}

#pragma endregion

#pragma endregion

} // End of namespace Ring
