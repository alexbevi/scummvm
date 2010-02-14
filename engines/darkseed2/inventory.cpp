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

#include "engines/darkseed2/inventory.h"
#include "engines/darkseed2/resources.h"
#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/graphics.h"
#include "engines/darkseed2/datfile.h"
#include "engines/darkseed2/sprite.h"
#include "engines/darkseed2/script.h"

namespace DarkSeed2 {

Inventory::Inventory(Resources &resources, Variables &variables,
		ScriptRegister &scriptRegister, Graphics &graphics, Cursors &cursors) :
	ObjectContainer(variables, scriptRegister) {

	_resources = &resources;
	_variables = &variables;
	_graphics  = &graphics;
	_cursors   = &cursors;

	_checkedLast = 0;
}

Inventory::~Inventory() {
	clear();
}

void Inventory::clear() {
	ObjectContainer::clear();

	for (SpriteMap::iterator sprite = _origSprites.begin(); sprite != _origSprites.end(); ++sprite)
		delete sprite->_value;
	_origSprites.clear();

	for (SpriteMap::iterator sprite = _sprites.begin(); sprite != _sprites.end(); ++sprite)
		delete sprite->_value;
	_sprites.clear();

	_items.clear();
}

void Inventory::newPalette() {
	resetSprites();
	assignSprites();
}

void Inventory::resetSprites() {
	for (SpriteMap::iterator it = _sprites.begin(); it != _sprites.end(); ++it)
		delete it->_value;
	_sprites.clear();

	for (SpriteMap::iterator it = _origSprites.begin(); it != _origSprites.end(); ++it) {
		Sprite *&sprite = _sprites[it->_key];

		sprite = new Sprite(*it->_value);

		_graphics->mergePalette(*sprite);
	}
}

void Inventory::assignSprites() {
	// Go through all looks in all objects and refresh the sprite pointers
	for (Common::Array<Item>::iterator item = _items.begin(); item != _items.end(); ++item) {
		for (Common::Array<ItemLook>::iterator look = item->looks.begin(); look != item->looks.end(); ++look) {
			look->sprite = 0;

			if (_sprites.contains(look->spriteName))
				look->sprite = _sprites.getVal(look->spriteName);
		}
	}
}

bool Inventory::parse(DATFile &dat) {
	clear();

	if (!ObjectContainer::parse(dat))
		return false;

	Common::Array<Object> &objects = getObjects();

	_items.resize(objects.size());
	for (uint i = 0; i < objects.size(); i++) {
		Object &object = objects[i];
		Item   &item   = _items[i];

		item.name = object.getName();

		// Parse looks
		Common::List<ScriptChunk *> &lookScripts = object.getScripts(kObjectVerbLook);
		for (Common::List<ScriptChunk *>::iterator it = lookScripts.begin(); it != lookScripts.end(); ++it)
			if (!parseLook(item, **it))
				return false;

		// Parse uses
		Common::List<ScriptChunk *> &useScripts  = object.getScripts(kObjectVerbUse);
		for (Common::List<ScriptChunk *>::iterator it = useScripts.begin(); it != useScripts.end(); ++it)
			if (!parseUse(item, **it))
				return false;

		// Load sprites
		for (Common::Array<ItemLook>::iterator it = item.looks.begin(); it != item.looks.end(); ++it) {
			if (_origSprites.contains(it->spriteName))
				// Sprite already loaded
				continue;

			Sprite *sprite = new Sprite;

			if (!sprite->loadFromBMP(*_resources, it->spriteName)) {
				delete sprite;
				return false;
			}

			_origSprites[it->spriteName] = sprite;
		}

		// Load cursors
		for (Common::Array<ItemUse>::iterator it = item.uses.begin(); it != item.uses.end(); ++it) {
			it->cursor = _cursors->getCursor(it->cursorName);

			if (!it->cursor) {
				warning("Inventory::parse(): Cursor \"%s\" does not exist", it->cursorName.c_str());
				return false;
			}
		}
	}

	resetSprites();
	assignSprites();

	_checkedLast = 0;

	updateItems();

	return true;
}

bool Inventory::parseLook(Item &item, ScriptChunk &lookScript) {
	ItemLook look;

	look.conditions = lookScript.getConditions();

	const Common::List<ScriptChunk::Action> &actions = lookScript.getActions();
	for (Common::List<ScriptChunk::Action>::const_iterator it = actions.begin(); it != actions.end(); ++it) {
		if (     it->action == kScriptActionCursor)
			look.spriteName = it->arguments;
		else if (it->action == kScriptActionText)
			look.text = it->arguments;
	}

	item.looks.push_back(look);

	return true;
}

bool Inventory::parseUse(Item &item, ScriptChunk &useScript) {
	ItemUse use;

	use.conditions = useScript.getConditions();

	const Common::List<ScriptChunk::Action> &actions = useScript.getActions();
	for (Common::List<ScriptChunk::Action>::const_iterator it = actions.begin(); it != actions.end(); ++it) {
		if (     it->action == kScriptActionCursor)
			use.cursorName = it->arguments;
		else if (it->action == kScriptActionChange)
			use.changes.push_back(it->arguments);
	}

	item.uses.push_back(use);

	return true;
}

bool Inventory::parse(Resources &resources, const Common::String &inv) {
	Common::String datFile = Resources::addExtension(inv, "DAT");
	if (!resources.hasResource(datFile))
		return false;

	Resource *resInv = resources.getResource(datFile);

	DATFile invParser(*resInv);

	bool result = parse(invParser);

	delete resInv;

	return result;
}

bool Inventory::getItems(const Common::Array<Item> *&items) {
	bool changed = updateItems();

	items = &_items;

	return changed;
}

const Inventory::Item *Inventory::findItem(const Common::String &name) const {
	for (Common::Array<Item>::const_iterator item = _items.begin(); item != _items.end(); ++item)
		if (item->name == name)
			return &*item;

	return 0;
}

bool Inventory::updateItems() {
	uint32 changedLast = _variables->getLastChanged();
	if (changedLast <= _checkedLast)
		// Nothing changed
		return false;

	bool changed = false;

	_checkedLast = changedLast;

	for (Common::Array<Item>::iterator item = _items.begin(); item != _items.end(); ++item) {
		// Search for the new active look

		ItemLook *curLook = item->curLook;

		item->curLook = 0;
		for (Common::Array<ItemLook>::iterator look = item->looks.begin(); look != item->looks.end(); ++look) {
			if (_variables->evalCondition(look->conditions)) {
				item->curLook = &*look;
				break;
			}
		}

		if (item->curLook != curLook)
			// It changed
			changed = true;

		// Search for the new active use

		ItemUse *curUse = item->curUse;

		item->curUse = 0;
		for (Common::Array<ItemUse>::iterator use = item->uses.begin(); use != item->uses.end(); ++use) {
			if (_variables->evalCondition(use->conditions)) {
				item->curUse = &*use;
				break;
			}
		}

		if (item->curUse != curUse)
			// It changed
			changed = true;
	}

	return changed;
}

} // End of namespace DarkSeed2
