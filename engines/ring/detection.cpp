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
 * $Id: detection.cpp 291 2010-01-03 08:58:57Z julien.templier $
 *
 */

#include "ring/ring.h"

namespace Ring {

static const char *directoryGlobs[] = {
	"DATA",
	0
};

static const PlainGameDescriptor ringGames[] = {
	// Games
	{"ring", "The Ring: The Legend of the Nibelungen"},
	{0, 0}
};

static const ADGameDescription gameDescriptions[] = {

	// The Ring: The Legend of the Nibelungen (Windows DVD)
	{
		"liath",
			"",
		{
			{"RING.EXE", 0, "", 0},
			{"AS.AT2", 0, "", 0},
		},
		Common::EN_ANY,
		Common::kPlatformWindows,
		ADGF_NO_FLAGS,
		Common::GUIO_NONE
	},
	AD_TABLE_END_MARKER
};

static const ADParams detectionParams = {
	// Pointer to ADGameDescription or its superset structure
	(const byte *)gameDescriptions,
	// Size of that superset structure
	sizeof(ADGameDescription),
	// Number of bytes to compute MD5 sum for
	5000,
	// List of all engine targets
	ringGames,
	// Structure for autoupgrading obsolete targets
	0,
	// Name of single gameid (optional)
	"liath",
	// List of files for file-based fallback detection (optional)
	0,
	// Flags
	0,
	// Additional GUI options (for every game}
	Common::GUIO_NOSUBTITLES | Common::GUIO_NOSFX,
	// Maximum directory depth
	2,
	// List of directory globs
	directoryGlobs
};


class RingMetaEngine : public AdvancedMetaEngine {
public:
	RingMetaEngine() : AdvancedMetaEngine(detectionParams) {}

	const char *getName() const {
		return "Ring Engine";
	}

	const char *getOriginalCopyright() const {
		return "Ring Engine (C) 1999 Arxel Tribe";
	}

	bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *gd) const;
};

bool RingMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *gd) const {
	if (gd) {
		*engine = new RingEngine(syst, (const ADGameDescription *)gd);
	}
	return gd != 0;
}

} // End of namespace Ring

#if PLUGIN_ENABLED_DYNAMIC(RING)
	REGISTER_PLUGIN_DYNAMIC(RING, PLUGIN_TYPE_ENGINE, Ring::RingMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(RING, PLUGIN_TYPE_ENGINE, Ring::RingMetaEngine);
#endif
