#include "common/file.h"
#include "common/util.h"

#include "innocent/logic.h"

#include "innocent/graphics.h"

namespace Innocent {

Graphics::Graphics(Pointer<Logic>logic) : _logic(logic) {}

void Graphics::load() {
	Common::File fd;
	if (!fd.open("iuc_graf.dat"))
		error("Graphics::load() Could not open iuc_graf.dat");

	uint32 actually_read = fd.read(&_map, 1200);
	debug(1, "Read %d bytes from iuc_graf.", actually_read);
	
	if (actually_read >> 2 != _logic->graphicsCount())
		error("ILL Error: Incorrect graphics for current logic.");
}

} // End of namespace Innocent
