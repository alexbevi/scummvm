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

#include "liath/play.h"

#include "liath/game/action.h"
#include "liath/game/segment.h"
#include "liath/game/interpreter.h"

#include "liath/helpers.h"
#include "liath/liath.h"
#include "liath/resource.h"

namespace Liath {

Play::Play(LiathEngine *engine) : _engine(engine) {
	_interpreter = new Interpreter(_engine);
}

Play::~Play() {
	delete _interpreter;

	// Zero-out passed pointers
	_engine = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////////

void Play::playGame(ActionIndex action) {

	// TODO allocate different data (shadow, make, etc.)

	// Read game segment
	getSegment()->load(kSegmentGame, 1);


	uint32 *gameData = getSegment()->getData(kSegmentGame, 2);
	// TODO update game data and set different objects

	// TODO setup current action

	// TODO Setup hero objects

	// Load hero data

	// Load sound data

	// Load text palette

	// Run the interpreter
	ObjectIndex *pObjectIndex = (!gameData[2]) ? 0 : getSegment()->getData(kSegmentGame, gameData[2]);

	_interpreter->interpret(pObjectIndex, getSegment()->get(kSegmentGame));
	while (getAction()->getCurrentAction())
		playAction();

	// TODO cleanup heros

	// TODO cleanup sound data

	// TODO cleanup palette and text data

	// TODO cleanup other data
}

void Play::playAction() {
	error("Play::playAction: Not implemented!");
}


} // End of namespace Liath
