/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 11
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 011110-1301, USA.
 */

#ifndef RING_FAUST_ZONE_11_H
#define RING_FAUST_ZONE_11_H

#include "ring/base/event.h"

#include "ring/shared.h"

namespace Ring {

class ApplicationFaust;

class Zone11Faust : public EventHandlerZone {
public:
	Zone11Faust(ApplicationFaust *application);
	~Zone11Faust();

	virtual void onInit();
	virtual void onTimer(TimerId id);
	virtual void onButtonUp(ObjectId id, Id target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	virtual void onAnimationNextFrame(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount);

private:
	ApplicationFaust *_app;
};

} // End of namespace Ring

#endif // RING_FAUST_ZONE_11_H
