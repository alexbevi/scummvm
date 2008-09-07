#ifndef INNOCENT_GRAPHICS_H
#define INNOCENT_GRAPHICS_H

#include <memory>

#include "config.h"

class OSystem;

namespace Graphics {
class Surface;
}

namespace Innocent {

class Engine;
class Resources;

class Graphics {
public:
	Graphics(Engine *engine);

	/**
	 * Load interface image and palette; sets the palette.
	 */
	void loadInterface();

	void init();

	/**
	 * Paint the interface to proper portion of the screen.
	 */
	void paintInterface();

	void setBackdrop(uint16 id);
	void paintBackdrop();

private:
	byte _interface[0x3c00];
	Engine *_engine;
	Resources *_resources;
	OSystem *_system;
	std::auto_ptr< ::Graphics::Surface> _backdrop;
};

} // End of namespace Innocent

#endif // INNOCENT_GRAPHICS_H
