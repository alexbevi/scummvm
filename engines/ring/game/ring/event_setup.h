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

#ifndef RING_EVENTSETUPRING_H
#define RING_EVENTSETUPRING_H

#include "ring/shared.h"

namespace Ring {

class ApplicationRing;

class EventSetupRing {
public:
	EventSetupRing(ApplicationRing *application);
	~EventSetupRing();

	void onSetupZoneNI(SetupType type);
	void onSetupZoneRH(SetupType type);
	void onSetupZoneFO(SetupType type);
	void onSetupZoneRO(SetupType type);
	void onSetupZoneWA(SetupType type);
	void onSetupZoneAS(SetupType type);
	void onSetupZoneN2(SetupType type);

	void onSetupLoadTimers(Common::String zoneName, Id testId1, Id puzzleRotationId, Id testId2);


private:
	ApplicationRing *_app;
};

} // End of namespace Ring

#endif // RING_EVENTSETUPRING_H
