#include "innocent/prog_dat.h"

#include "common/file.h"
#include "common/util.h"

#include "innocent/resources.h"
#include "innocent/main_dat.h"
#include "innocent/program.h"

using namespace Common;

namespace Innocent {

ProgDat::ProgDat(Resources *res) : Datafile(res), _data(0) {}

ProgDat::~ProgDat() {
	if (_data)
		delete[] _data;
}

void ProgDat::load() {
	File *file = new File;

	if (!file->open(filename()))
		error("could not open %s", filename());

	readFile(*file);
	_file.reset(file);
}

void ProgDat::readFile(Common::SeekableReadStream &stream) {
	// actually reads just header
	uint16 total_entries = _resources->mainDat()->progEntriesCount0();
	total_entries += _resources->mainDat()->progEntriesCount1();

	debug(kDataRead, "will read %d entries from iuc_prog.dat", total_entries);

	_data = new byte[total_entries * 4];
	uint32 read_bytes = stream.read(_data, total_entries * 4);
	debug(kDataRead, "read %d entries from iuc_prog.dat", read_bytes / 4);
}

Program *ProgDat::getScript(uint16 id) {
	if (!id) return 0;

	uint32 offset = READ_LE_UINT32(_data + (id - 1) * 4);
	debug(kDataRead, "loading script 0x%02x from 0x%08x", id, offset);
	_file->seek(offset);

	return new Program(*_file);
}

} // End of namespace Innocent
