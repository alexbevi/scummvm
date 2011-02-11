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

#ifndef RING_EVENTHANDLER_POMPEII_H
#define RING_EVENTHANDLER_POMPEII_H

#include "ring/base/event.h"

namespace Ring {

class Application;

class EventHandlerPompeii : public EventHandler {
public:
	EventHandlerPompeii(Application *application);
	virtual ~EventHandlerPompeii();

	virtual void onMouseLeftButtonUp(Common::Event &evt, bool isControlPressed);
	virtual void onMouseLeftButtonDown(Common::Event &evt);
	virtual void onMouseRightButtonUp(Common::Event &evt);
	virtual void onKeyDown(Common::Event &evt);
	virtual void onTimer(TimerId id);
	virtual void onSound(Id id, SoundType type, uint32 a3);
	virtual void onSetup(Zone zone, uint32 a2);

private:
	Application *_app;
};

} // End of namespace Ring

#endif // RING_EVENTHANDLER_POMPEII_H
