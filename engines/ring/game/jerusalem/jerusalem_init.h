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
 */

#ifndef RING_JERUSALEM_INIT_H
#define RING_JERUSALEM_INIT_H

#include "ring/shared.h"

namespace Ring {

class ApplicationJerusalem;

class EventInitJerusalem {
public:
	EventInitJerusalem(ApplicationJerusalem *application);
	~EventInitJerusalem();

private:
	ApplicationJerusalem *_app;

	void initZoneSystem();
	void initZone1();
	void initZone2();
	void initZone3();
	void initZone4();
	void initZone5();
	void initZone6();
	void initZone7();
	void initZone8();
	void initZone9();
	void initZone10();
	void initZone11();
	void initZone12();
};

} // End of namespace Ring

#endif // RING_JERUSALEM_INIT_H
