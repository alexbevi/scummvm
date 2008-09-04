#include "resources.h"

#include "common/endian.h"
#include "common/file.h"
#include "common/util.h"

#include "innocent/main_dat.h"
#include "innocent/graph_dat.h"

using namespace Common;

namespace Innocent {

Resources::Resources() : _main(new MainDat(this)), _graphicsMap(new GraphicsMap(this)) {
}

void Resources::load() {
	_main->load();
	_graphicsMap->load();
}

} // End of namespace Innocent
