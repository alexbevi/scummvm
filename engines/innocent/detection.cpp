#include "innocent/innocent.h"

#include "common/fs.h"
#include "base/game.h"
#include "base/plugins.h"
#include "engines/metaengine.h"
#include "common/config-manager.h"

using namespace Common;

static const PlainGameDescriptor innocent_setting[] = {
	{ "innocent", "Innocent Until Caught" },
	{ 0, 0 }
};

class InnocentMetaEngine : public MetaEngine {
public:
	virtual const char *getName() const {
		return "Innocent Until Caught";
	}

	virtual const char *getCopyright() const {
		return "Copyright (c) 1993 Divide by Zero";
	}

	virtual GameList getSupportedGames() const {
		GameList games;
		const PlainGameDescriptor *g = innocent_setting;
		while (g->gameid) {
			games.push_back(*g);
			g++;
		}

		return games;
	}

	virtual GameDescriptor findGame(const char *gameid) const {
		const PlainGameDescriptor *g = innocent_setting;
		while (g->gameid) {
			if (0 == scumm_stricmp(gameid, g->gameid))
				break;
			g++;
		}
		return GameDescriptor(g->gameid, g->description);
	}

	virtual GameList detectGames(const FSList &fslist) const {
		GameList detectedGames;

		for (FSList::const_iterator file = fslist.begin(); file != fslist.end(); ++file) {
			if (!file->isDirectory()) {
				const char *gameName = file->getName().c_str();

				if (0 == scumm_stricmp("IUC_MAIN.DAT", gameName)) {
					detectedGames.push_back(innocent_setting[0]);
					break;
				}
			}
		}
		return detectedGames;
	}

	virtual PluginError createInstance(OSystem *syst, Engine **engine) const {
		assert(syst);
		assert(engine);

		FSList fslist;
		FilesystemNode dir(ConfMan.get("path"));
		if (!dir.getChildren(fslist, FilesystemNode::kListAll)) {
			return kInvalidPathError;
		}

		Common::String gameid = ConfMan.get("gameid");
		GameList detectedGames = detectGames(fslist);

		for (uint i = 0; i < detectedGames.size(); i++) {
			if (detectedGames[i].gameid() == gameid) {
				*engine = new Innocent::Engine(syst);
				return kNoError;
			}
		}

		return kNoGameDataFoundError;
	}
};

#if PLUGIN_ENABLED_DYNAMIC(INNOCENT)
	REGISTER_PLUGIN_DYNAMIC(INNOCENT, PLUGIN_TYPE_ENGINE, InnocentMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(INNOCENT, PLUGIN_TYPE_ENGINE, InnocentMetaEngine);
#endif
