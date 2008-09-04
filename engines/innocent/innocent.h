#ifndef INNOCENT_H
#define INNOCENT_H

#include "common/ptr.h"
#include "engines/engine.h"
#include "gui/debugger.h"

#include "innocent/util.h"

namespace Innocent {

class Console;
class Logic;
class Graphics;

enum {
	kDebug = 1 << 0
};

class Engine : public ::Engine {
public:
	Engine(OSystem *syst);
	~Engine();

	virtual int init();
	virtual int go();

private:
	Console *_console;

	Common::RandomSource _rnd;

	Pointer<Logic> _logic;
	Pointer<Graphics> _graphics;
};

class Console : public GUI::Debugger {
public:
	Console(Engine *vm) : GUI::Debugger() {}
	virtual ~Console() {}
};

} // End of namespace Innocent

#endif // INNOCENT_H
