#include "innocent/logic.h"

#include "common/util.h"

#include "innocent/innocent.h"
#include "innocent/inter.h"
#include "innocent/resources.h"
#include "innocent/program.h"

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
	if (newRoom == _currentRoom)
		return;
	_currentRoom = newRoom;
	debug(2, "changing room to %d", newRoom);
	uint16 newBlock = _resources->blockOfRoom(_currentRoom);
	if (newBlock != _currentBlock) {
		debug(2, "new block %d", newBlock);
		_currentBlock = newBlock;
		_blockProgram.reset(_resources->loadCodeBlock(newBlock));
		_blockInterpreter.reset(new Interpreter(this, _blockProgram->base()));

		debug(2, "running block init code");
		_blockInterpreter->run(_blockProgram->begin(), kCodeNewBlock);
	}

	debug(2, "running room entry code");
	_blockInterpreter->run(_blockProgram->roomHandler(newRoom), kCodeNewRoom);
}

} // End of namespace Innocent
