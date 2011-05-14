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

#ifndef _UNITY_H
#define _UNITY_H

#include "engines/engine.h"
#include "common/random.h"

#include "unity/console.h"

#include "data.h"

namespace Unity {

class Graphics;
class Sound;
class SpritePlayer;
class Object;
class Trigger;

enum AwayTeamMode {
	mode_Look,
	mode_Use,
	mode_Walk,
	mode_Talk
};

class UnityEngine : public Engine {
public:
	UnityEngine(class OSystem *syst);
	~UnityEngine();

	Common::Error init();
	Common::Error run();
        GUI::Debugger *getDebugger() { return _console; }

	Object *objectAt(unsigned int x, unsigned int y);

	Common::RandomSource _rnd;

	UnityData data;

	Sound *_snd;
	Graphics *_gfx;

	bool _on_bridge;
	AwayTeamMode _mode;

	Object *_current_away_team_member;
	SpritePlayer *_current_away_team_icon;
	Common::Array<Object *> _away_team_members;
	Common::Array<Object *> _inventory_items;
	Common::Array<SpritePlayer *> _inventory_icons;
	unsigned int _inventory_index;
	void addToInventory(Object *obj);
	void removeFromInventory(Object *obj);

	Common::String _status_text;

	unsigned int _dialog_x, _dialog_y;
	bool _in_dialog;
	bool _dialog_choosing;
	Common::String _dialog_text;
	Common::Array<Common::String> _choice_list;
	void setSpeaker(objectID s);

	// TODO: horrible hack
	unsigned int _dialog_choice_situation;
	Common::Array<unsigned int> _dialog_choice_states;

	Conversation *_next_conversation;
	unsigned int _next_situation;

	uint16 _beam_world, _beam_screen;

	unsigned int _viewscreen_sprite_id;

	unsigned int runDialogChoice(Conversation *conversation);
	void runDialog();

	void startBridge();
	void startAwayTeam(unsigned int world, unsigned int screen, byte entrance = 0);

	ResultType performAction(ActionType action_type, Object *target, objectID who = objectID(), objectID other = objectID(), unsigned int target_x = 0xffff, unsigned int target_y = 0xffff);

	void playDescriptionFor(Object *obj);
	Common::String voiceFileFor(byte voice_group, byte voice_subgroup, objectID speaker, byte voice_id, char type = 0);

protected:
	UnityConsole *_console;

	objectID _speaker;
	SpritePlayer *_icon;

	void openLocation(unsigned int world, unsigned int screen);

	void checkEvents();
	void handleBridgeMouseMove(unsigned int x, unsigned int y);
	void handleAwayTeamMouseMove(unsigned int x, unsigned int y);
	void handleBridgeMouseClick(unsigned int x, unsigned int y);
	void handleAwayTeamMouseClick(unsigned int x, unsigned int y);

	void drawObjects();
	void processTriggers();
	void processTimers();

	void startupScreen();

	void drawDialogFrameAround(unsigned int x, unsigned int y, unsigned int width,
		unsigned int height, bool use_thick_frame, bool with_icon);

	void drawDialogWindow();
	void drawAwayTeamUI();
	void drawBridgeUI();

	void handleLook(Object *obj);
	void handleUse(Object *obj);
	void handleWalk(Object *obj);
	void handleTalk(Object *obj);

	void DebugNextScreen();
};

} // Unity

#endif

