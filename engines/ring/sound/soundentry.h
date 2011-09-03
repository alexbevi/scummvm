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

#ifndef RING_SOUNDENTRY_H
#define RING_SOUNDENTRY_H

#include "ring/shared.h"

#include "audio/audiostream.h"
#include "audio/mixer.h"

namespace Ring {

class SoundLoader;

class SoundEntry : public BaseObject {
public:
	SoundEntry(Id soundId, SoundType type, Common::String name, LoadFrom loadFrom, SoundFormat format);
	virtual ~SoundEntry();

	virtual void play(bool loop) = 0;
	virtual void stop() = 0;
	virtual void stopAndClear() = 0;
	virtual void preload() {}
	virtual void unload() {}
	virtual bool isPreloaded() { return false; }
	bool isPlaying() const ;

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

	void setVolumeAndPan() const ;

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

class SoundEntryStream : public SoundEntry {
public:
	SoundEntryStream(Id soundId, SoundType type, Common::String name, LoadFrom loadFrom, SoundFormat format, uint32 soundChunk);
	~SoundEntryStream();

	virtual void play(bool loop);
	virtual void stop();
	virtual void stopAndClear();

private:
	SoundLoader   *_loader;
	Audio::RewindableAudioStream *_audioStream;
	void*          _field_132;
	uint32         _field_136;
	int32          _size;
	uint32         _bufferOffset;
	uint32         _field_142;
	uint32         _field_146;
	uint32         _field_14A;
	uint32         _loop;
	uint32         _field_152;
	//uint32         _threadId;
	//uint32         _event1;
	//uint32         _event2;
	bool           _isBufferPlaying;
	//uint32         _event;
	uint32         _soundChunk;


	bool loadData(SoundFormat format, const Common::String &path, uint32 soundChunk);
	void initSoundBuffer(const Common::String &path, uint32 soundChunk, bool loop, SoundFormat format);
	void stopAndReleaseSoundBuffer();
};

class SoundEntryData : public SoundEntry {
public:
	SoundEntryData(Id soundId, SoundType type, Common::String name, LoadFrom loadFrom, SoundFormat format);
	~SoundEntryData();

	virtual void play(bool loop);
	virtual void stop();
	virtual void stopAndClear();
	virtual void preload();
	virtual void unload();
	virtual bool isPreloaded() { return _isPreloaded; }

private:
	Audio::RewindableAudioStream *_audioStream;
	uint32         _isPreloaded;

	void initSoundBuffer(const Common::String &path);
	void stopAndReleaseSoundBuffer();
};

} // End of namespace Ring

#endif // RING_SOUNDENTRY_H
