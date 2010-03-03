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

#include "engines/darkseed2/inventorybox.h"
#include "engines/darkseed2/imageconverter.h"
#include "engines/darkseed2/resources.h"
#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/graphics.h"
#include "engines/darkseed2/talk.h"
#include "engines/darkseed2/saveload.h"

namespace DarkSeed2 {

// Sprite file names
static const char *kSpriteFrame         = "INVNTRY1"; ///< The inventory box's general frame.
static const char *kSpriteScrollLeft    = "ARWLFT1";  ///< Scrolling left active.
static const char *kSpriteScrollNoLeft  = "ARWLFT2";  ///< Scrolling left grayed out.
static const char *kSpriteScrollRight   = "ARWRGHT1"; ///< Scrolling right active.
static const char *kSpriteScrollNoRight = "ARWRGHT2"; ///< Scrolling right grayed out.

static const int32 kItems[4]        = { 64,  10, 576,  60};
static const int32 kVisibleItems[4] = { 95,  10, 545,  60};

static const  int32 kItemWidth         = 50;
static const uint32 kVisibleItemsCount = (kVisibleItems[2] - kVisibleItems[0]) / kItemWidth;

// Scroll button coordinates
static const int32 kScrollLeft [4] = { 11,  27,  32,  53};
static const int32 kScrollRight[4] = {608,  27, 629,  53};

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

	initInventory();

	_items = 0;

	_visible = false;

	_firstItem = 0;

	_scrolled = false;

	_area = Common::Rect(kWidth, kHeight);

	_sprites     = new Sprite[8];

	_itemsArea      = Common::Rect(kItems      [0], kItems      [1], kItems      [2], kItems      [3]);
	_scrollAreas[0] = Common::Rect(kScrollLeft [0], kScrollLeft [1], kScrollLeft [2], kScrollLeft [3]);
	_scrollAreas[1] = Common::Rect(kScrollRight[0], kScrollRight[1], kScrollRight[2], kScrollRight[3]);

	updateColors();
	loadSprites();
	build();
}

InventoryBox::~InventoryBox() {
	delete _inventory;

	delete[] _sprites;
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
	ImageType boxImageType = _resources->getBoxImageType();

	if (boxImageType == kImageType256) {
		warning("TODO: Sega Saturn inventory box images");
	} else {
		bool loaded0, loaded1, loaded2, loaded3, loaded4;

		loaded0 = _sprites[3].loadFromImage(*_resources, kSpriteFrame);
		loaded1 = _sprites[4].loadFromImage(*_resources, kSpriteScrollLeft);
		loaded2 = _sprites[5].loadFromImage(*_resources, kSpriteScrollNoLeft);
		loaded3 = _sprites[6].loadFromImage(*_resources, kSpriteScrollRight);
		loaded4 = _sprites[7].loadFromImage(*_resources, kSpriteScrollNoRight);

		assert(loaded0 && loaded1 && loaded2 && loaded3 && loaded4);
	}
}

void InventoryBox::updateScroll() {
	if (canScrollLeft())
		_box.blit(_sprites[4], 0, 0, true);
	else
		_box.blit(_sprites[5], 0, 0, true);

	if (canScrollRight())
		_box.blit(_sprites[6], kWidth - _sprites[6].getWidth(), 0, true);
	else
		_box.blit(_sprites[7], kWidth - _sprites[7].getWidth(), 0, true);

	Common::Rect scroll1 = _sprites[4].getArea();
	Common::Rect scroll2 = _sprites[6].getArea();
	scroll2.moveTo(kWidth - _sprites[6].getWidth(), 0);

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
	for (uint32 i = _firstItem, n = 0; (i < _visibleItems.size()) && (n < kVisibleItemsCount); i++, n++) {
		const Sprite *sprite = _visibleItems[i]->curLook->sprite;

		if (!sprite) {
			warning("Object %d has no sprite?!?", i);
			continue;
		}

		_sprites[2].blit(*sprite, n * kItemWidth, 0, true);
	}

	return true;
}

void InventoryBox::build() {
	_sprites[0].create(kWidth, kHeight);
	_box.create(kWidth, kHeight);

	// The shading grid
	_sprites[1].create(_itemsArea.width(), _itemsArea.height());
	_sprites[1].shade(_colorShading);

	_sprites[2].create(kVisibleItems[2] - kVisibleItems[0], kVisibleItems[3] - kVisibleItems[1]);

	// Put the shading grid
	_sprites[0].blit(_sprites[1], _itemsArea.left, _itemsArea.top, true);
	// Put the frame
	_sprites[0].blit(_sprites[3], 0, 0, true);

	// Put the visible items
	updateItems();
	_sprites[0].blit(_sprites[2], kVisibleItems[0], kVisibleItems[1], true);

	// Put the scroll sprites
	updateScroll();

	_box.blit(_sprites[0], 0, 0, false);
}

void InventoryBox::redrawItems() {
	Common::Rect visibleItemArea =
		Common::Rect(kVisibleItems[0], kVisibleItems[1], kVisibleItems[2], kVisibleItems[3]);

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
	return (_visibleItems.size() - _firstItem) > kVisibleItemsCount;
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

	if ((x < kVisibleItems[0]) || (x > kVisibleItems[2]))
		return -1;
	if ((y < kVisibleItems[1]) || (y > kVisibleItems[3]))
		return -1;

	return ((x - kVisibleItems[0]) / kItemWidth) + _firstItem;
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
