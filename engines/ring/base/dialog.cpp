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

#include "ring/base/dialog.h"

#include "ring/base/application.h"
#include "ring/base/text.h"

#include "ring/ring.h"
#include "ring/helpers.h"

namespace Ring {

#pragma region DialogLine

DialogLine::DialogLine() {
}

DialogLine::~DialogLine() {
}

#pragma endregion

#pragma region DialogLevel

DialogLevel::DialogLevel() {
}

DialogLevel::~DialogLevel() {
}

#pragma endregion

#pragma region DialogAnimation

DialogAnimation::DialogAnimation() {
}

DialogAnimation::~DialogAnimation() {
}

#pragma endregion

#pragma region Dialog

Dialog::Dialog() : BaseObject(0) {
	_startTicks = 0;
	_field_C = false;
	_field_D = 0;
}

Dialog::~Dialog() {
	CLEAR_ARRAY(DialogLine, _lines);
	CLEAR_ARRAY(DialogLevel, _levels);
	CLEAR_ARRAY(DialogAnimation, _animations);
}

void Dialog::setTicks() {
	_startTicks = g_system->getMillis();
}

void Dialog::hide() {
	error("[Dialog::hide] No implemented");
}

void Dialog::sub_427A10() {
	error("[Dialog::sub_427A10] No implemented");
}

int32 Dialog::getLineIndex() {
	error("[Dialog::getLineIndex] No implemented");
}

Common::String Dialog::getLine1(int32 index) {
	error("[Dialog::getLine1] No implemented");
}

Common::String Dialog::getLine2(int32 index) {
	error("[Dialog::getLine2] No implemented");
}

#pragma endregion

#pragma region DialogHandler

DialogHandler::DialogHandler() {
	_fontId = kFontInvalid;
	_field_1C = 0;
	_field_20 = 0;
	_field_28 = 1;
}

DialogHandler::~DialogHandler() {
	CLEAR_ARRAY(Dialog, _dialogs);
}

void DialogHandler::init(FontId fontId, const Color &subtitlesColor, const Color &subtitlesBackgroundColor, uint32 a8, uint32 a9) {
	_fontId = fontId;
	_subtitleColor = subtitlesColor;
	_subtitlesBackgroundColor = subtitlesBackgroundColor;
	_field_1C = a8;
	_field_20 = a9;
}

void DialogHandler::addDialog(Dialog *dialog) {
	if (!dialog)
		error("[DialogHandler::addDialog] Dialog is not initialized properly");

	dialog->setTicks();

	_dialogs.push_back(dialog);
}

bool DialogHandler::removeDialog(Id id) {
	if (!_dialogs.has(id))
		return false;

	// Hide dialog
	_dialogs.get(id)->hide();

	_dialogs.remove_at(_dialogs.getIndex(id));

	return true;
}

void DialogHandler::play() {
	if (_dialogs.empty())
		return;

	// Play first dialog
	Dialog *dialog = _dialogs[0];

	int32 index = dialog->getLineIndex();
	if (index != -1) {
		Text *line1 = NULL;
		Text *line2 = NULL;

		// Create text from dialog lines
		if (!dialog->getLine1(index).empty()) {
			line1 = new Text();
			line1->init(dialog->getLine1(index), Common::Point(0, 0), _fontId, _subtitleColor, _subtitlesBackgroundColor);
		}

		if (!dialog->getLine2(index).empty()) {
			line2 = new Text();
			line2->init(dialog->getLine2(index), Common::Point(0, 0), _fontId, _subtitleColor, _subtitlesBackgroundColor);
		}

		// Set text coordinates
		if (line1) {
			if (line2) {
				line1->setCoordinates(Common::Point(320 - line1->getWidth() / 2, _field_1C - line1->getHeight() - _field_20 - line2->getHeight()));
				line2->setCoordinates(Common::Point(320 - line2->getWidth() / 2, _field_1C - line2->getHeight()));
			} else {
				line1->setCoordinates(Common::Point(320 - line1->getWidth() / 2, _field_1C - line1->getHeight()));
			}
		}

		if (_field_28) {
			if (line1)
				line1->draw();

			if (line2)
				line2->draw();
		}

		SAFE_DELETE(line1);
		SAFE_DELETE(line2);

		if (dialog->getFieldC())
			dialog->sub_427A10();

		error("[DialogHandler::play] Not implemented");
	}

	Id dialogId = dialog->getId();
	removeDialog(dialogId);
	getApp()->onSound(dialogId, kSoundTypeDialog, 4097);
}

bool DialogHandler::isPlaying(Id id) {
	return _dialogs.has(id);
}

bool DialogHandler::isPlaying() const {
	return !_dialogs.empty();
}

#pragma endregion

} // End of namespace Ring
