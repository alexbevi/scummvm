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

#include "liath/game/expression.h"
#include "liath/game/segment.h"
#include "liath/game/work.h"

#include "liath/helpers.h"
#include "liath/liath.h"
#include "liath/resource.h"

namespace Liath {

GraphicsManager::GraphicsManager(LiathEngine *engine) : _engine(engine),
	_textPalette(NULL), _textPalette2(NULL), _colorTable(NULL),
	_shadow(0), _hMemBackgroundExt(NULL) {}

GraphicsManager::~GraphicsManager() {
	// Zero-out passed pointers
	_engine = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Loading
//////////////////////////////////////////////////////////////////////////
void GraphicsManager::load(uint32 paletteOffset) {

	// Initialize palette
	if (paletteOffset)
		setPalette((char *)getSegment()->getData(kSegmentGame, paletteOffset));

	setPalette("logo.pal");

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
	EXPOSE_PARAMS(OpcodeParametersDefault);

	if ((!params->param1 && _hMemBackgroundExt) || params->param1)
		drawBMP(params->param1);

	return kOpcodeRetDefault;
}

OpcodeRet GraphicsManager::xscroll(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	getWork()->getCurrent()->xScroll += DSI2INT(EXPR(params->param1, params->param2));

	return kOpcodeRetDefault;
}

OpcodeRet GraphicsManager::yscroll(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	getWork()->getCurrent()->yScroll += DSI2INT(EXPR(params->param1, params->param2));

	return kOpcodeRetDefault;
}

OpcodeRet GraphicsManager::playAvs(OpcodeParameters *parameters, bool doStopMusic) {
	error("GraphicsManager::playAvs: Not implemented!");
}

OpcodeRet GraphicsManager::setBackground(OpcodeParameters *parameters) {
	error("GraphicsManager::set_bg: Not implemented!");
}

OpcodeRet GraphicsManager::setHeroRGB(OpcodeParameters *parameters) {
	error("GraphicsManager::set_hero_rgb: Not implemented!");
}

OpcodeRet GraphicsManager::look(OpcodeParameters *parameters) {
	error("GraphicsManager::look: Not implemented!");
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
	//g_system->setPalette(dummy_palette, 0, 3);
	warning("GraphicsManager::setPalette: Not implemented!");
}

void GraphicsManager::makeSoftTable(int16 maxSize) {
	warning("GraphicsManager::makeSoftTable: Not implemented!");
}

bool GraphicsManager::loadBackgroundPalette(Common::String paletteName) {
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

} // End of namespace Liath
