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

#include "common/file.h"

#include "kom/kom.h"
#include "kom/panel.h"
#include "kom/screen.h"

using Common::File;

namespace Kom {

Panel::Panel(KomEngine *vm, FilesystemNode fileNode) : _vm(vm),
	_isEnabled(true), _isLoading(false), _noLoading(0),
	_locationDesc(0), _actionDesc(0), _hotspotDesc(0) {
	File f;
	f.open(fileNode);

	f.seek(4);

	_panelData = new byte[SCREEN_W * PANEL_H];
	f.read(_panelData, SCREEN_W * PANEL_H);

	f.close();

	_panelBuf = new byte[SCREEN_W * PANEL_H];
	memcpy(_panelBuf, _panelData, SCREEN_W * PANEL_H);

	_isDirty = true;
}

Panel::~Panel() {
	delete[] _panelData;
	delete[] _panelBuf;
	delete[] _locationDesc;
}

void Panel::clear() {
	memcpy(_panelBuf, _panelData, SCREEN_W * PANEL_H);
}

void Panel::update() {
	_isDirty = false;
	enable(true);
	clear();

	// Draw texts
	// FIXME - the location desc is fully centered, while the original prints it
	//         4 pixels to the right
	if (_locationDesc)
		_vm->screen()->writeTextCentered(_panelBuf, _locationDesc, 3, 31, true);

	if (_actionDesc)
		_vm->screen()->writeText(_panelBuf, _actionDesc, 12, 10, 31, true);

	if (_hotspotDesc)
		_vm->screen()->writeText(_panelBuf, _hotspotDesc, 22, 10, 31, true);

	// TODO: lose/get items
	//

	_vm->screen()->drawPanel(_panelBuf);
}

void Panel::showLoading(bool isLoading) {
	if (_isLoading != isLoading) {
		_isLoading = isLoading;

		_vm->screen()->drawPanel(_panelBuf);

		Actor *mouse = _vm->actorMan()->getMouse();
		if (_isLoading) {
			mouse->enable(1);
			mouse->setScope(6, 0);
			mouse->setPos(303, 185);
			mouse->display();
			mouse->enable(0);
		} else {
			mouse->setPos(0, 0);
		}

		// The 'loading' icon should be copied directly to screen
		_vm->screen()->updatePanelOnScreen();
	}
}

void Panel::setLocationDesc(const char *desc) {
	delete[] _locationDesc;
	_locationDesc = new char[strlen(desc) + 1];
	strcpy(_locationDesc, desc);
	_isDirty = true;
}

void Panel::setActionDesc(const char *desc) {
	delete[] _actionDesc;
	_actionDesc = new char[strlen(desc) + 1];
	strcpy(_actionDesc, desc);
	_isDirty = true;
}

void Panel::setHotspotDesc(const char *desc) {
	delete[] _hotspotDesc;
	_hotspotDesc = new char[strlen(desc) + 1];
	strcpy(_hotspotDesc, desc);
	_isDirty = true;
}

} // End of namespace Kom
