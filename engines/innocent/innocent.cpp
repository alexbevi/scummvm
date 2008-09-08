#include "innocent/innocent.h"


#include "common/scummsys.h"

#include "common/events.h"

#include "innocent/debug.h"
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

	Common::addSpecialDebugLevel(kDebugLevelScript, "script", "bytecode scripts");
	Common::addSpecialDebugLevel(kDebugLevelScript, "graphics", "graphics handling");

	syst->getEventManager()->registerRandomSource(_rnd, "innocent");
}

Engine::~Engine() {
	Common::clearAllSpecialDebugLevels();
}

int Engine::init() {
	_resources->init();
	GFX_TRANSACTION {
		initCommonGFX(false);
		_system->initSize(320, 200);
	}
	_debugger.reset(new Debugger(this));
	_graphics->init();
	_logic->init();

	return 0;
}

int Engine::go() {
	while(!quit()) {
//		_graphics->paintBackdrop();
		_logic->tick();
		_debugger->onFrame();
		_system->updateScreen();
		_system->delayMillis(1000/20);
		handleEvents();
	}

	return 0;
}

void Engine::handleEvents() {
	Common::Event event;
	while (_eventMan->pollEvent(event)) {
		if (event.type == Common::EVENT_KEYUP)
			if (event.kbd.keycode == Common::KEYCODE_BACKQUOTE)
				_debugger->attach();
	}
}

uint16 Engine::getRandom(uint16 max) const {
	return _rnd.getRandomNumber(max);
}

} // End of namespace Innocent
