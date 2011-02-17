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

#pragma region Dialog

Dialog::Dialog(ObjectId id, Common::String name) : BaseObject(id) {
	_startTicks = 0;
	_field_D = 0;

	if (name.size() < 4)
		error("[Dialog::Dialog] Invalid dialog name (length should be >= 4)");

	// Read lyrics
	readLyrics(Common::String::format("DATA/%s/DIA/%s/%sdia", getApp()->getCurrentZoneString().c_str(), getApp()->languageGetFolder().c_str(), name.c_str()));

	// Read animations
	_visible = readAnimation(Common::String::format("DATA/%s/DIA/%s/%sdan", getApp()->getCurrentZoneString().c_str(), getApp()->languageGetFolder().c_str(), name.c_str()));
}

Dialog::~Dialog() {
	CLEAR_ARRAY(DialogLine, _lines);
	CLEAR_ARRAY(DialogAnimation, _levels);
	CLEAR_ARRAY(DialogAnimation, _animations);
}

void Dialog::setTicks() {
	_startTicks = g_system->getMillis();
}

void Dialog::hide() {
	if (!_visible)
		return;

	for (Common::Array<DialogAnimation *>::iterator it = _levels.begin(); it != _levels.end(); it++)
		getApp()->objectPresentationHide((*it)->objectId, (*it)->presentationIndex);
}

void Dialog::show() {
	error("[Dialog::show] No implemented");
}

int32 Dialog::getLineIndex() {
	if (_lines.empty())
		return -1;

	if (_lines.size() == 1)
		return 0;

	// Tick offset
	uint32 ticks = g_system->getMillis() - _startTicks;

	// Get the proper line
	for (uint32 i = 0; i < _lines.size() - 1; i++) {
		if (ticks >= _lines[i]->ticks && ticks <= _lines[i + 1]->ticks)
			return i;
	}

	return -1;
}

Common::String Dialog::getLine1(uint32 index) {
	if (index >= _lines.size())
		error("[Dialog::getLine1] Invalid index (was:%d, max:%d)", index, _lines.size() - 1);

	return _lines[index]->line1;
}

Common::String Dialog::getLine2(uint32 index) {
	if (index >= _lines.size())
		error("[Dialog::getLine2] Invalid index (was:%d, max:%d)", index, _lines.size() - 1);

	return _lines[index]->line2;
}

void Dialog::readLyrics(Common::String filename) {
	error("[Dialog::readLyrics] No implemented");
}

void Dialog::parseLyrics(Common::String line) {
	error("[Dialog::parseLyrics] No implemented");
}

bool Dialog::readAnimation(Common::String filename) {
	error("[Dialog::readDialogAnimation] No implemented");
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

		if (dialog->isVisible())
			dialog->show();

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
