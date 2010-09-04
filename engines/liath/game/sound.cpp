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

#include "liath/game/sound.h"

#include "liath/helpers.h"
#include "liath/liath.h"

namespace Liath {

SoundManager::SoundManager(LiathEngine *engine) : _engine(engine) {}

SoundManager::~SoundManager() {
	// Zero-out passed pointers
	_engine = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Loading
//////////////////////////////////////////////////////////////////////////
void SoundManager::load() {
	warning("SoundManager::load: Not implemented!");
}

void SoundManager::unload() {
	warning("SoundManager::unload: Not implemented!");
}

//////////////////////////////////////////////////////////////////////////
// Opcodes
//////////////////////////////////////////////////////////////////////////
OpcodeRet SoundManager::cash(OpcodeParameters *parameters) {
	error("SoundManager::cash: Not implemented!");
}

OpcodeRet SoundManager::playWave(OpcodeParameters *parameters) {
	error("SoundManager::playWave: Not implemented!");
}

OpcodeRet SoundManager::playMusic(OpcodeParameters *parameters, bool doAdjustEffects) {
	error("SoundManager::playMusic: Not implemented!");
}

OpcodeRet SoundManager::stopMusic(OpcodeParameters *parameters) {
	error("SoundManager::stopMusic: Not implemented!");
}

OpcodeRet SoundManager::gstopMusic(OpcodeParameters *parameters) {
	error("SoundManager::gstopMusic: Not implemented!");
}

OpcodeRet SoundManager::playMidi(OpcodeParameters *parameters, bool doLoop) {
	error("SoundManager::playMidi: Not implemented!");
}

OpcodeRet SoundManager::stopMidi() {
	error("SoundManager::stopMidi: Not implemented!");
}

OpcodeRet SoundManager::volume(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDWDD);

	debugC(kLiathDebugInterpreter, "  sound type: %d / level: %d\n", params->param1, params->param2);

	setLevel((SoundManager::SoundType)params->param1, 140 * params->param2 - 15);

	return kOpcodeRetDefault;
}

//////////////////////////////////////////////////////////////////////////
// Private functions
//////////////////////////////////////////////////////////////////////////

void SoundManager::setLevel(SoundType type, uint32 level) {
	switch (type) {
	default:
		error("SoundManager::setVolume: Invalid volume type (was: %d, valid=1-3)", type);

	case kSoundMusic:
		_musicLevel = level;
		break;

	case kSoundEffects:
		_effectsLevel = level;
		break;

	case kSoundDialog:
		_dialogLevel = level;
		break;
	}
}

} // End of namespace Liath
