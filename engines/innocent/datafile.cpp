#include "innocent/datafile.h"

#include "common/file.h"
#include "common/util.h"

using namespace Common;

namespace Innocent {

void Datafile::load() {
	File file;

	debug(kAck, "Resources::DataFile::load()ing %s", filename());
	if (!file.open(filename()))
		error("could not open %s", filename());

	readFile(file);
	file.close();
	debug(kAck, "Resources::DataFile::load()ed %s", filename());
}

} // End of namespace Innocent
