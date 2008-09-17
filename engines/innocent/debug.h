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

#ifndef INNOCENT_DEBUG_H
#define INNOCENT_DEBUG_H

#include "config.h"

#include "common/rect.h"
#include "common/scummsys.h"

namespace Innocent {

class Inspectable {
public:
	virtual ~Inspectable() {};
	virtual const char *operator+() const = 0;
};

class StaticInspectable : public Inspectable {
public:
	virtual ~StaticInspectable() {}

	virtual const char *operator+() const { return _debugInfo; }

protected:
	#define DEBUG_INFO
	char _debugInfo[100];
};

template <typename T>
class NumericInspectable : public Inspectable {
public:
	virtual ~NumericInspectable() {}
	virtual const char *operator+() const {
		snprintf(_debugInfo, 10, "%d", T(*this));
		return _debugInfo;
	}
	virtual operator T() const = 0;
private:
	mutable char _debugInfo[10];
};

enum DebugLevel {
	kDebugLevelScript    = 1,
	kDebugLevelGraphics  = 2,
	kDebugLevelFlow		 = 4,
	kDebugLevelAnimation = 8,
	kDebugLevelValues    = 16,
	kDebugLevelFiles	 = 32,
	kDebugLevelEvents	 = 64,
	kDebugLevelMusic	 = 128,
	kDebugLevelActor	 = 256
};

const char *operator+(const Common::Rect &r);

}

#endif
