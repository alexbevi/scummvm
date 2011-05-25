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

#include "bridge.h"
#include "graphics.h"

namespace Unity {

BridgeScreen::BridgeScreen(UnityEngine *vm) : UIScreen(vm) {
}

BridgeScreen::~BridgeScreen() {
}

void BridgeScreen::start() {
	_vm->data._currentScreen.objects.clear();
	_vm->data._currentScreen.polygons.clear();
	_vm->data._currentScreen.world = 0x5f;
	_vm->data._currentScreen.screen = 0xff;

	// TODO: episode title should only appear once
	const char *bridge_sprites[5] = {
		"brdgldor.spr", // Left Door (conference room)
		"brdgdoor.spr", // Door
		"brdgtitl.spr", // Episode Title
		"advice.spr", // advice button in UI :(
		0 };

	for (unsigned int i = 0; bridge_sprites[i] != 0; i++) {
		Object *obj = new Object(_vm);
		obj->x = obj->y = 0;
		obj->y_adjust = -1;
		obj->flags = OBJFLAG_ACTIVE;
		obj->objwalktype = OBJWALKTYPE_NORMAL;
		obj->sprite = new SpritePlayer(new Sprite(_vm->data.openFile(bridge_sprites[i])), obj, _vm);
		obj->sprite->startAnim(0);
		_vm->data._currentScreen.objects.push_back(obj);
	}

	for (unsigned int i = 0; i < _vm->data._bridgeObjects.size(); i++) {
		Object *obj = new Object(_vm);
		// TODO: correct?
		obj->x = _vm->data._bridgeObjects[i].x;
		obj->y = _vm->data._bridgeObjects[i].y;
		obj->y_adjust = -1;
		obj->flags = OBJFLAG_ACTIVE;
		obj->objwalktype = OBJWALKTYPE_NORMAL;
		obj->sprite = new SpritePlayer(new Sprite(_vm->data.openFile(_vm->data._bridgeObjects[i].filename)), obj, _vm);
		obj->sprite->startAnim(0);
		/*debugN("%s: %d, %d\n", data._bridgeObjects[i].filename.c_str(),
			data._bridgeObjects[i].unknown1,
			data._bridgeObjects[i].unknown2);*/
		_vm->data._currentScreen.objects.push_back(obj);
	}

	_vm->_gfx->setBackgroundImage("bridge.rm");
	_vm->_on_away_team = false;
	mouseMove(Common::Point(0, 0));
}

void BridgeScreen::mouseMove(const Common::Point &pos) {
	_status_text.clear();

	for (unsigned int i = 0; i < _vm->data._bridgeItems.size(); i++) {
		BridgeItem &item = _vm->data._bridgeItems[i];
		if ((uint)pos.x < item.x) continue;
		if ((uint)pos.y < item.y) continue;
		if ((uint)pos.x > item.x + item.width) continue;
		if ((uint)pos.y > item.y + item.height) continue;

		_status_text = item.description;

		// TODO: this is kinda random :)
		if (item.unknown1 < 0x32)
			_vm->_gfx->setCursor(3, false);
		else
			_vm->_gfx->setCursor(5, false);

		return;
	}

	_vm->_gfx->setCursor(0xffffffff, false);
}

void BridgeScreen::mouseClick(const Common::Point &pos) {
	for (unsigned int i = 0; i < _vm->data._bridgeItems.size(); i++) {
		BridgeItem &item = _vm->data._bridgeItems[i];
		if ((uint)pos.x < item.x) continue;
		if ((uint)pos.y < item.y) continue;
		if ((uint)pos.x > item.x + item.width) continue;
		if ((uint)pos.y > item.y + item.height) continue;

		debug(1, "user clicked bridge item %d", i);
		if (item.id.world != 0) {
			// bridge crew member
			Object *obj = _vm->data.getObject(item.id);
			obj->runHail(obj->talk_string);
		} else {
			switch (i) {
				case 0: // conference lounge
					// TODO
					break;
				case 1: // turbolift (menu)
					_vm->_dialog_text.clear();
					assert(!_vm->_choice_list.size());
					_vm->_choice_list.clear();
					assert(!_vm->_dialog_choosing);

					for (unsigned int j = 0; j < _vm->data._bridgeScreenEntries.size(); j++) {
						_vm->_choice_list.push_back(_vm->data._bridgeScreenEntries[j].text);
					}
					_vm->runDialog();

					// TODO: do this properly
					if (_vm->_beam_world != 0) {
						_vm->startAwayTeam(_vm->_beam_world, _vm->_beam_screen);
					}

					_vm->_choice_list.clear();
					break;
				case 2: // comms
					// TODO
					break;
				case 3: // tactical
					// TODO
					break;
				case 4: // astrogation
					// TODO
					break;
				case 5: // computer
					// TODO
					break;
				case 10: // replay conversation
					// TODO
					break;
				default:
					error("unknown bridge item");
			}
		}

		return;
	}
}

void BridgeScreen::draw() {
	// the advice button is drawn as a sprite (see startBridge), sigh
	// TODO: draw icons there if there's no subtitles

	// sensor.mrg
	// 4 sprites: "bridge" in/out and "viewscreen" in/out

	// draw viewscreen button
	MRGFile mrg;
	_vm->_gfx->loadMRG("sensor.mrg", &mrg);
	_vm->_gfx->drawMRG(&mrg, 3, 484, 426);

	// transp.mrg
	// 0-5: up arrows (normal, hilight, grayed) + down arrows
	// 5-11: seek arrows (normal, hilight, grayed): 3 left then 3 right
	// 11-17: 6 transporter room stuff?
	// 18-19: normal admin menu, hilighted admin menu
	// 20: some grey thing

	// draw grayed up/down arrows
	MRGFile tmrg;
	_vm->_gfx->loadMRG("transp.mrg", &tmrg);
	_vm->_gfx->drawMRG(&tmrg, 2, 117, 426);
	_vm->_gfx->drawMRG(&tmrg, 5, 117, 450);

	// display text (TODO: list of visited sectors)
	char buffer[30];

	// TODO: sane max width/heights

	// TODO: the location doesn't actually seem to come from this object
	Object *ship = _vm->data.getObject(objectID(0x00, 0x01, 0x5f));

	// TODO: updates while warping, etc
	Common::String sector_name = _vm->data.getSectorName(ship->universe_x, ship->universe_y, ship->universe_z);
	snprintf(buffer, 30, "SECTOR: %s", sector_name.c_str());
	_vm->_gfx->drawString(9, 395, 9999, 9999, buffer, 2);

	unsigned int warp_hi = 0, warp_lo = 0; // TODO
	snprintf(buffer, 30, "WARP: %d.%d", warp_hi, warp_lo);
	_vm->_gfx->drawString(168, 395, 9999, 9999, buffer, 2);

	snprintf(buffer, 30, "%s", _status_text.c_str());
	Common::Array<unsigned int> strwidths, starts; unsigned int height;
	_vm->_gfx->calculateStringBoundary(110, strwidths, starts, height, buffer, 2);
	// draw centered, with 544 being the centre
	_vm->_gfx->drawString(544 - strwidths[0]/2, 393, strwidths[0], 9999, buffer, 2);
}

} // Unity

