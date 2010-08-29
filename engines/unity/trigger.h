#ifndef _UNITY_TRIGGER_H
#define _UNITY_TRIGGER_H

#include "object.h"

namespace Unity {

enum {
	TRIGGERTYPE_NORMAL = 0x0,
	TRIGGERTYPE_TIMER = 0x1,
	TRIGGERTYPE_PROXIMITY = 0x2,
	TRIGGERTYPE_UNUSED = 0x3
};

struct Trigger {
	uint32 type;
	uint32 id;
	objectID target;
	bool enabled;
	byte unknown_a, unknown_b;
	uint32 timer_start;

	// timer
	uint32 target_time;

	// proximity
	uint16 dist;
	objectID from, to;
	bool reversed, instant;

	Trigger() : target_time(0) { }
	bool tick(UnityEngine *_vm);

protected:
	bool timerTick(UnityEngine *_vm);
	bool proximityTick(UnityEngine *_vm);
};

} // Unity

#endif
