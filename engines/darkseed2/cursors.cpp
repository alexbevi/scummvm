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

#include "graphics/cursorman.h"

#include "engines/darkseed2/cursors.h"
#include "engines/darkseed2/imageconverter.h"
#include "engines/darkseed2/resources.h"
#include "engines/darkseed2/palette.h"
#include "engines/darkseed2/neresources.h"
#include "engines/darkseed2/saveload.h"

namespace DarkSeed2 {

#include "engines/darkseed2/cursordata.h"

const char *Cursors::_saturnCursors[] = {
	"c4Ways"  , "cArrow"  , "cBCard"  , "cBGun"   , "cCamera" ,
	"cChanger", "cCTicket", "cCWrench", "cDCard"  , "cDFood"  ,
	"cDPhoto" , "cEgoMGR" , "cGKey"   , "cHand"   , "cJGun"   ,
	"cKeyCh"  , "cLetter" , "cLight"  , "cLook"   , "cLookAt" ,
	"cMagnet" , "cNPaper" , "cPhoneBk", "cPills"  , "cQuarter",
	"cRingC"  , "cRPhoto" , "crplush" , "cRTicket", "cScroll" ,
	"cSword"  , "cTargetC", "cTplush" , "cUseIt"  , "cWheelC" ,
	"cWplush" , "cXBow"
};

Cursors::Cursors(const Common::String &exe) {
	Palette palette;

	palette.resize(3);

	// Standard palette: transparent, black, white
	palette.get()[0 * 3 + 0] = 0;
	palette.get()[0 * 3 + 1] = 0;
	palette.get()[0 * 3 + 2] = 255;
	palette.get()[1 * 3 + 0] = 0;
	palette.get()[1 * 3 + 1] = 0;
	palette.get()[1 * 3 + 2] = 0;
	palette.get()[2 * 3 + 0] = 255;
	palette.get()[2 * 3 + 1] = 255;
	palette.get()[2 * 3 + 2] = 255;

	ImgConv.registerStandardPalette(palette);

	// Loading the default pointer cursor from static memory
	_defaultResource = new NECursor;

	_defaultResource->setDimensions(staticCursorPointer.width   , staticCursorPointer.height  );
	_defaultResource->setHotspot   (staticCursorPointer.hotspotX, staticCursorPointer.hotspotY);
	_defaultResource->setData      (staticCursorPointer.data    , staticCursorPointer.dataSize);

	bool loaded;

	Cursor def;

	loaded = loadFromResource(def, *_defaultResource);
	assert(loaded);

	_cursors.setVal("cArrow", def);

	if (!exe.empty()) {
		// Loading the rest of the cursors out of the EXE resource table
		loaded = loadFromNEEXE(exe);
		assert(loaded);
	}

	_visible = true;

	ImgConv.unregisterStandardPalette();
}

Cursors::~Cursors() {
	clearCursors();
}

void Cursors::clearCursors() {
	delete _defaultResource;
	_defaultResource = 0;

	for (CursorMap::iterator it = _cursors.begin(); it != _cursors.end(); ++it)
		delete it->_value.sprite;

	_cursors.clear();
}

bool Cursors::loadSaturnCursors(Resources &resources) {
	clearCursors();

	for (int i = 0; i < ARRAYSIZE(_saturnCursors); i++) {
		Cursor cursor;

		cursor.sprite = new Sprite;

		if (!cursor.sprite->loadFromSaturnCursor(resources, _saturnCursors[i])) {
			delete cursor.sprite;
			return false;
		}

		cursor.name     = _saturnCursors[i];
		cursor.width    = cursor.sprite->getWidth();
		cursor.height   = cursor.sprite->getHeight();
		cursor.hotspotX = cursor.sprite->getFeetX();
		cursor.hotspotY = cursor.sprite->getFeetY();

		_cursors.setVal(cursor.name, cursor);
	}

	return true;
}

void Cursors::assertCursorProperties() {
	setVisible(_visible);
	setCursor(_currentCursor);
}

bool Cursors::isVisible() const {
	return _visible;
}

void Cursors::setVisible(bool visible) {
	_visible = visible;
	CursorMan.showMouse(visible);
}

const Cursors::Cursor *Cursors::getCursor(const Common::String &cursor) const {
	// "" = default arrow cursor
	if (cursor.empty())
		return getCursor("cArrow");

	if (!_cursors.contains(cursor))
		// Doesn't exist
		return 0;

	// Named cursor
	return &_cursors.getVal(cursor);
}

bool Cursors::setCursor(const Common::String &cursor) {
	const Cursor *cur = getCursor(cursor);

	if (!cur)
		return false;

	return setCursor(*cur);
}

bool Cursors::setCursor(const Cursors::Cursor &cursor) {
	_currentCursor = cursor.name;

	CursorMan.replaceCursor((const byte *) cursor.sprite->getTrueColor().pixels,
			cursor.width, cursor.height, cursor.hotspotX, cursor.hotspotY, ImgConv.getColor(0, 0, 255), 1,
			&ImgConv.getPixelFormat());

	return true;
}

const Common::String &Cursors::getCurrentCursor() const {
	return _currentCursor;
}

bool Cursors::loadFromNEEXE(const Common::String &exe) {
	NEResources resources;

	// Load the resources from the EXE
	if (!resources.loadFromEXE(exe))
		return false;

	// Convert cursor resources to usable cursors
	const Common::Array<NECursorGroup> &cursorGroups = resources.getCursors();
	Common::Array<NECursorGroup>::const_iterator cursorGroup;
	for (cursorGroup = cursorGroups.begin(); cursorGroup != cursorGroups.end(); ++cursorGroup) {
		if (cursorGroup->cursors.empty())
			continue;

		const NECursor &neCursor = cursorGroup->cursors[0];
		Cursor cursor;

		if (!loadFromResource(cursor, neCursor))
			return false;

		cursor.name = cursorGroup->name;

		_cursors.setVal(cursorGroup->name, cursor);
	}

	return true;
}

bool Cursors::loadFromResource(Cursor &cursor, const NECursor &resource) {
	// Load image
	cursor.sprite = new Sprite;
	if (!cursor.sprite->loadFromCursorResource(resource)) {
		delete cursor.sprite;
		return false;
	}

	// Copy properties
	cursor.width    = resource.getWidth();
	cursor.height   = resource.getHeight();
	cursor.hotspotX = resource.getHotspotX();
	cursor.hotspotY = resource.getHotspotY();
	return true;
}

bool Cursors::saveLoad(Common::Serializer &serializer, Resources &resources) {
	SaveLoad::sync(serializer, _visible);
	SaveLoad::sync(serializer, _currentCursor);
	return true;
}

bool Cursors::loading(Resources &resources) {
	assertCursorProperties();
	return true;
}

} // End of namespace DarkSeed2
