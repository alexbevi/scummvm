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

#ifndef RING_EVENTRIDERING_H
#define RING_EVENTRIDERING_H

#include "ring/shared.h"

namespace Ring {

class ApplicationRing;
class EventHandlerRing;

class EventRideRing {
public:
	EventRideRing(ApplicationRing *application, EventHandlerRing *eventHandler);
	~EventRideRing();

	void onBeforeRideZoneNI(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onBeforeRideZoneRH(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onBeforeRideZoneFO(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onBeforeRideZoneRO(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onBeforeRideZoneWA(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onBeforeRideZoneAS(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onBeforeRideZoneN2(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);

	void onAfterRideZoneNI(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZoneRH(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZoneFO(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZoneRO(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZoneWA(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZoneAS(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);
	void onAfterRideZoneN2(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 target, MovabilityType movabilityType);

private:
	ApplicationRing  *_app;
	EventHandlerRing *_event;

	uint32            _dword_4A1C00;
};

} // End of namespace Ring

#endif // RING_EVENTRIDERING_H
