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
 * $URL: https://scummvm-dgds.googlecode.com/svn/trunk/resources/resource.cpp $
 * $Id: resource.cpp 25 2010-02-02 03:59:10Z alexbevi $
 *
 */

#include "dgds/resource.h"

#include "common/file.h"

namespace Dgds {

Resource::Resource(Common::String name, Common::SeekableReadStream *stream, bool hasSubres) :
	_name(name),
	_stream(stream) {

	assert (stream);

	if (hasSubres) {
		// Find the tags
		int32 currentPos = 0;

		while (currentPos + 4 < _stream->size()) {
			ResourceInfo current;
			current.offset = currentPos + 8;

			// Go to the beginning of the new resource
			_stream->seek(currentPos);

			char tmp[4];
			_stream->read(tmp, 4);

			if (tmp[3] != ':') {
				printf("TAG error\n");
				return;
			}

			Common::String tag(tmp, 3);

			// Get the size of the resource
			current.size = _stream->readUint32LE();

			if (current.size & (1 << 31)) {
				// This includes sub-resources
				current.size -= 1 << 31;
				current.hasSubres = true;
			} else {
				current.hasSubres = false;
			}

			currentPos = current.offset + current.size;
			current.tag = tag;
			_subResources.push_back(current);
		}
	}
}

Resource::~Resource() {
	delete _stream;
	_subResources.clear();
}

void Resource::listTags() {
	for (uint32 i = 0; i < _subResources.size(); i++) {
		printf("%s\n", _subResources[i].tag.c_str());
		break;
	}
}

Common::Array<Resource *> Resource::getSubResourceCollection(Common::String const &tag) {
	Common::Array<Resource *> col;
	printf("%d\n", _subResources.size());

	return col;
}

Resource *Resource::hasSubResource(Common::String const &tag) {
	for (uint32 i = 0; i < _subResources.size(); i++) {
		ResourceInfo sub = _subResources[i];
		if (sub.tag == tag)
			return getSubResource(sub);
	}

	return 0;
}

Resource *Resource::getSubResource(Common::String const &tag) {
	Common::String tag1(tag.c_str(), 3);
	Resource *sub = hasSubResource(tag1);

	if (sub)
		return sub;
	else {
		warning("tag [%s] not found", tag1.c_str());
		return NULL;
	}
}

Resource *Resource::getSubResource(ResourceInfo const &subResourceInfo) {
	// Return the substream corresponding to the subresource
	Common::SeekableSubReadStream *stream = new Common::SeekableSubReadStream(_stream,
			subResourceInfo.offset,
			subResourceInfo.offset + subResourceInfo.size);
	Resource *sub = new Resource(_name + "." + subResourceInfo.tag, stream, subResourceInfo.hasSubres);

	return sub;
}

void Resource::dump(Common::String const &outFilename, bool dumpSubres) {
	if (dumpSubres && _subResources.size() > 0) {
		for (uint32 i = 0; i < _subResources.size(); i++) {
			Resource *subres = getSubResource(_subResources[i]);
			// append the iterator value to the filename to handle
			// subresource collections (so filenames aren't overwritten)
			subres->dump(outFilename + "." + _subResources[i].tag + i, dumpSubres);
			delete subres;
		}
	} else {
		//printf("%s\n", outFilename.c_str());

		// Go to the beginning of the resource
		_stream->seek(0);

		Common::DumpFile outFile;
		outFile.open(outFilename);

		char* fileContents = new char[1024];
		int len;
		for (int i = _stream->size(); i > 0; i -= len) {
			len = _stream->read(fileContents, 1024 < i ? 1024 : i);
			outFile.write(fileContents, len);
		}
		delete fileContents;

		outFile.close();
	}
}

Common::String Resource::to_s(bool rewind) {
	Common::String out = "";

	if (rewind)
		_stream->seek(0);
	while (!_stream->eos()) {
		byte b = _stream->readByte();
		if (b)
			out += Common::String(b);
		else
			break;
	}

	return out;
}

} // End of namespace Dgds
