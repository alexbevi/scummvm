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

#ifndef DARKSEED2_INVENTORY_H
#define DARKSEED2_INVENTORY_H

#include "common/str.h"
#include "common/array.h"
#include "common/list.h"

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/objects.h"
#include "engines/darkseed2/cursors.h"

namespace DarkSeed2 {

class Resources;
class Variables;
class ScriptRegister;
class Graphics;

class DATFile;
class Sprite;

class Inventory : private ObjectContainer {
public:
	/** An item look. */
	struct ItemLook {
		/** The conditions needed to be met for the look to be active. */
		Common::List<Common::String> conditions;

		Common::String spriteName; ///< The name of the sprite.
		const Sprite  *sprite;     ///< The item's sprite.

		Common::String text;  ///< The text to be spooken.
	};

	/** An item use. */
	struct ItemUse {
		/** The conditions needed to be met for the use to be active. */
		Common::List<Common::String> conditions;

		Common::String         cursorName; ///< The name of the cursor.
		const Cursors::Cursor *cursor;     ///< The item's cursor.

		Common::List<Common::String> changes; ///< The changes brought in by the use.
	};

	/** An item. */
	struct Item {
		Common::String name; ///< The item's name.

		Common::Array<ItemLook> looks; ///< All looks for this item.
		Common::Array<ItemUse>  uses;  ///< All uses for this item.

		ItemLook *curLook; ///< The currently active look, or 0 if none.
		ItemUse  *curUse;  ///< The currently active use, or 0 if none.
	};

	Inventory(Resources &resources, Variables &variables,
			ScriptRegister &scriptRegister, Graphics &graphics, Cursors &cursors);
	~Inventory();

	/** Empty the inventory. */
	void clear();

	/** Notify the box that a new palette is active. */
	void newPalette();

	/** Parse an inventory file. */
	bool parse(Resources &resources, const Common::String &inv);

	/** Get all items. */
	bool getItems(const Common::Array<Item> *&items);

	/** Find a specific item. */
	const Item *findItem(const Common::String &name) const;

private:
	typedef Common::HashMap<Common::String, Sprite *, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> SpriteMap;

	Resources *_resources;
	Variables *_variables;
	Graphics  *_graphics;
	Cursors   *_cursors;

	SpriteMap _origSprites; ///< The original item sprites.
	SpriteMap _sprites;     ///< The item sprites adapted to the current palette.

	Common::Array<Item> _items; ///< All available items.

	uint32 _checkedLast; ///< Timestamp of when the item conditions were checked last.

	// Parsing helpers
	bool parse(DATFile &dat);
	bool parseLook(Item &item, ScriptChunk &lookScript);
	bool parseUse(Item &item, ScriptChunk &useScript);

	void resetSprites();
	void assignSprites();

	bool updateItems();
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_INVENTORY_H
