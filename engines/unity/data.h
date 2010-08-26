#ifndef _UNITY_DATA_H
#define _UNITY_DATA_H

#include "common/stream.h"
#include "common/archive.h"
#include "common/rect.h"

#include "object.h"

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
	Common::Array<Common::Array<Common::Point> > entrypoints;

	Common::Array<ScreenPolygon> polygons;
};

struct UnityData {
public:
	Common::SeekableReadStream *openFile(Common::String filename);
	Common::String getSpriteFilename(unsigned int id);

	Common::Archive *data;

	Screen current_screen;

	Common::Array<Trigger *> triggers;
	Common::Array<Object *> objects;
	Common::Array<Common::String> sprite_filenames;
	Common::Array<Common::String> sector_names;
	Common::HashMap<uint32, Common::String> icon_sprites;

	void loadTriggers();

	void loadSpriteFilenames();
	void loadScreenPolys(Common::String filename);

	void loadSectorNames();
	Common::String getSectorName(unsigned int x, unsigned int y, unsigned int z);
	void loadIconSprites();
	Common::String getIconSprite(objectID id);
};

} // Unity

#endif

