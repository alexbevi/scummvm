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

#ifndef TITANIC_TT_STRING_H
#define TITANIC_TT_STRING_H

#include "titanic/support/string.h"

namespace Titanic {

class TTStringData {
private:
	CString _string;
public:
	int _referenceCount;
public:
	TTStringData() : _referenceCount(1) {}
	TTStringData(const char *str) : _string(str), _referenceCount(1) {}
	TTStringData(const CString &str) : _string(str), _referenceCount(1) {}
};

enum TTStringStatus { SS_VALID = 0, SS_5 = 5, SS_7 = 7 };

class TTString {
private:
	TTStringData *_data;
	TTStringStatus _status;
public:
	TTString();
	TTString(const char *str);
	TTString(const CString &str);
	TTString(TTString &str);
	virtual ~TTString();

	/**
	 * Returns true if the string is valid
	 */
	bool isValid() const;

	/**
	 * Get the status of the string
	 */
	TTStringStatus getStatus() const { return _status; }
};

} // End of namespace Titanic

#endif /* TITANIC_TT_STRING_H */
