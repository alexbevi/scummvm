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

#ifndef RING_FAUST_APPLICATION_H
#define RING_FAUST_APPLICATION_H

#include "ring/base/application.h"

namespace Ring {

class ApplicationFaust : public Application {
public:
	ApplicationFaust(RingEngine *engine);
	~ApplicationFaust();

	//////////////////////////////////////////////////////////////////////////
	// Initialization
	virtual void initLanguages();
	virtual void initFont();
	virtual void setup();
	virtual void initZones();
	virtual void initBag();

	//////////////////////////////////////////////////////////////////////////
	// Startup & Menu
	virtual void showStartupScreen();
	virtual void startMenu(bool savegame);
	virtual void showMenu(ZoneId zone, MenuAction menuAction);
	virtual void showCredits();
	virtual void loadPreferences();

	//////////////////////////////////////////////////////////////////////////
	// Drawing
	virtual void draw();

	//////////////////////////////////////////////////////////////////////////
	// Messages
	virtual void messageInsertCd(ZoneId zone);

	//////////////////////////////////////////////////////////////////////////
	// Zone
	virtual void setupZone(ZoneId zone, SetupType type);

	//////////////////////////////////////////////////////////////////////////
	// Visual
	virtual Visual *createVisual(Id visualId, uint32 a3, uint32 a4, uint32 left, uint32 top, uint32 offsetY, uint32 height, uint32 progressMultiplier, uint32 progressColor);

private:
	int32 _slot;
	ZoneId _zone;

	//////////////////////////////////////////////////////////////////////////
	// Start menu
	//////////////////////////////////////////////////////////////////////////
	void setProgressAndShowMenu(ProgressState progress);
	void loadAndStartLogin();
	void loadAndInitZone();
	void initZone();
	void initMenuSave(bool savegame);
	void initMenu2();
	void initMenu3();
};

} // End of namespace Ring

#endif // RING_FAUST_APPLICATION_H
