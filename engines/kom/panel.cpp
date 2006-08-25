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

Panel::Panel(KomEngine *vm, FilesystemNode fileNode) : _vm(vm) {
	File f;
	f.open(fileNode);

	f.seek(4);

	_panelData = new byte[SCREEN_W * PANEL_H];
	f.read(_panelData, SCREEN_W * PANEL_H);

	f.close();
}

Panel::~Panel() {
	delete[] _panelData;
}

void Panel::display() {
	_vm->screen()->drawPanel(_panelData);
}

} // End of namespace Kom
