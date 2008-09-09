#ifndef INNOCENT_EXIT_H
#define INNOCENT_EXIT_H

#include <memory>

#include "common/rect.h"

#include "innocent/debug.h"
#include "innocent/value.h"

namespace Innocent {
//
class Graphics;
class Sprite;

class Exit : public StaticInspectable {
	DEBUG_INFO
public:
	enum { Size = 0xe };

	uint16 room() const { return _room; }

	void paint(Graphics *g);

	friend class Program;
private:
	Exit(const CodePointer &code);

	Sprite *sprite() const { return _sprite.get(); }
	byte zIndex() const { return _zIndex; }

	byte _zIndex;
	std::auto_ptr<Sprite> _sprite;
	Common::Point _position;
	uint16 _room;
};

}

#endif
