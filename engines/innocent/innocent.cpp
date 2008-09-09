#include "innocent/innocent.h"


#include "common/scummsys.h"

#include "common/events.h"

#include "innocent/debug.h"
#include "innocent/debugger.h"
#include "innocent/eventmanager.h"
#include "innocent/graphics.h"
#include "innocent/logic.h"
#include "innocent/resources.h"

#define GFX_TRANSACTION for (int ___i = 1; ___i && (_system->beginGFXTransaction(), true); ___i--, _system->endGFXTransaction() )

namespace Innocent {

Engine::Engine(OSystem *syst) :
		::Engine(syst) {
	_resources.reset(new Resources(this));
	_graphics = &Graphics::instance();
	_graphics->setEngine(this);
	_logic.reset(new Logic(this));

	Common::addSpecialDebugLevel(kDebugLevelScript, "script", "bytecode scripts");
	Common::addSpecialDebugLevel(kDebugLevelGraphics, "graphics", "graphics handling");
	Common::addSpecialDebugLevel(kDebugLevelFlow, "flow", "game code flow status");
	Common::addSpecialDebugLevel(kDebugLevelAnimation, "animation", "animations");
	Common::addSpecialDebugLevel(kDebugLevelValues, "values", "really low-level debugging of value manipulation");
	Common::addSpecialDebugLevel(kDebugLevelFiles, "files", "file input and output");
	Common::addSpecialDebugLevel(kDebugLevelEvents, "events", "event handling");

	syst->getEventManager()->registerRandomSource(_rnd, "innocent");
}

Engine::~Engine() {
	Common::clearAllSpecialDebugLevels();
}

int Engine::init() {
	GFX_TRANSACTION {
		initCommonGFX(false);
		_system->initSize(320, 200);
	}
	_resources->init();
	_graphics->init();
	_logic->init();
	_debugger.reset(new Debugger(this));

	return 0;
}

int Engine::go() {
	_resources->loadActors();
	_graphics->showCursor();
	while(!quit()) {
		_graphics->paintBackdrop();
		_logic->tick();
		_graphics->paint();
		_graphics->updateScreen();
		_debugger->onFrame();
		_system->delayMillis(1000/40);
		handleEvents();
	}

	return 0;
}

void Engine::handleEvents() {
	Common::Event event;
	while (_eventMan->pollEvent(event)) {
		switch(event.type) {

		case Common::EVENT_KEYUP:
			if (event.kbd.keycode == Common::KEYCODE_BACKQUOTE)
				_debugger->attach();
			break;

		case Common::EVENT_LBUTTONUP:
			EventManager::instance().clicked(event.mouse);

		default:
			break;
		}
	}
}

uint16 Engine::getRandom(uint16 max) const {
	return _rnd.getRandomNumber(max);
}

} // End of namespace Innocent
