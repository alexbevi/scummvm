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

#ifndef RING_RING_BAG_H
#define RING_RING_BAG_H

#include "ring/shared.h"

#include "common/rect.h"

namespace Ring {

class ApplicationRing;
class DragControl;
class EventHandlerRing;

class EventBagRing {
public:
	EventBagRing(ApplicationRing *application, EventHandlerRing *eventHandler);
	~EventBagRing();

	void onBagZoneSY(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZoneNI(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZoneFO(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZoneRO(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZoneN2(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);

	//////////////////////////////////////////////////////////////////////////
	// Input
	//////////////////////////////////////////////////////////////////////////
	void onBagClickedObjectZoneFO(ObjectId id);

	//////////////////////////////////////////////////////////////////////////
	// Update
	//////////////////////////////////////////////////////////////////////////
	void onUpdateBagZoneSY(const Common::Point &point);

	//////////////////////////////////////////////////////////////////////////
	// Zone
	//////////////////////////////////////////////////////////////////////////
	void onBagZoneSwitch();

private:
	ApplicationRing  *_app;
	EventHandlerRing *_event;

	// SY
	int16        _offsetX;
	int16        _offsetX0;
	int16        _coordX;

	// NI
	bool         _stateBagNI;
	int32        _presentationIndexNI;
	int32        _indexOffsetNI;
	int32        _distNI;

	// RO
	int32        _presentationIndexBagRO;
	int32        _presentationIndexBagRO2;

	// N2
	int32        _presentationIndexN2;
	int32        _distN2;

	void sub_445930();
};

} // End of namespace Ring

#endif // RING_RING_BAG_H
