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

#ifndef RING_GAME_H
#define RING_GAME_H

#include "ring/shared.h"

namespace Ring {

class Application;
class SaveLoad;

class Game {
public:
	Game(Application* application);
	~Game();

	// Setup
	void setup();

	// Set
	void setupZone(Zone zone, uint32 a2);
	void setZone(Zone zone, uint32 a2);
	void setZoneAndEnableBag(Zone zone);
	void setZoneNI(Zone zone, uint32 a2);
	void setZoneRH(Zone zone, uint32 a2);
	void setZoneFO(Zone zone, uint32 a2);
	void setZoneRO(Zone zone, uint32 a2);
	void setZoneWA(Zone zone, uint32 a2);
	void setZoneAS(Zone zone, uint32 a2);
	void setZoneN2(Zone zone, uint32 a2);

	// Initialization
	void initZones();
	void initZoneSY();
	void initZoneNI();
	void initZoneRH();
	void initZoneFO();
	void initZoneRO();
	void initZoneWA();
	void initZoneAS();
	void initZoneN2();

private:
	Application *_app;
	SaveLoad    *_saveLoad;

};

} // End of namespace Ring

#endif // RING_GAME_H
