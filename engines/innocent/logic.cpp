#include "innocent/logic.h"

#include "common/util.h"

#include "innocent/innocent.h"
#include "innocent/inter.h"
#include "innocent/resources.h"

namespace Innocent {

Logic::Logic(Engine *e) :
		_engine(e),
		_resources(e->resources()),
		_interpreter(new Interpreter(this))
	{}

void Logic::init() {
	_interpreter->init();
	byte *initial_code = _resources->initialCode();
	_interpreter->run(initial_code, kCodeInitial);
}

void Logic::setProtagonist(uint16 actor) {
	debug(2, "setting protagonist to %d", actor);
	_protagonist = actor;
}

} // End of namespace Innocent
