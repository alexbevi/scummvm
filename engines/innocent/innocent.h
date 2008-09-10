#ifndef INNOCENT_H
#define INNOCENT_H

#include <memory>

#include "common/ptr.h"
#include "engines/engine.h"

namespace Common {
//
class EventManager;

}

namespace Innocent {

class Console;
class Interpreter;
class Resources;
class Graphics;
class Logic;
class Debugger;

class Engine : public ::Engine {
public:
	Engine(OSystem *syst);
	~Engine();

	virtual int init();
	virtual int go();

	Logic *logic() { return _logic.get(); }
	Resources *resources() { return _resources.get(); }
	Graphics *graphics() { return _graphics; }
	Debugger *debugger() { return _debugger; }
	Common::EventManager *eventMan() { return _eventMan; }

	uint16 getRandom(uint16 max) const;

	friend class Interpreter;
private:
	std::auto_ptr<Logic> _logic;
	std::auto_ptr<Resources> _resources;
	Graphics *_graphics;
	Debugger *_debugger;

	mutable Common::RandomSource _rnd;

	void handleEvents();
};

} // End of namespace Innocent

#endif // INNOCENT_H
