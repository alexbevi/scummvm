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

#include "liath/game/graphics.h"

#include "liath/data/resource.h"
#include "liath/data/segment.h"

#include "liath/game/action.h"
#include "liath/game/expression.h"
#include "liath/game/work.h"

#include "liath/helpers.h"
#include "liath/liath.h"

#include "common/system.h"

namespace Liath {

GraphicsManager::GraphicsManager(LiathEngine *engine) : _engine(engine),
	_textPalette(NULL), _textPalette2(NULL), _colorTable(NULL),
	_shadow(0), _hMemBackgroundExt(NULL), _nBg(0), _nBg2(0) {}

GraphicsManager::~GraphicsManager() {
	// Zero-out passed pointers
	_engine = NULL;

	unload();
}

//////////////////////////////////////////////////////////////////////////
// Loading
//////////////////////////////////////////////////////////////////////////
void GraphicsManager::load(uint32 paletteOffset) {

	// Initialize palette
	if (paletteOffset)
		setPalette((char *)getSegment()->getData(kSegmentGame, paletteOffset));

	// Load text palettes
	_textPalette = malloc(768);
	_textPalette2 = malloc(512);

	Common::SeekableReadStream *stream = getResource()->createReadStreamForMember("text.col");
	if (!stream)
		error("GraphicsManager::load: File not found (text.col)!");

	stream->read(_textPalette, 768);

	delete stream;
}

void GraphicsManager::unload() {
	CLEAR_ARRAY(PaletteEntry, _palette);

	SAFE_FREE(_textPalette);
	SAFE_FREE(_textPalette2);
	SAFE_FREE(_colorTable);

	// Recompute color table
	clearSoftTable();
}

//////////////////////////////////////////////////////////////////////////
// Opcodes
//////////////////////////////////////////////////////////////////////////
OpcodeRet GraphicsManager::redraw(OpcodeParameters *parameters) {
	if ((!parameters->getDword(0) && _hMemBackgroundExt) || parameters->getDword(0))
		drawBMP(parameters->getDword(0));

	return kOpcodeRetDefault;
}

OpcodeRet GraphicsManager::xscroll(OpcodeParameters *parameters) {
	uint32 scroll = DSI2INT(EXPR(parameters->getDword(0), parameters->getDword(4)));
	getWork()->getCurrent()->xScroll += scroll;

	debugC(kLiathDebugInterpreter, "  xscroll += %d [expr(%d, %d)]\n", scroll, parameters->getDword(0), parameters->getDword(4));

	return kOpcodeRetDefault;
}

OpcodeRet GraphicsManager::yscroll(OpcodeParameters *parameters) {
	uint32 scroll = DSI2INT(EXPR(parameters->getDword(0), parameters->getDword(4)));
	getWork()->getCurrent()->yScroll += scroll;

	debugC(kLiathDebugInterpreter, "  yscroll += %d [expr(%d, %d)]\n", scroll, parameters->getDword(0), parameters->getDword(4));

	return kOpcodeRetDefault;
}

OpcodeRet GraphicsManager::playAvs(OpcodeParameters *parameters, bool doStopMusic) {
	error("GraphicsManager::playAvs: Not implemented!");
}

OpcodeRet GraphicsManager::setBackground(OpcodeParameters *parameters) {
	uint32 index = DSI2INT(EXPR(parameters->getDword(0), parameters->getDword(4)));

	if (_nBg != index)
		getWork()->resetField_E00();

	_nBg = index;

	return (parameters->test ? kOpcodeRetNext : kOpcodeRetDefault);
}

OpcodeRet GraphicsManager::setHeroRGB(OpcodeParameters *parameters) {
	error("GraphicsManager::set_hero_rgb: Not implemented!");
}

OpcodeRet GraphicsManager::look(OpcodeParameters *parameters) {
	error("GraphicsManager::look: Not implemented!");
}

//////////////////////////////////////////////////////////////////////////
// Drawing functions
//////////////////////////////////////////////////////////////////////////
void GraphicsManager::viewBackground() {
	drawBMP(_bgBas);
}

void GraphicsManager::intersecShowSpr(int spriteAddress) {
	error("GraphicsManager::intersecShowSpr: Not implemented!");
}

//////////////////////////////////////////////////////////////////////////
// Coordinates
//////////////////////////////////////////////////////////////////////////
int32 GraphicsManager::PicX(int32 a1) {
	Action *action = (Action *)getSegment()->getData(kSegmentAction, 2);

	return (a1 * action->field_20[5]) + action->field_20[3] + (action->field_20[5] / 2) + 1;
}

int32 GraphicsManager::PicY(int32 a1) {
	Action *action = (Action *)getSegment()->getData(kSegmentAction, 2);

	return (a1 * action->field_20[7]) + action->field_20[4] + (action->field_20[7] / 2) + 1;
}

int32 GraphicsManager::XPic(int32 a1) {
	Action *action = (Action *)getSegment()->getData(kSegmentAction, 2);

	int32 val = (a1 - action->field_20[3]) / action->field_20[5];

	return (val <= 0) ? 0 : val;
}

int32 GraphicsManager::YPic(int32 a1) {
	Action *action = (Action *)getSegment()->getData(kSegmentAction, 2);

	int32 val = (a1 - action->field_20[4]) / action->field_20[7];

	return (val <= 0) ? 0 : val;
}

int32 GraphicsManager::HPic(int32 a1) {
	Action *action = (Action *)getSegment()->getData(kSegmentAction, 2);

	return a1 / action->field_20[9] + 1;
}

int32 GraphicsManager::LPic(int32 a1, int32 a2) {
	Action *action = (Action *)getSegment()->getData(kSegmentAction, 2);

	if (action->field_20[12] <= a1)
		a1 = action->field_20[12] - 1;

	if (a1 < 0)
		a1 = 0;

	if (action->field_20[13] <= a2)
		a2 = action->field_20[13] - 1;

	if (a2 < 0)
		a2 = 0;

	return a1 + a2 * action->field_20[12];
}

//////////////////////////////////////////////////////////////////////////
// Private functions
//////////////////////////////////////////////////////////////////////////

void GraphicsManager::drawBMP(bool doSetBackground) {
	warning("GraphicsManager::drawBMP: Not implemented!");
}

void GraphicsManager::setPalette(Common::String paletteName) {
	if (!loadBackgroundPalette(paletteName))
		return;

	// Set the screen palette
	byte *palette = toSystemPalette(_palette);
	g_system->setPalette(palette, 0, 3);
	free(palette);
}

void GraphicsManager::makeSoftTable(uint16 maxSize) {
	for (uint i = 0; i < 3; i++) {

		if (i >= maxSize) {
			if (_softTable[i])
				free(_softTable[i]);

			continue;
		}

		if (!_softTable[i])
			_softTable[i] = (uint32 *)malloc(131074);
	}
}

bool GraphicsManager::loadBackgroundPalette(Common::String paletteName) {
	CLEAR_ARRAY(PaletteEntry, _palette);

	Common::SeekableReadStream *stream = getResource()->createReadStreamForMember(paletteName);
	if (!stream) {
		warning("GraphicsManager::loadBackgroundPalette: File not found (%s)!", paletteName.c_str());
		return false;
	}

	// Read version and entry count
	stream->readUint16LE();
	uint16 count = stream->readUint16LE();

	for (uint i = 0; i < count; i++) {
		PaletteEntry *entry = new PaletteEntry();

		entry->red = stream->readByte();
		entry->green = stream->readByte();
		entry->blue = stream->readByte();
		entry->flags = stream->readByte();

		_palette.push_back(entry);
	}

	delete stream;

	// Set first entry to black
	_palette[0]->red = 0;
	_palette[0]->green = 0;
	_palette[0]->blue = 0;

	// TODO update palette on a certain action

	return true;
}

byte *GraphicsManager::toSystemPalette(Common::Array<PaletteEntry *> _palette) {
	error("GraphicsManager::makeSoftTable: Not implemented!");
}

} // End of namespace Liath
