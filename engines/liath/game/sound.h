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

#ifndef LIATH_SOUND_H
#define LIATH_SOUND_H

#include "liath/shared.h"

namespace Liath {

class LiathEngine;

class SoundManager {
public:
	enum SoundType {
		kSoundMusic = 1,
		kSoundEffects = 2,
		kSoundDialog = 3
	};

	SoundManager(LiathEngine *engine);
	~SoundManager();

	void load();
	void unload();

	// Opcodes
	OpcodeRet cash(OpcodeParameters *parameters);
	OpcodeRet playWave(OpcodeParameters *parameters);
	OpcodeRet playMusic(OpcodeParameters *parameters, bool doAdjustEffects);
	OpcodeRet stopMusic(OpcodeParameters *parameters);
	OpcodeRet gstopMusic(OpcodeParameters *parameters);
	OpcodeRet playMidi(OpcodeParameters *parameters, bool doLoop);
	OpcodeRet stopMidi();
	OpcodeRet volume(OpcodeParameters *parameters);

private:
	LiathEngine* _engine;

	Common::Array<Common::String> _waves;

	uint32 _musicLevel;
	uint32 _effectsLevel;
	uint32 _dialogLevel;

	void setLevel(SoundType type, uint32 level);
};

} // End of namespace Liath

#endif // LIATH_SOUND_H
