#include "innocent/logic.h"

#include "innocent/innocent.h"
#include "innocent/inter.h"
#include "innocent/resources.h"
#include "innocent/main_dat.h"

namespace Innocent {

Logic::Logic(Engine *e) :
		_engine(e),
		_interpreter(new Interpreter(this)),
		_status(kStatusOk)
	{}

void Logic::start() {
	const byte *entry_point = _engine->_resources->_main->getEntryPoint();

	_interpreter->run(entry_point, 0);
}

} // End of namespace Innocent
