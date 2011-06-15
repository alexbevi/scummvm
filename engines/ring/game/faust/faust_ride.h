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

#ifndef RING_FAUST_RIDE_H
#define RING_FAUST_RIDE_H

#include "ring/shared.h"

namespace Ring {

class ApplicationFaust;
class EventHandlerFaust;

class EventRideFaust {
public:
	EventRideFaust(ApplicationFaust *application, EventHandlerFaust *eventHandler);
	~EventRideFaust();

	void onBeforeRideZone2(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onBeforeRideZone4(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onBeforeRideZone5(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onBeforeRideZone8(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onBeforeRideZone14(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onBeforeRideZone15(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);

	void onAfterRideZone2(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone3(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone4(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone6(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone8(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone10(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone12(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone13(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone14(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone15(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone16(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZone17(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);

private:
	ApplicationFaust  *_app;
	EventHandlerFaust *_event;
};

} // End of namespace Ring

#endif // RING_FAUST_RIDE_H
