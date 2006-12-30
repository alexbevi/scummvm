/* ScummVM - Scumm Interpreter
 * Copyright (C) 2003-2006 The ScummVM project
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
#include "kom/font.h"

namespace Kom {

Font::Font(FilesystemNode fileNode) : _data(0) {
	Common::File f;
	f.open(fileNode);
	_data = new byte[f.size()];
	f.read(_data, f.size());
	f.close();
}

Font::~Font() {
	delete[] _data;
}

const byte *Font::getCharData(char c) {
	uint16 offset = READ_LE_UINT16(_data + 2*c);
	return _data + offset;
}

} // End of namespace Kom
