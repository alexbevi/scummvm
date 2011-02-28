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

#ifndef RING_ROTATION_H
#define RING_ROTATION_H

#include "ring/helpers.h"
#include "ring/shared.h"

namespace Ring {

class Accessibility;
class Animation;
class ImageHandle;
class Movability;
class ObjectPresentation;
class ScreenManager;
class SoundEntry;
class SoundItem;
class Text;

struct NodeData {
	uint32 field_0;
	uint32 field_4;
	uint32 field_8;
	uint32 field_1C;
	uint32 field_20;
	uint32 field_24;
	uint32 field_28;
	uint32 *field_34;
	uint32 field_38;
	uint32 field_3C;

	NodeData() {
		field_0 = 0;
		field_4 = 0;
		field_8 = 0;
		field_1C = 0;
		field_20 = 0;
		field_24 = 0;
		field_28 = 0;
		field_34 = NULL;
		field_38 = 0;
		field_3C = 0;
	}

	~NodeData() {
		SAFE_DELETE(field_34);
	}
};

struct NodeInfo {
	uint32 field_0;
	uint32 field_4;
	Common::Array<NodeData *> nodeDatas;
	NodeData nodeData;
	int32 field_4C;

	NodeInfo() {
		field_0 = 0;
		field_4 = 0;
		field_4C = -1;
	}

	~NodeInfo() {
		CLEAR_ARRAY(NodeData, nodeDatas);
	}
};

class Node {
public:
	Node();
	~Node();

	void update(uint32 val);

private:
	uint32 _field_0;
	uint32 _field_4;
	uint32 _field_8;
	NodeInfo _info;
	uint32 _field_5C;
};

class RotationData {
public:
	RotationData(uint32 count, Common::String path);
	~RotationData();

	void update(uint32 index, uint32 val);
	uint32 getCount() { return _count; }

private:
	Common::Array<Node *> _nodes;
	Common::String _path;
	NodeData _nodeData;
	uint32 _count;
};

class Rotation : public BaseObject, public Common::Serializable {
public:
	Rotation(Id id, Common::String name, byte a3, LoadFrom loadFrom, uint32 nodeCount, uint32 a6);
	~Rotation();

	void dealloc();

	void load();
	void update();
	void setCoordinates(const Common::Point &point);
	void loadImage();
	void destroyImage();
	void drawImage(ScreenManager *screen);
	void draw(ScreenManager *screen);
	void drawText();

	// Accessibility
	void addAccessibility(Accessibility *accessibility);

	// Movability
	void addMovability(Movability *movability);
	void setMovabilityOnOrOff(bool enableHotspot);
	void setMovabilityOnOrOff(bool enableHotspot, uint32 fromMovability, uint32 toMovability);
	void setMovabilityRideName(uint32 movabilityIndex, Common::String name);

	void setAlp(float alp) { _alp = alp; }
	void setBet(float bet) { _bet = bet; }
	void setRan(float ran) { _ran = ran; }
	void setRolTo(float a2, float a3, float a4);

	float getAlp() { return _alp; };
	float getBet() { return _bet; }
	float getRan() { return _ran; }
	bool  getFre() { return _fre; }

	// Presentation
	Animation *addPresentationAnimation(ObjectPresentation *presentation, uint32 layer, uint32 frameCount, float frameRate, byte a5);

	// Nodes
	void updateNode(uint32 index, uint32 val);

	// Sound
	void addAmbientSound(SoundEntry *entry, uint32 volume, int32 pan, bool isOn, uint32 fadeFrames, uint32 a6, uint32 a7);
	void add3DSound(SoundEntry *entry, uint32 volume, bool isOn, uint32 a4, uint32 a5, uint32 fadeFrames, float angle, uint32 a9);
	void setAmbientSoundOn(Id soundId);
	void setAmbientSoundOff(Id soundId);
	void updateAmbientSoundPan(bool apply);
	void updateSoundItems();
	uint32 getSoundItemsCount() { return _soundItems.size(); }

	// Serializable
	void saveLoadWithSerializer(Common::Serializer &s);

	// Accessors
	void setFreOnOff(bool state);
	void setAmplitudeAndSpeed(float amplitude, float speed);
	uint32 getLayerCount();
	void setComBufferLength(uint32 length) { _comBufferLength = length; }
	Movability *getMovability(uint32 index);
	void setField28(byte val) { _field_28 = val; }
	byte getField28() { return _field_28; }
	bool hasImage() { return _imageHandle != NULL; }

private:
	Common::String                      _path;
	uint32                              _field_8;
	uint32                              _comBufferLength;
	Common::Array<Movability *>         _movabilities;
	Common::Array<Accessibility *>      _accessibilities;
	Common::Array<ObjectPresentation *> _presentations;
	Common::Array<Animation *>          _animations;
	Common::Array<Text *>               _texts;
	Common::Array<SoundItem *>          _soundItems;
	byte                                _field_28;
	ImageHandle                        *_imageHandle;
	RotationData                       *_data;
	float                               _field_31;
	float                               _field_35;
	float                               _amplitude;
	float                               _field_3D;
	float                               _speed;
	uint32                              _field_45;
	uint32                              _field_49;
	uint32                              _field_4D;
	uint32                              _field_51;
	uint32                              _field_55;
	uint32                              _field_59;
	uint32                              _field_5D;
	uint32                              _field_61;
	byte                                _field_65;
	byte                                _field_66;
	bool                                _fre;
	float                               _alp;
	float                               _bet;
	float                               _ran;

	SoundItem *getSoundItem(Id soundId);
};

} // End of namespace Ring

#endif // RING_ROTATION_H
