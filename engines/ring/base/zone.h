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

#ifndef RING_ZONE_H
#define RING_ZONE_H

#include "ring/shared.h"

namespace Ring {

class Zone : public BaseObject {
public:
	Zone(Id id, Common::String name, Common::String folder, ArchiveType archiveType, LoadFrom loadFrom);
	~Zone() {}

	// Accessors
	Common::String getName()        { return _name; }
	Common::String getFolder()      { return _folder; }
	ArchiveType    getArchiveType() { return _archiveType; }
	LoadFrom       getLoadFrom()    { return _loadFrom; }

private:
	Common::String _name;
	Common::String _folder;
	ArchiveType    _archiveType;
	LoadFrom       _loadFrom;
};

class Episode : public BaseObject {
public:
	Episode(Id id, Common::String name, uint32 cd);
	~Episode() {}

	// Accessors
	Common::String getName() { return _name; }
	uint32         getCd()   { return _cd; }

private:
	Common::String _name;
	uint32         _cd;
};

class ZoneHandler {
public:
	ZoneHandler();
	~ZoneHandler();

	// Zones
	void addZone(ZoneId id, Common::String name, Common::String folder, ArchiveType archiveType, LoadFrom loadFrom);
	Zone *getZone(ZoneId id);

	// Episodes
	void addEpisode(Id id, Common::String name, uint32 cd);
	Episode *getEpisode(Id id);

	// Current values
	void   setCurrentEpisode(Id id);
	Id     getCurrentEpisode() { return _episode; }

	void   setCurrentZone(ZoneId id);
	ZoneId getCurrentZone() { return _zone; }

private:
	AssociativeArray<Zone *>    _zones;
	AssociativeArray<Episode *> _episodes;
	ZoneId                      _zone;
	Id                          _episode;
};

} // End of namespace Ring

#endif // RING_ZONE_H
