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

#ifndef RING_RING_ANIMATION_H
#define RING_RING_ANIMATION_H

#include "ring/shared.h"

namespace Ring {

class ApplicationRing;

class EventAnimationRing {
public:
	EventAnimationRing(ApplicationRing *application);
	~EventAnimationRing();

	//////////////////////////////////////////////////////////////////////////
	// Animation
	//////////////////////////////////////////////////////////////////////////
	void onAnimationNextFrameZoneNI(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZoneRH(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZoneFO(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZoneRO(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZoneWA(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZoneAS(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZoneN2(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);

	void onAnimationZoneWA(uint32 type, Id animationId, const Common::String &name, uint32 frame, uint32 a5);

private:
	ApplicationRing  *_app;

	void sub_433FA0();
	void sub_4340C0();
};

} // End of namespace Ring

#endif // RING_RING_ANIMATION_H
