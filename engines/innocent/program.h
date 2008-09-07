#ifndef INNOCENT_PROGRAM_H
#define INNOCENT_PROGRAM_H

#include "common/stream.h"

namespace Innocent {

class Program {
public:
	Program(Common::ReadStream &file);
	~Program();

	uint16 begin();
	byte *localVariable(uint16 offset);
	uint16 roomHandler(uint16 room);
	byte *base() const { return _code; }

private:
	Program() { /* can only be created from a file */ }
	byte *_code;
	byte _footer[0x10];

	uint16 entryPointOffset();
};

} // End of namespace Innocent

#endif // INNOCENT_PROGRAM_H
