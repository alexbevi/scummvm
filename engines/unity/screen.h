/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef UNITY_SCREEN_H
#define UNITY_SCREEN_H

#include "common/array.h"
#include "common/rect.h"

namespace Unity {

class UnityEngine;

struct UIControl {
	UIControl() : _id(0), _sprite(0xffffffff), _state(0) { }
	virtual ~UIControl() { }

	uint _id;
	uint _sprite;
	uint _state;
	Common::Rect _bounds;
};

class UIScreen {
public:
	UIScreen(UnityEngine *vm);
	virtual ~UIScreen();

	virtual void start() = 0;
	virtual void shutdown() = 0;
	virtual void mouseMove(const Common::Point &pos) = 0;
	virtual void mouseClick(const Common::Point &pos) = 0;
	virtual void draw() = 0;

protected:
	UnityEngine *_vm;

	Common::Array<UIControl *> _controls;
	UIControl *getControlAt(const Common::Point &pos);
};

} // Unity

#endif

