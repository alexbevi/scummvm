#include "innocent/room.h"

#include "innocent/logic.h"
#include "innocent/program.h"

namespace Innocent {
//

Room::Room(Logic *l) : _logic(l) {
	_exits = l->blockProgram()->exitsForRoom(l->roomNumber());
	snprintf(_debugInfo, 50, "room %d in %s", l->roomNumber(), +*l->blockProgram());
}

void Room::addActorFrame(Common::Point pos, std::vector<byte> nexts) {
	Actor::Frame f(pos, nexts, _actorFrames.size() + 1);
	_actorFrames.push_back(f);
}

}
