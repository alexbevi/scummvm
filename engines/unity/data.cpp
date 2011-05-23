/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "unity.h"
#include "sprite_player.h"
#include "object.h"
#include "common/system.h"
#include "common/memstream.h"
#include "common/substream.h"
#include "common/macresman.h"
#include "common/fs.h"
#include "common/config-manager.h"
#include "common/textconsole.h"
#include "trigger.h"

namespace Unity {

UnityData::~UnityData() {
	for (uint i = 0; i < _computerEntries.size(); i++) {
		delete[] _computerEntries[i].imageData;
	}
	for (Common::HashMap<uint32, Object *>::iterator i = objects.begin();
		i != objects.end(); i++) {
		delete i->_value;
	}
	for (Common::HashMap<unsigned int, Common::HashMap<unsigned int, Conversation *>*>::iterator i = conversations.begin();
		i != conversations.end(); i++) {
		for (Common::HashMap<unsigned int, Conversation *>::iterator j = i->_value->begin();
			j != i->_value->end(); j++) {
			delete j->_value;
		}
		delete i->_value;
	}
}

void UnityData::loadScreenPolys(Common::String filename) {
	Common::SeekableReadStream *mrgStream = openFile(filename);

	uint16 num_entries = mrgStream->readUint16LE();
	Common::Array<uint32> offsets;
	Common::Array<uint32> ids;
	offsets.reserve(num_entries);
	for (unsigned int i = 0; i < num_entries; i++) {
		uint32 id = mrgStream->readUint32LE();
		ids.push_back(id);
		uint32 offset = mrgStream->readUint32LE();
		offsets.push_back(offset);
	}

	current_screen.polygons.clear();
	current_screen.polygons.reserve(num_entries);
	for (unsigned int i = 0; i < num_entries; i++) {
		ScreenPolygon poly;

		bool r = mrgStream->seek(offsets[i]);
		assert(r);

		poly.id = ids[i];
		poly.type = mrgStream->readByte();
		assert(poly.type == 0 || poly.type == 1 || poly.type == 3 || poly.type == 4);

		uint16 something2 = mrgStream->readUint16LE();
		assert(something2 == 0);

		byte count = mrgStream->readByte();
		for (unsigned int j = 0; j < count; j++) {
			uint16 x = mrgStream->readUint16LE();
			uint16 y = mrgStream->readUint16LE();

			// 0-256, higher is nearer (larger characters);
			// (maybe 0 means not shown at all?)
			uint16 distance = mrgStream->readUint16LE();
			assert(distance <= 0x100);

			poly.distances.push_back(distance);
			poly.points.push_back(Common::Point(x, y));
		}

		for (unsigned int p = 2; p < poly.points.size(); p++) {
			// make a list of triangle vertices (0, p - 1, p)
			// this makes the code easier to understand for now
			Triangle tri;
			tri.points[0] = poly.points[0];
			tri.distances[0] = poly.distances[0];
			tri.points[1] = poly.points[p - 1];
			tri.distances[1] = poly.distances[p - 1];
			tri.points[2] = poly.points[p];
			tri.distances[2] = poly.distances[p];
			poly.triangles.push_back(tri);
		}

		current_screen.polygons.push_back(poly);
	}

	delete mrgStream;
}

void UnityData::loadTriggers() {
	Common::SeekableReadStream *triggerstream = openFile("trigger.dat");

	while (true) {
		uint32 id = triggerstream->readUint32LE();
		if (triggerstream->eos()) break;

		Trigger *trigger = new Trigger;
		triggers.push_back(trigger);
		trigger->id = id;

		uint16 unused = triggerstream->readUint16LE();
		assert(unused == 0xffff);

		trigger->unknown_a = triggerstream->readByte(); // XXX

		byte unused2 = triggerstream->readByte();
		assert(unused2 == 0xff);

		trigger->type = triggerstream->readUint32LE();
		assert(trigger->type <= 3);
		trigger->target = readObjectID(triggerstream);
		byte enabled = triggerstream->readByte();
		assert(enabled == 0 || enabled == 1);
		trigger->enabled = (enabled == 1);

		trigger->unknown_b = triggerstream->readByte(); // XXX

		unused = triggerstream->readUint16LE();
		assert(unused == 0xffff);

		trigger->timer_start = triggerstream->readUint32LE();

		switch (trigger->type) {
			case TRIGGERTYPE_TIMER:
				{
					uint32 zero = triggerstream->readUint32LE();
					assert(zero == 0);
					zero = triggerstream->readUint32LE();
					assert(zero == 0);
				}
				break;

			case TRIGGERTYPE_PROXIMITY:
				{
					trigger->dist = triggerstream->readUint16LE();
					unused = triggerstream->readUint16LE();
					assert(unused == 0xffff);
					trigger->from = readObjectID(triggerstream);
					trigger->to = readObjectID(triggerstream);
					byte unknown1 = triggerstream->readByte();
					assert(unknown1 == 0 || unknown1 == 1);
					trigger->reversed = (unknown1 == 0);
					byte unknown2 = triggerstream->readByte();
					assert(unknown2 == 0 || unknown2 == 1);
					trigger->instant = (unknown2 == 1);
					unused = triggerstream->readUint16LE();
					assert(unused == 0xffff);
				}
				break;

			case TRIGGERTYPE_UNUSED:
				{
					uint32 zero = triggerstream->readUint32LE();
					assert(zero == 0);
				}
				break;
		}
	}

	delete triggerstream;
}

Trigger *UnityData::getTrigger(uint32 id) {
	for (unsigned int i = 0; i < triggers.size(); i++) {
		if (triggers[i]->id == id)
			return triggers[i];
	}

	return NULL;
}

Common::SeekableReadStream *UnityData::openFile(Common::String filename) {
	Common::SeekableReadStream *stream = SearchMan.createReadStreamForMember(filename);
	if (stream)
		return stream;

	Common::MacResManager macres;
	if (macres.open(filename)) {
		stream = macres.getDataFork();
		if (stream)
			return stream->readStream(stream->size());
	}

	error("couldn't open '%s'", filename.c_str());
}

void UnityData::loadSpriteFilenames() {
	Common::SeekableReadStream *stream = openFile("sprite.lst");

	uint32 num_sprites = stream->readUint32LE();
	sprite_filenames.reserve(num_sprites);

	for (unsigned int i = 0; i < num_sprites; i++) {
		char buf[9]; // DOS filenames, so 9 should really be enough
		for (unsigned int j = 0; j < 9; j++) {
			char c = stream->readByte();
			buf[j] = c;
			if (c == 0) break;
			assert (j != 8);
		}
		sprite_filenames.push_back(buf);
	}

	delete stream;
}

Common::String UnityData::getSpriteFilename(unsigned int id) {
	assert(id != 0xffff && id != 0xfffe);
	assert(id < sprite_filenames.size());
	return sprite_filenames[id] + ".spr";
}

void UnityData::loadSectorNames() {
	Common::SeekableReadStream *stream = openFile("sector.ast");

	for (unsigned int i = 0; i < 8*8*8; i++) {
		sector_names.push_back(stream->readLine());
	}

	delete stream;
}

Common::String UnityData::getSectorName(unsigned int x, unsigned int y, unsigned int z) {
	// sectors are 20*20*20, there are 8*8*8 sectors total, work out the index
	unsigned int sector_id = (x/20) + (y/20)*(8) + (z/20)*(8*8);
	return sector_names[sector_id];
}

void UnityData::loadIconSprites() {
	Common::SeekableReadStream *stream = openFile("icon.map");

	while (!stream->eos() && !stream->err()) {
		Common::String line = stream->readLine();

		Common::String id;
		Common::String str;
		for (unsigned int i = 0; i < line.size(); i++) {
			if (line[i] == '#') {
				break;
			} else if (line[i] == ' ' && !id.size()) {
				id = str;
				str.clear();
			} else {
				str += line[i];
			}
		}
		str.trim();
		if (!str.size()) continue; // commented-out or blank line

		char *parseEnd;
		uint32 identifier = strtol(id.c_str(), &parseEnd, 16);
		if (*parseEnd != 0) {
			warning("failed to parse '%s' from icon.map", line.c_str());
			continue;
		}

		// irritatingly, they use '0' to disable things, but object 0 is Picard..
		if (icon_sprites.contains(identifier)) continue;

		icon_sprites[identifier] = str;
	}

	delete stream;
}

Common::String UnityData::getIconSprite(objectID id) {
	uint32 identifier = id.id + (id.screen << 8) + (id.world << 16);
	return icon_sprites[identifier];
}

void UnityData::loadMovieInfo() {
	Common::SeekableReadStream *stream = openFile("movie.map");

	while (!stream->eos() && !stream->err()) {
		Common::String line = stream->readLine();

		Common::String id;
		Common::String filename;
		Common::String str;
		for (unsigned int i = 0; i < line.size(); i++) {
			if (line[i] == '#') {
				break;
			} else if (line[i] == ' ' && !str.size()) {
				// swallow spaces
			} else if (line[i] == ' ' && !id.size()) {
				id = str;
				str.clear();
			} else if (line[i] == ' ' && !filename.size()) {
				filename = str;
				str.clear();
			} else {
				str += line[i];
			}
		}
		id.trim();
		if (!id.size()) continue; // commented-out or blank line

		assert(filename.size());

		char *parseEnd;
		uint32 identifier = strtol(id.c_str(), &parseEnd, 10);
		if (*parseEnd != 0) {
			error("failed to parse '%s' from movie.map", line.c_str());
			continue;
		}

		if (movie_filenames.contains(identifier)) {
			error("there are multiple movie.map entries for %d", identifier);
		}

		movie_filenames[identifier] = filename;
		movie_descriptions[identifier] = str;
	}

	delete stream;
}

static Common::String readStringFromOffset(Common::SeekableReadStream *stream, uint32 offset) {
	Common::String temp;
	stream->seek(offset, SEEK_SET);
	while (!stream->eos()) {
		byte b = stream->readByte();
		if (b == 0)
			break;
		temp += b;
	}
	return temp;
}

void UnityData::loadComputerDatabase() {
	Common::SeekableReadStream *stream = openFile("computer.db");
	uint32 numEntries = stream->readUint32LE();

	Common::Array<uint32> offsets;
	for (uint i = 0; i < numEntries; i++) {
		offsets.push_back(stream->readUint32LE());
	}
	// we skip one last offset, which marks the end of the file
	uint32 startOffset = 8 + 4 * offsets.size();

	for (uint i = 0; i < offsets.size(); i++) {
		ComputerEntry entry;

		stream->seek(startOffset + offsets[i], SEEK_SET);

		uint16 numSubentries = stream->readUint16LE();
		entry.flags = stream->readUint16LE();
		uint32 titleOffset = stream->readUint32LE();
		uint32 headingOffset = stream->readUint32LE();
		uint32 textOffset = stream->readUint32LE();
		uint32 imageOffset = stream->readUint32LE();
		for (uint j = 0; j < numSubentries; j++) {
			uint32 offset = stream->readUint32LE();

			// Look up the offset, store the index instead.
			uint32 entryId = 0; // the root entry is never a subentry
			for (uint k = 0; k < offsets.size(); k++) {
				if (offsets[k] != offset)
					continue;

				entryId = k;
				break;
			}
			if (entryId == 0)
				error("corrupt computer.db (couldn't find subentry %d (of %d) of entry %d)", j, numSubentries, i);
			entry.subentries.push_back(entryId);
		}

		if (!titleOffset || !headingOffset || !textOffset)
			error("invalid computer.db (missing offsets)");
		entry.title = readStringFromOffset(stream, startOffset + titleOffset);
		entry.heading = readStringFromOffset(stream, startOffset + headingOffset);
		entry.text = readStringFromOffset(stream, startOffset + textOffset);
		if (imageOffset) {
			stream->seek(startOffset + imageOffset, SEEK_SET);
			entry.imageWidth = stream->readUint16LE();
			entry.imageHeight = stream->readUint16LE();
			entry.imageData = new byte[entry.imageWidth * entry.imageHeight];
		} else {
			entry.imageData = NULL;
		}

		debug(6, "Computer entry %d: title '%s', heading '%s'", i, entry.title.c_str(), entry.heading.c_str());
		_computerEntries.push_back(entry);
	}

	delete stream;
}

Object *UnityData::getObject(objectID id) {
	uint32 identifier = id.id + (id.screen << 8) + (id.world << 16);
	if (objects.contains(identifier)) {
		return objects[identifier];
	}
	Object *obj = new Object(_vm);
	obj->loadObject(id.world, id.screen, id.id);
	objects[identifier] = obj;
	return obj;
}

Common::String readStringFromOffset(Common::SeekableReadStream *stream, int32 offset) {
	bool r = stream->seek(offset, SEEK_SET);
	assert(r);

	Common::String s;
	byte b;
	while ((b = stream->readByte()) != 0) {
		s += (char)b;
	}

	return s;
}

uint32 getPEFArgument(Common::SeekableReadStream *stream, unsigned int &pos) {
	uint32 r = 0;
	byte num_entries = 0;
	while (true) {
		num_entries++;

		byte in = stream->readByte();
		pos++;

		if (num_entries == 5) {
			r <<= 4;
		} else {
			r <<= 7;
		}
		r += (in & 0x7f);
		if (!(in & 0x80)) return r;

		if (num_entries == 5) error("bad argument in PEF");
	}
}

// so, the data segment in a PEF is compressed! whoo!
Common::SeekableReadStream *decompressPEFDataSegment(Common::SeekableReadStream *stream, unsigned int segment_id) {
	uint32 tag1 = stream->readUint32BE();
	if (tag1 != MKTAG('J','o','y','!')) error("bad PEF tag1");
	uint32 tag2 = stream->readUint32BE();
	if (tag2 != MKTAG('p','e','f','f')) error("bad PEF tag2");
	uint32 architecture = stream->readUint32BE();
	if (architecture != MKTAG('p','w','p','c')) error("PEF header is not powerpc");
	uint32 formatVersion = stream->readUint32BE();
	if (formatVersion != 1) error("PEF header is not version 1");
	stream->skip(16); // dateTimeStamp, oldDefVersion, oldImpVersion, currentVersion
	uint16 sectionCount = stream->readUint16BE();
	stream->skip(6); // instSectionCount, reservedA

	if (segment_id >= sectionCount) error("not enough segments in PEF");

	stream->skip(28 * segment_id);

	stream->skip(8); // nameOffset, defaultAddress
	uint32 totalSize = stream->readUint32BE();
	uint32 unpackedSize = stream->readUint32BE();
	uint32 packedSize = stream->readUint32BE();
	assert(unpackedSize <= totalSize);
	assert(packedSize <= unpackedSize);
	uint32 containerOffset = stream->readUint32BE();
	byte sectionKind = stream->readByte();
	switch (sectionKind) {
	case 2: break; // pattern-initialized data
	default: error("unsupported PEF sectionKind %d", sectionKind);
	}

	debug(1, "unpacking PEF segment of size %d (total %d, packed %d) at 0x%x", unpackedSize, totalSize, packedSize, containerOffset);

	bool r = stream->seek(containerOffset, SEEK_SET);
	assert(r);

	// note that we don't bother with the zero-initialised section..
	byte *data = (byte *)malloc(unpackedSize);

	// unpack the data
	byte *targ = data;
	unsigned int pos = 0;
	while (pos < packedSize) {
		byte next = stream->readByte();
		byte opcode = next >> 5;
		uint32 count = next & 0x1f;
		pos++;

		if (count == 0) {
			count = getPEFArgument(stream, pos);
		}

		switch (opcode) {
		case 0: // Zero
			memset(targ, 0, count);
			targ += count;
			break;

		case 1: // blockCopy
			stream->read(targ, count);
			targ += count;
			pos += count;
			break;

		case 2:	{ // repeatedBlock
				uint32 repeatCount = getPEFArgument(stream, pos);

				byte *src = targ;
				stream->read(src, count);
				targ += count;
				pos += count;

				for (unsigned int i = 0; i < repeatCount; i++) {
					memcpy(targ, src, count);
					targ += count;
				}
			} break;

		case 3: { // interleaveRepeatBlockWithBlockCopy
				uint32 customSize = getPEFArgument(stream, pos);
				uint32 repeatCount = getPEFArgument(stream, pos);

				byte *commonData = targ;
				stream->read(commonData, count);
				targ += count;
				pos += count;

				for (unsigned int i = 0; i < repeatCount; i++) {
					stream->read(targ, customSize);
					targ += customSize;
					pos += customSize;

					memcpy(targ, commonData, count);
					targ += count;
				}
			} break;

		case 4: { // interleaveRepeatBlockWithZero
				uint32 customSize = getPEFArgument(stream, pos);
				uint32 repeatCount = getPEFArgument(stream, pos);

				for (unsigned int i = 0; i < repeatCount; i++) {
					memset(targ, 0, count);
					targ += count;

					stream->read(targ, customSize);
					targ += customSize;
					pos += customSize;
				}
				memset(targ, 0, count);
				targ += count;
			} break;

		default: error("unknown opcode %d in PEF pattern-initialized section", opcode);
		}
	}

	if (pos != packedSize) error("failed to parse PEF pattern-initialized section (parsed %d of %d)", pos, packedSize);
	if (targ != data + unpackedSize) error("failed to unpack PEF pattern-initialized section");

	delete stream;
	return new Common::MemoryReadStream(data, unpackedSize, DisposeAfterUse::YES);
}

class SeekableReadStreamWrapperEndian : virtual public Common::SeekableReadStream {
protected:
	Common::SeekableReadStream *_parentStream;
	const bool _bigEndian;
	const bool _disposeParentStream;

public:
	SeekableReadStreamWrapperEndian(Common::SeekableReadStream *parentStream, bool bigEndian,
		DisposeAfterUse::Flag disposeParentStream = DisposeAfterUse::NO) :
		_parentStream(parentStream), _bigEndian(bigEndian), _disposeParentStream(disposeParentStream) { }
	~SeekableReadStreamWrapperEndian() { if (_disposeParentStream) delete _parentStream; }

	// Stream
	bool err() const { return _parentStream->err(); }
	void clearErr() const { _parentStream->clearErr(); }

	// ReadStream
	bool eos() const { return _parentStream->eos(); }
	uint32 read(void *dataPtr, uint32 dataSize) { return _parentStream->read(dataPtr, dataSize); }

	// SeekableReadStream
	int32 pos() const { return _parentStream->pos(); }
	int32 size() const { return _parentStream->size(); }
	bool seek(int32 offset, int whence = SEEK_SET) { return _parentStream->seek(offset, whence); }
	char *readLine(char *s, size_t bufSize) { return _parentStream->readLine(s, bufSize); }
	Common::String readLine() { return _parentStream->readLine(); }

	uint16 readUint16() {
		uint16 val;
		read(&val, 2);
		return (_bigEndian) ? TO_BE_16(val) : TO_LE_16(val);
	}

	uint32 readUint32() {
		uint32 val;
		read(&val, 4);
		return (_bigEndian) ? TO_BE_32(val) : TO_LE_32(val);
	}

	FORCEINLINE int16 readSint16() {
		return (int16)readUint16();
	}

	FORCEINLINE int32 readSint32() {
		return (int32)readUint32();
	}
};

objectID readObjectIDEndian(Common::SeekableReadStream *stream, bool bigEndian) {
	if (bigEndian) return readObjectIDBE(stream);
	else return readObjectID(stream);
}

void UnityData::loadExecutableData() {
	// TODO: check md5sum/etc
	Common::SeekableReadStream *base_stream;
	Common::MacResManager macres;
	bool isMac = false;
	if (SearchMan.hasFile("sttng.ovl")) {
		Common::SeekableReadStream *ovl_stream = openFile("sttng.ovl");
		base_stream = new Common::SeekableSubReadStream(ovl_stream, DATA_SEGMENT_OFFSET_DOS, ovl_stream->size(), DisposeAfterUse::YES);
	} else {
		if (!macres.open("Star Trek TNG/\"A Final Unity\""))
			error("couldn't find sttng.ovl (DOS) or \"A Final Unity\" (Mac)");
		isMac = true;
		// we use the powerpc data segment, in the data fork
		base_stream = decompressPEFDataSegment(macres.getDataFork(), 1);
	}

	SeekableReadStreamWrapperEndian *stream = new SeekableReadStreamWrapperEndian(base_stream, isMac, DisposeAfterUse::YES);

	// bridge data
	int32 offset = BRIDGE_ITEM_OFFSET_DOS;
	if (isMac) offset = BRIDGE_ITEM_OFFSET_MAC;

	for (unsigned int i = 0; i < NUM_BRIDGE_ITEMS; i++) {
		bool r = stream->seek(offset, SEEK_SET);
		assert(r);

		BridgeItem item;
		uint32 desc_offset = stream->readUint32();
		item.id = readObjectIDEndian(stream, isMac);
		item.x = stream->readUint32();
		item.y = stream->readUint32();
		item.width = stream->readUint32();
		item.height = stream->readUint32();
		item.unknown1 = stream->readUint32();
		item.unknown2 = stream->readUint32();
		item.unknown3 = stream->readUint32();
		item.description = readStringFromOffset(stream, desc_offset);
		bridge_items.push_back(item);

		offset += BRIDGE_ITEM_SIZE;
	}

	if (isMac) offset = BRIDGE_OBJECT_OFFSET_MAC;
	for (unsigned int i = 0; i < NUM_BRIDGE_OBJECTS; i++) {
		bool r = stream->seek(offset, SEEK_SET);
		assert(r);

		BridgeObject obj;
		obj.id = readObjectIDEndian(stream, isMac);
		uint32 desc_offset = stream->readUint32();
		obj.x = stream->readUint32();
		obj.y = stream->readUint32();
		obj.unknown1 = stream->readUint32();
		obj.unknown2 = stream->readUint32();
		obj.filename = readStringFromOffset(stream, desc_offset);
		bridge_objects.push_back(obj);

		offset += BRIDGE_OBJECT_SIZE;
	}

	if (isMac) offset = BRIDGE_SCREEN_ENTRY_OFFSET_MAC;
	for (unsigned int i = 0; i < NUM_BRIDGE_SCREEN_ENTRIES; i++) {
		bool r = stream->seek(offset, SEEK_SET);
		assert(r);

		BridgeScreenEntry entry;
		uint32 desc_offset = stream->readUint32();
		entry.unknown = stream->readUint32();
		entry.text = readStringFromOffset(stream, desc_offset);
		bridge_screen_entries.push_back(entry);

		offset += BRIDGE_SCREEN_ENTRY_SIZE;
	}

	offset = FAIL_HAIL_OFFSET_DOS;
	if (isMac) offset = FAIL_HAIL_OFFSET_MAC;
	while (true) {
		bool r = stream->seek(offset, SEEK_SET);
		assert(r);

		FailHailEntry entry;
		entry.action_id = stream->readUint32();
		if (entry.action_id == 0xffffffff) break;
		entry.source = readObjectIDEndian(stream, isMac);
		entry.fail_flag = stream->readUint32();
		uint32 hail_offset = stream->readUint32();
		entry.hail = readStringFromOffset(stream, hail_offset);
		fail_hail_entries.push_back(entry);

		offset += FAIL_HAIL_ENTRY_SIZE;
	}

	offset = AWAY_TEAM_DATA_OFFSET_DOS;
	if (isMac) offset = AWAY_TEAM_DATA_OFFSET_MAC;
	for (unsigned int i = 0; i < NUM_AWAY_TEAM_DATA; i++) {
		bool r = stream->seek(offset, SEEK_SET);
		assert(r);

		AwayTeamScreenData entry;
		bool reading_members = true;
		for (unsigned int j = 0; j < 5; j++) {
			objectID default_member;
			default_member = readObjectIDEndian(stream, isMac);

			if (!reading_members) continue;
			if (default_member.id == 0xff) { reading_members = false; continue; }
			if (j == 4) error("too many default away team members");
			entry.default_members.push_back(default_member);
		}

		uint32 allowed_inv_offset = stream->readUint32();
		r = stream->seek(allowed_inv_offset, SEEK_SET);
		assert(r);
		while (true) {
			objectID allowed_inv;
			allowed_inv = readObjectIDEndian(stream, isMac);
			if (allowed_inv.id == 0xff) break;
			entry.inventory_items.push_back(allowed_inv);
		}
		away_team_screen_data.push_back(entry);

		offset += (5 + 1) * 4;
	}

	offset = TRANSPORTER_SPRITE_NAMES_OFFSET_DOS;
	if (isMac) offset = TRANSPORTER_SPRITE_NAMES_OFFSET_MAC;
	for (unsigned int i = 0; i < NUM_TRANSPORTER_SPRITE_NAMES; i++) {
		bool r = stream->seek(offset, SEEK_SET);
		assert(r);

		uint32 sprite_name_offset = stream->readUint32();
		transporter_sprite_names.push_back(readStringFromOffset(stream, sprite_name_offset));
		offset += 4;
	}

	offset = PRESET_SOUND_OFFSET_DOS;
	if (isMac) offset = PRESET_SOUND_OFFSET_MAC;
	for (unsigned int i = 0; i < NUM_PRESET_SOUNDS; i++) {
		bool r = stream->seek(offset, SEEK_SET);
		assert(r);

		uint32 preset_sound_id = stream->readUint32();
		uint32 preset_sound_offset = stream->readUint32();
		if (preset_sounds.contains(preset_sound_id)) error("duplicate sound id %d", preset_sound_id);
		preset_sounds[preset_sound_id] = readStringFromOffset(stream, preset_sound_offset);

		offset += 8;
	}

	offset = ADVICE_NAMES_OFFSET_DOS;
	if (isMac) offset = ADVICE_NAMES_OFFSET_MAC;
	for (unsigned int i = 0; i < NUM_ADVICE_NAMES; i++) {
		bool r = stream->seek(offset, SEEK_SET);
		assert(r);

		uint32 advice_offset = stream->readUint32();
		uint32 advice_id = stream->readUint32();
		if (advice_names.contains(advice_id)) error("duplicate advice id %d", advice_id);
		advice_names[advice_id] = readStringFromOffset(stream, advice_offset);

		offset += 8;
	}

	offset = ACTION_DEFAULT_STRINGS_OFFSET_DOS;
	if (isMac) offset = ACTION_DEFAULT_STRINGS_OFFSET_MAC;
	for (unsigned int i = 0; i < 4; i++) {
		bool r = stream->seek(offset, SEEK_SET);
		assert(r);

		uint32 action_offset = stream->readUint32();
		action_strings.push_back(readStringFromOffset(stream, action_offset));

		offset += 4;
	}

	offset = BACKGROUND_SOUND_DEFAULTS_OFFSET_DOS;
	if (isMac) offset = BACKGROUND_SOUND_DEFAULTS_OFFSET_MAC;
	for (unsigned int i = 0; i < NUM_BACKGROUND_SOUND_DEFAULTS; i++) {
		bool r = stream->seek(offset, SEEK_SET);
		assert(r);

		BackgroundSoundDefault entry;
		uint32 format_offset = stream->readUint32();
		entry.first = stream->readUint32();
		entry.last = stream->readUint32();
		if (format_offset != 0) {
			entry.format_string = readStringFromOffset(stream, format_offset);
		}
		background_sound_defaults.push_back(entry);

		offset += BACKGROUND_SOUND_DEFAULT_ENTRY_SIZE;
	}

	delete stream;
}

Conversation *UnityData::getConversation(unsigned int world, unsigned int id) {
	if (!conversations.contains(world)) {
		conversations[world] = new Common::HashMap<unsigned int, Conversation *>();
	}
	if (!conversations[world]->contains(id)) {
		(*conversations[world])[id] = new Conversation();
		(*conversations[world])[id]->loadConversation(*this, world, id);
	}
	return (*conversations[world])[id];
}

} // Unity

