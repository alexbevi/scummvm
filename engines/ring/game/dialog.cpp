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

#include "ring/game/dialog.h"

#include "ring/helpers.h"

namespace Ring {

//////////////////////////////////////////////////////////////////////////
// Dialog
//////////////////////////////////////////////////////////////////////////
Dialog::Dialog() : BaseObject(kDialogInvalid) {
}

Dialog::~Dialog() {
}

void Dialog::setTicks() {
	_ticks = g_system->getMillis();
}

void Dialog::hide() {
	error("[Dialog::hide] No implemented");
}

//////////////////////////////////////////////////////////////////////////
// DialogHandler
//////////////////////////////////////////////////////////////////////////
DialogHandler::DialogHandler() {
	_field_0 = 0;
	_field_1C = 0;
	_field_20 = 0;
	_field_28 = 1;
}

DialogHandler::~DialogHandler() {
	CLEAR_ARRAY(Dialog, _dialogs);
}

void DialogHandler::init(uint32 a1, Color subtitlesColor, Color subtitlesBackgroundColor, uint32 a8, uint32 a9) {
	_field_0 = a1;
	_subtitleColor = subtitlesColor;
	_subtitlesBackgroundColor = subtitlesBackgroundColor;
	_field_1C = a8;
	_field_20 = a9;
}

void DialogHandler::addDialog(Dialog *dialog) {
	if (!dialog)
		error("[DialogHandler::addDialog] dialog is NULL");

	dialog->setTicks();

	_dialogs.push_back(dialog);
}

void DialogHandler::removeDialog(DialogId id) {
	for (uint32 i = 0; i < _dialogs.size();) {
		if (_dialogs[i]->getId() == id) {
			_dialogs[i]->hide();
			SAFE_DELETE(_dialogs[i]);
			_dialogs.remove_at(i);
		} else {
			++i;
		}
	}
}

bool DialogHandler::isPlaying(DialogId id) {
	return _dialogs.has(id);
}

bool DialogHandler::isPlaying() {
	return !_dialogs.empty();
}

} // End of namespace Ring
