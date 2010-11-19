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

#ifndef LIATH_WORKMANAGER_H
#define LIATH_WORKMANAGER_H

#include "liath/shared.h"

#include "common/array.h"

namespace Liath {

class LiathEngine;

class WorkManager {
public:

	WorkManager(LiathEngine *engine);
	~WorkManager();

	void unload();

	// Opcodes
	OpcodeRet startObj(ObjectIndex index);
	OpcodeRet cel(OpcodeParameters *parameters);
	OpcodeRet celExt(OpcodeParameters *parameters);
	OpcodeRet htime(OpcodeParameters *parameters);
	OpcodeRet hdark(OpcodeParameters *parameters);
	OpcodeRet restart();
	OpcodeRet popHerous();
	OpcodeRet pushHerous();
	OpcodeRet glass(OpcodeParameters *parameters, bool doIncrement);
	OpcodeRet stop(OpcodeParameters *parameters);
	OpcodeRet stopGlobal(OpcodeParameters *parameters);
	OpcodeRet stopHeroVar(OpcodeParameters *parameters);
	OpcodeRet stopVar(OpcodeParameters *parameters);
	OpcodeRet cont(OpcodeParameters *parameters);
	OpcodeRet contGlobal(OpcodeParameters *parameters);
	OpcodeRet contHeroVar(OpcodeParameters *parameters);
	OpcodeRet contVar(OpcodeParameters *parameters);
	OpcodeRet savetop(OpcodeParameters *parameters);
	OpcodeRet endtop();

	Work *getCurrent() { return _currentWork; }
	Work *get(HeroIndex index);
	Work *getFromTextIndex(int32 index);

	void setWorkInfo(WorkInfo *info) { _info = info; }
	WorkInfo *getWorkInfo() { return _info; }

	// Helpers
	void resetField_E00();

private:
	LiathEngine* _engine;

	Work* _currentWork;
	WorkInfo *_info;

	Common::Array<Work *> _works;
};

} // End of namespace Liath

#endif // LIATH_WORKMANAGER_H
