/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "engines/metaengine.h"
#include "common/fs.h"
#include "common/unzip.h"

#include "unity.h"

static const PlainGameDescriptor unityGames[] = {
	{ "unity", "Star Trek: The Next Generation \"A Final Unity\"" },
	{ 0, 0 }
};

class UnityMetaEngine : public MetaEngine {
	virtual const char *getName() const { return "A Final Unity engine"; }
	virtual const char *getOriginalCopyright() const { return "Copyright (C) Spectrum Holobyte Inc."; }

	virtual GameList getSupportedGames() const {
		GameList games;
		const PlainGameDescriptor *g = unityGames;
		while (g->gameid) {
			games.push_back(*g);
			g++;
		}
		return games;
	}

	virtual GameDescriptor findGame(const char *gameid) const {
		const PlainGameDescriptor *g = unityGames;
		while (g->gameid) {
			if (0 == scumm_stricmp(gameid, g->gameid))
				break;
			g++;
		}
		return GameDescriptor(g->gameid, g->description);
	}

	virtual GameList detectGames(const Common::FSList &fslist) const {
		GameList detectedGames;

		for (Common::FSList::const_iterator file = fslist.begin(); file != fslist.end(); ++file) {
			if (!file->isDirectory()) {
				const char *gameName = file->getName().c_str();
 
				// game data is stored on the CD in this file;
				// TODO: might want to support unzipped installs too
				if (0 == scumm_stricmp("STTNG.ZIP", gameName)) {
					Common::Archive *archive = Common::makeZipArchive(*file);
					// just some random file as a sanity check
					if (archive && archive->hasFile("PENTARA.BIN")) {
						detectedGames.push_back(unityGames[0]);
						delete archive;
						break;
					}
					delete archive;
				}
			}
		}
		return detectedGames;
	}

	virtual Common::Error createInstance(OSystem *syst, Engine **engine) const {
		*engine = new Unity::UnityEngine(syst);
		return Common::kNoError;
	}
};

#if PLUGIN_ENABLED_DYNAMIC(UNITY)
	REGISTER_PLUGIN_DYNAMIC(UNITY, PLUGIN_TYPE_ENGINE, UnityMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(UNITY, PLUGIN_TYPE_ENGINE, UnityMetaEngine);
#endif
