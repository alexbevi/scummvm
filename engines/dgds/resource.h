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
 * $URL: https://scummvm-dgds.googlecode.com/svn/trunk/resources/resource.h $
 * $Id: resource.h 25 2010-02-02 03:59:10Z alexbevi $
 *
 */

#ifndef DGDS_RESOURCE_H
#define DGDS_RESOURCE_H

#include "common/substream.h"
#include "common/array.h"
#include "common/str.h"
#include "common/debug.h"

namespace Dgds {

struct ResourceInfo {
	Common::String tag;
	uint32 offset;
	uint32 size;
	bool hasSubres;
};

class Resource : public Common::SeekableReadStream {
public:
	Resource(Common::String name, Common::SeekableReadStream *stream, bool hasSubres);
	~Resource();

	const char *getName() {
		return _name.c_str();
	}

	Resource *getSubResource(Common::String const &tag);
	Common::Array<Resource *> getSubResourceCollection(Common::String const &tag);

	void dump(Common::String const &outFilename, bool dumpSubres);

	// SeekableReadStream members
	bool   eos() const { return _stream->eos(); }
	uint32 read(void *dataPtr, uint32 dataSize) { return _stream->read(dataPtr, dataSize); }
	int32  pos() const { return _stream->pos(); }
	int32  size() const { return _stream->size(); }
	bool   seek(int32 offset, int whence = SEEK_SET) { return _stream->seek(offset, whence); }

	/**
	 * Process the stream contents as a null-terminated string
	 *
	 * @param rewind indicates if the stream should be read from
	 *               the beginning, or from the current pos()
	 */
	Common::String to_s(bool rewind = true);

	void listTags();

protected:
	Resource *hasSubResource(Common::String const &tag);

private:
	Resource *getSubResource(ResourceInfo const &subResourceInfo);

	Common::String _name;
	Common::SeekableReadStream *_stream;
	Common::Array<ResourceInfo> _subResources;
};

} // End of namespace Dgds

#endif
