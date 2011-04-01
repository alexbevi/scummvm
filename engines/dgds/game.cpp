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
 * $URL: https://scummvm-dgds.googlecode.com/svn/trunk/game.cpp $
 * $Id: game.cpp 12 2010-01-23 12:01:58Z alexbevi $
 *
 */

#include "dgds/game.h"

namespace Dgds {

Game::Game(DgdsEngine *engine, ResourceManager *resman):
	_vm(engine), _resMan(resman) {

}

Game::~Game() {

}

bool Game::init() {
	bool ret = false;

	Common::String gameName = _resMan->findGDS();

	Resource *res = _resMan->getResource(gameName + ".GDS");

	if (res) {
		Resource *resGDS = res->getSubResource("GDS:");
		if (resGDS) {
			ret = loadINF(resGDS->getSubResource("INF:"));
			if (ret)
				ret = loadSDS(resGDS->getSubResource("SDS:"));
			delete resGDS;
		} else {
			warning("The resource doesn't contain a game description");
		}
		delete res;
	} else {
		warning("Couldn't find the resource");
	}

	return ret;
}

bool Game::loadINF(Resource *resINF) {
	bool ret = false;

	if (resINF) {
		_gameID = resINF->readUint32LE();

		char tmp[7];
		resINF->read(tmp, 7);
		_version = tmp;

		delete resINF;
		ret = true;
	} else {
		warning("The resource doesn't contain game description information");
	}

	return ret;
}

bool Game::loadSDS(Resource *resSDS) {
	bool ret = false;

	if (resSDS) {
		// TODO

		delete resSDS;
		ret = true;
	} else {
		warning("The resource doesn't contain game description data");
	}

	return ret;
}

} // End of namespace Dgds
