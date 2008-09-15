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

#ifndef KOM_CHARACTER_H
#define KOM_CHARACTER_H

#include "common/list.h"
#include "engines/engine.h"

#include "kom/database.h"
#include "kom/actor.h"

namespace Kom {

struct Character {
public:
	Character() :
		_mode(0), _modeCount(0), _isBusy(false), _isAlive(true), _isVisible(true),
		_spellMode(0), _gold(0),
		_actorId(-1), _screenH(0), _screenDH(0), _offset0c(0), _offset10(0),
		_offset14(262144), _offset1c(0), _offset20(262144), _offset28(0), _ratioX(262144),
		_ratioY(262144), _relativeSpeed(1024), _direction(0), _lastDirection(2),
		_stoppedTime(0), _spriteName(0),
		_scopeInUse(-1), _scopeWanted(8), _loadedScopeXtend(-1), _priority(0), _fightPartner(-1),
		_spriteCutState(0), _spriteScope(0), _spriteTimer(0), _spriteBox(0), _somethingX(0),
		_somethingY(0) {}

	int _id;

	// character
	char _name[7];
	int _xtend;
	int _data2;
	char _desc[50];
	int _proc;
	int _locationId;
	int _box;
	int _data5;
	int _data6;
	int _data7;
	int _data8;
	int _data9;
	int _hitPoints;
	int _hitPointsMax;
	uint16 _mode;
	uint16 _modeCount;
	bool _isBusy;
	bool _isAlive;
	bool _isVisible;
	uint8 _spellMode;
	int16 _spellDuration;
	int _strength;
	int _defense;
	int _oldHitPoints;
	int _oldStrength;
	int _oldDefense;
	int _damageMin;
	int _damageMax;
	int _data14; // spell speed - unused
	int _data15; // spell time - unused
	int _data16;
	int _spellPoints;
	int _spellPointsMax;
	int16 _destLoc;
	int16 _destBox;
	int _gold;
	Common::List<int> _inventory;
	Common::List<int> _weapons;
	Common::List<int> _spells;

	// scope
	Scope _scopes[18];
	int16 _actorId;
	int16 _screenX;
	int16 _screenY;
	int16 _screenH;
	int16 _screenDH;
	int32 _offset0c;
	int32 _offset10;
	int32 _offset14;
	int32 _offset1c;
	int32 _offset20;
	int32 _offset28;
	int32 _ratioX;
	int32 _ratioY;
	int16 _priority;
	int32 _lastLocation;
	int32 _lastBox;
	int32 _gotoBox;
	int16 _gotoX;
	int16 _gotoY;
	int16 _gotoLoc;
	uint16 _walkSpeed;
	uint16 _relativeSpeed;
	uint16 _animSpeed;
	uint16 _direction;
	uint16 _lastDirection;
	bool _stopped;
	uint16 _stoppedTime;
	uint16 _timeout;
	int32 _start3;
	int32 _start3Prev;
	int32 _start3PrevPrev;
	int32 _start4;
	int32 _start4Prev;
	int32 _start4PrevPrev;
	int32 _somethingX;
	int32 _somethingY;
	int32 _start5;
	int32 _start5Prev;
	int32 _start5PrevPrev;
	uint32 _offset78;
	int16 _fightPartner;
	uint8 _spriteCutState;
	uint16 _spriteScope;
	uint16 _spriteTimer;
	uint16 _spriteBox;
	uint16 _sprite8c;
	const char *_spriteName;
	int16 _scopeInUse;
	int16 _scopeWanted;
	int16 _loadedScopeXtend;

	void moveChar(bool param);
	void moveCharOther();
	void stopChar();
	void setScope(int16 scope);
	void unsetSpell();

	friend class Database;

private:
	void setScopeX(int16 scope);
	void setAnimation(int16 anim, int16 scope);

protected:
	static KomEngine *_vm;

};
} // End of namespace Kom

#endif
