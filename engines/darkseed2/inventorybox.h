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

#ifndef DARKSEED2_INVENTORYBOX_H
#define DARKSEED2_INVENTORYBOX_H

#include "common/rect.h"

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/saveable.h"
#include "engines/darkseed2/inventory.h"
#include "engines/darkseed2/objects.h"
#include "engines/darkseed2/sprite.h"
#include "engines/darkseed2/cursors.h"

namespace DarkSeed2 {

class Resources;
class Variables;
class ScriptRegister;
class Graphics;
class TalkManager;

class InventoryBox : public Saveable {
public:
	typedef const Inventory::Item *ItemRef;

	static const int32 kWidth  = 640; ///< The box's width.
	static const int32 kHeight =  70; ///< The box's heigth.

	InventoryBox(Resources &resources, Variables &variables, ScriptRegister &scriptRegister,
			Graphics &graphics, TalkManager &talkManager, Cursors &cursors);
	~InventoryBox();

	/** Notify the box that a new palette is active. */
	void newPalette();

	/** Move the box to these coordinates. */
	void move(int32 x, int32 y);

	/** Is the inventory currently visible? */
	bool isVisible() const;

	/** Show the inventory. */
	void show();
	/** Hide the inventory. */
	void hide();

	/** Find a specific item. */
	ItemRef findItem(const Common::String &name) const;

	/** Redraw the inventory box. */
	void redraw(Sprite &sprite, Common::Rect area);

	/** Has the inventory an action for this verb at these coordinates? */
	bool hasAction(int32 x, int32 y, ObjectVerb verb);

	/** Do the action the inventory has for this verb at these coordinates. */
	ItemRef doAction(int32 x, int32 y, ObjectVerb verb, const Cursors::Cursor *&changeTo);

	/** Undo that item action. */
	void undoAction(ItemRef item, ObjectVerb verb);

	/** Check for changes in the box's status. */
	void updateStatus();

protected:
	bool saveLoad(Common::Serializer &serializer, Resources &resources);
	bool loading(Resources &resources);

private:
	/** A scrolling action. */
	enum ScrollAction {
		kScrollActionLeft,  ///< Scroll left.
		kScrollActionRight, ///< Scroll right.
		kScrollActionNone   ///< No scroll.
	};

	Resources      *_resources;
	Variables      *_variables;
	ScriptRegister *_scriptRegister;
	Graphics       *_graphics;
	TalkManager    *_talkMan;
	Cursors        *_cursors;

	/** The actual inventory. */
	Inventory *_inventory;

	Common::Rect _area; ///< The area where the box is visible.

	Sprite _box; ///< The box's sprite.

	Sprite *_origSprites; ///< The original box part sprites.
	Sprite *_sprites;     ///< The box part sprites adapted to the current palette.

	byte _colorShading; ///< Color index of the background shading.

	Common::Rect _itemsArea;      ///< Area where the items are visible.
	Common::Rect _scrollAreas[2]; ///< Areas of the scroll left/right buttons.

	/** The items in the inventory. */
	const Common::Array<Inventory::Item> *_items;

	/** All visible items. */
	Common::Array<const Inventory::Item *> _visibleItems;

	bool _visible; ///< Is the inventory currently visible?

	uint32 _firstItem;  ///< The first visible item.

	bool _scrolled;

	void initInventory();

	void updateColors();
	void loadSprites();
	void resetSprites();
	void updateScroll();
	bool updateItems();

	void rebuild();
	void redrawItems();

	/** Get the number of the item on these coordinates. */
	int32 getItemNumber(int32 x, int32 y);
	/** Get the scroll action area the coordinates are in. */
	ScrollAction getScrollAction(int32 x, int32 y);

	/** Scroll the items. */
	void doScroll(ScrollAction scroll);

	/** Is it possible to scroll to the left? */
	bool canScrollLeft() const;
	/** Is it possible to scroll to the right? */
	bool canScrollRight() const;
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_INVENTORYBOX_H
