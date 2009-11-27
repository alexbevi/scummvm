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
 *
 * $URL$
 * $Id$
 *
 */

#ifndef DARKSEED2_EVENTS_H
#define DARKSEED2_EVENTS_H

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/cursors.h"
#include "engines/darkseed2/graphicalobject.h"

namespace DarkSeed2 {

enum CursorMode {
	kCursorModeWalk = 0,
	kCursorModeLook,
	kCursorModeUse,
	kCursorModeNone
};

class Events {
public:
	Events(DarkSeed2Engine &vm);
	~Events();

	bool setupIntroSequence();

	void mainLoop();

private:
	struct ModeCursors {
		const Cursors::Cursor *inactive;
		const Cursors::Cursor *active;
	};

	DarkSeed2Engine *_vm;

	bool _inIntro;

	bool _canSwitchCursors;

	ModeCursors *_cursors;
	CursorMode _cursorMode;
	bool _cursorActive;

	SpriteObject _titleSprites[5];

	void handleInput();

	void mouseMoved(uint32 x, uint32 y);
	void mouseClickedLeft(uint32 x, uint32 y);
	void mouseClickedRight(uint32 x, uint32 y);

	void cycleCursorMode();
	void setCursor();
	void setCursor(CursorMode cursor, bool active);
	void setCursor(const Cursors::Cursor &cursor);

	bool roomEnter();
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_EVENTS_H
