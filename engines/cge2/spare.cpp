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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

/*
 * This code is based on original Sfinx source code
 * Copyright (c) 1994-1997 Janus B. Wisniewski and L.K. Avalon
 */

#include "cge2/spare.h"

namespace CGE2 {

void Spare::sync(Common::Serializer &s) {
	if (s.isSaving()) {
		int size = 0;
		for (uint i = 0; i < _container.size(); i++)
			if (_container[i]->_ref >= 141)
				size++;
		s.syncAsSint16LE(size);

		for (uint i = 0; i < _container.size(); i++)
			if (_container[i]->_ref >= 141)
				_container[i]->sync(s);
	} else {
		int size;
		s.syncAsSint16LE(size);

		for (int i = 0; i < size; i++) {
			Sprite *sprite = new Sprite(_vm);
			sprite->sync(s);

			// In case the reference of the sprite is changed comapred to CGE.INI
			// TODO: Rework the whole loading so it doesn't load every sprite from CGE.INI
			// and then update them, but load everything from file, so this check isn't
			// needed anymore. To do that, I also have to save/load the toolbar's sprites too.
			Sprite *loc = locate(sprite->_file);
			if (loc->_ref != sprite->_ref) {
				loc->contract();
				*loc = *sprite;
			} else
				update(sprite);
		}
	}
}

void Spare::clear() {
	for (uint i = 0; i < _container.size(); i++)
		delete _container[i];

	_container.clear();
}

Sprite *Spare::locate(int ref) {
	for (uint i = 0; i < _container.size(); ++i) {
		if (_container[i]->_ref == ref)
			return _container[i];
	}
	return nullptr;
}

Sprite *Spare::locate(char *file) {
	for (uint i = 0; i < _container.size(); ++i) {
		if (strcmp(_container[i]->_file, file) == 0)
			return _container[i];
	}
	return nullptr;
}

Sprite *Spare::take(int ref) {
	Sprite *spr = nullptr;
	if ((spr = locate(ref)) != nullptr) {
		for (uint i = 0; i < _container.size(); ++i) {
			if (spr == _container[i]) {
				_container.remove_at(i);
				break;
			}
		}
	}
	return spr;
}

void Spare::takeScene(int cav) {
	int bakRef = cav << 8;
	Common::Array<Sprite*> tempCont = _container;
	for (uint i = 0; i < tempCont.size(); ++i) {
		Sprite *spr = tempCont[i];
		int c = spr->_scene;
		if ((c == _vm->_now || c == 0) && spr->_ref != bakRef) {
			spr = locate(spr->_ref);
			_vm->_vga->_showQ->insert(spr);
		}
	}
}

void Spare::store(Sprite *spr) {
	_container.push_back(spr);
}

void Spare::update(Sprite *spr) {
	Sprite *sp = locate(spr->_ref);
	if (sp == nullptr)
		store(spr);
	else {
		sp->contract();
		*sp = *spr;
	}
}

void Spare::dispose(Sprite *spr) {
	if (spr) {
		_vm->_vga->_showQ->remove(spr);
		update(spr->contract());
	}
}

void Spare::dispose(int ref) {
	dispose(_vm->_vga->_showQ->locate(ref));
}

void Spare::dispose() {
	for (uint i = 0; i < _container.size(); ++i) {
		if (_container[i]->_ref > 255)
			dispose(_container[i]);
	}
}

} // End of namespace CGE2
