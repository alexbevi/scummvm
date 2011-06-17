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

#ifndef RING_FAUST_BUTTON_H
#define RING_FAUST_BUTTON_H

#include "ring/shared.h"

#include "common/rect.h"

namespace Ring {

class ApplicationFaust;
class EventHandlerFaust;

class EventButtonFaust {
public:
	EventButtonFaust(ApplicationFaust *application);
	~EventButtonFaust();

	void onButtonDownZoneSY(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonDownZone5(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);

	void onButtonUpZoneSY(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone2(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone3(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone4(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone5(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone6(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone7(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone8(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone9(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone10(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone11(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone12(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone13(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone14(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone15(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone16(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);
	void onButtonUpZone17(ObjectId id, uint32 target, Id puzzleRotationId, uint32 a4, const Common::Point &point);

private:
	ApplicationFaust  *_app;
};

} // End of namespace Ring

#endif // RING_FAUST_BUTTON_H
