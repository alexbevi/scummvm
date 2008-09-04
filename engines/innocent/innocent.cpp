#include "innocent/innocent.h"


#include "common/scummsys.h"

#include "common/events.h"

#include "innocent/graphics.h"
#include "innocent/resources.h"

#define GFX_TRANSACTION for (int ___i = 1; ___i && (_system->beginGFXTransaction(), true); ___i--, _system->endGFXTransaction() )

namespace Innocent {

Engine::Engine(OSystem *syst) :
		::Engine(syst),
		_resources(new Resources),
		_graphics(new Graphics(this)) {
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
	_graphics->init();
	GFX_TRANSACTION {
		initCommonGFX(true);
		_system->initSize(320, 200);
	}

	_console = new Console(this);

	printf("Innocent::Engine::init\n");
	return 0;
}

int Engine::go() {
	printf("Innocent::Engine: Hello, world!\n");
	debugC(1, kDebug, "example debug call");
	
	_graphics->paintInterface();
	
	_system->updateScreen();
	_system->delayMillis(1000);

	return 0;
}

} // End of namespace Innocent
