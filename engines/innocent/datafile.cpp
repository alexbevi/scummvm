#include "innocent/datafile.h"

#include "common/file.h"
#include "common/util.h"

using namespace Common;

namespace Innocent {

void Datafile::load() {
	File file;

	if (!file.open(filename()))
		error("could not open %s", filename());

	readFile(file);
	file.close();
}

} // End of namespace Innocent
