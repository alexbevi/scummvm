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
 */

#ifndef RING_TIMER_H
#define RING_TIMER_H

#include "ring/shared.h"

namespace Ring {

class Timer : public BaseObject, public Common::Serializable {
public:
	Timer() : BaseObject(0), _tickStart(0), _fired(0), _elapseTime(0) {}
	Timer(TimerId id, uint32 elapseTime);
	~Timer();

	void start();
	void incrementFiredCount() { ++_fired; }

	// Accessors
	void setTickStart(uint32 ticks) { _tickStart = ticks; }
	void setFiredCount(uint32 count) { _fired = count; }

	uint32 getTickStart() { return _tickStart; }
	uint32 getFiredCount() { return _fired; }
	uint32 getElapseTime() { return _elapseTime; }

	// Serializable
	void saveLoadWithSerializer(Common::Serializer &s);

private:
	uint32 _tickStart;
	uint32 _fired;
	uint32 _elapseTime;

	static void handler(void *ptr);
	void handle();
};

class TimerHandler : public Common::Serializable {
public:
	TimerHandler();
	~TimerHandler();

	void start(TimerId id, uint32 elapseTime);
	void stop(TimerId id);
	void stopAll();
	bool has(TimerId id);
	void incrementFiredCount(TimerId id);

	// Serializable
	void saveLoadWithSerializer(Common::Serializer &s);

private:
	AssociativeArray<Timer *> _timers;
};

} // End of namespace Ring

#endif // RING_TIMER_H
