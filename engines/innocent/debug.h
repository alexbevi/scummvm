#ifndef INNOCENT_DEBUG_H
#define INNOCENT_DEBUG_H

#include "config.h"
#include "common/scummsys.h"

namespace Innocent {

template <typename T>
class Inspectable {
public:
	virtual ~Inspectable() {}
	virtual const char *operator+() const { return T(*this); }
	virtual operator T() const = 0;
};

template <>
class Inspectable<uint16> {
public:
	virtual ~Inspectable() {}
	virtual const char *operator+() const {
		snprintf(_inspectBuffer, 6, "%d", uint16(*this));
		return _inspectBuffer;
	}
	virtual operator uint16() const = 0;
private:
	mutable char _inspectBuffer[6];
};

template <>
class Inspectable<uint32> {
public:
	virtual ~Inspectable() {}
	virtual const char *operator+() const {
		snprintf(_inspectBuffer, 9, "%d", uint32(*this));
		return _inspectBuffer;
	}
	virtual operator uint32() const = 0;
private:
	mutable char _inspectBuffer[9];
};

}

#endif
