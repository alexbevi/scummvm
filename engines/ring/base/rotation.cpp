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

#include "ring/base/rotation.h"

#include "ring/base/application.h"
#include "ring/base/accessibility.h"
#include "ring/base/movability.h"
#include "ring/base/object.h"
#include "ring/base/saveload.h"
#include "ring/base/text.h"

#include "ring/graphics/animation.h"
#include "ring/graphics/aquator.h"
#include "ring/graphics/hotspot.h"
#include "ring/graphics/image.h"
#include "ring/graphics/screen.h"

#include "ring/sound/soundentry.h"
#include "ring/sound/soundhandler.h"

#include "ring/helpers.h"
#include "ring/ring.h"

namespace Ring {

#pragma region Rotation

Rotation::Rotation(Id id, Common::String name, byte a3, LoadFrom, uint32 nodeCount, bool isCompressedStream) : BaseObject(id) {
	_isCompressedStream = isCompressedStream;
	_comBufferLength = 0;

	// Compute path (Original checks loadFrom)
	_path = Common::String::format("DATA/%s/NODE/%s", getApp()->getCurrentZoneFolder().c_str(), name.c_str());

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
	_nodeCount = nodeCount;
	_fre       = false;
	_alp       = 225.0f;
	_bet       = 0;
	_ran       = 85.0f;

	_imageHandle = NULL;

	_ticks       = 0;
	_startTicks  = 0;

	memset(_rotationTable, 0, sizeof(_rotationTable));

	_float1 = 0.0f;
	_float2 = 0.0f;
	_float3 = 0.0f;
	_float4 = 0.0f;
	_float5 = 0.0f;
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
	if (_stream->isInitialized())
		return;

	_stream->alloc(_isCompressedStream, g_system->getScreenFormat(), _comBufferLength);

	_field_31 = 0;

	// Init ticks
	_startTicks = g_system->getMillis();
	_ticks= _startTicks;

	// Initialize rotation data and tables
	ImageHeaderEntry::Header header = _stream->getEntry()->getHeader();
	_float1 = header.field_4;
	_float2 = header.field_10 - header.field_C;
	_float3 = (header.field_10 + header.field_C) * 0.5f;
	_float4 = header.field_18 - header.field_14;
	_float5 = (header.field_18 + header.field_14) * 0.5f;

	for (uint32 i = 0; i < ARRAYSIZE(_rotationTable) - 64; i += 64) {
		for (uint32 j = 0; j < 32; j++)
			_rotationTable[i + j] = rnd((uint32)_amplitude);
	}

	_field_45 = rnd((uint32)_field_35);
	_field_49 = rnd((uint32)_field_35);
	_field_4D = rnd((uint32)_field_35);
	_field_51 = rnd((uint32)_field_35);
	_field_55 = rnd((uint32)_field_35);
	_field_59 = rnd((uint32)_field_35);
	_field_5D = rnd((uint32)_field_35);
	_field_61 = rnd((uint32)_field_35);
}

void Rotation::dealloc() {
	if (_stream->isInitialized())
		_stream->dealloc();
}

void Rotation::update() {
	_ticks = g_system->getMillis();
	float timeOffset = (_ticks - _startTicks) * 0.001f;

	// Update presentations
	for (Common::Array<ObjectPresentation *>::iterator it = _presentations.begin(); it != _presentations.end(); it++) {
		ObjectPresentation *presentation = (*it);

		presentation->playPuzzleAnimations(_ticks);
		presentation->playRotationAnimations(_ticks);
	}

	// Update aquator
	for (uint32 i = 0; i < _stream->getCount(); i++) {
		if (!_stream->sub_410F50(i))
			continue;

		if (_animations[i]->getField26())
			_stream->sub_411530(i, _animations[i]->getField2D());
		else
			_stream->setChannel(i, 0);
	}

	updateAmbientSoundPan(true);
	_stream->updateEntries(timeOffset);

	if (_field_31 < 1.0f)
		_field_31 += timeOffset;

	if (_field_31 > 1.0f)
		_field_31 = 1.0f;
}

void Rotation::updateAndDraw(float alp, float bet, float ran) {
	update();

	// Update coordinates
	_alp = alp - 135.0f;
	if (_alp < 0.0f)
		_alp += 360.0f;

	_ran = ran;
	_bet = bet;

	updateView();
	draw();
	getApp()->getScreenManager()->updateScreen();
}

void Rotation::setCoordinates(Common::Point *point, Common::KeyCode keycode) {

	if (!_fre) {
		float alpDiff = point->x * 0.0015625f    - 0.5f;
		float betDiff = point->y * 0.0020833334f - 0.5f;

		if (abs(alpDiff) > 0.25f)
			_alp += (abs(alpDiff) - 0.25f) * 48.0f * alpDiff;

		if (abs(betDiff) > 0.25f)
			_bet += (abs(betDiff) - 0.25f) * 48.0f * betDiff;

		if (keycode == Common::KEYCODE_UP)
			_ran -= 1.0f;

		if (keycode == Common::KEYCODE_DOWN)
			_ran += 1.0f;
	}

	point->y -= 16;

	if (_stream->isInitialized()) {
		updateView();
		_stream->getEntry()->computeCoordinates(point);
		_stream->getEntry()->adjustCoordinates(point);
	}
}

void Rotation::updateView() {
	error("[Rotation::updateView] Not implemented");
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
		filename = Common::String::format("DATA/%s/IMAGE/%s", getApp()->getZoneFolder(_imageHandle->getZone()).c_str(), _imageHandle->getNameId().c_str());
		break;

	case kArchiveArt:
		filename = Common::String::format("/DATA/%s", _imageHandle->getNameId().c_str());
		break;
	}

	_imageHandle->load(filename, _imageHandle->getArchiveType(), _imageHandle->getZone(), _imageHandle->getLoadFrom(), _imageHandle->getDrawType());
}

void Rotation::destroyImage() {
	if (_imageHandle && _imageHandle->isInitialized())
		_imageHandle->destroy();
}

void Rotation::drawImage() {
	if (_imageHandle)
		getApp()->getScreenManager()->draw(_imageHandle, _imageHandle->getCoordinates(), kDrawType1);
}

void Rotation::draw() {
	if (!_stream->isInitialized())
		return;

	_stream->getEntry()->prepareBuffer();
	_stream->getEntry()->drawBuffer();
}

void Rotation::drawText() {
	for (Common::Array<Text *>::iterator it = _texts.begin(); it != _texts.end(); it++)
		getApp()->getScreenManager()->drawText(*it);
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

bool Rotation::setRolTo(float alp, float bet, float ran) {
	alp -= 135.0f;
	if (alp < 0.0f)
		alp += 360.0f;

	// Compute differences
	float betDiff = abs(bet - _bet);
	float alpDiff = abs(alp - _alp);
	float ranDiff = abs(ran - _ran);

	// Get the higher difference
	if (alpDiff > 180) {
		alpDiff = 360 - alpDiff;
	}

	if (alpDiff <= betDiff)
		alpDiff = betDiff;

	if (alpDiff <= ranDiff)
		alpDiff = ranDiff;

	// Compute alp
	if ((alp - _alp) <= 180.0f) {
		if ((alp - _alp) < -180.0f)
			alp += 360.0f;
	} else {
		alp -= 360.0f;
	}

	// Compute count
	uint32 count = (uint32)(alpDiff * 0.8f);
	if (count == 0)
		return true;

	for (uint32 i = 0; i < count; i++) {
		float delta = i;
		if (count != 1)
			delta /= (count - 1);

		// Update alp, bet and ran
		_alp = _alp * (1.0f - delta) + alp * delta;
		if (_alp > 360.0f)
			_alp -= 360.0f;

		_ran = _ran * (1.0f - delta) + ran * delta;
		_bet = _bet * (1.0f - delta) + bet * delta;

		_field_31 = 1.0f - delta;

		// Update rotation
		update();
		updateView();
		draw();
		getApp()->getScreenManager()->updateScreen();

		if (checkEscape())
			return false;
	}

	return true;
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
	if (val < 360.0f)
		val -= 360.0f;

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

void Rotation::updateSoundItems(bool updateItems) {
	updateAmbientSoundPan(false);

	if (!updateItems)
		return;

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
	SaveManager::syncWithFlag(s, _imageHandle);

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
