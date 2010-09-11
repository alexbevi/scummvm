#ifndef _UNITY_H
#define _UNITY_H

#include "engines/engine.h"
#include "common/random.h"

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

enum ActionType {
	ACTION_USE = 0,
	ACTION_GET = 1,
	ACTION_LOOK = 2,
	ACTION_TIMER = 3,
	ACTION_WALK = 4,
	ACTION_TALK = 5
};

class UnityEngine : public Engine {
public:
	UnityEngine(class OSystem *syst);
	~UnityEngine();

	Common::Error init();
	Common::Error run();

	Object *objectAt(unsigned int x, unsigned int y);

	Common::RandomSource _rnd;

	UnityData data;

	Sound *_snd;
	Graphics *_gfx;

	bool on_bridge;
	AwayTeamMode mode;

	Object *_current_away_team_member;
	Common::Array<Object *> _away_team_members;

	Common::String status_text;

	bool in_dialog;
	bool dialog_choosing;
	Common::String dialog_text;
	Common::Array<Common::String> choice_list;
	void setSpeaker(objectID s);

	Conversation *current_conversation;

	// TODO: horrible hack
	Common::Array<unsigned int> dialog_choice_responses;
	Common::Array<unsigned int> dialog_choice_states;
	unsigned int next_situation, next_state;

	uint16 beam_world, beam_screen;

	void runDialogChoice();
	void runDialog();

	void startBridge();
	void startAwayTeam(unsigned int world, unsigned int screen);

	void performAction(ActionType action, Object *target, objectID other = objectID(), objectID who = objectID());

	Common::String voiceFileFor(byte voice_group, byte voice_subgroup, objectID speaker, byte voice_id, char type = 0);

protected:
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

