#ifndef INNOCENT_GRAPHICS_H
#define INNOCENT_GRAPHICS_H

#include "config.h"

namespace Innocent {

class Engine;

class Graphics {
public:
	Graphics(Engine *engine) : _engine(engine) {}

	/**
	 * Load interface image and palette; sets the palette.
	 */
	void loadInterface();

	void init() { loadInterface(); }

	/**
	 * Paint the interface to proper portion of the screen.
	 */
	void paintInterface();

private:
	byte _interface[0x3c00];
	Engine *_engine;
};

} // End of namespace Innocent

#endif // INNOCENT_GRAPHICS_H
