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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#ifndef ASYLUM_BARRIER_H
#define ASYLUM_BARRIER_H

#include "asylum/shared.h"

#include "asylum/system/sound.h"

#include "common/rect.h"
#include "common/stream.h"

namespace Asylum {

class Actor;
class AsylumEngine;

class Barrier {
public:
	Barrier(AsylumEngine *engine);
	virtual ~Barrier();

	//////////////////////////////////////////////////////////////////////////
	// Public variables & accessors
	//////////////////////////////////////////////////////////////////////////
	int32 x;
	int32 y;
	int32 flags;
	int32 actionType;

	void setFrameIndex(int32 index) { _frameIndex = index; }
	void setPriority(int32 priority) { _priority = priority; }
	void setResourceId(ResourceId id) { _resourceId = id; }

	void setField67C(int32 val) { _field_67C = val; }

	Common::Rect  *getBoundingRect() { return &_boundingRect; }
	uint32         getFrameIndex() { return _frameIndex; }
	uint32         getFrameCount() { return _frameCount; }
	int32          getId() { return _id; }
	int32          getPolygonIndex() { return _polygonIndex; }
	int32          getPriority() { return _priority; }
	ResourceId     getResourceId() { return _resourceId; }

	int32          getField74() { return _field_74; }
	int32          getField78() { return _field_78; }
	int32          getField7C() { return _field_7C; }
	int32          getField80() { return _field_80; }
	int32          getField67C() { return _field_67C; }
	int32          getField688() { return _field_688; }

	/////////////////////////////////////////////////////////////////////////
	// Loading & disabling
	/////////////////////////////////////////////////////////////////////////

	/**
	 * Loads the barrier data
	 *
	 * @param stream If non-null, the Common::SeekableReadStream to load from
	 */
	void load(Common::SeekableReadStream *stream);

	/**
	 * Sets the barrier disabled flag
	 */
	void disable();

	/**
	 * Sets the barrier destroyed flag and remove this barrier from the graphics queue
	 */
	void disableAndRemoveFromQueue();

	/////////////////////////////////////////////////////////////////////////
	// Visibility
	//////////////////////////////////////////////////////////////////////////

	/**
	 * Query if this barrier is on screen.
	 *
	 * @return true if on screen, false if not.
	 */
	bool isOnScreen();

	/**
	 * Query if this barrier is visible.
	 *
	 * @return true if visible, false if not.
	 */
	bool isVisible();

	/////////////////////////////////////////////////////////////////////////
	// Drawing & update
	//////////////////////////////////////////////////////////////////////////

	/**
	 * Draws the barrier
	 */
	void draw();

	/**
	 * Updates the barrier.
	 */
	void update();

	/**
	 * Sets the next frame.
	 *
	 * @param flags The flags.
	 */
	void setNextFrame(int flags);

	/////////////////////////////////////////////////////////////////////////
	// Misc
	////////////////////////////////////////////////////////////////////////

	/**
	 * Check if any items in the barrier sound array are playing,
	 * and based on their flag values, stop them accordingly
	 */
	void updateSoundItems();

	/**
	 * Stop the barrier related sounds
	 */
	void stopSound();

	/**
	 * Stop all barrier sounds (called from scripts)
	 */
	void stopAllSounds();

	bool checkFlags();

	int32 getRandomId(); // TODO Give this a better name?

private:
	AsylumEngine *_vm;

	int32 _index;   ///< our index

	//////////////////////////////////////////////////////////////////////////
	// Data
	//////////////////////////////////////////////////////////////////////////
	int32		   _id;
	ResourceId     _resourceId;
	// x, y
	Common::Rect   _boundingRect;
	int32		   _field_20;
	uint32		   _frameIndex;
	uint32		   _frameCount;
	int32		   _field_2C;
	int32		   _field_30;
	int32		   _field_34;
	// flags
	int32		   _field_3C;
	uint8		   _name[52];
	int32		   _field_74; // XXX looks like fields
	int32		   _field_78; // 74 => 80 have something
	int32		   _field_7C; // to do with calculating
	int32		   _field_80; // actor intersection
	int32		   _polygonIndex;
	// actionType
	GameFlag	   _gameFlags[10];
	int32		   _field_B4;
	int32		   _tickCount;
	int32		   _tickCount2;
	int32		   _field_C0;
	int32		   _priority;
	int32		   _actionListIdx;
	SoundItem	   _soundItems[16];
	FrameSoundItem _frameSoundItems[50];
	int32		   _field_67C;
	int32		   _soundX;
	int32		   _soundY;
	int32		   _field_688;
	int32		   _field_68C[5];
	ResourceId     _soundResourceId;
	int32		   _field_6A4;

}; // end of class Barrier

} // end of namespace Asylum

#endif
