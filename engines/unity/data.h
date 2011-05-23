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
	Common::Archive *_data, *_instData;
	Common::SeekableReadStream *openFile(Common::String filename);

	// current away team screen
	Screen _currentScreen;
	void loadScreenPolys(Common::String filename);

	// triggers
	Common::Array<Trigger *> _triggers;
	void loadTriggers();
	Trigger *getTrigger(uint32 id);

	// all objects
	Common::HashMap<uint32, Object *> _objects;
	Object *getObject(objectID id);

	// sprite filenames
	Common::Array<Common::String> _spriteFilenames;
	void loadSpriteFilenames();
	Common::String getSpriteFilename(unsigned int id);

	// sector names
	Common::Array<Common::String> _sectorNames;
	void loadSectorNames();
	Common::String getSectorName(unsigned int x, unsigned int y, unsigned int z);

	// icon sprites
	Common::HashMap<uint32, Common::String> _iconSprites;
	void loadIconSprites();
	Common::String getIconSprite(objectID id);

	// movie info
	Common::HashMap<unsigned int, Common::String> _movieFilenames;
	Common::HashMap<unsigned int, Common::String> _movieDescriptions;
	void loadMovieInfo();

	// computer database
	Common::Array<ComputerEntry> _computerEntries;
	void loadComputerDatabase();

	// hardcoded data
	Common::Array<BridgeItem> _bridgeItems;
	Common::Array<BridgeObject> _bridgeObjects;
	Common::Array<BridgeScreenEntry> _bridgeScreenEntries;
	Common::Array<FailHailEntry> _failHailEntries;
	Common::Array<AwayTeamScreenData> _awayTeamScreenData;
	Common::Array<Common::String> _transporterSpriteNames;
	Common::HashMap<uint32, Common::String> _presetSounds;
	Common::HashMap<uint32, Common::String> _adviceNames;
	Common::Array<Common::String> _actionStrings;
	Common::Array<BackgroundSoundDefault> _backgroundSoundDefaults;
	void loadExecutableData();

	Common::HashMap<unsigned int, Common::HashMap<unsigned int, Conversation *>*> _conversations;
	Conversation *getConversation(unsigned int world, unsigned int id);
};

} // Unity

#endif

