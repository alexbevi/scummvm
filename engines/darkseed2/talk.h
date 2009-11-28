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

/** A talk line containing it's text, sprite and sound. */
class TalkLine {
public:
	TalkLine(const Resources &resources, const Common::String &talkName);
	~TalkLine();

	/** Get the resource's name. */
	const Common::String &getResourceName() const;

	/** Get the line's name. */
	const Common::String &getName() const;
	/** Set the line's name. */
	void setName(const Common::String &name);

	/** Get the line's speaker. */
	const Common::String &getSpeaker() const;
	/** Set the line's speaker. */
	void setSpeaker(const Common::String &speaker);

	/** Has this line a WAV sound? */
	bool hasWAV() const;
	/** Has this line a TXT text? */
	bool hasTXT() const;

	/** Get the line's WAV. */
	const Resource &getWAV() const;
	/** Get the line's TXT. */
	const Common::String &getTXT() const;

private:
	const Resources *_resources;

	Common::String _resource; ///< The line's resource name.
	Common::String _name;     ///< The line's name.
	Common::String _speaker;  ///< The line's speaker.

	Resource *_wav;      ///< The WAV.
	Common::String _txt; ///< The TXT.
};

/** The talk manager. */
class TalkManager {
public:
	TalkManager(Sound &sound, Graphics &graphics);
	~TalkManager();

	/** Speak the given line. */
	bool talk(const TalkLine &talkLine);
	/** End talking. */
	void endTalk();

	/** Is someone currently talking? */
	bool isTalking() const;

	/** Check for status changes. */
	void updateStatus();

private:
	Sound *_sound;
	Graphics *_graphics;

	int _curTalk; ///< The current talk ID.
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_TALKLINE_H
