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

#ifndef _UNITY_DATA_H
#define _UNITY_DATA_H

#include "common/stream.h"
#include "common/archive.h"
#include "common/rect.h"
#include "common/hashmap.h"

#include "object.h"
#include "origdata.h"

namespace Unity {

class Graphics;
class Sound;
class SpritePlayer;
class Object;
class Trigger;

struct Triangle {
	Common::Point points[3];
	uint16 distances[3];
};

struct ScreenPolygon {
	unsigned int id;
	byte type;

	Common::Array<Common::Point> points;
	Common::Array<uint16> distances;

	Common::Array<Triangle> triangles;

	bool insideTriangle(unsigned int x, unsigned int y, unsigned int &triangle);
};

struct Screen {
	unsigned int world, screen;

	Common::Array<Common::Array<Common::Point> > entrypoints;

	Common::Array<ScreenPolygon> polygons;
	Common::Array<Object *> objects;
};

struct ComputerEntry {
	uint16 flags;
	Common::String title, heading, text;
	Common::Array<uint> subentries;
	uint16 imageWidth, imageHeight;
	byte *imageData;
};

class UnityData {
protected:
	class UnityEngine *_vm;

public:
	UnityData(UnityEngine *p) : _vm(p) { }
	~UnityData();

	// data file access
	Common::Archive *data, *instdata;
	Common::SeekableReadStream *openFile(Common::String filename);

	// current away team screen
	Screen current_screen;
	void loadScreenPolys(Common::String filename);

	// triggers
	Common::Array<Trigger *> triggers;
	void loadTriggers();
	Trigger *getTrigger(uint32 id);

	// all objects
	Common::HashMap<uint32, Object *> objects;
	Object *getObject(objectID id);

	// sprite filenames
	Common::Array<Common::String> sprite_filenames;
	void loadSpriteFilenames();
	Common::String getSpriteFilename(unsigned int id);

	// sector names
	Common::Array<Common::String> sector_names;
	void loadSectorNames();
	Common::String getSectorName(unsigned int x, unsigned int y, unsigned int z);

	// icon sprites
	Common::HashMap<uint32, Common::String> icon_sprites;
	void loadIconSprites();
	Common::String getIconSprite(objectID id);

	// movie info
	Common::HashMap<unsigned int, Common::String> movie_filenames;
	Common::HashMap<unsigned int, Common::String> movie_descriptions;
	void loadMovieInfo();

	// computer database
	Common::Array<ComputerEntry> _computerEntries;
	void loadComputerDatabase();

	// hardcoded data
	Common::Array<BridgeItem> bridge_items;
	Common::Array<BridgeObject> bridge_objects;
	Common::Array<BridgeScreenEntry> bridge_screen_entries;
	Common::Array<FailHailEntry> fail_hail_entries;
	Common::Array<AwayTeamScreenData> away_team_screen_data;
	Common::Array<Common::String> transporter_sprite_names;
	Common::HashMap<uint32, Common::String> preset_sounds;
	Common::HashMap<uint32, Common::String> advice_names;
	Common::Array<Common::String> action_strings;
	Common::Array<BackgroundSoundDefault> background_sound_defaults;
	void loadExecutableData();

	Common::HashMap<unsigned int, Common::HashMap<unsigned int, Conversation *>*> conversations;
	Conversation *getConversation(unsigned int world, unsigned int id);
};

} // Unity

#endif

