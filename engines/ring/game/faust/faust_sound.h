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

#ifndef RING_FAUST_SOUND_H
#define RING_FAUST_SOUND_H

#include "ring/shared.h"

namespace Ring {

class ApplicationFaust;

class EventSoundFaust {
public:
	EventSoundFaust(ApplicationFaust *application);
	~EventSoundFaust();

	void onSoundZoneSY(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZone3(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZone4(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZone5(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZone8(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZone12(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZone14(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZone15(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZone17(Id id, SoundType type, uint32 a3, bool process);


private:
	ApplicationFaust  *_app;
};

} // End of namespace Ring

#endif // RING_FAUST_SOUND_H
