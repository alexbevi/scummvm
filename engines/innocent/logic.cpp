#include "common/scummsys.h"
#include "common/system.h"
#include "common/endian.h"
#include "common/util.h"

#include "innocent/logic.h"

#include "common/file.h"

using namespace Common;

namespace Innocent {

MainDat::MainDat() : _data(0) {}

MainDat::~MainDat() {
	if (_data)
		delete[] _data;
}

void MainDat::open(const char *filename) {
	debug(1, "MainDat::open() Loading from %s", filename);
	Common::File fd;
	if (!fd.open(filename))
		error("MainDat::open() Could not open %s", filename);
	load(fd);
	fd.close();
}

void MainDat::load(Common::SeekableReadStream &s) {
	uint16 length = s.readUint16LE();
	_data = new byte[length];

	s.seek(0);
	s.read(_data, length);

	descramble(_data + 2, length - 2);

	if (gDebugLevel >= 5) {
		Common::DumpFile fd;
		fd.open("maindat.dump");
		fd.write(_data, length);
		fd.close();
		debug(5, "MainDat::load() dumped descrambled file to  maindat.dump");
	}
}

void MainDat::descramble(byte *data, uint16 length) {
	for (; length; length--)
		(*(data++)) ^= 0x6f;
}

} // End of namespace Innocent
