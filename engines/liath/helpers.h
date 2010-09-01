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
 * $Id: helpers.h 606 2010-08-17 21:30:59Z julien.templier $
 *
 */

#ifndef LIATH_HELPERS_H
#define LIATH_HELPERS_H

//////////////////////////////////////////////////////////////////////////
// Misc helpers
//////////////////////////////////////////////////////////////////////////

//#define LOBYTE(b)           ((b) & 0xFF)
//#define HIBYTE(b)           (((b) >> 8) & 0xFF)
#define LOWORD(l)           ((l) & 0xffff)
#define HIWORD(l)           (((l) >> 16) & 0xffff)

// Parameters
#define EXPOSE_PARAMS(type) type *params = (type *)parameters

// Misc
#define GLOBAL(val)         getGame()->getGlobal(val)
#define EXPR(offset, count) getExpression()->expression(offset, count)
#define RET(val1, val2)     getGame()->getReturnValue(val1, val2)


// Shared objects
#define getAction()     _engine->getActionManager()
#define getArray()      _engine->getArrayManager()
#define getExpression() _engine->getExpressionManager()
#define getGame()       _engine->getGameManager()
#define getGraphics()   _engine->getGraphicsManager()
#define getHero()       _engine->getHeroManager()
#define getMouse()      _engine->getMouseManager()
#define getProgress()   _engine->getProgressManager()
#define getPosition()   _engine->getPositionManager()
#define getResource()   _engine->getResourceManager()
#define getSave()       _engine->getSavegameManager()
#define getSegment()    _engine->getSegmentManager()
#define getSound()      _engine->getSoundManager()
#define getText()       _engine->getTextManager()
#define getWork()       _engine->getWorkManager()

// Operations
#define DSI2INT(val) ((val) >> 16)
#define INT2DSI(val) ((val) << 16)
#define CEL2SCR(val) (uint16)HIWORD(val)
#define SCR2CEL(val) ((uint32)val << 16 ) | 0
#define DSCR2CEL(val) (float)(val * 65536.0)

//////////////////////////////////////////////////////////////////////////
// Cleanup
//////////////////////////////////////////////////////////////////////////

#define SAFE_DELETE(_p) { if(_p) { delete (_p); (_p) = NULL; } }

#define CLEAR_ARRAY(type, name) { \
	for (Common::Array<type *>::iterator it = name.begin(); it != name.end(); it++) \
		SAFE_DELETE(*it); \
	name.clear(); \
}

#endif // LIATH_HELPERS_H
