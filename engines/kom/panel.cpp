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

Panel::Panel(KomEngine *vm, const char *filename) : _vm(vm),
	_isEnabled(true), _isLoading(false), _noLoading(0),
	_locationDesc(0), _actionDesc(0), _hotspotDesc(0), _gotObjTime(0) {
	File f;
	f.open(filename);

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
	delete[] _actionDesc;
	delete[] _hotspotDesc;
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
	//         4 pixels to the right.
	// Actually, the original prints all text 4 pixels to the right, so we're using
	// 6 as the column instead of 10 for the two bottom lines.
	if (_locationDesc)
		_vm->screen()->writeTextCentered(_panelBuf, _locationDesc, 3, 31, true);

	if (_actionDesc)
		_vm->screen()->writeText(_panelBuf, _actionDesc, 12, 6, 31, true);

	if (_hotspotDesc)
		_vm->screen()->writeText(_panelBuf, _hotspotDesc, 22, 6, 31, true);

	// FIXME: check loading in the middle of object animation
	if (_isLoading)
		return;

	_vm->screen()->drawPanel(_panelBuf);

	// Draw got and lost objects

	if (_gotObjTime == 0) {
		if (!_gotObjects.empty())
			_gotObjTime = 24;

	} else {
		// Don't animate if narrator is talking
		if (_vm->game()->isNarratorPlaying())
			_gotObjTime = 24;

		Actor *objects = _vm->actorMan()->getObjects();
		uint16 xRatio, yRatio;
		uint8 frame;
		int currObj = *_gotObjects.begin();

		_gotObjTime--;

		// Got object
		if (currObj > 0) {

			// Regular object
			if (currObj < 1000) {
				frame = currObj + 1;
				if (_gotObjTime == 16)
					_vm->sound()->playSampleSFX(_vm->_swipeSample, false);

			// Money
			} else {
				currObj -= 1000;
				frame = currObj;
				if (_gotObjTime == 16)
					_vm->sound()->playSampleSFX(_vm->_cashSample, false);
			}

			objects->enable(1);

			// Zoom into screen
			if (_gotObjTime > 16) {
				xRatio = yRatio = 0x600 - 0x600 / (24 - _gotObjTime);
				objects->setEffect(0);
				objects->setPos(303, 185);
				objects->setRatio(xRatio, yRatio);
				objects->setFrame(frame);

			// Stay on screen
			} else if (_gotObjTime > 8) {
				objects->setEffect(4);
				objects->setPos(303, 185);
				objects->setRatio(1024, 1024);
				objects->setFrame(frame);

			// Move below screen
			} else {
				objects->setEffect(4);
				objects->setPos(303, 185 + (8 - _gotObjTime)*(8 - _gotObjTime));
				objects->setRatio(1024, 1024);
				objects->setFrame(frame);
			}

		// Lost object
		} else {

			currObj = -currObj;

			if (_gotObjTime == 12)
				_vm->sound()->playSampleSFX(_vm->_loseItemSample, false);

			// Regular object
			if (currObj < 1000) {
				frame = currObj + 1;
				if (_gotObjTime == 12)
					_vm->sound()->playSampleSFX(_vm->_swipeSample, false);

			// Money
			} else {
				currObj -= 1000;
				frame = currObj;
			}

			objects->enable(1);

			// Move from below screen
			if (_gotObjTime > 16) {
				objects->setEffect(4);
				objects->setPos(303, 185 + (_gotObjTime - 16)*(_gotObjTime - 16));
				objects->setRatio(1024, 1024);
				objects->setFrame(frame);

			// Stay on screen
			} else if (_gotObjTime > 8) {
				objects->setEffect(4);
				objects->setPos(303, 185);
				objects->setRatio(1024, 1024);
				objects->setFrame(frame);

			// Zoom out of screen
			} else {
				xRatio = yRatio = 0x2710 / (24 - _gotObjTime);
				objects->setEffect(0);
				objects->setPos(303, 185);
				objects->setRatio(xRatio, yRatio);
				objects->setFrame(frame);
			}
		}

		// Display object
		objects->display();
		objects->enable(0);
		if (_gotObjTime == 0)
			_gotObjects.pop_front();
	}
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
