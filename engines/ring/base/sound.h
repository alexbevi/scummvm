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
	virtual ~SoundEntry();

	virtual void play(bool loop) = 0;
	virtual void preload() = 0;
	virtual void stop() = 0;
	virtual bool isPreloaded() = 0;
	bool isPlaying();

    void setVolume(int32 volume);
	void setMultiplier(int32 multiplier);
	void setPan(int32 pan);

	bool checkPlaying();

	// Accessors
	SoundType getType() { return _type; }
	Common::String getName() { return _name; }
	void setField125(uint32 val) { _field_125 = val; }
	void setPlaying(bool state) { _isPlaying = state; }

	int32 getVolume() { return _volume; }
	int32 getMultiplier() { return _multiplier; }
	int32 getPan() { return _pan; }
	bool isLooping() { return _loop; }
	void setSoundInfo(int32 volume, int32 multiplier, int32 pan) { _volume = volume; _multiplier = multiplier; _pan = pan; }

	static SoundFormat getFormat(Common::String filename);

protected:
	SoundType      _type;
	Common::String _name;
	bool           _isPlaying;
	LoadFrom       _loadFrom;
	int32          _volume;
	int32          _multiplier;
	int32          _pan;
	bool           _loop;
	SoundFormat    _format;
	uint32         _field_125;

	Audio::SoundHandle _handle;

	void setVolumeAndPan();

	//////////////////////////////////////////////////////////////////////////
	// Conversion functions
	//////////////////////////////////////////////////////////////////////////

	/**
	 * Convert volume to a ScummVM mixer value
	 *
	 * @param [in,out] vol The volume.
	 */
	static void convertVolumeFrom(int32 &vol);

	/**
	 * Convert ScummVM mixer value to a volume
	 *
	 * @param [in,out] vol The volume.
	 */
	static void convertVolumeTo(int32 &vol);

	/**
	 * Convert pan.
	 *
	 * @param [in,out] pan The pan.
	 */
	static void convertPan(int32 &pan);
};

class SoundEntryS : public SoundEntry {
public:
	SoundEntryS(Id soundId, SoundType type, Common::String name, LoadFrom loadFrom, SoundFormat format, uint32 soundChunk);
	~SoundEntryS();

	virtual void play(bool loop);
	virtual void preload();
	virtual void stop();
	virtual bool isPreloaded() { return false; }

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

	virtual void play(bool loop);
	virtual void preload();
	virtual void stop();
	virtual bool isPreloaded() { return _isPreloaded; }

private:
	uint32         _field_126;
	uint32         _field_12A;
	uint32         _field_12E;
	uint32         _field_132;
	uint32         _field_136;
	uint32         _field_13A;
	uint32         _isPreloaded;
};

class SoundManager : public Common::Serializable {
public:
	SoundManager(Application *application, Audio::Mixer *mixer);
	~SoundManager();

	// Playing
	void updateQueue();
	void play(Id soundId, bool loop);
	void stop(Id soundId, uint32 a2);
	void preload(Id soundId);
	void setVolume(Id soundId, uint32 volume);
	void stopType(SoundType soundType, uint32 a2);
	void setMultiplier(SoundType soundType, uint32 multiplier);
	void setMultiplierIfNotType(SoundType soundType, int32 multiplier);
	void stopAll(uint32 a1);
	void setPan(Id soundId, int32 pan);
	bool isPlaying(Id soundId);
	bool isPlayingType(SoundType soundType);
	void clear();

	// Play loaded sounds
	void playSounds();

	// Sound entries
	void addEntry(Id soundId, SoundType type, Common::String filename, LoadFrom loadFrom, SoundFormat format, bool a4, int soundChunk);
	SoundEntry *getEntry(Id soundId);

	// Accessors
	float getGlobalVolume() { return _globalVolume; }
	Audio::Mixer *getMixer() { return _mixer; }

	// Serializable
	void saveLoadWithSerializer(Common::Serializer &s);

private:
	struct SavedSound {
		Id soundId;
		bool loop;

		SavedSound() {
			soundId = 0;
			loop = false;
		}
	};

	Application                    *_app;
	Audio::Mixer                   *_mixer;
	AssociativeArray<SoundEntry *>  _entries;
	float                           _globalVolume;

	// Saved sounds
	Common::Array<SavedSound *> _savedSounds;
};

//////////////////////////////////////////////////////////////////////////
// Game classes
//////////////////////////////////////////////////////////////////////////
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
	void setField35(uint32 val) { _field_35 = val; }
	void setField39(uint32 val) { _field_39 = val; }
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
	uint32 getField35() { return _field_35; }
	uint32 getField39() { return _field_39; }
	uint32 getField3D() { return _field_3D; }
	bool isOn() { return _isOn; }

	// Helpers
	int32 computePan(float angle);
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
	uint32      _field_35;
	uint32      _field_39;
	float       _field_3D;

	bool checkCurrentPuzzle();
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

#endif // RING_SOUND_H
