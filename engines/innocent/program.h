#ifndef INNOCENT_PROGRAM_H
#define INNOCENT_PROGRAM_H

#include "common/stream.h"

namespace Innocent {

class Program {
public:
	Program(Common::ReadStream &file);
	~Program();

	byte *begin();

private:
	Program() { /* can only be created from a file */ }
	byte *_code;
	byte _footer[10];

	uint16 entryPointOffset();
};

} // End of namespace Innocent

#endif // INNOCENT_PROGRAM_H
