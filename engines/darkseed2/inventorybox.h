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

#include "darkseed2/darkseed2.h"
#include "darkseed2/saveable.h"
#include "darkseed2/inventory.h"
#include "darkseed2/objects.h"
#include "darkseed2/sprite.h"
#include "darkseed2/cursors.h"

namespace DarkSeed2 {

class Resources;
class Variables;
class ScriptRegister;
class Graphics;
class TalkManager;

class InventoryBox : public Saveable {
public:
	typedef const Inventory::Item *ItemRef;

	InventoryBox(Resources &resources, Variables &variables, ScriptRegister &scriptRegister,
			Graphics &graphics, TalkManager &talkManager, Cursors &cursors);
	~InventoryBox();

	int32 getWidth () const;
	int32 getHeight() const;

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
	struct BoxProperties {
		int32 width;  ///< The box's width.
		int32 height; ///< The box's height.

		const char *frameFile; ///< File used for the full frame.

		const char *frameTopFile;    ///< File used for the top part of the frame.
		const char *frameBottomFile; ///< File used for the bottom part of the frame.
		const char *frameLeftFile;   ///< File used for the left part of the frame.
		const char *frameRightFile;  ///< File used for the right part of the frame.

		const char *scrollLeftFile;    ///< Scrolling left active.
		const char *scrollNoLeftFile;  ///< Scrolling left grayed out.
		const char *scrollRightFile;   ///< Scrolling right active.
		const char *scrollNoRightFile; ///< Scrolling right grayed out.

		int32 frameLeftRightWidth; ///< The width of the left and right parts of the frame.
		int32 frameTopDownHeight;  ///< The height of the top and bottom parts of the frame.

		int32 scrollLeft[4];   ///< The scroll left button's coordinates.
		int32 scrollRight[4]; ///< The scroll right button's coordinates.

		int32 items[4];
		int32 visibleItems[4];

		int32 itemWidth;
		uint32 visibleItemsCount;
	} _boxProps;

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

	Sprite *_sprites; ///< The box part sprites.

	uint32 _colorShading; ///< Color index of the background shading.

	Common::Rect _itemsArea;      ///< Area where the items are visible.
	Common::Rect _scrollAreas[2]; ///< Areas of the scroll left/right buttons.

	/** The items in the inventory. */
	const Common::Array<Inventory::Item> *_items;

	/** All visible items. */
	Common::Array<const Inventory::Item *> _visibleItems;

	bool _visible; ///< Is the inventory currently visible?

	uint32 _firstItem;  ///< The first visible item.

	bool _scrolled; ///< Has a scroll-button been pressed?

	/** Fill in the box poperties struct, depending on the game version. */
	void fillInBoxProperties(GameVersion gameVersion);

	/** Initialize the inventory. */
	void initInventory();

	/** Update the used colors. */
	void updateColors();
	/** Load all item sprites. */
	void loadSprites();
	/** Update the scrolling sprites. */
	void updateScroll();
	/** Update the items sprite. */
	bool updateItems();

	/** Build the box. */
	void build();
	/** Redraw the items sprite. */
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
