#include "innocent/graph_dat.h"

#include "common/endian.h"
#include "common/util.h"

#include "innocent/main_dat.h"
#include "innocent/resources.h"

using namespace Common;

namespace Innocent {

void GraphicsMap::readFile(SeekableReadStream &stream) {
	uint32 actually_read = stream.read(_data, 1200);

	_imgCount = actually_read / 4;
	debug(kDataRead, "loaded %d image entries from iuc_graf.dat", _imgCount);

	if (_imgCount != _resources->mainDat()->imagesCount())
		error("ILL Error: Incorrect graphics for current logic.");
}

uint32 GraphicsMap::offsetOfImage(uint16 index) {
	debug(kDataRead, "moved to offset %04x in iuc_graf, read %08x", (index-1)*4, READ_LE_UINT32(_data + (index-1)*4));
	return READ_LE_UINT32(_data + (index-1)*4);
}

} // End of namespace Innocent
