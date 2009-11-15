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
#include "common/events.h"

#include "sound/mixer.h"
#include "sound/mididrv.h"

#include "darkseed2/darkseed2.h"
#include "darkseed2/resources.h"
#include "darkseed2/sprite.h"
#include "darkseed2/graphics.h"
#include "darkseed2/sound.h"
#include "darkseed2/music.h"

namespace DarkSeed2 {

DarkSeed2Engine::DarkSeed2Engine(OSystem *syst) : Engine(syst) {
	Common::addDebugChannel(kDebugResources, "Resources", "Resource handling debug level");

	// Setup mixer
	_mixer->setVolumeForSoundType(Audio::Mixer::kMusicSoundType, ConfMan.getInt("music_volume"));

	_resources = 0;
	_graphics  = 0;
	_sound     = 0;
	_music     = 0;
}

DarkSeed2Engine::~DarkSeed2Engine() {
	_mixer->stopAll();

	delete _music;
	delete _sound;
	delete _graphics;
	delete _resources;
}

Common::Error DarkSeed2Engine::run() {
	if (!init())
		return Common::kUnknownError;

	if (!initGraphics())
		return Common::kUnknownError;

	Resource *bmp = _resources->getResource("RM0101.BMP");

	if (bmp) {
		Sprite sprite;

		if (sprite.loadFromBMP(*bmp)) {
			_graphics->setPalette(sprite.getPalette());
			sprite.blitToScreen(0, 0);
		} else
			warning("BMP loading failed");

		delete bmp;
	} else
		warning("No BMP resource");

	Resource *wav = _resources->getResource("DNA006.WAV");

	if (wav) {
		if (!_sound->playWAV(*wav))
			warning("WAV playing failed");
	} else
		warning("No WAV resource");

	Resource *mid = _resources->getResource("sndtrack/mm001gm.mid");

	if (mid) {
		if (!_music->playMID(*mid))
			warning("MID playing failed");
	} else
		warning("No MID resource");

	while (!shouldQuit()) {
		Common::Event event;
		while (g_system->getEventManager()->pollEvent(event)) {
		}
		g_system->delayMillis(50);
	}

	return Common::kNoError;
}

void DarkSeed2Engine::pauseEngineIntern(bool pause) {
	_mixer->pauseAll(pause);
}

void DarkSeed2Engine::syncSoundSettings() {
	Engine::syncSoundSettings();

	_sound->syncSettings();
	_music->syncSettings();
}

bool DarkSeed2Engine::init() {
	int midiDriver = MidiDriver::detectMusicDriver(MDT_MIDI | MDT_ADLIB | MDT_PREFER_MIDI);
	bool native_mt32 = ((midiDriver == MD_MT32) || ConfMan.getBool("native_mt32"));

	_midiDriver = MidiDriver::createMidi(midiDriver);

	if (native_mt32)
		_midiDriver->property(MidiDriver::PROP_CHANNEL_MASK, 0x03FE);

	_resources = new Resources();
	_graphics  = new Graphics();
	_sound     = new Sound(*_mixer);
	_music     = new Music(*_mixer, *_midiDriver);

	if (!_resources->index("gfile.hdr")) {
		warning("Couldn't index resources");
		return false;
	}

	return true;
}

bool DarkSeed2Engine::initGraphics() {
	::initGraphics(640, 480, true);
	return true;
}

} // End of namespace DarkSeed2
