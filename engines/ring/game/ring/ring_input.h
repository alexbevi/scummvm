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

#ifndef RING_EVENTINPUTRING_H
#define RING_EVENTINPUTRING_H

#include "ring/shared.h"

#include "common/events.h"

namespace Ring {

class ApplicationRing;
class EventHandlerRing;

class Accessibility;
class Movability;

class EventInputRing {
public:
	EventInputRing(ApplicationRing *application, EventHandlerRing *eventHandler);
	~EventInputRing();

	void onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed);
	void onMouseLeftButtonDown(const Common::Event &evt);
	void onMouseRightButtonUp(const Common::Event &evt) const;
	void onKeyDown(Common::Event &evt);

private:
	ApplicationRing  *_app;
	EventHandlerRing *_event;

	bool         _controlNotPressed;

	//////////////////////////////////////////////////////////////////////////
	// Mouse
	//////////////////////////////////////////////////////////////////////////
	void onMouseLeftButtonUp(const Common::Event &evt);
	bool handleLeftButtonDown(Accessibility *accessibility, uint32 index, Id id, const Common::Point &point);

	bool handleLeftButtonUp(Accessibility *accessibility, Id id, const Common::Point &point);
	bool handleLeftButtonUp(Movability *movability, uint32 index, Id id, bool isRotation = false);

	//////////////////////////////////////////////////////////////////////////
	// Key
	//////////////////////////////////////////////////////////////////////////
	void onKeyDownZone(const Common::KeyState &keyState);
	void onKeyDownZoneSY(const Common::KeyState &keyState);
};

} // End of namespace Ring

#endif // RING_EVENTINPUTRING_H
