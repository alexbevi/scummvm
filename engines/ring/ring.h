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

#ifndef RING_H
#define RING_H

#include "ring/debug.h"

#include "common/random.h"

#include "engines/advancedDetector.h"
#include "engines/engine.h"

#include "graphics/pixelformat.h"

/**
 * This is the namespace of the Ring engine.
 *
 * Status of this engine:
 *  Work in progress
 *
 * Maintainers:
 *  Littleboy
 *
 * Supported games:
 *  - Ring: The Legend of the Nibelungen
 *
 * Games using the engine
 *  - Pilgrim: Faith as a Weapon (1998) ???
 *  - Ring: The Legend of the Nibelungen (1998)
 *  - Faust: Seven Games of the Soul (1999)
 *  - TimeScape: Journey to Pompeii (2000)
 *  - Pilgrim 2: The Legend of the Prophet and the Assassin (2000)
 *  - Pilgrim 3: The Secrets of Alamut (2001) ???
 *  - Jerusalem: The Three Roads to the Holy Land (2002)
 */
namespace Ring {

struct RingGameDescription {
	ADGameDescription desc;

	int gameType;
	uint32 features;
};

enum RingGameType {
	GameTypeRing = 1
};

// Available languages
enum {
	LANG_ENG = 1 << 0,
	LANG_FRA = 1 << 1,
	LANG_GER = 1 << 2,
	LANG_GRE = 1 << 3,
	LANG_HEB = 1 << 4,
	LANG_HOL = 1 << 5,
	LANG_ITA = 1 << 6,
	LANG_SLO = 1 << 7,
	LANG_SPA = 1 << 8,
	LANG_SWE = 1 << 9
};

class Application;
class Debugger;

class RingEngine : public Engine {
protected:
	// Engine APIs
	Common::Error run();
	virtual void errorString(const char *buf_input, char *buf_output, int buf_output_size);
	virtual bool hasFeature(EngineFeature f) const;
	virtual Debugger *getDebugger() { return _debugger; }

public:
	RingEngine(OSystem *syst, const RingGameDescription *gd);
	~RingEngine();

	Application *getApplication() { return _application; }

	// Misc
	Common::RandomSource getRandom() const {return _random; }

private:
	const RingGameDescription *_gameDescription;
	Graphics::PixelFormat _pixelFormat;

	// Misc
	Debugger *_debugger;
	Common::RandomSource _random;

	// Game
	Application *_application;
	bool _mouseButtonPressed;

};

} // End of namespace Ring

#endif // RING_H
