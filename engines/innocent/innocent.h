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

#ifndef INNOCENT_H
#define INNOCENT_H

#include <memory>

#include "common/ptr.h"
#include "common/random.h"
#include "engines/engine.h"

class MidiDriver;

namespace Common {
//
class EventManager;

}

namespace Innocent {

class Console;
class Interpreter;
class Resources;
class Graphics;
class Logic;
class Debugger;

class Engine : public ::Engine {
public:
	Engine(OSystem *syst);
	~Engine();

	virtual Common::Error run();
	void delay(int millis) const;

	Logic *logic() { return _logic; }
	Resources *resources() { return _resources; }
	Graphics *graphics() { return _graphics; }
	Debugger *debugger() { return _debugger; }
	Common::EventManager *eventMan() { return _eventMan; }
	MidiDriver *musicDriver() const { return _musicDriver.get(); }

	uint16 getRandom(uint16 max) const;

	friend class Interpreter;
	bool _copyProtection;

	static Engine &instance() { return *me; }
	bool escapePressed() const;

private:
	Logic *_logic;
	Resources *_resources;
	Graphics *_graphics;
	Debugger *_debugger;
	std::auto_ptr<MidiDriver> _musicDriver;

	Common::RandomSource *_rnd;
	mutable int _lastTicks, _startRoom;

	void handleEvents();
	static Engine *me;
};

#define Eng Engine::instance()

} // End of namespace Innocent

#endif // INNOCENT_H
