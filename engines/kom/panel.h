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

#ifndef KOM_PANEL_H
#define KOM_PANEL_H

#include "common/system.h"
#include "common/fs.h"
#include "common/list.h"

#include "kom/kom.h"

namespace Kom {

class Panel {
public:
	Panel(KomEngine *vm, const char *filename);
	~Panel();

	void update();
	void showLoading(bool isLoading);
	void suppressLoading() { _suppressLoading++; }
	void allowLoading() { if (_suppressLoading > 0) _suppressLoading--; }
	void setLocationDesc(const char *desc);
	void setActionDesc(const char *desc);
	void setHotspotDesc(const char *desc);
	bool isEnabled() const { return _isEnabled; }
	bool isDirty() const { return _isDirty; }
	void enable(bool state) { _isEnabled = state; }
	void clear();

	const byte *getPanelBackground() { return _panelData; }

	void addObject(int obj) { _gotObjects.push_back(obj); }

private:

	KomEngine *_vm;

	byte *_panelData;
	byte *_panelBuf;

	bool _isEnabled;
	bool _isLoading;
	bool _isDirty;
	int _suppressLoading;

	char *_locationDesc;
	char *_actionDesc;
	char *_hotspotDesc;

	Common::List<int> _gotObjects;
	int _gotObjTime;

};
} // End of namespace Kom

#endif
