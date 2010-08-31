#ifndef _ORIGDATA_H
#define _ORIGDATA_H

namespace Unity {

// some data (strings, coordinates, etc) is hardcoded into the executable..
// this presumably only works for the DOS English executable right now.

// offset of the data segment
#define OVERLAY_OFFSET 0x5fea4
#define DATA_SEGMENT_OFFSET (OVERLAY_OFFSET + 0xf0000)

// bridge items (coordinates, name)
#define BRIDGE_ITEM_OFFSET 0x6fb54

#define NUM_BRIDGE_ITEMS 11
#define BRIDGE_ITEM_SIZE 36
struct BridgeItem {
	Common::String description;
	objectID id;
	uint32 x, y;
	uint32 width, height;
	uint32 unknown1;
	uint32 unknown2;
	uint32 unknown3; // icon?
};

#define NUM_BRIDGE_OBJECTS 5
#define BRIDGE_OBJECT_SIZE 24
struct BridgeObject {
	objectID id;
	Common::String filename;
	uint32 unknown1;
	uint32 unknown2;
	uint32 unknown3;
	uint32 unknown4;
};

#define NUM_BRIDGE_SCREEN_ENTRIES 3
#define BRIDGE_SCREEN_ENTRY_SIZE 8
struct BridgeScreenEntry {
	Common::String text;
	uint32 unknown;
};

} // Unity

#endif

