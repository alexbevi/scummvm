#include "unity.h"

#include "common/fs.h"
#include "common/config-manager.h"
#include "engines/util.h"

namespace Unity {

UnityEngine::UnityEngine(OSystem *syst) : Engine(syst) {
	const Common::FSNode gameDataDir(ConfMan.get("path"));
}

UnityEngine::~UnityEngine() {
}

Common::Error UnityEngine::init() {
	return Common::kNoError;
}

Common::Error UnityEngine::run() {
	initGraphics(640, 480, true);

	// TODO

	return Common::kNoError;
}

} // Unity

