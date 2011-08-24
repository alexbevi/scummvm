#ifndef DGDS_MOVIEPLAYER_H_
#define DGDS_MOVIEPLAYER_H_

#include "common/array.h"

#include "dgds/eventhandler.h"

namespace Dgds {

class MoviePlayer: public EventHandler {
public:
	MoviePlayer();
	virtual ~MoviePlayer();

	bool handleEvent(const DgdsEvent &evt);
};

} /* namespace Dgds */
#endif
