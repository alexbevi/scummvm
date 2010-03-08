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

#ifndef DARKSEED2_DARKSEED2_H
#define DARKSEED2_DARKSEED2_H

#include "common/system.h"

#include "engines/engine.h"
#include "engines/game.h"

class MidiDriver;

namespace Common {
	class Serializer;
}

namespace DarkSeed2 {

enum {
	kDebugResources    = 1 <<  0,
	kDebugGraphics     = 1 <<  1,
	kDebugMusic        = 1 <<  2,
	kDebugSound        = 1 <<  3,
	kDebugTalk         = 1 <<  4,
	kDebugMovie        = 1 <<  5,
	kDebugScript       = 1 <<  6,
	kDebugRooms        = 1 <<  7,
	kDebugObjects      = 1 <<  8,
	kDebugConversation = 1 <<  9,
	kDebugOpcodes      = 1 << 10,
	kDebugRoomConf     = 1 << 11,
	kDebugGameflow     = 1 << 12
};

struct DS2GameDescription;

class Options;
class Cursors;
class Resources;
class FontManager;
class Sound;
class Music;
class Variables;
class ScriptRegister;
class Graphics;
class TalkManager;
class Mike;
class Movie;
class RoomConfigManager;
class ScriptInterpreter;
class Events;

struct SaveMetaInfo;

class DarkSeed2Engine : public Engine {
public:
	// Subsystems
	Options           *_options;
	Cursors           *_cursors;
	Resources         *_resources;
	FontManager       *_fontMan;
	Sound             *_sound;
	Music             *_music;
	Variables         *_variables;
	ScriptRegister    *_scriptRegister;
	Graphics          *_graphics;
	TalkManager       *_talkMan;
	Mike              *_mike;
	Movie             *_movie;
	RoomConfigManager *_roomConfMan;
	ScriptInterpreter *_inter;
	Events            *_events;

	/** Pause the engine. */
	void pauseGame();

	DarkSeed2Engine(OSystem *syst, const DS2GameDescription *gameDesc);
	virtual ~DarkSeed2Engine();

	void initGame(const DS2GameDescription *gd);

	/** Show the load dialog. */
	bool doLoadDialog();

	bool isWindowsPC    () const;
	bool isSaturn() const;

private:
	struct SaveMeta {
		Common::String description;
		uint32 saveData;
		uint16 saveTime;
		uint32 playTime;
	};

	const DS2GameDescription *_gameDescription;

	MidiDriver *_midiDriver;

	Common::RandomSource *_rnd;

	uint32 _engineStartTime;
	uint32 _playTime;

	/** Clear all subsystems */
	void clearAll();

	// Engine APIs
	virtual Common::Error run();
	virtual bool hasFeature(EngineFeature f) const;
	virtual void pauseEngineIntern(bool pause);
	virtual void syncSoundSettings();

	bool getScreenResolution(int32 &width, int32 &height) const;

	bool init(int32 width, int32 height);
	bool initGraphics(int32 width, int32 height);
	bool initGraphicsSystem();

	const char *getGameId() const;
	Common::Language getLanguage() const;
	Common::Platform getPlatform() const;

	// Saving/Loading
	bool canLoadGameStateCurrently();
	bool canSaveGameStateCurrently();

	Common::Error loadGameState(int slot);
	Common::Error saveGameState(int slot, const char *desc);

	bool saveLoad(Common::Serializer &serializer, SaveMetaInfo &meta);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_DARKSEED2_H
