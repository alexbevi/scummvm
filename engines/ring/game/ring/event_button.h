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

#ifndef RING_EVENTBUTTONRING_H
#define RING_EVENTBUTTONRING_H

#include "ring/shared.h"

#include "common/rect.h"

namespace Ring {

class ApplicationRing;
class EventHandlerRing;

class EventButtonRing {
public:
	EventButtonRing(ApplicationRing *application, EventHandlerRing *eventHandler);
	~EventButtonRing();

	//////////////////////////////////////////////////////////////////////////
	// Button down
	//////////////////////////////////////////////////////////////////////////
	void onButtonDownZoneNI(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonDownZoneRO(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonDownZoneN2(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);

	void onButtonUpZoneSY(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZoneNI(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZoneRH(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZoneFO(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZoneRO(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZoneWA(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZoneAS(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZoneN2(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);

	void onButtonUp2ZoneWA(ObjectId id, uint32 index, Id puzzleRotationId, uint32 a4, const Common::Point &point);

	// Accessors

private:
	ApplicationRing  *_app;
	EventHandlerRing *_event;

	// SY
	uint32       _presentationIndexSY;
	bool         _prefsSubtitles;
	int32        _prefsVolumeDialog;
	bool         _object1Visible;

	void waitTicks(uint32 ticks) const;

	// Savegames
	bool copySavedTimers(uint32 slot) const;
	bool removeSavedTimers(uint32 slot) const;

	friend class EventHandlerRing;
};

} // End of namespace Ring

#endif // RING_EVENTBUTTONRING_H
