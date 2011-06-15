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

#ifndef RING_EVENTZONERING_H
#define RING_EVENTZONERING_H

#include "ring/shared.h"

#include "common/rect.h"

namespace Ring {

class ApplicationRing;

class EventZoneRing {
public:
	EventZoneRing(ApplicationRing *application);
	~EventZoneRing();

	//////////////////////////////////////////////////////////////////////////
	// Zone switching
	//////////////////////////////////////////////////////////////////////////
	void onSwitchZoneNI(uint32 type);
	void onSwitchZoneRH(uint32 type);
	void onSwitchZoneFO(uint32 type);
	void onSwitchZoneRO(uint32 type);
	void onSwitchZoneWA(uint32 type);
	void onSwitchZoneAS(uint32 type);
	void onSwitchZoneN2(uint32 type);

	//////////////////////////////////////////////////////////////////////////
	// Update
	//////////////////////////////////////////////////////////////////////////
	void onUpdateBeforeZoneSY(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	void onUpdateBeforeZoneNI(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);
	void onUpdateBeforeZoneN2(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, const Common::Point &point);

private:
	ApplicationRing *_app;
};

} // End of namespace Ring

#endif // RING_EVENTZONERING_H
