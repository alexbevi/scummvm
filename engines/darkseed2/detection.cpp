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

#include "darkseed2/darkseed2.h"
#include "darkseed2/saveload.h"
#include "graphics/surface.h"

namespace DarkSeed2 {

struct DS2GameDescription {
	ADGameDescription desc;
};

const char *DarkSeed2Engine::getGameId() const {
	return _gameDescription->desc.gameid;
}
Common::Language DarkSeed2Engine::getLanguage() const {
	return _gameDescription->desc.language;
}
Common::Platform DarkSeed2Engine::getPlatform() const {
	return _gameDescription->desc.platform;
}

}

using namespace Common;

static const PlainGameDescriptor darkseed2Games[] = {
	{"darkseed2", "Dark Seed II"},
	{0, 0}
};

namespace DarkSeed2 {

using Common::GUIO_NOSPEECH;
using Common::GUIO_NOSUBTITLES;
using Common::GUIO_NONE;

static const DS2GameDescription gameDescriptions[] = {
	{ // German version from the PC Joker. English speech, german text
		{
			"darkseed2",
			"",
			{
				{"gfile.hdr",    0, "454ab83dfb35a7232ee0eb635427f761", 210856},
				{"gl00_txt.000", 0, "e195b792c29e53717a6364b66721731f", 140771},
			},
			DE_DEU,
			kPlatformPC,
			ADGF_NO_FLAGS,
			GUIO_NONE
		},
	},
	{ // English version
		{
			"darkseed2",
			"",
			{
				{"gfile.hdr",    0, "454ab83dfb35a7232ee0eb635427f761", 210856},
				{"gl00_txt.000", 0, "0f1c8f78fa670e015115b9f2dcdcd4ae", 125377},
			},
			EN_ANY,
			kPlatformPC,
			ADGF_NO_FLAGS,
			GUIO_NONE
		},
	},
	{ // French version. English speech, french text
		{
			"darkseed2",
			"",
			{
				{"gfile.hdr",    0, "454ab83dfb35a7232ee0eb635427f761", 210856},
				{"gl00_txt.000", 0, "edbd13f748c306a4e61eb4ca2f41d3d8", 139687},
			},
			FR_FRA,
			kPlatformPC,
			ADGF_NO_FLAGS,
			GUIO_NONE
		},
	},
	{ // Japanese Sega Saturn version
		{
			"darkseed2",
			"",
			{
				{"initial.idx",  0, "92438db5d1b4b8946ec59ecfbd6107c0", 1284},
				{"initial.glu",  0, "558a62491c612a890a25991016ab3f81", 540300},
				{"conv0000.pgf", 0, "a82a02d0f825b54010938586b76b3019", 368}
			},
			JA_JPN,
			kPlatformUnknown, // kPlatformSaturn
			ADGF_NO_FLAGS,
			GUIO_NOSPEECH
		},
	},
	{ AD_TABLE_END_MARKER }
};

static const DS2GameDescription fallbackDescs[] = {
	{ // 0
		{
			"darkseed2",
			"unknown",
			AD_ENTRY1(0, 0),
			UNK_LANG,
			kPlatformPC,
			ADGF_NO_FLAGS,
			GUIO_NONE
		},
	},
	{ // 1
		{
			"darkseed2",
			"unknown",
			AD_ENTRY1(0, 0),
			UNK_LANG,
			kPlatformUnknown, // kPlatformSaturn
			ADGF_NO_FLAGS,
			GUIO_NONE
		},
	},
};

static const ADFileBasedFallback fileBased[] = {
	{ &fallbackDescs[ 0], { "gfile.hdr", "dark0001.exe", 0 } },
	{ &fallbackDescs[ 1], { "initial.idx", "initial.glu", "conv0000.pgf", 0 } },
	{ 0, { 0 } }
};

}

static const ADParams detectionParams = {
	// Pointer to ADGameDescription or its superset structure
	(const byte *)DarkSeed2::gameDescriptions,
	// Size of that superset structure
	sizeof(DarkSeed2::DS2GameDescription),
	// Number of bytes to compute MD5 sum for
	10000,
	// List of all engine targets
	darkseed2Games,
	// Structure for autoupgrading obsolete targets
	0,
	// Name of single gameid (optional)
	"darkseed2",
	// List of files for file-based fallback detection (optional)
	DarkSeed2::fileBased,
	// Flags
	0,
	// Additional GUI options (for every game}
	Common::GUIO_NOLAUNCHLOAD
};

class DarkSeed2MetaEngine : public AdvancedMetaEngine {
public:
	DarkSeed2MetaEngine() : AdvancedMetaEngine(detectionParams) {}

	virtual const char *getName() const {
		return "Dark Seed II Engine";
	}

	virtual const char *getOriginalCopyright() const {
		return "Dark Seed II (C) Cyberdreams, Inc., Destiny Software Productions, Inc.";
	}

	virtual bool hasFeature(MetaEngineFeature f) const;
	virtual bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const;

	SaveStateList listSaves(const char *target) const;
	int getMaximumSaveSlot() const;
	void removeSaveState(const char *target, int slot) const;
	SaveStateDescriptor querySaveMetaInfos(const char *target, int slot) const;

};

bool DarkSeed2MetaEngine::hasFeature(MetaEngineFeature f) const {
	return (f == kSupportsListSaves           ) ||
	       (f == kSupportsLoadingDuringStartup) ||
	       (f == kSupportsDeleteSave          ) ||
	       (f == kSavesSupportMetaInfo        ) ||
	       (f == kSavesSupportThumbnail       ) ||
	       (f == kSavesSupportCreationDate    ) ||
	       (f == kSavesSupportPlayTime        );
	return false;
}

bool DarkSeed2::DarkSeed2Engine::hasFeature(EngineFeature f) const {
	return (f == kSupportsRTL                 ) ||
	       (f == kSupportsLoadingDuringRuntime) ||
	       (f == kSupportsSavingDuringRuntime ) ||
	       (f == kSupportsSubtitleOptions     );
}

SaveStateList DarkSeed2MetaEngine::listSaves(const char *target) const {
	SaveStateList list;
	if (!DarkSeed2::SaveLoad::getStates(list, target))
		return SaveStateList();

	return list;
}

int DarkSeed2MetaEngine::getMaximumSaveSlot() const {
	return DarkSeed2::SaveLoad::kMaxSlot;
}

void DarkSeed2MetaEngine::removeSaveState(const char *target, int slot) const {
	DarkSeed2::SaveLoad::removeSave(target, slot);
}

SaveStateDescriptor DarkSeed2MetaEngine::querySaveMetaInfos(const char *target, int slot) const {
	SaveStateDescriptor state;
	if (!DarkSeed2::SaveLoad::getState(state, target, slot))
		return SaveStateDescriptor();

	return state;
}

bool DarkSeed2MetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	const DarkSeed2::DS2GameDescription *gd = (const DarkSeed2::DS2GameDescription *)desc;
	if (gd) {
		*engine = new DarkSeed2::DarkSeed2Engine(syst, gd);
		((DarkSeed2::DarkSeed2Engine *)*engine)->initGame(gd);
	}
	return gd != 0;
}

#if PLUGIN_ENABLED_DYNAMIC(DARKSEED2)
	REGISTER_PLUGIN_DYNAMIC(DARKSEED2, PLUGIN_TYPE_ENGINE, DarkSeed2MetaEngine);
#else
	REGISTER_PLUGIN_STATIC(DARKSEED2, PLUGIN_TYPE_ENGINE, DarkSeed2MetaEngine);
#endif

namespace DarkSeed2 {

void DarkSeed2Engine::initGame(const DS2GameDescription *gd) {
}

} // End of namespace DarkSeed2
