#include "innocent/eventmanager.h"

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

}
