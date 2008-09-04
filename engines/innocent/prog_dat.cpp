#include "innocent/prog_dat.h"

#include "common/util.h"

#include "innocent/main_dat.h"
#include "innocent/resources.h"

namespace Innocent {

ProgDat::ProgDat(Resources *res) : Datafile(res), _data(0) {}

ProgDat::~ProgDat() {
	if (_data)
		delete[] _data;
}

void ProgDat::readFile(Common::SeekableReadStream &stream) {
	uint16 total_entries = _resources->mainDat()->progEntriesCount0();
	total_entries += _resources->mainDat()->progEntriesCount1();

	debug(kDataRead, "will read %d entries from iuc_prog.dat", total_entries);

	_data = new byte[total_entries * 4];
	uint32 read_bytes = stream.read(_data, total_entries * 4);
	debug(kDataRead, "read %d entries from iuc_prog.dat", read_bytes / 4);
}

} // End of namespace Innocent
