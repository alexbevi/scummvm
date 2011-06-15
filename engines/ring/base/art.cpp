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

#include "ring/base/art.h"

#include "ring/base/application.h"

#include "ring/debug.h"
#include "ring/helpers.h"
#include "ring/ring.h"

#include "common/file.h"
#include "common/substream.h"

namespace Ring {

#pragma region Art

Art::Art() : _zone(kZoneNone), _loadFrom(kLoadFromInvalid) {
}

Art::~Art() {
}

void Art::init(const Common::String &path, ZoneId zone, LoadFrom loadFrom) {
	_path = path;
	_zone = zone;
	_loadFrom = loadFrom;

	// Open a stream to the archive
	Common::SeekableReadStream *archive = SearchMan.createReadStreamForMember(path);
	if (!archive)
		error("[Art::init] Error opening file: %s", path.c_str());

	debugC(2, kRingDebugResource, "Opened archive: %s", path.c_str());

	// Read header
	archive->read(&_header.format, sizeof(_header.format));
	_header.field_8 = archive->readUint32LE();
	_header.count = archive->readUint32LE();
	_header.field_10 = archive->readUint32LE();
	_header.field_14 = archive->readUint32LE();
	_header.field_18 = archive->readUint32LE();
	_header.field_1C = archive->readUint32LE();

	// Read file list
	for (uint32 i = 0; i < _header.count; i++) {
		char name[243];
		archive->read(&name, sizeof(char) * 243);

		Record record;
		record.offset = archive->readUint32LE();
		record.size = archive->readUint32LE();
		record.field_FB = archive->readUint32LE();

		// Replace path separators & convert to lowercase
		Common::String filename = name;
		for (Common::String::iterator c = filename.begin(); c != filename.end(); ++c)
			if (*c == '\\')
				*c = '/';

		filename.toLowercase();

		// Add to file map
		_records[filename] = record;
	}

	// Close stream
	delete archive;
}

bool Art::hasFile(const Common::String &name) {
	return (_records.find(name) != _records.end());
}

int Art::listMembers(Common::ArchiveMemberList &list) {
	int numMembers = 0;

	for (RecordMap::const_iterator i = _records.begin(); i != _records.end(); ++i) {
		list.push_back(Common::ArchiveMemberList::value_type(new Common::GenericArchiveMember(i->_key, this)));
		numMembers++;
	}

	return numMembers;
}

Common::ArchiveMemberPtr Art::getMember(const Common::String &name) {
	if (!hasFile(name))
		return Common::ArchiveMemberPtr();

	return Common::ArchiveMemberPtr(new Common::GenericArchiveMember(name, this));
}

Common::SeekableReadStream *Art::createReadStreamForMember(const Common::String &name) const {
	// Convert to lowercase for match
	Common::String filename(name);
	filename.toLowercase();

	RecordMap::const_iterator fDesc = _records.find(filename);
	if (fDesc == _records.end())
		return NULL;

	Common::File *archive = new Common::File();
	if (!archive->open(_path)) {
		delete archive;
		return NULL;
	}

	return new Common::SeekableSubReadStream(archive, fDesc->_value.offset, fDesc->_value.offset + fDesc->_value.size, DisposeAfterUse::YES);
}

#pragma endregion

#pragma region ArtHandler

ArtHandler::ArtHandler(Application *application) : _app(application) {
}

ArtHandler::~ArtHandler() {
	CLEAR_ARRAY(Art, _arts);

	// Zero-out passed pointers
	_app = NULL;
}

void ArtHandler::open(ZoneId zone, LoadFrom loadFrom) {
	// Check if the archive is already loaded
	if (isPresent(zone, loadFrom))
		return;

	// We only support loading from cd and from disk
	if (loadFrom != kLoadFromCd && loadFrom != kLoadFromDisk)
		error("[ArtHandler::open] Load From not supported (%d)", loadFrom);

	// Compute path
	// - All data should be on disk, so we ignore the loadFrom value
	// - We support multi-language, so for the system zone, we need to read from the language folder
	// - We have different versions of the archive format (art, at2, at3)
	Common::String path;
	switch (((RingEngine *)g_engine)->getGameType()) {
	default:
		error("[ArtHandler::open] Unsupported game type (%d)", ((RingEngine *)g_engine)->getGameType());

	case GameTypePilgrim:
		path = Common::String::format("%s/%s.art", getArtFolder(zone).c_str(), _app->getZoneFolder(zone).c_str());
		break;

	case GameTypeRing:
		path = Common::String::format("%s/%s.at2", getArtFolder(zone).c_str(), _app->getZoneFolder(zone).c_str());
		break;

	case GameTypeFaust:
	case GameTypePompeii:
	case GameTypePilgrim2:
	case GameTypePilgrim3:
	case GameTypeJerusalem:
		path = Common::String::format("%s/%s.at3", getArtFolder(zone).c_str(), _app->getZoneFolder(zone).c_str());
		break;
	}


	// Create new archive
	Art *art = new Art();
	art->init(path, zone, loadFrom);

	_arts.push_back(art);
}

void ArtHandler::reset() {
	for (uint i = 0; i < _arts.size();) {
		if (_arts[i]->getLoadFrom() == kLoadFromCd) {
			SAFE_DELETE(_arts[i]);
			_arts.remove_at(i);
		} else {
			++i;
		}
	}
}

Common::SeekableReadStream *ArtHandler::get(Common::String filename, ZoneId zone, LoadFrom loadFrom) {
	int32 index = getIndex(zone, loadFrom);

	if (index == -1) {
		warning("[ArtHandler::get] File for Zone %d and LoadFrom %d not opened!", zone, loadFrom);
		return NULL;
	}

	return _arts[index]->createReadStreamForMember(filename);
}

bool ArtHandler::isPresent(ZoneId zone, LoadFrom loadFrom) {
	return (getIndex(zone, loadFrom) != -1);
}

int32 ArtHandler::getIndex(ZoneId zone, LoadFrom loadFrom) {
	ZoneId z = zone;
	LoadFrom l = loadFrom;

	// Compute zone & loadFrom
	if (loadFrom == kLoadFromDisk || loadFrom == kLoadFromCursor || loadFrom == kLoadFromListIcon || loadFrom == kLoadFrom5) {
		z = getZoneSY();
		l = kLoadFromDisk;
	}

	if (_arts.empty())
		return -1;

	for (uint32 i = 0; i < _arts.size(); i++)
		if (_arts[i]->getZone() == z && _arts[i]->getLoadFrom() == l)
			return i;

	return -1;
}

Common::String ArtHandler::getArtFolder(ZoneId zone) {
	// For zones other than system, just read from the data folder
	if (zone != getZoneSY())
		return "DATA";

	// If the game handles multiple languages, we need to read from the language subfolder
	if (((RingEngine *)g_engine)->isMultiLanguage())
		return "DATA/" + _app->getLanguageFolder();

	// Otherwise, the system archive file is in the data folder with all the others
	return "DATA";
}

#pragma endregion

} // End of namespace Ring
