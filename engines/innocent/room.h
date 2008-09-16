#ifndef INNOCENT_ROOM_H
#define INNOCENT_ROOM_H

#include <vector>

#include "common/list.h"
#include "common/rect.h"

#include "innocent/actor.h"
#include "innocent/debug.h"

namespace Innocent {
//

class Exit;
class Logic;

class Room : public StaticInspectable {
//
public:
	class Rect {
	public:
		Rect() : _zindex(999) {}
		Rect(int16 z, Common::Rect r) : _zindex(z), _rect(r) {}

	private:
		int16 _zindex;
		Common::Rect _rect;
	};

	const Common::List<Exit *> &exits() const { return _exits; }

	void addActorFrame(const Actor::Frame &f) { _actorFrames.push_back(f); }
	const Actor::Frame &getFrame(uint16 index) const { assert(index); return _actorFrames[index-1]; }

	void addRect(const Room::Rect &f) { _rects.push_back(f); }

	friend class Logic;

private:
	Room(Logic *l);

	// just in case, we'll explicitly add those if needed
	Room();
	Room(const Room &);
	Room &operator=(const Room &);

	Common::List<Exit *> _exits;
	Logic *_logic;

	std::vector<Actor::Frame> _actorFrames;
	Common::List<Rect> _rects;

	DEBUG_INFO
};

}

#endif
