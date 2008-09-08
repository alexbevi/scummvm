#ifndef INNOCENT_PROGRAM_H
#define INNOCENT_PROGRAM_H

#include "common/list.h"
#include "common/stream.h"

#include "innocent/resources.h"

namespace Innocent {
//

class Actor;

class Program {
public:
	Program(Common::ReadStream &file);
	~Program();

	uint16 begin();
	byte *localVariable(uint16 offset);
	uint16 roomHandler(uint16 room);
	byte *base() const { return _code; }

	SpriteInfo getSpriteInfo(uint16 index) const;

	void loadActors(Interpreter *i);

private:
	Program() { /* can only be created from a file */ }

	uint16 entryPointOffset();

	byte *_code;
	byte _footer[0x10];
	Common::List<Actor *> _actors;
};

} // End of namespace Innocent

#endif // INNOCENT_PROGRAM_H
