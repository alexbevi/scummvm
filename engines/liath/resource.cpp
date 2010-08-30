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

#include "liath/debug.h"

#include "common/archive.h"
#include "common/file.h"

namespace Liath {

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {
	_files.clear();
}

//////////////////////////////////////////////////////////////////////////
// Archives
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
			FilePath *path = new FilePath();
			Common::String filename(t, sep);
			path->directory = Common::String(sep2 + 1);

			// Convert media type
			Common::String media(p, sep2);
			if (media == "cd")
				path->type = kMediaCd;
			else if (media == "hd")
				path->type = kMediaHd;
			else
				error("ResourceManager::readPathFile: Invalid media type (was:%s)", media.c_str());

			_files[filename] = path;
		}
	}

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

	while (!multigenFile->eos() && !multigenFile->err()) {

		char name[20];
		multigenFile->read(&name, sizeof(name));

		// Search for existing FilePath and update archive
		if (_files.contains(name)) {
			// Found an existing file with that name
			char archive[20];
			multigenFile->read(&archive, sizeof(archive));

			_files[name]->archive = Common::String(archive);
			_files[name]->indicator = multigenFile->readUint16LE();

			//debugC(kLiathDebugResource, "%s - %s", &name, (*path)->toString().c_str());
		}
	}

	delete multigenFile;
}

//////////////////////////////////////////////////////////////////////////
// Data
//////////////////////////////////////////////////////////////////////////
Common::Array<FileData *> *ResourceManager::readData(Common::String name, uint32 index) {
	error("ResourceManager::readData: Not implemented!");
}

} // End of namespace Liath
