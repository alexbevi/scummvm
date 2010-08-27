#include "unity.h"
#include "sprite.h"
#include "object.h"
#include "common/system.h"
#include "trigger.h"

namespace Unity {

UnityData::~UnityData() {
	for (Common::HashMap<uint32, Object *>::iterator i = objects.begin();
		i != objects.end(); i++) {
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

Common::SeekableReadStream *UnityData::openFile(Common::String filename) {
	Common::SeekableReadStream *stream = SearchMan.createReadStreamForMember(filename);
	if (!stream) error("couldn't open '%s'", filename.c_str());
	return stream;
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

Object *UnityData::getObject(objectID id) {
	uint32 identifier = id.id + (id.screen << 8) + (id.world << 16);
	if (objects.contains(identifier)) {
		return objects[identifier];
	}
	Object *obj = new Object;
	obj->loadObject(*this, id.world, id.screen, id.id);
	objects[identifier] = obj;
	return obj;
}

} // Unity

