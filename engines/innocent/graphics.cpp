#include "innocent/graphics.h"

#include "common/system.h"

#include "innocent/innocent.h"
#include "innocent/resources.h"

namespace Innocent {

void Graphics::loadInterface() {
	byte palette[0x400];

	_engine->_resources->loadInterfaceImage(_interface, palette);
	debug(kAck, "loaded interface image");

	_engine->_system->setPalette(palette + 160 * 4, 160, 96);
	debug(kAck, "set interface palette");
}

void Graphics::paintInterface() {
	debug(kAck, "painting interface");
	_engine->_system->copyRectToScreen(_interface, 320, 0, 152, 320, 48);
	debug(kAck, "painted interface");
}

} // End of namespace Innocent
