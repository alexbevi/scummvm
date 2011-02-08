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

#include "ring/graphics/rotation.h"

#include "ring/base/sound.h"
#include "ring/base/text.h"

#include "ring/game/application.h"

#include "ring/graphics/accessibility.h"
#include "ring/graphics/animation.h"
#include "ring/graphics/image.h"
#include "ring/graphics/movability.h"
#include "ring/graphics/presentation.h"

#include "ring/ring.h"

namespace Ring {

#pragma region Node

Node::Node() {
	_field_0 = 0;
	_field_4 = 0;
	_field_8 = 0;
	_field_5C = 0;
}

Node::~Node() {
}

void Node::update(uint32 val) {
	if (_field_5C != val) {
		_field_5C = val;
		_field_4 = 1;
	}
}

#pragma endregion

#pragma region RotationData

RotationData::RotationData(uint32 count, Common::String path) {
	_path = path;

	for (uint32 i = 0; i < count; i++)
		_nodes.push_back(new Node());

	_count = count;
}

RotationData::~RotationData() {
	CLEAR_ARRAY(Node, _nodes);
}

void RotationData::update(uint32 index, uint32 val) {
	_nodes[index]->update(val);
}

#pragma endregion

#pragma region Rotation

Rotation::Rotation(Id id, Common::String name, uint32 a3, LoadFrom loadFrom, uint32 nodeCount, uint32 a6) : BaseObject(id) {
	_field_8 = a6;
	_comBufferLength = 0;

	// Compute path
	_path = Common::String::format("DATA/%s/NODE/%s", getApp()->getCurrentZoneString().c_str(), name.c_str());

	// Create animation for each node
	for (uint32 i = 0; i < nodeCount; i++)
		_animations.push_back(new Animation());

	_field_28 = a3;
	_field_65 = 0;
	_field_66 = 0;

	initNodes(nodeCount);
}

Rotation::~Rotation() {
	CLEAR_ARRAY(Movability,         _movabilities);
	CLEAR_ARRAY(Accessibility,      _accessibilities);
	CLEAR_ARRAY(ObjectPresentation, _presentations);
	CLEAR_ARRAY(Animation,          _animations);
	CLEAR_ARRAY(Text,               _texts);
	CLEAR_ARRAY(SoundItem,          _soundItems);
	CLEAR_ARRAY(ImageHandle,        _imageHandles);

	SAFE_DELETE(_data);
}

void Rotation::initNodes(uint32 count) {
	_data = new RotationData(count, _path);

	_field_31  = 1.0f;
	_field_35  = 0.3f;
	_amplitude = 30.0f;
	_field_3D  = 1.0f;
	_speed     = 1.0f;
	_field_67  = 0;
	_field_68  = 225.0f;
	_field_6C  = 0;
	_field_70  = 85.0f;
}

void Rotation::addAccessibility(Accessibility *accessibility) {
	if (accessibility == NULL)
		error("[Rotation::addAccessibility] Accessibility is NULL!");

	_accessibilities.push_back(accessibility);
}

void Rotation::addMovability(Movability *movability) {
	if (movability == NULL)
		error("[Rotation::addMovability] Movability is NULL!");

	_movabilities.push_back(movability);
}

Animation *Rotation::addPresentationAnimation(ObjectPresentation *presentation, uint32 layer, uint32 a3, float a4, uint32 a5) {
	if (!presentation)
		error("[Rotation::addPresentationAnimation] Presentation is NULL!");

	if (layer >= _animations.size())
		error("[Rotation::addPresentationAnimation] Invalid layer index (was: %d, max:%d)", layer, _animations.size() - 1);

	Animation *animation = _animations[layer];
	animation->initAnimation(a3, a4, 1, a5, 0);

	_presentations.push_back(presentation);

	return animation;
}

void Rotation::updateNode(uint32 index, uint32 val) {
	_data->update(index, val);
}

void Rotation::addAmbientSound(SoundEntry *entry, uint32 volume, uint32 a3, bool isOn, uint32 fadeFrames, uint32 a6, uint32 a7) {
	SoundItem *item = new SoundItem(_id);
	item->init(entry, volume, a3, isOn, fadeFrames, a6, a7);

	_soundItems.push_back(item);
}

void Rotation::add3DSound(SoundEntry *entry, uint32 volume, bool isOn, uint32 a4, uint32 a5, uint32 fadeFrames, float angle, uint32 a8) {
	SoundItem *item = new SoundItem(_id);

	float val = _field_68 + 135.0f;
	if (val < 360.0)
		val -= 360.0;

	item->setField1D(a8);
	item->setAngle(angle);
	item->init(entry, volume, item->computeFieldC(val), isOn, a4, a5, fadeFrames, angle, a8);

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

uint32 Rotation::getLayerCount() {
	if (!_data)
		error("[Rotation::getLayerCount] Data not initialized!");

	return _data->getCount();
}

Movability *Rotation::getMovability(uint32 index) {
	if (index >= _movabilities.size())
		error("[Rotation::getMovability] Invalid movability index (was: %d, max:%d)", index, _movabilities.size() - 1);

	return _movabilities[index];
}

#pragma endregion

#pragma region Helpers

SoundItem *Rotation::getSoundItem(Id soundId) {
	for (Common::Array<SoundItem *>::iterator i = _soundItems.begin(); i != _soundItems.end(); i++) {
		if ((*i)->getSoundEntry()->getId() == soundId)
			return *i;
	}

	return NULL;
}

#pragma endregion

} // End of namespace Ring
