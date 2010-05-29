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

struct Object {
	unsigned int x;
	unsigned int y;
	SpritePlayer *sprite;
};

struct Screen {
	Common::Array<Common::Array<Common::Point> > entrypoints;
	Common::String polygonsFilename;
};

class UnityEngine : public Engine {
public:
	UnityEngine(class OSystem *syst);
	~UnityEngine();

	Common::Error init();
	Common::Error run();

	Common::SeekableReadStream *openFile(Common::String filename);
	Common::String getSpriteFilename(unsigned int id);

	Common::RandomSource _rnd;

protected:
	Graphics *_gfx;
	Sound *_snd;
	Common::Archive *data;

	Screen current_screen;

	Common::Array<Object *> objects;
	Common::Array<Common::String> sprite_filenames;

	void loadSpriteFilenames();
	void openLocation(unsigned int location, unsigned int screen);
	void loadObject(unsigned int location, unsigned int screen, unsigned int id);
};

} // Unity

#endif

