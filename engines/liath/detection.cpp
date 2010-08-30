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

#include "liath/liath.h"

namespace Liath {

static const char *directoryGlobs[] = {
	"Heros",
	0
};

static const PlainGameDescriptor liathGames[] = {
	// Games
	{"liath", "WordSpiral - Liath"},
	{0, 0}
};

static const ADGameDescription gameDescriptions[] = {

	// WordSpiral - Liath (Original)
	{
		"liath",
			"",
		{
			{"liath.exe", 0, "d376cb43dde70019ea01a4358198f085", 300731},
			{"actions.mul", 0, "", 0},
		},
		Common::EN_ANY,
		Common::kPlatformWindows,
		ADGF_NO_FLAGS,
		Common::GUIO_NONE
	},

	// WordSpiral - Liath (Patched 1.0)
	{
		"liath",
			"",
		{
			{"liath.exe", 0, "d376cb43dde70019ea01a4358198f085", 300731},
			{"actions.mul", 0, "d5b7666e3a36728db6ca3688b305bee6", 1009189},
		},
		Common::EN_ANY,
		Common::kPlatformWindows,
		ADGF_NO_FLAGS,
		Common::GUIO_NONE
	},

	// WordSpiral - Liath (Patched 2.0)
	{
		"liath",
			"",
		{
			{"liath.exe", 0, "d376cb43dde70019ea01a4358198f085", 300731},
			{"actions.mul", 0, "1f03d34988dc3c633f0c6a5d6b650b40", 1009189},
		},
		Common::EN_ANY,
		Common::kPlatformWindows,
		ADGF_NO_FLAGS,
		Common::GUIO_NONE
	},

	// WordSpiral - Liath (Patched 3.0)
	{
		"liath",
		"",
		{
			{"liath.exe", 0, "d376cb43dde70019ea01a4358198f085", 300731},
			{"actions.mul", 0, "5ab96c427dab9621052db81f65723ab2", 1009546},
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
	liathGames,
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


class LiathMetaEngine : public AdvancedMetaEngine {
public:
	LiathMetaEngine() : AdvancedMetaEngine(detectionParams) {}

	const char *getName() const {
		return "Liath Engine";
	}

	const char *getOriginalCopyright() const {
		return "Liath Engine / DS:SI (C) 1998 AMBER Company";
	}

	bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *gd) const;
};

bool LiathMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *gd) const {
	if (gd) {
		*engine = new LiathEngine(syst, (const ADGameDescription *)gd);
	}
	return gd != 0;
}

} // End of namespace Liath

#if PLUGIN_ENABLED_DYNAMIC(LIATH)
	REGISTER_PLUGIN_DYNAMIC(LIATH, PLUGIN_TYPE_ENGINE, Liath::LiathMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(LIATH, PLUGIN_TYPE_ENGINE, Liath::LiathMetaEngine);
#endif
