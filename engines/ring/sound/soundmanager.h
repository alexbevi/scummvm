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

#ifndef RING_SOUNDMANAGER_H
#define RING_SOUNDMANAGER_H

#include "ring/shared.h"

#include "audio/mixer.h"

namespace Ring {

class Application;
class SoundEntry;

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
	void setMultiplier(SoundType soundType, int32 multiplier);
	void setMultiplierIfNotType(SoundType soundType, int32 multiplier);
	void stopAll(uint32 a1);
	void setPan(Id soundId, int32 pan);
	bool isPlaying(Id soundId);
	bool isPlayingType(SoundType soundType);
	void clear();

	// Play loaded sounds
	void playSounds();

	// Sound entries
	void addEntry(Id soundId, SoundType type, Common::String filename, LoadFrom loadFrom, SoundFormat format, bool a4, int32 soundChunk);
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

} // End of namespace Ring

#endif // RING_SOUNDMANAGER_H
