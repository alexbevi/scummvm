#include "innocent/inter.h"

#include "common/util.h"

namespace Innocent {
#define OPCODE(num) template<> void Interpreter::opcodeHandler<num>(Argument *args[])

OPCODE(0x12) {
	// if sound is on then
	debug(1, "sound test opcode 0x12 stub");
	failedCondition();
}

OPCODE(0x9d) {
	// set protagonist
	_logic->setProtagonist(*args[0]);
}


} // End of namespace Innocent
