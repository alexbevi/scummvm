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

#include <stdio.h>

#include "common/endian.h"
#include "common/file.h"
#include "common/str.h"
#include "common/textconsole.h"

#include "graphics/palette.h"

#include "kom/kom.h"
#include "kom/character.h"
#include "kom/conv.h"
#include "kom/database.h"
#include "kom/game.h"
#include "kom/input.h"
#include "kom/panel.h"
#include "kom/screen.h"

namespace Kom {

static Common::String lineBuffer;

Conv::Conv(KomEngine *vm, uint16 charId)
	: _vm(vm), _convs(0), _text(0) {
	_charId = charId;
	_codename = _vm->database()->getChar(charId)->_name;
	_convData = _vm->database()->getConvData();
	_convEntry = _vm->database()->getConvIndex(_codename);

	// Backup the palette
	_vm->_system->getPaletteManager()->grabPalette(_backupPalette, 0, 256);

	_vm->screen()->showMouseCursor(false);
}

Conv::~Conv() {
	delete[] _convs;
	delete[] _text;

	// Restore the palette
	_vm->_system->getPaletteManager()->setPalette(_backupPalette, 0, 256);

	_vm->screen()->showMouseCursor(true);
}

void Conv::initConvs(uint32 offset) {
	int16 charId, cmd;
	int32 optNum;

	_convData->seek(offset);

	do {
		lineBuffer = _convData->readLine();
	} while (lineBuffer.empty());
	sscanf(lineBuffer.c_str(), "%hd", &charId);

	// Read conversations
	while (charId != -2) {
		Conversation convObject;
		convObject.charId = charId;

		sscanf(lineBuffer.c_str(), "%*d %hd", &(convObject.convNum));

		// Read options
		do {
			lineBuffer = _convData->readLine();
		} while (lineBuffer.empty());
		sscanf(lineBuffer.c_str(), "%d", &optNum);

		while (optNum != -1) {
			Option optObject;
			optObject.optNum = optNum;

			sscanf(lineBuffer.c_str(), "%*u %u", &(optObject.textOffset));

			do {
				lineBuffer = _convData->readLine();
			} while (lineBuffer.empty());
			sscanf(lineBuffer.c_str(), "%hd", &cmd);

			// Read statements
			while (cmd != -1) {
				Statement statObject;

				statObject.command = cmd;

				switch (cmd) {

				case 306:
					sscanf(lineBuffer.c_str(), " %*d %hd %hd %u %s",
						&(statObject.charId),
						&(statObject.emotion),
						&(statObject.textOffset),
						statObject.filename);
					break;

				case 307:
					break;

				case 309:
					sscanf(lineBuffer.c_str(), " %*d %u",
						&(statObject.optNum));
					break;

				case 326:
					sscanf(lineBuffer.c_str(), " %*d %hu %hd",
						&(statObject.address),
						&(statObject.val));
					break;

				case 419:
					sscanf(lineBuffer.c_str(), " %*d %hd",
						&(statObject.val));
					break;

				case 467:
				case 469:
					lineBuffer = _convData->readLine();
					strcpy(statObject.filename, lineBuffer.c_str());
					break;

				default:
					error("Unknown command %hd", cmd);
					break;
				}

				optObject.statements.push_back(statObject);

				do {
					lineBuffer = _convData->readLine();
				} while (lineBuffer.empty());
				sscanf(lineBuffer.c_str(), "%hd", &cmd);
			}

			convObject.options.push_back(optObject);

			do {
				lineBuffer = _convData->readLine();
			} while (lineBuffer.empty());
			sscanf(lineBuffer.c_str(), "%d", &optNum);
		}

		_conversations.push_back(convObject);

		do {
			lineBuffer = _convData->readLine();
		} while (lineBuffer.empty());
		sscanf(lineBuffer.c_str(), "%hd", &charId);
	}
}

void Conv::initText(uint32 offset) {
	int num;
	_convData->seek(offset);
	lineBuffer = _convData->readLine();
	sscanf(lineBuffer.c_str(), "%d", &num);
	_text = new char[num];
	_convData->read(_text, num);
}

bool Conv::doTalk(int16 convNum, int32 optNum) {

	initConvs(READ_LE_UINT32(_convEntry + 20));
	initText(READ_LE_UINT32(_convEntry + 8));

	// Find conversation
	for (Common::List<Conversation>::iterator c = _conversations.begin(); c != _conversations.end(); c++) {
		if (_charId == c->charId && c->convNum == convNum) {
			talkInit();
			bool result = doOptions(&(*c), optNum);
			talkDeInit();
			return result;
		}
	}

	return false;
}

void Conv::doResponse(int responseNum) {
	int count;
	int num = -1;
	int charId = 0, offset = 0;

	initText(READ_LE_UINT32(_convEntry + 12));

	_convData->seek(READ_LE_UINT32(_convEntry + 16));
	lineBuffer = _convData->readLine();
	sscanf(lineBuffer.c_str(), "%d", &count);

	for (int i = 0; i < count && num != responseNum; i++) {
		do {
			lineBuffer = _convData->readLine();
		} while (lineBuffer.empty());
		sscanf(lineBuffer.c_str(), "%d %d %d", &num, &charId, &offset);
	}

	if (num == responseNum) {
		warning("TODO talk: %hd - %s", charId, _text + offset);
	} else {
		error("Could not find response %d of %s", responseNum, _codename);
	}
}

void Conv::talkInit() {
}

void Conv::talkDeInit() {
}

bool Conv::doOptions(Conversation *conv, int32 optNum) {
	while (1) {
		assert(conv);

		assert(optNum != 0);

		for (int i = 0; i < 3; i++) {
			_options[i].offset = 0;
			_options[i].statements = 0;

			// Find option
			for (Common::List<Option>::iterator o = conv->options.begin(); o != conv->options.end(); o++) {
				if (optNum + i == o->optNum) {
					_options[i].offset = _text + o->textOffset;
					_options[i].statements = &(o->statements);
					break;
				}
			}
		}

		int sel = -1;

		while (sel == -1) {
			sel = showOptions();

			if (sel != -1)
				break;

			if (optNum == 1)
				return false;
		}

		optNum = doStat(sel);

		if (optNum == 0)
			return true;
	}
}

int Conv::showOptions() {
	static const byte active[] = {255, 255, 255};
	static const byte inactive[] = {162, 130, 130};

	int validOptions = 0;

	_vm->screen()->setPaletteColor(1, active);
	_vm->screen()->setPaletteColor(2, inactive);

	// Add a bullet at the beginning
	for (int i = 0; i < 3; i++) {
		if (_options[i].offset == 0) {
			_options[i].text = 0;
		} else {
			if (_options[i].offset) {
				_options[i].text = new char[strlen(_options[i].offset) + 3];
				strcpy(_options[i].text, "\x08 ");
				strcat(_options[i].text, _options[i].offset);
			}

			validOptions++;
		}
	}

	if (validOptions == 1) {
		freeOptions();
		return 0;
	}

	for (int i = 0; i < 3; i++) {

		int col = 4;
		int line = 0;
		char *word;

		if (_options[i].offset == 0) {
			_options[i].text = 0;
			continue;
		}

		word = strtok(_options[i].text, " ");
		while (word) {
			int width = _vm->screen()->getTextWidth(word) + 4 + 1;

			if (col + width > 308) {
				line++;

				if (line >= 24)
					break;
				col = 18;
			}

			_options[i].lines[line] += word;
			_options[i].lines[line] += " ";

			col += width;

			word = strtok(NULL, " ");
		}
	}

	_surfaceHeight = 0;

	// Calculate position of rows
	for (int i = 0; i < 3; i++) {
		if (i == 0)
			_options[i].pixelTop = 0;
		else
			_options[i].pixelTop = _surfaceHeight;

		for (int j = 0; j < 24 && !_options[i].lines[j].empty(); j++)
			_surfaceHeight += 8;
		_surfaceHeight += 3;
		_options[i].pixelBottom = _surfaceHeight;
	}

	_scrollPos = 0;
	_vm->input()->setMousePos(_vm->input()->getMouseX(), 0);

	do {
		_selectedOption = getOption();
		displayMenuOptions();
		_vm->screen()->gfxUpdate();

	} while (!_vm->input()->getLeftClick() || _selectedOption == 9999);

	freeOptions();

	return _selectedOption;
}

void Conv::freeOptions() {
	for (int i = 0; i < 3; i++) {
		delete[] _options[i].text;
		_options[i].text = 0;

		for (int j = 0; j < 24; j++) {
			_options[i].lines[j] = "";
		}
	}
}

int Conv::getOption() {

	if (_scrollPos <= _options[0].pixelBottom)
		return 0;

	for (int i = 1; i < 3; i++) {
		if (_options[i].text == 0)
			continue;

		if (_options[i].pixelTop < _scrollPos && _scrollPos <= _options[i].pixelBottom)
			return i;
	}

	return 9999;
}

void Conv::displayMenuOptions() {
	int row = 12;

	byte *optionsSurface = new byte[SCREEN_W * (_surfaceHeight + PANEL_H)];
	memset(optionsSurface, 0, SCREEN_W * (_surfaceHeight + PANEL_H));

	// Original method of scrolling - requires slow mouse sampling

	//_scrollPos += _vm->input()->getMouseY() - PANEL_H;
	//_scrollPos = CLIP(_scrollPos, 0, _surfaceHeight - PANEL_H);
	//_vm->input()->setMousePos(_vm->input()->getMouseX(), PANEL_H);

	// New scrolling method
	_scrollPos = (_surfaceHeight + 24 - PANEL_H) * _vm->input()->getMouseY() / 200;

	for (int i = 0; i < 3; i++) {

		if (_options[i].text == 0)
			continue;

		if (_selectedOption == i) {

			for (int j = 0; j < 24; j++) {
				if (_options[i].lines[j].empty())
					break;
				_vm->screen()->writeText(optionsSurface, _options[i].lines[j].c_str(), row, j == 0 ? 4 : 18, 1, false);
				row += 8;
			}

		} else {

			for (int j = 0; j < 24; j++) {
				if (_options[i].lines[j].empty())
					break;
				_vm->screen()->writeText(optionsSurface, _options[i].lines[j].c_str(), row, j == 0 ? 4 : 18, 2, false);
				row += 8;
			}
		}

		row += 3;
	}

	_vm->screen()->drawPanel(SCREEN_W * _scrollPos + optionsSurface);

	delete[] optionsSurface;
}

int Conv::doStat(int selection) {

	int ret = 0;

	for (Common::List<Statement>::const_iterator i = _options[selection].statements->begin();
			i != _options[selection].statements->end(); i++) {

		switch (i->command) {

		case 306:
			// i->charId
			// i->emotion
			// i->textOffset
			// i->filename
			warning("TODO talk: %hd - %s", i->charId, _text + i->textOffset);
			break;

		case 307:
			ret = 0;
			break;

		case 309:
			ret = i->optNum;
			break;

		case 326:
			_vm->database()->setVar(i->address, i->val);
			break;

		case 419:
			// i->val
			warning("TODO talk: unset spell");
			break;

		case 467:
			_vm->game()->doActionPlayVideo(i->filename);
			break;

		case 469:
			_vm->game()->doActionPlaySample(i->filename);
			break;

		default:
			warning("Unhandled talk opcode: %hd", i->command);
			break;
		}
	}

	return ret;
}

} // End of namespace Kom
