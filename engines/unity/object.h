#ifndef _OBJECT_H
#define _OBJECT_H

#include "common/array.h"
#include "common/str.h"

namespace Common {
	class SeekableReadStream;
}

namespace Unity {

class UnityEngine;
class SpritePlayer;

struct objectID {
	byte id;
	byte screen;
	byte world;
	byte unused;
};

objectID readObjectID(Common::SeekableReadStream *stream);

struct EntryList {
};

struct Description {
	Common::String text;
	uint32 entry_id;
	uint32 voice_group, voice_subgroup, voice_id;
};

class Object {
public:
	objectID id;
	unsigned int x, y;
	unsigned int width, height;
	uint16 z_adjust;
	bool active;
	bool scaled; // XXX
	SpritePlayer *sprite;

	Common::Array<Description> descriptions;
	EntryList use_entries, get_entries, look_entries, timer_entries;

	void loadObject(UnityEngine *_vm, unsigned int world, unsigned int screen, unsigned int id);

protected:
	int readBlockHeader(Common::SeekableReadStream *objstream);
	void readBlock(int type, Common::SeekableReadStream *objstream);
	void readEntryList(Common::SeekableReadStream *objstream, EntryList &entries);
	void readEntry(int type, Common::SeekableReadStream *objstream, EntryList &entries);
	void readDescriptions(Common::SeekableReadStream *objstream);
	void readDescriptionBlock(Common::SeekableReadStream *objstream);
};

} // Unity

#endif

