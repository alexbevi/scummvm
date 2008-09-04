#include "innocent/graph_dat.h"

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

} // End of namespace Innocent
