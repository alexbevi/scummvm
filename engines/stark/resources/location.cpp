/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "engines/stark/resources/location.h"

#include "engines/stark/resources/layer.h"
#include "engines/stark/xrcreader.h"

namespace Stark {

Location::~Location() {
}

Location::Location(Resource *parent, byte subType, uint16 index, const Common::String &name) :
				Resource(parent, subType, index, name) {
	_type = TYPE;
}

void Location::onAllLoaded() {
	Resource::onAllLoaded();

	_layers = listChildren<Layer>();
}

RenderEntryArray Location::listRenderEntries() {
	RenderEntryArray renderEntries;

	for (uint i = 0; i < _layers.size(); i++) {
		Layer *layer = _layers[i];
		renderEntries.push_back(layer->listRenderEntries());
	}

	return renderEntries;
}

void Location::printData() {
}

} // End of namespace Stark
