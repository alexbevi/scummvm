#ifndef INNOCENT_GRAPHICS_H
#define INNOCENT_GRAPHICS_H

#include "innocent/util.h"

namespace Innocent {

class Logic;

class Graphics {
public:
	Graphics(Pointer<Logic> logic);
	void load();

private:
	Pointer<Logic> _logic;
	byte _map[1200];
};

} // End of namespace Innocent

#endif // INNOCENT_GRAPHICS_H
