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

#ifndef DARKSEED2_TALKLINE_H
#define DARKSEED2_TALKLINE_H

#include "common/str.h"

#include "engines/darkseed2/darkseed2.h"

namespace DarkSeed2 {

class Resources;
class Graphics;

class Resource;
class Sound;

class TalkLine {
public:
	TalkLine(const Resources &resources, const Common::String &talkName);
	~TalkLine();

	const Common::String &getName() const;
	void setName(const Common::String &name);

	bool hasWAV() const;
	bool hasTXT() const;

	const Resource &getWAV() const;
	const Common::String &getTXT() const;

private:
	const Resources *_resources;

	Common::String _resource;
	Common::String _name;

	Resource *_wav;
	Common::String _txt;
};

class TalkManager {
public:
	TalkManager(Sound &sound, Graphics &graphics);
	~TalkManager();

	bool talk(const TalkLine &talkLine);
	void endTalk();

	void updateStatus();

private:
	Sound *_sound;
	Graphics *_graphics;

	int _curTalk;
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_TALKLINE_H
