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
 */

#ifndef ASYLUM_PIPES_H
#define ASYLUM_PIPES_H

#include "asylum/puzzles/puzzle.h"

#include "common/list.h"
#include "common/hashmap.h"

#include "asylum/puzzles/pipesclasses.h"

namespace Asylum {

class AsylumEngine;

class PuzzlePipes : public Puzzle {
public:
	PuzzlePipes(AsylumEngine *engine);
	~PuzzlePipes();

	void reset();
private:
	int32 _previousMusicVolume;
	int32 _rectIndex;
	uint32 _frameIndex, _frameIndexLever;
	bool _levelFlags[5];
	float _levelValues[4];
	bool _isLeverReady;
	Common::HashMap<uint32, uint32> _connectorResources;
	Connector _connectors[connectorsCount];
	Peephole _peepholes[peepholesCount];
	Peephole *_sinks[4], *_sources[4];

	//////////////////////////////////////////////////////////////////////////
	// Event Handling
	//////////////////////////////////////////////////////////////////////////
	bool init(const AsylumEvent &evt);
	bool update(const AsylumEvent &evt);
	bool mouseLeftDown(const AsylumEvent &evt);
	bool mouseRightDown(const AsylumEvent &evt);

	//////////////////////////////////////////////////////////////////////////
	// Helpers
	//////////////////////////////////////////////////////////////////////////
	void initResources();
	void setup();
	void updateCursor();
	int32 findRect();
	uint32 checkFlags();
	void startUpWater();
};

} // End of namespace Asylum

#endif // ASYLUM_PIPES_H
