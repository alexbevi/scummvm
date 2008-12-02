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

#ifndef KOM_H
#define KOM_H

#include "engines/engine.h"
#include "common/fs.h"
#include "common/error.h"

#include "kom/screen.h"
#include "kom/database.h"
#include "kom/actor.h"
#include "kom/panel.h"
#include "kom/input.h"
#include "kom/sound.h"
#include "kom/debugger.h"
#include "kom/game.h"
#include "kom/font.h"

namespace Kom {

class Screen;
class Database;
class ActorManager;
class Debugger;
class Game;
class Font;

enum {
	GAMELOOP_RUNNING = 0,
	GAMELOOP_DEATH,
	GAMELOOP_SELECTION,
	GAMELOOP_QUIT = 4
};

class KomEngine : public Engine {
public:
	KomEngine(OSystem *system);
	~KomEngine();
	
	Common::Error init();
	Common::Error go();

	::GUI::Debugger *getDebugger() { return _debugger; }
	void errorString(const char *buf1, char *buf2);

	Input *input() const { return _input; }
	ActorManager *actorMan() const { return _actorMan; }
	Screen *screen() const { return _screen; }
	Database *database() const { return _database; }
	Panel *panel() const { return _panel; }
	Game *game() const { return _game; }
	Sound *sound() const { return _sound; }

	int gameLoopTimer() { return _gameLoopTimer; }
	void endGame() { _gameLoopState = GAMELOOP_DEATH; }

	void ambientStart(int locId);

	// TODO - original deletes the data. check if there's a benefit
	void ambientStop() { _sound->stopSample(_ambientSample); }

	uint8 _flicLoaded;

	// Samples
	SoundSample _hotspotSample;
	SoundSample _doorsSample;
	SoundSample _clickSample;
	SoundSample _swipeSample;
	SoundSample _cashSample;
	SoundSample _loseItemSample;
	SoundSample _ambientSample;

	// music
	int16 _playingMusicId, _playingMusicVolume;

private:
	void gameLoop();

	Screen *_screen;
	Database *_database;
	ActorManager *_actorMan;
	Input *_input;
	Sound *_sound;
	Panel *_panel;
	Debugger *_debugger;
	Game *_game;
	Font *_font;

	int _gameLoopState;
	int _gameLoopTimer;
};

} // End of namespace Kom

#endif
