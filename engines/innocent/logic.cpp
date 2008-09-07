#include "innocent/logic.h"

#include "innocent/innocent.h"
#include "innocent/inter.h"
#include "innocent/resources.h"

namespace Innocent {

Logic::Logic(Engine *e) :
		_engine(e),
		_resources(e->resources()),
		_interpreter(new Interpreter(_engine))
	{}

void Logic::init() {
	byte *initial_code = _resources->initialCode();
	_interpreter->run(initial_code, kCodeInitial);
}

} // End of namespace Innocent
