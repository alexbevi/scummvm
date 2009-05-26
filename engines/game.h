/* Residual - A 3D game interpreter
 *
 * Residual is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
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

#ifndef ENGINES_GAME_H
#define ENGINES_GAME_H

#include "common/str.h"
#include "common/array.h"
#include "common/hash-str.h"
#include "common/ptr.h"


/**
 * A simple structure used to map gameids (like "monkey", "sword1", ...) to
 * nice human readable and descriptive game titles (like "The Secret of Monkey Island").
 * This is a plain struct to make it possible to declare NULL-terminated C arrays
 * consisting of PlainGameDescriptors.
 */
struct PlainGameDescriptor {
	const char *gameid;
	const char *description;
};

/**
 * Given a list of PlainGameDescriptors, returns the first PlainGameDescriptor
 * matching the given gameid. If not match is found return 0.
 * The end of the list must marked by a PlainGameDescriptor with gameid equal to 0.
 */
const PlainGameDescriptor *findPlainGameDescriptor(const char *gameid, const PlainGameDescriptor *list);

/**
 * A hashmap describing details about a given game. In a sense this is a refined
 * version of PlainGameDescriptor, as it also contains a gameid and a description string.
 * But in addition, platform and language settings, as well as arbitrary other settings,
 * can be contained in a GameDescriptor.
 * This is an essential part of the glue between the game engines and the launcher code.
 */
class GameDescriptor : public Common::StringMap {
public:
	GameDescriptor();
	GameDescriptor(const PlainGameDescriptor &pgd);
	GameDescriptor(const Common::String &gameid,
	              const Common::String &description,
	              Common::Language language = Common::UNK_LANG,
	              Common::Platform platform = Common::kPlatformUnknown);

	/**
	 * Update the description string by appending (LANG/PLATFORM/EXTRA) to it.
	 */
	void updateDesc(const char *extra = 0);

	Common::String &gameid() { return getVal("gameid"); }
	Common::String &description() { return getVal("description"); }
	const Common::String &gameid() const { return getVal("gameid"); }
	const Common::String &description() const { return getVal("description"); }
	Common::Language language() const { return contains("language") ? Common::parseLanguage(getVal("language")) : Common::UNK_LANG; }
	Common::Platform platform() const { return contains("platform") ? Common::parsePlatform(getVal("platform")) : Common::kPlatformUnknown; }

	const Common::String &preferredtarget() const {
		return contains("preferredtarget") ? getVal("preferredtarget") : getVal("gameid");
	}
};

/** List of games. */
class GameList : public Common::Array<GameDescriptor> {
public:
	GameList() {}
	GameList(const GameList &list) : Common::Array<GameDescriptor>(list) {}
	GameList(const PlainGameDescriptor *g) {
		while (g->gameid) {
			push_back(GameDescriptor(g->gameid, g->description));
			g++;
		}
	}
};

/**
 * A hashmap describing details about a given save state.
 * TODO
 * Guaranteed to contain save_slot and description values.
 * Additional ideas: Playtime, creation date, thumbnail, ...
 */
class SaveStateDescriptor : public Common::StringMap {
protected:

public:
	SaveStateDescriptor() {
		setVal("save_slot", "-1");	// FIXME: default to 0 (first slot) or to -1 (invalid slot) ?
		setVal("description", "");
	}

	SaveStateDescriptor(int s, const Common::String &d) {
		char buf[16];
		sprintf(buf, "%d", s);
		setVal("save_slot", buf);
		setVal("description", d);
	}

	SaveStateDescriptor(const Common::String &s, const Common::String &d) {
		setVal("save_slot", s);
		setVal("description", d);
	}

	/** The saveslot id, as it would be passed to the "-x" command line switch. */
	Common::String &save_slot() { return getVal("save_slot"); }

	/** The saveslot id, as it would be passed to the "-x" command line switch (read-only variant). */
	const Common::String &save_slot() const { return getVal("save_slot"); }

	/** A human readable description of the save state. */
	Common::String &description() { return getVal("description"); }

	/** A human readable description of the save state (read-only variant). */
	const Common::String &description() const { return getVal("description"); }

	/** Optional entries only included when querying via MetaEngine::querySaveMetaInfo */

	/**
	 * Returns the value of a given key as boolean.
	 * It accepts 'true', 'yes' and '1' for true and
	 * 'false', 'no' and '0' for false.
	 * (FIXME:) On unknown value it errors out ScummVM.
	 * On unknown key it returns false as default.
	 */
	bool getBool(const Common::String &key) const;

	/**
	 * Sets the 'is_deletable' key, which indicates if the
	 * given savestate is safe for deletion.
	 */
	void setDeletableFlag(bool state);

	/**
	 * Sets the 'is_write_protected' key, which indicates if the
	 * given savestate can be overwritten or not
	 */
	void setWriteProtectedFlag(bool state);

	/**
	 * Sets the 'save_date' key properly, based on the given values.
	 */
	void setSaveDate(int year, int month, int day);

	/**
	 * Sets the 'save_time' key properly, based on the given values.
	 */
	void setSaveTime(int hour, int min);

	/**
	 * Sets the 'play_time' key properly, based on the given values.
	 */
	void setPlayTime(int hours, int minutes);
};

/** List of savestates. */
typedef Common::Array<SaveStateDescriptor> SaveStateList;

#endif
