#ifndef INNOCENT_DEBUG_H
#define INNOCENT_DEBUG_H

#include "config.h"
#include "common/scummsys.h"

namespace Innocent {

class Inspectable {
public:
	virtual ~Inspectable() {};
	virtual const char *operator+() const = 0;
};

class StaticInspectable : public Inspectable {
public:
	virtual ~StaticInspectable() {}

	virtual const char *operator+() const { return _debugInfo; }

protected:
	/** just to remember to fill the debug info; use macro DEBUG_INFO to confirm */
	virtual void initDebugInfo() = 0;
	#define DEBUG_INFO protected: void initDebugInfo() {}
	char _debugInfo[100];
};

template <typename T>
class NumericInspectable : public Inspectable {
public:
	virtual ~NumericInspectable() {}
	virtual const char *operator+() const {
		snprintf(_debugInfo, 10, "%d", T(*this));
		return _debugInfo;
	}
	virtual operator T() const = 0;
private:
	mutable char _debugInfo[10];
};

enum DebugLevel {
	kDebugLevelScript    = 1,
	kDebugLevelGraphics  = 2,
	kDebugLevelFlow		 = 4,
	kDebugLevelAnimation = 8,
	kDebugLevelValues    = 16,
	kDebugLevelFiles	 = 32
};

}

#endif
