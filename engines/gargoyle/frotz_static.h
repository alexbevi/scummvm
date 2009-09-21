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

#ifndef FROTZ_STATIC_H
#define FROTZ_STATIC_H

#include "common/scummsys.h"
#include "graphics/font.h"
#include "gargoyle/frotz_interp.h"

namespace Frotz {

extern const char *err_messages[];

struct GameVersionRecord {
	enum story story_id;
	zword release;
	zbyte serial[7];
};
extern GameVersionRecord game_records[];

struct GameMapperRecord {
	enum story story_id;
	int pic;
	int pic1;
	int pic2;
};
extern GameMapperRecord game_mapper[];

extern zbyte lh_repeats[];

extern zword zscii_to_latin1[];

extern const unsigned char tolower_basic_latin[0x100];
extern const unsigned char tolower_latin_extended_a[0x80];
extern const unsigned char tolower_greek[0x50];
extern const unsigned char tolower_cyrillic[0x60];

extern const Graphics::FontDesc infocomGfxFont;

enum SystemStringId {
	IDS_BLORB_GLULX, IDS_BLORB_NOEXEC, IDS_MORE, IDS_HIT_KEY_EXIT, IDS_TITLE, 
	IDS_FATAL, IDS_ZCODE_FILTER, IDS_ZCODE_TITLE, IDS_SAVE_FILTER,
	IDS_SAVE_TITLE, IDS_RESTORE_TITLE, IDS_SCRIPT_FILTER, IDS_SCRIPT_TITLE,
	IDS_RECORD_FILTER, IDS_RECORD_TITLE, IDS_PLAYBACK_TITLE
};

extern const char *sysString(Common::Language lang, SystemStringId stringId);

} // End of namespace Frotz

#endif

