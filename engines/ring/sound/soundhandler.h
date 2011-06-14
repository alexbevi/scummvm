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

#ifndef RING_SOUNDHANDLER_H
#define RING_SOUNDHANDLER_H

#include "ring/shared.h"

namespace Ring {

class SoundEntry;

class SoundItem : public BaseObject, public Common::Serializable {
public:
	SoundItem(Id id);
	~SoundItem();

	// Initialization
	void init(SoundEntry *entry, uint32 volume, int32 pan, bool isTurnedOn, uint32 fadeFrames, uint32 a6, uint32 a7);
    void init(SoundEntry *entry, uint32 volume, int32 pan, bool isTurnedOn, uint32 a5, uint32 a6, uint32 fadeFrames, float a8, int a9);

	// Sound on/off & volume
	void on();
	void off();
	void turnOn();
	void turnOff();
	void setVolume(int32 volume);

	// Serializable
	void saveLoadWithSerializer(Common::Serializer &s);

	// Accessors
	SoundEntry *getSoundEntry() { return _entry; }
	void setField1D(int32 val);
	void setAngle(float angle);
	void setVolumes(float volume1, float volume2);
	void setPans(float pan1, float pan2);

	void setPan(int32 pan) { _pan = pan; }
	void setVolume1(float volume1) { _volume1 = volume1; }
	void setPan1(float pan1) { _pan1 = pan1; }
	void setField35(float val) { _field_35 = val; }
	void setField39(float val) { _field_39 = val; }
	void setField3D(float val) { _field_3D = val; }

	SoundEntry *getEntry() { return _entry; }
	float getVolume() { return _volume; }
	float getVolume1() { return _volume1; }
	float getVolume2() { return _volume2; }
	float getPan() { return _pan; }
	float getPan1() { return _pan1; }
	float getPan2() { return _pan2; }
	uint32 getField10() { return _field_10; }
	uint32 getField14() { return _field_14; }
	uint32 getField19() { return _field_19; }
	float  getField35() { return _field_35; }
	float  getField39() { return _field_39; }
	uint32 getField3D() { return _field_3D; }
	bool isOn() { return _isOn; }

	// Helpers
	int32 computePan(float angle) const;
	void computeAndSetPan(float alp, bool apply);

private:
	SoundEntry *_entry;
	int32       _volume;
	int32       _pan;
	uint32      _field_10;
	uint32      _field_14;
	bool        _isOn;
	uint32      _field_19;
	uint32      _field_1D;
	float       _angle;
	float       _volume1;
	float       _pan1;
	float       _volume2;
	float       _pan2;
	float       _field_35;
	float       _field_39;
	float       _field_3D;

	bool checkCurrentPuzzle() const;
	void setPanOnEntry(int32 pan);
};

class SoundHandler {
public:
	SoundHandler();
	~SoundHandler();

	void turnOffSounds1(bool process);
	bool processSounds();
	bool updateItems(uint32 chunkCount);
	void turnOffItems1();
	bool updateItems2(uint32 chunkCount);
	bool updateItems3(uint32 chunkCount);
	void turnOnItems4();

	void reset();

	// Accessors
	float getDirection() { return _direction; }
	bool getField0() { return _field_0; }

	void setField0(bool state) { _field_0 = state; }
	void setReverseStereo(int32 reverseStereo);
	void setSounds1(AssociativeArray<SoundItem *>* sounds) { _sounds1 = sounds; }
	void setSounds2(AssociativeArray<SoundItem *>* sounds) { _sounds2 = sounds; }

private:
	// Data
	bool _field_0;
	AssociativeArray<SoundItem *>* _sounds1;
	AssociativeArray<SoundItem *>* _sounds2;
	Common::Array<SoundItem *>  _soundItems1;
	Common::Array<SoundItem *>  _soundItems2;
	Common::Array<SoundItem *>  _soundItems3;
	Common::Array<SoundItem *>  _soundItems4;

	// Sound direction? (-1.0 or 1.0)
	float _direction;
};

} // End of namespace Ring

#endif // RING_SOUNDHANDLER_H
