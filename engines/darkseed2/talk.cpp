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

#include "engines/darkseed2/talk.h"
#include "engines/darkseed2/imageconverter.h"
#include "engines/darkseed2/resources.h"
#include "engines/darkseed2/options.h"
#include "engines/darkseed2/sound.h"
#include "engines/darkseed2/font.h"
#include "engines/darkseed2/graphics.h"
#include "engines/darkseed2/graphicalobject.h"
#include "engines/darkseed2/conversationbox.h"

namespace DarkSeed2 {

TalkLine::TalkLine(Resources &resources, const Common::String &talkName) {
	_resources = &resources;

	_resource = talkName;

	_wav = 0;
	_txt = 0;

	_speaker = 0;

	Common::String wavFile = Resources::addExtension(talkName, "WAV");
	Common::String txtFile = Resources::addExtension(talkName, "TXT");

	// Reading the sound
	if (_resources->hasResource(wavFile))
		_wav = _resources->getResource(wavFile);

	// Reading the text
	if (_resources->hasResource(txtFile)) {
		Common::SeekableReadStream *txt = _resources->getResource(txtFile);

		if (_resources->getVersionFormats().getLanguage() == Common::JA_JPN) {
			_txt = new TextLine(*txt);
		} else {
			Common::String str;
			while (!txt->err() && !txt->eos()) {
				Common::String line = txt->readLine();
				if (line.empty() && txt->eos())
					continue;

				if (!str.empty())
					str += '\n';
				str += line;
			}

			_txt = new TextLine(str);
		}

		delete txt;
	}

}

TalkLine::~TalkLine() {
	delete _wav;
	delete _txt;
	delete _speaker;
}

bool TalkLine::hasWAV() const {
	return _wav != 0;
}

bool TalkLine::hasTXT() const {
	return _txt != 0;
}

Common::SeekableReadStream &TalkLine::getWAV() const {
	if (!_wav)
		error("Resource %s.WAV does not exist", _resource.c_str());

	return *_wav;
}

const TextLine &TalkLine::getTXT() const {
	if (!_txt)
		error("Resource %s.TXT does not exist", _resource.c_str());

	return *_txt;
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

const TextLine *TalkLine::getSpeaker() const {
	return _speaker;
}

uint8 TalkLine::getSpeakerNum() const {
	return _speakerNum;
}

void TalkLine::setSpeaker(uint8 speakerNum, const TextLine &speaker) {
	_speakerNum = speakerNum;

	delete _speaker;
	_speaker = new TextLine(speaker);
}


TalkManager::TalkManager(const VersionFormats &versionFormats, Sound &sound,
		Graphics &graphics, const FontManager &fontManager) {

	_versionFormats = &versionFormats;

	_sound    = &sound;
	_graphics = &graphics;

	_fontMan  = &fontManager;

	_curTalk = -1;

	_curTalkLine = 0;

	_txtEnabled = true;

	_waitTextLength =  0;
	_waitTextUntil  = -1;
}

TalkManager::~TalkManager() {
	delete _curTalkLine;
}

bool TalkManager::talkInternal(const TalkLine &talkLine) {
	if (talkLine.hasWAV()) {
		// Sound
		if (!_sound->playSound(talkLine.getWAV(), &_curTalk, Audio::Mixer::kSpeechSoundType)) {
			warning("TalkManager::talk(): WAV playing failed");
			return false;
		}
	} else {
		_sound->playDummySound(_curTalk, 1000, Audio::Mixer::kSpeechSoundType);
	}

	if (_txtEnabled && talkLine.hasTXT()) {
		// Text
		const TextLine &text = talkLine.getTXT();
		const TextLine *speaker = talkLine.getSpeaker();

		TextLine *textLine;
		if (speaker) {
			textLine = new TextLine(*speaker);
			textLine->append(_versionFormats->getSpeakerSeparator());
			textLine->append(text);
		} else
			textLine = new TextLine(text);

		_graphics->getConversationBox().talk(*textLine);

		delete textLine;
	}

	return true;
}

bool TalkManager::talk(const TalkLine &talkLine) {
	endTalk();

	return talkInternal(talkLine);
}

bool TalkManager::talk(Resources &resources, const Common::String &talkName) {
	endTalk();

	_curTalkLine = new TalkLine(resources, talkName);

	return talkInternal(*_curTalkLine);
}

void TalkManager::endTalk() {
	if (_curTalk < -1) {
		_curTalk = -(_curTalk + 2);
	}

	_sound->stopID(_curTalk);
	_sound->signalSpeechEnd(_curTalk);
	_graphics->talkEnd();
	_curTalk = -1;

	delete _curTalkLine;
	_curTalkLine = 0;
}

int TalkManager::getSoundID() const {
	return _curTalk;
}

bool TalkManager::isTalking() const {
	return _curTalk != -1;
}

void TalkManager::syncSettings(const Options &options) {
	_txtEnabled = options.getSubtitlesEnabled();

	if (_txtEnabled) {
		int subSpeed = options.getSubtitleSpeed();

		if (subSpeed >= 255) {
			// Don't wait after the sound has finished playing
			_waitTextLength = 0;
		} else if (subSpeed <= 0) {
			// Wait until aborted by mouse click
			_waitTextLength = -1;
		} else {
			// Wait for 20ms per missing speed step, to a max of 5.08s
			_waitTextLength = (255 - subSpeed) * 20;
		}
	} else
		// Subtitles not enabled, so don't wait
		_waitTextLength = 0;
}

void TalkManager::updateStatus() {
	_sound->updateStatus();

	if (_waitTextUntil >= 0) {
		if (g_system->getMillis() >= ((uint32) _waitTextUntil)) {
			// Waited long enough, end talking

			endTalk();
			_waitTextUntil = -1;
		}
	}

	if (_curTalk < 0)
		return;

	if (!_sound->isIDPlaying(_curTalk)) {
		if (_waitTextLength < 0)
			// Wait til aborted
			_waitTextUntil = -2;
		else if (_waitTextLength > 0)
			// Wait _waitTextLength ms
			_waitTextUntil = g_system->getMillis() + _waitTextLength;
		else
			// End at once
			endTalk();

		_curTalk = -_curTalk - 2;
	}
}

} // End of namespace DarkSeed2
