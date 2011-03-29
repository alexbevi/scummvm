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
 * $URL: https://scummvm-dgds.googlecode.com/svn/trunk/resourcemanager.cpp $
 * $Id: resourcemanager.cpp 12 2010-01-23 12:01:58Z alexbevi $
 *
 */

#include "dgds/resourcemanager.h"

#include "common/fs.h"

namespace Dgds {

#define FILENAME_LEN 13

ResourceManager::ResourceManager(Common::String indexFilename) {
	Common::File indexFile;

	if (!Common::File::exists(indexFilename))
		return;

	indexFile.open(indexFilename);

	// Verify the file signature
	char tmp[FILENAME_LEN];
	indexFile.read(tmp, 4);
	if (tmp[0] != 0x0 || tmp[1] != 0x2 || tmp[2] != 0x4 || tmp[3] != 0x7) {
		printf("Unknown index file signature: %x %x %x %x\n", tmp[0], tmp[1], tmp[2], tmp[3]);
		//exit(1);
	}

	// Get the number of volumes
	uint16 nVolumes = indexFile.readUint16LE();

	for (int volume = 0; volume < nVolumes; volume++) {
		// Get the name of the volume file
		indexFile.read(tmp, FILENAME_LEN);
		Common::String volumeFilename = tmp;

		// Number of files in the volume
		uint16 nFiles = indexFile.readUint16LE();

		// Open the volume file
		Common::File volumeFile;
		volumeFile.open(volumeFilename);

		for (int i = 1; i <= nFiles; i++) {
			ResourceFileInfo resourceFileInfo;
			resourceFileInfo.fileName = volumeFilename;

			// Hash of the resource name
			resourceFileInfo.hash = indexFile.readUint32LE();

			// Position inside the volume file
			resourceFileInfo.offset = indexFile.readUint32LE();


			// Find the resource inside the volume
			volumeFile.seek(resourceFileInfo.offset);

			// Read the resource filename
			volumeFile.read(tmp, FILENAME_LEN);
			int len = strlen(tmp);
			if (len > 12)
				len = 12;
			Common::String filename(tmp, len);

			// Get the size of the resource
			resourceFileInfo.size = volumeFile.readUint32LE();

			if (resourceFileInfo.size == (uint32)-1)
				resourceFileInfo.size = 0;

			// Adjust the beginning of the contents to skip the previous read information
			resourceFileInfo.offset += FILENAME_LEN + 4;

			/*
			printf("%s/%s: hash(%x) offset(%x) size(%x)\n",
				resourceFileInfo.fileName.c_str(), filename.c_str(),
				resourceFileInfo.hash, resourceFileInfo.offset, resourceFileInfo.size);
			*/

			// See if there's a tag
			volumeFile.read(tmp, 4);
			if (tmp[3] == ':')
				resourceFileInfo.hasSubres = true;
			else
				resourceFileInfo.hasSubres = false;

			_resourceFiles.setVal(filename, resourceFileInfo);
		}

		volumeFile.close();
	}

	indexFile.close();
}

ResourceManager::~ResourceManager() {
	// Close the posibly open file
	if (_currentOpenFile.isOpen())
		_currentOpenFile.close();
}

Resource *ResourceManager::getResource(Common::String const &resourceName) {
	Resource *res = NULL;

	if (Common::File::exists(resourceName)) {
		// Open the resource file
		Common::File* file = new Common::File();
		file->open(resourceName);

		// See if there's a tag
		char tmp[4];
		file->read(tmp, 4);

		res = new Resource(file, tmp[3] == ':');
	} else if (_resourceFiles.contains(resourceName)) {
		// Return the resource from the volume files
		ResourceFileInfo resourceFileInfo = _resourceFiles.getVal(resourceName);
		res = getResource(resourceFileInfo);
	}

	return res;
}

Resource *ResourceManager::getResource(ResourceFileInfo const &resourceFileInfo) {
	if (!_currentOpenFile.isOpen() || _currentOpenFilename != resourceFileInfo.fileName) {
		// Close the old opened file
		if (_currentOpenFile.isOpen())
			_currentOpenFile.close();

		// Open the requested file
		_currentOpenFile.open(resourceFileInfo.fileName);
	}

	// Return the substream corresponding to the resource
	return new Resource(new Common::SeekableSubReadStream(&_currentOpenFile, resourceFileInfo.offset,
		resourceFileInfo.offset + resourceFileInfo.size), resourceFileInfo.hasSubres);
}

void ResourceManager::dumpResources(Common::String const &path, bool subres) {
	Common::FSNode node(path);
	if (!node.isDirectory()) {
		printf("%s isn't a directory. Please create it.\n", path.c_str());
		return;
	}

	// Read the files
	ResourceFiles::const_iterator iter = _resourceFiles.begin();

	while (iter != _resourceFiles.end()) {
		Resource *res = getResource(iter->_value);
		res->dump(path + iter->_key, subres);
		delete res;

		++iter;
	}
}

} // End of namespace Dgds
