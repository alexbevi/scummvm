#ifndef DGDS_EVENTHANDLER_H_
#define DGDS_EVENTHANDLER_H_

#include "common/events.h"

namespace Dgds {

struct DgdsEvent: public Common::Event {
	DgdsEvent(): Event() {}

};

class EventHandler {
public:
	EventHandler();
	virtual ~EventHandler();

	virtual bool handleEvent(const DgdsEvent &evt) = 0;
};

} /* namespace Dgds */
#endif
