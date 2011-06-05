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

#include "unity.h"
#include "sprite_player.h"
#include "object.h"
#include "sound.h"

#include "common/system.h"
#include "common/stream.h"
#include "common/textconsole.h"

namespace Unity {

SpritePlayer::SpritePlayer(const char *filename, Object *par, UnityEngine *vm) : _parent(par), _vm(vm) {
	_spriteStream = vm->data.openFile(filename);
	_sprite = new Sprite(_spriteStream);
	_currentEntry = ~0;
	_currentSprite = NULL;
	_currentSpeechSprite = NULL;
	_currentPalette = NULL;
	_waitTarget = 0;

	_normal.xpos = _normal.ypos = 0;
	_normal.xadjust = _normal.yadjust = 0;
	_speech.xpos = _speech.ypos = 0;
	_speech.xadjust = _speech.yadjust = 0;

	_wasSpeech = false;
	_wasMarked = false;
}

SpritePlayer::~SpritePlayer() {
	delete _sprite;
	delete _spriteStream;
}

void SpritePlayer::resetState() {
	_normal.xadjust = _normal.yadjust = 0;
	_speech.xadjust = _speech.yadjust = 0;

	_waitTarget = 0;
}

void SpritePlayer::startAnim(unsigned int a) {
	if (a >= _sprite->getNumAnims()) {
		error("animation %d is too high (only %d animation(s))", a, _sprite->getNumAnims());
	}

	_currentEntry = _sprite->getIndexFor(a);
	assert(_currentEntry != (unsigned int)~0);

	resetState();
}

unsigned int SpritePlayer::getCurrentWidth() {
	assert(_currentSprite);
	return _currentSprite->width;
}

unsigned int SpritePlayer::getCurrentHeight() {
	assert(_currentSprite);
	return _currentSprite->height;
}

byte *SpritePlayer::getCurrentData() {
	assert(_currentSprite);
	return _currentSprite->data;
}

bool SpritePlayer::speaking() {
	return _currentSpeechSprite != NULL;
}

unsigned int SpritePlayer::getSpeechWidth() {
	assert(_currentSpeechSprite);
	return _currentSpeechSprite->width;
}

unsigned int SpritePlayer::getSpeechHeight() {
	assert(_currentSpeechSprite);
	return _currentSpeechSprite->height;
}

byte *SpritePlayer::getSpeechData() {
	assert(_currentSpeechSprite);
	return _currentSpeechSprite->data;
}

byte *SpritePlayer::getPalette() {
	if (!_currentPalette)
		return NULL;

	byte *palette = _currentPalette->palette;
	_currentPalette = NULL;
	return palette;
}

bool SpritePlayer::playing() {
	SpriteEntry *e = _sprite->getEntry(_currentEntry);
	assert(e);
	return !(e->type == se_Pause || e->type == se_Exit);
}

void SpritePlayer::update() {
	unsigned int oldEntry = ~0;
	while (true) {
		SpriteEntry *e = _sprite->getEntry(_currentEntry);
		assert(e);
		switch (e->type) {
		case se_None:
			_currentEntry++;
			break;

		case se_Sprite:
			_currentSprite = (SpriteEntrySprite *)e;
			// XXX: don't understand how this works either
			_wasSpeech = false;
			_currentSpeechSprite = NULL;

			_currentEntry++;

			e = _sprite->getEntry(_currentEntry);
			if (e->type == se_Sprite || e->type == se_SpeechSprite)
				return;
			break;

		case se_SpeechSprite:
			if (!_wasMarked) {
				error("no marked data during sprite playback");
			}

			_currentSpeechSprite = (SpriteEntrySprite *)e;
			// XXX: don't understand how this works :(
			if (!_wasSpeech) {
				_speech = _marked;
			}
			_wasSpeech = true;

			_currentEntry++;
			e = _sprite->getEntry(_currentEntry);
			if (e->type == se_Sprite || e->type == se_SpeechSprite)
				return;
			break;

		case se_Palette:
			_currentPalette = (SpriteEntryPalette *)e;
			_currentEntry++;
			break;

		case se_Position:
			{
				SpriteEntryPosition *p = (SpriteEntryPosition *)e;
				if (_wasSpeech) {
					_speech.xpos = p->newx;
					_speech.ypos = p->newy;
				} else {
					_normal.xpos = p->newx;
					_normal.ypos = p->newy;
				}
			}
			_currentEntry++;
			break;

		case se_RelPos:
			{
				SpriteEntryRelPos *p = (SpriteEntryRelPos *)e;
				if (_wasSpeech) {
					_speech.xadjust = p->adjustx;
					_speech.yadjust = p->adjusty;
				} else {
					_normal.xadjust = p->adjustx;
					_normal.yadjust = p->adjusty;
				}
			}
			_currentEntry++;
			break;

		case se_MouthPos:
			{
				//SpriteEntryMouthPos *p = (SpriteEntryMouthPos *)e;
				// TODO: okay, this isn't MouthPos :-) what is it?
				// set on characters, drones, cable, welds..
			}
			_currentEntry++;
			break;

		case se_Pause:
		case se_Exit:
			return;

		case se_Mark:
			// XXX: do something here
			_wasMarked = true;
			_marked = _normal;
			_currentEntry++;
			break;

		case se_Mask:
			warning("sprite mask mode not implemented");
			_currentEntry++;
			break;

		case se_Static:
			warning("sprite static mode not implemented");
			_currentEntry++;
			break;

		case se_RandomWait:
			if (!_waitTarget) {
				unsigned int wait = _vm->_rnd->getRandomNumberRng(0,
					((SpriteEntryRandomWait *)e)->rand_amt);
				// TODO: is /6 correct? see below
				_waitTarget = g_system->getMillis() + ((SpriteEntryRandomWait *)e)->base/6 + wait/6;
				return;
			}
			// fall through
		case se_Wait:
			if (!_waitTarget) {
				// TODO: is /6 correct? just a guess, so almost certainly not
				// example values are 388, 777, 4777, 288, 666, 3188, 2188, 700, 200000, 1088, 272..
				_waitTarget = g_system->getMillis() + ((SpriteEntryWait *)e)->wait/6;
				return;
			}
			if (_waitTarget < g_system->getMillis()) {
				_waitTarget = 0;
				_currentEntry++;
				continue;
			}
			return;

		case se_Jump:
			_currentEntry = _sprite->getIndexFor(((SpriteEntryJump *)e)->target);
			assert(_currentEntry != (unsigned int)~0);
			if (oldEntry == _currentEntry) {
				// XXX: work out why this happens: missing handling?
				error("sprite file in infinite loop");
			}
			oldEntry = _currentEntry;
			resetState();
			break;

		case se_Audio:
			_vm->_snd->playAudioBuffer(((SpriteEntryAudio *)e)->length,
					((SpriteEntryAudio *)e)->data);
			_currentEntry++;
			break;

		case se_WaitForSound:
			warning("sprite waiting for sound not implemented");
			_currentEntry++;
			break;

		case se_Silent:
			warning("sprite silencing not implemented");
			_currentEntry++;
			break;

		case se_StateSet:
			warning("sprite state setting not implemented");
			_currentEntry++;
			break;

		case se_SetFlag:
			warning("flag setting not implemented");
			_currentEntry++;
			break;

		default:
			error("internal error: bad sprite player entry");
		}
	}
}

} // Unity

