#include "innocent/logic.h"

#include "common/util.h"

#include "innocent/innocent.h"
#include "innocent/inter.h"
#include "innocent/resources.h"

namespace Innocent {

Logic::Logic(Engine *e) :
		_engine(e),
		_resources(e->resources())
	{}

void Logic::init() {
	_toplevelInterpreter.reset(new Interpreter(this, _resources->mainBase()));
	_toplevelInterpreter->run(_resources->mainEntryPoint(), kCodeInitial);
}

void Logic::setProtagonist(uint16 actor) {
	debug(2, "setting protagonist to %d", actor);
	_protagonist = actor;
}

void Logic::changeRoom(uint16 newRoom) {
	debug(2, "changing room to %d", newRoom);
	_currentRoom = newRoom;
	// TODO transitions, triggers etc.
}

} // End of namespace Innocent
