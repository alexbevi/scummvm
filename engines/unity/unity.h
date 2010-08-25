#ifndef _UNITY_H
#define _UNITY_H

#include "engines/engine.h"
#include "common/stream.h"
#include "common/archive.h"
#include "common/rect.h"
#include "common/random.h"

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

class UnityEngine : public Engine {
public:
	UnityEngine(class OSystem *syst);
	~UnityEngine();

	Common::Error init();
	Common::Error run();

	Common::SeekableReadStream *openFile(Common::String filename);
	Common::String getSpriteFilename(unsigned int id);

	Object *objectAt(unsigned int x, unsigned int y);

	Common::RandomSource _rnd;

protected:
	Graphics *_gfx;
	Sound *_snd;
	Common::Archive *data;

	Screen current_screen;

	Common::Array<Trigger *> triggers;
	Common::Array<Object *> objects;
	Common::Array<Common::String> sprite_filenames;

	void loadTriggers();
	void processTriggers();

	void loadSpriteFilenames();
	void openLocation(unsigned int world, unsigned int screen);
	void loadScreenPolys(Common::String filename);

	void startupScreen();
	void startBridge();
	void startAwayTeam(unsigned int world, unsigned int screen);
};

} // Unity

#endif

