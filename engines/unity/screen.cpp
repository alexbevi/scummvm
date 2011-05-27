/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "screen.h"

namespace Unity {

UIScreen::UIScreen(UnityEngine *vm) : _vm(vm) {
}

UIScreen::~UIScreen() {
	for (uint i = 0; i < _controls.size(); i++) {
		delete _controls[i];
	}
}

UIControl *UIScreen::getControlAt(const Common::Point &pos) {
	for (uint i = 0; i < _controls.size(); i++) {
		if (_controls[i]->_bounds.contains(pos))
			return _controls[i];
	}

	return NULL;
}

} // Unity

