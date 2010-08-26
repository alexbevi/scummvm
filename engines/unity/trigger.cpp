#include "trigger.h"
#include "common/system.h"

namespace Unity {

bool Trigger::tick() {
	if (!enabled) return false;

	switch (type) {
		case TRIGGERTYPE_NORMAL: return true;
		case TRIGGERTYPE_TIMER: return timerTick();
		case TRIGGERTYPE_PROXIMITY: return proximityTick();
		case TRIGGERTYPE_UNUSED: return false;
	}

	abort();
}

bool Trigger::timerTick() {
	bool ticked = false;
	if (target_time && g_system->getMillis() >= target_time) {
		target_time = 0;
		ticked = true;
	}
	if (!target_time) {
		target_time = g_system->getMillis() + (timer_start * 1000);
	}
	return ticked;
}

bool Trigger::proximityTick() {
	// TODO: check distance between 'from' and 'to'..
	// if 'reversed' set, check >=, otherwise check <=

	// the initial run-away trigger (dist 0) is only fired once you entered astrogation(!)
	// this is because 'instant' is set to false; set it to true, and it will fire at once
	return false;
}

} // Unity
