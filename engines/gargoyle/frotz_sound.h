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

#ifndef FROTZ_SOUND_H
#define FROTZ_SOUND_H

#include "common/fs.h"
#include "common/scummsys.h"
#include "common/singleton.h"
#include "common/util.h"

#include "gargoyle/blorb.h"

namespace Frotz {

struct FrotzSoundHeader {
    uint16 prefix;
    uint8 repeats;
    uint8 base_note;
    uint16 frequency;
    uint16 unused;
    uint16 length;
};

class FrotzSound {
private:
	Gargoyle::BlorbFile *_blorbFile;
	const Common::FSNode _soundFolder;
	Common::String *_gamePrefix;
	int _currentSample;
	bool _has_played_sound;
	byte *_sampleData;
	FrotzSoundHeader _sheader;

	void loadSample(int soundNumber);
	void stopSample() {};
	void resetSample();
public:
	FrotzSound(Gargoyle::BlorbFile *bf, const Common::FSNode &soundFolder);
	~FrotzSound();
	static FrotzSound *init(Gargoyle::BlorbFile *blorbFile, const Common::FSNode &soundFolder);

	void stop(int number) { warning("TODO: FrotzSound::stop"); }
	void play(int sound, int repeat, int volume, unsigned short eos);
};

} // End of namespace Frotz

#define SoundFrotz (::Frotz::FrotzSound::instance())

#endif

