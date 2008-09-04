#ifndef INNOCENT_GRAPHICS_H
#define INNOCENT_GRAPHICS_H

#include <vector>

#include "common/file.h"

namespace Innocent {

class Logic;

class Graphics {
public:
	Graphics(Logic *logic);
	void load();

private:
	Logic *_logic;
	byte _map[1200];

	void loadDataFile(string name);
	std::vector<Common::File> _dataFiles;
};

} // End of namespace Innocent

#endif // INNOCENT_GRAPHICS_H
