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

#ifndef RING_VISUALRING_H
#define RING_VISUALRING_H

#include "ring/graphics/visual.h"

namespace Ring {

class ApplicationRing;

class VisualElementRing : public Visual {
public:
	VisualElementRing(Id id);
	virtual ~VisualElementRing();

	virtual void alloc();
	virtual void dealloc();

	virtual uint32 handleLeftButtonUp(const Common::Point &point) { return 0; }
	virtual uint32 handleUpdate(const Common::Point &point) { return 0; }

	void init(uint32 a1, uint32 a2, uint32 left, uint32 top, uint32 offsetY, uint32 height, uint32 progressMultiplier, uint32 progressColor);
	virtual void draw();

private:
	uint32 _field_D;
	uint32 _field_11;
	uint32 _left;
	uint32 _top;
	uint32 _offsetY;
	uint32 _height;
	uint32 _progressMultiplier;
	uint32 _progressColor;  ///< The progress color in RGB form (0x00bbggrr)
	bool   _initialized;
	float  _progress1;
	float  _progress2;
	float  _progress3;
	float  _progress4;

	/**
	 * 	Sets up the progress.
	 *
	 * @param	progressId			Identifier for the progress.
	 * @param	textIndex			Zero-based index of the text.
	 * @param [in,out]	progress	If non-null, the progress.
	 */
	void setupProgress(Id progressId, uint32 textIndex, float *progress) const;
};

class EventVisualRing {
public:
	EventVisualRing(ApplicationRing *application);
	~EventVisualRing();

	void onVisualListZoneSY(Id id, uint32 a2, const Common::Point &point);

private:
	ApplicationRing  *_app;
};

} // End of namespace Ring

#endif // RING_VISUALRING_H
