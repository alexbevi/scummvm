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
	kDebug = 1 << 0
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

	mutable Common::RandomSource _rnd;
};

} // End of namespace Innocent

#endif // INNOCENT_H
