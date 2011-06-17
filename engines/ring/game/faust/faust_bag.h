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

#ifndef RING_FAUST_BAG_H
#define RING_FAUST_BAG_H

#include "ring/shared.h"

#include "common/rect.h"

namespace Ring {

class ApplicationFaust;
class EventHandlerFaust;

class DragControl;

class EventBagFaust {
public:
	EventBagFaust(ApplicationFaust *application);
	~EventBagFaust();

	void onBagZoneSY(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZone3(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZone4(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZone5(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZone8(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZone12(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZone13(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);
	void onBagZone14(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte type);

	void onBagClickedObjectZone2(ObjectId id);
	void onBagClickedObjectZone3(ObjectId id);
	void onBagClickedObjectZone4(ObjectId id);
	void onBagClickedObjectZone8(ObjectId id);
	void onBagClickedObjectZone14(ObjectId id);
	void onBagClickedObjectZone15(ObjectId id);

	void onUpdateBagZoneSY(const Common::Point &point);
	void onUpdateBagZone3(const Common::Point &point);
	void onUpdateBagZone4(const Common::Point &point);
	void onUpdateBagZone6(const Common::Point &point);
	void onUpdateBagZone12(const Common::Point &point);
	void onUpdateBagZone14(const Common::Point &point);
	void onUpdateBagZone15(const Common::Point &point);
	void onUpdateBagZone16(const Common::Point &point);


private:
	ApplicationFaust  *_app;
};

} // End of namespace Ring

#endif // RING_FAUST_BAG_H
