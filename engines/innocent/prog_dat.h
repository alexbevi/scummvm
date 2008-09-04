#ifndef INNOCENT_PROG_DAT_H
#define INNOCENT_PROG_DAT_H

#include "common/stream.h"

#include "innocent/datafile.h"

namespace Innocent {

class Resources;

class ProgDat : public Datafile {
public:
	ProgDat(Resources *resources);
	~ProgDat();

	const char *filename() const { return "iuc_prog.dat"; }
	void readFile(Common::SeekableReadStream &stream);

private:
	byte *_data;
};

} // End of namespace Innocent

#endif // INNOCENT_PROG_DAT
