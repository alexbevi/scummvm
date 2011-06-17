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

#ifndef RING_RING_SOUND_H
#define RING_RING_SOUND_H

#include "ring/shared.h"

namespace Ring {

class ApplicationRing;

class EventSoundRing {
public:
	EventSoundRing(ApplicationRing *application);
	~EventSoundRing();

	void onSoundZoneSY(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZoneNI(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZoneRH(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZoneFO(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZoneRO(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZoneWA(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZoneAS(Id id, SoundType type, uint32 a3, bool process);
	void onSoundZoneN2(Id id, SoundType type, uint32 a3, bool process);

private:
	ApplicationRing  *_app;
};

} // End of namespace Ring

#endif // RING_RING_SOUND_H
