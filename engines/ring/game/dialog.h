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

#ifndef RING_DIALOG_H
#define RING_DIALOG_H

#include "ring/shared.h"

namespace Ring {

class Dialog : public BaseObject {
public:
	Dialog();
	~Dialog();

	void setTicks();
	void hide();

private:
	uint32 _field_4;
	uint32 _ticks;
	uint32 _field_C;
	uint32 _field_10;
	uint32 _field_14;
	byte _field_18;
};

class DialogHandler {
public:
	DialogHandler();
	~DialogHandler();

	void init(uint32 a1, Color subtitlesColor, Color subtitlesBackgroundColor, uint32 a8, uint32 a9);

	void addDialog(Dialog *dialog);
	void removeDialog(Id id);

	bool isPlaying(Id id);
	bool isPlaying();

	// Accessors
	void setSubtitleColor(Color color) { _subtitleColor = color; }
	void setSubtitleBackgroundColor(Color color) { _subtitlesBackgroundColor = color; }

	void setField28(uint32 val) { _field_28 = val; }

private:
	uint32 _field_0;
	Color _subtitleColor;
	Color _subtitlesBackgroundColor;
	uint32 _field_1C;
	uint32 _field_20;
	AssociativeArray<Dialog *> _dialogs;
	uint32 _field_28;
};

} // End of namespace Ring

#endif // RING_DIALOG_H
