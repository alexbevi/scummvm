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

struct ScreenPolygon {
	byte type;

	Common::Array<Common::Point> points;
	Common::Array<uint16> distances;
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

	Common::Array<Object *> objects;
	Common::Array<Common::String> sprite_filenames;

	void loadSpriteFilenames();
	void openLocation(unsigned int world, unsigned int screen);
	void loadScreenPolys(Common::String filename);
};

} // Unity

#endif

