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

#ifndef DARKSEED2_DARKSEED2_H
#define DARKSEED2_DARKSEED2_H

#include "common/system.h"

#include "engines/engine.h"

class MidiDriver;

namespace DarkSeed2 {

enum {
	kDebugResources = 1 << 0
};

struct DS2GameDescription;

class Resources;
class Graphics;
class Sound;
class Music;

class DarkSeed2Engine : public Engine {
public:
	Resources *_resources;
	Graphics  *_graphics;
	Sound     *_sound;
	Music     *_music;

	void pauseGame();

	DarkSeed2Engine(OSystem *syst);
	virtual ~DarkSeed2Engine();

	void initGame(const DS2GameDescription *gd);

private:
	MidiDriver *_midiDriver;

	// Engine APIs
	virtual Common::Error run();
	virtual bool hasFeature(EngineFeature f) const;
	virtual void pauseEngineIntern(bool pause);
	virtual void syncSoundSettings();

	bool init();
	bool initGraphics();
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_DARKSEED2_H
