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

#include "common/macresman.h"
#include "common/ne_exe.h"

#include "graphics/cursorman.h"

#include "darkseed2/cursors.h"
#include "darkseed2/imageconverter.h"
#include "darkseed2/resources.h"
#include "darkseed2/palette.h"
#include "darkseed2/saveload.h"

namespace DarkSeed2 {

static const byte staticCursorPointerData[] = {
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0,
	1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0,
	1, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0,
	1, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0,
	1, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0,
	1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0,
	1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0,
	1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1,
	1, 2, 2, 2, 1, 2, 2, 1, 0, 0, 0, 0,
	1, 2, 2, 1, 1, 2, 2, 1, 0, 0, 0, 0,
	1, 2, 1, 0, 1, 1, 2, 2, 1, 0, 0, 0,
	1, 1, 0, 0, 0, 1, 2, 2, 1, 0, 0, 0,
	1, 0, 0, 0, 0, 0, 1, 2, 2, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 2, 2, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0
};

Cursors::Cursors() {
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

	Cursor def;
	def.name     = "cArrow";
	def.width    = 12;
	def.height   = 20;
	def.hotspotX = 0;
	def.hotspotY = 0;

	def.sprite = new Sprite();
	def.sprite->create(def.width, def.height);
	def.sprite->setPalette(palette);
	def.sprite->copyFrom(staticCursorPointerData, 1);

	_cursors.setVal("cArrow", def);

	_visible = true;

	ImgConv.unregisterStandardPalette();
}

Cursors::~Cursors() {
	for (CursorMap::iterator it = _cursors.begin(); it != _cursors.end(); ++it)
		delete it->_value.sprite;
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

bool Cursors::saveLoad(Common::Serializer &serializer, Resources &resources) {
	SaveLoad::sync(serializer, _visible);
	SaveLoad::sync(serializer, _currentCursor);
	return true;
}

bool Cursors::loading(Resources &resources) {
	assertCursorProperties();
	return true;
}

CursorsWindows::CursorsWindows(const Common::String &exeName) : Cursors(), _exeName(exeName) {
}

bool CursorsWindows::load() {
	Common::NEResources resources;

	// Load the resources from the EXE
	if (!resources.loadFromEXE(_exeName))
		return false;

	// Convert cursor resources to usable cursors
	const Common::Array<Common::NECursorGroup> &cursorGroups = resources.getCursors();
	Common::Array<Common::NECursorGroup>::const_iterator cursorGroup;
	for (cursorGroup = cursorGroups.begin(); cursorGroup != cursorGroups.end(); ++cursorGroup) {
		if (cursorGroup->cursors.empty())
			continue;

		const Common::NECursor *neCursor = cursorGroup->cursors[0];
		Cursor cursor;

		if (!loadFromResource(cursor, *neCursor))
			return false;

		cursor.name = cursorGroup->id.toString();

		_cursors.setVal(cursor.name, cursor);
	}

	return true;
}

bool CursorsWindows::loadFromResource(Cursor &cursor, const Common::NECursor &resource) {
	// Copy properties
	cursor.width    = resource.getWidth();
	cursor.height   = resource.getHeight();
	cursor.hotspotX = resource.getHotspotX();
	cursor.hotspotY = resource.getHotspotY();

	Palette palette;
	palette.resize(256);

	const byte *srcPalette = resource.getPalette();

	for (int j = 0; j < 256; j++) {
		palette.get()[j * 3 + 0] = srcPalette[j * 4 + 0];
		palette.get()[j * 3 + 1] = srcPalette[j * 4 + 1];
		palette.get()[j * 3 + 2] = srcPalette[j * 4 + 2];
	}

	// Ensure the key color is transparent
	palette.get()[0] = 0;
	palette.get()[1] = 0;
	palette.get()[2] = 255;

	cursor.sprite = new Sprite;
	cursor.sprite->create(cursor.width, cursor.height);
	cursor.sprite->setPalette(palette);
	cursor.sprite->copyFrom(resource.getSurface());
	return true;
}

CursorsSaturn::CursorsSaturn(Resources &resources) : Cursors(), _resources(&resources) {
}

const char *CursorsSaturn::_saturnCursors[] = {
	"c4Ways"  , "cArrow"  , "cBCard"  , "cBGun"   , "cCamera" ,
	"cChanger", "cCTicket", "cCWrench", "cDCard"  , "cDFood"  ,
	"cDPhoto" , "cEgoMGR" , "cGKey"   , "cHand"   , "cJGun"   ,
	"cKeyCh"  , "cLetter" , "cLight"  , "cLook"   , "cLookAt" ,
	"cMagnet" , "cNPaper" , "cPhoneBk", "cPills"  , "cQuarter",
	"cRingC"  , "cRPhoto" , "crplush" , "cRTicket", "cScroll" ,
	"cSword"  , "cTargetC", "cTplush" , "cUseIt"  , "cWheelC" ,
	"cWplush" , "cXBow"
};

bool CursorsSaturn::load() {
	for (int i = 0; i < ARRAYSIZE(_saturnCursors); i++) {
		Cursor cursor;

		cursor.sprite = new Sprite;

		if (!cursor.sprite->loadFromSaturnCursor(*_resources, _saturnCursors[i])) {
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

CursorsMac::CursorsMac(Common::MacResManager &exeResFork) : _exeResFork(&exeResFork) {
}

bool CursorsMac::load() {
	Common::MacResIDArray idArray = _exeResFork->getResIDArray(MKID_BE('crsr'));

	for (uint32 i = 0; i < idArray.size(); i++) {
		Cursor cursor;

		cursor.name = _exeResFork->getResName(MKID_BE('crsr'), idArray[i]);

		if (cursor.name.empty())
			continue;

		Common::SeekableReadStream *stream = _exeResFork->getResource(MKID_BE('crsr'), idArray[i]);
		uint32 streamSize = stream->size();
		byte *data = new byte[streamSize];
		stream->read(data, streamSize);
		delete stream;

		byte *cursorData, *rawPalette;
		int keyColor, palSize;

		Common::MacResManager::convertCrsrCursor(data, streamSize, &cursorData, &cursor.width, &cursor.height, &cursor.hotspotX, &cursor.hotspotY, &keyColor, true, &rawPalette, &palSize);

		Palette palette;

		palette.resize(MAX<int>(palSize, keyColor + 1));

		for (int j = 0; j < palSize; j++) {
			palette.get()[j * 3 + 0] = rawPalette[j * 4 + 0];
			palette.get()[j * 3 + 1] = rawPalette[j * 4 + 1];
			palette.get()[j * 3 + 2] = rawPalette[j * 4 + 2];
		}

		// Ensure the keyColor is transparent
		palette.get()[keyColor * 3 + 0] = 0;
		palette.get()[keyColor * 3 + 1] = 0;
		palette.get()[keyColor * 3 + 2] = 255;

		delete[] rawPalette;

		cursor.sprite = new Sprite();
		cursor.sprite->create(cursor.width, cursor.height);
		cursor.sprite->setPalette(palette);
		cursor.sprite->copyFrom(cursorData);

		delete[] cursorData;

		_cursors[cursor.name] = cursor;
	}

	return true;
}

} // End of namespace DarkSeed2
