#include "common/scummsys.h"

#include "common/events.h"

#include "innocent/innocent.h"
#include "innocent/logic.h"
#include "innocent/graphics.h"

#define GFX_TRANSACTION for (int ___i = 1; ___i && (_system->beginGFXTransaction(), true); ___i--, _system->endGFXTransaction() )

namespace Innocent {

Engine::Engine(OSystem *syst) : ::Engine(syst) {
	Common::addSpecialDebugLevel(kDebug, "example", "example special debug level");

	syst->getEventManager()->registerRandomSource(_rnd, "innocent");
	printf("Innocent::Engine::Engine\n");
}

Engine::~Engine() {
	printf("Innocent::Engine::Engine\n");
	Common::clearAllSpecialDebugLevels();
}

int Engine::init() {
	GFX_TRANSACTION {
		initCommonGFX(true);
		_system->initSize(320, 200);
	}

	_console = new Console(this);

	_logic = new Logic();
	_graphics = new Graphics(_logic);

	printf("Innocent::Engine::init\n");
	return 0;
}

int Engine::go() {
	printf("Innocent::Engine: Hello, world!\n");
	debugC(1, kDebug, "example debug call");

	_logic->load();
	_graphics->load();
	
	return 0;
}

} // End of namespace Innocent
