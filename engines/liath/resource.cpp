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
 * $Id: resource.cpp 379 2010-04-29 23:02:31Z julien.templier $
 *
 */

#include "liath/resource.h"

#include "liath/data/archive.h"

#include "liath/debug.h"

#include "common/archive.h"
#include "common/file.h"

namespace Liath {

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {
	for (ArchiveCache::iterator i = _archives.begin(); i != _archives.end(); ++i)
		delete i->_value;

	_archives.clear();

	_files.clear();
}

//////////////////////////////////////////////////////////////////////////
// Archive
//////////////////////////////////////////////////////////////////////////

bool ResourceManager::hasFile(const Common::String &name) {
	// Files can be either part of a MUL archive or in one of the data folders

	if (_files.find(name) != _files.end())
		return true;

	return SearchMan.hasFile(name);
}

int ResourceManager::listMembers(Common::ArchiveMemberList &list) {
	int numMembers = 0;

	for (FileMap::const_iterator i = _files.begin(); i != _files.end(); ++i) {
		list.push_back(Common::ArchiveMemberList::value_type(new Common::GenericArchiveMember(i->_key, this)));
		numMembers++;
	}

	Common::ArchiveMemberList files;
	numMembers += SearchMan.listMembers(files);

	for (Common::ArchiveMemberList::iterator i = files.begin(); i != files.end(); ++i) {
		list.push_back(Common::ArchiveMemberList::value_type(new Common::GenericArchiveMember((*i)->getName(), this)));
	}

	return numMembers;
}

Common::ArchiveMemberPtr ResourceManager::getMember(const Common::String &name) {
	if (!hasFile(name))
		return Common::ArchiveMemberPtr();

	return Common::ArchiveMemberPtr(new Common::GenericArchiveMember(name, this));
}

Common::SeekableReadStream *ResourceManager::createReadStreamForMember(const Common::String &name) const {
	// Load a normal file
	if (_files.find(name) == _files.end()) {
		Common::File *file = new Common::File();
		if (!file->open(name)) {
			delete file;
			return NULL;
		}

		return file;
	}

	// Get the archive file from the cache
	Common::String archiveName = _files[name].archiveName;

	// FIXME
	//if (!_archives.contains(archiveName))
		//_archives[archiveName] = new MultiArchive(archiveName);

	// Load the file from the archive
	MultiArchive *archive = new MultiArchive(archiveName); //_archives[archiveName];
	if (!archive->hasFile(name)) {
		delete archive;
		return NULL;
	}

	Common::SeekableReadStream *stream = archive->createReadStreamForMember(name);

	delete archive;

	return stream;
}

//////////////////////////////////////////////////////////////////////////
// Path data
//////////////////////////////////////////////////////////////////////////

/**
 * Reads the path file and initialize the path list
 *
 * Note: Using ConfigFile cannot work here, as it expects key/value pairs
 */
void ResourceManager::readPathFile() {
	// Open a stream to the path file
	Common::File *pathFile = new Common::File();
	if (!pathFile->open("liath.ini"))
		error("Could not open config file (liath.ini)");

	bool readingPaths = false;

	while (!pathFile->eos() && !pathFile->err()) {

		// Read a line
		Common::String line = pathFile->readLine();
		line.trim();

		if (line.size() == 0) {
			// Do nothing
		} else if (line.c_str()[0] == 26) { // There is a single SUB character at the end of the file
			// Do nothing
		} else if (line[0] == '[') {
			// Check if this is a Path section, and start reading the list of path if true
			readingPaths = (line == "[Path]" ? true : false);
		} else if (readingPaths) {
			// We are inside a Path section, separate elements and store into a FilePath object
			const char *t = line.c_str();

			// Split string at ' ' into 'name' and 'full path'. First, find the " " delimiter.
			const char *sep = strchr(t, ' ');
			if (!sep)
				error("ResourceManager::readPathFile: path file invalid: no valid separator between paths");

			// Now extract components from the full path
			Common::String fullpath(sep + 1);
			fullpath.trim();

			// Split string at ':' into 'media type' and 'directory'
			const char *p = fullpath.c_str();
			const char *sep2 = strchr(p, ':');

			// Extract the name/path pair
			FilePath path;
			Common::String filename(t, sep);
			path.folderName = Common::String(sep2 + 1);

			// Convert media type
			Common::String media(p, sep2);
			if (media == "cd")
				path.type = kMediaCd;
			else if (media == "hd")
				path.type = kMediaHd;
			else
				error("ResourceManager::readPathFile: Invalid media type (was:%s)", media.c_str());

			_files[filename] = path;
		}
	}

	debugC(2, kLiathDebugResource, "Loaded %d file paths", _files.size());

	delete pathFile;
}

/**
 * Reads the multigen.dat file. This contains the list of associations between archives and files
 */
void ResourceManager::readMultiData() {
	// Open a stream to the path file
	Common::File *multigenFile = new Common::File();
	if (!multigenFile->open("multigen.dat"))
		error("ResourceManager::readMultiData: Could not open multigen.dat file!");

	uint32 count = 0;

	while (!multigenFile->eos() && !multigenFile->err()) {

		char name[20];
		multigenFile->read(&name, sizeof(name));

		// Search for existing FilePath and update archive
		if (_files.contains(name)) {
			// Found an existing file with that name
			char archive[20];
			multigenFile->read(&archive, sizeof(archive));

			_files[name].archiveName = Common::String(archive);
			_files[name].indicator = multigenFile->readUint16LE();

			//debugC(kLiathDebugResource, "%s - %s", &name, (*path)->toString().c_str());

			count++;
		}
	}

	debugC(2, kLiathDebugResource, "Updated %d file paths with archive name", count);

	delete multigenFile;
}

//////////////////////////////////////////////////////////////////////////
// Data
//////////////////////////////////////////////////////////////////////////
Common::Array<FileData *> *ResourceManager::readData(Common::String name, uint32 index) {
	error("ResourceManager::readData: Not implemented!");
}

} // End of namespace Liath
