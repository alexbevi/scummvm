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
 */

#include "ring/base/dialog.h"

#include "ring/base/application.h"
#include "ring/base/event.h"
#include "ring/base/text.h"

#include "ring/graphics/screen.h"

#include "ring/ring.h"
#include "ring/helpers.h"

#include "common/file.h"
#include "common/tokenizer.h"

namespace Ring {

#pragma region Dialog

Dialog::Dialog(Id id, Common::String name) : BaseObject(id) {
	_startTicks = 0;
	_field_D = 0;

	if (name.size() < 4)
		error("[Dialog::Dialog] Invalid dialog name (length should be >= 4)");

	// Read lyrics
	readLyrics(Common::String::format("DATA/%s/DIA/%s/%s.dia", getApp()->getCurrentZoneFolder().c_str(), getApp()->getLanguageFolder().c_str(), name.c_str()));

	// Read animations
	_visible = readAnimation(Common::String::format("DATA/%s/DIA/%s/%s.dan", getApp()->getCurrentZoneFolder().c_str(), getApp()->getLanguageFolder().c_str(), name.c_str()));
}

Dialog::~Dialog() {
	CLEAR_ARRAY(DialogLine, _lines);
	CLEAR_ARRAY(DialogObject, _objects);
	CLEAR_ARRAY(DialogAnimation, _animations);
}

void Dialog::setTicks() {
	_startTicks = g_system->getMillis();
}

void Dialog::hide() {
	if (!_visible)
		return;

	for (Common::Array<DialogObject *>::iterator it = _objects.begin(); it != _objects.end(); it++)
		getApp()->objectPresentationHide((*it)->objectId, (*it)->presentationIndex);
}

void Dialog::show() {
	// Tick offset
	uint32 ticks = g_system->getMillis() - _startTicks;

	// Check animation to show
	int32 index = -1;
	for (uint32 i = 0; i < _animations.size(); i++) {
		if (ticks >= _animations[i]->tickStart && ticks <= _animations[i]->tickEnd) {
			index = i;
			break;
		}
	}

	uint32 val = (index != -1) ? _animations[index]->field_8 : 0;
	if (val == _field_D)
		return;

	// Hide all objects
	for (Common::Array<DialogObject *>::iterator it = _objects.begin(); it != _objects.end(); it++)
		getApp()->objectPresentationHide((*it)->objectId, (*it)->presentationIndex);

	if (val) {
		// Get the object to show
		int32 objectIndex = -1;
		for (uint32 i = 0; i < _objects.size(); i++) {
			if (_objects[i]->field_8 == val) {
				objectIndex = i;
				break;
			}
		}

		// Not found
		if (objectIndex == -1) {
			_field_D = val;
			return;
		}

		getApp()->objectPresentationShow(_objects[objectIndex]->objectId, _objects[objectIndex]->presentationIndex);
	}

	_field_D = val;
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
	// Open a stream to the dialog file
	Common::SeekableReadStream *archive = SearchMan.createReadStreamForMember(filename);
	if (!archive)
		error("[Dialog::readLyrics] Error opening dialog file (%s)", filename.c_str());

	while (!archive->eos() && !archive->err()) {
		Common::String line = archive->readLine();
		if (archive->eos() || archive->err())
			break;

		parseLyrics(line);
	}

	delete archive;
}

void Dialog::parseLyrics(Common::String line) {
	// Trim whitespace
	line.trim();

	// Parse line
	//  - ticks
	//  - message (line1#line2)
	Common::StringTokenizer tokenizer(line, " ");

	Common::String ticks = tokenizer.nextToken();
	if (tokenizer.empty())
		error("[Dialog::parseLyrics] Invalid line format (%s)", line.c_str());

	// First, try parsing ticks
	uint32 tick = 0;
	if (!ticks.contains(",")) {
		// Normal tick value
		if (sscanf(ticks.c_str(), "%d", (int *)&tick) == 0)
			error("[Dialog::parseLyrics] Cannot parse tick value (%s) for line %s", ticks.c_str(), line.c_str());
	} else {
		// Normal tick value, followed by a tick interval (separated by ',')
		int minutes = 0;
		int seconds = 0;
		int centiseconds = 0;
		if (sscanf(ticks.c_str(), "%d,%d:%d.%d", (int *)&tick, &minutes, &seconds, &centiseconds) == 0)
			error("[Dialog::parseLyrics] Cannot parse tick value (%s) for line %s", ticks.c_str(), line.c_str());

		// The values other than the ticks are ignored in the original (Ring)
	}

	// Parse dialog lines
	Common::String dialogs = line.begin() + ticks.size() + 1;
	dialogs.trim();

	Common::StringTokenizer lines(dialogs, "#");
	if (lines.empty())
		error("[Dialog::parseLyrics] Invalid dialog line format (%s)", dialogs.c_str());

	// Store line
	DialogLine *dialogLine = new DialogLine();
	dialogLine->ticks = tick;
	dialogLine->line1 = lines.nextToken();
	dialogLine->line2 = lines.nextToken();

	// Special case for Greek language (ascii code 160: Greek lower case iota with dialytika in codepage 869)
	for (Common::String::iterator c = dialogLine->line1.begin(); c != dialogLine->line1.end(); ++c)
		if (*c == 'á')
			*c = ' ';

	for (Common::String::iterator c = dialogLine->line2.begin(); c != dialogLine->line2.end(); ++c)
		if (*c == 'á')
			*c = ' ';

	_lines.push_back(dialogLine);
}

bool Dialog::readAnimation(Common::String filename) {
	if (!Common::File::exists(filename))
		return false;

	// Open a stream to the dialog file
	Common::SeekableReadStream *archive = SearchMan.createReadStreamForMember(filename);
	if (!archive)
		error("[Dialog::readAnimation] Error opening dialog animation file (%s)", filename.c_str());

	// Parse object levels
	Common::String levelLine = archive->readLine();
	if (archive->eos() || archive->err())
		error("[Dialog::readAnimation] Error reading object levels (EOS in %s)", filename.c_str());

	int levels = 0;
	if (sscanf(levelLine.c_str(), "%d", (int *)&levels) == 0)
		error("[Dialog::readAnimation] Error reading object levels (Wrong format in %s)", filename.c_str());

	// Read objects
	if (levels > 0) {
		for (int i = 0; i < levels; i++) {
			Common::String line = archive->readLine();
			if (archive->eos() || archive->err())
				error("[Dialog::readAnimation] Error reading object animations (EOS in %s)", filename.c_str());

			// Parse object line
			uint32 objectId = 0;
			uint32 presentationIndex = 0;
			uint32 a3 = 0;
			if (sscanf(line.c_str(), "%d %d %d", (int *)&objectId, (int *)&presentationIndex, (int *)&a3) != 3)
				error("[Dialog::readAnimation] Error reading object levels (Wrong format in %s)", filename.c_str());

			_objects.push_back(new DialogObject(objectId, presentationIndex, a3));
		}
	}

	// Read animations
	while (!archive->eos() && !archive->err()) {
		Common::String line = archive->readLine();
		if (archive->eos() || archive->err())
			break;

		// Parse animation line
		uint32 start = 0;
		uint32 end = 0;
		uint32 a3 = 0;
		if (sscanf(line.c_str(), "%d %d %d", (int *)&start, (int *)&end, (int *)&a3) != 3)
			break;

		_animations.push_back(new DialogAnimation(start, end, a3));
	}

	delete archive;

	return true;
}

#pragma endregion

#pragma region DialogHandler

DialogHandler::DialogHandler() {
	_fontId = kFontInvalid;
	_field_1C = 0;
	_field_20 = 0;
	_showSubtitles = true;
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

	_dialogs.remove(id);

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
		Common::String dialogLine1 = dialog->getLine1((uint32)index);
		if (!dialogLine1.empty()) {
			line1 = new Text();
			line1->init(dialogLine1, Common::Point(0, 0), _fontId, _subtitleColor, _subtitlesBackgroundColor);
		}

		Common::String dialogLine2 = dialog->getLine2((uint32)index);
		if (!dialogLine2.empty()) {
			line2 = new Text();
			line2->init(dialogLine2, Common::Point(0, 0), _fontId, _subtitleColor, _subtitlesBackgroundColor);
		}

		// Set text coordinates
		if (line1) {
			if (line2) {
				line1->setCoordinates(Common::Point((int16)(320 - line1->getWidth() / 2), (int16)(_field_1C - (line1->getHeight() + _field_20 + line2->getHeight()))));
				line2->setCoordinates(Common::Point((int16)(320 - line2->getWidth() / 2), (int16)(_field_1C - line2->getHeight())));
			} else {
				line1->setCoordinates(Common::Point((int16)(320 - line1->getWidth() / 2), (int16)(_field_1C - line1->getHeight())));
			}
		}

		if (_showSubtitles) {
			if (line1)
				getApp()->getScreenManager()->drawText(line1);

			if (line2)
				getApp()->getScreenManager()->drawText(line2);
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
