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

#ifndef RING_FAUST_ANIMATION_H
#define RING_FAUST_ANIMATION_H

#include "ring/shared.h"

namespace Ring {

class ApplicationFaust;

class EventAnimationFaust {
public:
	EventAnimationFaust(ApplicationFaust *application);
	~EventAnimationFaust();

	void onAnimationNextFrameZoneSY(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone3(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone4(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone5(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone6(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone7(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone8(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone9(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone10(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone11(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone12(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone13(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone14(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);
	void onAnimationNextFrameZone15(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);

private:
	ApplicationFaust *_app;
};

} // End of namespace Ring

#endif // RING_FAUST_ANIMATION_H
