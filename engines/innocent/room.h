#ifndef INNOCENT_ROOM_H
#define INNOCENT_ROOM_H

#include "common/list.h"

#include "innocent/debug.h"

namespace Innocent {
//

class Exit;
class Logic;

class Room : public StaticInspectable {
//
public:
	const Common::List<Exit *> &exits() const { return _exits; }

	friend class Logic;

private:
	Room(Logic *l);

	// just in case, we'll explicitly add those if needed
	Room();
	Room(const Room &);
	Room &operator=(const Room &);

	Common::List<Exit *> _exits;
	Logic *_logic;

	DEBUG_INFO
};

}

#endif
