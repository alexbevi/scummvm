#ifndef _UNITY_H
#define _UNITY_H

#include "engines/engine.h"

namespace Unity {

class UnityEngine : public Engine {
public:
	UnityEngine(class OSystem *syst);
	~UnityEngine();

	Common::Error init();
	Common::Error run();
};

} // Unity

#endif

