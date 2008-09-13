#include "innocent/mapfile.h"

#include "common/endian.h"
#include "common/util.h"

using namespace Common;

namespace Innocent {

void MapFile::readFile(SeekableReadStream &stream) {
	/*uint32 actually_read = */stream.read(_data, 1200);

//	_entryCount = actually_read / 4;
}

uint32 MapFile::offsetOfEntry(uint16 index) {
	return READ_LE_UINT32(_data + (index-1)*4);
}

} // End of namespace Innocent
