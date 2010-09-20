#ifndef _ORIGDATA_H
#define _ORIGDATA_H

namespace Unity {

// some data (strings, coordinates, etc) is hardcoded into the executable..
// this presumably only works for the DOS English executable right now.

// offset of the data segment
#define OVERLAY_OFFSET_DOS 0x5fea4
#define DATA_SEGMENT_OFFSET_DOS (OVERLAY_OFFSET_DOS + 0xf0000)

// bridge items (coordinates, name)
#define BRIDGE_ITEM_OFFSET_DOS 0x6fb54

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
	uint32 x, y;
	int32 unknown1;
	uint32 unknown2;
};

#define NUM_BRIDGE_SCREEN_ENTRIES 3
#define BRIDGE_SCREEN_ENTRY_SIZE 8
struct BridgeScreenEntry {
	Common::String text;
	uint32 unknown;
};

#define FAIL_HAIL_OFFSET_DOS 0x77544
#define FAIL_HAIL_ENTRY_SIZE 16
struct FailHailEntry {
	uint32 action_id;
	objectID source;
	uint32 fail_flag;
	Common::String hail;
};

#define AWAY_TEAM_DATA_OFFSET_DOS 0x7025c
#define NUM_AWAY_TEAM_DATA 8
struct AwayTeamScreenData {
	Common::Array<objectID> default_members; // up to 4, always terminated by -1
	Common::Array<objectID> inventory_items; // offset, terminated by -1
};

#define TRANSPORTER_SPRITE_NAMES_OFFSET_DOS 0x703e4
#define NUM_TRANSPORTER_SPRITE_NAMES 9

#define TRANSPORTER_SCREEN_ENTRY_OFFSET_DOS 0x70408

#define PRESET_SOUND_OFFSET_DOS 0x12f64
#define NUM_PRESET_SOUNDS 60
struct PresetSound {
	uint32 id;
	Common::String filename;
};

#define ADVICE_NAMES_OFFSET_DOS 0x6890C
#define NUM_ADVICE_NAMES 9
struct AdviceName {
	objectID id;
	Common::String name;
};

// for 4 actions (look at, walk to, talk to, use)
#define ACTION_DEFAULT_STRINGS_OFFSET 0x688A8

// like adint%02d.rac, with 1 and 4
#define BACKGROUND_SOUND_DEFAULTS_OFFSET 0x688B8
#define NUM_BACKGROUND_SOUND_DEFAULTS 7
#define BACKGROUND_SOUND_DEFAULT_ENTRY_SIZE 12
struct BackgroundSoundDefault {
	Common::String format_string;
	uint32 first;
	uint32 last;
};

} // Unity

#endif

