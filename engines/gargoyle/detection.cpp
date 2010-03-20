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
#include "common/error.h"
#include "common/file.h"
#include "common/savefile.h"
#include "gargoyle/gargoyle.h"
#include "gargoyle/frotz_interp.h"

namespace Gargoyle {

struct GargoyleGameDescription {
	ADGameDescription desc;

	Common::String filename;
	int engine;
	int gameID;
//	uint32 features;
//	uint16 version;
};

GargoyleGameDescription gameDesc;

GargoyleEngineType GargoyleEngine::getEngine() const {
	return (GargoyleEngineType) _gameDescription->engine;
}

const Common::String &GargoyleEngine::getFilename() const {
	return _gameDescription->filename;
}

uint32 GargoyleEngine::getGameID() const {
	return _gameDescription->gameID;
}

const char *GargoyleEngine::getGameId() const {
	return _gameDescription->desc.gameid;
}


uint32 GargoyleEngine::getFeatures() const {
	return 0; //_gameDescription->features;
}

Common::Platform GargoyleEngine::getPlatform() const {
	return _gameDescription->desc.platform;
}

Common::Language GargoyleEngine::getLanguage() const {
	return _gameDescription->desc.language;
}

uint16 GargoyleEngine::getVersion() const {
	return 0; //_gameDescription->version;
}

}

static const PlainGameDescriptor gargoyleGames[] = {
	{"gargoyle", "Gargoyle Text Adventure Module"},

	/* Frotz engine games */
	// Infocom games
	{"amfv", "A Mind Forever Voyaging"},
	{"arthur", "Arthur, The Quest For Excalibur"},
	{"ballyhoo", "Ballyhoo"},
	{"beyondzork", "Beyond Zork"},
	{"borderzone", "Border Zone"},
	{"bureaucracy", "Bureaucracy"},
	{"cutthroats", "Cutthroats"},
	{"deadline", "Deadline"},
	{"enchanter", "Enchanter"},
	{"hhgttg", "Hitchhiker's Guide To The Galaxy"},
	{"hijinx", "Hollywood Hijinx"},
	{"infidel", "Infidel"},
	{"journey", "Journey"},
	{"lgop", "Leather Goddesses Of Phobos"},
	{"lurking", "Lurking Horror"},
	{"minizork1", "Mini-Zork I"},
	{"moonmist", "Moonmist"},
	{"nordbert", "Nord And Bert Couldnt Make Heads Or Tails Of It"},
	{"planetfall", "Planetfall"},
	{"plundered", "Plundered Hearts"},
	{"sampler1", "Infocom Sampler 1"},
	{"sampler2", "Infocom Sampler 2"},
	{"seastalker", "Seastalker"},
	{"sherlock", "Sherlock, The Riddle Of The Crown Jewels"},
	{"shogun", "Shogun"},
	{"sorcerer", "Sorcerer"},
	{"spellbreaker", "Spellbreaker"},
	{"starcross", "Starcross"},
	{"stationfall", "Stationfall"},
	{"suspect", "Suspect"},
	{"suspended", "Suspended"},
	{"trinity", "Trinity"},
	{"wishbringer", "Wishbringer, The Magic Stone Of Dreams"},
	{"witness", "Witness"},
	{"zork1", "Zork 1"},
	{"zork2", "Zork 2"},
	{"zork3", "Zork 3"},
	{"zork0", "Zork Zero"},
	{"zorkuu", "Zork, The Undiscovered Underground"},

	// Known fan-made z-code games
	{"freefall", "Freefall"},

	// Generic z-code game entry
	{"zcode", "Fanmade Z-Code game"},

	{0, 0}
};


namespace Gargoyle {

struct GameSettings {
	const char *target;
	const char *description;
	const char *md5;
	GargoyleEngineType engine;
	GargoyleGameId gameId;
};

static const ADParams detectionParams = {
	// Pointer to ADGameDescription or its superset structure
	NULL, //(const byte *)Gargoyle::gameDescriptions,
	// Size of that superset structure
	0, //sizeof(Gargoyle::GargoyleGameDescription),
	// Number of bytes to compute MD5 sum for
	5000,
	// List of all engine targets
	gargoyleGames,
	// Structure for autoupgrading obsolete targets
	0,
	// Name of single gameid (optional)
	"gargoyle",
	// List of files for file-based fallback detection (optional)
	0,
	// Flags
	0,
	// Additional GUI options (for every game}
	Common::GUIO_NOSPEECH
};

} // End of namespace Gargoyle

using namespace Gargoyle;

class GargoyleMetaEngine : public AdvancedMetaEngine {
	mutable Common::String	_gameid;
	mutable Common::String	_extra;

public:
	GargoyleMetaEngine() : AdvancedMetaEngine(detectionParams) {}

	virtual const char *getName() const {
		return "Gargoyle Engine";
	}
	virtual const char *getOriginalCopyright() const {
		return "Z-Code VM (C) Infocom";
	}

	virtual bool hasFeature(MetaEngineFeature f) const;
	virtual SaveStateList listSaves(const char *target) const;
	virtual int getMaximumSaveSlot() const;
	virtual void removeSaveState(const char *target, int slot) const;
	virtual Common::Error createInstance(OSystem *syst, Engine **engine) const;
	virtual bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const;

	virtual GameList detectGames(const Common::FSList &fslist) const;
};

bool GargoyleMetaEngine::hasFeature(MetaEngineFeature f) const {
	return
		(f == kSupportsListSaves) ||
		(f == kSupportsDeleteSave) ||
		(f == kSupportsLoadingDuringStartup);
}

SaveStateList GargoyleMetaEngine::listSaves(const char *target) const {
	Common::SaveFileManager *saveFileMan = g_system->getSavefileManager();
	Common::StringArray filenames;
	Common::String pattern(target);
	char description[100];
	pattern += ".s??";

	Common::FSNode savePath(ConfMan.get("savepath"));

	filenames = saveFileMan->listSavefiles(pattern);

	SaveStateList saveList;
	for (Common::StringArray::const_iterator file = filenames.begin(); file != filenames.end(); ++file) {
		// Obtain the last 2 digits of the filename, since they correspond to the save slot
		int slotNum = atoi(file->c_str() + file->size() - 2);

		if (slotNum >= 0 && slotNum <= 99) {
			Common::FSNode saveFile = savePath.getChild(file->c_str());
			Common::ReadStream *in = saveFile.createReadStream();
			if (in) {
				if (Frotz::FrotzInterpreter::validate_savegame(in, description))
					saveList.push_back(SaveStateDescriptor(slotNum, description));
				delete in;
			}
		}
	}

	return saveList;
}

int GargoyleMetaEngine::getMaximumSaveSlot() const { return 99; }

void GargoyleMetaEngine::removeSaveState(const char *target, int slot) const {
	char filename[100];
	snprintf(filename, 100, "%s.s%.2d", target, slot);
	
	g_system->getSavefileManager()->removeSavefile(filename);
}

Common::Error GargoyleMetaEngine::createInstance(OSystem *syst, Engine **engine) const {
	assert(engine);
	GargoyleEngineType engineId;

	// This is used to identify certain specific games that need special handling
	Gargoyle::GargoyleGameId gameIdent;
	
	// Get the gameId to use, and scan through the settings list to find any entry for it
	Common::String gameId = ConfMan.get("gameid");

	if (Frotz::FrotzInterpreter::identifyGame(gameId, gameIdent))
		engineId = GTYPE_ZCODE;
	else
		return Common::kUnknownError;

	// Create a skeleton game descriptor to pass to the engine
	gameDesc.desc.gameid = strdup(gameId.c_str());
	gameDesc.desc.language = Common::parseLanguage(ConfMan.get("language"));
	gameDesc.desc.platform = Common::parsePlatform(ConfMan.get("platform"));
	gameDesc.filename = ConfMan.get("filename");
	gameDesc.engine = engineId;
	gameDesc.gameID = gameIdent;

	*engine = new Gargoyle::GargoyleEngine(syst, &gameDesc);
	return Common::kNoError;
}

bool GargoyleMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	const Gargoyle::GargoyleGameDescription *gd = (const Gargoyle::GargoyleGameDescription *)desc;

	*engine = new Gargoyle::GargoyleEngine(syst, gd);
	return Common::kNoError;
}

GameList GargoyleMetaEngine::detectGames(const Common::FSList &fslist) const {
	GameList detectedGames;
	char target[20], version[20];
	Common::Language lang;

	// Loop through the files of the folder
	for (Common::FSList::const_iterator file = fslist.begin(); file != fslist.end(); ++file) {
		if (file->isDirectory()) continue;

		if (Frotz::FrotzInterpreter::validateFile(*file, target, version, lang)) {
			// Scan through the Gargoyle game list to get the game description
			const PlainGameDescriptor *p = &gargoyleGames[0];
			while ((p->gameid != NULL) && (strcmp(p->gameid, target) != 0))
				++p;

			// Set up the full game description
			bool isKnown = (p != NULL) && (strcmp(target, "zcode") != 0);
			Common::String desc = isKnown ? Common::String(p->description) : file->getName();
			desc += " (";
			if (strlen(version) != 0)
				desc += Common::String(version) + "/";
			desc += Common::getLanguageDescription(lang);
			desc += ")";

			GameDescriptor gd(target, desc);
			gd["filename"] = file->getName();
			gd["language"] = "en";
			gd["platform"] = "pc";

			detectedGames.push_back(gd);
		}
	}

	return detectedGames;
}

#if PLUGIN_ENABLED_DYNAMIC(GARGOYLE)
	REGISTER_PLUGIN_DYNAMIC(GARGOYLE, PLUGIN_TYPE_ENGINE, GargoyleMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(GARGOYLE, PLUGIN_TYPE_ENGINE, GargoyleMetaEngine);
#endif

