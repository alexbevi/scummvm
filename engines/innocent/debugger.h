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

#ifndef INNOCENT_DEBUGGER_H
#define INNOCENT_DEBUGGER_H

#include "common/singleton.h"
#include "gui/debugger.h"

namespace Innocent {

class Engine;
class Logic;

class Debugger : public ::GUI::Debugger, public Common::Singleton<Debugger> {
public:
	Debugger();
	void setEngine(Engine *vm);

	inline void opcodeStep() { if (_stepOpcodes) { _stepOpcodes = false; attach(); onFrame(); } }
	void clickHandler();

private:
	Logic *logic() const;

	Engine *_vm;

	#define CMD(name) bool cmd_##name(int argc, const char **argv)
	bool cmd_setBackdrop(int argc, const char **argv);
	bool cmd_paintText(int argc, const char **argv);
	bool cmd_paintSprite(int argc, const char **argv);
	bool cmd_listExits(int argc, const char **argv);
	bool cmd_showClickable(int argc, const char **argv);
	bool cmd_break(int argc, const char **argv);
	CMD(step);
	CMD(setVar);
	CMD(debugActor);
	#undef CMD

	bool _stepOpcodes;
	bool _breakOnClickHandler;
};

#define Debug Debugger::instance()

} // End of namespace Innocent

#endif // defined INNOCENT_DEBUGGER_H
