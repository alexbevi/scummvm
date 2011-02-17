/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#ifndef RING_TIMER_H
#define RING_TIMER_H

#include "ring/shared.h"

namespace Ring {

class Timer : public BaseId {
public:
	Timer(TimerId id, uint32 elapseTime);
	~Timer();

	void incrementFiredCount() { ++_fired; }
	void setFiredCount(uint32 count) { _fired = count; }
	uint32 getFiredCount() { return _fired; }

private:
	uint32 _tickStart;
	uint32 _fired;
	uint32 _elapseTime;
};

class TimerHandler {
public:
	TimerHandler();
	~TimerHandler();

	void start(TimerId id, uint32 elapseTime);
	void stop(TimerId id);
	void stopAll();
	bool has(TimerId id);
	void incrementFiredCount(TimerId id);

private:
	AssociativeArray<Timer *> _timers;
};

} // End of namespace Ring

#endif // RING_TIMER_H
