#include "innocent/logic.h"

#include "common/util.h"

#include "innocent/innocent.h"
#include "innocent/inter.h"
#include "innocent/resources.h"
#include "innocent/program.h"
#include "innocent/animation.h"

namespace Innocent {

Logic::Logic(Engine *e) :
		_engine(e),
		_resources(e->resources()),
		_currentRoom(0xffff),
		_currentBlock(0xffff)
	{}

Logic::~Logic() {
	for (Common::List<Animation *>::iterator it = _animations.begin(); it != _animations.end(); ++it)
		delete *it;
}

void Logic::init() {
	_toplevelInterpreter.reset(new Interpreter(this, _resources->mainBase(), "main code"));
	debugC(2, kDebugLevelScript, ">>>running initial code");
	_toplevelInterpreter->run(_resources->mainEntryPoint(), kCodeInitial);
	debugC(2, kDebugLevelScript, "<<<finished initial code");
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
		char buf[100];
		snprintf(buf, 100, "block %d code", newBlock);
		_blockInterpreter.reset(new Interpreter(this, _blockProgram->base(), buf));

		debugC(2, kDebugLevelScript, ">>>running block entry code for block %d", newBlock);
		_blockInterpreter->run(_blockProgram->begin(), kCodeNewBlock);
		debugC(2, kDebugLevelScript, "<<<finished block entry code for block %d", newBlock);
	}

	debugC(2, kDebugLevelScript, ">>>running room entry code for block %d", newRoom);
	_blockInterpreter->run(_blockProgram->roomHandler(newRoom), kCodeNewRoom);
	debugC(2, kDebugLevelScript, "<<<finished room entry code for block %d", newRoom);
}

void Logic::addAnimation(Animation *anim) {
	_animations.push_back(anim);
}

} // End of namespace Innocent
