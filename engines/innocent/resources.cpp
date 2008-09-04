#include "resources.h"

#include "common/endian.h"
#include "common/file.h"
#include "common/util.h"

#include "innocent/main_dat.h"
#include "innocent/graph_dat.h"

using namespace Common;
using namespace std;

namespace Innocent {

Resources::Resources() : _main(new MainDat(this)), _graphicsMap(new GraphicsMap(this)) {
}

void Resources::load() {
	_main->load();
	_graphicsMap->load();

	loadGraphicFiles();
}

void Resources::loadGraphicFiles() {
	const list<MainDat::GraphicFile> files(_main->graphicFiles());

	for (list<MainDat::GraphicFile>::const_iterator it = files.begin(); it != files.end(); ++it)
		debug(1, "would open file %s of dataset %04x", it->filename, it->data_set);
}

} // End of namespace Innocent
