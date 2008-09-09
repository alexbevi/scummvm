#include "innocent/eventmanager.h"

#include "innocent/debug.h"
#include "innocent/graphics.h"
#include "innocent/util.h"

using namespace std;

namespace Innocent {
//

DECLARE_SINGLETON(EventManager);

Clickable::Clickable() {
	EventManager::instance().push(this);
}

Clickable::~Clickable() {
	EventManager::instance().pop(this);
}

void EventManager::clicked(Common::Point pos) {
	Clickable *handler = 0;

	foreach(Clickable *, _handlers)
		if ((*it)->area().contains(pos))
			if (!handler || handler->zIndex() > (*it)->zIndex())
				handler = *it;

	if (handler)
		handler->clicked();
}

void EventManager::push(Clickable *c) {
	_handlers.push_back(c);
}

void EventManager::pop(Clickable *c) {
	_handlers.remove(c);
}

void EventManager::paint(Graphics *g) const {
	debugC(3, kDebugLevelEvents | kDebugLevelGraphics, "EventManager got paint event");
	if (!_debug)
		return;
	debugC(3, kDebugLevelEvents | kDebugLevelGraphics, "EventManager paints clickable areas");

	foreach(Clickable *, _handlers)
		g->paintRect((*it)->area());
}

void EventManager::toggleDebug() {
	_debug = !_debug;
	debugC(3, kDebugLevelEvents, "EventManager toggled debug mode to %s", _debug ? "on" : "off");

	if (_debug)
		Graphics::instance().push(this);
	else
		Graphics::instance().pop(this);
}

}
