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

#ifndef RING_FAUST_ZONE_H
#define RING_FAUST_ZONE_H

#include "ring/shared.h"

#include "common/rect.h"

namespace Ring {

class ApplicationFaust;
class EventHandlerFaust;

class EventZoneFaust {
public:
	EventZoneFaust(ApplicationFaust *application, EventHandlerFaust *eventHandler);
	~EventZoneFaust();

	//////////////////////////////////////////////////////////////////////////
	// Update
	//////////////////////////////////////////////////////////////////////////
	void onUpdateBeforeZoneSY(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	void onUpdateBeforeZone3(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	void onUpdateBeforeZone4(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	void onUpdateBeforeZone6(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	void onUpdateBeforeZone8(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	void onUpdateBeforeZone12(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	void onUpdateBeforeZone14(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	void onUpdateBeforeZone15(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	void onUpdateBeforeZone16(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);

private:
	ApplicationFaust  *_app;
	EventHandlerFaust *_event;
};

} // End of namespace Ring

#endif // RING_FAUST_ZONE_H
