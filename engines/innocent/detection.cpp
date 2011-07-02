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

#include "innocent/innocent.h"

#include "common/error.h"
#include "common/fs.h"
#include "engines/game.h"
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

	virtual const char *getOriginalCopyright() const {
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

	virtual Common::Error createInstance(OSystem *syst, Engine **engine) const {
		assert(syst);
		assert(engine);

		FSList fslist;
		FSNode dir(ConfMan.get("path"));
		if (!dir.getChildren(fslist, FSNode::kListAll)) {
			return kPathDoesNotExist;
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
