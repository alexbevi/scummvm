#include "innocent/room.h"

#include "innocent/logic.h"
#include "innocent/program.h"

namespace Innocent {
//

Room::Room(Logic *l) : _logic(l) {
	_exits = l->blockProgram()->exitsForRoom(l->roomNumber());
	snprintf(_debugInfo, 50, "room %d in %s", l->roomNumber(), +*l->blockProgram());
}

}
