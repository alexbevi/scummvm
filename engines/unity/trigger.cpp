/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "trigger.h"
#include "unity.h"
#include "common/system.h"

namespace Unity {

bool Trigger::tick(UnityEngine *_vm) {
	if (!enabled) return false;

	switch (type) {
		case TRIGGERTYPE_NORMAL: return true;
		case TRIGGERTYPE_TIMER: return timerTick(_vm);
		case TRIGGERTYPE_PROXIMITY: return proximityTick(_vm);
		case TRIGGERTYPE_UNUSED: return false;
	}

	error("bad trigger type %x\n", type);
}

bool Trigger::timerTick(UnityEngine *_vm) {
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

bool Trigger::proximityTick(UnityEngine *_vm) {
	Object *from_obj = _vm->data.getObject(from);
	Object *to_obj = _vm->data.getObject(to);

	int xdiff = from_obj->universe_x - to_obj->universe_x;
	int ydiff = from_obj->universe_y - to_obj->universe_y;
	int zdiff = from_obj->universe_z - to_obj->universe_z;

	unsigned int distsquared = xdiff*xdiff + ydiff*ydiff + zdiff*zdiff;
	unsigned int tocheck = dist*dist;

	// check distance between 'from' and 'to'..
	// if 'reversed' set, check >=, otherwise check <=
	if (reversed) {
		if (distsquared < tocheck)
			return false;
	} else {
		if (distsquared > tocheck)
			return false;
	}

	// TODO: the initial run-away trigger (dist 0) is only fired once you entered astrogation(!)
	// this is because 'instant' is set to false; set it to true, and it will fire at once

	return true;
}

} // Unity
