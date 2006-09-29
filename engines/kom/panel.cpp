/* ScummVM - Scumm Interpreter
 * Copyright (C) 2005-2006 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
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

Panel::Panel(KomEngine *vm, FilesystemNode fileNode) : _vm(vm), _isEnabled(true), _isLoading(false) {
	File f;
	f.open(fileNode);

	f.seek(4);

	_panelDataRaw = new byte[SCREEN_W * PANEL_H];
	f.read(_panelDataRaw, SCREEN_W * PANEL_H);

	f.close();

	_panelData = new byte[SCREEN_W * PANEL_H];
	memcpy(_panelData, _panelDataRaw, SCREEN_W * PANEL_H);
}

Panel::~Panel() {
	delete[] _panelDataRaw;
}

void Panel::clear() {
	memcpy(_panelData, _panelDataRaw, SCREEN_W * PANEL_H);
}

void Panel::update() {
	enable(true);
	clear();

	// TODO: Draw texts

	_vm->screen()->drawPanel(_panelData);

	if (_isLoading) {
		Actor *mouse = _vm->actorMan()->getMouse();
		mouse->enable(true);
		mouse->setScope(6, 0);
		mouse->setPos(303, 185);
		mouse->display();
		mouse->enable(false);
	}

	// TODO: lose/get items
}

void Panel::showLoading(bool isLoading) {
	if (isLoading != _isLoading) {
		_isLoading = isLoading;
		update();
		_vm->screen()->updatePanelArea();
	}
}

void Panel::setLocationDesc(char *desc) {
}

} // End of namespace Kom
