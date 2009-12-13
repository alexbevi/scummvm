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

#include "common/endian.h"
#include "common/md5.h"

#include "base/plugins.h"

#include "common/config-manager.h"

#include "sound/mixer.h"
#include "sound/mididrv.h"

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/options.h"
#include "engines/darkseed2/cursors.h"
#include "engines/darkseed2/resources.h"
#include "engines/darkseed2/graphics.h"
#include "engines/darkseed2/sound.h"
#include "engines/darkseed2/music.h"
#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/talk.h"
#include "engines/darkseed2/mike.h"
#include "engines/darkseed2/movie.h"
#include "engines/darkseed2/roomconfig.h"
#include "engines/darkseed2/inter.h"
#include "engines/darkseed2/events.h"

namespace DarkSeed2 {

DarkSeed2Engine::DarkSeed2Engine(OSystem *syst) : Engine(syst) {
	Common::addDebugChannel(kDebugResources   , "Resources"   , "Resource handling debug level");
	Common::addDebugChannel(kDebugGraphics    , "Graphics"    , "Graphics debug level");
	Common::addDebugChannel(kDebugMusic       , "Music"       , "Music debug level");
	Common::addDebugChannel(kDebugSound       , "Sound"       , "Sound debug level");
	Common::addDebugChannel(kDebugTalk        , "Talk"        , "Talk debug level");
	Common::addDebugChannel(kDebugMovie       , "Movie"       , "Movie debug level");
	Common::addDebugChannel(kDebugScript      , "Script"      , "Script debug level");
	Common::addDebugChannel(kDebugRooms       , "Rooms"       , "Rooms debug level");
	Common::addDebugChannel(kDebugObjects     , "Objects"     , "Objects debug level");
	Common::addDebugChannel(kDebugConversation, "Conversation", "Conversation debug level");
	Common::addDebugChannel(kDebugOpcodes     , "Opcodes"     , "Script functions debug level");
	Common::addDebugChannel(kDebugRoomConf    , "RoomConf"    , "Room config debug level");
	Common::addDebugChannel(kDebugGameflow    , "Gameflow"    , "Gameflow debug level");

	// Setup mixer
	_mixer->setVolumeForSoundType(Audio::Mixer::kMusicSoundType, ConfMan.getInt("music_volume"));

	_options     = 0;
	_cursors     = 0;
	_resources   = 0;
	_sound       = 0;
	_music       = 0;
	_variables   = 0;
	_graphics    = 0;
	_talkMan     = 0;
	_mike        = 0;
	_movie       = 0;
	_roomConfMan = 0;
	_inter       = 0;
	_events      = 0;
}

DarkSeed2Engine::~DarkSeed2Engine() {
	_music->stop();
	_sound->stopAll();

	_mixer->stopAll();

	delete _events;
	delete _inter;
	delete _movie;
	delete _mike;
	delete _talkMan;
	delete _graphics;
	delete _roomConfMan;

	delete _variables;
	delete _music;
	delete _sound;
	delete _resources;
	delete _cursors;
	delete _options;

	delete _midiDriver;
}

Common::Error DarkSeed2Engine::run() {
	if (!init())
		return Common::kUnknownError;

	if (!initGraphics())
		return Common::kUnknownError;

	debug(-1, "Done initializing.");

	if (!_events->setupIntroSequence()) {
		warning("DarkSeed2Engine::run(): Failed setting up the intro sequence");
		return Common::kUnknownError;
	}

	_events->mainLoop();

	return Common::kNoError;
}

void DarkSeed2Engine::pauseEngineIntern(bool pause) {
	_mixer->pauseAll(pause);
}

void DarkSeed2Engine::syncSoundSettings() {
	Engine::syncSoundSettings();

	_options->syncSettings();

	_sound->syncSettings(*_options);
	_music->syncSettings(*_options);
	_talkMan->syncSettings(*_options);
}

bool DarkSeed2Engine::init() {
	MidiDriverType midiDriver = MidiDriver::detectMusicDriver(MDT_MIDI | MDT_ADLIB | MDT_PREFER_MIDI);
	bool native_mt32 = ((midiDriver == MD_MT32) || ConfMan.getBool("native_mt32"));

	_midiDriver = MidiDriver::createMidi(midiDriver);

	if (native_mt32)
		_midiDriver->property(MidiDriver::PROP_CHANNEL_MASK, 0x03FE);

	debug(-1, "Creating subclasses...");

	_options     = new Options();
	_cursors     = new Cursors();
	_variables   = new Variables();
	_resources   = new Resources();
	_sound       = new Sound(*_mixer, *_variables);
	_music       = new Music(*_mixer, *_midiDriver);
	_graphics    = new Graphics(*_resources, *_variables, *_cursors);
	_talkMan     = new TalkManager(*_sound, *_graphics);
	_mike        = new Mike(*_resources, *_variables, *_graphics);
	_movie       = new Movie(*_mixer, *_graphics);

	_roomConfMan = new RoomConfigManager(*this);
	_inter       = new ScriptInterpreter(*this);
	_events      = new Events(*this);

	syncSoundSettings();

	debug(-1, "Indexing resources...");

	if (!_resources->index("gfile.hdr")) {
		warning("DarkSeed2Engine::init(): Couldn't index resources");
		return false;
	}

	debug(-1, "Initializing game variables...");

	if (!_variables->loadFromIDX(*_resources, "GAMEVAR")) {
		warning("DarkSeed2Engine::init(): Couldn't load initial variables values");
		return false;
	}

	if (!_mike->init()) {
		warning("DarkSeed2Engine::init(): Couldn't initialize Mike");
		return false;
	}

	return true;
}

bool DarkSeed2Engine::initGraphics() {
	debug(-1, "Setting up graphics...");

	_graphics->init(*_talkMan, *_roomConfMan, *_movie, *_mike);

	::initGraphics(Graphics::kScreenWidth, Graphics::kScreenHeight, true);
	return true;
}

} // End of namespace DarkSeed2
