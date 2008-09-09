#ifndef INNOCENT_EVENTMANAGER_H
#define INNOCENT_EVENTMANAGER_H

#include "common/list.h"
#include "common/rect.h"
#include "common/singleton.h"

#include "innocent/types.h"

namespace Innocent {
//

class EventManager;
class Debugger;

/**
 * Abstract base class of everything clickable. Automatically registers itself with the event manager.
 */
class Clickable {
public:
	Clickable();
	virtual ~Clickable();

	virtual void clicked() = 0;

	virtual Common::Rect area() const = 0;
	virtual byte zIndex() const = 0;

private:
	friend class EventManager;
};

class EventManager : public Common::Singleton<EventManager>, public Paintable {
public:
	EventManager() : _debug(false) {}
	void clicked(Common::Point pos);

	void paint(Graphics *g) const;
	byte zIndex() const { return 0; }

	friend class Debugger;

private:
	void toggleDebug();

	friend class Clickable;
	void push(Clickable *c);
	void pop(Clickable *c);

	Common::List<Clickable *> _handlers;
	bool _debug;
};

}

#endif
