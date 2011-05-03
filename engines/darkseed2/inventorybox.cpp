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

#include "common/serializer.h"

#include "darkseed2/inventorybox.h"
#include "darkseed2/imageconverter.h"
#include "darkseed2/resources.h"
#include "darkseed2/variables.h"
#include "darkseed2/graphics.h"
#include "darkseed2/talk.h"
#include "darkseed2/saveload.h"

namespace DarkSeed2 {

// Colors
static const byte kColorShading[3] = {  0,   0,   0};

InventoryBox::InventoryBox(Resources &resources, Variables &variables, ScriptRegister &scriptRegister,
		Graphics &graphics, TalkManager &talkManager, Cursors &cursors) {

	_resources      = &resources;
	_variables      = &variables;
	_scriptRegister = &scriptRegister;
	_graphics       = &graphics;
	_talkMan        = &talkManager;
	_cursors        = &cursors;

	fillInBoxProperties(resources.getVersionFormats().getGameVersion());

	initInventory();

	_items = 0;

	_visible = false;

	_firstItem = 0;

	_scrolled = false;

	_area = Common::Rect(_boxProps.width, _boxProps.height);

	_sprites     = new Sprite[8];

	_itemsArea      = Common::Rect(_boxProps.items      [0], _boxProps.items      [1],
	                               _boxProps.items      [2], _boxProps.items      [3]);
	_scrollAreas[0] = Common::Rect(_boxProps.scrollLeft [0], _boxProps.scrollLeft [1],
	                               _boxProps.scrollLeft [2], _boxProps.scrollLeft [3]);
	_scrollAreas[1] = Common::Rect(_boxProps.scrollRight[0], _boxProps.scrollRight[1],
	                               _boxProps.scrollRight[2], _boxProps.scrollRight[3]);

	updateColors();
	loadSprites();
	build();
}

InventoryBox::~InventoryBox() {
	delete _inventory;

	delete[] _sprites;
}

int32 InventoryBox::getWidth() const {
	return _boxProps.width;
}

int32 InventoryBox::getHeight() const {
	return _boxProps.height;
}

void InventoryBox::fillInBoxProperties(GameVersion gameVersion) {
	switch(gameVersion) {
	case kGameVersionWindows:
		_boxProps.width =  640;
		_boxProps.height =  70;

		_boxProps.frameFile = "INVNTRY1";

		_boxProps.scrollLeftFile    = "ARWLFT1";
		_boxProps.scrollNoLeftFile  = "ARWLFT2";
		_boxProps.scrollRightFile   = "ARWRGHT1";
		_boxProps.scrollNoRightFile = "ARWRGHT2";

		_boxProps.frameTopFile    = 0;
		_boxProps.frameBottomFile = 0;
		_boxProps.frameLeftFile   = 0;
		_boxProps.frameRightFile  = 0;

		_boxProps.frameLeftRightWidth = 0;
		_boxProps.frameTopDownHeight  = 0;

		_boxProps.scrollLeft[0]  =  11; _boxProps.scrollLeft[1]  =  27;
		_boxProps.scrollLeft[2]  =  32; _boxProps.scrollLeft[3]  =  53;
		_boxProps.scrollRight[0] = 608; _boxProps.scrollRight[1] =  27;
		_boxProps.scrollRight[2] = 629; _boxProps.scrollRight[3] =  53;

		_boxProps.items[0] =  64; _boxProps.items[1] =  10;
		_boxProps.items[2] = 576; _boxProps.items[3] =  60;

		_boxProps.visibleItems[0] =  95; _boxProps.visibleItems[1] =  10;
		_boxProps.visibleItems[2] = 545; _boxProps.visibleItems[3] =  60;

		_boxProps.itemWidth = 50;

		_boxProps.visibleItemsCount = (_boxProps.visibleItems[2] - _boxProps.visibleItems[0]) / _boxProps.itemWidth;

		break;

	case kGameVersionSaturn:
		_boxProps.width =  320;
		_boxProps.height =  48;

		_boxProps.frameFile = 0;

		_boxProps.frameTopFile    = "DLG_TOP";
		_boxProps.frameBottomFile = "DLG_BTM";
		_boxProps.frameLeftFile   = "DLG_L";
		_boxProps.frameRightFile  = "DLG_R";

		_boxProps.scrollLeftFile    = "ITEM_L1";
		_boxProps.scrollNoLeftFile  = "ITEM_L2";
		_boxProps.scrollRightFile   = "ITEM_R1";
		_boxProps.scrollNoRightFile = "ITEM_R2";

		_boxProps.frameLeftRightWidth = 40;
		_boxProps.frameTopDownHeight  =  7;

		_boxProps.scrollLeft[0]  =   6; _boxProps.scrollLeft[1]  =  19;
		_boxProps.scrollLeft[2]  =  17; _boxProps.scrollLeft[3]  =  36;
		_boxProps.scrollRight[0] = 303; _boxProps.scrollRight[1] =  19;
		_boxProps.scrollRight[2] = 314; _boxProps.scrollRight[3] =  36;

		_boxProps.items[0] =  35; _boxProps.items[1] =   7;
		_boxProps.items[2] = 285; _boxProps.items[3] =  41;

		_boxProps.visibleItems[0] =  48; _boxProps.visibleItems[1] =   8;
		_boxProps.visibleItems[2] = 272; _boxProps.visibleItems[3] =  40;

		_boxProps.itemWidth = 32;

		_boxProps.visibleItemsCount = (_boxProps.visibleItems[2] - _boxProps.visibleItems[0]) / _boxProps.itemWidth;

		break;

	default:
		error("Unknown game version");
	}
}

void InventoryBox::initInventory() {
	_inventory = new Inventory(*_resources, *_variables, *_scriptRegister, *_graphics, *_cursors);

	bool loaded = _inventory->parse(*_resources, "OBJ_9999");
	assert(loaded);
}

void InventoryBox::updateColors() {
	_colorShading = ImgConv.getColor(kColorShading[0], kColorShading[1], kColorShading[2]);
}

void InventoryBox::loadSprites() {
	ImageType boxImageType = _resources->getVersionFormats().getBoxImageType();

	if (boxImageType == kImageType256) {
		bool loaded0, loaded1, loaded2, loaded3, loaded4, loaded5, loaded6, loaded7;

		Palette palette;

		if (!palette.loadFromPAL555(*_resources, "PARTS"))
			error("Failed to load PARTS.PAL");

		ImgConv.registerStandardPalette(palette);

		_sprites[3].create(_boxProps.width, _boxProps.height);

		Sprite boxPart;

		loaded0 = boxPart.loadFromBoxImage(*_resources, _boxProps.frameLeftFile,
				_boxProps.frameLeftRightWidth, _boxProps.height);

		_sprites[3].blit(boxPart, 0, 0);

		loaded1 = boxPart.loadFromBoxImage(*_resources, _boxProps.frameRightFile,
				_boxProps.frameLeftRightWidth, _boxProps.height);

		_sprites[3].blit(boxPart, _boxProps.width - _boxProps.frameLeftRightWidth, 0);

		loaded2 = boxPart.loadFromBoxImage(*_resources, _boxProps.frameTopFile,
				_boxProps.width - (2 * _boxProps.frameLeftRightWidth), _boxProps.frameTopDownHeight);

		_sprites[3].blit(boxPart, _boxProps.frameLeftRightWidth, 0);

		loaded3 = boxPart.loadFromBoxImage(*_resources, _boxProps.frameBottomFile,
				_boxProps.width - (2 * _boxProps.frameLeftRightWidth), _boxProps.frameTopDownHeight);

		_sprites[3].blit(boxPart, _boxProps.frameLeftRightWidth,
				_boxProps.height - _boxProps.frameTopDownHeight);

		assert(loaded0 && loaded1 && loaded2 && loaded3);

		loaded4 = _sprites[4].loadFromBoxImage(*_resources, _boxProps.scrollLeftFile,
				_boxProps.frameLeftRightWidth, _boxProps.height);
		loaded5 = _sprites[5].loadFromBoxImage(*_resources, _boxProps.scrollNoLeftFile,
				_boxProps.frameLeftRightWidth, _boxProps.height);
		loaded6 = _sprites[6].loadFromBoxImage(*_resources, _boxProps.scrollRightFile,
				_boxProps.frameLeftRightWidth, _boxProps.height);
		loaded7 = _sprites[7].loadFromBoxImage(*_resources, _boxProps.scrollNoRightFile,
				_boxProps.frameLeftRightWidth, _boxProps.height);

		assert(loaded4 && loaded5 && loaded6 && loaded7);

		ImgConv.unregisterStandardPalette();

	} else {
		bool loaded0, loaded1, loaded2, loaded3, loaded4;

		loaded0 = _sprites[3].loadFromImage(*_resources, _boxProps.frameFile);
		loaded1 = _sprites[4].loadFromImage(*_resources, _boxProps.scrollLeftFile);
		loaded2 = _sprites[5].loadFromImage(*_resources, _boxProps.scrollNoLeftFile);
		loaded3 = _sprites[6].loadFromImage(*_resources, _boxProps.scrollRightFile);
		loaded4 = _sprites[7].loadFromImage(*_resources, _boxProps.scrollNoRightFile);

		assert(loaded0 && loaded1 && loaded2 && loaded3 && loaded4);
	}
}

void InventoryBox::updateScroll() {
	if (canScrollLeft())
		_box.blit(_sprites[4], 0, 0, true);
	else
		_box.blit(_sprites[5], 0, 0, true);

	if (canScrollRight())
		_box.blit(_sprites[6], _boxProps.width - _sprites[6].getWidth(), 0, true);
	else
		_box.blit(_sprites[7], _boxProps.width - _sprites[7].getWidth(), 0, true);

	Common::Rect scroll1 = _sprites[4].getArea();
	Common::Rect scroll2 = _sprites[6].getArea();
	scroll2.moveTo(_boxProps.width - _sprites[6].getWidth(), 0);

	scroll1.translate(_area.left, _area.top);
	scroll2.translate(_area.left, _area.top);

	_graphics->requestRedraw(scroll1);
	_graphics->requestRedraw(scroll2);
}

bool InventoryBox::updateItems() {
	bool first   = _items == 0;
	bool changed = _inventory->getItems(_items);

	if (!first && !changed && !_scrolled)
		// Nothing to do
		return false;

	// Find all visible items (items with an active look)
	_visibleItems.clear();
	for (Common::Array<Inventory::Item>::const_iterator item = _items->begin(); item != _items->end(); ++item)
		if (item->curLook)
			_visibleItems.push_back(&*item);

	// Draw the sprites
	_sprites[2].clear();
	for (uint32 i = _firstItem, n = 0; (i < _visibleItems.size()) && (n < _boxProps.visibleItemsCount); i++, n++) {
		const Sprite *sprite = _visibleItems[i]->curLook->sprite;

		if (!sprite) {
			warning("Object %d has no sprite?!?", i);
			continue;
		}

		_sprites[2].blit(*sprite, n * _boxProps.itemWidth, 0, true);
	}

	return true;
}

void InventoryBox::build() {
	_sprites[0].create(_boxProps.width, _boxProps.height);
	_box.create(_boxProps.width, _boxProps.height);

	// The shading grid
	_sprites[1].create(_itemsArea.width(), _itemsArea.height());
	_sprites[1].shade(_colorShading);

	_sprites[2].create(_boxProps.visibleItems[2] - _boxProps.visibleItems[0], _boxProps.visibleItems[3] - _boxProps.visibleItems[1]);

	// Put the shading grid
	_sprites[0].blit(_sprites[1], _itemsArea.left, _itemsArea.top, true);
	// Put the frame
	_sprites[0].blit(_sprites[3], 0, 0, true);

	// Put the visible items
	updateItems();
	_sprites[0].blit(_sprites[2], _boxProps.visibleItems[0], _boxProps.visibleItems[1], true);

	// Put the scroll sprites
	updateScroll();

	_box.blit(_sprites[0], 0, 0, false);
}

void InventoryBox::redrawItems() {
	Common::Rect visibleItemArea =
		Common::Rect(_boxProps.visibleItems[0], _boxProps.visibleItems[1], _boxProps.visibleItems[2], _boxProps.visibleItems[3]);

	Common::Rect shadingArea = _sprites[1].getArea();

	// Calculate the area of the shading grid that needs to be redrawn
	shadingArea.translate(_itemsArea.left, _itemsArea.top);
	shadingArea.clip(visibleItemArea);
	shadingArea.translate(-_itemsArea.left, -_itemsArea.top);

	// Draw the shading grid
	_box.blit(_sprites[1], shadingArea, visibleItemArea.left, visibleItemArea.top, false);
	// Put the visible items
	_box.blit(_sprites[2], visibleItemArea.left, visibleItemArea.top, true);

	visibleItemArea.translate(_area.left, _area.top);
	_graphics->requestRedraw(visibleItemArea);
}

bool InventoryBox::canScrollLeft() const {
	return _firstItem > 0;
}

bool InventoryBox::canScrollRight() const {
	return (_visibleItems.size() - _firstItem) > _boxProps.visibleItemsCount;
}

void InventoryBox::move(int32 x, int32 y) {
	_area.moveTo(x, y);
}

bool InventoryBox::isVisible() const {
	return _visible;
}

void InventoryBox::show() {
	if (_visible)
		return;

	_visible = true;

	_graphics->requestRedraw(_area);
}

void InventoryBox::hide() {
	if (!_visible)
		return;

	_visible = false;

	_graphics->requestRedraw(_area);
}

InventoryBox::ItemRef InventoryBox::findItem(const Common::String &name) const {
	return _inventory->findItem(name);
}

void InventoryBox::redraw(Sprite &sprite, Common::Rect area) {
	if (!_area.intersects(area))
		return;

	area.clip(_area);

	int32 x = area.left;
	int32 y = area.top;

	area.moveTo(area.left - _area.left, area.top - _area.top);

	sprite.blit(_box, area, x, y, true);
}

bool InventoryBox::hasAction(int32 x, int32 y, ObjectVerb verb) {
	int32 item = getItemNumber(x, y);
	if (item < 0)
		return false;

	if (((uint32) item) >= _visibleItems.size())
		return false;

	if ((verb == kObjectVerbLook) && (_visibleItems[item]->curLook))
		return true;
	if ((verb == kObjectVerbUse) && (_visibleItems[item]->curUse))
		return true;

	return false;
}

InventoryBox::ItemRef InventoryBox::doAction(int32 x, int32 y, ObjectVerb verb,
		const Cursors::Cursor *&changeTo) {

	changeTo = 0;

	ScrollAction scroll = getScrollAction(x - _area.left, y - _area.top);
	if (scroll != kScrollActionNone) {
		doScroll(scroll);
		return 0;
	}

	int32 item = getItemNumber(x, y);
	if (item < 0)
		return 0;

	if (((uint32) item) >= _visibleItems.size())
		return 0;

	if ((verb == kObjectVerbLook) && (_visibleItems[item]->curLook))
		_talkMan->talk(*_resources, _visibleItems[item]->curLook->text);

	if ((verb == kObjectVerbUse) && (_visibleItems[item]->curUse)) {
		if (_visibleItems[item]->curUse->cursor) {
			_variables->evalChange(_visibleItems[item]->curUse->changes);
			changeTo = _visibleItems[item]->curUse->cursor;
			return _visibleItems[item];
		} else
			warning("Object has no cursor?!?");
	}

	return 0;
}

void InventoryBox::undoAction(ItemRef item, ObjectVerb verb) {
	if (!item)
		return;

	if (verb == kObjectVerbUse) {
		if (_variables->get("UsingNothing") == 1)
			// Don't undo an use action when it was already cleared
			return;

		Common::Array<Inventory::ItemUse>::const_iterator use;
		for (use = item->uses.begin(); use != item->uses.end(); ++use) {
			if (_variables->evalCondition(use->conditions)) {
				_variables->evalChange(use->changes);
				return;
			}
		}

		// If no uses were possible, just reset UsingNothing
		_variables->set("UsingNothing", 1);

	}
}

int32 InventoryBox::getItemNumber(int32 x, int32 y) {
	if (x < _area.left)
		return -1;
	if (y < _area.top)
		return -1;

	x -= _area.left;
	y -= _area.top;

	if ((x < _boxProps.visibleItems[0]) || (x > _boxProps.visibleItems[2]))
		return -1;
	if ((y < _boxProps.visibleItems[1]) || (y > _boxProps.visibleItems[3]))
		return -1;

	return ((x - _boxProps.visibleItems[0]) / _boxProps.itemWidth) + _firstItem;
}

void InventoryBox::updateStatus() {
	if (updateItems()) {
		redrawItems();
		updateScroll();
	}

	_scrolled = false;
}

InventoryBox::ScrollAction InventoryBox::getScrollAction(int32 x, int32 y) {
	for (int i = 0; i < 2; i++)
		if (_scrollAreas[i].contains(x, y))
			return (ScrollAction) i;

	return kScrollActionNone;
}

void InventoryBox::doScroll(ScrollAction scroll) {
	switch (scroll) {
	case kScrollActionLeft:
		if (canScrollLeft())
			_firstItem--;
			_scrolled = true;
		break;

	case kScrollActionRight:
		if (canScrollRight())
			_firstItem++;
			_scrolled = true;
		break;

	default:
		break;
	}
}

bool InventoryBox::saveLoad(Common::Serializer &serializer, Resources &resources) {
	SaveLoad::sync(serializer, _visible);
	SaveLoad::sync(serializer, _firstItem);
	return true;
}

bool InventoryBox::loading(Resources &resources) {
	build();
	return true;
}

} // End of namespace DarkSeed2
