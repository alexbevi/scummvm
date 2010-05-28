#ifndef _UNITY_H
#define _UNITY_H

#include "engines/engine.h"
#include "common/stream.h"
#include "common/archive.h"
#include "common/rect.h"

namespace Unity {

class Graphics;
class Sound;

class UnityEngine : public Engine {
public:
	UnityEngine(class OSystem *syst);
	~UnityEngine();

	Common::Error init();
	Common::Error run();

	Common::SeekableReadStream *openFile(Common::String filename);

protected:
	Graphics *_gfx;
	Sound *_snd;
	Common::Archive *data;

	Common::Point entrypoints[4];
	Common::String polygonsFilename;

	void openLocation(unsigned int location, unsigned int screen);
};

} // Unity

#endif

