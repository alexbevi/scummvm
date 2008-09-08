#ifndef INNOCENT_H
#define INNOCENT_H

#include <memory>

#include "common/ptr.h"
#include "engines/engine.h"

namespace Innocent {

class Console;
class Resources;
class Graphics;
class Logic;
class Debugger;

enum {
	kDebugLevelScript = 1 << 0
};

class Engine : public ::Engine {
public:
	Engine(OSystem *syst);
	~Engine();

	virtual int init();
	virtual int go();

	Logic *logic() { return _logic.get(); }
	Resources *resources() { return _resources.get(); }
	Graphics *graphics() { return _graphics.get(); }

	uint16 getRandom(uint16 max) const;

private:
	std::auto_ptr<Logic> _logic;
	std::auto_ptr<Resources> _resources;
	std::auto_ptr<Graphics> _graphics;
	std::auto_ptr<Debugger> _debugger;

	mutable Common::RandomSource _rnd;

	void handleEvents();
};

} // End of namespace Innocent

#endif // INNOCENT_H
