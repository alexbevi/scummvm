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
 */

#include "ring/ring.h"
#include "ring/base/saveload.h"

namespace Ring {

static const char *directoryGlobs[] = {
	"DATA",
	0
};

static const PlainGameDescriptor ringGames[] = {
	// Games
	{"pilgrim", "Pilgrim: Faith as a Weapon"},
	{"ring", "The Ring: The Legend of the Nibelungen"},
	{"faust", "Faust: Seven Games of the Soul"},
	{"pompeii", "TimeScape: Journey to Pompeii"},
	{"pilgrim2", "Pilgrim 2: The Legend of the Prophet and the Assassin"},
	{"pilgrim3", "Pilgrim 3: The Secrets of Alamut"},
	{"jerusalem", "Jerusalem: The Three Roads to the Holy Land"},
	{0, 0}
};

static const RingGameDescription gameDescriptions[] = {

	/////////////////////////////////////////////////////////////////////////////////////////
	// Pilgrim: Faith as a Weapon (Windows CD - 2 disks)
#if 0
	{
		{
			"pilgrim",
			"",
			{
				{"", 0, "", 0},
				{"", 0, "", 0},
			},
			Common::UNK_LANG,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			Common::GUIO_NONE
		},
		GameTypePilgrim,
		LANG_ENG,
	},
#endif

	/////////////////////////////////////////////////////////////////////////////////////////
	// The Ring: The Legend of the Nibelungen (Windows DVD - 1 game disk, 1 video disk)
	{
		{
			"ring",
			"",
			{
				{"RING.EXE", 0, "10e21ce9cf937c56c5891113ac1cfcc2", 618496},
				{"AS.AT2", 0, "5f65ee721fdf50bc074dd25bb28592fb", 1533273},
			},
			Common::UNK_LANG,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			Common::GUIO_NONE
		},
		GameTypeRing,
		LANG_ENG | LANG_FRA | LANG_GER | LANG_GRE | LANG_HEB | LANG_HOL | LANG_ITA | LANG_SLO | LANG_SPA | LANG_SWE,
	},

	/////////////////////////////////////////////////////////////////////////////////////////
	// Faust: Seven Games of the Soul (Windows DVD - 2 disks)
	{
		{
			"faust",
			"",
			{
				{"FAUST.EXE", 0, "61dd0a190da5f339b52827a915d2b0eb", 843776},
				{"A01S01.AT3", 0, "bfa5d20991e568333229461cac05db20", 190573},
			},
			Common::UNK_LANG,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			Common::GUIO_NONE
		},
		GameTypeFaust,
		LANG_ENG | LANG_FRA | LANG_GER | LANG_HOL | LANG_ITA | LANG_SPA | LANG_SWE,
	},

	/////////////////////////////////////////////////////////////////////////////////////////
	// TimeScape: Journey to Pompeii (Windows CD - 2 disks)
	{
		{
			"pompeii",
			"",
			{
				{"POMPEI.EXE", 0, "bd19e3a5cfdf654917c79f99a3fb5e8c", 733184},
				{"SY.AT3", 0, "b56c1d7872cfb39da16ce4e290df8a3c", 19496860},
			},
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			Common::GUIO_NONE
		},
		GameTypePompeii,
		LANG_ENG,
	},

	/////////////////////////////////////////////////////////////////////////////////////////
	// Pilgrim 2: The Legend of the Prophet and the Assassin (Windows CD - 2 disks)
	{
		{
			"pilgrim2",
			"",
			{
				{"Legend.exe", 0, "169ea41f544cfecc37b1549ecd7a706b", 1112195},
				{"sy.at3", 0, "1b7bd7cd45930beb894e6f9a29b90cc4", 8506050},
			},
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			Common::GUIO_NONE
		},
		GameTypePilgrim2,
		LANG_ENG,
	},

	/////////////////////////////////////////////////////////////////////////////////////////
	// Pilgrim 3: The Secrets of Alamut (Windows CD - 2 disks)
	{
		{
			"pilgrim3",
			"",
			{
				{"Legend.exe", 0, "985e2a88576fbe008b0aedc8373766fd", 856064},
				{"sy.at3", 0, "b7084371c0adb1a84e708843130514d8", 10381851},
			},
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			Common::GUIO_NONE
		},
		GameTypePilgrim3,
		LANG_ENG,
	},

	/////////////////////////////////////////////////////////////////////////////////////////
	// Jerusalem: The Three Roads to the Holy Land (Windows CD - 2 disks)
	{
		{
			"jerusalem",
			"",
			{
				{"Jerusalem.exe", 0, "f85c88d05799d5238828a794178839d9", 811008},
				{"sy.at3", 0, "75689a89e7edc959e7fe86d71895222c", 14603762},
			},
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			Common::GUIO_NONE
		},
		GameTypeJerusalem,
		LANG_ENG,
	},

	{AD_TABLE_END_MARKER, 0, 0}
};


class RingMetaEngine : public AdvancedMetaEngine {
public:
	RingMetaEngine() : AdvancedMetaEngine(gameDescriptions, sizeof(Ring::RingGameDescription), ringGames) {
		_singleid = "ring";
		_guioptions = Common::GUIO_NOSUBTITLES | Common::GUIO_NOSFX;
		_maxScanDepth = 2;
		_directoryGlobs = directoryGlobs;
	}

	const char *getName() const {
		return "Ring";
	}

	const char *getOriginalCopyright() const {
		return "Ring Engine (C) 1999 Arxel Tribe";
	}

	virtual bool hasFeature(MetaEngineFeature f) const;
	virtual int getMaximumSaveSlot() const { return 99; }

protected:
	bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *gd) const;
};

bool RingMetaEngine::hasFeature(MetaEngineFeature f) const {
	return (f == kSupportsListSaves) ||
	       (f == kSupportsDeleteSave) ||
	       (f == kSavesSupportMetaInfo) ||
	       (f == kSavesSupportThumbnail) ||
	       (f == kSavesSupportCreationDate) ||
	       (f == kSavesSupportPlayTime);
}

bool RingMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	if (desc)
		*engine = new RingEngine(syst, (const RingGameDescription *)desc);

	return desc != 0;
}

} // End of namespace Ring

#if PLUGIN_ENABLED_DYNAMIC(RING)
	REGISTER_PLUGIN_DYNAMIC(RING, PLUGIN_TYPE_ENGINE, Ring::RingMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(RING, PLUGIN_TYPE_ENGINE, Ring::RingMetaEngine);
#endif
