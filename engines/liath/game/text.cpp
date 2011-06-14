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

#include "liath/game/text.h"

#include "liath/game/game.h"
#include "liath/game/hero.h"
#include "liath/game/work.h"

#include "liath/helpers.h"
#include "liath/liath.h"

namespace Liath {

TextManager::TextManager(LiathEngine *engine) : _engine(engine) {
	memset(&_arrayTextNumber, 0, sizeof(_arrayTextNumber));
}

TextManager::~TextManager() {
	// Zero-out passed pointers
	_engine = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Opcodes
//////////////////////////////////////////////////////////////////////////

OpcodeRet TextManager::out(OpcodeParameters *parameters) {
	int32 index = getFirstEmptyText();

	if (index == -1)
		return kOpcodeRetDefault;

	parameters->setDword(181, index);

	Work *work;
	getHero()->start(parameters, &work, NULL);
	getGame()->letValue((ParamOrigin)parameters->getByte(164), parameters->getDword(165), parameters->getDword(169), index);

	// TODO Store current values

	// TODO show text
	//getGraphics()->intersecShowSpr();

	// TODO Restore current values
	error("TextManager::out: Not implemented!");
}

OpcodeRet TextManager::quit(OpcodeParameters *parameters) {
	return exitText(parameters);
}

OpcodeRet TextManager::exitText(OpcodeParameters *parameters) {
	int32 index = getGame()->getValue((ParamOrigin)parameters->getByte(0), parameters->getDword(1), parameters->getDword(5));

	exitText(index);
	freeNumberText(index);

	getGame()->letValue((ParamOrigin)parameters->getByte(0), parameters->getDword(1), parameters->getDword(5), 0);

	return kOpcodeRetDefault;
}

OpcodeRet TextManager::check(OpcodeParameters *parameters) {
	int32 index = getGame()->getValue((ParamOrigin)parameters->getByte(0), parameters->getDword(1), parameters->getDword(5));

	if (!_arrayTextNumber[index])
		return RET(_arrayTextNumber[index], parameters->test);

	Work *work = getWork()->getFromTextIndex(index);

	if (!work)
		return (parameters->test ? kOpcodeRetNext : kOpcodeRetDefault);

	return RET(!work->workHeroData->music
	        || (work->workHeroData->music && work->workHeroData->music->field_16D),
			  parameters->test);
}

OpcodeRet TextManager::change(OpcodeParameters *parameters) {
	// Original just returns 1
	return kOpcodeRetDefault;
}


//////////////////////////////////////////////////////////////////////////
// Private functions
//////////////////////////////////////////////////////////////////////////

OpcodeRet TextManager::exitText(int32 index) {
	error("[WorkManager::exit]: Not implemented!");
}


//////////////////////////////////////////////////////////////////////////
// Helper functions
//////////////////////////////////////////////////////////////////////////

int32 TextManager::getFirstEmptyText() {
	for (uint32 i = 1; i < sizeof(_arrayTextNumber); i++)
		if (!_arrayTextNumber[i]) {
			_arrayTextNumber[i] = 1;
			return i;
		}

	return -1;
}

void TextManager::freeNumberText(int32 index) {
	if (index < 0 || index >= (int32)sizeof(_arrayTextNumber))
		error("Invalid text number: %d (must be in [0-%d]", index, sizeof(_arrayTextNumber));

	_arrayTextNumber[index] = 0;
}

} // End of namespace Liath
