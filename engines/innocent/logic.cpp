#include "innocent/logic.h"

#include "common/util.h"

#include "innocent/innocent.h"
#include "innocent/inter.h"
#include "innocent/resources.h"
#include "innocent/program.h"

namespace Innocent {

Logic::Logic(Engine *e) :
		_engine(e),
		_resources(e->resources()),
		_currentRoom(0xffff),
		_currentBlock(0xffff)
	{}

void Logic::init() {
	_toplevelInterpreter.reset(new Interpreter(this, _resources->mainBase()));
	_toplevelInterpreter->run(_resources->mainEntryPoint(), kCodeInitial);
}

void Logic::tick() {
	_blockInterpreter->tick();
}

void Logic::setProtagonist(uint16 actor) {
	_protagonist = actor;
}

void Logic::changeRoom(uint16 newRoom) {
	if (newRoom == _currentRoom)
		return;
	_currentRoom = newRoom;
	uint16 newBlock = _resources->blockOfRoom(_currentRoom);
	if (newBlock != _currentBlock) {
		_currentBlock = newBlock;
		_blockProgram.reset(_resources->loadCodeBlock(newBlock));
		_blockInterpreter.reset(new Interpreter(this, _blockProgram->base()));

		_blockInterpreter->run(_blockProgram->begin(), kCodeNewBlock);
	}

	_blockInterpreter->run(_blockProgram->roomHandler(newRoom), kCodeNewRoom);
}

} // End of namespace Innocent
