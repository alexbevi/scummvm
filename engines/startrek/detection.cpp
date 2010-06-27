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

#include "base/plugins.h"

#include "engines/advancedDetector.h"
#include "common/config-manager.h"
#include "common/file.h"

#include "startrek/startrek.h"

namespace StarTrek {

struct StarTrekGameDescription {
	ADGameDescription desc;

	uint8 gameType;
	uint32 features;
	uint16 version;
};

uint32 StarTrekEngine::getFeatures() const {
	return _gameDescription->features;
}

Common::Platform StarTrekEngine::getPlatform() const {
	return _gameDescription->desc.platform;
}

uint16 StarTrekEngine::getVersion() const {
	return _gameDescription->version;
}

uint8 StarTrekEngine::getGameType() {
	return _gameDescription->gameType;
}

Common::Language StarTrekEngine::getLanguage() {
	return _gameDescription->desc.language;
}

} // End of Namespace StarTrek

static const PlainGameDescriptor starTrekGames[] = {
	{"startrek", "Star Trek game"},
	{"st25", "Star Trek: 25th Anniversary"},
	{"stjr", "Star Trek: Judgment Rites"},
	{0, 0}
};


namespace StarTrek {

static const StarTrekGameDescription gameDescriptions[] = {
	{
		{
			"st25",
			"",
			AD_ENTRY1("data.001", "57040928a0f374281aa86ba4e7db8444"),
			Common::EN_ANY,
			Common::kPlatformPC,
			ADGF_NO_FLAGS,
			Common::GUIO_NONE
		},
		GType_ST25,
		0,
		0,
	},
	
	{
		{
			"st25",
			"",
			AD_ENTRY1("data.000", "f0918b6d096455ce2ae6dd5ef973292e"),
			Common::EN_ANY,
			Common::kPlatformAmiga,
			ADGF_NO_FLAGS,
			Common::GUIO_NONE
		},
		GType_ST25,
		0,
		0,
	},
	
	{
		{
			"st25",
			"",
			AD_ENTRY1("data.000", "70d0e374d5fa973e536dba0f42310672"),
			Common::DE_DEU,
			Common::kPlatformAmiga,
			ADGF_NO_FLAGS,
			Common::GUIO_NONE
		},
		GType_ST25,
		0,
		0,
	},
	
	{
		{
			"st25",
			"",
			AD_ENTRY1("data.000", "d0299af1385edd7c7612ed453e417dd8"),
			Common::FR_FRA,
			Common::kPlatformAmiga,
			ADGF_NO_FLAGS,
			Common::GUIO_NONE
		},
		GType_ST25,
		0,
		0,
	},
	
	{
		{
			"st25",
			"",
			AD_ENTRY1("Star Trek Data", "9b078190a09db7417c60e9d6b7233236"),
			Common::EN_ANY,
			Common::kPlatformMacintosh,
			ADGF_MACRESFORK,
			Common::GUIO_NONE
		},
		GType_ST25,
		0,
		0,
	},
	
	{
		{
			"st25",
			"Demo",
			AD_ENTRY1("data.001", "f68126e7e36ce6286c4c8575f8b594f5"),
			Common::EN_ANY,
			Common::kPlatformPC,
			ADGF_DEMO,
			Common::GUIO_NONE
		},
		GType_ST25,
		GF_DEMO,
		0,
	},
	
	{
		{
			"stjr",
			"",
			AD_ENTRY1("data.001", "1c8de3c02f69c07c582d59d3c29e4dd9"),
			Common::EN_ANY,
			Common::kPlatformPC,
			ADGF_NO_FLAGS,
			Common::GUIO_NONE
		},
		GType_STJR,
		0,
		0,
	},	
	
	{ AD_TABLE_END_MARKER, 0, 0, 0 }
};

} // End of namespace StarTrek

static const ADParams detectionParams = {
	// Pointer to ADGameDescription or its superset structure
	(const byte *)StarTrek::gameDescriptions,
	// Size of that superset structure
	sizeof(StarTrek::StarTrekGameDescription),
	// Number of bytes to compute MD5 sum for
	5000,
	// List of all engine targets
	starTrekGames,
	// Structure for autoupgrading obsolete targets
	0,
	// Name of single gameid (optional)
	"startrek",
	// List of files for file-based fallback detection (optional)
	0,
	// Flags
	0,
	// Additional GUI options (for every game)
	Common::GUIO_NONE,
	// Maximum directory depth
	1,
	// List of directory globs
	0
};

class StarTrekMetaEngine : public AdvancedMetaEngine {
public:
	StarTrekMetaEngine() : AdvancedMetaEngine(detectionParams) {}

	virtual const char *getName() const {
		return "Star Trek Engine";
	}

	virtual const char *getOriginalCopyright() const {
		return "Star Trek: 25th Anniversary, Star Trek: Judgment Rites (C) Interplay";
	}

	virtual bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const;
};

bool StarTrekMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	const StarTrek::StarTrekGameDescription *gd = (const StarTrek::StarTrekGameDescription *)desc;
	
	*engine = new StarTrek::StarTrekEngine(syst, gd);
	
	return (gd != 0);
}

#if PLUGIN_ENABLED_DYNAMIC(STARTREK)
	REGISTER_PLUGIN_DYNAMIC(STARTREK, PLUGIN_TYPE_ENGINE, StarTrekMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(STARTREK, PLUGIN_TYPE_ENGINE, StarTrekMetaEngine);
#endif

