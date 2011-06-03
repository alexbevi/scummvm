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

#include "computer.h"

#include "graphics.h"
#include "sound.h"

namespace Unity {

ComputerScreen::ComputerScreen(UnityEngine *vm) : UIScreen(vm) {
}

ComputerScreen::~ComputerScreen() {
	for (uint i = 0; i < _controls.size(); i++)
		delete _controls[i];
}

enum {
	ListUpButton,
	ListDownButton,
	TextUpButton,
	TextDownButton,
	BridgeButton,
	AstrogationButton,
	TacticalButton
};

void ComputerScreen::start() {
	_vm->clearObjects();
	_vm->data._currentScreen.polygons.clear();
	_vm->data._currentScreen.world = 0x5f;
	_vm->data._currentScreen.screen = 0xff;

	_sectionStack.clear();
	_sectionStack.push_back(0);
	_selection = 0;

	for (uint i = 0; i < _controls.size(); i++)
		delete _controls[i];
	_controls.clear();

	UIControl *control;

	/*
	 * 2/3: normal/highlight up button
	 * 4/5: normal/highlight down button
	 * 12/13: disabled (empty) up/down buttons
	 */
	// list/text up/down buttons
	control = new UIControl;
	control->_id = ListUpButton;
	control->_sprite = 12;
	control->_bounds = Common::Rect(61, 412, 118, 440);
	_controls.push_back(control);
	control = new UIControl;
	control->_id = ListDownButton;
	control->_sprite = 13;
	control->_bounds = Common::Rect(61, 440, 118, 468);
	_controls.push_back(control);
	control = new UIControl;
	control->_id = TextUpButton;
	control->_sprite = 12;
	control->_bounds = Common::Rect(217, 412, 274, 440);
	_controls.push_back(control);
	control = new UIControl;
	control->_id = TextDownButton;
	control->_sprite = 13;
	control->_bounds = Common::Rect(217, 440, 274, 468);
	_controls.push_back(control);

	// bridge, astrogation and tactical buttons
	control = new UIControl;
	control->_id = BridgeButton;
	control->_sprite = 6; // up (down is 7)
	control->_bounds = Common::Rect(490, 370, 602, 400);
	_controls.push_back(control);
	control = new UIControl;
	control->_id = AstrogationButton;
	control->_sprite = 8; // up (down is 9)
	control->_bounds = Common::Rect(490, 402, 602, 432);
	_controls.push_back(control);
	control = new UIControl;
	control->_id = TacticalButton;
	control->_sprite = 10; // up (down is 11)
	control->_bounds = Common::Rect(490, 434, 602, 464);
	_controls.push_back(control);

	_vm->_gfx->setBackgroundImage("compupnl.ast");
	mouseMove(Common::Point(0, 0));

	_vm->_snd->playMusic("compute.rac", 0x3f, 0x19740);
}

void ComputerScreen::shutdown() {
}

void ComputerScreen::mouseMove(const Common::Point &pos) {
	// No special cursor handling here.
	_vm->_gfx->setCursor(0xffffffff, false);
}

void ComputerScreen::mouseClick(const Common::Point &pos) {
	if (pos.x >= 18 && pos.x <= 158) {
		if (pos.y >= 56 && pos.y <= 77 + 23*15) {
			// section
			uint entryId = (pos.y - 56) / 23;
			debug(5, "ComputerScreen::mouseClick() entryId: %d", entryId);

			// TODO
			return;
		}
	}

	UIControl *control = getControlAt(pos);
	if (!control)
		return;

	// play sound
	switch (control->_id) {
	case BridgeButton:
	case AstrogationButton:
	case TacticalButton:
		// new screen, fancy beep
		_vm->_snd->playSfx("beep7.mac");
		break;
	default:
		// up/down buttons, short beep
		_vm->_snd->playSfx("level2.mac");
		break;
	}

	// perform action
	switch (control->_id) {
	case BridgeButton:
		control->_state = 1;
		_vm->changeToScreen(BridgeScreenType);
		return;
	case AstrogationButton:
		control->_state = 1;
		_vm->changeToScreen(AstrogationScreenType);
		return;
	case TacticalButton:
		control->_state = 1;
		_vm->changeToScreen(TacticalScreenType);
		return;
	default:
		error("unhandled control id %d in ComputerScreen", control->_id);
	}
}

void ComputerScreen::draw() {
	MRGFile mrg;
	_vm->_gfx->loadMRG("compute1.pic", &mrg);

	// draw title list
	for (uint i = 0; i < 15; i++) {
		uint toDraw = i;
		uint entryId;
		Common::String text;
		byte color = 3;
		uint indent = 0;
		if (toDraw < _sectionStack.size()) {
			// parents
			entryId = _sectionStack[toDraw];
			color = 2;
			indent = toDraw;
		} else {
			// children
			uint subentry = toDraw - _sectionStack.size();
			uint parentId = _sectionStack[_sectionStack.size() - 1];
			const Common::Array<uint> &subentries = _vm->data._computerEntries[parentId].subentries;
			if (subentry >= subentries.size())
				continue;
			entryId = subentries[subentry];
			indent = _sectionStack.size();
		}
		text = _vm->data._computerEntries[entryId].title;
		text.toUppercase();

		// 0/1: normal/full section background
		_vm->_gfx->drawMRG(&mrg, (toDraw == _selection) ? 1 : 0, 1, 47 + (i * 23));

		Common::Rect rect(15 + ((indent + 1) * 10), 61 + (i * 23), 167, 76 + (i * 23));
		_vm->_gfx->drawString(rect.left, rect.top, /*rect.right, rect.bottom,*/ text, color);
	}

	// draw UI controls
	for (uint i = 0; i < _controls.size(); i++) {
		UIControl *control = _controls[i];
		if (control->_sprite == 0xffffffff)
			continue;

		_vm->_gfx->drawMRG(&mrg, control->_sprite + control->_state,
				control->_bounds.left, control->_bounds.top);
	}
}

} // Unity

