#include "innocent/innocent.h"


#include "common/scummsys.h"

#include "common/events.h"

#include "innocent/debugger.h"
#include "innocent/graphics.h"
#include "innocent/logic.h"
#include "innocent/resources.h"

#define GFX_TRANSACTION for (int ___i = 1; ___i && (_system->beginGFXTransaction(), true); ___i--, _system->endGFXTransaction() )

namespace Innocent {

Engine::Engine(OSystem *syst) :
		::Engine(syst) {
	_resources.reset(new Resources);
	_graphics.reset(new Graphics(this));
	_logic.reset(new Logic(this));
	Common::addSpecialDebugLevel(kDebug, "example", "example special debug level");

	syst->getEventManager()->registerRandomSource(_rnd, "innocent");
	printf("Innocent::Engine::Engine\n");
}

Engine::~Engine() {
	printf("Innocent::Engine::Engine\n");
	Common::clearAllSpecialDebugLevels();
}

int Engine::init() {
	_resources->init();
	GFX_TRANSACTION {
		initCommonGFX(true);
		_system->initSize(320, 200);
	}
	_graphics->init();
	_logic->init();

	_debugger.reset(new Debugger(this));

	printf("Innocent::Engine::init\n");
	return 0;
}

int Engine::go() {
	printf("Innocent::Engine: Hello, world!\n");
	debugC(1, kDebug, "example debug call");

	while(!quit()) {
		_graphics->paintBackdrop();
		_logic->tick();
		_debugger->onFrame();
		_system->updateScreen();
		_system->delayMillis(1000/20);
	}

/*	while (1) {
		_logic->invokeNewRoomCode();
		while (1) {
			_graphics->clearScreen();
			_graphics->loadBackdrop();
			_graphics->paintBackdrop();
			_graphics->paintInterface();

			_system->updateScreen();
		}
	}*/

	return 0;
}

uint16 Engine::getRandom(uint16 max) const {
	return _rnd.getRandomNumber(max);
}

} // End of namespace Innocent
