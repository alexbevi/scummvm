#include "innocent/innocent.h"


#include "common/config-manager.h"
#include "common/scummsys.h"
#include "common/events.h"
#include "sound/mididrv.h"

#include "innocent/debug.h"
#include "innocent/debugger.h"
#include "innocent/eventmanager.h"
#include "innocent/graphics.h"
#include "innocent/logic.h"
#include "innocent/musicparser.h"
#include "innocent/resources.h"

#define GFX_TRANSACTION for (int ___i = 1; ___i && (_system->beginGFXTransaction(), true); ___i--, _system->endGFXTransaction() )

namespace Innocent {

Engine *Engine::me;

Engine::Engine(OSystem *syst) :
		::Engine(syst) {
	_resources = &Res;
	_resources->setEngine(this);
	_graphics = &Graphics::instance();
	_graphics->setEngine(this);
	_logic = &Logic::instance();
	_logic->setEngine(this);
	_copyProtection = false;
	me = this;
	_lastTicks = 0;

	Common::addSpecialDebugLevel(kDebugLevelScript, "script", "bytecode scripts");
	Common::addSpecialDebugLevel(kDebugLevelGraphics, "graphics", "graphics handling");
	Common::addSpecialDebugLevel(kDebugLevelFlow, "flow", "game code flow status");
	Common::addSpecialDebugLevel(kDebugLevelAnimation, "animation", "animations");
	Common::addSpecialDebugLevel(kDebugLevelValues, "values", "really low-level debugging of value manipulation");
	Common::addSpecialDebugLevel(kDebugLevelFiles, "files", "file input and output");
	Common::addSpecialDebugLevel(kDebugLevelEvents, "events", "event handling");
	Common::addSpecialDebugLevel(kDebugLevelMusic, "music", "music loading and playing");

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
	_copyProtection = ConfMan.getBool("copy_protection");
	_startRoom = ConfMan.getInt("boot_param");
	_debugger = &Debug;
	Debug.setEngine(this);
	_resources->init();
	_graphics->init();

/*	int midiDriver = MidiDriver::detectMusicDriver(MDT_MIDI);
*/
	MidiDriver *driver = MidiDriver::createMidi(MD_MT32);

	_musicDriver.reset(driver);
	Music.setMidiDriver(driver);


	_logic->init();

	return 0;
}

int Engine::go() {
	_resources->loadActors();
	_logic->initCode();
	_graphics->showCursor();
	while(!quit()) {
		_logic->callAnimations();
		_graphics->paint();
		_logic->tick();
//		_graphics->paintAnimations();
		_graphics->updateScreen();
		_debugger->onFrame();
		delay(20);
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

bool Engine::escapePressed() const {
	Common::Event event;
	while (_eventMan->pollEvent(event)) {
		switch(event.type) {

		case Common::EVENT_KEYUP:
			if (event.kbd.keycode == Common::KEYCODE_BACKQUOTE)
				_debugger->attach();
			if (event.kbd.keycode == Common::KEYCODE_ESCAPE)
				return true;
			break;
		default:
			break;
		}
	}
	return false;
}

uint16 Engine::getRandom(uint16 max) const {
	return _rnd.getRandomNumber(max);
}

void Engine::delay(int millis) const {
	int target = _lastTicks + millis;
	while ((_lastTicks = _system->getMillis()) < target) {
		_system->delayMillis(target - _lastTicks);
	}
}

} // End of namespace Innocent
