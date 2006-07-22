/* ScummVM - Scumm Interpreter
 * Copyright (C) 2005-2006 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#include "common/stdafx.h"
#include "base/plugins.h"

#include "common/fs.h"
#include "common/system.h"
#include "common/md5.h"

#include "kom/kom.h"

namespace Kom {

static const PlainGameDescriptor kom_list[] = {
	{ "kom", "Kingdom O\' Magic" },
	{ 0, 0 }
};

GameList Engine_KOM_gameIDList() {
	GameList games;
	const PlainGameDescriptor *g = kom_list;

	while (g->gameid) {
		games.push_back(*g);
		g++;
	}
	return games;
}

GameDescriptor Engine_KOM_findGameID(const char *gameid) {
	const PlainGameDescriptor *g = kom_list;
	while (g->gameid) {
		if (0 == scumm_stricmp(gameid, g->gameid))
			break;
		g++;
	}
	return *g;
}

DetectedGameList Engine_KOM_detectGames(const FSList &fslist) {
	DetectedGameList detectedGames;
	for (FSList::const_iterator file = fslist.begin(); file != fslist.end(); ++file) {
		if (!file->isDirectory()) {
			const char *filename = file->displayName().c_str();

			if (0 == scumm_stricmp("thidney.dsk", filename) ||
			    0 == scumm_stricmp("shahron.dsk", filename)) {
				// Only 1 target ATM
				detectedGames.push_back(DetectedGame(kom_list[0].gameid, kom_list[0].description, Common::EN_ANY, Common::kPlatformPC));
				break;
			}
		}
	}
	return detectedGames;
}

PluginError Engine_KOM_create(OSystem *syst, Engine **engine) {
	assert(engine);
	*engine = new KomEngine(syst);
	return kNoError;
}

} // End of namespace Kom

using namespace Kom;

REGISTER_PLUGIN(KOM, "Kingdom O\' Magic Engine");
