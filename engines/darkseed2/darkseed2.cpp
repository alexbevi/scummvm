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
#include "engines/darkseed2/sprite.h"
#include "engines/darkseed2/graphics.h"
#include "engines/darkseed2/sound.h"
#include "engines/darkseed2/music.h"
#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/datfile.h"
#include "engines/darkseed2/room.h"
#include "engines/darkseed2/talk.h"
#include "engines/darkseed2/conversation.h"
#include "engines/darkseed2/conversationbox.h"
#include "engines/darkseed2/events.h"

namespace DarkSeed2 {

DarkSeed2Engine::DarkSeed2Engine(OSystem *syst) : Engine(syst) {
	Common::addDebugChannel(kDebugResources, "Resources", "Resource handling debug level");

	// Setup mixer
	_mixer->setVolumeForSoundType(Audio::Mixer::kMusicSoundType, ConfMan.getInt("music_volume"));

	_options   = 0;
	_cursors   = 0;
	_resources = 0;
	_sound     = 0;
	_music     = 0;
	_variables = 0;
	_graphics  = 0;
	_talkMan   = 0;
	_events    = 0;
}

DarkSeed2Engine::~DarkSeed2Engine() {
	_music->stop();
	_sound->stopAll();

	_mixer->stopAll();

	delete _events;
	delete _talkMan;
	delete _graphics;

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

	warning("Done initializing");

	Sprite sprite;

	if (sprite.loadFromBMP(*_resources, "RM0101")) {
		_graphics->registerBackground(sprite);
		_graphics->retrace();
	} else
		warning("BMP loading failed");

	if (!_music->playMID(*_resources, "mm001gm"))
		warning("MID playing failed");

	warning("Evaluating condition: %d",
			_variables->evalCondition("!FALSE TRUE TRUE TRUE !FALSE TRUE =TRUE,1 =FALSE,0") == 1);

	_variables->set("Foobar01", 0);
	_variables->set("Foobar02", 1);

	Room room(*_variables);
	if (!room.parse(*_resources, "0806"))
		warning("Failed parsing room 0806");
	else
		warning("Successfully parsed room 0806");

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
}

bool DarkSeed2Engine::init() {
	int midiDriver = MidiDriver::detectMusicDriver(MDT_MIDI | MDT_ADLIB | MDT_PREFER_MIDI);
	bool native_mt32 = ((midiDriver == MD_MT32) || ConfMan.getBool("native_mt32"));

	_midiDriver = MidiDriver::createMidi(midiDriver);

	if (native_mt32)
		_midiDriver->property(MidiDriver::PROP_CHANNEL_MASK, 0x03FE);

	warning("Creating subclasses...");

	_options   = new Options();
	_cursors   = new Cursors();
	_variables = new Variables();
	_resources = new Resources();
	_sound     = new Sound(*_mixer);
	_music     = new Music(*_mixer, *_midiDriver);
	_graphics  = new Graphics(*_resources, *_variables);
	_talkMan   = new TalkManager(*_sound, *_graphics);

	_events    = new Events(*this);

	syncSoundSettings();

	warning("Indexing resources...");

	if (!_resources->index("gfile.hdr")) {
		warning("Couldn't index resources");
		return false;
	}

	warning("Initializing game variables...");

	if (!_variables->loadFromIDX(*_resources, "GAMEVAR")) {
		warning("Couldn't load initial variables values");
		return false;
	}

	return true;
}

bool DarkSeed2Engine::initGraphics() {
	warning("Setting up graphics");

	_graphics->init();

	::initGraphics(_graphics->_screenWidth, _graphics->_screenHeight, true);
	return true;
}

} // End of namespace DarkSeed2
