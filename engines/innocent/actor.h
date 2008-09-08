#ifndef INNOCENT_ACTOR_H
#define INNOCENT_ACTOR_H

#include "innocent/animation.h"

namespace Innocent {
//

class MainDat;
class Program;

class Actor : public Animation {
//
public:
	friend class MainDat;
	friend class Program;
	enum {
		Size = 0x71
	};

	uint16 room() const { return _room; }
private:
	Actor(const CodePointer &code);

	// just in case, we'll explicitly add those if needed
	Actor();
	Actor(const Actor &);
	Actor &operator=(const Actor &);

	void readHeader(const byte *code);

	uint16 _room;
};

}

#endif
