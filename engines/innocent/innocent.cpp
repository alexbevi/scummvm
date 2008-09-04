#include "common/scummsys.h"

#include "common/events.h"

#include "innocent/innocent.h"

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
	_resources.load();
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
	byte interface[0x3c00];
	byte palette[0x400];
	_resources.loadInterfaceImage(interface, palette);

	_system->setPalette(palette, 0, 256);
	_system->copyRectToScreen(interface, 320, 0, 152, 320, 24);
	_system->updateScreen();
	_system->delayMillis(1000);

	return 0;
}

} // End of namespace Innocent
