#ifndef _UNITY_H
#define _UNITY_H

#include "engines/engine.h"
#include "common/random.h"

#include "data.h"

namespace Unity {

class Graphics;
class Sound;
class SpritePlayer;
class Object;
class Trigger;

class UnityEngine : public Engine {
public:
	UnityEngine(class OSystem *syst);
	~UnityEngine();

	Common::Error init();
	Common::Error run();

	Object *objectAt(unsigned int x, unsigned int y);

	Common::RandomSource _rnd;

	UnityData data;

	Sound *_snd;

protected:
	Graphics *_gfx;

	void openLocation(unsigned int world, unsigned int screen);

	void checkEvents();
	void drawObjects();
	void processTriggers();

	void startupScreen();
	void startBridge();
	void startAwayTeam(unsigned int world, unsigned int screen);

	void drawDialogWindow();
	void drawBridgeUI();
};

} // Unity

#endif

