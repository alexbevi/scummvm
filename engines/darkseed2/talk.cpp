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

#include "common/stream.h"

#include "engines/darkseed2/talk.h"
#include "engines/darkseed2/resources.h"
#include "engines/darkseed2/sound.h"
#include "engines/darkseed2/graphics.h"
#include "engines/darkseed2/graphicalobject.h"

namespace DarkSeed2 {

TalkLine::TalkLine(const Resources &resources, const Common::String &talkName) {
	_resources = &resources;

	_resource = talkName;

	_wav = 0;

	// Reading the sound
	if (_resources->hasResource(talkName + ".WAV"))
		_wav = _resources->getResource(talkName + ".WAV");

	// Reading the text
	Resource *txt;
	if (_resources->hasResource(talkName + ".TXT")) {
		txt = _resources->getResource(talkName + ".TXT");

		Common::SeekableReadStream &txtStream = txt->getStream();

		while (!txtStream.err() && !txtStream.eos()) {
			Common::String line = txtStream.readLine();
			if (line.empty() && txtStream.eos())
				continue;

			if (!_txt.empty())
				_txt += '\n';
			_txt += line;
		}

		delete txt;
	}
}

TalkLine::~TalkLine() {
	delete _wav;
}

bool TalkLine::hasWAV() const {
	return _wav != 0;
}

bool TalkLine::hasTXT() const {
	return !_txt.empty();
}

const Resource &TalkLine::getWAV() const {
	if (!_wav)
		error("Resource %s.WAV does not exist", _resource.c_str());

	return *_wav;
}

const Common::String &TalkLine::getTXT() const {
	return _txt;
}

const Common::String &TalkLine::getResourceName() const {
	return _resource;
}

const Common::String &TalkLine::getName() const {
	return _name;
}

void TalkLine::setName(const Common::String &name) {
	_name = name;
}

const Common::String &TalkLine::getSpeaker() const {
	return _speaker;
}

void TalkLine::setSpeaker(const Common::String &speaker) {
	_speaker = speaker;
}


TalkManager::TalkManager(Sound &sound, Graphics &graphics) {
	_sound    = &sound;
	_graphics = &graphics;

	_curTalk = -1;

	_curTalkLine = 0;
}

TalkManager::~TalkManager() {
}

bool TalkManager::talkInternal(const TalkLine &talkLine,
		const Common::String &speechVar) {

	if (talkLine.hasWAV()) {
		// Sound
		if (!_sound->playWAV(talkLine.getWAV(), _curTalk, Audio::Mixer::kSpeechSoundType, speechVar)) {
			warning("TalkManager::talk(): WAV playing failed");
			return false;
		}

		debugC(-1, kDebugTalk, "Speaking line \"%s\"", talkLine.getResourceName().c_str());

		// Text
		Common::String text = talkLine.getTXT();
		if (!talkLine.getSpeaker().empty())
			text = talkLine.getSpeaker() + ":\n" + text;

		TextObject *talkObject = new TextObject(text, 5, 0,
				_graphics->getPalette().findWhite(), 300);

		_graphics->talk(talkObject);

	} else {
		warning("TalkManager::talk(): Talk line has no WAV");
		return false;
	}

	return true;
}

bool TalkManager::talk(const TalkLine &talkLine) {
	endTalk();

	return talkInternal(talkLine);
}

bool TalkManager::talk(Resources &resources, const Common::String &talkName,
		const Common::String &speechVar) {

	endTalk();

	_curTalkLine = new TalkLine(resources, talkName);

	return talkInternal(*_curTalkLine, speechVar);
}

void TalkManager::endTalk() {
	_sound->stopID(_curTalk);
	_graphics->talkEnd();
	_curTalk = -1;

	delete _curTalkLine;
}

bool TalkManager::isTalking() const {
	return _curTalk != -1;
}

void TalkManager::updateStatus() {
	_sound->updateStatus();

	if (_curTalk == -1)
		return;

	if (!_sound->isIDPlaying(_curTalk))
		endTalk();
}

} // End of namespace DarkSeed2
