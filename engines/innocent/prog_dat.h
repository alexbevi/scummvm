#ifndef INNOCENT_PROG_DAT_H
#define INNOCENT_PROG_DAT_H

#include <memory>

#include "common/stream.h"

#include "innocent/datafile.h"

namespace Innocent {

class Resources;
class Program;

class ProgDat : public Datafile {
public:
	ProgDat(Resources *resources);
	~ProgDat();

	void load();

	const char *filename() const { return "iuc_prog.dat"; }
	void readFile(Common::SeekableReadStream &stream);

	Program *getScript(uint16 id);

private:
	byte *_data;
	std::auto_ptr<Common::SeekableReadStream> _file;
};

} // End of namespace Innocent

#endif // INNOCENT_PROG_DAT
