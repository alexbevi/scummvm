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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "twine/console.h"
#include "common/scummsys.h"
#include "twine/twine.h"
#include "twine/gamestate.h"
#include "twine/scene.h"
#include "twine/debug_scene.h"
#include "twine/debug_grid.h"
#include "twine/text.h"

namespace TwinE {

TwinEConsole::TwinEConsole(TwinEEngine *engine) : _engine(engine), GUI::Debugger() {
	registerCmd("give_allitems", WRAP_METHOD(TwinEConsole, doGiveAllItems));
	registerCmd("give_key", WRAP_METHOD(TwinEConsole, doGiveKey));
	registerCmd("change_scene", WRAP_METHOD(TwinEConsole, doChangeScene));
	registerCmd("list_menutext", WRAP_METHOD(TwinEConsole, doListMenuText));
	registerCmd("toggle_debug", WRAP_METHOD(TwinEConsole, doToggleDebug));
	registerCmd("toggle_zones", WRAP_METHOD(TwinEConsole, doToggleZoneRendering));
	registerCmd("toggle_freecamera", WRAP_METHOD(TwinEConsole, doToggleFreeCamera));
	registerCmd("toggle_scenechanges", WRAP_METHOD(TwinEConsole, doToggleSceneChanges));
	registerCmd("scene_actor", WRAP_METHOD(TwinEConsole, doSkipSceneActorsBut));
	registerCmd("hero_pos", WRAP_METHOD(TwinEConsole, doSetHeroPosition));
	registerCmd("set_game_flag", WRAP_METHOD(TwinEConsole, doSetGameFlag));
	registerCmd("show_game_flag", WRAP_METHOD(TwinEConsole, doPrintGameFlag));
	registerCmd("inventory_flag", WRAP_METHOD(TwinEConsole, doSetInventoryFlag));
}

TwinEConsole::~TwinEConsole() {
}

#define TOGGLE_DEBUG(var, description) \
	if ((var)) { \
		debugPrintf("Disabling " description); \
		(var) = false; \
	} else { \
		debugPrintf("Enabling " description); \
		(var) = true; \
	} \
	if ((var) && !_engine->cfgfile.Debug) { \
		doToggleDebug(0, nullptr); \
	}

bool TwinEConsole::doToggleZoneRendering(int argc, const char **argv) {
	TOGGLE_DEBUG(_engine->_debugScene->showingZones, "zone rendering\n")
	return true;
}

bool TwinEConsole::doSkipSceneActorsBut(int argc, const char **argv) {
	if (argc < 2) {
		debugPrintf("Usage: give actor id of scene or -1 to disable\n");
		return false;
	}
	const int16 actorIdx = atoi(argv[1]);
	debugPrintf("Only load actor %d in the next scene\n", actorIdx);
	_engine->_debugScene->onlyLoadActor = actorIdx;
	return true;
}

bool TwinEConsole::doToggleFreeCamera(int argc, const char **argv) {
	TOGGLE_DEBUG(_engine->_debugGrid->useFreeCamera, "free camera movement\n")
	return true;
}

bool TwinEConsole::doToggleSceneChanges(int argc, const char **argv) {
	TOGGLE_DEBUG(_engine->_debugGrid->canChangeScenes, "scene switching via keybinding\n")
	return true;
}

bool TwinEConsole::doSetInventoryFlag(int argc, const char **argv) {
	if (argc <= 1) {
		debugPrintf("Expected to get a inventory flag index as first parameter\n");
		return false;
	}

	const uint8 idx = atoi(argv[1]);
	if (idx >= NUM_INVENTORY_ITEMS) {
		debugPrintf("given index exceeds the max allowed value of %i\n", NUM_INVENTORY_ITEMS - 1);
		return false;
	}
	const uint8 val = argc == 3 ? atoi(argv[2]) : 0;
	_engine->_gameState->inventoryFlags[idx] = val;

	return true;
}

bool TwinEConsole::doSetGameFlag(int argc, const char **argv) {
	if (argc <= 1) {
		debugPrintf("Expected to get a game flag index as first parameter\n");
		return false;
	}

	const uint8 idx = atoi(argv[1]);
	const uint8 val = argc == 3 ? atoi(argv[2]) : 0;
	_engine->_gameState->gameFlags[idx] = val;

	return true;
}

bool TwinEConsole::doPrintGameFlag(int argc, const char **argv) {
	if (argc <= 1) {
		for (int i = 0; i < NUM_GAME_FLAGS; ++i) {
			debugPrintf("[%03d] = %d\n", i, _engine->_gameState->gameFlags[i]);
		}
		return true;
	}

	const uint8 idx = atoi(argv[1]);
	debugPrintf("[%03d] = %d\n", idx, _engine->_gameState->gameFlags[idx]);

	return true;
}

bool TwinEConsole::doGiveKey(int argc, const char **argv) {
	int amount = 1;
	if (argc >= 2) {
		amount = atoi(argv[1]);
	}
	_engine->_gameState->inventoryNumKeys += amount;
	return true;
}

bool TwinEConsole::doToggleDebug(int argc, const char **argv) {
	if (_engine->cfgfile.Debug) {
		debugPrintf("Disabling debug mode\n");
		_engine->cfgfile.Debug = false;
	} else {
		debugPrintf("Enabling debug mode\n");
		_engine->cfgfile.Debug = true;
	}
	return true;
}

bool TwinEConsole::doListMenuText(int argc, const char **argv) {
	_engine->_text->initTextBank(TextBankId::Inventory_Intro_and_Holomap);
	for (int32 i = 0; i < 1000; ++i) {
		char buf[256];
		if (_engine->_text->getMenuText(i, buf, sizeof(buf))) {
			debugPrintf("%4i: %s\n", i, buf);
		}
	}
	return true;
}

bool TwinEConsole::doSetHeroPosition(int argc, const char **argv) {
	if (argc < 4) {
		debugPrintf("Current hero position: %i:%i:%i\n", _engine->_scene->sceneHero->x, _engine->_scene->sceneHero->y, _engine->_scene->sceneHero->z);
		return true;
	}
	const int16 x = atoi(argv[1]);
	const int16 y = atoi(argv[2]);
	const int16 z = atoi(argv[3]);
	_engine->_scene->sceneHero->x = x;
	_engine->_scene->sceneHero->y = y;
	_engine->_scene->sceneHero->z = z;
	return true;
}

bool TwinEConsole::doChangeScene(int argc, const char **argv) {
	if (argc <= 1) {
		debugPrintf("Expected to get a scene index as first parameter\n");
		return false;
	}
	byte newSceneIndex = atoi(argv[1]);
	if (newSceneIndex >= LBA1SceneId::SceneIdMax) {
		debugPrintf("Scene index out of bounds\n");
		return false;
	}
	_engine->_scene->needChangeScene = atoi(argv[1]);
	_engine->_scene->changeScene();
	return true;
}

bool TwinEConsole::doGiveAllItems(int argc, const char **argv) {
	for (int32 i = 0; i < NUM_INVENTORY_ITEMS; ++i) {
		_engine->_gameState->gameFlags[i] = 1;
		_engine->_gameState->inventoryFlags[i] = 1;
	}
	_engine->_gameState->gameFlags[GAMEFLAG_INVENTORY_DISABLED] = 0;
	int amount = 10;
	if (argc >= 2) {
		amount = atoi(argv[1]);
	}
	_engine->_gameState->inventoryNumKeys += amount;
	_engine->_gameState->inventoryNumKashes += amount;
	_engine->_gameState->inventoryNumLeafsBox += amount;
	_engine->_gameState->inventoryNumLeafs += amount;
	_engine->_gameState->inventoryMagicPoints += amount;
	_engine->_gameState->inventoryNumGas += amount;
	return true;
}

} // End of namespace TwinE
