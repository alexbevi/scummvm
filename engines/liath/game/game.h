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

#ifndef LIATH_GAME_H
#define LIATH_GAME_H

#include "liath/shared.h"

namespace Liath {

class LiathEngine;

class Interpreter;

class GameManager {
public:

	GameManager(LiathEngine *engine);
	~GameManager();

	void playGame(ActionIndex action);

	// Opcodes
	OpcodeRet rnd(OpcodeParameters *parameters);
	OpcodeRet global(OpcodeParameters *parameters);
	OpcodeRet key(OpcodeParameters *parameters);
	OpcodeRet bkey(OpcodeParameters *parameters);

	// Functions
	void letValue(ParamOrigin type, HeroIndex index, uint32 offset, uint32 val);
	int32 getValue(ParamOrigin type, HeroIndex index, uint32 offset);

	int32 getGlobal(uint32 offset);
	void setGlobal(uint32 offset, int32 val);
	OpcodeRet getReturnValue(int val, bool testValue);

private:
	LiathEngine* _engine;

	// Interpreter
	Interpreter *_interpreter;

	// Data
	uint32 *_gInt;
	uint32 *_gQuit;
	uint32 _param;
	uint32 _gParam;
	int32 _timer;
	uint32 _oldTimer;
	uint32 _countHero;
	uint32 _countVar;

	// Global area
	void *_globalVar;

	// Variables
	CdNumber _currentCd;
	ActionIndex _currentAction;
	WorkInfo *_currentWorkInfo;

	void load(ActionIndex action, GameData *data);
	void unload();
	void playAction();
	void processAction();
};

} // End of namespace Liath

#endif // LIATH_GAME_H
