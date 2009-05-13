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

#include "kom/kom.h"
#include "kom/conv.h"

namespace Kom {

static Common::String line;

Conv::Conv(KomEngine *vm, uint16 charId)
	: _vm(vm), _convs(0), _text(0) {
	_charId = charId;
	_codename = _vm->database()->getChar(charId)->_name;
	_convData = _vm->database()->getConvData();
	byte *entry = _vm->database()->getConvIndex(_codename);

	initConvs(READ_LE_UINT32(entry + 20));
	initText(READ_LE_UINT32(entry + 8));
}

Conv::~Conv() {
	delete[] _convs;
	delete[] _text;
}

void Conv::initConvs(uint32 offset) {
	int16 charId, cmd;
	int32 optNum;

	_convData->seek(offset);

	do {
		line = _convData->readLine();
	} while (line.empty());
	sscanf(line.c_str(), "%hd", &charId);

	// Read conversations
	while (charId != -2) {
		Conversation convObject;
		convObject.charId = charId;

		sscanf(line.c_str(), "%*d %hd", &(convObject.convNum));

		// Read options
		do {
			line = _convData->readLine();
		} while (line.empty());
		sscanf(line.c_str(), "%d", &optNum);

		while (optNum != -1) {
			Option optObject;
			optObject.optNum = optNum;

			sscanf(line.c_str(), "%*u %u", &(optObject.textOffset));

			do {
				line = _convData->readLine();
			} while (line.empty());
			sscanf(line.c_str(), "%hd", &cmd);

			// Read statements
			while (cmd != -1) {
				Statement statObject;

				statObject.command = cmd;

				switch (cmd) {

				case 306:
					sscanf(line.c_str(), " %*d %hd %hd %u %s",
						&(statObject.charId),
						&(statObject.emotion),
						&(statObject.textOffset),
						statObject.filename);
					break;

				case 307:
					break;

				case 309:
					sscanf(line.c_str(), " %*d %u",
						&(statObject.optNum));
					break;

				case 326:
					sscanf(line.c_str(), " %*d %hu %hd",
						&(statObject.address),
						&(statObject.wVal));
					break;

				case 419:
					sscanf(line.c_str(), " %*d %hd",
						&(statObject.wVal));
					break;

				case 467:
				case 469:
					line = _convData->readLine();
					strcpy(statObject.filename, line.c_str());
					break;

				default:
					error("Unknown command %hd", cmd);
					break;
				}

				optObject.statements.push_back(statObject);

				do {
					line = _convData->readLine();
				} while (line.empty());
				sscanf(line.c_str(), "%hd", &cmd);
			}

			convObject.options.push_back(optObject);

			do {
				line = _convData->readLine();
			} while (line.empty());
			sscanf(line.c_str(), "%d", &optNum);
		}

		_conversations.push_back(convObject);

		do {
			line = _convData->readLine();
		} while (line.empty());
		sscanf(line.c_str(), "%hd", &charId);
	}
}

void Conv::initText(uint32 offset) {
	int num;
	_convData->seek(offset);
	line = _convData->readLine();
	sscanf(line.c_str(), "%d", &num);
	_text = new byte[num];
	_convData->read(_text, num);
}

bool Conv::doTalk(int a, int b) {
	return true;
}

} // End of namespace Kom
