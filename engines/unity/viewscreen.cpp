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

#include "viewscreen.h"
#include "graphics.h"

namespace Unity {

ViewscreenScreen::ViewscreenScreen(UnityEngine *vm) : UIScreen(vm) {
}

ViewscreenScreen::~ViewscreenScreen() {
}

void ViewscreenScreen::start() {
	_vm->data._currentScreen.objects.clear();
	_vm->data._currentScreen.polygons.clear();
	_vm->data._currentScreen.world = 0x5f;
	_vm->data._currentScreen.screen = 0xff;

	Object *obj;

	// TODO: icon at 33, 444

	// viewscreen image
	obj = new Object(_vm);
	obj->x = 320;
	obj->y = 440;
	obj->y_adjust = 1000;
	obj->flags = OBJFLAG_ACTIVE;
	obj->objwalktype = OBJWALKTYPE_NORMAL;
	Common::String sprFilename = _vm->data.getSpriteFilename(_vm->_viewscreen_sprite_id);
	obj->sprite = new SpritePlayer(_vm->data.openFile(sprFilename), obj, _vm);
	obj->sprite->startAnim(0);
	_vm->data._currentScreen.objects.push_back(obj);

	// viewscreen animation
	obj = new Object(_vm);
	obj->x = 320;
	obj->y = 440;
	obj->y_adjust = -1;
	obj->flags = OBJFLAG_ACTIVE;
	obj->objwalktype = OBJWALKTYPE_NORMAL;
	obj->sprite = new SpritePlayer(_vm->data.openFile("viewscan.spr"), obj, _vm);
	obj->sprite->startAnim(0);
	_vm->data._currentScreen.objects.push_back(obj);

	_vm->_gfx->setBackgroundImage("viewscr.rm");
	mouseMove(Common::Point(0, 0));
}

void ViewscreenScreen::mouseMove(const Common::Point &pos) {
	_vm->_gfx->setCursor(0xffffffff, false);
}

void ViewscreenScreen::mouseClick(const Common::Point &pos) {
}

void ViewscreenScreen::draw() {
}

} // Unity

