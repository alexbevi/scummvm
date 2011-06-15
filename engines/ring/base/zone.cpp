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

#include "ring/base/zone.h"

#include "ring/helpers.h"

namespace Ring {

#pragma region Zone

Zone::Zone(ZoneId id, Common::String name, Common::String folder, ArchiveType archiveType, LoadFrom loadFrom) : BaseObject(id) {
	_name = name;
	_folder = folder;
	_archiveType = archiveType;
	_loadFrom = loadFrom;
}

#pragma endregion

#pragma region Episode

Episode::Episode(ZoneId id, Common::String name, uint32 cd) : BaseObject(id) {
	_name = name;
	_cd = cd;
}

#pragma endregion

#pragma region ZoneHandler

ZoneHandler::ZoneHandler() {
	_zone = kZoneNone;
	_episode = kZoneNone;
}

ZoneHandler::~ZoneHandler() {
	CLEAR_ARRAY(Zone, _zones);
	CLEAR_ARRAY(Episode, _episodes);
}

//////////////////////////////////////////////////////////////////////////
// Zones
void ZoneHandler::addZone(ZoneId id, Common::String name, Common::String folder, ArchiveType archiveType, LoadFrom loadFrom) {
	if (_zones.has(id))
		error("[ZoneHandler::addZone] Zone already exists (%d)", id);

	_zones.push_back(new Zone(id, name, folder, archiveType, loadFrom));
}

Zone *ZoneHandler::getZone(ZoneId id) {
	if (!_zones.has(id))
		error("[ZoneHandler::getZone] Zone doesn't exist (%d)", id);

	return _zones.get(id);
}

bool ZoneHandler::has(ZoneId id) {
	return _zones.has(id);
}

//////////////////////////////////////////////////////////////////////////
// Episodes
void ZoneHandler::addEpisode(ZoneId id, Common::String name, uint32 cd) {
	if (_episodes.has(id))
		error("[ZoneHandler::addEpisode] Episode already exists (%d)", id);

	_episodes.push_back(new Episode(id, name, cd));
}

Episode *ZoneHandler::getEpisode(ZoneId id) {
	if (!_episodes.has(id))
		error("[ZoneHandler::getEpisode] Episode doesn't exist (%d)", id);

	return _episodes.get(id);
}

//////////////////////////////////////////////////////////////////////////
// Current values
void ZoneHandler::setCurrentEpisode(ZoneId id) {
	if (!_episodes.has(id))
		error("[ZoneHandler::setCurrentEpisode] Episode doesn't exist (%d)", id);

	_episode = id;
}

void ZoneHandler::setCurrentZone(ZoneId id) {
	if (!_zones.has(id))
		error("[ZoneHandler::setCurrentZone] Zone doesn't exist (%d)", id);

	_zone = id;
}

#pragma endregion

} // End of namespace Ring
