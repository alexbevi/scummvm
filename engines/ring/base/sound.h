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

#ifndef RING_SOUND_H
#define RING_SOUND_H

#include "ring/shared.h"

#include "audio/mixer.h"

namespace Ring {

class Application;

//////////////////////////////////////////////////////////////////////////
// Engine sound
//////////////////////////////////////////////////////////////////////////
class SoundEntry : public BaseObject {
public:
	SoundEntry(Id soundId, SoundType type, Common::String name, LoadFrom loadFrom, SoundFormat format);
	~SoundEntry();

	void play(bool loop);
	void stop();
	bool isPlaying();

    void setVolume(uint32 volume);
	void setMultiplier(uint32 multiplier);
	void setPan(int32 pan);

	bool checkPlaying();

	// Accessors
	SoundType getType() { return _type; }
	void setField125(uint32 val) { _field_125 = val; }
	void setPlaying(bool state) { _isPlaying = state; }

	static SoundFormat getFormat(Common::String filename);

protected:
	void setVolumeAndPan();

private:
	SoundType      _type;
	Common::String _name;
	bool           _isPlaying;
	LoadFrom       _loadFrom;
	uint32         _volume;
	uint32         _multiplier;
	int32          _pan;
	uint32         _field_11D;
	SoundFormat    _format;
	uint32         _field_125;

	Audio::SoundHandle _handle;

	//////////////////////////////////////////////////////////////////////////
	// Conversion functions
	//////////////////////////////////////////////////////////////////////////

	/**
	 * Convert volume to a ScummVM mixer value
	 *
	 * @param [in,out] vol The volume.
	 */
	static void convertVolumeFrom(float &vol);

	/**
	 * Convert ScummVM mixer value to a volume
	 *
	 * @param [in,out] vol The volume.
	 */
	static void convertVolumeTo(float &vol);

	/**
	 * Convert pan.
	 *
	 * @param [in,out] pan The pan.
	 */
	static void convertPan(float &pan);
};

class SoundEntryS : public SoundEntry {
public:
	SoundEntryS(Id soundId, SoundType type, Common::String name, LoadFrom loadFrom, SoundFormat format, uint32 soundChunk);
	~SoundEntryS();

private:
	uint32         _field_126;
	uint32         _field_12A;
	uint32         _field_12E;
	uint32         _field_132;
	uint32         _field_136;
	uint32         _field_13A;
	uint32         _field_13E;
	uint32         _field_142;
	uint32         _field_146;
	uint32         _field_14A;
	uint32         _field_14E;
	uint32         _field_152;
	uint32         _field_156;
	uint32         _field_15A;
	uint32         _field_15E;
	uint32         _field_162;
	//uint32         _event;
	uint32         _soundChunk;
};

class SoundEntryD : public SoundEntry {
public:
	SoundEntryD(Id soundId, SoundType type, Common::String name, LoadFrom loadFrom, SoundFormat format);
	~SoundEntryD();

private:
	uint32         _field_126;
	uint32         _field_12A;
	uint32         _field_12E;
	uint32         _field_132;
	uint32         _field_136;
	uint32         _field_13A;
	uint32         _field_13E;
};

class SoundManager {
public:
	SoundManager(Application *application, Audio::Mixer *mixer);
	~SoundManager();

	// Playing
	void updateQueue();
	void play(Id soundId, int a2);
	void stop(Id soundId, uint32 a2);
	void setVolume(Id soundId, uint32 volume);
	void stopType(SoundType soundType, uint32 a2);
	void setMultiplier(SoundType soundType, uint32 a2);
	void setMultiplierIfNotType(SoundType soundType, int32 multiplier);
	void stopAll(uint32 a1);
	void setPan(Id soundId, int32 pan);
	bool isPlaying(Id soundId);
	bool isPlayingType(SoundType soundType);

	void sub_4696F0();

	// Sound entries
	void addEntry(Id soundId, SoundType type, Common::String filename, LoadFrom loadFrom, SoundFormat format, bool a4, int soundChunk);
	SoundEntry *getSoundEntry(Id soundId);

	// Accessors
	float getGlobalVolume() { return _globalVolume; }
	Audio::Mixer *getMixer() { return _mixer; }

private:
	Application                    *_application;
	Audio::Mixer                   *_mixer;
	AssociativeArray<SoundEntry *>  _entries;
	float                           _globalVolume;
};

//////////////////////////////////////////////////////////////////////////
// Game classes
//////////////////////////////////////////////////////////////////////////
class SoundItem : public BaseObject {
public:
	SoundItem(Id id);
	~SoundItem();

	// Initialization
	void init(SoundEntry *entry, uint32 volume, int32 pan, bool isOn, uint32 fadeFrames, uint32 a6, uint32 a7);
    void init(SoundEntry *entry, uint32 volume, int32 pan, bool isOn, uint32 a5, uint32 a6, uint32 fadeFrames, float a8, int a9);

	// Sound on/off & volume
	void on();
	void off();
	void turnOn();
	void turnOff();
	void setVolume(uint32 volume);
	void setPan(int32 pan);

	// Accessors
	SoundEntry *getSoundEntry() { return _entry; }
	void setField1D(int32 val);
	void setAngle(float angle);
	bool isOn() { return _isOn; }

	// Helpers
	float computePan(float angle);
	void computeAndSetPan(float alp);

private:
	SoundEntry *_entry;
	uint32      _volume;
	int32       _pan;
	uint32      _field_10;
	uint32      _field_14;
	bool        _isOn;
	uint32      _field_19;
	uint32      _field_1D;
	float       _angle;
	float       _field_25;
	float       _field_29;
	float       _field_2D;
	float       _field_31;
	uint32      _field_35;
	uint32      _field_39;
	uint32      _field_3D;

	bool checkCurrentPuzzle();
};

class SoundHandler {
public:
	SoundHandler();
	~SoundHandler();

	void turnOffItems2(bool a1);
	bool sub_41AA00();
	bool sub_41AEE0(uint32 a1);
	void turnOffItems1();
	void sub_41B180(uint32 a1);
	void sub_41B350(uint32 a1);
	void sub_41B520();

	void reset();

	// Accessors
	void setReverseStereo(int32 reverseStereo);
	float getDirection() { return _direction; }
	bool getField0() { return _field_0; }
	void setCount1(uint32 count) { _count1 = count; }
	void setCount2(uint32 count) { _count2 = count; }

private:
	// Data
	bool _field_0;
	uint32 _count1;
	uint32 _count2;
	Common::Array<SoundItem *> _soundItems1;
	Common::Array<SoundItem *> _soundItems2;
	Common::Array<SoundItem *> _soundItems3;
	Common::Array<SoundItem *> _soundItems4;

	// Sound direction? (-1.0 or 1.0)
	float _direction;
};

} // End of namespace Ring

#endif // RING_SOUND_H
