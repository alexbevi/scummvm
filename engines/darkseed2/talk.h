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
#include "engines/darkseed2/versionformats.h"

namespace DarkSeed2 {

class VersionFormats;
class Resources;
class Graphics;

class Options;
class Resource;
class Sound;
class FontManager;

class TextLine;

/** A talk line containing it's text, sprite and sound. */
class TalkLine {
public:
	TalkLine(Resources &resources, const Common::String &talkName);
	~TalkLine();

	/** Get the resource's name. */
	const Common::String &getResourceName() const;

	/** Get the line's name. */
	const Common::String &getName() const;
	/** Set the line's name. */
	void setName(const Common::String &name);

	/** Get the line's speaker. */
	const TextLine *getSpeaker() const;
	/** Get the line's speaker. */
	uint8 getSpeakerNum() const;
	/** Set the line's speaker. */
	void setSpeaker(uint8 speakerNum, const TextLine &speaker);

	/** Has this line a WAV sound? */
	bool hasWAV() const;
	/** Has this line a TXT text? */
	bool hasTXT() const;

	/** Get the line's WAV. */
	const Resource &getWAV() const;
	/** Get the line's TXT. */
	const TextLine &getTXT() const;

	SoundType getSoundType() const;

private:
	Resources *_resources;

	Common::String _resource;   ///< The line's resource name.
	Common::String _name;       ///< The line's name.
	TextLine      *_speaker;    ///< The line's speaker.
	uint8          _speakerNum; ///< The line's speaker's number.

	Resource *_wav; ///< The WAV.
	TextLine *_txt; ///< The TXT.
};

/** The talk manager. */
class TalkManager {
public:
	TalkManager(const VersionFormats &versionFormats, Sound &sound,
			Graphics &graphics, const FontManager &fontManager);
	~TalkManager();

	/** Speak the given line. */
	bool talk(const TalkLine &talkLine);
	/** Speak the given line. */
	bool talk(Resources &resources, const Common::String &talkName);
	/** End talking. */
	void endTalk();

	/** Return the sound ID of the currently playing line. */
	int getSoundID() const;

	/** Is someone currently talking? */
	bool isTalking() const;

	/** Apply subtitle settings. */
	void syncSettings(const Options &options);

	/** Check for status changes. */
	void updateStatus();

private:
	const VersionFormats *_versionFormats;

	Sound    *_sound;
	Graphics *_graphics;

	const FontManager *_fontMan;

	/** The current mananged talk line. */
	TalkLine *_curTalkLine;

	int _curTalk; ///< The current talk ID.

	bool _txtEnabled; ///< Should the TXT displayed?

	int _waitTextLength; ///< Duration of waiting after the sound has finished.
	int _waitTextUntil;  ///< Time to wait until this line is considered finished.

	bool talkInternal(const TalkLine &talkLine);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_TALKLINE_H
