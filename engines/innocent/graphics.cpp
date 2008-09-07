#include "innocent/graphics.h"

#include "common/system.h"

#include "innocent/innocent.h"
#include "innocent/resources.h"

namespace Innocent {

Graphics::Graphics(Engine *engine)
	 : _engine(engine) {
}

void Graphics::init() {
	 _resources = _engine->resources();
	 loadInterface();
}

void Graphics::loadInterface() {
	byte palette[0x400];

	_resources->loadInterfaceImage(_interface, palette);
	debug(kAck, "loaded interface image");

	_engine->_system->setPalette(palette + 160 * 4, 160, 96);
	debug(kAck, "set interface palette");
}

void Graphics::paintInterface() {
	debug(kAck, "painting interface");
	_engine->_system->copyRectToScreen(_interface, 320, 0, 152, 320, 48);
	debug(kAck, "painted interface");
}

void Graphics::setBackdrop(uint16 id) {
	debug(2, "setting backdrop image id to 0x%04x", id);

	byte palette[0x400];
	_backdrop.reset(_resources->loadBackdrop(id, palette));
	debug(3, "backdrop loaded");
}

} // End of namespace Innocent
