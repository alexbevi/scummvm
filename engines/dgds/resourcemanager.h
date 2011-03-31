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
 * $URL: https://scummvm-dgds.googlecode.com/svn/trunk/resourcemanager.h $
 * $Id: resourcemanager.h 12 2010-01-23 12:01:58Z alexbevi $
 *
 */

#ifndef DGDS_RESOURCEMANAGER_H
#define DGDS_RESOURCEMANAGER_H

#include "dgds/resource.h"

#include "common/file.h"
#include "common/hash-str.h"

namespace Dgds {

struct ResourceFileInfo : public ResourceInfo {
	Common::String fileName;
	uint32 hash;
};

typedef Common::HashMap<Common::String, ResourceFileInfo, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> ResourceFiles;

class ResourceManager {
public:
	ResourceManager(Common::String indexFilename);
	~ResourceManager();

	Resource *getResource(Common::String const &resourceName);

	Common::String findGDS(void);

	void dumpResources(Common::String const &path, bool subres);

	ResourceFiles _resourceFiles; //XXX: make private

private:
	Resource *getResource(Common::String const &resourceName, ResourceFileInfo const &resourceFileInfo);

	Common::String _currentOpenFilename;
	Common::File   _currentOpenFile;
};

} // End of namespace Dgds

#endif // DGDS_RESOURCEMANAGER_H
