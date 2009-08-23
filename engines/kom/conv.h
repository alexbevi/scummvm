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

#ifndef KOM_CONV_H
#define KOM_CONV_H

#include "common/file.h"
#include "common/list.h"
#include "common/str.h"

namespace Kom {

struct Statement {
	Statement() : command(0), charId(0), emotion(0), textOffset(0),
		          optNum(0), address(0), val(0) { filename[0] = '\0'; }
	int16 command;
	int16 charId;
	int16 emotion;
	uint32 textOffset;
	uint32 optNum;
	char filename[11];
	uint16 address;
	int16 val;
};

struct Option {
	int32 optNum;
	uint32 textOffset;
	Common::List<Statement> statements;
};

struct Conversation {
	int16 charId;
	int16 convNum;
	Common::List<Option> options;
};

class Conv {
public:
	Conv(KomEngine *vm, uint16 charId);
	~Conv();

	bool doTalk(int16 convNum, int32 optNum);

private:

	void initConvs(uint32 offset);
	void initText(uint32 offset);

	void talkInit();
	void talkDeInit();

	int showOptions();
	void freeOptions();
	int getOption();
	void displayMenuOptions();
	int doStat(int selection);

	KomEngine *_vm;

	uint16 _charId;
	const char *_codename;
	Common::File *_convData;


	byte _backupPalette[256 * 4];

	byte *_convs;
	char *_text;

	int _selectedOption;
	int _scrollPos;
	int _surfaceHeight;

	struct {
		char *offset;
		char *text;
		int16 b;
		Common::List<Statement>* statements;
		int16 d;
		Common::String lines[24];
		int pixelTop;
		int pixelBottom;
	} _options[3];

	Common::List<Conversation> _conversations;
};

} // End of namespace Kom

#endif
