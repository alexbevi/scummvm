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

#ifndef KOM_PANEL_H
#define KOM_PANEL_H

#include "common/system.h"
#include "common/fs.h"

#include "kom/kom.h"

namespace Kom {

class Panel {
public:
	Panel(KomEngine *vm, FilesystemNode fileNode);
	~Panel();

	void display();
	void showLoading(bool isLoading);
	void setLocationDesc(char *desc);

private:

	KomEngine *_vm;

	byte* _panelData;

};
} // End of namespace Kom

#endif
