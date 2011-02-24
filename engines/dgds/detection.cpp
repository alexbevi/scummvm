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
 * $URL: https://scummvm-dgds.googlecode.com/svn/trunk/detection.cpp $
 * $Id: detection.cpp 12 2010-01-23 12:01:58Z alexbevi $
 *
 */

#include "common/config-manager.h"
#include "engines/advancedDetector.h"
#include "common/savefile.h"
#include "common/system.h"

#include "base/plugins.h"

#include "dgds/dgds.h"

static const PlainGameDescriptor DgdsGameTitles[] = {
	{"dgds",   "Unknown DGDS game"},
	{"china",  "Heart of China"},
	{"dragon", "Rise of the Dragon"},
	{"willy",  "The Adventures of Willy Beamish"},
	{0, 0}
};

namespace Dgds {

/**
 * Detection entries for Dynamix games
 *
 * When adding a new entry, use Game.getVersion() for the
 * version information.
 */
static const ADGameDescription DgdsGameDescriptions[] = {
	// Heart of China - Spanish PC
	{"china", "",
		AD_ENTRY1s("volume.rmf", "072cffb026d91b8d25d11c70f4a57fb2", 10032),
		Common::ES_ESP,
		Common::kPlatformPC,
		ADGF_NO_FLAGS,
		Common::GUIO_NONE
	},

	// Heart of China - English PC v1.216
	{"china", "",
		AD_ENTRY1s("volume.rmf", "2da720143c1e857490daded15682df87", 9791),
		Common::EN_ANY,
		Common::kPlatformPC,
		ADGF_NO_FLAGS,
		Common::GUIO_NONE
	},

	// Rise of the Dragon - English Amiga
	{"dragon", "",
		AD_ENTRY1s("volume.rmf", "44cd1ffdfeb385dcfcd60563e1036167", 8972),
		Common::EN_ANY,
		Common::kPlatformAmiga,
		ADGF_NO_FLAGS,
		Common::GUIO_NONE
	},

	// Rise of the Dragon - English PC v1.211
	{"dragon", "",
		AD_ENTRY1s("volume.vga", "0d9590fdc883d6d99c06e7935423d180", 7823),
		Common::EN_ANY,
		Common::kPlatformPC,
		ADGF_NO_FLAGS,
		Common::GUIO_NONE
	},

	// The Adventures of Willy Beamish - English PC v1.224
	{"willy","",
		AD_ENTRY1s("volume.rmf", "ba5cf24ec4b6dc044b03115a0cd5f3ff", 9896),
		Common::EN_ANY,
		Common::kPlatformPC,
		ADGF_NO_FLAGS,
		Common::GUIO_NONE
	},

	AD_TABLE_END_MARKER
};

} // End of namespace Dgds

static const ADGameDescription DgdsGameGeneric[] = {
	{"dgds", 0,
		AD_ENTRY1("volume.rmf", NULL),
		Common::UNK_LANG,
		Common::kPlatformUnknown,
		0,
		Common::GUIO_NONE
	},
	AD_TABLE_END_MARKER
};

static const ADFileBasedFallback DgdsGameFallback[] = {
	{(const void*)&DgdsGameGeneric[0], {"volume.rmf", NULL} },
	{(const void*)&DgdsGameGeneric[0], {"volume.vga", NULL} },
	{(const void*)&DgdsGameGeneric[0], {"resource.map", NULL} },
	{0, {NULL}}
};

static const ADParams detectionParams = {
	(const byte *)Dgds::DgdsGameDescriptions,
	sizeof(ADGameDescription),
	0,                // number of md5 bytes
	DgdsGameTitles,
	0,                // no obsolete targets data
	"dgds",
	DgdsGameFallback, // file-based detection data to enable not yet
	                  // known versions to start
	kADFlagPrintWarningOnFileBasedFallback,
	Common::GUIO_NONE,
	1,                // Maximum directory depth
	0                 // List of directory globs
};

class DgdsMetaEngine : public AdvancedMetaEngine {
public:
	DgdsMetaEngine() : AdvancedMetaEngine(detectionParams) {}

	virtual const char *getName() const { return "Dynamix Game Development System"; }
	virtual const char *getOriginalCopyright() const { return "Dynamix Game Development System (c) Dynamix"; }

	virtual bool hasFeature(MetaEngineFeature f) const;
	virtual bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const;
	virtual SaveStateList listSaves(const char *target) const;
	virtual int getMaximumSaveSlot() const;
	virtual void removeSaveState(const char *target, int slot) const;
};

bool DgdsMetaEngine::hasFeature(MetaEngineFeature f) const {
	return
		(f == kSupportsListSaves) ||
		(f == kSupportsLoadingDuringStartup) ||
		(f == kSupportsDeleteSave);
}

bool Dgds::DgdsEngine::hasFeature(EngineFeature f) const {
	return
		(f == kSupportsRTL) ||
		(f == kSupportsLoadingDuringRuntime) ||
		(f == kSupportsSavingDuringRuntime);
}

bool DgdsMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	if (desc) {
		*engine = new Dgds::DgdsEngine(syst, desc->language);
	}
	return desc != 0;
}

SaveStateList DgdsMetaEngine::listSaves(const char *target) const {
	SaveStateList saveList;

	// TODO

	return saveList;
}

int DgdsMetaEngine::getMaximumSaveSlot() const {
	return 0;
}

void DgdsMetaEngine::removeSaveState(const char *target, int slot) const {
	// TODO
}

#if PLUGIN_ENABLED_DYNAMIC(DGDS)
REGISTER_PLUGIN_DYNAMIC(DGDS, PLUGIN_TYPE_ENGINE, DgdsMetaEngine);
#else
REGISTER_PLUGIN_STATIC(DGDS, PLUGIN_TYPE_ENGINE, DgdsMetaEngine);
#endif
