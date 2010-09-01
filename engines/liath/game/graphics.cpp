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
#include "liath/game/work.h"

#include "liath/helpers.h"
#include "liath/liath.h"

namespace Liath {

GraphicsManager::GraphicsManager(LiathEngine *engine) : _engine(engine), _hMemBackgroundExt(NULL) {}

GraphicsManager::~GraphicsManager() {
	// Zero-out passed pointers
	_engine = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Loading
//////////////////////////////////////////////////////////////////////////
void GraphicsManager::loadData() {
	warning("GraphicsManager::loadData: Not implemented!");
}

//////////////////////////////////////////////////////////////////////////
// Opcodes
//////////////////////////////////////////////////////////////////////////
OpcodeRet GraphicsManager::redraw(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	if (!params->param && _hMemBackgroundExt || params->param)
		drawBMP(params->param);

	return kOpcodeRetDefault;
}

OpcodeRet GraphicsManager::xscroll(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	getWork()->getCurrent()->xScroll += DSI2INT(EXPR(params->param, params->param2));

	return kOpcodeRetDefault;
}

OpcodeRet GraphicsManager::yscroll(OpcodeParameters *parameters) {
	EXPOSE_PARAMS(OpcodeParametersDefault);

	getWork()->getCurrent()->yScroll += DSI2INT(EXPR(params->param, params->param2));

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

}

} // End of namespace Liath
