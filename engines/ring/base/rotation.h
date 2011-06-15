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

#ifndef RING_ROTATION_H
#define RING_ROTATION_H

#include "ring/shared.h"

#include "common/keyboard.h"
#include "common/rect.h"

namespace Ring {

class Accessibility;
class AquatorStream;
class Animation;
class ImageHandle;
class Movability;
class ObjectPresentation;
class ScreenManager;
class SoundEntry;
class SoundItem;
class Text;

class Rotation : public BaseObject, public Common::Serializable {
public:
	Rotation(Id id, Common::String name, byte a3, LoadFrom loadFrom, uint32 nodeCount, bool isCompressedStream);
	~Rotation();

	void alloc();
	void dealloc();

	void update();
	void updateAndDraw(float alp, float bet, float ran);
	void setCoordinates(Common::Point *point, Common::KeyCode keycode);
	void loadImage();
	void destroyImage();
	void drawImage();
	void draw();
	void drawText();

	// Accessibility
	void addAccessibility(Accessibility *accessibility);
	Accessibility *getAccessibility(const Common::Point &point);
	int32 getAccessibilityIndex(const Common::Point &point);
	Accessibility *getAccessibility(Common::KeyCode key);

	// Movability
	void addMovability(Movability *movability);
	void setMovabilityOnOrOff(bool enableHotspot);
	void setMovabilityOnOrOff(bool enableHotspot, uint32 fromMovability, uint32 toMovability);
	void setMovabilityRideName(uint32 movabilityIndex, Common::String name);
	Movability *getMovability(const Common::Point &point);
	int32 getMovabilityIndex(const Common::Point &point);
	Movability *getMovability(uint32 index);

	void setAlp(float alp) { _alp = alp; }
	void setBet(float bet) { _bet = bet; }
	void setRan(float ran) { _ran = ran; }
	bool setRolTo(float alp, float bet, float ran);

	float getAlp() { return _alp; };
	float getBet() { return _bet; }
	float getRan() { return _ran; }
	bool  getFre() { return _fre; }

	// Presentation
	Animation *addPresentationAnimation(ObjectPresentation *presentation, uint32 layer, uint32 frameCount, float frameRate, byte a5);

	// Stream
	void setChannel(uint32 index, uint32 channel);
	uint32 getChannel(uint32 index);
	uint32 getLayerCount();

	// Sound
	void addAmbientSound(SoundEntry *entry, uint32 volume, int32 pan, bool isOn, uint32 fadeFrames, uint32 a6, uint32 a7);
	void add3DSound(SoundEntry *entry, uint32 volume, bool isOn, uint32 a4, uint32 a5, uint32 fadeFrames, float angle, uint32 a9);
	void setAmbientSoundOn(Id soundId);
	void setAmbientSoundOff(Id soundId);
	void updateAmbientSoundPan(bool apply);
	void updateSoundItems(bool updateItems);
	AssociativeArray<SoundItem *> *getSoundItems() { return &_soundItems; }

	// Serializable
	void saveLoadWithSerializer(Common::Serializer &s);

	// Accessors
	void setFreOnOff(bool state);
	void setAmplitudeAndSpeed(float amplitude, float speed);
	void setCompressionBufferLength(uint32 length) { _comBufferLength = length; }
	void setField28(byte val) { _field_28 = val; }
	byte getField28() { return _field_28; }
	bool hasImage() { return _imageHandle != NULL; }

private:
	Common::String                      _path;
	bool                                _isCompressedStream;
	uint32                              _comBufferLength;
	Common::Array<Movability *>         _movabilities;
	Common::Array<Accessibility *>      _accessibilities;
	Common::Array<ObjectPresentation *> _presentations;
	Common::Array<Animation *>          _animations;
	Common::Array<Text *>               _texts;
	AssociativeArray<SoundItem *>       _soundItems;
	byte                                _field_28;
	ImageHandle                        *_imageHandle;
	AquatorStream                      *_stream;
	float                               _field_31;
	float                               _field_35;
	float                               _amplitude;
	float                               _field_3D;
	float                               _speed;
	float                               _field_45;
	float                               _field_49;
	float                               _field_4D;
	float                               _field_51;
	float                               _field_55;
	float                               _field_59;
	float                               _field_5D;
	float                               _field_61;
	byte                                _field_65;
	byte                                _field_66;
	uint32                              _nodeCount;
	bool                                _fre;
	float                               _alp;
	float                               _bet;
	float                               _ran;

	// Working data
	uint32                              _ticks;
	uint32                              _startTicks;
	int32                               _rotationTable[2048];
	float                               _float1;
	float                               _float2;
	float                               _float3;
	float                               _float4;
	float                               _float5;

	SoundItem *getSoundItem(Id soundId);
	void updateView();
};

} // End of namespace Ring

#endif // RING_ROTATION_H
