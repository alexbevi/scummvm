#include "common/file.h"
#include "common/func.h"
#include "common/util.h"

#include "innocent/logic.h"

#include "innocent/graphics.h"

using namespace std;

namespace Innocent {

Graphics::Graphics(Logic *logic) : _logic(logic) {}

void Graphics::load() {
	Common::File fd;
	if (!fd.open("iuc_graf.dat"))
		error("Graphics::load() Could not open iuc_graf.dat");

	uint32 actually_read = fd.read(&_map, 1200);
	debug(1, "Read %d bytes from iuc_graf.", actually_read);
	
	if (actually_read >> 2 != _logic->graphicsCount())
		error("ILL Error: Incorrect graphics for current logic.");

	const vector<string> graphic_files = _logic->graphicFileNames();
	for_each(graphic_files.begin(), graphic_files.end(), bind1st(mem_fun(loadDataFile), this));
}

void Graphics::loadDataFile(string name) {
	Common::File file;
	if (!file.open(name))
		error("Graphics::loadDataFile() couldn't load %s", name);
	_dataFiles.push_back(file);
}

} // End of namespace Innocent
